#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct Utf8CodePoint {
	size_t len;
	uint8_t bytes[4];
};

struct Utf8String {
	size_t size;
	uint8_t *bytes;
};

struct Utf8CodePoint Utf8CodePoint_from_bytes(unsigned char *bytes, size_t len);

bool bytes_are_valid_utf8(uint8_t *bytes, size_t len);

struct Utf8String Utf8String_from_bytes(uint8_t *bytes, size_t len);

void Utf8Strig_free(struct Utf8String *s);

struct Utf8String Utf8String_copy(struct Utf8String *target);