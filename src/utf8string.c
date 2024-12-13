#include <string.h>

#include "cunicode.h"

bool bytes_are_valid_utf8(const uint8_t *bytes, size_t len) {
	size_t i = 0;
	size_t continuation_bytes_left = 0;
	while (i < len) {
		uint8_t first_byte = bytes[i];

		if (first_byte >> 7 == 0) {
			continuation_bytes_left = 0;
			i++;
			continue;
		} else if (first_byte >> 5 == 0b0110) {
			continuation_bytes_left = 1;
		} else if (first_byte >> 4 == 0b01110) {
			continuation_bytes_left = 2;
		} else if (first_byte >> 3 == 0b011110) {
			continuation_bytes_left = 3;
		} else {
			return false;
		}
		i++;
		while (continuation_bytes_left > 0) {
			if (bytes[i] >> 6 != 0b10) return false;
			i++;
			continuation_bytes_left--;
		}
	}
	if (continuation_bytes_left != 0) {
		return false;
	} else {
		return true;
	}
}

struct Utf8String Utf8String_from_bytes(const uint8_t *bytes, size_t len) {
	struct Utf8String error = {.bytes = NULL, .size = 0};
	if (!bytes_are_valid_utf8(bytes, len)) {
		return error;
	}
	uint8_t *new_bytes = malloc(len);
	if (new_bytes == NULL) {
		return error;
	}
	struct Utf8String result = {.bytes = new_bytes, .size = len};
	memcpy(result.bytes, bytes, len);
	return result;
}

void Utf8String_free(struct Utf8String *s) { free(s->bytes); }

struct Utf8String Utf8String_copy(const struct Utf8String *target) {
	uint8_t *new_bytes = malloc(target->size);
	struct Utf8String result = {.bytes = NULL, .size = 0};
	if (new_bytes == NULL) {
		return result;
	}
	memcpy(new_bytes, target->bytes, target->size);
	result.bytes = new_bytes;
	result.size = target->size;
	return result;
}

struct Utf8String Utf8String_from_cstring(const char *s) {
	struct Utf8String result = {.bytes = NULL, .size = 0};
	uint8_t *bytes = malloc(strlen(s));
	if (bytes == NULL) {
		return result;
	}
	memcpy(bytes, s, strlen(s));
	result.bytes = bytes;
	result.size = strlen(s);
	return result;
}

bool Utf8String_eq(const struct Utf8String *a, const struct Utf8String *b) {
	if (a->size != b->size) {
		return false;
	}
	if (memcmp(a->bytes, b->bytes, a->size) == 0)
		return true;
	else
		return false;
}

bool Utf8String_append(struct Utf8String *s,
                       const struct Utf8String *extension) {
	uint8_t *new_bytes = realloc(s->bytes, s->size + extension->size);
	if (new_bytes == NULL) {
		return false;
	}
	memcpy(new_bytes + s->size, extension->bytes, extension->size);
	s->size = s->size + extension->size;
	s->bytes = new_bytes;
	return true;
}

bool Utf8String_starts_with(struct Utf8String *s,
                            const struct Utf8String *start) {
	if (start->size > s->size) return false;
	size_t len = start->size;
	if (memcmp(s->bytes, start->bytes, len) == 0)
		return true;
	else
		return false;
}
