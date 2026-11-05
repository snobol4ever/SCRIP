/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "ir/ct_arena.h"
#include "ir/ct_vec.h"
#include "driver/zmq_abi.h"
#include "driver/kernel.h"
typedef struct { uint32_t h[8]; uint64_t len; unsigned char buf[64]; uint32_t n; } sha256_t;
typedef struct jn { int t; const char *s; uint32_t sl; double num; cv_t kids; cv_t keys; } jn_t;
typedef struct { const char *p; uint32_t n; } kpart_t;
typedef struct { cv_t ids; kpart_t raw[5]; jn_t *header, *parent, *metadata, *content; } kmsg_t;
typedef struct { void *ctx, *shell, *control, *stdin_s, *iopub, *hb; const unsigned char *key; size_t keylen; char session[33]; long count; pid_t child; int quit; } kctx_t;
typedef struct { const char *tag, *ext; int unicon; } klang_t;
static const klang_t k_langs[] = { { "snobol4", ".sno", 0 }, { "snobol", ".sno", 0 }, { "spitbol", ".spt", 0 }, { "snocone", ".sc", 0 }, { "rebus", ".reb", 0 }, { "icon", ".icn", 0 }, { "unicon",
    ".icn", 1 }, { "prolog", ".pl", 0 }, { "pascal", ".pas", 0 }, { "raku", ".raku", 0 }, { "scrip", ".md", 0 }, { "polyglot", ".md", 0 }, { "md", ".md", 0 }, { "markdown", ".md", 0 } };
static const uint32_t k_sha[64] = { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74,
    0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3,
    0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819,
    0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa,
    0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };
static const uint32_t k_sha_h0[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint32_t k_rotr(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_sha_block(sha256_t *s, const unsigned char *p) {
    uint32_t w[64], a = s->h[0], b = s->h[1], c = s->h[2], d = s->h[3], e = s->h[4], f = s->h[5], g = s->h[6], h = s->h[7];
    for (int i = 0; i < 16; i++) w[i] = ((uint32_t)p[4 * i] << 24) | ((uint32_t)p[4 * i + 1] << 16) | ((uint32_t)p[4 * i + 2] << 8) | (uint32_t)p[4 * i + 3];
    for (int i = 16; i < 64; i++) w[i] = w[i - 16] + (k_rotr(w[i - 15], 7) ^ k_rotr(w[i - 15], 18) ^ (w[i - 15] >> 3)) + w[i - 7] + (k_rotr(w[i - 2], 17) ^ k_rotr(w[i - 2], 19) ^ (w[i - 2] >> 10));
    for (int i = 0; i < 64; i++) {
        uint32_t t1 = h + (k_rotr(e, 6) ^ k_rotr(e, 11) ^ k_rotr(e, 25)) + ((e & f) ^ (~e & g)) + k_sha[i] + w[i];
        uint32_t t2 = (k_rotr(a, 2) ^ k_rotr(a, 13) ^ k_rotr(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    s->h[0] += a;
    s->h[1] += b;
    s->h[2] += c;
    s->h[3] += d;
    s->h[4] += e;
    s->h[5] += f;
    s->h[6] += g;
    s->h[7] += h;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_sha_init(sha256_t *s) { memcpy(s->h, k_sha_h0, sizeof s->h); s->len = 0; s->n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_sha_update(sha256_t *s, const void *d, size_t n) {
    const unsigned char *p = (const unsigned char *)d;
    s->len += n;
    while (n > 0) { uint32_t k = 64 - s->n; if (k > n) k = (uint32_t)n; memcpy(s->buf + s->n, p, k); s->n += k; p += k; n -= k; if (s->n == 64) { k_sha_block(s, s->buf); s->n = 0; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_sha_final(sha256_t *s, unsigned char out[32]) {
    uint64_t bits = s->len * 8;
    unsigned char pad = 0x80, z = 0, L[8];
    k_sha_update(s, &pad, 1);
    while (s->n != 56) k_sha_update(s, &z, 1);
    for (int i = 0; i < 8; i++) L[i] = (unsigned char)(bits >> (56 - 8 * i));
    k_sha_update(s, L, 8);
    for (int i = 0; i < 8; i++) {
        out[4 * i] = (unsigned char)(s->h[i] >> 24);
        out[4 * i + 1] = (unsigned char)(s->h[i] >> 16);
        out[4 * i + 2] = (unsigned char)(s->h[i] >> 8);
        out[4 * i + 3] = (unsigned char)s->h[i];
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_hmac(const unsigned char *key, size_t klen, const kpart_t *parts, int np, unsigned char out[32]) {
    unsigned char k[64], ipad[64], opad[64], inner[32];
    sha256_t s;
    memset(k, 0, 64);
    if (klen > 64) { k_sha_init(&s); k_sha_update(&s, key, klen); k_sha_final(&s, k); } else memcpy(k, key, klen);
    for (int i = 0; i < 64; i++) { ipad[i] = k[i] ^ 0x36; opad[i] = k[i] ^ 0x5c; }
    k_sha_init(&s);
    k_sha_update(&s, ipad, 64);
    for (int i = 0; i < np; i++) k_sha_update(&s, parts[i].p, parts[i].n);
    k_sha_final(&s, inner);
    k_sha_init(&s);
    k_sha_update(&s, opad, 64);
    k_sha_update(&s, inner, 32);
    k_sha_final(&s, out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_hex(const unsigned char *in, size_t n, char *out) {
    for (size_t i = 0; i < n; i++) { out[2 * i] = "0123456789abcdef"[in[i] >> 4]; out[2 * i + 1] = "0123456789abcdef"[in[i] & 15]; }
    out[2 * n] = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_uuid(char out[33]) {
    unsigned char r[16];
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0 || read(fd, r, 16) != 16) { for (int i = 0; i < 16; i++) r[i] = (unsigned char)(getpid() * 131 + (int)time(0) * 7 + i * 37); }
    if (fd >= 0) close(fd);
    k_hex(r, 16, out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cv_putc(cv_t *b, char c) { CV_PUSH(*b, char) = c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cv_puts(cv_t *b, const char *s, size_t n) { cv_reserve(b, 1, (uint64_t)b->len + n + 1, "kernel_bytes"); memcpy((char *)b->p + b->len, s, n); b->len += (uint32_t)n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *cv_cstr(cv_t *b) { cv_reserve(b, 1, (uint64_t)b->len + 1, "kernel_bytes"); ((char *)b->p)[b->len] = 0; return (const char *)b->p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cv_utf8(cv_t *b, uint32_t cp) {
    if (cp < 0x80) cv_putc(b, (char)cp);
    else if (cp < 0x800) {
        cv_putc(b, (char)(0xC0 | (cp >> 6)));
        cv_putc(b, (char)(0x80 | (cp & 63)));
    } else if (cp < 0x10000) {
        cv_putc(b, (char)(0xE0 | (cp >> 12)));
        cv_putc(b, (char)(0x80 | ((cp >> 6) & 63)));
        cv_putc(b, (char)(0x80 | (cp & 63)));
    } else {
        cv_putc(b, (char)(0xF0 | (cp >> 18)));
        cv_putc(b, (char)(0x80 | ((cp >> 12) & 63)));
        cv_putc(b, (char)(0x80 | ((cp >> 6) & 63)));
        cv_putc(b, (char)(0x80 | (cp & 63)));
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static jn_t *jn_new(int t) { jn_t *n = (jn_t *)ct_zalloc(1, sizeof *n); n->t = t; return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *js_ws(const char *p, const char *e) { while (p < e && (*p == ' ' || *p == '\n' || *p == '\r' || *p == '\t')) p++; return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int js_hex(char c) { return c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c >= 'A' && c <= 'F' ? c - 'A' + 10 : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int js_hex4(const char *p, const char *e, uint32_t *cp) {
    if (e - p < 4) return 0;
    *cp = 0;
    for (int i = 0; i < 4; i++) { int h = js_hex(p[i]); if (h < 0) return 0; *cp = *cp * 16 + (uint32_t)h; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *js_string(const char *p, const char *e, jn_t **out) {
    cv_t b = { 0, 0, 0, 0 };
    p++;
    while (p < e && *p != '"') {
        if (*p != '\\') { cv_putc(&b, *p++); continue; }
        if (++p >= e) return 0;
        char c = *p++;
        if (c == 'n') cv_putc(&b, '\n');
        else if (c == 't') cv_putc(&b, '\t');
        else if (c == 'r') cv_putc(&b, '\r');
        else if (c == 'b') cv_putc(&b, '\b');
        else if (c == 'f') cv_putc(&b, '\f');
        else if (c == 'u') {
            uint32_t cp, lo;
            if (!js_hex4(p, e, &cp)) return 0;
            p += 4;
            if (cp >= 0xD800 && cp < 0xDC00 && e - p >= 6 && p[0] == '\\' && p[1] == 'u' && js_hex4(p + 2, e, &lo)) { p += 6; cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00); }
            cv_utf8(&b, cp);
        } else cv_putc(&b, c);
    }
    if (p >= e) return 0;
    jn_t *n = jn_new(4);
    n->sl = b.len;
    n->s = cv_cstr(&b);
    *out = n;
    return p + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *js_value(const char *p, const char *e, jn_t **out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *js_object(const char *p, const char *e, jn_t **out) {
    jn_t *n = jn_new(6);
    p = js_ws(p + 1, e);
    if (p < e && *p == '}') { *out = n; return p + 1; }
    for (;;) {
        jn_t *k = 0, *v = 0;
        p = js_ws(p, e);
        if (p >= e || *p != '"') return 0;
        p = js_string(p, e, &k);
        if (!p) return 0;
        p = js_ws(p, e);
        if (p >= e || *p != ':') return 0;
        p = js_value(p + 1, e, &v);
        if (!p) return 0;
        CV_PUSH(n->keys, jn_t *) = k;
        CV_PUSH(n->kids, jn_t *) = v;
        p = js_ws(p, e);
        if (p >= e) return 0;
        if (*p == ',') { p++; continue; }
        if (*p == '}') { *out = n; return p + 1; }
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *js_array(const char *p, const char *e, jn_t **out) {
    jn_t *n = jn_new(5);
    p = js_ws(p + 1, e);
    if (p < e && *p == ']') { *out = n; return p + 1; }
    for (;;) {
        jn_t *v = 0;
        p = js_value(p, e, &v);
        if (!p) return 0;
        CV_PUSH(n->kids, jn_t *) = v;
        p = js_ws(p, e);
        if (p >= e) return 0;
        if (*p == ',') { p++; continue; }
        if (*p == ']') { *out = n; return p + 1; }
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int js_numch(char c) { return (c >= '0' && c <= '9') || c == '-' || c == '+' || c == '.' || c == 'e' || c == 'E'; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *js_value(const char *p, const char *e, jn_t **out) {
    p = js_ws(p, e);
    if (p >= e) return 0;
    if (*p == '"') return js_string(p, e, out);
    if (*p == '{') return js_object(p, e, out);
    if (*p == '[') return js_array(p, e, out);
    if (e - p >= 4 && !memcmp(p, "null", 4)) { *out = jn_new(0); return p + 4; }
    if (e - p >= 4 && !memcmp(p, "true", 4)) { *out = jn_new(2); return p + 4; }
    if (e - p >= 5 && !memcmp(p, "false", 5)) { *out = jn_new(1); return p + 5; }
    const char *q = p;
    while (q < e && js_numch(*q)) q++;
    if (q == p) return 0;
    jn_t *n = jn_new(3);
    char *s = (char *)ct_alloc((size_t)(q - p) + 1);
    memcpy(s, p, (size_t)(q - p));
    s[q - p] = 0;
    n->s = s;
    n->sl = (uint32_t)(q - p);
    n->num = strtod(s, 0);
    *out = n;
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static jn_t *jn_parse(const char *p, size_t n) { jn_t *v = 0; const char *r = js_value(p, p + n, &v); return r ? v : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static jn_t *jn_get(const jn_t *o, const char *key) {
    if (!o || o->t != 6) return 0;
    for (uint32_t i = 0; i < o->kids.len; i++) if (!strcmp(CV_AT(o->keys, jn_t *, i)->s, key)) return CV_AT(o->kids, jn_t *, i);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *jn_str(const jn_t *o, const char *key, const char *dflt) { jn_t *v = jn_get(o, key); return v && v->t == 4 ? v->s : dflt; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int jn_bool(const jn_t *o, const char *key, int dflt) { jn_t *v = jn_get(o, key); return v ? (v->t == 2 ? 1 : v->t == 1 ? 0 : dflt) : dflt; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long jn_int(const jn_t *o, const char *key, long dflt) { jn_t *v = jn_get(o, key); return v && v->t == 3 ? (long)v->num : dflt; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int k_utf8_len(const unsigned char *s, size_t n) {
    int k = s[0] < 0x80 ? 1 : (s[0] & 0xE0) == 0xC0 ? 2 : (s[0] & 0xF0) == 0xE0 ? 3 : (s[0] & 0xF8) == 0xF0 ? 4 : 0;
    if (k == 0 || (size_t)k > n) return 0;
    for (int i = 1; i < k; i++) if ((s[i] & 0xC0) != 0x80) return 0;
    return k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void jw_raw(cv_t *b, const char *s) { cv_puts(b, s, strlen(s)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void jw_str(cv_t *b, const char *s, size_t n) {
    cv_putc(b, '"');
    for (size_t i = 0; i < n;) {
        unsigned char c = (unsigned char)s[i];
        if (c == '"' || c == '\\') {
            cv_putc(b, '\\');
            cv_putc(b, (char)c);
            i++;
        } else if (c == '\n') {
            jw_raw(b, "\\n");
            i++;
        } else if (c == '\t') {
            jw_raw(b, "\\t");
            i++;
        } else if (c == '\r') {
            jw_raw(b, "\\r");
            i++;
        } else if (c < 0x20) {
            char t[8];
            snprintf(t, sizeof t, "\\u%04x", c);
            jw_raw(b, t);
            i++;
        } else if (c < 0x80) {
            cv_putc(b, (char)c);
            i++;
        } else {
            int k = k_utf8_len((const unsigned char *)s + i, n - i);
            if (k) { cv_puts(b, s + i, (size_t)k); i += (size_t)k; } else { jw_raw(b, "\\ufffd"); i++; }
        }
    }
    cv_putc(b, '"');
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void jw_key(cv_t *b, const char *k, int comma) { if (comma) cv_putc(b, ','); jw_str(b, k, strlen(k)); cv_putc(b, ':'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void jw_kv(cv_t *b, const char *k, const char *v, int comma) { jw_key(b, k, comma); jw_str(b, v, strlen(v)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_frame(void *sock, const void *d, size_t n, int more) { zmq_msg_t m; zmq_msg_init_size(&m, n); memcpy(zmq_msg_data(&m), d, n); zmq_msg_send(&m, sock, more ? ZMQ_SNDMORE : 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_send(kctx_t *k, void *sock, const kmsg_t *req, const char *type, const char *content) {
    cv_t h = { 0, 0, 0, 0 };
    char id[33], date[40], sig[65];
    time_t now = time(0);
    struct tm tmv;
    gmtime_r(&now, &tmv);
    strftime(date, sizeof date, "%Y-%m-%dT%H:%M:%S.000000Z", &tmv);
    k_uuid(id);
    jw_raw(&h, "{");
    jw_kv(&h, "msg_id", id, 0);
    jw_kv(&h, "session", k->session, 1);
    jw_kv(&h, "username", "kernel", 1);
    jw_kv(&h, "date", date, 1);
    jw_kv(&h, "msg_type", type, 1);
    jw_kv(&h, "version", "5.3", 1);
    jw_raw(&h, "}");
    kpart_t parts[4] = { { (const char *)h.p, h.len }, { req && req->raw[1].p ? req->raw[1].p : "{}", req && req->raw[1].p ? req->raw[1].n : 2 }, { "{}", 2 }, { content, (uint32_t)strlen(content) } };
    sig[0] = 0;
    if (k->keylen) { unsigned char d[32]; k_hmac(k->key, k->keylen, parts, 4, d); k_hex(d, 32, sig); }
    if (sock == k->iopub) k_frame(sock, type, strlen(type), 1);
    else if (req) for (uint32_t i = 0; i < req->ids.len; i++) k_frame(sock, CV_AT(req->ids, kpart_t, i).p, CV_AT(req->ids, kpart_t, i).n, 1);
    k_frame(sock, "<IDS|MSG>", 9, 1);
    k_frame(sock, sig, strlen(sig), 1);
    for (int i = 0; i < 4; i++) k_frame(sock, parts[i].p, parts[i].n, i < 3);
    ct_drop(h.p);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int k_recv(kctx_t *k, void *sock, kmsg_t *m) {
    int phase = 0, np = 0;
    memset(m, 0, sizeof *m);
    for (;;) {
        zmq_msg_t z;
        zmq_msg_init(&z);
        if (zmq_msg_recv(&z, sock, 0) < 0) { zmq_msg_close(&z); return 0; }
        size_t n = zmq_msg_size(&z);
        const char *d = (const char *)zmq_msg_data(&z);
        int more = zmq_msg_more(&z);
        if (phase == 0 && n == 9 && !memcmp(d, "<IDS|MSG>", 9)) phase = 1;
        else if (phase == 0 || np < 5) {
            char *c = (char *)ct_alloc(n + 1);
            memcpy(c, d, n);
            c[n] = 0;
            if (phase == 0) { kpart_t *f = &CV_PUSH(m->ids, kpart_t); f->p = c; f->n = (uint32_t)n; } else { m->raw[np].p = c; m->raw[np].n = (uint32_t)n; np++; }
        }
        zmq_msg_close(&z);
        if (!more) break;
    }
    if (phase == 0 || np < 5) return 0;
    if (k->keylen) { unsigned char d[32]; char sig[65]; k_hmac(k->key, k->keylen, m->raw + 1, 4, d); k_hex(d, 32, sig); if (m->raw[0].n != 64 || memcmp(sig, m->raw[0].p, 64)) return 0; }
    m->header = jn_parse(m->raw[1].p, m->raw[1].n);
    m->parent = jn_parse(m->raw[2].p, m->raw[2].n);
    m->metadata = jn_parse(m->raw[3].p, m->raw[3].n);
    m->content = jn_parse(m->raw[4].p, m->raw[4].n);
    return m->header != 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_status(kctx_t *k, const kmsg_t *req, const char *st) {
    cv_t c = { 0, 0, 0, 0 };
    jw_raw(&c, "{");
    jw_kv(&c, "execution_state", st, 0);
    jw_raw(&c, "}");
    k_send(k, k->iopub, req, "status", cv_cstr(&c));
    ct_drop(c.p);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_stream(kctx_t *k, const kmsg_t *req, const char *name, cv_t *text) {
    if (text->len == 0) return;
    cv_t c = { 0, 0, 0, 0 };
    jw_raw(&c, "{");
    jw_kv(&c, "name", name, 0);
    jw_key(&c, "text", 1);
    jw_str(&c, (const char *)text->p, text->len);
    jw_raw(&c, "}");
    k_send(k, k->iopub, req, "stream", cv_cstr(&c));
    ct_drop(c.p);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_reply(kctx_t *k, void *sock, const kmsg_t *req, const char *content) {
    const char *t = jn_str(req->header, "msg_type", "");
    size_t n = strlen(t);
    if (n < 8 || strcmp(t + n - 8, "_request")) return;
    char *r = (char *)ct_alloc(n + 8);
    memcpy(r, t, n - 8);
    strcpy(r + n - 8, "_reply");
    k_send(k, sock, req, r, content);
    ct_drop(r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_control_tick(kctx_t *k);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int k_run_cell(kctx_t *k, const char *code, size_t clen, const char *lang, cv_t *out, cv_t *err) {
    const klang_t *L = 0;
    for (size_t i = 0; i < sizeof k_langs / sizeof *k_langs; i++) if (!strcasecmp(k_langs[i].tag, lang)) L = &k_langs[i];
    if (!L) { jw_raw(err, "scrip: unknown language tag "); jw_raw(err, lang); jw_raw(err, " (SNOBOL4 Snocone Rebus Icon Unicon Prolog Pascal Raku scrip)\n"); return -1; }
    char dir[] = "/tmp/scrip-cell-XXXXXX";
    if (!mkdtemp(dir)) { jw_raw(err, "scrip: no temp dir\n"); return -1; }
    size_t pl = strlen(dir) + 6 + strlen(L->ext);
    char *path = (char *)ct_alloc(pl);
    snprintf(path, pl, "%s/cell%s", dir, L->ext);
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600), po[2], pe[2], st = 0, nopen = 2;
    if (fd < 0 || (clen && write(fd, code, clen) != (ssize_t)clen) || (!(clen && code[clen - 1] == '\n') && write(fd, "\n", 1) != 1)) { jw_raw(err, "scrip: cannot write the cell\n"); return -1; }
    close(fd);
    if (pipe(po) || pipe(pe)) { jw_raw(err, "scrip: no pipe\n"); return -1; }
    pid_t pid = fork();
    if (pid == 0) {
        int dn = open("/dev/null", O_RDONLY);
        const char *av[4];
        int n = 0;
        dup2(dn, 0);
        dup2(po[1], 1);
        dup2(pe[1], 2);
        close(po[0]);
        close(pe[0]);
        if (chdir(dir)) _exit(126);
        av[n++] = "scrip";
        if (L->unicon) av[n++] = "--dialect=unicon";
        av[n++] = path;
        av[n] = 0;
        execv("/proc/self/exe", (char *const *)av);
        _exit(127);
    }
    close(po[1]);
    close(pe[1]);
    k->child = pid;
    zmq_pollitem_t it[3] = { { 0, po[0], ZMQ_POLLIN, 0 }, { 0, pe[0], ZMQ_POLLIN, 0 }, { k->control, 0, ZMQ_POLLIN, 0 } };
    char buf[4096];
    while (nopen > 0 && pid > 0) {
        if (zmq_poll(it, 3, -1) < 0) { if (errno == EINTR) continue; break; }
        for (int i = 0; i < 2; i++) if (it[i].fd >= 0 && it[i].revents) {
            ssize_t r = read(it[i].fd, buf, sizeof buf);
            if (r <= 0) { close(it[i].fd); it[i].fd = -1; it[i].events = 0; nopen--; } else cv_puts(i == 0 ? out : err, buf, (size_t)r);
        }
        if (it[2].revents & ZMQ_POLLIN) k_control_tick(k);
    }
    waitpid(pid, &st, 0);
    k->child = 0;
    if (it[0].fd >= 0) close(it[0].fd);
    if (it[1].fd >= 0) close(it[1].fd);
    unlink(path);
    rmdir(dir);
    ct_drop(path);
    return pid < 0 ? -1 : WIFEXITED(st) ? WEXITSTATUS(st) : 128 + WTERMSIG(st);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_execute(kctx_t *k, void *sock, const kmsg_t *req) {
    jn_t *cj = jn_get(req->content, "code");
    const char *code = cj && cj->t == 4 ? cj->s : "", *lang = jn_str(jn_get(req->metadata, "scrip"), "language", 0);
    size_t clen = cj && cj->t == 4 ? cj->sl : 0;
    int silent = jn_bool(req->content, "silent", 0);
    cv_t c = { 0, 0, 0, 0 }, out = { 0, 0, 0, 0 }, err = { 0, 0, 0, 0 };
    char num[32];
    if (!silent) k->count++;
    snprintf(num, sizeof num, "%ld", k->count);
    jw_raw(&c, "{");
    jw_key(&c, "code", 0);
    jw_str(&c, code, clen);
    jw_key(&c, "execution_count", 1);
    jw_raw(&c, num);
    jw_raw(&c, "}");
    k_send(k, k->iopub, req, "execute_input", cv_cstr(&c));
    c.len = 0;
    if (clen >= 2 && code[0] == '%' && code[1] == '%') {
        size_t i = 2, j;
        while (i < clen && code[i] == ' ') i++;
        j = i;
        while (j < clen && code[j] != '\n' && code[j] != ' ' && code[j] != '\r') j++;
        char *tag = (char *)ct_alloc(j - i + 1);
        memcpy(tag, code + i, j - i);
        tag[j - i] = 0;
        lang = tag;
        while (j < clen && code[j] != '\n') j++;
        if (j < clen) j++;
        code += j;
        clen -= j;
    }
    if (!lang) lang = getenv("SCRIP_DEFAULT_LANG") ? getenv("SCRIP_DEFAULT_LANG") : "SNOBOL4";
    int rc = k_run_cell(k, code, clen, lang, &out, &err);
    if (!silent) { k_stream(k, req, "stdout", &out); k_stream(k, req, "stderr", &err); }
    if (rc == 0) {
        jw_raw(&c, "{");
        jw_kv(&c, "status", "ok", 0);
        jw_key(&c, "execution_count", 1);
        jw_raw(&c, num);
        jw_raw(&c, ",\"payload\":[],\"user_expressions\":{}}");
    } else {
        char ev[96];
        snprintf(ev, sizeof ev, "%s cell exited %d%s", lang, rc, rc == 124 ? " (timeout)" : rc == 130 || rc == 143 ? " (interrupted)" : "");
        cv_t e = { 0, 0, 0, 0 };
        jw_raw(&e, "{");
        jw_kv(&e, "ename", "scrip", 0);
        jw_kv(&e, "evalue", ev, 1);
        jw_key(&e, "traceback", 1);
        jw_raw(&e, "[");
        jw_str(&e, ev, strlen(ev));
        jw_raw(&e, "]}");
        if (!silent) k_send(k, k->iopub, req, "error", cv_cstr(&e));
        jw_raw(&c, "{");
        jw_kv(&c, "status", "error", 0);
        jw_kv(&c, "ename", "scrip", 1);
        jw_kv(&c, "evalue", ev, 1);
        jw_key(&c, "traceback", 1);
        jw_raw(&c, "[");
        jw_str(&c, ev, strlen(ev));
        jw_raw(&c, "]");
        jw_key(&c, "execution_count", 1);
        jw_raw(&c, num);
        jw_raw(&c, "}");
        ct_drop(e.p);
    }
    k_reply(k, sock, req, cv_cstr(&c));
    ct_drop(c.p);
    ct_drop(out.p);
    ct_drop(err.p);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_dispatch(kctx_t *k, void *sock, const kmsg_t *req) {
    const char *t = jn_str(req->header, "msg_type", "");
    cv_t c = { 0, 0, 0, 0 };
    k_status(k, req, "busy");
    if (!strcmp(t, "execute_request")) k_execute(k, sock, req);
    else if (!strcmp(t, "kernel_info_request")) {
        jw_raw(&c,
            "{\"status\":\"ok\",\"protocol_version\":\"5.3\",\"implementation\":\"SCRIP\",\"implementation_version\":\"0.2\",\"language_info\":{\"name\":\"scrip\",\"version\":\"0.2\",\"mimetype\":\""
            "text/plain\",");
        jw_raw(&c, "\"file_extension\":\".md\",\"pygments_lexer\":\"text\",\"codemirror_mode\":\"text\"},\"banner\":\"SCRIP kernel: SNOBOL4, Snocone, Rebus, Icon, Unicon, Prolog, Pascal, Raku. ");
        jw_raw(&c, "Tag a cell with metadata scrip.language or a first line %%Prolog.\",\"help_links\":[]}");
        k_reply(k, sock, req, cv_cstr(&c));
    } else if (!strcmp(t, "is_complete_request")) k_reply(k, sock, req, "{\"status\":\"complete\"}");
    else if (!strcmp(t, "complete_request")) {
        char b[128];
        long p = jn_int(req->content, "cursor_pos", 0);
        snprintf(b, sizeof b, "{\"status\":\"ok\",\"matches\":[],\"cursor_start\":%ld,\"cursor_end\":%ld,\"metadata\":{}}", p, p);
        k_reply(k, sock, req, b);
    } else if (!strcmp(t, "inspect_request")) k_reply(k, sock, req, "{\"status\":\"ok\",\"found\":false,\"data\":{},\"metadata\":{}}");
    else if (!strcmp(t, "history_request")) k_reply(k, sock, req, "{\"status\":\"ok\",\"history\":[]}");
    else if (!strcmp(t, "comm_info_request")) k_reply(k, sock, req, "{\"status\":\"ok\",\"comms\":{}}");
    else if (!strcmp(t, "interrupt_request")) {
        if (k->child > 0) kill(k->child, SIGINT);
        k_reply(k, sock, req, "{\"status\":\"ok\"}");
    } else if (!strcmp(t, "shutdown_request")) {
        if (k->child > 0) kill(k->child, SIGTERM);
        k_reply(k, sock, req, jn_bool(req->content, "restart", 0) ? "{\"status\":\"ok\",\"restart\":true}" : "{\"status\":\"ok\",\"restart\":false}");
        k->quit = 1;
    }
    k_status(k, req, "idle");
    ct_drop(c.p);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_control_tick(kctx_t *k) { kmsg_t m; if (k_recv(k, k->control, &m)) k_dispatch(k, k->control, &m); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void k_heartbeat(void *hb) {
    for (;;) {
        zmq_msg_t z;
        zmq_msg_init(&z);
        if (zmq_msg_recv(&z, hb, 0) < 0) { zmq_msg_close(&z); return; }
        int more = zmq_msg_more(&z);
        zmq_msg_send(&z, hb, more ? ZMQ_SNDMORE : 0);
        if (!more) return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *k_bind(kctx_t *k, int type, const char *transport, const char *ip, long port) {
    void *s = zmq_socket(k->ctx, type);
    int linger = 0;
    size_t n = strlen(transport) + strlen(ip) + 32;
    char *addr = (char *)ct_alloc(n);
    snprintf(addr, n, "%s://%s:%ld", transport, ip, port);
    zmq_setsockopt(s, ZMQ_LINGER, &linger, sizeof linger);
    if (zmq_bind(s, addr)) { fprintf(stderr, "scrip --kernel: cannot bind %s: %s\n", addr, strerror(errno)); _exit(3); }
    ct_drop(addr);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int scrip_kernel_main(const char *connection_file) {
    kctx_t k;
    struct stat sb;
    int fd = open(connection_file, O_RDONLY);
    if (fd < 0 || fstat(fd, &sb)) { fprintf(stderr, "scrip --kernel: cannot read %s\n", connection_file); return 2; }
    char *txt = (char *)ct_alloc((size_t)sb.st_size + 1);
    ssize_t got = read(fd, txt, (size_t)sb.st_size);
    close(fd);
    jn_t *cf = got >= 0 ? jn_parse(txt, (size_t)got) : 0;
    if (!cf) { fprintf(stderr, "scrip --kernel: %s is not a connection file\n", connection_file); return 2; }
    const char *ip = jn_str(cf, "ip", "127.0.0.1"), *tr = jn_str(cf, "transport", "tcp"), *key = jn_str(cf, "key", ""), *scheme = jn_str(cf, "signature_scheme", "hmac-sha256");
    if (key[0] && strcmp(scheme, "hmac-sha256")) { fprintf(stderr, "scrip --kernel: signature scheme %s is not hmac-sha256\n", scheme); return 2; }
    memset(&k, 0, sizeof k);
    k.key = (const unsigned char *)key;
    k.keylen = strlen(key);
    k_uuid(k.session);
    k.ctx = zmq_ctx_new();
    k.shell = k_bind(&k, ZMQ_ROUTER, tr, ip, jn_int(cf, "shell_port", 0));
    k.control = k_bind(&k, ZMQ_ROUTER, tr, ip, jn_int(cf, "control_port", 0));
    k.stdin_s = k_bind(&k, ZMQ_ROUTER, tr, ip, jn_int(cf, "stdin_port", 0));
    k.iopub = k_bind(&k, ZMQ_PUB, tr, ip, jn_int(cf, "iopub_port", 0));
    k.hb = k_bind(&k, ZMQ_REP, tr, ip, jn_int(cf, "hb_port", 0));
    k_status(&k, 0, "starting");
    while (!k.quit) {
        zmq_pollitem_t it[3] = { { k.shell, 0, ZMQ_POLLIN, 0 }, { k.control, 0, ZMQ_POLLIN, 0 }, { k.hb, 0, ZMQ_POLLIN, 0 } };
        if (zmq_poll(it, 3, -1) < 0) { if (errno == EINTR) continue; break; }
        if (it[2].revents & ZMQ_POLLIN) k_heartbeat(k.hb);
        if (it[1].revents & ZMQ_POLLIN) k_control_tick(&k);
        if (it[0].revents & ZMQ_POLLIN) { kmsg_t m; if (k_recv(&k, k.shell, &m)) k_dispatch(&k, k.shell, &m); }
    }
    zmq_close(k.shell);
    zmq_close(k.control);
    zmq_close(k.stdin_s);
    zmq_close(k.iopub);
    zmq_close(k.hb);
    zmq_ctx_term(k.ctx);
    return 0;
}
