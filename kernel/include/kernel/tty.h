#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stdint.h>

void tty_init();
void tty_write(const char *str, uint64_t size);
void tty_putchar(const char c);

#endif