#include <stdio.h>
#include <cpuid.h>
#include <stdbool.h>
#include <stddef.h>

#include <kernel/io.h>
#include <kernel/apic.h>
#include <kernel/acpi.h>
#include <kernel/cpuid.h>

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800

uint64_t lapic;

void lapic_write_reg(uint16_t reg, uint32_t value) {
    *(uint32_t volatile*)(lapic + reg) = value;
}

uint32_t lapic_read_reg(uint16_t reg) {
    return *(volatile uint32_t*)lapic + reg;
}

bool check_apic() {
    uint32_t eax, unused, edx;
    __get_cpuid(1, &eax, &unused, &unused, &edx);
    return edx & CPUID_FEAT_EDX_APIC;
}

void set_apic_base(uint64_t apic) {
    uint32_t edx = 0;
    uint32_t eax = (apic & 0xfffff0000) | IA32_APIC_BASE_MSR_ENABLE;

#ifdef __PHYSICAL_MEMORY_EXTENSION__
    edx = (apic >> 32) & 0x0f;
#endif

    cpu_set_msr(IA32_APIC_BASE_MSR, eax, edx);
}

uint64_t get_apic_base() {
    uint32_t eax, edx;
    cpu_get_msr(IA32_APIC_BASE_MSR, &eax, &edx);

#ifdef __PHYSICAL_MEMORY_EXTENSION__
   return (eax & 0xfffff000) | ((edx & 0x0f) << 32);
#else
   return (eax & 0xfffff000);
#endif
}

void apic_init() {
    if(!check_apic()) {
        panic("No APIC Found");
    }

    outb(0xa1, 0xff); // Disable pic
    outb(0x21, 0xff);

    madt_t *madt = findTable("APIC");

    lapic = madt->lapic_address;

    set_apic_base(get_apic_base());

    lapic_write_reg(0xF0, lapic_read_reg(0xF0) | 0x100);
}