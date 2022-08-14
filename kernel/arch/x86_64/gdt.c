// Used for creating GDT segment descriptors in 64-bit integer form.
// osdev wiki my beloved
 
#include <stdio.h>
#include <stdint.h>

#include <kernel/io.h>
#include <kernel/gdt.h>

// Each define here is for a specific flag in the descriptor.
// Refer to the intel documentation for a description of what each one does.
#define SEG_DESCTYPE(x)  ((x) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define SEG_PRES(x)      ((x) << 0x07) // Present
#define SEG_SAVL(x)      ((x) << 0x0C) // Available for system use
#define SEG_LONG(x)      ((x) << 0x0D) // Long mode
#define SEG_SIZE(x)      ((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define SEG_GRAN(x)      ((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define SEG_PRIV(x)     (((x) &  0x03) << 0x05)   // Set privilege level (0 - 3)
 
#define SEG_DATA_RD        0x00 // Read-Only
#define SEG_DATA_RDA       0x01 // Read-Only, accessed
#define SEG_DATA_RDWR      0x02 // Read/Write
#define SEG_DATA_RDWRA     0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD    0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA   0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD  0x06 // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA 0x07 // Read/Write, expand-down, accessed
#define SEG_CODE_EX        0x08 // Execute-Only
#define SEG_CODE_EXA       0x09 // Execute-Only, accessed
#define SEG_CODE_EXRD      0x0A // Execute/Read
#define SEG_CODE_EXRDA     0x0B // Execute/Read, accessed
#define SEG_CODE_EXC       0x0C // Execute-Only, conforming
#define SEG_CODE_EXCA      0x0D // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC     0x0E // Execute/Read, conforming
#define SEG_CODE_EXRDCA    0x0F // Execute/Read, conforming, accessed

#define GDT_16_CODE  SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(0) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_CODE_EXRD
 
#define GDT_16_DATA  SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(0) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_DATA_RDWR

#define GDT_32_CODE  SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_CODE_EXRD
 
#define GDT_32_DATA  SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_DATA_RDWR

#define GDT_64_CODE  SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(1)     | SEG_SIZE(0) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_CODE_EXRD
 
#define GDT_64_DATA  SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(1)     | SEG_SIZE(0) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_DATA_RDWR

uint8_t number = 0;

uint64_t gdt[7];

typedef struct {
    uint16_t limit;
    uint64_t base;
}__attribute__((packed)) gdtr_t;

gdtr_t gdtr;

void gdt_set_descriptor(uint32_t base, uint32_t limit, uint16_t flag) {
    uint64_t descriptor;
 
    // Create the high 32 bit segment
    descriptor  =  limit       & 0x000F0000;         // set limit bits 19:16
    descriptor |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
    descriptor |= (base >> 16) & 0x000000FF;         // set base bits 23:16
    descriptor |=  base        & 0xFF000000;         // set base bits 31:24
 
    // Shift by 32 to allow for low part of segment
    descriptor <<= 32;
 
    // Create the low 32 bit segment
    descriptor |= base  << 16;                       // set base bits 15:0
    descriptor |= limit  & 0x0000FFFF;               // set limit bits 15:0
 
    gdt[number] = descriptor;
    sprintf("GDT %d(Base: 0x%x Limit: 0x%x Flag: 0x%x Offset: 0x%x) = 0x%x\r\n", number, base, limit, flag, number*0x08, descriptor);
    number++;
}

void gdt_init() {
    gdtr.base = (uint64_t)&gdt;
    gdtr.limit = (uint16_t)sizeof(gdt) -1;
    
    cli(); // Disable interrupts
    
    gdt_set_descriptor(0, 0, 0); // Null descriptor 0x00
    gdt_set_descriptor(0, 0xffff, (GDT_16_CODE)); //0x08
    gdt_set_descriptor(0, 0xffff, (GDT_16_DATA)); //0x10
    gdt_set_descriptor(0, 0xffffffff, (GDT_32_CODE)); //0x18
    gdt_set_descriptor(0, 0xffffffff, (GDT_32_DATA)); //0x20
    gdt_set_descriptor(0, 0, (GDT_64_CODE)); //0x28
    gdt_set_descriptor(0, 0, (GDT_64_DATA)); //0x30

    __asm__ volatile ("lgdt %0" : : "m"(gdtr));
    sprintf("GDT Installed (Limit: 0x%x Base: 0x%x)\r\n\r\n", gdtr.limit, gdtr.base);
}