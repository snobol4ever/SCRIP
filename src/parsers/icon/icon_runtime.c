#include <string.h>
#include "ir/descr.h"
extern const char *kw_cset_intern(const char *canon, int len);
extern const char *kw_cset_empty(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cset_bits_of(const char *s, int blk, int len, unsigned char w[32]) {
    if (s && blk) { memcpy(w, cset_blk_bits(s), 32); return; }
    memset(w, 0, 32);
    if (s) { const unsigned char *p = (const unsigned char *)s, *e = p + (len > 0 ? len : 0); for (; p < e; p++) { unsigned c = *p; w[c >> 3] |= (unsigned char)(1u << (c & 7)); } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *cset_from_bits(const unsigned char w[32], int *outlen) {
    char buf[257]; int n = 0;
    for (int base = 0; base < 32; base += 8) { unsigned long long chunk; memcpy(&chunk, w + base, 8); if (!chunk) continue;
        for (int by = base; by < base + 8; by++) { unsigned m = w[by]; while (m) { int bit = __builtin_ctz(m); buf[n++] = (char)(by * 8 + bit); m &= m - 1; } } }
    buf[n] = '\0';
    if (outlen) *outlen = n;
    return n ? kw_cset_intern(buf, n) : kw_cset_empty();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *cset_union(const char *a, int alen, int ablk, const char *b, int blen, int bblk, int *outlen) {
    unsigned char x[32], y[32]; cset_bits_of(a, ablk, alen, x); cset_bits_of(b, bblk, blen, y);
    for (int i = 0; i < 32; i++) x[i] |= y[i];
    return cset_from_bits(x, outlen);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *cset_diff(const char *a, int alen, int ablk, const char *b, int blen, int bblk, int *outlen) {
    unsigned char x[32], y[32]; cset_bits_of(a, ablk, alen, x); cset_bits_of(b, bblk, blen, y);
    for (int i = 0; i < 32; i++) x[i] &= (unsigned char)~y[i];
    return cset_from_bits(x, outlen);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *cset_inter(const char *a, int alen, int ablk, const char *b, int blen, int bblk, int *outlen) {
    unsigned char x[32], y[32]; cset_bits_of(a, ablk, alen, x); cset_bits_of(b, bblk, blen, y);
    for (int i = 0; i < 32; i++) x[i] &= y[i];
    return cset_from_bits(x, outlen);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *cset_canonical(const char *cs, int len) {
    if (!cs || len <= 0) return kw_cset_empty();
    unsigned char x[32]; cset_bits_of(cs, 0, len, x);
    return cset_from_bits(x, (int *)0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_icn_cset_member_n(const char *needle, long len, int ch) {
    if (!needle) return 0;
    if (len < 0) len = (long)strlen(needle);
    return memchr(needle, (unsigned char)ch, (unsigned long)len) != 0;
}
