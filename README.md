# Bread-OS

> BREAD operate system based on X86_64

## Requirement

### Windows

We only support
[Windows Subsystem for Linux](https://docs.microsoft.com/en-us/windows/wsl/install-win10)
on Windows10.

### Linux

Fow now, we use Linux to compile `*.cpp` files and link binaries.

```bash
apt install gcc g++ nasm qemu
```

Please **add** them to environment variables

## Usage

```sh
# if you are Windows user, add this line
wsl
# then, we all on the Linux
# make project
make

# run
qemu-system-x86_64 out/kernel.bin
```

## LICENSE

[MIT LICENSE](LICENSE)
