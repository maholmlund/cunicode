#include <criterion/criterion.h>

#include "../src/cunicode.h"

Test(bytes_are_valid_utf8_valid, ascii) {
	char *bytes = "abab";
	cr_assert(bytes_are_valid_utf8((uint8_t *)bytes, 4));
}

Test(bytes_are_valid_utf8_valid, multibyte) {
	uint8_t bytes[4] = {0xCF, 0x90, 0xD0, 0x9A};
	cr_assert(bytes_are_valid_utf8(&bytes[0], 4));
}

Test(bytes_are_valid_utf8_valid, random_ascii) {
	char *bytes = "2?gjts.F U0Tw[gG$jONc||OXjf;vd|/";
	cr_assert(bytes_are_valid_utf8((uint8_t *)&bytes[0], strlen(bytes)));
}

Test(bytes_are_valid_utf8_valid, random_multibyte) {
	char *bytes =
	    "“d𔮹h𲤃󗁫ϣ𪑊ԣ㚩ｵ-١񛃖҈(돇ӣ<"
	    "꤂򽶓抛󉛵䘁㏌ГꪓԿt򉖾𗮀ꒉ◓쒿ݙײַ.ǂ歳񢕓-"
	    "̶馴툣𕏳Ha񒭄ěU."
	    "°ζתl빊TJ�~ɤ";
	cr_assert(bytes_are_valid_utf8((uint8_t *)&bytes[0], strlen(bytes)));
}

Test(bytes_are_valid_utf8_valid, null) {
	uint8_t byte = 0;
	cr_assert(bytes_are_valid_utf8(&byte, 1));
}

Test(bytes_are_valid_utf8_invalid, 1) {
	uint8_t bytes[4] = {0x0F, 0x90, 0xF0, 0x9A};
	cr_assert(!bytes_are_valid_utf8(&bytes[0], 4));
}

Test(bytes_are_valid_utf8_invalid, 2) {
	uint8_t bytes[4] = {0x0F, 0xF0, 0xA0, 0x9A};
	cr_assert(!bytes_are_valid_utf8(&bytes[0], 4));
}

Test(bytes_are_valid_utf8_invalid, 3) {
	uint8_t bytes[16] = {0x61, 0xC4, 0x2E, 0xA1, 0x58, 0x70, 0x0F, 0x88,
	                     0x18, 0x38, 0x5D, 0x52, 0x83, 0x47, 0xFE, 0xAF};
	cr_assert(!bytes_are_valid_utf8(&bytes[0], 16));
}

Test(Utf8String_creation, simple) {
	char *bytes = "Hello, World!";
	cr_assert(bytes_are_valid_utf8((uint8_t *)bytes, strlen(bytes)));
	struct Utf8String s =
	    Utf8String_from_bytes((uint8_t *)bytes, strlen(bytes));
	cr_assert(s.bytes != NULL);
	cr_assert((char)s.bytes[0] == 'H');
	Utf8String_free(&s);
}

Test(Utf8String_creation, invalid) {
	uint8_t bytes[3] = {0xFF, 0x0, 0x0};
	struct Utf8String s = Utf8String_from_bytes(&bytes[0], 3);
	cr_assert(s.bytes == NULL);
}

Test(Utf8String_copy, first) {
	struct Utf8String s =
	    Utf8String_from_bytes((uint8_t *)"Hello World!", 12);
	cr_assert(s.size == 12);
	struct Utf8String copy = Utf8String_copy(&s);
	cr_assert(copy.size == s.size);
	cr_assert(memcmp(s.bytes, copy.bytes, s.size) == 0);
	Utf8String_free(&s);
	Utf8String_free(&copy);
}