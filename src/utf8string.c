#include <string.h>

#include "cunicode.h"

int get_codepoint_len(const uint8_t byte) {
	if (byte >> 7 == 0b00000000) {
		return 1;
	} else if (byte >> 5 == 0b00000110) {
		return 2;
	} else if (byte >> 4 == 0b00001110) {
		return 3;
	} else if (byte >> 3 == 0b00011110) {
		return 4;
	} else {
		return -1;
	}
}

bool bytes_are_valid_utf8(const uint8_t *bytes, size_t len) {
	size_t i = 0;
	int continuation_bytes_left = 0;
	while (i < len) {
		uint8_t first_byte = bytes[i];

		continuation_bytes_left = get_codepoint_len(first_byte) - 1;
		if (continuation_bytes_left == 0) {
			i++;
			continue;
		} else if (continuation_bytes_left == -2) {
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

bool bytes_are_ascii(const uint8_t *bytes, size_t len) {
	for (size_t i = 0; i < len; i++) {
		if (bytes[i] > 127) return false;
	}
	return true;
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

size_t Utf8String_find_after(const struct Utf8String *s,
                             const struct Utf8String *target, size_t start) {
	if (s->size - start < target->size) {
		return s->size;
	}
	size_t result;
	bool found = false;
	for (size_t i = start; i < s->size;) {
		if (s->size - i < target->size) {
			found = false;
			break;
		}
		if (memcmp(s->bytes + i, target->bytes, target->size) == 0) {
			found = true;
			result = i;
			break;
		}
		int len = get_codepoint_len(*(s->bytes + i));
		if (len == -1) {
			return s->size;
		}
		i += len;
	}
	if (found) {
		return result;
	} else {
		return s->size;
	}
}

size_t Utf8String_find(const struct Utf8String *s,
                       const struct Utf8String *target) {
	return Utf8String_find_after(s, target, 0);
}

struct Utf8String Utf8String_get_substr(const struct Utf8String *s, size_t i,
                                        size_t len) {
	struct Utf8String invalid = {0, NULL};
	if (s->size - i < len) {
		return invalid;
	}
	if (get_codepoint_len(*(s->bytes + i)) == -1 ||
	    (s->size - i != len &&
	     get_codepoint_len(*(s->bytes + i + len)) == -1)) {
		return invalid;
	}

	uint8_t *new_bytes = malloc(len);
	if (new_bytes == NULL) {
		return invalid;
	}
	memcpy(new_bytes, s->bytes + i, len);
	struct Utf8String result = {len, new_bytes};
	return result;
}

bool Utf8String_is_codepoint_start(const struct Utf8String *s, size_t i) {
	if (i >= s->size) {
		return false;
	} else
		return get_codepoint_len(*(s->bytes + i)) != -1;
}

bool Utf8String_ends_with(struct Utf8String *s, const struct Utf8String *end) {
	if (end->size > s->size) {
		return false;
	}
	if (memcmp(s->bytes + (s->size - end->size), end->bytes, end->size) ==
	    0)
		return true;
	else
		return false;
}
