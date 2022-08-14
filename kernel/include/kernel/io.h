#ifndef _KERNEL_IO_H
#define _KERNEL_IO_H

#include <stdint.h>
#include <stdbool.h>

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void io_wait(void);
void halt(void);
void cli(void);
void sti(void);
bool cpu_has_msr();
void cpu_get_msr(uint32_t msr, uint32_t *lo, uint32_t *hi);
void cpu_set_msr(uint32_t msr, uint32_t lo, uint32_t hi);
void panic(const char *str);

#endif