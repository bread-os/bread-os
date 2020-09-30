## UEFI files preparation
source .env

dd if=/dev/zero of="${OUTPUT_DIR}"/fat.img bs=1k count=1440
mformat -i "${OUTPUT_DIR}"/fat.img -f 1440 ::
mmd -i "${OUTPUT_DIR}"/fat.img ::/EFI
mmd -i "${OUTPUT_DIR}"/fat.img ::/EFI/BOOT
mcopy -i "${OUTPUT_DIR}"/fat.img "${OUTPUT_DIR}"/BOOTX64.EFI ::/EFI/BOOT
