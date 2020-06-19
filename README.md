# Bread-OS

> BREAD operate system based on X86_64

## Build

### Windows

We only support
[Windows Subsystem for Linux](https://docs.microsoft.com/en-us/windows/wsl/install-win10)
on Windows10.

### Linux

Fow now, we use Linux to compile `*.cpp` files and link binaries.

```shell script
# include deps requirements
apt install cmake make gcc g++ nasm qemu qemu-kvm build-essential git uuid-dev iasl python

# start bread-os using qemu-system-x86_64
make start
```

## Dependencies

Make sure following dependencies have installed.

- [bootboot](https://gitlab.com/bztsrc/bootboot): for micro-kernel loader

- [edk2](https://github.com/tianocore/edk2): development environment for the UEFI

## Troubleshooting

- deps/xxx not exist

```shell script
git submodule update --init --recursive
```

If there have any `socket` problem, make sure you network connection.

- how to build `edk2`

There are many tutorials, for example: [UEFI-EDK2](https://wiki.ubuntu.com/UEFI/EDK2)(for Ubuntu only, but Windows user also could learn how to do)

## LICENSE

[MIT LICENSE](LICENSE)
