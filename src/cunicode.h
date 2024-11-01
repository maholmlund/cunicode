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

struct Utf8String Utf8String_from_cstring(const char *s);

bool Utf8String_eq(const struct Utf8String *a, const struct Utf8String *b);

bool Utf8String_append(struct Utf8String *s,
                       const struct Utf8String *extension);