#include <stdio.h>
#include <kernel/io.h>
#include <kernel/tty.h>

const char *exception_messages[] = {
    "Divide By Zero",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved"
};

__attribute__((noreturn))
void exception_handler(uint8_t vector) {
  panic(exception_messages[vector]);
}

__attribute__((noreturn))
void panic(const char *str) {
  printf("\nPANIC!\nError: %s", str);
  sprintf("\r\nPANIC!\r\nError: %s", str);
  halt();
}

char keyboard_uk [128] =
{
    0,  '\\', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
  '\t', /* <-- Tab */
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     
    0, /* <-- control key */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0, '#', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

void irq_handler(uint8_t vector) {
    uint8_t irq = vector - 40;
    if(irq == 1) {
        uint8_t scan_code = inb(0x60);
        char key = keyboard_uk[scan_code];
        if(key && scan_code < 128 ) {
            tty_putchar(key);
            if(key == '\b') {
              tty_putchar(' ');
              tty_putchar('\b');
            }
        }
    } else {
        printf("IRQ %d triggered\n", irq);
    }

    pic_eoi();
}