# Bread-OS

> BREAD operate system based on X86_64

## Requirement

- [`node.js`](https://nodejs.org)(using [nvm](https://github.com/nvm-sh/nvm))

### Windows

- [Windows Subsystem for Linux](https://docs.microsoft.com/en-us/windows/wsl/install-win10)

- [`nasm`](https://www.nasm.us/)

- [`cmake`](https://cmake.org/)

### Linux

Fow now, we use Linux to compile `*.cpp` files and link binaries.

```bash
apt install gcc g++
```

Please **add** them to environment variables

## Usage

```sh
sh build.bash --help
# run
qemu-system-x86_64 out/kernel.bin
```

## LICENSE

[MIT LICENSE](LICENSE)
