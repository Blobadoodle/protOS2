#include <string.h>
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (size_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}

uint64_t strcmp(const char *x, const char *y) {
	while(*x && (*x == *y)) {
		x++;
		y++;
	}
	return *(const unsigned char*)x - *(const unsigned char*)y;
}

uint64_t strncmp(const char *x, const char *y, size_t n) {
	while(n && *x && (*x == *y)) {
		x++;
		y++;
		n--;
	}
	if(n == 0) return 0;
	else return *(const unsigned char *)x - *(const unsigned char*)y;
}