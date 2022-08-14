#include <stdio.h>
#include <stdbool.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/io.h>
#include <kernel/serial.h>
#include <kernel/pic.h>
#include <kernel/acpi.h>
#include <kernel/apic.h>
//#include <kernel/paging.h>

void kmain(void) {
    serial_init(COM1);
    tty_init();
    gdt_init();
    idt_init();
    acpi_init();
    apic_init();
    //pic_init();
    //paging_init();

    printf("protOS 2\n");
    for(;;) {
        __asm__ volatile ("hlt");
    }
}