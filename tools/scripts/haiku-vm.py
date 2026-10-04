#!/usr/bin/env python3
"""Run a disposable Haiku build VM on a Linux host with QEMU and OpenSSH."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import shutil
import socket
import subprocess
import sys
import tempfile
import time
import uuid


RELEASE = "r1beta6"
BUILDER_VERSION = "v2.0.3"
DOWNLOAD_URL = f"https://github.com/anyvm-org/haiku-builder/releases/download/{BUILDER_VERSION}"
IMAGE_NAME = f"haiku-{RELEASE}.qcow2.zst"
KEY_NAME = f"haiku-{RELEASE}-host.id_rsa"
SSH_USER = "user"
X86_IMAGE_NAME = f"haiku-{RELEASE}-x86_gcc2h-anyboot.iso"
X86_IMAGE_URL = f"https://haiku-release.cdn.haiku-os.org/{RELEASE}/{X86_IMAGE_NAME}"
X86_IMAGE_SHA256 = "5a55ad9429f1de9fb5ca5cbcd54de3733ff247cc7919ce5d690fb1271cb1dc76"
X86_DISK_NAME = f"haiku-{RELEASE}-x86.qcow2"
X86_DISK_BYTES = 8 * 1024 * 1024 * 1024
X86_SOURCE_DEVICE = "/dev/disk/ata/0/slave/0"
X86_TARGET_DEVICE = "/dev/disk/ata/1/master/raw"
# Asset digests published by the image builder's GitHub release API.
CHECKSUMS = {
    IMAGE_NAME: "df8be357c5d3795da80dbec0b56d1d4e50f182b2ddf5e272f657ec4b54a9b792",
    KEY_NAME: "56d8a666bb7a77fbdf493893a82fde3d17e88e0d99ea02071005e37c796d86e0",
}
DEFAULT_PORT = 2223
DEFAULT_MEMORY_MB = 2048
DEFAULT_CPUS = 2
SOFTWARE_CPU = "qemu64"
BOOT_TIMEOUT_SECONDS = 180
PREPARE_TIMEOUT_SECONDS = 20 * 60
STOP_TIMEOUT_SECONDS = 30
SSH_TIMEOUT_SECONDS = 15
POLL_SECONDS = 2
DOWNLOAD_RETRIES = 3
DOWNLOAD_TIMEOUT_SECONDS = 20
HASH_BLOCK_BYTES = 1024 * 1024
PRIVATE_MODE = 0o600
DIRECTORY_MODE = 0o700


def command(arguments, **kwargs):
    return subprocess.run([str(argument) for argument in arguments], check=True, **kwargs)


def download(directory, name, url=None, expected=None):
    expected = expected or CHECKSUMS[name]
    destination = directory / name
    if not destination.is_file():
        partial = destination.with_suffix(destination.suffix + ".partial")
        command(["curl", "--fail", "--location", "--retry", DOWNLOAD_RETRIES,
                 "--connect-timeout", DOWNLOAD_TIMEOUT_SECONDS,
                 url or f"{DOWNLOAD_URL}/{name}", "--output", partial])
        verify_digest(partial, expected)
        partial.replace(destination)
    verify_digest(destination, expected)
    return destination


def verify_digest(path, expected):
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for block in iter(lambda: source.read(HASH_BLOCK_BYTES), b""):
            digest.update(block)
    if digest.hexdigest() != expected:
        raise ValueError(f"SHA-256 mismatch: {path}")


def qmp(directory, request):
    """Address only the VM belonging to this directory, without trusting a PID."""
    with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as connection:
        connection.settimeout(SSH_TIMEOUT_SECONDS)
        connection.connect(str(directory / "qmp.sock"))
        with connection.makefile("rwb") as stream:
            json.loads(stream.readline())
            for operation in ("qmp_capabilities", request):
                stream.write(json.dumps({"execute": operation}).encode() + b"\n")
                stream.flush()
                while True:
                    response = json.loads(stream.readline())
                    if "error" in response:
                        raise ValueError(f"QEMU: {response['error']}")
                    if "return" in response:
                        break
            return response["return"]


def is_running(directory):
    try:
        qmp(directory, "query-status")
        return True
    except (OSError, ValueError):
        return False


def ssh_options(directory):
    config = json.loads((directory / "connection.json").read_text())
    identity = (["-o", f"HostKeyAlias=restunts-haiku-{config['arch']}"]
                if "arch" in config else [])
    return ["-i", str(directory / KEY_NAME), "-o", "BatchMode=yes",
            "-o", "IdentitiesOnly=yes", "-o", "ForwardAgent=no",
            "-o", "StrictHostKeyChecking=accept-new", *identity,
            "-o", f"UserKnownHostsFile={directory / 'known_hosts'}",
            "-o", f"ConnectTimeout={config.get('ssh_timeout', SSH_TIMEOUT_SECONDS)}",
            "-o", f"Port={config['port']}"]


def ssh(directory, arguments, **kwargs):
    return command(["ssh", *ssh_options(directory), f"{SSH_USER}@127.0.0.1",
                    shlex.join(arguments)], **kwargs)


def stop(directory):
    try:
        ssh(directory, ["sync"], timeout=STOP_TIMEOUT_SECONDS)
        ssh(directory, ["shutdown", "-q"], timeout=STOP_TIMEOUT_SECONDS)
    except (OSError, subprocess.CalledProcessError, subprocess.TimeoutExpired):
        # The guest often closes SSH while shutting down.
        pass
    deadline = time.monotonic() + STOP_TIMEOUT_SECONDS
    while is_running(directory) and time.monotonic() < deadline:
        time.sleep(POLL_SECONDS)
    if is_running(directory):
        # Address our QMP socket, never a potentially recycled process ID.
        qmp(directory, "quit")
        deadline = time.monotonic() + STOP_TIMEOUT_SECONDS
        while is_running(directory):
            if time.monotonic() >= deadline:
                raise ValueError(f"The VM in {directory} did not stop after QMP quit.")
            time.sleep(POLL_SECONDS)


def start(args, extra_drives=()):
    directory = args.directory
    if is_running(directory):
        raise ValueError(f"The VM in {directory} is already running.")
    for executable in ("curl", "zstd", "qemu-img", "qemu-system-x86_64", "ssh", "scp"):
        if not shutil.which(executable):
            raise ValueError(f"Missing host tool: {executable}")
    directory.mkdir(parents=True, exist_ok=True, mode=DIRECTORY_MODE)
    key = download(directory, KEY_NAME)
    key.chmod(PRIVATE_MODE)
    if args.arch == "x86":
        base = directory / X86_DISK_NAME
        if not base.is_file():
            raise ValueError("The x86 disk is missing; run prepare-x86 first.")
    else:
        archive = download(directory, IMAGE_NAME)
        base = directory / f"haiku-{RELEASE}.qcow2"
        if not base.is_file():
            temporary = base.with_suffix(".partial")
            command(["zstd", "--decompress", "--force", archive, "-o", temporary])
            temporary.replace(base)
    disk = directory / f"work-{args.arch}.qcow2"
    if not disk.is_file():
        command(["qemu-img", "create", "-f", "qcow2", "-F", "qcow2", "-b", base, disk])
    (directory / "connection.json").write_text(
        json.dumps({"port": args.port, "arch": args.arch,
                    "ssh_timeout": args.ssh_timeout}) + "\n")
    kvm_available = os.access("/dev/kvm", os.R_OK | os.W_OK)
    if args.accel == "kvm" and not kvm_available:
        raise ValueError("KVM is not accessible; use --accel tcg for software emulation.")
    kvm = args.accel == "kvm" or (args.accel == "auto" and kvm_available)
    accelerator = (["-accel", "kvm", "-cpu", "host"] if kvm else
                   ["-accel", "tcg", "-cpu", SOFTWARE_CPU])
    display = "none" if args.vnc_display is None else f"vnc=127.0.0.1:{args.vnc_display}"
    qemu_arguments = [
        "qemu-system-x86_64", "-name", "restunts-haiku", "-daemonize",
        "-pidfile", directory / "qemu.pid", "-qmp", f"unix:{directory}/qmp.sock,server=on,wait=off",
        "-machine", "pc,hpet=off,smm=off,vmport=off,usb=on", *accelerator,
        "-m", args.memory, "-smp", args.cpus, "-rtc", "base=localtime,clock=host,driftfix=slew",
        "-netdev", "user,id=net0,net=192.168.122.0/24,host=192.168.122.1,"
        f"dhcpstart=192.168.122.254,ipv6=off,hostfwd=tcp:127.0.0.1:{args.port}-192.168.122.254:22",
        "-device", "e1000,netdev=net0", "-drive", f"file={disk},format=qcow2,if=ide,index=0",
        "-vga", "std", "-display", display, "-device", "usb-tablet",
        "-audiodev", "none,id=audio0", "-device", "intel-hda",
        "-device", "hda-duplex,audiodev=audio0",
        "-serial", f"file:{directory}/serial.log",
    ]
    for drive in extra_drives:
        qemu_arguments.extend(["-drive", drive])
    try:
        command(qemu_arguments)
        deadline = time.monotonic() + args.boot_timeout
        while time.monotonic() < deadline:
            try:
                ssh(directory, ["uname", "-a"], timeout=args.ssh_timeout,
                    stderr=subprocess.DEVNULL)
                print(f"Ready: {directory} (SSH port {args.port})")
                return
            except (subprocess.CalledProcessError, subprocess.TimeoutExpired):
                pass
            if not is_running(directory):
                raise ValueError("Haiku VM exited during boot; inspect serial.log.")
            time.sleep(POLL_SECONDS)
        raise ValueError(f"Haiku SSH did not become ready; inspect {directory / 'serial.log'}.")
    except BaseException:
        if is_running(directory):
            qmp(directory, "quit")
        raise


def prepare_x86(args):
    """Use the x64 guest's BFS tools instead of an interactive 32-bit installer."""
    directory = args.directory
    directory.mkdir(parents=True, exist_ok=True, mode=DIRECTORY_MODE)
    destination = directory / X86_DISK_NAME
    if destination.exists():
        raise ValueError(f"Prepared disk already exists: {destination}")
    source = download(directory, X86_IMAGE_NAME, X86_IMAGE_URL, X86_IMAGE_SHA256)
    bootstrap = directory / "bootstrap-x64"
    bootstrap.mkdir(exist_ok=True, mode=DIRECTORY_MODE)
    if is_running(bootstrap):
        raise ValueError(f"The bootstrap VM in {bootstrap} is already running.")
    remote = f"/boot/home/restunts-vm-bootstrap-{uuid.uuid4().hex}"
    key = download(directory, KEY_NAME)
    key.chmod(PRIVATE_MODE)
    archive = download(directory, IMAGE_NAME)
    for asset in (key, archive):
        cached = bootstrap / asset.name
        if not cached.exists():
            cached.symlink_to(asset)
    public_key = directory / "authorized_key.pub"
    with public_key.open("wb") as output:
        command(["ssh-keygen", "-y", "-f", key], stdout=output)
    script = Path(__file__).with_name("prepare-haiku-x86-vm.sh")
    # A BFS volume backed by a file on the guest's own BFS disk can deadlock
    # during package copying. Give Haiku separate source and target IDE disks.
    with tempfile.NamedTemporaryFile(dir=directory, suffix=".raw", delete=False) as output:
        raw = Path(output.name)
        output.truncate(X86_DISK_BYTES)
    try:
        start(argparse.Namespace(directory=bootstrap, arch="x64", port=args.port,
                                 memory=DEFAULT_MEMORY_MB, cpus=args.cpus,
                                 vnc_display=None, accel=args.accel,
                                 boot_timeout=args.boot_timeout,
                                 ssh_timeout=args.ssh_timeout), extra_drives=[
            f"file={source},format=raw,if=ide,index=1,snapshot=on",
            f"file={raw},format=raw,if=ide,index=2",
        ])
        ssh(bootstrap, ["mkdir", "-p", remote])
        command(["scp", "-O", *ssh_options(bootstrap), public_key, script,
                 f"{SSH_USER}@127.0.0.1:{remote}/"])
        ssh(bootstrap, ["sh", f"{remote}/{script.name}", X86_SOURCE_DEVICE,
                        X86_TARGET_DEVICE, f"{remote}/{public_key.name}"],
            timeout=PREPARE_TIMEOUT_SECONDS)
        stop(bootstrap)
        partial = destination.with_suffix(".partial")
        command(["qemu-img", "convert", "-f", "raw", "-O", "qcow2", raw, partial])
        partial.replace(destination)
        print(f"Prepared {destination}")
    finally:
        if is_running(bootstrap):
            stop(bootstrap)
        raw.unlink(missing_ok=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--directory", type=Path,
                        default=Path(tempfile.gettempdir()) / "restunts-haiku-vm",
                        help="VM cache and writable overlay (default: %(default)s)")
    subparsers = parser.add_subparsers(dest="operation", required=True)
    boot = subparsers.add_parser("start", help="Download, verify and boot the VM")
    boot.add_argument("--arch", choices=("x64", "x86"), default="x64")
    boot.add_argument("--port", type=int, default=DEFAULT_PORT)
    boot.add_argument("--memory", type=int, default=DEFAULT_MEMORY_MB, help="RAM in MiB")
    boot.add_argument("--vnc-display", type=int, help="Optional localhost VNC display number")
    prepare = subparsers.add_parser("prepare-x86", help="Build an x86 disk from official install media")
    prepare.add_argument("--port", type=int, default=DEFAULT_PORT,
                         help="SSH port for the temporary x64 bootstrap VM")
    for operation in (boot, prepare):
        operation.add_argument("--cpus", type=int, default=DEFAULT_CPUS)
        operation.add_argument("--accel", choices=("auto", "kvm", "tcg"), default="auto",
                               help="QEMU accelerator; auto uses KVM when accessible")
        operation.add_argument("--boot-timeout", type=int, default=BOOT_TIMEOUT_SECONDS,
                               help="Seconds to wait for guest SSH (default: %(default)s)")
        operation.add_argument("--ssh-timeout", type=int, default=SSH_TIMEOUT_SECONDS,
                               help="Seconds per SSH connection attempt (default: %(default)s)")
    subparsers.add_parser("stop", help="Power off only this VM; retain its overlay")
    remote = subparsers.add_parser("ssh", help="Run a command inside the VM")
    remote.add_argument("command", nargs=argparse.REMAINDER)
    for operation in ("upload", "download"):
        copy = subparsers.add_parser(operation, help="Recursively copy files or directories")
        copy.add_argument("source")
        copy.add_argument("destination")
    args = parser.parse_args()
    args.directory = args.directory.resolve()
    if args.operation == "start":
        start(args)
        return
    if args.operation == "prepare-x86":
        prepare_x86(args)
        return
    if not is_running(args.directory):
        raise ValueError(f"No running VM in {args.directory}; run start first.")
    if args.operation == "stop":
        stop(args.directory)
    elif args.operation == "ssh":
        if not args.command:
            parser.error("ssh requires a command")
        ssh(args.directory, args.command)
    else:
        source, destination = args.source, args.destination
        # Use legacy SCP: the base Haiku image does not promise an SFTP subsystem.
        # Quote guest paths because the remote SCP process is started through a shell.
        if args.operation == "upload":
            source = str(Path(source).resolve())
            destination = f"{SSH_USER}@127.0.0.1:" + shlex.quote(destination)
        else:
            source = f"{SSH_USER}@127.0.0.1:" + shlex.quote(source)
            destination = str(Path(destination).resolve())
        command(["scp", "-O", "-r", *ssh_options(args.directory), source, destination])


if __name__ == "__main__":
    try:
        main()
    except (OSError, ValueError, subprocess.CalledProcessError, subprocess.TimeoutExpired) as error:
        print(f"haiku-vm: {error}", file=sys.stderr)
        sys.exit(1)
