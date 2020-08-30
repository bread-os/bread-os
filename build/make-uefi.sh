## UEFI files preparation
source .env

# generate UEFI bootable dir
mkdir -p ${OUTPUT_DIR}/uefi ${OUTPUT_DIR}/uefi/EFI/BOOT ${OUTPUT_DIR}/uefi/BOOTBOOT ${OUTPUT_DIR}/mnt
# generate kernel dir
mkdir -p ${OUTPUT_DIR}/initrd ${OUTPUT_DIR}/initrd/sys

# todo: use env variables, auto switch 'CMAKE_DEBUG_DIR' and 'CMAKE_RELEASE_DIR'
#  currently we don't use CMAKE_RELEASE_DIR for no release outputs.
# tip: use sys/core as the entry point
cp ${CMAKE_DEBUG_DIR}/kernel.elf ${OUTPUT_DIR}/initrd/sys/core

# zip files inside 'initrd'
cd ${OUTPUT_DIR}/initrd && (find . | cpio -H hpodc -o | gzip >../initrd.bin) && cd ../..
rm -rf ${OUTPUT_DIR}/initrd # remove tmp dir
# cp initrd.bin for bootboot format
cp ${OUTPUT_DIR}/initrd.bin ${OUTPUT_DIR}/uefi/BOOTBOOT/INITRD

# copy bootboot files
cp ${BOOTBOOT_EFI} ${OUTPUT_DIR}/uefi/EFI/BOOT/BOOTX64.EFI
cp ${BOOTBOOT_BIN} ${OUTPUT_DIR}/uefi/BOOTBOOT/LOADER
cp build/config.txt ${OUTPUT_DIR}/uefi/BOOTBOOT/CONFIG  # config file

## Image generate
dd if=/dev/zero of=${OUTPUT_DIR}/uefi.img bs=512 count=93750
parted ${OUTPUT_DIR}/uefi.img -s -a minimal mklabel gpt
parted ${OUTPUT_DIR}/uefi.img -s -a minimal mkpart EFI FAT32 2048s 93716s
parted ${OUTPUT_DIR}/uefi.img -s -a minimal toggle 1 boot
dd if=/dev/zero of=${OUTPUT_DIR}/temp.img bs=512 count=91669
mformat -i ${OUTPUT_DIR}/temp.img -h 32 -t 32 -n 64 -c 1

dd if=${OUTPUT_DIR}/temp.img of=${OUTPUT_DIR}/uefi.img bs=512 count=91669 seek=2048 conv=notrunc
rm ${OUTPUT_DIR}/temp.img

## Copy UEFI files
# tip: I use loop10, you can change it
sudo losetup --offset 1048576 --sizelimit 46934528 /dev/loop10 ${OUTPUT_DIR}/uefi.img
sudo mount /dev/loop10 ${OUTPUT_DIR}/mnt
# copy files
sudo cp -r ${OUTPUT_DIR}/uefi/* ${OUTPUT_DIR}/mnt/
sudo umount ${OUTPUT_DIR}/mnt
sudo losetup -d /dev/loop10
