#include <stdint.h>
#include <stdbool.h>
#include <cpuid.h>

#include <kernel/io.h>
#include <kernel/cpuid.h>

void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

void io_wait(void) {
    outb(0x80, 0);
}

__attribute__((noreturn))
void halt(void) {
    for (;;) {
        __asm__ volatile("cli; hlt");
    }
}

void cli(void) {
    __asm__("cli");
}

void sti(void) {
    __asm__("sti");
}

bool cpu_has_msr() {
    uint32_t eax, edx, unused;
    __get_cpuid(1, &unused, &unused, &unused, &edx);
    return edx & CPUID_FEAT_EDX_MSR;
}

void cpu_get_msr(uint32_t msr, uint32_t *lo, uint32_t *hi) {
   __asm__ volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}
 
void cpu_set_msr(uint32_t msr, uint32_t lo, uint32_t hi) {
   __asm__ volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}