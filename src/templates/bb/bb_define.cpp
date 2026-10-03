#include <algorithm>
#include <string>
#include <cstdint>
#include <cstdio>
#include <vector>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "ab_abi.h"
#include "pin_va.h"
#include "rt.h"
extern int64_t kw_fnclevel;
extern int g_monitor_bin;
extern int rt_g_want_name;
extern int rt_g_ret_by_name;
void rt_kw_set_rtntype_role(int);
void rt_define_bind_entry(const char *fname, const char *entry);
extern int * const rt_k_level_p;
extern long rt_stno_stack[];
extern int g_core_errjmp_n;
extern long g_stno;
extern long g_line;
extern long g_lastno;
extern long g_lastline;
void *rt_proc_get_fn(const char *name);
void sno_trace_call(const char *fname);
void sno_trace_return(const char *fname, DESCR_t retval);
extern long g_trace_budget;
void rt_trace_call_hook(const char *fname);
void rt_trace_return_hook(const char *fname, DESCR_t retval);
void rt_trace_fail_hook(const char *fname);
extern long g_trace;
extern int64_t kw_ftrace;
const char *rt_define_query(const char *, int *, int *, int *, void **);
void rt_define_site(const char *, const char *, int, int, int, void *);
void rt_define_site_entry(const char *, const char *);
int bb_tiny_shim_ok(const char *, int);
}
extern "C" { extern int g_rt_fragment_emit; int xa_flat_class_c_pred(void); }
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" const char * bb_ab_sym_name(const char * nm) {
    enum { MAXN = 48, KEEP = 31 }; static_assert(KEEP + 17 <= MAXN, "a long name is its kept units, a dollar and 16 hex"); static char b[MAXN + 1]; int j = 0, bnd = 0, over = 0;
    unsigned long long h = 1469598103934665603ULL; for (const char * c = nm ? nm : ""; *c; c++) { unsigned char u = (unsigned char) *c; char e[4]; int k; h = (h ^ u) * 1099511628211ULL;
        if ((u >= 'A' && u <= 'Z') || (u >= 'a' && u <= 'z') || (u >= '0' && u <= '9') || u == '_' || u == '$' || u == '.') { e[0] = (char) u; k = 1; } else k = snprintf(e, sizeof e, "$%02X", u);
        if (!over && j + k <= MAXN) { memcpy(b + j, e, (size_t) k); j += k; if (j <= KEEP) bnd = j; } else over = 1; }
    if (over) j = bnd + snprintf(b + bnd, sizeof b - (size_t) bnd, "$%016llx", h); b[j] = 0; return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_ab_seal_entry_cells(const char * pname, void * fnbase, int alpha_face) {
    extern int emit_label_lookup_offset(const char *);
    if (!pname || !fnbase) return;
    char lbl[300], cell[300];
    if (alpha_face) { snprintf(lbl, sizeof lbl, "%s_\xce\xb1", pname); snprintf(cell, sizeof cell, "alpha$%s", pname); }
    else            { snprintf(lbl, sizeof lbl, "LBL__%s",  bb_ab_sym_name(pname)); snprintf(cell, sizeof cell, "entry$%s",  pname); }
    int off = emit_label_lookup_offset(lbl); if (off < 0) { if (getenv("SCRIP_SEAL_DIAG")) fprintf(stderr, "[SEAL] MISS lbl=%s cell=%s\n", lbl, cell); return; }
    *(void **)bb_ab_fn_cell_ptr(cell) = (void *)((char *)fnbase + off);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_ab_seal_alpha(const char * pname, void * alpha) {
    if (!pname || !alpha) return;
    char cell[300]; snprintf(cell, sizeof cell, "alpha$%s", pname);
    *(void **)bb_ab_fn_cell_ptr(cell) = alpha;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_define_entry_cell_data(const std::string & lbl, const std::string & init) {
    static std::vector<std::string> seen;
    return IF(std::count(seen.begin(), seen.end(), lbl) == 0 && (seen.push_back(lbl), true),
               x86("directive", std::string(".section .data"))
             + x86("directive", std::string(".align 8"))
             + x86("directive", lbl + std::string(":"))
             + x86("directive", std::string(".quad ") + init)
             + x86("directive", std::string(".section .text"))
             + x86("directive", std::string(".intel_syntax noprefix")));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_fnclevel_enter() {
    return x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
         + x86("mov", "rax", RDQ("rax", 0))
         + x86("add", RDD("rax", 0), (long)1)
         + x86("mov", "ecx", RDD("rax", 0))
         + x86("movsxd", "rcx", "ecx")
         + x86("sub", "rcx", (long)1)
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&kw_fnclevel, "kw_fnclevel")
         + x86("mov", RDQ("rax", 0), "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_stno_slot_rcx() { return bb_glue_lvl_slot_rcx(); }
static std::string bb_stno_save() {
    return x86("comment", "&STNO SAVE (trace-trunk row, cfo): the CALLER's &STNO/&LINE go into the slot of the level just entered, so RETURN can put them back")
         + bb_stno_slot_rcx()
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_stno, "g_stno")
         + x86("mov", "rax", RDQ("rax", 0))
         + x86("mov", RDQ("rcx", 0), "rax")
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_line, "g_line")
         + x86("mov", "rax", RDQ("rax", 0))
         + x86("mov", RDQ("rcx", 8), "rax");
}
static std::string bb_stno_last_from_callee() {
    return x86("comment", "&LASTNO/&LASTLINE ON RETURN: the returning function's statement and line become the previous ones, as SPITBOL answers them in the calling statement")
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_stno, "g_stno")
         + x86("mov", "rcx", RDQ("rax", 0))
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_lastno, "g_lastno")
         + x86("mov", RDQ("rax", 0), "rcx")
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_line, "g_line")
         + x86("mov", "rcx", RDQ("rax", 0))
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_lastline, "g_lastline")
         + x86("mov", RDQ("rax", 0), "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_stno_restore_act() {
    return x86("comment", "&STNO RESTORE + ACTIVATION RECORD RETIRED: the level record's activation base is cleared on the way out, "
                          "so a SETEXIT handler can never resume into a frame that has returned")
         + bb_stno_last_from_callee()
         + bb_stno_slot_rcx()
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_stno, "g_stno")
         + x86("mov", "rcx", RDQ("rcx", SNO_LVL_STNO))
         + x86("mov", RDQ("rax", 0), "rcx")
         + bb_stno_slot_rcx()
         + x86("mov", RDQ("rcx", SNO_LVL_ACT_RSP), (long)0)
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_line, "g_line")
         + x86("mov", "rcx", RDQ("rcx", SNO_LVL_LINE))
         + x86("mov", RDQ("rax", 0), "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_fnclevel_leave() {
    return x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
         + x86("mov", "rax", RDQ("rax", 0))
         + x86("mov", "ecx", RDD("rax", 0))
         + x86("movsxd", "rcx", "ecx")
         + x86("sub", "rcx", (long)1)
         + x86("mov", RDD("rax", 0), "ecx")
         + x86("sub", "rcx", (long)1)
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&kw_fnclevel, "kw_fnclevel")
         + x86("mov", RDQ("rax", 0), "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_define_bind() {
    x86_begin();
    const char * fname = _.op_sval ? _.op_sval : "?";
    int _np = 0, _nf = 0, _fb = 0; void * _fn = 0; const char * _csv = rt_define_query(fname, &_np, &_nf, &_fb, &_fn);
    if (_.op_proto && strchr(_.op_proto, '|')) { _nf = atoi(_.op_proto); _csv = strchr(_.op_proto, '|') + 1; _np = *_csv ? 1 : 0; for (const char * c = _csv; *c; c++) if (*c == ',') _np++; _fn = 0; }
    uint64_t _site_fp; { void (*fp)(const char *, const char *, int, int, int, void *) = rt_define_site; _site_fp = (uint64_t)(uintptr_t)(void *)fp; }
    std::string blbl = _.lbl_t0 ? std::string(_.lbl_t0) : std::string("rt_ab_undef_fn_stub");
    std::string reg = x86("comment", "DEFINE-SITE s57: constant-folded registration AT the statement (shared chain)")
         + x86("mov", "rdi", ROQ(0))
         + x86("mov", "rsi", ROQ(1))
         + x86("mov32", "edx", (long)_np)
         + x86("mov32", "ecx", (long)_nf)
         + x86("mov32", "r8d", (long)_fb)
         + (_.lbl_t0 ? x86("lea", "r9", std::string("[rip + __]"), (uint64_t)(uintptr_t)_fn, blbl.c_str()) : x86_load_got("r9", blbl.c_str(), (uint64_t)(uintptr_t)_fn))
         + x86_scan_sync_out()
         + x86("call", "rt_define_site", _site_fp)
         + x86_rt_gc_poll()
         + x86_scan_sync_in_rr();
    {
    static int _m4seal = -1; if (_m4seal < 0) { const char * _e = getenv("SCRIP_M4_ALPHA_SEAL"); _m4seal = (_e && *_e == '0') ? 0 : 1; }
      if (_m4seal && !bb_ab_cell_addr(fname) && bb_tiny_shim_ok(fname, 0)) {
        uint64_t _seal_fp; { void (*fp)(const char *, void *) = bb_ab_seal_alpha; _seal_fp = (uint64_t)(uintptr_t)(void *)fp; }
        reg = reg + x86("comment", "M4-ALPHA-SEAL: alpha$<FN> <- &<FN>_α, the m4 twin of the driver seal")
            + x86("mov", "rdi", ROQ(0))
            + x86("lea", "rsi", std::string("[rip + __]"), (uint64_t)0, (std::string(fname) + "_\xce\xb1").c_str())
            + x86_scan_sync_out()
            + x86("call", "bb_ab_seal_alpha", _seal_fp)
            + x86_scan_sync_in_rr(); } }
    { if (_.lbl_t0 && !bb_ab_cell_addr(fname) && bb_tiny_shim_ok(fname, 0)) {
        reg = reg + x86("comment", "M4-ENTRY-SEAL (term 3): entry_cell$<FN> <- &LBL__<this DEFINE's entry>, so a call reads the binding in force when it runs rather than a baked winner")
         + bb_define_entry_cell_data(std::string("entry_cell$") + std::string(bb_ab_sym_name(fname)), std::string(_.lbl_t0))
            + x86("lea", "rax", std::string("[rip + __]"), (uint64_t)0, _.lbl_t0)
            + x86("mov", "rcx", std::string("[rip@got + __]"), (uint64_t)0, (std::string("entry_cell$") + std::string(bb_ab_sym_name(fname))).c_str())
            + x86("mov", RDQ("rcx", 0), "rax"); } }
    std::string bind_seal;
    { if (_.lbl_t0 && bb_ab_cell_addr(fname)) {
        const char * _ent = (strncmp(_.lbl_t0, "LBL__", 5) == 0) ? _.lbl_t0 + 5 : _.lbl_t0;
        uint64_t _bind_fp; { void (*fp)(const char *, const char *) = rt_define_bind_entry; _bind_fp = (uint64_t)(uintptr_t)(void *)fp; }
        reg = reg
            + x86("mov", "rdi", ROQ(0))
            + x86("mov", "rsi", ROQ(2))
            + x86_scan_sync_out()
            + x86("call", "rt_define_bind_entry", _bind_fp)
            + x86_scan_sync_in_rr();
        bind_seal = x86("def", L(2))
            + x86(".quad", LS(2), _ent)
            + x86("label", LS(2))
            + x86(".string", _ent); } }
    std::string entry_seal;
    { if (_.op_proto && strchr(_.op_proto, '|') && _.op_entry && *_.op_entry) {
        uint64_t _ent_fp; { void (*fp)(const char *, const char *) = rt_define_site_entry; _ent_fp = (uint64_t)(uintptr_t)(void *)fp; }
        reg = reg
            + x86("mov", "rdi", ROQ(0))
            + x86("mov", "rsi", ROQ(3))
            + x86_scan_sync_out()
            + x86("call", "rt_define_site_entry", _ent_fp)
            + x86_scan_sync_in_rr();
        entry_seal = x86("def", L(3))
            + x86(".quad", LS(3), _.op_entry)
            + x86("label", LS(3))
            + x86(".string", _.op_entry); } }
    std::string seals = x86("def", L(0))
        + x86(".quad", LS(0), fname)
        + x86("label", LS(0))
        + x86(".string", fname)
        + x86("def", L(1))
        + x86(".quad", LS(1), (_csv ? _csv : ""))
        + x86("label", LS(1))
        + x86(".string", (_csv ? _csv : ""))
        + bind_seal
        + entry_seal;
    return x86_alpha() + reg + x86_pair_loop() + seals;
}
#include <string>
#include <cstdint>
#include "emit.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fnsig(void) {
    static int v = -1;
    if (v < 0) {
        const char * e = getenv("SCRIP_FN_SIG");
        v = (e && *e == '0') ? 0 : 1;
    } return v;
}
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "ab_abi.h"
#include "pin_va.h"
int   bb_scc_probe(const char *fname, int nargs, int *np_out, int *nsave_out, int *gk_out, int *res_gk_out);
int   rt_proc_nformals(const char *);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_define_sr() {
    x86_begin();
    long role = (long)_.op_ival;
    int inl5 = (role == 5) ? 1 : 0; if (inl5) role = 4;
    if (role == 3) {
        return x86("comment", "IR_DEFINE wire-adopt (s58: EMPTY — shim deleted, tiny sites carry the pushdown protocol)")
             + x86_alpha()
             + x86_gamma();
    }
    if (role == 4) {
        const char * fn4 = _.op_sval; const char * en4 = _.lbl_t0 ? _.lbl_t0 : fn4;
        int np4 = 0, ns4 = 0, rg4 = -1; int gk4[64];
        int ok4 = (fn4 && en4 && bb_tiny_shim_ok(fn4, 0)) ? bb_scc_probe(fn4, 0, &np4, &ns4, gk4, &rg4) : 0;
        int nf4 = ok4 ? rt_proc_nformals(fn4) : 0;
        if (!(ok4 && nf4 >= 0 && nf4 <= np4)) return inl5
             ? x86("comment", "role 5: shim refused inline (hatch or probe/formals shape) — sites fall to the slim arm")
             : (x86("comment", "IR_DEFINE role 4: shim refused (hatch, non-TEXT, or probe/formals shape) — sites fall to the slim arm")
                 + x86_alpha()
                 + x86_gamma());
        int xt4 = ns4 - nf4;
        long T4 = 16L * xt4 + 32;
        int rgx = rg4 < 0 ? 0 : rg4;
        auto GQ = [&](int gk, int w) { return (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, w) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + (unsigned long)w); };
        auto R8Q = [&](long d) { return std::string("[r8 + ") + std::to_string(d) + "]"; };
        auto CHAIN = [&](int base, const char * reg, auto arg, auto ext) {
            if (nf4 <= 0) return std::string();
            return FOR(0, nf4, [&](int i) { return x86("cmp", reg, (long)i) + x86_jcc_id("jbe", base + i) + arg(i); })
                 + x86_jmp_id(base + nf4)
                 + FOR(0, nf4, [&](int i) { return x86_deflabel_id(base + i) + ext(i); })
                 + x86_deflabel_id(base + nf4); };
        long WNOFF = 16L * xt4 + 24;
        auto WNSAVE = [&]() {
            return  x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_want_name, "rt_g_want_name")
                 + x86("mov", "edx", RDD("rax", 0))
                 + x86("movsxd", "rdx", "edx")
                 + x86_rsp_store64((int)WNOFF, "rdx")
                 + x86("mov", RDD("rax", 0), (long)0); };
        auto WNRESTORE = [&]() {
            return  x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_want_name, "rt_g_want_name")
                 + x86_rsp_load64("rdx", (int)WNOFF)
                 + x86("mov", RDD("rax", 0), "edx"); };
        std::string la = std::string(fn4) + "_\xce\xb1", lb = std::string(fn4) + "_\xce\xb3", lo = std::string(fn4) + "_\xcf\x89";
        std::string blb = inl5 ? std::string(en4) : (std::string("LBL__") + en4);
        const struct bb_label_t * lbl_b = emit_label_intern(lb.c_str()); const struct bb_label_t * lbl_o = emit_label_intern(lo.c_str());
        uint64_t entry_cell = (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("entry$") + fn4).c_str());
        std::string bcell = std::string("entry_cell$") + std::string(bb_ab_sym_name(fn4));
        auto SCALE16 = [&]() { return x86("mov", "rax", "rcx")
                 + x86("add", "rax", "rax")
                 + x86("add", "rax", "rax")
                 + x86("add", "rax", "rax")
                 + x86("add", "rax", "rax"); };
        auto RESTORE4 = [&](int lid) {
            return x86_rsp_load64("rcx", (int)(16 * xt4 + 16))
                 + SCALE16()
                 + x86("cmp", "rcx", (long)nf4)
                 + x86_jcc_id("jbe", lid - 5)
                 + x86("mov32", "eax", 16L * nf4)
                 + x86_deflabel_id(lid - 5)
                 + x86("lea", "r8", std::string("[rsp + ") + std::to_string(T4) + "]")
                 + x86("sub", "r8", "rax")
                 + FOR(0, xt4, [&](int j) { int k = xt4 - 1 - j;
                       return x86_rsp_load64("rax", 16 * k)
                            + x86("note", gva_name(gk4[nf4 + k]))
                            + x86("mov", GQ(gk4[nf4 + k], 0), "rax")
                            + x86_rsp_load64("rax", 16 * k + 8)
                            + x86("mov", GQ(gk4[nf4 + k], 8), "rax"); })
                 + CHAIN(lid, "rcx",
                       [&](int i) {
                           return x86("mov", "rax", R8Q(16L * nf4 + 32 + 16L * i))
                                + x86("note", gva_name(gk4[i]))
                                + x86("mov", GQ(gk4[i], 0), "rax")
                                + x86("mov", "rax", R8Q(16L * nf4 + 32 + 16L * i + 8))
                                + x86("mov", GQ(gk4[i], 8), "rax"); },
                       [&](int i) {
                           return x86("mov", "rax", R8Q(16L * i))
                                + x86("note", gva_name(gk4[i]))
                                + x86("mov", GQ(gk4[i], 0), "rax")
                                + x86("mov", "rax", R8Q(16L * i + 8))
                                + x86("mov", GQ(gk4[i], 8), "rax"); }); };
        if (fnsig()) {
            long F4 = T4 + 16L * nf4;
            auto SIGQ = [&](long d) { return std::string("[rcx + ") + std::to_string(d) + "]"; };
            auto EXTQ = [&](long d) { return std::string("[rsp + ") + std::to_string(T4 + d) + "]"; };
            auto R8AT = [&]() { return x86("lea", "r8", std::string("[rsp + ") + std::to_string(F4) + "]"); };
            auto FRESTORE = [&](int lid) {
                return x86_rsp_load64("rcx", (int)(16 * xt4 + 16))
                     + x86("mov", "rdx", SIGQ(0))
                     + R8AT()
                     + FOR(0, xt4, [&](int j) { int k = xt4 - 1 - j;
                           return x86_rsp_load64("rax", 16 * k)
                                + x86("note", gva_name(gk4[nf4 + k]))
                                + x86("mov", GQ(gk4[nf4 + k], 0), "rax")
                                + x86_rsp_load64("rax", 16 * k + 8)
                                + x86("mov", GQ(gk4[nf4 + k], 8), "rax"); })
                     + CHAIN(lid, "rdx",
                           [&](int i) {
                               return x86("mov", "rax", SIGQ(24 + 8L * i))
                                    + x86("add", "rax", "r8")
                                    + x86("mov", "rax", "[rax + 0]")
                                    + x86("note", gva_name(gk4[i]))
                                    + x86("mov", GQ(gk4[i], 0), "rax")
                                    + x86("mov", "rax", SIGQ(24 + 8L * i))
                                    + x86("add", "rax", "r8")
                                    + x86("mov", "rax", "[rax + 8]")
                                    + x86("mov", GQ(gk4[i], 8), "rax"); },
                           [&](int i) {
                               return x86("mov", "rax", EXTQ(16L * i).c_str())
                                    + x86("note", gva_name(gk4[i]))
                                    + x86("mov", GQ(gk4[i], 0), "rax")
                                    + x86("mov", "rax", EXTQ(16L * i + 8).c_str())
                                    + x86("mov", GQ(gk4[i], 8), "rax"); }); };
            return x86("comment", "IR_DEFINE role 4: SIG s66 per-DEFINE shim (alpha=swap-by-map, gamma/omega=restore-by-map, CONSTANT frame)")
                 + IF(inl5, x86_jmp_id(245))
                 + x86("commentrule", std::string(119, '-'))
                 + IF(!inl5, x86_alpha())
                 + x86_def_ext(emit_label_intern(la.c_str()))
                 + x86("sub", "rsp", F4)
                 + WNSAVE()
                 + FOR(0, xt4, [&](int k) {
                       return x86("note", gva_name(gk4[nf4 + k]))
                            + x86("mov", "rax", GQ(gk4[nf4 + k], 0))
                            + x86_rsp_store64(16 * k, "rax")
                            + x86("mov", "rax", GQ(gk4[nf4 + k], 8))
                            + x86_rsp_store64(16 * k + 8, "rax")
                            + x86("mov", GQ(gk4[nf4 + k], 0), (long)DT_SNUL)
                            + x86("mov", GQ(gk4[nf4 + k], 8), (long)0); })
                 + x86_rsp_store64(16 * xt4 + 16, "rcx")
                 + x86("mov", "rdx", SIGQ(0))
                 + R8AT()
                 + CHAIN(BB_SHIM_ID_ALPHA, "rdx",
                       [&](int i) {
                           return x86("mov", "rdi", SIGQ(24 + 8L * i))
                                + x86("add", "rdi", "r8")
                                + x86("mov", "rax", "[rdi + 0]")
                                + x86("note", gva_name(gk4[i]))
                                + x86("mov", "rsi", GQ(gk4[i], 0))
                                + x86("mov", GQ(gk4[i], 0), "rax")
                                + x86("mov", "[rdi + 0]", "rsi")
                                + x86("mov", "rax", "[rdi + 8]")
                                + x86("mov", "rsi", GQ(gk4[i], 8))
                                + x86("mov", GQ(gk4[i], 8), "rax")
                                + x86("mov", "[rdi + 8]", "rsi"); },
                       [&](int i) {
                           return x86("note", gva_name(gk4[i]))
                                + x86("mov", "rax", GQ(gk4[i], 0))
                                + x86("mov", EXTQ(16L * i).c_str(), "rax")
                                + x86("mov", "rax", GQ(gk4[i], 8))
                                + x86("mov", EXTQ(16L * i + 8).c_str(), "rax")
                                + x86("mov", GQ(gk4[i], 0), (long)DT_SNUL)
                                + x86("mov", GQ(gk4[i], 8), (long)0); })
         + x86("push", "rcx")
                 + bb_fnclevel_enter()
                 + bb_stno_save()
                 + x86("pop", "rcx")
         + IF(x86_trace_hooks_on(), x86_load_got("rax", "g_trace", (uint64_t)(uintptr_t)(void *)&g_trace)
                 + x86("mov", "rax", RDQ("rax", 0))
                 + x86("cmp", "rax", (long)0)
                 + x86_jcc_id("jg", 246)
                 + x86_load_got("rax", "kw_ftrace", (uint64_t)(uintptr_t)(void *)&kw_ftrace)
                 + x86("mov", "rax", RDQ("rax", 0))
                 + x86("cmp", "rax", (long)0)
                 + x86_jcc_id("jle", 230)
                 + x86_deflabel_id(246)
                 + x86("push", "rdi")
                 + x86("push", "rsi")
                 + x86("push", "rdx")
                 + x86("push", "rcx")
                 + x86("push", "r8")
                 + x86("push", "r9")
                 + x86("push", "r12")
                 + x86("push", "rdi")
                 + x86_align_enter()
                 + x86("mov", "rdi", ROQ(232))
                 + x86("call", "rt_trace_call_hook", (uint64_t)(uintptr_t)(void *)rt_trace_call_hook)
                 + x86_rt_gc_poll()
                 + x86_align_leave()
                 + x86("pop", "rdi")
                 + x86("pop", "r12")
                 + x86("pop", "r9")
                 + x86("pop", "r8")
                 + x86("pop", "rcx")
                 + x86("pop", "rdx")
                 + x86("pop", "rsi")
                 + x86("pop", "rdi")
                 + x86_deflabel_id(230))
                 + x86_jmp_id(231)
                 + x86("def", L(232))
                 + x86(".quad", LS(232), fn4)
                 + x86("label", LS(232))
                 + x86(".string", fn4)
                 + x86_deflabel_id(231)
                 + x86("lea", "rcx", "extlbl", (uint64_t)(uintptr_t)lbl_b)
                 + x86("lea", "rax", "extlbl", (uint64_t)(uintptr_t)lbl_o)
                 + (x86("comment", "s64 RSP-ONLY WRITER (see the s58 arm's full comment — unchanged under SIG)")
                             + x86("push", "rax")
                             + x86("push", "rcx"))
                 + bb_glue_act_record(0)
                 + bb_define_entry_cell_data(bcell, blb) + x86("jmp_fn_cell", bcell.c_str(), entry_cell)
                 + x86_def_ext(lbl_b)
                 + x86("note", gva_name(rgx))
                 + x86("mov", "rdi", GQ(rgx, 0))
                 + x86("mov", "rsi", GQ(rgx, 8))
                 + x86("mov", "rax", "rdi")
                 + x86("mov", "rdx", "rsi")
         + x86("push", "rdx")
                 + x86("push", "rax")
                 + IF(x86_trace_hooks_on(), x86_load_got("rax", "g_trace", (uint64_t)(uintptr_t)(void *)&g_trace)
                 + x86("mov", "rax", RDQ("rax", 0))
                 + x86("cmp", "rax", (long)0)
                 + x86_jcc_id("jne", 247)
                 + x86_load_got("rax", "kw_ftrace", (uint64_t)(uintptr_t)(void *)&kw_ftrace)
                 + x86("mov", "rax", RDQ("rax", 0))
                 + x86("cmp", "rax", (long)0)
                 + x86_jcc_id("jle", 235)
                 + x86_deflabel_id(247)
                 + x86("push", "rsi")
                 + x86("push", "rdi")
                 + x86("push", "rcx")
                 + x86("push", "r8")
                 + x86("push", "r9")
                 + x86("push", "r12")
                 + x86_align_enter()
                 + x86("mov", "rdi", ROQ(237))
                 + x86_rsp_load64("rsi", 48)
                 + x86_rsp_load64("rdx", 56)
                 + x86("call", "rt_trace_return_hook", (uint64_t)(uintptr_t)(void *)rt_trace_return_hook)
                 + x86_rt_gc_poll()
                 + x86_align_leave()
                 + x86("pop", "r12")
                 + x86("pop", "r9")
                 + x86("pop", "r8")
                 + x86("pop", "rcx")
                 + x86("pop", "rdi")
                 + x86("pop", "rsi")
                 + x86("note", gva_name(rgx))
                 + x86("mov", "rdi", GQ(rgx, 0))
                 + x86("mov", "rsi", GQ(rgx, 8))
                 + x86_deflabel_id(235))
                 + x86_jmp_id(236)
                 + x86("def", L(237))
                 + x86(".quad", LS(237), fn4)
                 + x86("label", LS(237))
                 + x86(".string", fn4)
                 + x86_deflabel_id(236)
                 + x86("pop", "rax")
                 + x86("pop", "rdx")
                 + FRESTORE(BB_SHIM_ID_BETA)
         + x86("push", "rcx")
                 + bb_stno_restore_act()
                 + bb_fnclevel_leave()
                 + x86("pop", "rcx")
                 + WNRESTORE()
                 + x86("mov", "rcx", SIGQ(8))
                 + x86("add", "rsp", F4)
                 + x86("comment", "re-stage the return value: FRESTORE above uses rax/rdx as scratch, so the pair staged for the tap is gone by here "
                                  "(row 521; the d067ceae4 revert was exactly this clobber)")
                 + x86("mov", "rax", "rdi")
                 + x86("mov", "rdx", "rsi")
                 + x86("jmp", "rcx")
                 + x86_def_ext(lbl_o)
                 + FRESTORE(BB_SHIM_ID_OMEGA)
         + IF(x86_trace_hooks_on(), x86_load_got("rax", "g_trace", (uint64_t)(uintptr_t)(void *)&g_trace)
                 + x86("mov", "rax", RDQ("rax", 0))
                 + x86("cmp", "rax", (long)0)
                 + x86_jcc_id("jne", 248)
                 + x86_load_got("rax", "kw_ftrace", (uint64_t)(uintptr_t)(void *)&kw_ftrace)
                 + x86("mov", "rax", RDQ("rax", 0))
                 + x86("cmp", "rax", (long)0)
                 + x86_jcc_id("jle", 249)
                 + x86_deflabel_id(248)
                 + x86("push", "rdi")
                 + x86("push", "rsi")
                 + x86("push", "rdx")
                 + x86("push", "rcx")
                 + x86("push", "r8")
                 + x86("push", "r9")
                 + x86("push", "r12")
                 + x86("push", "rdi")
                 + x86_align_enter()
                 + x86("mov", "rdi", ROQ(237))
                 + x86("call", "rt_trace_fail_hook", (uint64_t)(uintptr_t)(void *)rt_trace_fail_hook)
                 + x86_rt_gc_poll()
                 + x86_align_leave()
                 + x86("pop", "rdi")
                 + x86("pop", "r12")
                 + x86("pop", "r9")
                 + x86("pop", "r8")
                 + x86("pop", "rcx")
                 + x86("pop", "rdx")
                 + x86("pop", "rsi")
                 + x86("pop", "rdi")
                 + x86_deflabel_id(249))
         + x86("push", "rcx")
                 + bb_stno_restore_act()
                 + bb_fnclevel_leave()
                 + x86("pop", "rcx")
                 + WNRESTORE()
                 + x86("mov", "rcx", SIGQ(16))
                 + x86("add", "rsp", F4)
                 + x86("mov32", "eax", (long)DT_FAIL)
                 + x86("xor", "edx", "edx")
                 + x86("jmp", "rcx")
                 + IF(inl5, x86_deflabel_id(245))
                 + IF(!inl5, x86_gamma());
        }
        return x86("comment", "IR_DEFINE role 4: TINY-REAL s58 per-DEFINE shim (alpha=swap/extend, beta/omega=restore)")
             + IF(inl5, x86_jmp_id(245))
             + x86("commentrule", std::string(119, '-'))
             + IF(!inl5, x86_alpha())
             + x86_def_ext(emit_label_intern(la.c_str()))
             + x86_rsp_load64("rcx", 0)
             + x86("mov", "r8", "rsp")
             + x86("sub", "r8", 16L * nf4)
             + SCALE16()
             + x86("cmp", "rcx", (long)nf4)
             + x86_jcc_id("jbe", 2)
             + x86("mov32", "eax", 16L * nf4)
             + x86_deflabel_id(2)
             + x86("sub", "rsp", T4 + 16L * nf4)
             + x86("add", "rsp", "rax")
             + WNSAVE()
             + FOR(0, xt4, [&](int k) {
                   return x86("note", gva_name(gk4[nf4 + k]))
                        + x86("mov", "rax", GQ(gk4[nf4 + k], 0))
                        + x86_rsp_store64(16 * k, "rax")
                        + x86("mov", "rax", GQ(gk4[nf4 + k], 8))
                        + x86_rsp_store64(16 * k + 8, "rax")
                        + x86("mov", GQ(gk4[nf4 + k], 0), (long)DT_SNUL)
                        + x86("mov", GQ(gk4[nf4 + k], 8), (long)0); })
             + x86_rsp_store64(16 * xt4 + 16, "rcx")
             + CHAIN(BB_SHIM_ID_ALPHA, "rcx",
                   [&](int i) {
                       return x86("mov", "rax", R8Q(16L * nf4 + 32 + 16L * i))
                            + x86("note", gva_name(gk4[i]))
                            + x86("mov", "rdx", GQ(gk4[i], 0))
                            + x86("mov", GQ(gk4[i], 0), "rax")
                            + x86("mov", R8Q(16L * nf4 + 32 + 16L * i).c_str(), "rdx")
                            + x86("mov", "rax", R8Q(16L * nf4 + 32 + 16L * i + 8))
                            + x86("mov", "rdx", GQ(gk4[i], 8))
                            + x86("mov", GQ(gk4[i], 8), "rax")
                            + x86("mov", R8Q(16L * nf4 + 32 + 16L * i + 8).c_str(), "rdx"); },
                   [&](int i) {
                       return x86("note", gva_name(gk4[i]))
                            + x86("mov", "rax", GQ(gk4[i], 0))
                            + x86("mov", R8Q(16L * i).c_str(), "rax")
                            + x86("mov", "rax", GQ(gk4[i], 8))
                            + x86("mov", R8Q(16L * i + 8).c_str(), "rax")
                            + x86("mov", GQ(gk4[i], 0), (long)DT_SNUL)
                            + x86("mov", GQ(gk4[i], 8), (long)0); })
             + x86("lea", "rcx", "extlbl", (uint64_t)(uintptr_t)lbl_b)
             + x86("lea", "rax", "extlbl", (uint64_t)(uintptr_t)lbl_o)
             + ( x86("push", "rax")
                         + x86("push", "rcx"))
             + bb_define_entry_cell_data(bcell, blb) + x86("jmp_fn_cell", bcell.c_str(), entry_cell)
             + x86_def_ext(lbl_b)
             + x86("note", gva_name(rgx))
             + x86("mov", "rdi", GQ(rgx, 0))
             + x86("mov", "rsi", GQ(rgx, 8))
             + RESTORE4(BB_SHIM_ID_BETA)
             + WNRESTORE()
             + x86("mov32", "eax", T4 + 32 + 16L * nf4)
             + x86("cmp", "rcx", (long)nf4)
             + x86_jcc_id("jbe", 3)
             + SCALE16()
             + x86("add", "rax", T4 + 32)
             + x86_deflabel_id(3)
             + x86("mov", "rcx", R8Q(16L * nf4 + 16))
             + x86("add", "rsp", "rax")
             + x86("mov", "rax", "rdi")
             + x86("mov", "rdx", "rsi")
             + x86("jmp", "rcx")
             + x86_def_ext(lbl_o)
             + RESTORE4(BB_SHIM_ID_OMEGA)
             + WNRESTORE()
             + x86("mov32", "eax", T4 + 32 + 16L * nf4)
             + x86("cmp", "rcx", (long)nf4)
             + x86_jcc_id("jbe", 4)
             + SCALE16()
             + x86("add", "rax", T4 + 32)
             + x86_deflabel_id(4)
             + x86("mov", "rcx", R8Q(16L * nf4 + 24))
             + x86("add", "rsp", "rax")
             + x86("mov32", "eax", (long)DT_FAIL)
             + x86("xor", "edx", "edx")
             + x86("jmp", "rcx")
             + IF(inl5, x86_deflabel_id(245))
             + IF(!inl5, x86_gamma());
    }
    if (role == 1 || role == 2 || role == -1 ) {
        uint64_t _rtn_fp; { void (*_f)(int) = rt_kw_set_rtntype_role; _rtn_fp = (uint64_t)(uintptr_t)(void *)_f; }
        std::string rtn_set = x86("mov", "edi", (long)role)
                            + x86("call", "rt_kw_set_rtntype_role", _rtn_fp);
        std::string frag_release = (g_rt_fragment_emit && xa_flat_class_c_pred()) ? x86("add", "rsp", (long)_.flat_frame_bytes) : std::string();
        return x86("comment", role == 1 ? "IR_DEFINE RETURN floater (s64 RSP-ONLY: pop {gamma,omega} pair at TOS — depth IS the anchor)" :
                                   role == 2 ? "IR_DEFINE FRETURN floater (s64 RSP-ONLY: skip gamma, pop omega — depth IS the anchor)" :
                                               "IR_DEFINE NRETURN floater (s64 RSP-ONLY: pop gamma — by-name result)")
                 + x86_alpha()
                 + rtn_set
                 + frag_release
                 + (role == 2 ? x86("add", "rsp", (long)8)
                                + x86("pop", "rcx")
                              : x86("pop", "rcx")
                                + x86("add", "rsp", (long)8))
                 + x86("jmp", "rcx");
    }
    return x86_alpha() + x86_bomb("IR_DEFINE role 0 (the CALL2BB slice-2 producer) retired 2026-09-26 on a plant proof: "
                                  "no lowerer builds an SR-citizen DEFINE outside roles 1..5 (0 of 4693 corpus sources reached this arm or armed a handoff)");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_nreturn_mark() {
    extern int rt_g_ret_by_name;
    return x86_alpha()
         + x86("comment", "NRETURN floater: by-name mark (manual p.133); rt_nret_fix in the call epilogue reads+clears it; depth-agnostic — zero [rsp+K], zero calls; glue continues at RETURN")
         + x86("push", "rax")
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_ret_by_name, "rt_g_ret_by_name")
         + x86("mov", RDD("rax", 0), (long)1)
         + x86("pop", "rax")
         + x86_gamma();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_define() { return g_emit.op_define_role == 6 ? bb_define_bind() : bb_define_sr(); }
