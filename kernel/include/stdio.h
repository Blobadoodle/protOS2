#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h>
#include <stdint.h>

#define printf kprintf

uint64_t kprintf(const char* format, ...);
uint64_t kvprintf(const char* format, va_list arg);
uint64_t sprintf(const char *format, ...);

#endif