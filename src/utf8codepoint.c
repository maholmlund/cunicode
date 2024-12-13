#include <string.h>

#include "cunicode.h"

struct Utf8CodePoint Utf8CodePoint_from_bytes(const uint8_t *bytes,
                                              size_t len) {
	struct Utf8CodePoint result = {0, ""};
	if (len < 1 || len > 4) {
		return result;
	}
	if (len == 1) {
		if (bytes[0] >> 7 == 0b00000000) {
			result.len = 1;
			result.bytes[0] = bytes[0];
		}
	} else {
		bool valid = true;
		switch (len) {
			case 2:
				if (bytes[0] >> 5 != 0b00000110) valid = false;
				break;
			case 3:
				if (bytes[0] >> 4 != 0b00001110) valid = false;
				break;
			case 4:
				if (bytes[0] >> 3 != 0b00011110) valid = false;
				break;
		}
		for (size_t i = 0; i < len - 1; i++) {
			if (bytes[1 + i] >> 6 != 0b00000010) {
				valid = false;
				break;
			}
		}
		if (valid) {
			result.len = len;
			memcpy(&result.bytes, bytes, len);
		}
	}
	return result;
}

struct Utf8CodePoint Utf8CodePoint_from_index(const struct Utf8String *s,
                                              size_t i) {
	size_t len = 0;
	if (*(s->bytes + i) >> 3 == 0b00011110)
		len = 4;
	else if (*(s->bytes + i) >> 4 == 0b00001110)
		len = 3;
	else if (*(s->bytes + i) >> 5 == 0b00000110)
		len = 2;
	else if (*(s->bytes + i) >> 7 == 0b00000000)
		len = 1;
	else {
		struct Utf8CodePoint invalid = {0, ""};
		return invalid;
	}
	struct Utf8CodePoint point;
	memcpy(&point.bytes, s->bytes + i, len);
	point.len = len;
	return point;
}