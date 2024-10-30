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

struct Utf8CodePoint Utf8CodePoint_from_bytes(const unsigned char *bytes,
                                              size_t len);

bool bytes_are_valid_utf8(const uint8_t *bytes, size_t len);

struct Utf8String Utf8String_from_bytes(const uint8_t *bytes, size_t len);

void Utf8String_free(struct Utf8String *s);

struct Utf8String Utf8String_copy(const struct Utf8String *target);