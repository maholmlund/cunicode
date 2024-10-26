#include <string.h>

#include "cunicode.h"

bool bytes_are_valid_utf8(uint8_t *bytes, size_t len) {
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