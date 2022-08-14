#!/bin/bash

set -e

make

cp -v kernel.elf limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-cd-efi.bin isodir/boot

xorriso -as mkisofs -b boot/limine-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot boot/limine-cd-efi.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        isodir -o image.iso

./limine/limine-deploy image.iso