#include <stdio.h>

#include "idt.h"

#include <kernel/idt.h>
#include <kernel/io.h>

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t *descriptor = &idt[vector];
 
    descriptor->isr_low        = (uint64_t)isr & 0xFFFF;
    descriptor->cs             = 0x28;
    descriptor->ist            = 0;
    descriptor->attributes     = flags;
    descriptor->isr_mid        = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high       = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved       = 0;
}

void idt_init() {
    idtr.base = (uint64_t)&idt;
    idtr.limit = (uint16_t)sizeof(idt) - 1;

    for(uint8_t vector = 0; vector < 48; vector++) {
        sprintf("IDT %d(0x%x)\r\n", vector, isr_stub_table[vector]);
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr));

    sti(); // Enable interrupts
    
    sprintf("IDT Installed (Limit: 0x%x Base: 0x%x)\r\n\r\n", idtr.limit, idtr.base);
}