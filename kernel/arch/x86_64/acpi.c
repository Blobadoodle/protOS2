#include <stdio.h>
#include <stddef.h>
#include <limine.h>
#include <string.h>
#include <stdbool.h>

#include <kernel/acpi.h>
#include <kernel/io.h>

#include "acpi.h"

static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};

struct limine_rsdp_response *response;

rsdp_descriptor_t *rsdp;
rsdt_t *rsdt;

bool sdt_checksum(sdt_header_t *table_header) {
    uint8_t sum = 0;
    for(uint32_t i = 0; i < table_header->length; i++) {
        sum += ((char *) table_header)[i];
    }

    return sum == 0;
}

void* findTable(const char *str) {
    uint32_t entries = (rsdt->h.length - sizeof(rsdt->h)) / 4;

    for(uint32_t i = 0; i < entries; i++) {
        sdt_header_t *h = (sdt_header_t *) rsdt->PointerToOtherSDT[i];
        if(!strncmp(h->signature, str, 4)) return (void *) h;
    }

    return NULL; // No table found
}

void acpi_init() {
    response = rsdp_request.response;
    if(response == NULL) {
        panic("Cannot find ACPI RSDP");
    }
    sprintf("RSDP Revision: %d\r\nRSDP Address: 0x%x\r\n", response->revision, response->address);
    rsdp = response->address;

    char rsdp_signature[9];
    memcpy(rsdp_signature, rsdp->signature, 8);
    rsdp_signature[9] = '\0';
    sprintf("RSDP Signature: \"%s\"\r\n", rsdp_signature);
    if(strcmp(rsdp_signature, "RSD PTR ")) {
        panic("ACPI RSDP Signature Invalid");
    };

    char oemid[7];
    memcpy(oemid, rsdp->oem_id, 6);
    oemid[7] = '\0';
    sprintf("RSDP OEM ID: \"%s\"\r\n", oemid);

    rsdt = rsdp->rsdt_address;

    char rsdt_signature[5];
    memcpy(rsdt_signature, rsdt->h.signature, 4);
    rsdt_signature[5] = '\0';
    sprintf("RSDT Signature: \"%s\"\r\n", rsdt_signature);

    if(findTable("FACP") == NULL) {
        panic("Cannout find ACPI FADT");
    }
}