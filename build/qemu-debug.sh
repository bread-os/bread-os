export $(grep -v '^#' .env | xargs -d '\n')

make uefi

qemu-system-x86_64 -cpu qemu64 \
  -s -S \
  -cpu qemu64 -bios "${OVMF_PATH}" \
  -drive file="${OUTPUT_DIR}"/uefi.img,if=ide \
  -net none
