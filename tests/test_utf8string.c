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

Test(Utf8String_from_cstring, first) {
	char *string = "Hêllö World!";
	struct Utf8String result = Utf8String_from_cstring(string);
	cr_assert(result.size == strlen(string));
	cr_assert(memcmp(string, result.bytes, strlen(string)) == 0);
}

Test(Utf8String_eq, equal_1) {
	char *s = "Hello World!\n";
	struct Utf8String a = Utf8String_from_cstring(s);
	struct Utf8String b = Utf8String_from_cstring(s);
	cr_assert(Utf8String_eq(&a, &b));
}

Test(Utf8String_eq, equal_2) {
	char *first = "Hello World!\n";
	char *second = "Hello World!\n";
	struct Utf8String a = Utf8String_from_cstring(first);
	struct Utf8String b = Utf8String_from_cstring(second);
	cr_assert(Utf8String_eq(&a, &b));
}

Test(Utf8String_eq, not_equal_1) {
	char *first = "Hellö";   // last character is U+0308
	char *second = "Hellö";  // last character is U+00F6
	struct Utf8String a = Utf8String_from_cstring(first);
	struct Utf8String b = Utf8String_from_cstring(second);
	cr_assert(!Utf8String_eq(&a, &b));
}

Test(Utf8String_eq, not_equal_2) {
	char *first = "Hello World!";  // last character is U+0308
	char *second = "Foo Bar";      // last character is U+00F6
	struct Utf8String a = Utf8String_from_cstring(first);
	struct Utf8String b = Utf8String_from_cstring(second);
	cr_assert(!Utf8String_eq(&a, &b));
}

Test(Utf8String_append, first) {
	struct Utf8String original = Utf8String_from_cstring("Hello ");
	struct Utf8String extension = Utf8String_from_cstring("World!");
	struct Utf8String goal = Utf8String_from_cstring("Hello World!");
	Utf8String_append(&original, &extension);
	cr_assert(Utf8String_eq(&original, &goal));
}

Test(Utf8String_append, append_empty) {
	struct Utf8String original = Utf8String_from_cstring("Hello");
	struct Utf8String extension = Utf8String_from_cstring("");
	struct Utf8String goal = Utf8String_from_cstring("Hello");
	Utf8String_append(&original, &extension);
	cr_assert(Utf8String_eq(&original, &goal));
}

Test(Utf8String_starts_with, basic) {
	struct Utf8String s = Utf8String_from_cstring("Hello World!");
	struct Utf8String start = Utf8String_from_cstring("Hello");
	cr_assert(Utf8String_starts_with(&s, &start));
}

Test(Utf8String_starts_with, empty) {
	struct Utf8String s = Utf8String_from_cstring("Hello World!");
	struct Utf8String start = Utf8String_from_cstring("");
	cr_assert(Utf8String_starts_with(&s, &start));
}

Test(Utf8String_starts_with, does_not_start) {
	struct Utf8String s = Utf8String_from_cstring("Hello World!");
	struct Utf8String start = Utf8String_from_cstring("ello");
	cr_assert(!Utf8String_starts_with(&s, &start));
}

Test(Utf8String_starts_with, longer_start) {
	struct Utf8String s = Utf8String_from_cstring("Hello World!");
	struct Utf8String start = Utf8String_from_cstring("Hello World!!!!");
	cr_assert(!Utf8String_starts_with(&s, &start));
}

Test(Utf8String_find, valid_simple) {
	struct Utf8String s = Utf8String_from_cstring("Hello World!");
	struct Utf8String target = Utf8String_from_cstring("Wor");
	cr_assert(Utf8String_find(&s, &target) == 6);
}

Test(Utf8String_find, valid_complex) {
	struct Utf8String s = Utf8String_from_cstring("🐪εa👣🎳fa🆓\n🤗");
	struct Utf8String target = Utf8String_from_cstring("🎳f");
	cr_assert(Utf8String_find(&s, &target) == 11);
}

Test(Utf8String_find, invalid_simple) {
	struct Utf8String s = Utf8String_from_cstring("Hello World!");
	struct Utf8String target = Utf8String_from_cstring("orld!n");
	cr_assert(Utf8String_find(&s, &target) == s.size);
}

Test(Utf8String_find, invalid_complex) {
	struct Utf8String s = Utf8String_from_cstring("🐪εa👣🎳fa🆓\n🤗");
	struct Utf8String target = Utf8String_from_cstring("f\n🤗");
	cr_assert(Utf8String_find(&s, &target) == s.size);
}

Test(Utf8String_find, empty_target) {
	struct Utf8String s = Utf8String_from_cstring("🐪εa👣🎳fa🆓\n🤗");
	struct Utf8String target = Utf8String_from_cstring("");
	cr_assert(Utf8String_find(&s, &target) == 0);
}

Test(Utf8String_find_after, valid_simple) {
	struct Utf8String s = Utf8String_from_cstring("abbabba");
	struct Utf8String target = Utf8String_from_cstring("bb");
	cr_assert(Utf8String_find_after(&s, &target, 2) == 4);
}

Test(Utf8String_find_after, valid_complex) {
	struct Utf8String s = Utf8String_from_cstring("🐪🎳fεa👣🎳fa🆓\n🤗");
	struct Utf8String target = Utf8String_from_cstring("🎳f");
	cr_assert(Utf8String_find_after(&s, &target, 11) == 16);
}

Test(Utf8String_find_after, invalid_simple) {
	struct Utf8String s = Utf8String_from_cstring("Hello World!");
	struct Utf8String target = Utf8String_from_cstring("llo");
	cr_assert(Utf8String_find_after(&s, &target, 3) == s.size);
}

Test(Utf8String_find_after, invalid_complex) {
	struct Utf8String s = Utf8String_from_cstring("🐪🎳fεa👣🎳fa🆓\n🤗");
	struct Utf8String target = Utf8String_from_cstring("🎳f");
	cr_assert(Utf8String_find_after(&s, &target, 6) == s.size);
}

Test(Utf8String_get_substr, simple_first) {
	struct Utf8String s = Utf8String_from_cstring("Hello World!");
	struct Utf8String target = Utf8String_from_cstring("Wor");
	struct Utf8String result = Utf8String_get_substr(&s, 6, 3);
	cr_assert(Utf8String_eq(&target, &result));
}

Test(Utf8String_get_substr, simple_second) {
	struct Utf8String s = Utf8String_from_cstring("Hello World!");
	struct Utf8String target = Utf8String_from_cstring("World!");
	struct Utf8String result = Utf8String_get_substr(&s, 6, 6);
	cr_assert(Utf8String_eq(&target, &result));
}

Test(Utf8String_get_substr, complex_first) {
	struct Utf8String s = Utf8String_from_cstring("🐪🎳fεa👣🎳fa🆓\n🤗");
	struct Utf8String target = Utf8String_from_cstring("εa👣");
	struct Utf8String result = Utf8String_get_substr(&s, 9, 7);
	cr_assert(Utf8String_eq(&target, &result));
}

Test(Utf8String_get_substr, invalid_first) {
	struct Utf8String s = Utf8String_from_cstring("Hello World!");
	struct Utf8String result = Utf8String_get_substr(&s, 6, 7);
	cr_assert(result.bytes == NULL);
}

Test(Utf8String_get_substr, invalid_second) {
	struct Utf8String s = Utf8String_from_cstring("🐪🎳fεa👣🎳fa🆓\n🤗");
	struct Utf8String result = Utf8String_get_substr(&s, 2, 5);
	cr_assert(result.bytes == NULL);
}
