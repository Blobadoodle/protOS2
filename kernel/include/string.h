#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str);
void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size);
uint64_t strcmp(const char *x, const char *y);
uint64_t strncmp(const char *x, const char *y, size_t n);

#endif