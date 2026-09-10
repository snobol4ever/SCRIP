#define ICN_STACK_MAX 256
static long icn_stack[ICN_STACK_MAX];
static int  icn_sp = 0;
long icn_retval = 0;
int  icn_failed = 0;
static char subscript_buf[2];
extern void rt_icn_cset_register(const char *ptr, int len);
extern const unsigned char *kw_cset_bits(const char *ptr);
extern const char *kw_cset_intern(const char *canon, int len);
extern int kw_cset_len(const char *ptr);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cset_bits_of(const char *s, int len, unsigned char w[32]) {
    const unsigned char *kb = s ? kw_cset_bits(s) : (const unsigned char *)0;
    if (kb && kw_cset_len(s) == len) { memcpy(w, kb, 32); return; }
    memset(w, 0, 32);
    if (s) for (int i = 0; i < len; i++) { unsigned c = (unsigned char)s[i]; w[c >> 3] |= (unsigned char)(1u << (c & 7)); }
}
static const char *cset_from_bits(const unsigned char w[32], int *outlen) {
    char buf[257]; int n = 0;
    for (int by = 0; by < 32; by++) { unsigned char m = w[by]; if (!m) continue; for (int bit = 0; bit < 8; bit++) if (m & (unsigned char)(1u << bit)) buf[n++] = (char)(by * 8 + bit); }
    buf[n] = '\0';
    if (outlen) *outlen = n;
    return kw_cset_intern(buf, n);
}
static int cset_is_canonical(const char *s, int len) {
    for (int i = 1; i < len; i++) if ((unsigned char)s[i] <= (unsigned char)s[i - 1]) return 0;
    return 1;
}
const char *cset_union(const char *a, int alen, const char *b, int blen, int *outlen) {
    unsigned char x[32], y[32]; cset_bits_of(a, alen, x); cset_bits_of(b, blen, y);
    for (int i = 0; i < 32; i++) x[i] |= y[i];
    return cset_from_bits(x, outlen);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *cset_diff(const char *a, int alen, const char *b, int blen, int *outlen) {
    unsigned char x[32], y[32]; cset_bits_of(a, alen, x); cset_bits_of(b, blen, y);
    for (int i = 0; i < 32; i++) x[i] &= (unsigned char)~y[i];
    return cset_from_bits(x, outlen);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *cset_inter(const char *a, int alen, const char *b, int blen, int *outlen) {
    unsigned char x[32], y[32]; cset_bits_of(a, alen, x); cset_bits_of(b, blen, y);
    for (int i = 0; i < 32; i++) x[i] &= y[i];
    return cset_from_bits(x, outlen);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *cset_canonical(const char *cs, int len) {
    if (!cs || len <= 0) return "";
    if (cset_is_canonical(cs, len) && kw_cset_bits(cs) && kw_cset_len(cs) == len) return cs;
    unsigned char x[32]; cset_bits_of(cs, len, x);
    return cset_from_bits(x, (int *)0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_icn_cset_member(const char *needle, int ch) {
    if (!needle) return 0;
    extern const unsigned char *kw_cset_bits(const char *);
    { const unsigned char *b = kw_cset_bits(needle); if (b) { unsigned c = (unsigned char)ch; return (b[c >> 3] >> (c & 7)) & 1; } }
    extern int kw_cset_len(const char *);
    int kn = kw_cset_len(needle);
    int len = (kn >= 0) ? kn : (int) strlen(needle);
    return memchr(needle, (unsigned char)ch, (unsigned long)len) != 0;
}
