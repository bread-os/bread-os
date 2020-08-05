## UEFI files preparation
# generate UEFI bootable dir
mkdir -p out/uefi
mkdir -p out/uefi/EFI/BOOT
mkdir -p out/uefi/BOOTBOOT
mkdir -p out/mnt

# make kernel
mkdir -p out/initrd out/initrd/sys

# fixme: use env variables
cp cmake-build-debug/kernel.elf out/initrd/sys/core # tip: use sys/core as the entry point
cd out/initrd && (find . | cpio -H hpodc -o | gzip >../initrd.bin) && cd ../..
rm -rf out/initrd # remove tmp dir
cp out/initrd.bin out/uefi/BOOTBOOT/INITRD

# copy bootboot files
cp deps/bootboot/dist/bootboot.efi out/uefi/EFI/BOOT/BOOTX64.EFI
cp deps/bootboot/dist/bootboot.bin out/uefi/BOOTBOOT/LOADER
cp build/config.txt out/uefi/BOOTBOOT/CONFIG  # config file

## Image generate
dd if=/dev/zero of=out/uefi.img bs=512 count=93750
parted out/uefi.img -s -a minimal mklabel gpt
parted out/uefi.img -s -a minimal mkpart EFI FAT32 2048s 93716s
parted out/uefi.img -s -a minimal toggle 1 boot
dd if=/dev/zero of=out/temp.img bs=512 count=91669
mformat -i out/temp.img -h 32 -t 32 -n 64 -c 1

dd if=out/temp.img of=out/uefi.img bs=512 count=91669 seek=2048 conv=notrunc
rm out/temp.img

## Copy UEFI files
# tip: I use loop10, you can change it
sudo losetup --offset 1048576 --sizelimit 46934528 /dev/loop10 out/uefi.img
sudo mount /dev/loop10 out/mnt
# copy files
sudo cp -r out/uefi/* out/mnt/
sudo umount out/mnt
sudo losetup -d /dev/loop10
