#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* A struct representing a Utf8 codepoint. len indicates the lenght of the
 * codepoint in bytes (1-4) and bytes holds the actual bytes. If len < 4 the
 * last bytes are unused. Guarantees to always hold valid codepoints if used
 * only through the provided functions.
 */
struct Utf8CodePoint {
	size_t len;
	uint8_t bytes[4];
};

/* Represents a Utf8 encoded string. size indicates the size of the string in
 * bytes and bytes is a pointer to the actual data. Guarantees to always hold
 * valid Utf8 data if used only through the provided functions.
 */
struct Utf8String {
	size_t size;
	uint8_t *bytes;
};

/* Creates a Utf8CodePoint from given byte array.
 * If bytes do not form a valid Utf8 Codepoint, the return value will be a
 * CodePoint with a size of 0. Otherwise a valid codepoint is returned.
 */
struct Utf8CodePoint Utf8CodePoint_from_bytes(const unsigned char *bytes,
                                              size_t len);

/* Creates a Utf8CodePoint from a Utf8String given a valid index.
 * Always returns a valid CodePoint if the given index is a valid start of a
 * Utf8 Codepoint. If the index is invalid, a codepoint with a size of 0 will be
 * returned. (see Utf8String_is_codepoint_start())
 */
struct Utf8CodePoint Utf8CodePoint_from_index(const struct Utf8String *s,
                                              size_t i);

// Checks wether the given bytes are a valid Utf8 sequence.
bool bytes_are_valid_utf8(const uint8_t *bytes, size_t len);

// Checks wether the given bytes are all ascii characters.
bool bytes_are_ascii(const uint8_t *bytes, size_t len);

/* Creates a new Utf8String form the given byte array. If the bytes are invalid
 * a string with the size set to 0 will be returned. (see
 * bytes_are_valid_utf8())
 */
struct Utf8String Utf8String_from_bytes(const uint8_t *bytes, size_t len);

// Frees the memory used by a Utf8String.
void Utf8String_free(struct Utf8String *s);

// Creates a copy of a Utf8String allocating new memory for the new string.
struct Utf8String Utf8String_copy(const struct Utf8String *target);

/* Creates a new Utf8String from a null-terminated string. If string is not
 * valid Utf8, a Utf8String with bytes set to NULL will be returned.
 */
struct Utf8String Utf8String_from_cstring(const char *s);

/*Checks wether the strings are equal. This is a byte-per-byte check, it does
 * not check for canonical equivalance.
 */
bool Utf8String_eq(const struct Utf8String *a, const struct Utf8String *b);

/* Appends the string pointed by extension to the end of s. Does not free
 * extension.
 */
bool Utf8String_append(struct Utf8String *s,
                       const struct Utf8String *extension);

// Checks wether s starts with start. Does not check for canonical equivalance.
bool Utf8String_starts_with(struct Utf8String *s,
                            const struct Utf8String *start);

// Checks wether s ends with end. Does not check for canonical equivalance.
bool Utf8String_ends_with(struct Utf8String *s, const struct Utf8String *end);

/* Finds the index of the first occurence of target in s. If nothing can be
 * found, s.size is returned.
 */
size_t Utf8String_find(const struct Utf8String *s,
                       const struct Utf8String *target);

/* Finds the index of the first occurence of target in s starting from index i.
 * If nothing can be found, s.size is returned.
 */
size_t Utf8String_find_after(const struct Utf8String *s,
                             const struct Utf8String *target, size_t i);

/* Creates a new Utf8String from a subsection of s. Returns a string with bytes
 * set to NULL if i is not a valid codepoint start, if the last byte of the
 * substring is not a last byte of a codepoint or if len is too long. (see
 * Utf8String_is_codepoint_start())
 */
struct Utf8String Utf8String_get_substr(const struct Utf8String *s, size_t i,
                                        size_t len);

/* Checks wether the given index is a valid start of a codepoint byte sequence
 * in the given string.
 */
bool Utf8String_is_codepoint_start(const struct Utf8String *s, size_t i);
