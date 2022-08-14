#ifndef _KERNEL_PIC_H
#define _KERNEL_PIC_H

void pic_init();
void pic_eoi(uint8_t irq);
void pic_disable();

#endif