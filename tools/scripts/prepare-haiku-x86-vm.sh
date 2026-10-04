#!/bin/sh
# Assemble a 32-bit boot disk inside Haiku, whose BFS driver supports writes.
set -eu

expected_source_device=/dev/disk/ata/0/slave/0
expected_target_device=/dev/disk/ata/1/master/raw
ssh_directory_mode=700
ssh_key_mode=600
source_device=${1:?Usage: prepare-haiku-x86-vm.sh SOURCE_PARTITION TARGET_DEVICE PUBLIC_KEY}
target_device=${2:?Missing target disk device}
public_key=${3:?Missing SSH public key}
source_mount=$(mktemp -d /tmp/restunts-haiku-source.XXXXXX)
target_mount=$(mktemp -d /tmp/restunts-haiku-target.XXXXXX)

cleanup() {
    unmount "$target_mount" 2>/dev/null || true
    unmount "$source_mount" 2>/dev/null || true
    rmdir "$target_mount" "$source_mount"
}
trap cleanup EXIT
trap 'exit 1' HUP INT TERM

if [ "$(uname -s)" != Haiku ]; then
    echo "Preparing the image requires Haiku's writable BFS driver." >&2
    exit 1
fi
# Require the dedicated secondary QEMU disks; never format the boot disk.
if [ "$source_device" != "$expected_source_device" ] || \
    [ "$target_device" != "$expected_target_device" ]; then
    echo "Expected the read-only installer disk and dedicated secondary target disk." >&2
    exit 1
fi
mount -ro -t bfs "$source_device" "$source_mount"
mkfs -q -t bfs "$target_device" restunts-haiku-x86
mount -t bfs "$target_device" "$target_mount"

# A direct BFS mount exposes the physical files, without packagefs overlays.
# Preserve the image's boot loader, package files, symlinks and BFS attributes.
cp -a "$source_mount/." "$target_mount/"
mkdir -p "$target_mount/system/cache/tmp" "$target_mount/system/packages/administrative"
printf '%s\n' 'First boot requested by Restunts VM preparation.' \
    > "$target_mount/system/packages/administrative/FirstBootProcessingNeeded"

# The first-login target starts the desktop only when Locale settings exists.
cp /boot/home/config/settings/Locale\ settings "$target_mount/home/config/settings/Locale settings"
ssh_directory="$target_mount/home/config/settings/ssh"
mkdir -p "$ssh_directory" "$target_mount/home/config/settings/boot"
cp "$public_key" "$ssh_directory/authorized_keys"
chmod "$ssh_directory_mode" "$ssh_directory"
chmod "$ssh_key_mode" "$ssh_directory/authorized_keys"
cat > "$target_mount/home/config/settings/boot/UserBootscript" <<'BOOT'
#!/bin/sh
ssh-keygen -A
/bin/sshd
BOOT
makebootable "$target_mount"
sync
