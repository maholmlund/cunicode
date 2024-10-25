#include <criterion/criterion.h>
#include "../src/cunicode.h"

Test(utf8codepoint_simple, valid_first)
{
        unsigned char a = 65;
        struct Utf8CodePoint result = Utf8CodePoint_from_bytes(&a, 1);
        cr_assert(result.len == 1);
        cr_assert(result.bytes[0] == 65);
}

Test(utf8codepoint_simple, valid_second)
{
        unsigned char a = 10;
        struct Utf8CodePoint result = Utf8CodePoint_from_bytes(&a, 1);
        cr_assert(result.len == 1);
        cr_assert(result.bytes[0] == 10);
}

Test(utf8codepoint_simple, invalid)
{
        unsigned char a = 0b10000000;
        struct Utf8CodePoint result = Utf8CodePoint_from_bytes(&a, 1);
        cr_assert(result.len == 0);
}

Test(utf8codepoint_complex, valid_first)
{
        char *a = "ö";
        struct Utf8CodePoint result = Utf8CodePoint_from_bytes((unsigned char *)a, 2);
        cr_assert(result.len == 2);
        cr_assert_arr_eq(a, result.bytes, 2);
}

Test(utf8codepoint_complex, valid_second)
{
        char *a = "😁";
        struct Utf8CodePoint result = Utf8CodePoint_from_bytes((unsigned char *)a, 4);
        cr_assert(result.len == 4);
        cr_assert_arr_eq(a, result.bytes, 4);
}

Test(utf8codepoint_complex, invalid_first)
{
        char *a = "😁";
        struct Utf8CodePoint result = Utf8CodePoint_from_bytes((unsigned char *)a, 3);
        cr_assert(result.len == 0);
}

Test(utf8codepoint_complex, invalid_second)
{
        unsigned char a[2] = {0b10000000, 0b00000000};
        struct Utf8CodePoint result = Utf8CodePoint_from_bytes(&a[0], 2);
        cr_assert(result.len == 0);
}

Test(utf8codepoint_complex, invalid_third)
{
        unsigned char a[2] = {0b11000000, 0b11000000};
        struct Utf8CodePoint result = Utf8CodePoint_from_bytes(&a[0], 2);
        cr_assert(result.len == 0);
}