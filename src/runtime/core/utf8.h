#ifndef UTF8_UTILS_H
#define UTF8_UTILS_H
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#define UTF8_HI_BITS 0x8080808080808080ull
static inline __attribute__((always_inline)) uint64_t utf8_load8(const char *p) { return *(const uint64_t *)(const void *)p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) int utf8_seqlen(unsigned char b) {
    if (b < 0x80) return 1;
    if (b < 0xC0) return 1;
    if (b < 0xE0) return 2;
    if (b < 0xF0) return 3;
    return 4;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline size_t utf8_strlen(const char *s) {
    if (!s) return 0;
    size_t n = 0;
    while (*s) { s += utf8_seqlen((unsigned char)*s); n++; }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline size_t utf8_strlen_n(const char *s, size_t blen) {
    if (!s) return 0;
    size_t off = 0, n = 0;
    while (off + 8 <= blen && !(utf8_load8(s + off) & UTF8_HI_BITS)) { off += 8; n += 8; }
    while (off < blen) { off += (size_t)utf8_seqlen((unsigned char)s[off]); n++; }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline size_t utf8_char_offset(const char *s, size_t slen, size_t ch) {
    size_t off = 0, i = 1;
    while (off + 8 <= slen && i + 8 <= ch && !(utf8_load8(s + off) & UTF8_HI_BITS)) { off += 8; i += 8; }
    while (off < slen && i < ch) { off += (size_t)utf8_seqlen((unsigned char)s[off]); i++; }
    return off;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline size_t utf8_char_bytes(const char *s, size_t slen, size_t off, size_t n) {
    size_t start = off, i = 0;
    while (off + 8 <= slen && i + 8 <= n && !(utf8_load8(s + off) & UTF8_HI_BITS)) { off += 8; i += 8; }
    while (off < slen && i < n) { off += (size_t)utf8_seqlen((unsigned char)s[off]); i++; }
    return off - start;
}
#endif
