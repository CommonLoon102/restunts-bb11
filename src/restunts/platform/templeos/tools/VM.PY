#!/usr/bin/env python3
"""Compile HolyC in an isolated TempleOS/TinkerOS live-CD QEMU guest.

Requires QEMU, dosfstools (mkfs.fat), and mtools (mcopy). Never attaches an
existing VM disk. Each run builds a disposable FAT32 disk from the input files.
"""

import argparse
import json
from pathlib import Path
import re
import shutil
import socket
import struct
import subprocess
import sys
import time


DEFAULT_WORK = Path("/tmp/restunts-templeos-vm")
SECTOR_BYTES = 512
PARTITION_SECTORS = 2048
DISK_SECTORS = 524288
FAT32_PARTITION_TYPE = 0x0C
PARTITION_TABLE_OFFSET = 446
BOOT_SIGNATURE_OFFSET = 510
DEBUG_PORT = 0xE9
BOOT_SECONDS = 20
PROMPT_SECONDS = 3
KEY_HOLD_MILLISECONDS = 40
KEY_MAX_HOLD_MILLISECONDS = 60000
KEY_SECONDS = 0.08
DIAGNOSTIC_TAIL_LINES = 40
POLL_SECONDS = 0.1
READY_ATTEMPTS = 5
READY_MARKER = "@"
PASS_MARKER = "RESTUNTS_VM_PASS"
FAIL_MARKER = "RESTUNTS_VM_FAIL"


class Qmp:
    def __init__(self, work):
        self.socket = socket.socket(socket.AF_UNIX)
        self.socket.settimeout(10)
        self.socket.connect(str(work / "QMP.SOCK"))
        self.stream = self.socket.makefile("rwb")
        self.stream.readline()
        self.command("qmp_capabilities")

    def close(self):
        self.stream.close()
        self.socket.close()

    def command(self, name, **arguments):
        request = {"execute": name, "arguments": arguments}
        self.stream.write((json.dumps(request) + "\n").encode())
        self.stream.flush()
        while True:
            line = self.stream.readline()
            if not line:
                raise RuntimeError("QEMU disconnected")
            response = json.loads(line)
            if "return" in response:
                return response["return"]
            if "error" in response:
                raise RuntimeError(response["error"])

    def key(self, keys, hold_milliseconds=KEY_HOLD_MILLISECONDS):
        if not 1 <= hold_milliseconds <= KEY_MAX_HOLD_MILLISECONDS:
            raise ValueError("Key hold must be between 1 and 60000 milliseconds")
        self.command("human-monitor-command", **{
            "command-line": "sendkey " + keys + " " + str(hold_milliseconds),
        })
        time.sleep(KEY_SECONDS)

    def text(self, value):
        plain = {
            " ": "spc", "\n": "ret", ";": "semicolon", "/": "slash", ".": "dot",
            ",": "comma", "-": "minus", "=": "equal", "[": "bracket_left",
            "]": "bracket_right", "'": "apostrophe", "\\": "backslash", "`": "grave_accent",
        }
        shifted = {
            ":": "semicolon", '"': "apostrophe", "#": "3", "(": "9", ")": "0",
            "_": "minus", "*": "8", "{": "bracket_left", "}": "bracket_right", "!": "1",
            "<": "comma", ">": "dot", "?": "slash", "+": "equal", "&": "7",
            "|": "backslash", "$": "4", "%": "5", "@": "2", "^": "6", "~": "grave_accent",
        }
        for character in value:
            if character in shifted:
                key = "shift-" + shifted[character]
            elif character.isascii() and character.isupper():
                key = "shift-" + character.lower()
            elif character in plain:
                key = plain[character]
            elif character.isascii() and character.isalnum():
                key = character
            else:
                raise ValueError("Unsupported keyboard character: " + repr(character))
            self.key(key)

    def screenshot(self, work):
        destination = work / "SCREEN.PPM"
        self.command("screendump", filename=str(destination))
        # PNG is convenient in Codex; PPM remains available without Pillow.
        try:
            from PIL import Image
        except ImportError:
            return destination
        destination = work / "SCREEN.PNG"
        Image.open(work / "SCREEN.PPM").save(destination)
        return destination


def guest_wrapper(entry):
    return '''#define RESTUNTS_VM_DEBUG_PORT 0xE9
#define legacy_vm_u8 U8
#define legacy_vm_void U0
legacy_vm_void RestuntsVmLog(legacy_vm_u8 *message)
{
  while(*message) {
    OutU8(RESTUNTS_VM_DEBUG_PORT,*message++);
  }
}
RestuntsVmLog("RESTUNTS_VM_START\\n");
// Preserve compiler diagnostics that would otherwise scroll out of DocSave.
DocMax;
try {
  Cd("C:/PORT");
  ExeFile2("%s");
  RestuntsVmLog("RESTUNTS_VM_PASS\\n");
} catch {
  Fs->catch_except=TRUE;
  RestuntsVmLog("RESTUNTS_VM_FAIL\\n");
}
legacy_vm_u8 *restunts_vm_output=DocSave(DocPut);
RestuntsVmLog(restunts_vm_output);
Free(restunts_vm_output);
RestuntsVmLog("\\nRESTUNTS_VM_DONE\\n");
''' % entry


def compiler_diagnostic_level(diagnostic):
    plain_diagnostic = re.sub(r"\$[^$]*\$", "", diagnostic)
    diagnostic_match = re.search(r"\b(ERROR|WARNING)\s*:", plain_diagnostic, re.IGNORECASE)
    return diagnostic_match.group(1).lower() if diagnostic_match else None


def require_tool(name):
    result = shutil.which(name)
    if not result:
        raise RuntimeError("Required host executable is missing: " + name)
    return result


def make_disk(args, work):
    source = args.source.resolve()
    entry = Path(args.entry)
    if entry.is_absolute() or ".." in entry.parts or not re.fullmatch(r"[A-Za-z0-9_./-]+", args.entry):
        raise ValueError("--entry must be a relative HolyC path inside --source")
    if not (source / entry).is_file():
        raise FileNotFoundError(source / entry)
    local_mcopy = work / "mtools/usr/bin/mcopy"
    mcopy = args.mcopy or shutil.which("mcopy")
    if not mcopy and local_mcopy.is_file():
        mcopy = str(local_mcopy)
    if not mcopy:
        raise RuntimeError("Install mtools, or supply --mcopy /path/to/mcopy")
    image = work / "SHARE.IMG"
    with image.open("wb") as stream:
        stream.truncate(DISK_SECTORS * SECTOR_BYTES)
        mbr = bytearray(SECTOR_BYTES)
        partition = struct.pack(
            "<B3sB3sII", 0, b"\0\2\0", FAT32_PARTITION_TYPE, b"\xff\xff\xff",
            PARTITION_SECTORS, DISK_SECTORS - PARTITION_SECTORS,
        )
        mbr[PARTITION_TABLE_OFFSET:PARTITION_TABLE_OFFSET + len(partition)] = partition
        mbr[BOOT_SIGNATURE_OFFSET:BOOT_SIGNATURE_OFFSET + 2] = b"\x55\xaa"
        stream.seek(0)
        stream.write(mbr)
    subprocess.run([
        require_tool("mkfs.fat"), "-F", "32", "--offset", str(PARTITION_SECTORS), str(image),
    ], check=True, stdout=subprocess.DEVNULL)
    disk_spec = str(image) + "@@" + str(PARTITION_SECTORS * SECTOR_BYTES)
    subprocess.run([mcopy, "-s", "-i", disk_spec, str(source), "::/PORT"], check=True)
    if args.data:
        subprocess.run([mcopy, "-s", "-i", disk_spec, str(args.data.resolve()), "::/DATA"], check=True)
    wrapper = work / "VMRUN.HC"
    wrapper.write_bytes(guest_wrapper(args.entry).replace("\n", "\r\n").encode("ascii"))
    subprocess.run([mcopy, "-i", disk_spec, str(wrapper), "::/VMRUN.HC"], check=True)
    return image


def stop(work):
    if not (work / "QMP.SOCK").exists():
        return
    try:
        qmp = Qmp(work)
        qmp.command("quit")
        qmp.close()
    except (ConnectionRefusedError, FileNotFoundError):
        return
    deadline = time.monotonic() + 5
    while (work / "QEMU.PID").exists() and time.monotonic() < deadline:
        time.sleep(POLL_SECONDS)


def run(args, work):
    if (work / "QMP.SOCK").exists():
        try:
            current = Qmp(work)
        except (ConnectionRefusedError, FileNotFoundError):
            pass
        else:
            current.close()
            raise RuntimeError("A test guest is already running here. Use the stop command first.")
    image = make_disk(args, work)
    command = [
        require_tool("qemu-system-x86_64"), "-machine", "pc", "-accel", "tcg",
        "-cpu", "max", "-m", "1024", "-smp", "2", "-cdrom", str(args.iso.resolve()),
        "-drive", "file=" + str(image) + ",format=raw,if=ide", "-boot", "d",
        "-display", "none", "-monitor", "none",
        "-qmp", "unix:" + str(work / "QMP.SOCK") + ",server=on,wait=off",
        "-debugcon", "file:" + str(work / "DEBUG.LOG"),
        "-global", "isa-debugcon.iobase=" + hex(DEBUG_PORT),
        "-pidfile", str(work / "QEMU.PID"), "-daemonize",
    ]
    if args.record_speaker:
        command.extend([
            "-audiodev", "wav,id=speaker,path=" + str(work / "SPEAKER.WAV"),
            "-machine", "pcspk-audiodev=speaker",
        ])
    subprocess.run(command, check=True)
    print("Booting isolated live-CD guest...", flush=True)
    qmp = None
    try:
        time.sleep(args.boot_seconds)
        qmp = Qmp(work)
        qmp.text("n")  # Decline hard-disk installation.
        time.sleep(PROMPT_SECONDS)
        qmp.text("n")  # Decline the introductory tour.
        time.sleep(PROMPT_SECONDS)
        # TCG can lag behind the keyboard under load. Verify the command prompt
        # consumes a complete expression before sending the drive setup.
        for attempt in range(READY_ATTEMPTS):
            if attempt:
                # Recover a missed installation/tour decline under heavy TCG
                # load. At a ready prompt, this is only an undefined identifier;
                # DocClear below removes that harmless diagnostic.
                qmp.text("n\n")
                time.sleep(PROMPT_SECONDS)
            qmp.text("\nOutU8(0xE9,64);\n")
            time.sleep(PROMPT_SECONDS)
            if READY_MARKER in (work / "DEBUG.LOG").read_text(errors="replace"):
                break
        else:
            print("Screenshot: " + str(qmp.screenshot(work)))
            qmp.close()
            raise RuntimeError("The guest did not reach a usable HolyC command prompt")
        qmp.text("DocClear;\n")
        time.sleep(PROMPT_SECONDS)
        qmp.text(
            "CBlkDev *restunts_vm_bd=BlkDevNextFreeSlot('C',BDT_ATA);"
            "restunts_vm_bd->unit=0;restunts_vm_bd->base0=0x1F0;"
            "restunts_vm_bd->base1=0x3F4;"
            "BlkDevAdd(restunts_vm_bd,I64_MIN,FALSE,TRUE);\n"
        )
        time.sleep(PROMPT_SECONDS)
        qmp.text('#include "C:/VMRUN.HC";\n')
        qmp.close()
        qmp = None
        if args.no_wait:
            print("Guest left running; diagnostics: " + str(work / "DEBUG.LOG"))
            return 0
        deadline = time.monotonic() + args.timeout
        result = 1
        while time.monotonic() < deadline:
            diagnostic = (work / "DEBUG.LOG").read_text(errors="replace")
            if "RESTUNTS_VM_DONE" in diagnostic:
                # HolyC reports some missing #include files without throwing.
                # A returned entry must also compile without errors or warnings.
                diagnostic_level = compiler_diagnostic_level(diagnostic)
                success = PASS_MARKER in diagnostic and FAIL_MARKER not in diagnostic
                result = 0 if success and not diagnostic_level else 1
                if diagnostic_level:
                    print("Guest diagnostics reported: " + diagnostic_level + ".", file=sys.stderr)
                break
            time.sleep(POLL_SECONDS)
        else:
            print("Guest did not finish before the timeout.", file=sys.stderr)
        qmp = Qmp(work)
        print("Screenshot: " + str(qmp.screenshot(work)))
        qmp.close()
        qmp = None
        diagnostic = (work / "DEBUG.LOG").read_text(errors="replace")
        print("Guest result: " + ("PASS" if result == 0 else "FAIL"))
        print("Full diagnostics: " + str(work / "DEBUG.LOG"))
        if args.verbose:
            print(diagnostic)
        elif result:
            plain_diagnostic = re.sub(r"\$[^$]*\$", "", diagnostic)
            print("\n".join(plain_diagnostic.splitlines()[-DIAGNOSTIC_TAIL_LINES:]))
        return result
    finally:
        if qmp is not None:
            qmp.close()
        if not args.keep and not args.no_wait:
            stop(work)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--work", type=Path, default=DEFAULT_WORK)
    commands = parser.add_subparsers(dest="command", required=True)
    runner = commands.add_parser("run")
    runner.add_argument("--iso", type=Path, required=True)
    runner.add_argument("--source", type=Path, required=True)
    runner.add_argument("--entry", required=True)
    runner.add_argument("--data", type=Path)
    runner.add_argument("--mcopy")
    runner.add_argument("--boot-seconds", type=float, default=BOOT_SECONDS)
    runner.add_argument("--timeout", type=float, default=120)
    runner.add_argument("--keep", action="store_true")
    runner.add_argument("--no-wait", action="store_true")
    runner.add_argument("--record-speaker", action="store_true")
    runner.add_argument("--verbose", action="store_true")
    commands.add_parser("stop")
    commands.add_parser("screenshot")
    key_command = commands.add_parser("key")
    key_command.add_argument("keys")
    key_command.add_argument("--hold-ms", type=int, default=KEY_HOLD_MILLISECONDS)
    commands.add_parser("text").add_argument("value")
    args = parser.parse_args()
    work = args.work.resolve()
    work.mkdir(parents=True, exist_ok=True)
    if args.command == "run":
        return run(args, work)
    if args.command == "stop":
        stop(work)
        return 0
    qmp = Qmp(work)
    try:
        if args.command == "screenshot":
            print(qmp.screenshot(work))
        elif args.command == "key":
            qmp.key(args.keys, args.hold_ms)
        elif args.command == "text":
            qmp.text(args.value)
    finally:
        qmp.close()
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (OSError, RuntimeError, ValueError, subprocess.CalledProcessError) as error:
        print("VM error: " + str(error), file=sys.stderr)
        sys.exit(1)
