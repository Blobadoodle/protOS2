#!/bin/bash

set -e

. ./iso.sh

qemu-system-x86_64 -cdrom image.iso -serial file:serial.log -no-reboot -d int,cpu_reset "$@"

make clean