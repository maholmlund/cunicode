#include <criterion/criterion.h>

#include "../src/cunicode.h"

Test(utf8codepoint_simple, valid_first) {
	unsigned char a = 65;
	struct Utf8CodePoint result = Utf8CodePoint_from_bytes(&a, 1);
	cr_assert(result.len == 1);
	cr_assert(result.bytes[0] == 65);
}

Test(utf8codepoint_simple, valid_second) {
	unsigned char a = 10;
	struct Utf8CodePoint result = Utf8CodePoint_from_bytes(&a, 1);
	cr_assert(result.len == 1);
	cr_assert(result.bytes[0] == 10);
}

Test(utf8codepoint_simple, invalid) {
	unsigned char a = 0b10000000;
	struct Utf8CodePoint result = Utf8CodePoint_from_bytes(&a, 1);
	cr_assert(result.len == 0);
}

Test(utf8codepoint_complex, valid_first) {
	char *a = "ö";
	struct Utf8CodePoint result =
	    Utf8CodePoint_from_bytes((unsigned char *)a, 2);
	cr_assert(result.len == 2);
	cr_assert_arr_eq(a, result.bytes, 2);
}

Test(utf8codepoint_complex, valid_second) {
	char *a = "😁";
	struct Utf8CodePoint result =
	    Utf8CodePoint_from_bytes((unsigned char *)a, 4);
	cr_assert(result.len == 4);
	cr_assert_arr_eq(a, result.bytes, 4);
}

Test(utf8codepoint_complex, invalid_first) {
	char *a = "😁";
	struct Utf8CodePoint result =
	    Utf8CodePoint_from_bytes((unsigned char *)a, 3);
	cr_assert(result.len == 0);
}

Test(utf8codepoint_complex, invalid_second) {
	unsigned char a[2] = {0b10000000, 0b00000000};
	struct Utf8CodePoint result = Utf8CodePoint_from_bytes(&a[0], 2);
	cr_assert(result.len == 0);
}

Test(utf8codepoint_complex, invalid_third) {
	unsigned char a[2] = {0b11000000, 0b11000000};
	struct Utf8CodePoint result = Utf8CodePoint_from_bytes(&a[0], 2);
	cr_assert(result.len == 0);
}

Test(test_Utf8CodePoint_from_index, valid_first) {
	struct Utf8String s = Utf8String_from_cstring("Hello World!");
	struct Utf8CodePoint target = {.bytes = {108, 0, 0, 0}, .len = 1};
	struct Utf8CodePoint actual = Utf8CodePoint_from_index(&s, 2);
	cr_assert(target.len == actual.len &&
	          target.bytes[0] == actual.bytes[0]);
}

Test(test_Utf8CodePoint_from_index, valid_second) {
	struct Utf8String s = Utf8String_from_cstring("Hel😼o World!");
	char *emoji = "😼";
	struct Utf8CodePoint target =
	    Utf8CodePoint_from_bytes((unsigned char *)emoji, 4);
	struct Utf8CodePoint actual = Utf8CodePoint_from_index(&s, 3);
	cr_assert(target.len == actual.len &&
	          memcmp(target.bytes, actual.bytes, 4) == 0);
}

Test(test_Utf8CodePoint_from_index, invalid_first) {
	struct Utf8String s = Utf8String_from_cstring("Hel😼o World!");
	struct Utf8CodePoint actual = Utf8CodePoint_from_index(&s, 5);
	cr_assert(actual.len == 0 && actual.bytes[0] == 0);
}

Test(test_Utf8CodePoint_from_index, invalid_second) {
	struct Utf8String s = Utf8String_from_cstring("He😼😼o World!");
	struct Utf8CodePoint actual = Utf8CodePoint_from_index(&s, 5);
	cr_assert(actual.len == 0 && actual.bytes[0] == 0);
}
