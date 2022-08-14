#include <stdio.h>

#include <kernel/pic.h>
#include <kernel/io.h>

#define PIC1		0x20		// IO base address for master PIC
#define PIC2		0xA0		// IO base address for slave PIC
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define PIC_EOI		0x20		// End-of-interrupt command code

#define ICW1_ICW4	0x01		// ICW4 (not) needed
#define ICW1_SINGLE	0x02		// Single (cascade) mode
#define ICW1_INTERVAL4	0x04	// Call address interval 4 (8)
#define ICW1_LEVEL	0x08		// Level triggered (edge) mode
#define ICW1_INIT	0x10		// Initialization - required!
 
#define ICW4_8086	0x01		// 8086/88 (MCS-80/85) mode
#define ICW4_AUTO	0x02		// Auto (normal) EOI
#define ICW4_BUF_SLAVE	0x08	// Buffered mode/slave
#define ICW4_BUF_MASTER	0x0C	// Buffered mode/master
#define ICW4_SFNM	0x10		// Special fully nested (not)

void pic_init() {
    uint8_t m1, m2;

    m1 = inb(PIC1_DATA); // save masks
    m2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, 0x28);
    io_wait();
    outb(PIC2_DATA, 0x20);
    io_wait();
    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, m1);
    outb(PIC2_DATA, m2);

    outb(PIC1_DATA, 0xfd);
    outb(PIC2_DATA, 0xff);

    sprintf("PIC Initialized with offsets 0x%x 0x%x\r\n\r\n", 0x20, 0x28);
}

void pic_eoi(uint8_t irq) {
    if(40 <= irq) outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

void pic_disable() {
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}