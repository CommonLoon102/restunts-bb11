#!/bin/sh
# Build in a complete i386 userspace on the NetBSD amd64 CI virtual machine.
set -eu

script_dir=$(cd -- "$(dirname -- "$0")" && pwd)
repo_dir=$(cd -- "$script_dir/../.." && pwd)
netbsd_release=10.2
sets_url="https://cdn.netbsd.org/pub/NetBSD/NetBSD-$netbsd_release/i386/binary/sets"
target=${1:-netbsd-x86-no-sse2}
default_jobs=2
host_ca_bundle=/etc/openssl/certs/ca-certificates.crt

case "$target" in
    netbsd-x86|netbsd-x86-no-sse2) ;;
    *) echo "Expected netbsd-x86 or netbsd-x86-no-sse2; found $target." >&2; exit 1 ;;
esac
case "$(uname -s):$(uname -m):$(uname -r)" in
    NetBSD:amd64:"$netbsd_release"|NetBSD:x86_64:"$netbsd_release") ;;
    *) echo "This helper requires a NetBSD $netbsd_release amd64 host." >&2; exit 1 ;;
esac
if [ "$(id -u)" != 0 ]; then
    echo "This helper requires root to create the i386 chroot." >&2
    exit 1
fi
if [ ! -s "$host_ca_bundle" ]; then
    echo "Install mozilla-rootcerts-openssl on the host before bootstrapping the i386 userspace." >&2
    exit 1
fi
if ! git -c safe.directory="$repo_dir" -C "$repo_dir" diff --quiet HEAD --; then
    echo "Refusing to package modified tracked source; the archive must match its source commit." >&2
    exit 1
fi
source_commit=$(git -c safe.directory="$repo_dir" -C "$repo_dir" rev-parse HEAD)

# The CI image mounts /tmp as tmpfs; keep the full toolchain and build on disk.
work_dir=$(mktemp -d /var/tmp/restunts-netbsd-i386.XXXXXX)
chroot_dir="$work_dir/root"
trap 'rm -rf -- "$work_dir"' 0
trap 'exit 1' 1 2 15
mkdir -p "$work_dir/downloads" "$chroot_dir"

# These hashes are pinned from the official release's SHA512 manifest.
for set_name in base comp etc xbase xcomp; do
    curl --fail --location --retry 3 --connect-timeout 20 \
        "$sets_url/$set_name.tgz" --output "$work_dir/downloads/$set_name.tgz"
done
(cd "$work_dir/downloads" && cksum -c "$script_dir/netbsd-x86-sets.sha512")
for set_name in base comp etc xbase xcomp; do
    tar -xzpf "$work_dir/downloads/$set_name.tgz" -C "$chroot_dir"
done

# COMPAT_NETBSD32 is enabled in the amd64 GENERIC kernel. It rewrites the
# i386 ELF interpreter to this path, including for programs inside a chroot.
ln -s ld.elf_so "$chroot_dir/usr/libexec/ld.elf_so-i386"
cp /etc/resolv.conf "$chroot_dir/etc/resolv.conf"
# Fresh base sets have no trusted roots. Bootstrap OpenSSL's default CA file
# from the prepared host; the certificate package owns the certs subdirectory.
cp "$host_ca_bundle" "$chroot_dir/etc/openssl/cert.pem"
(cd "$chroot_dir/dev" && sh MAKEDEV all)
if [ "$(chroot "$chroot_dir" /usr/bin/uname -m)" != i386 ]; then
    echo "The host cannot execute the NetBSD i386 userspace." >&2
    exit 1
fi

# Clone the verified commit with independent objects, including every tracked
# file. This also supports worktrees without copying host caches or .git links.
git -c safe.directory="$repo_dir" clone --no-local --no-checkout "$repo_dir" "$chroot_dir/workspace"
git -C "$chroot_dir/workspace" checkout --detach "$source_commit"
chroot "$chroot_dir" /usr/bin/env -i \
    PATH=/usr/pkg/sbin:/usr/pkg/bin:/usr/sbin:/usr/bin:/sbin:/bin \
    HOME=/root LC_ALL=C \
    /bin/sh /workspace/tools/scripts/install-bsd-package-dependencies.sh "$target"

# Pass provenance unchanged; avoid leaking the host's amd64 compiler settings.
chroot "$chroot_dir" /usr/bin/env -i \
    PATH=/usr/pkg/sbin:/usr/pkg/bin:/usr/sbin:/usr/bin:/sbin:/bin \
    HOME=/root LC_ALL=C \
    GITHUB_SHA="${GITHUB_SHA:?Missing source commit}" \
    GITHUB_REF="${GITHUB_REF:-}" \
    GITHUB_RUN_ID="${GITHUB_RUN_ID:-}" \
    GITHUB_RUN_ATTEMPT="${GITHUB_RUN_ATTEMPT:-}" \
    RESTUNTS_BUILD_JOBS="${RESTUNTS_BUILD_JOBS:-$default_jobs}" \
    /usr/pkg/bin/bash /workspace/tools/scripts/build-package.sh "$target"
mkdir -p "$repo_dir/dist/packages"
cp -p "$chroot_dir/workspace/dist/packages/"* "$repo_dir/dist/packages/"
