#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "templates/x86/x86_rsp_tracker.h"
#include "emitter/emit_medium.h"
#include "emitter/emit_state_.h"
#include "templates/x86/x86_frame.h"
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_zeta.h"
static int x86_rsp_trace_on(void);
static std::string x86_rsp_release_to(int disp);
static int x86_rsp_trace_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_GC_RSP_TRACE"); v = (e && *e == '1') ? 1 : 0; } return v; }
x86_rsp_st_t & x86_rsp_S(void) { static x86_rsp_st_t v; return v; }
int & x86_rsp_state(int k) { x86_rsp_st_t & S = x86_rsp_S(); return k == 0 ? S.delta : k == 1 ? S.before : k == 2 ? S.aligned : S.unknown; }
void x86_rsp_reset_all(void) { x86_rsp_st_t & S = x86_rsp_S(); S.delta = 0; S.before = 0; S.aligned = 0; S.unknown = 0; S.dead = 0; S.j.clear(); S.p.clear(); S.jp_done = 0; S.quiet = 0;
    S.mark9_set = 0; }
void x86_rsp_jump(int kind, uint64_t key) { x86_rsp_st_t & S = x86_rsp_S(); if (S.aligned || S.unknown) return;
    const x86_rsp_snap_t * b0 = S.j.data(), * q = b0 + S.j.size(); while (q != b0) { q--; if (q->key == key && q->kind == kind) return; }
    S.j.push_back(x86_rsp_snap_t{ key, kind, S.delta }); }
void x86_rsp_define(int kind, uint64_t key) { x86_rsp_st_t & S = x86_rsp_S();
    if (S.aligned) { S.dead = 0; return; }
    const x86_rsp_snap_t * fj = 0, * b0 = S.j.data(), * q = b0 + S.j.size(); while (q != b0) { q--; if (q->key == key && q->kind == kind) { fj = q; break; } }
    if (S.dead && !fj) { const x86_rsp_snap_t * p0 = S.p.data(), * r = p0 + S.p.size();
        while (r != p0) { r--; if (r->key == key && r->kind == kind) { S.delta = r->delta; S.unknown = 0; S.dead = 0; return; } } }
    if (S.dead) { if (fj) { S.delta = fj->delta; S.unknown = 0; } else { S.unknown = 1;
        if (!S.quiet && x86_rsp_trace_on())
        fprintf(stderr, "[RSP-TRACE] %s dead define kind=%d key=%llu no jump recorded (delta %d)\n", _.x86_uid_kind ? _.x86_uid_kind : "?", kind, (unsigned long long)key, S.delta); } }
    else if (fj && fj->delta != S.delta) { S.unknown = 1;
        if (!S.quiet && x86_rsp_trace_on())
        fprintf(stderr, "[RSP-TRACE] %s live define kind=%d key=%llu jump delta %d vs fallthrough %d\n", _.x86_uid_kind ? _.x86_uid_kind : "?", kind, (unsigned long long)key, fj->delta,
        S.delta); }
    S.dead = 0; }
x86_rsp_pre_t x86_rsp_prepass_open(void) { x86_rsp_st_t & S = x86_rsp_S();
    x86_rsp_pre_t b = { S.delta, S.before, S.aligned, S.unknown, S.dead, S.mark, S.mark_unknown, S.quiet, S.mark9, S.mark9_unknown, S.mark9_set, S.j.size() }; S.quiet = 1; return b; }
void x86_rsp_prepass_commit(const x86_rsp_pre_t & b) { x86_rsp_st_t & S = x86_rsp_S(); size_t n = S.j.size();
    for (size_t i = S.jp_done < b.jn ? S.jp_done : b.jn; i < n; i++) { const x86_rsp_snap_t a = S.j.data()[i]; const x86_rsp_snap_t * q = S.p.data(); size_t m = S.p.size(); int have = 0;
        for (const x86_rsp_snap_t * r = q + m; r != q;) { r--; if (r->key == a.key && r->kind == a.kind) { have = 1; break; } }
        if (!have) S.p.push_back(a); }
    S.j.resize(b.jn); S.jp_done = b.jn; S.delta = b.delta; S.before = b.before; S.aligned = b.aligned; S.unknown = b.unknown; S.dead = b.dead; S.mark = b.mark; S.mark_unknown = b.mark_unknown;
    S.quiet = b.quiet; S.mark9 = b.mark9; S.mark9_unknown = b.mark9_unknown; S.mark9_set = b.mark9_set; }
std::string x86_rsp_mark_save(void) { if (MEDIUM_BINARY) return std::string(1, 'K'); return MEDIUM_MACRO_DEF ? std::string() : std::string("#@rspK\n"); }
void x86_rsp_mark9_push(void) { x86_rsp_st_t & S = x86_rsp_S();
    if (S.mark9_set) { fprintf(stderr, "FATAL x86_rsp_mark9_push: a shim's mark is already saved; a glue nests one deep inside a SHIM-SELF site\n"); abort(); }
    S.mark9 = S.mark; S.mark9_unknown = S.mark_unknown; S.mark9_set = 1; }
void x86_rsp_mark9_pop(void) { x86_rsp_st_t & S = x86_rsp_S(); if (!S.mark9_set) { fprintf(stderr, "FATAL x86_rsp_mark9_pop: no shim mark is saved\n"); abort(); }
    S.mark = S.mark9; S.mark_unknown = S.mark9_unknown; S.mark9_set = 0; }
std::string x86_rsp_mark_push(void) { if (MEDIUM_BINARY) return std::string(1, 'M'); return MEDIUM_MACRO_DEF ? std::string() : std::string("#@rspM\n"); }
std::string x86_rsp_mark_pop(void) { if (MEDIUM_BINARY) return std::string(1, 'N'); return MEDIUM_MACRO_DEF ? std::string() : std::string("#@rspN\n"); }
std::string x86_rsp_land(long k) { if (MEDIUM_BINARY) { std::string r; r += (char)'k'; uint32_t u = (uint32_t)(int32_t)k; for (int j = 0; j < 4; j++) r += (char)((u >> (8 * j)) & 255);
    return r; } return MEDIUM_MACRO_DEF ? std::string() : std::string("#@rspk ") + std::to_string(k) + "\n"; }
std::string x86_rsp_goto(void) { if (MEDIUM_BINARY) return std::string(1, 'G'); return MEDIUM_MACRO_DEF ? std::string() : std::string("#@rspG\n"); }
std::string x86_rsp_rec(long d) { if (MEDIUM_BINARY) { std::string r; r += (char)'R'; uint32_t u = (uint32_t)(int32_t)d; for (int j = 0; j < 4; j++) r += (char)((u >> (8 * j)) & 255); return r;
    } return MEDIUM_MACRO_DEF ? std::string() : std::string("#@rsp ") + std::to_string(d) + "\n"; }
std::string x86_rsp_mark(char k) { if (MEDIUM_BINARY) return std::string(1, k); return MEDIUM_MACRO_DEF ? std::string() : std::string("#@rsp") + k + "\n"; }
std::string x86_rsp_unk(void) { return x86_rsp_mark('U'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rsp_mark_save(const char * slot) {
    return x86("note", HKN(5)) + x86("mov", slot, "rsp");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_rsp_release_to(int disp) {
    static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rsp + %d]", disp);
    return x86_align_leave()
         + x86("mov", "rsp", b[i])
         + x86_align_enter();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
uint64_t x86_rsp_hash(const std::string & nm) { uint64_t h = 1469598103934665603ull; for (size_t k = 0; k < nm.size(); k++) { h ^= (unsigned char)nm[k]; h *= 1099511628211ull; } return h; }
