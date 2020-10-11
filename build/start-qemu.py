import subprocess as sp
import sys
import platform

ARCH = 'x64'
FW_BASE = "OVMF"
QEMU_ARCH = 'x86_64'

FV = "{base}_{arch}.fd".format(base=FW_BASE, arch=ARCH.upper())
IS_WSL = "microsoft" in platform.uname().release.lower()
QEMU = 'qemu-system-{arch}{suffix}'.format(arch=QEMU_ARCH, suffix=(".exe" if IS_WSL else ""))


def main(argv):
    """Run the qemu"""
    sp.run([QEMU, "-bios", "./{}".format(FV), "-net", "none", "-hda", "fat:rw:image"])


if __name__ == '__main__':
    sys.exit(main(sys.argv))
