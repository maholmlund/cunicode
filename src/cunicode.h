#pragma once
#include <stdbool.h>
#include <stdlib.h>

struct Utf8CodePoint {
	size_t len;
	unsigned char bytes[4];
};

struct Utf8CodePoint Utf8CodePoint_from_bytes(unsigned char *bytes, size_t len);
