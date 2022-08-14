#ifndef _KERNEL_SERIAL_H
#define _KERNEL_SERIAL_H

#include <stdint.h>

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8
#define COM5 0x5F8
#define COM6 0x4F8
#define COM7 0x5E8
#define COM8 0x4E8

int serial_init(uint16_t iport);
void serial_putchar(char a);

#endif