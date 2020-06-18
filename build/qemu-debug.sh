export $(grep -v '^#' .env | xargs -d '\n')

make all
objcopy --only-keep-debug "${OUTPUT_DIR}"/kernel.elf "${OUTPUT_DIR}"/kernel.sym
return 
qemu-system-x86_64 -cpu qemu64 \
  -s -S \
  -cpu qemu64 -bios "${OVMF_PATH}" \
  -drive file="${OUTPUT_DIR}"/uefi.img,if=ide \
  -net none

# target remote :1234
# symbol-file /path/to/kernel.sym
# break _start
