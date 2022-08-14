#ifndef _ARCH_X86_64_ACPI_H
#define _ARCH_X86_64_ACPI_H 

#include <stdint.h>

typedef struct {
 char signature[8];
 uint8_t checksum;
 char oem_id[6];
 uint8_t revision;
 uint32_t rsdt_address;
} __attribute__ ((packed)) rsdp_descriptor_t;

typedef struct {
  sdt_header_t h;
  uint32_t PointerToOtherSDT[];
} rsdt_t;

#endif