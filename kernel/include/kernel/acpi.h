#ifndef _KERNEL_ACPI_H
#define _KERNEL_ACPI_H

void acpi_init();

typedef struct {
	char signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	char oem_id[6];
	char oem_table_id[8];
	uint32_t oem_revision;
	uint32_t creator_id;
	uint32_t creator_revision;
} sdt_header_t;

// MADT

typedef struct {
    uint8_t processor_id;
    uint8_t apic_id;
    uint32_t flags;
} lapic_t;

typedef struct {
    uint8_t apic_id;
    uint8_t reserved;
    uint32_t apic_address;
    uint32_t system_interrupt_base;
} ioapic_t;

typedef struct {
    uint8_t bus_source;
    uint8_t irq_source;
    uint32_t interrupt;
    uint16_t flags;
} ioapic_interrupt_t;

typedef struct {
    uint8_t nmi;
    uint8_t reserved;
    uint16_t flags;
    uint32_t gbi;
} ioapic_nmi_t;

typedef struct {
    uint8_t processor_id; // 0xff = all
    uint16_t flags;
    uint8_t lint;
} lapic_nmi_t;

typedef struct {
    uint16_t reserved;
    uint64_t lapic;
} lapic_address_t;

typedef struct {
    uint16_t reserved;
    uint32_t processor_id;
    uint32_t flags;
    uint32_t acpi_id;
} x2apic_t;

typedef struct {
    uint8_t type;
    uint8_t length;
} madt_entry_t;

typedef struct {
    sdt_header_t h;
    uint32_t lapic_address;
    uint32_t flags;
    madt_entry_t entries[];
} madt_t;

void* findTable(const char *str);

#endif