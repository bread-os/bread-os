## UEFI files preparation
source .env

dd if=/dev/zero of=fat.img bs=1k count=1440
mformat -i fat.img -f 1440 ::
mmd -i fat.img ::/EFI
mmd -i fat.img ::/EFI/BOOT
mcopy -i fat.img main.efi ::/EFI/BOOT/BOOTX64.EFI
