#include <string>
#include <cstdint>
#include <cstdio>
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
extern int * const rt_k_level_p;
void *rt_proc_get_fn(const char *name);
void mon_emit_call_bin(const char *fname);
void mon_emit_return_bin(const char *fname, DESCR_t retval);
const char *rt_define_query(const char *, int *, int *, int *, void **);
void rt_define_site(const char *, const char *, int, int, int, void *);
int bb_tiny_shim_ok(const char *, int);
}
#include "x86_asm.h"
#define AB_TC_REG   "r8"
#define AB_TC_REG_D "r8d"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" const char * bb_ab_sym_name(const char * nm) {
    static char b[256]; int j = 0;
    for (const char * c = nm ? nm : ""; *c && j < 250; c++) { unsigned char u = (unsigned char) *c;
        if ((u >= 'A' && u <= 'Z') || (u >= 'a' && u <= 'z') || (u >= '0' && u <= '9') || u == '_' || u == '$' || u == '.') b[j++] = (char) u;
        else j += snprintf(b + j, (size_t)(256 - j), "$%02X", u); }
    b[j] = 0; return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_ab_seal_entry_cells(const char * pname, void * fnbase, int alpha_face) {
    extern int emit_label_lookup_offset(const char *);
    if (!pname || !fnbase) return;
    char lbl[300], cell[300];
    if (alpha_face) { snprintf(lbl, sizeof lbl, "%s_\xce\xb1", pname); snprintf(cell, sizeof cell, "alpha$%s", pname); }
    else            { snprintf(lbl, sizeof lbl, "LBL__%s",  bb_ab_sym_name(pname)); snprintf(cell, sizeof cell, "body$%s",  pname); }
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
static std::string bb_define_activate() {
    x86_begin();
    long   nsave    = (long)_.op_ival;
    long   nformals = (long)_.op_ab_nformals;
    const char * fname = _.op_sval ? _.op_sval : "?";
    long   K     = ab_frame_k(nsave) + 8;
    long   BASE  = K - 8;
    void ** fn_cell_ptr = (void **)0;
    std::string fn_cell_lbl = std::string("fn_cell$") + fname;
    fn_cell_ptr = (void **)bb_ab_cell_addr(fname);
    std::string s =
        x86("directive", std::string(".section .data"))
      + x86("directive", std::string(".align 8"))
      + x86("directive", fn_cell_lbl + std::string(":"))
      + x86("directive", std::string(".quad rt_ab_undef_fn_stub"))
      + x86("directive", std::string(".section .text"))
      + x86("directive", std::string(".intel_syntax noprefix"))
      + x86_alpha()
      + x86("sub",  "rsp", K)
      + x86("mov", RDQ("rsp", BASE + AB_OFF_GW), "rcx")
      + x86("mov", RDQ("rsp", BASE + AB_OFF_WW), "rdx")
      + x86("lea", "rax", RDQ("rsp", K))
      + x86("mov", RDQ("rsp", BASE + AB_OFF_ERSP), "rax")
      + x86("mov", "rax", ABSQ(RT_AB_ANCHOR))
      + x86("mov", RDQ("rsp", BASE + AB_OFF_ANCHOR), "rax")
      + x86("lea", "rdx", RDQ("rsp", BASE))
      + x86("mov", ABSQ(RT_AB_ANCHOR), "rdx")
      + x86("lea", "rax", L(1))
      + x86("mov", RDQ("rsp", BASE + AB_OFF_BADDR), "rax")
      + x86("note", std::string("RTX-FUNC-1 inline enter_env"))
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&Σ, "Σ")
      + x86("mov", "rcx", RDQ("rax", 0))
      + x86("mov", RDQ("rsp", BASE + AB_OFF_SIGMA), "rcx")
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&Σlen, "Σlen")
      + x86("mov", "ecx", RDD("rax", 0))
      + x86("movsxd", "rcx", "ecx")
      + x86("mov", RDQ("rsp", BASE + AB_OFF_SIGMALEN), "rcx")
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_want_name, "rt_g_want_name")
      + x86("mov", "ecx", RDD("rax", 0))
      + x86("movsxd", "rcx", "ecx")
      + x86("mov", RDQ("rsp", BASE + AB_OFF_WN), "rcx")
      + x86("mov", RDD("rax", 0), (long)0)
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
      + x86("mov", "rax", RDQ("rax", 0))
      + x86("add", RDD("rax", 0), (long)1)
      + x86("mov", "ecx", RDD("rax", 0))
      + x86("movsxd", "rcx", "ecx")
      + x86("sub", "rcx", (long)1)
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&kw_fnclevel, "kw_fnclevel")
      + x86("mov", RDQ("rax", 0), "rcx")
      + FOR(0, (int)nsave, [&](int k) -> std::string {
            int gk  = (k < (int)_.op_arg_slot_n) ? _.op_arg_slot[k] : -1;
            int ot  = ab_save_off(nsave, k);
            int ov  = ab_save_off(nsave, k) + 8;
            if (gk >= 0) {
                return x86("note", gva_name(gk))
                     + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, 0) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16))
                     + x86("mov", RDQ("rsp", (int)BASE + ot), "rax")
                     + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, 8) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + 8))
                     + x86("mov", RDQ("rsp", (int)BASE + ov), "rax");
            }
            return x86("mov", RDQ("rsp", (int)BASE + ot), (long)0)
                 + x86("mov", RDQ("rsp", (int)BASE + ov), (long)0);
        })
      + FOR(0, (int)nsave, [&](int k) -> std::string {
            if (k >= 1 && k <= (int)nformals) return std::string();
            int gk = (k < (int)_.op_arg_slot_n) ? _.op_arg_slot[k] : -1;
            if (k == 0 && gk >= 0) { for (int f = 1; f <= (int)nformals && f < (int)_.op_arg_slot_n; f++) if (_.op_arg_slot[f] == gk) return std::string(); }
            if (gk >= 0) {
                return x86("note", gva_name(gk))
                     + x86("xor",  "eax", "eax")
                     + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, 0) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16),     "rax")
                     + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, 8) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + 8), "rax");
            }
            return std::string();
        })
      + x86_load_got("rax", "g_monitor_bin", (uint64_t)(uintptr_t)(void *)&g_monitor_bin)
      + x86("mov",    "rax", RDQ("rax", 0))
      + x86("test",   "rax", "rax")
      + x86("je",  L(2))
      + x86("push", "rdi") + x86("push", "rsi") + x86("push", "rdx") + x86("push", "rcx")
      + x86("push", "r8")  + x86("push", "r9")  + x86("push", "r12") + x86("push", "rdi")
      + x86_align_enter()
      + x86_ro_load_q("rdi", 0)
      + x86("call", "mon_emit_call_bin", (uint64_t)(uintptr_t)(void *)mon_emit_call_bin)
      + x86_align_leave()
      + x86("pop", "rdi") + x86("pop", "r12") + x86("pop", "r9")  + x86("pop", "r8")
      + x86("pop", "rcx") + x86("pop", "rdx") + x86("pop", "rsi") + x86("pop", "rdi")
      + x86("def", L(2))
      + [&]() -> std::string {
            char blbl[128];
            snprintf(blbl, sizeof blbl, "FN__%s", fname);
            return x86("jmp_fn", blbl, (uint64_t)(uintptr_t)rt_proc_get_fn(fname));
        }()
      + x86_gamma()
      + x86("def", L(1))
      + x86("movzx", AB_TC_REG, "cl")
      + x86("mov", "rsi", ABSQ(RT_AB_ANCHOR))
      + x86("cmp", AB_TC_REG_D, (long)AB_TC_FRETURN)
      + x86("je",  L(3))
      + [&]() -> std::string { int gk0 = (0 < (int)_.op_arg_slot_n) ? _.op_arg_slot[0] : -1;
            if (gk0 < 0) return x86("xor", "eax", "eax") + x86("xor", "edx", "edx");
            return x86("note", gva_name(gk0))
                 + x86("mov", "rax", ABSQ(RT_GVA_VA + (unsigned long)gk0 * 16))
                 + x86("mov", "rdx", ABSQ(RT_GVA_VA + (unsigned long)gk0 * 16 + 8)); }()
      + x86("mov", RDQ("rsi", AB_OFF_RES0), "rax")
      + x86("mov", RDQ("rsi", AB_OFF_RES1), "rdx")
      + x86("note", std::string("RTX-FUNC-2 leave_env fast-path guards"))
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_ret_by_name, "rt_g_ret_by_name")
      + x86("mov", "eax", RDD("rax", 0))
      + x86("cmp", "eax", (long)0)
      + x86("jne", L(7))
      + x86("mov", "rcx", RDQ("rsi", AB_OFF_SIGMA))
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&Σ, "Σ")
      + x86("mov", RDQ("rax", 0), "rcx")
      + x86("mov", "rcx", RDQ("rsi", AB_OFF_SIGMALEN))
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&Σlen, "Σlen")
      + x86("mov", RDD("rax", 0), "ecx")
      + x86("mov", "rcx", RDQ("rsi", AB_OFF_WN))
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_want_name, "rt_g_want_name")
      + x86("mov", RDD("rax", 0), "ecx")
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
      + x86("mov", "rax", RDQ("rax", 0))
      + x86("mov", "ecx", RDD("rax", 0))
      + x86("movsxd", "rcx", "ecx")
      + x86("sub", "rcx", (long)1)
      + x86("mov", RDD("rax", 0), "ecx")
      + x86("sub", "rcx", (long)1)
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&kw_fnclevel, "kw_fnclevel")
      + x86("mov", RDQ("rax", 0), "rcx")
      + x86("mov", "rax", RDQ("rsi", AB_OFF_RES0))
      + x86("mov", "rdx", RDQ("rsi", AB_OFF_RES1))
      + x86("jmp", L(8))
      + x86("def", L(7))
      + x86_align_enter()
      + x86("mov", "rdi", "rsi")
      + x86("mov", "rsi", RDQ("rdi", AB_OFF_RES0))
      + x86("mov", "rdx", RDQ("rdi", AB_OFF_RES1))
      + x86("xor", "ecx", "ecx")
      + x86("call", "rt_ab_leave_env", (uint64_t)(uintptr_t)(void *)rt_ab_leave_env)
      + x86_align_leave()
      + x86("mov", "rsi", ABSQ(RT_AB_ANCHOR))
      + x86("def", L(8))
      + x86("mov", RDQ("rsi", AB_OFF_RES0), "rax")
      + x86("mov", RDQ("rsi", AB_OFF_RES1), "rdx")
      + x86("jmp", L(4))
      + x86("def", L(3))
      + x86_align_enter()
      + x86("mov", "rdi", "rsi")
      + x86("xor", "esi", "esi")
      + x86("xor", "edx", "edx")
      + x86("mov", "ecx", (long)1)
      + x86("call", "rt_ab_leave_env", (uint64_t)(uintptr_t)(void *)rt_ab_leave_env)
      + x86_align_leave()
      + x86("mov", "rsi", ABSQ(RT_AB_ANCHOR))
      + x86("mov", RDQ("rsi", AB_OFF_RES0), "rax")
      + x86("mov", RDQ("rsi", AB_OFF_RES1), "rdx")
      + x86("def", L(4))
      + FOR(0, (int)nsave, [&](int k) -> std::string {
            int gk  = (k < (int)_.op_arg_slot_n) ? _.op_arg_slot[k] : -1;
            int ot  = ab_save_off(nsave, k);
            int ov  = ab_save_off(nsave, k) + 8;
            if (gk >= 0) {
                return x86("note", gva_name(gk))
                     + x86("mov", "rcx", RDQ("rsi", (int)0 + ot))
                     + x86("mov", ABSQ(RT_GVA_VA + (unsigned long)gk * 16),     "rcx")
                     + x86("mov", "rcx", RDQ("rsi", (int)0 + ov))
                     + x86("mov", ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + 8), "rcx");
            }
            return std::string();
        })
      + x86("mov", "rcx", RDQ("rsi", AB_OFF_ANCHOR))
      + x86_load_got("rax", "g_monitor_bin", (uint64_t)(uintptr_t)(void *)&g_monitor_bin)
      + x86("mov",    "rax", RDQ("rax", 0))
      + x86("test",   "rax", "rax")
      + x86("je",  L(5))
      + x86_align_enter()
      + x86_ro_load_q("rdi", 0)
      + x86("mov", "rdx", RDQ("rsi", AB_OFF_RES1))
      + x86("mov", "rsi", RDQ("rsi", AB_OFF_RES0))
      + x86("call", "mon_emit_return_bin", (uint64_t)(uintptr_t)(void *)mon_emit_return_bin)
      + x86_align_leave()
      + x86("def", L(5))
      + x86("mov", "rsi", ABSQ(RT_AB_ANCHOR))
      + x86("mov", "rax", RDQ("rsi", AB_OFF_RES0))
      + x86("mov", "rdx", RDQ("rsi", AB_OFF_RES1))
      + x86("mov", "rcx", RDQ("rsi", AB_OFF_ANCHOR))
      + x86("mov", ABSQ(RT_AB_ANCHOR), "rcx")
      + x86("mov", "rsp", RDQ("rsi", AB_OFF_ERSP))
      + x86("cmp", AB_TC_REG_D, (long)AB_TC_FRETURN)
      + x86("je",  L(6))
      + bb_glue_wire_γ()
      + x86("def", L(6))
      + bb_glue_wire_ω()
      + x86_deflabel(X86P_GAMMA)
      + x86_deflabel(X86P_OMEGA)
      + x86_ro_seal_str(0, fname);
    if (fn_cell_ptr) {
        *fn_cell_ptr = (void *)(uintptr_t)rt_ab_undef_fn_stub;
    }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_define_bind() {
    x86_begin();
    const char * fname = _.op_sval ? _.op_sval : "?";
    std::string albl = std::string(fname) + "_act_\xce\xb1";
    std::string clbl = std::string("fn_cell$") + fname;
    static int _ab = -1; if (_ab < 0) { const char * _e = getenv("SCRIP_AB"); _ab = (_e && *_e == '1') ? 1 : 0; }
    int _np = 0, _nf = 0, _fb = 0; void * _fn = 0; const char * _csv = rt_define_query(fname, &_np, &_nf, &_fb, &_fn);
    uint64_t _site_fp; { void (*fp)(const char *, const char *, int, int, int, void *) = rt_define_site; _site_fp = (uint64_t)(uintptr_t)(void *)fp; }
    static int _direct_alpha = -1; if (_direct_alpha < 0) { const char * _e = getenv("SCRIP_DEFINE_FN_DIRECT_ALPHA"); _direct_alpha = (_e && *_e == '0') ? 0 : 1; }
    int _alpha_defined = (!bb_ab_cell_addr(fname) && bb_tiny_shim_ok(fname, 0));
    std::string blbl = _.lbl_t0 ? ((_direct_alpha && _alpha_defined) ? (std::string(fname) + "_\xce\xb1") : std::string(_.lbl_t0)) : std::string("rt_ab_undef_fn_stub");
    std::string reg = x86("comment", "DEFINE-SITE s57: constant-folded registration AT the statement (shared chain)")
         + x86_ro_load_q("rdi", 0)
         + x86_ro_load_q("rsi", 1)
         + x86("mov32", "edx", (long)_np)
         + x86("mov32", "ecx", (long)_nf)
         + x86("mov32", "r8d", (long)_fb)
         + (_.lbl_t0 ? x86("lea", "r9", std::string("[rip + __]"), (uint64_t)(uintptr_t)_fn, blbl.c_str()) : x86_load_got("r9", blbl.c_str(), (uint64_t)(uintptr_t)_fn))
         + x86_scan_sync_out()
         + x86("call", "rt_define_site", _site_fp)
         + x86_scan_sync_in_rr();
    { static int _m4seal = -1; if (_m4seal < 0) { const char * _e = getenv("SCRIP_M4_ALPHA_SEAL"); _m4seal = (_e && *_e == '0') ? 0 : 1; }
      if (_m4seal && !bb_ab_cell_addr(fname) && bb_tiny_shim_ok(fname, 0)) {
        uint64_t _seal_fp; { void (*fp)(const char *, void *) = bb_ab_seal_alpha; _seal_fp = (uint64_t)(uintptr_t)(void *)fp; }
        reg = reg + x86("comment", "M4-ALPHA-SEAL: alpha$<FN> <- &<FN>_α, the m4 twin of the driver seal")
            + x86_ro_load_q("rdi", 0)
            + x86("lea", "rsi", std::string("[rip + __]"), (uint64_t)0, (std::string(fname) + "_\xce\xb1").c_str())
            + x86_scan_sync_out()
            + x86("call", "bb_ab_seal_alpha", _seal_fp)
            + x86_scan_sync_in_rr(); } }
    std::string seals = x86_ro_seal_str(0, fname) + x86_ro_seal_str(1, _csv ? _csv : "");
    if (!_ab) return x86_alpha() + reg + x86_pair_loop() + seals;
    if (bb_ab_cell_addr(fname)) return x86_alpha() + reg + x86_pair_loop() + seals;
    return x86_alpha()
         + reg
         + x86("lea", "rax", std::string("[rip + __]"), (uint64_t)0, albl.c_str())
         + x86("mov", "rcx", std::string("[rip@got + __]"), (uint64_t)0, clbl.c_str())
         + x86("mov", RDQ("rcx", 0), "rax")
         + x86_pair_loop()
         + seals;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_ab_emit_nodes(IR_graph_t *g, int gva_active)
{
    if (!g || g->ab_n <= 0) return;
    extern int g_gva_active;
    extern IR_graph_t *g_emit_cfg;
    extern int gva_index_of(const char *);
    sm_emit_t saved_emit   = g_emit;
    IR_graph_t *saved_cfg  = g_emit_cfg;
    int         saved_gva  = g_gva_active;
    g_gva_active = gva_active;
    char ab_lbl_α[128], ab_lbl_β[128], ab_lbl_γ[128], ab_lbl_ω[128];
    for (int i = 0; i < g->ab_n; i++) {
        IR_t *nd = g->ab_nodes[i];
        if (!nd || nd->op != IR_DEFINE) continue;
        g_emit         = saved_emit;
        g_emit_cfg     = g;
        g_emit.op_sval = IR_LIT(nd).sval;
        g_emit.op_ival = (long)nd->n_operands;
        g_emit.op_ab_nformals = nd->seal;
        g_emit.op_beta_dead = 0;
        const char *fn = g_emit.op_sval ? g_emit.op_sval : "unknown";
        snprintf(ab_lbl_α, sizeof ab_lbl_α, "%s_act_\xce\xb1", fn);
        snprintf(ab_lbl_β, sizeof ab_lbl_β, "%s_act_\xce\xb2", fn);
        snprintf(ab_lbl_γ, sizeof ab_lbl_γ, "%s_act_\xce\xb3", fn);
        snprintf(ab_lbl_ω, sizeof ab_lbl_ω, "%s_act_\xcf\x89", fn);
        g_emit.lbl_α = ab_lbl_α;
        g_emit.lbl_β = ab_lbl_β;
        g_emit.lbl_γ = ab_lbl_γ;
        g_emit.lbl_ω = ab_lbl_ω;
        g_emit.lbl_α_p = emit_label_intern(ab_lbl_α);
        g_emit.lbl_β_p = emit_label_intern(ab_lbl_β);
        g_emit.lbl_γ_p = emit_label_intern(ab_lbl_γ);
        g_emit.lbl_ω_p = emit_label_intern(ab_lbl_ω);
        int _ab_n = (int)nd->n_operands;
        drive_arg_slots_reserve(_ab_n);
        for (int k = 0; k < _ab_n; k++) {
            const char *nm = nd->operands[k] ? IR_LIT(nd->operands[k]).sval : (const char *)0;
            g_emit.op_arg_slot[k] = (nm && gva_active) ? gva_index_of(nm) : -1;
        }
        g_emit.op_arg_slot_n = _ab_n;
        bb_emit_x86(bb_define_activate());
        void **cell = (void **)bb_ab_cell_addr(fn);
        if (cell) {
            extern bb_buf_t bb_emit_buf;
            bb_label_t *al = emit_label_intern(ab_lbl_α);
            if (al && bb_label_defined(al))
                *cell = (void *)(bb_emit_buf + al->offset);
        }
    }
    g_emit     = saved_emit;
    g_emit_cfg = saved_cfg;
    g_gva_active = saved_gva;
}
#include <string>
#include <cstdint>
#include "emit.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fnrbp(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_FN_RBP"); v = e ? atoi(e) : 2; if (v == 1) v = 2; if (v < 0 || v > 2) v = 2; } return v; }
static int fnsig(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_FN_SIG"); v = (e && *e == '0') ? 0 : 1; } return v; }
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "ab_abi.h"
#include "pin_va.h"
long  rt_proc_call_open_slim(const char *name, int np, int nargs);
int   bb_scc_probe(const char *fname, int nargs, int *np_out, int *nsave_out, int *gk_out, int *res_gk_out);
int   bb_scc_handoff_arm(void);
int   bb_scc_handoff_room(void);
int   rt_proc_nformals(const char *);
void  bb_scc_handoff_pending_clear(void);
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
        if (!(ok4 && nf4 >= 0 && nf4 <= np4 && nf4 <= 29)) return inl5 ? x86("comment", "role 5: shim refused inline (hatch or probe/formals shape) — sites fall to the slim arm") :    (x86("comment", "IR_DEFINE role 4: shim refused (hatch, non-TEXT, or probe/formals shape) — sites fall to the slim arm") + x86_alpha() + x86_gamma());
        int xt4 = ns4 - nf4;
        long T4 = 16L * xt4 + 32;
        int rgx = rg4 < 0 ? 0 : rg4;
        auto GQ = [&](int gk, int w) { return (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, w) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + (unsigned long)w); };
        auto R8Q = [&](long d) { return std::string("[r8 + ") + std::to_string(d) + "]"; };
        std::string la = std::string(fn4) + "_\xce\xb1", lb = std::string(fn4) + "_\xce\xb3", lo = std::string(fn4) + "_\xcf\x89";
        std::string blb = inl5 ? std::string(en4) : (std::string("LBL__") + en4);
        const struct bb_label_t * lbl_b = emit_label_intern(lb.c_str()); const struct bb_label_t * lbl_o = emit_label_intern(lo.c_str());
        uint64_t body_cell = (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("body$") + en4).c_str());
        auto SCALE16 = [&]() { return x86("mov", "rax", "rcx") + x86("add", "rax", "rax") + x86("add", "rax", "rax") + x86("add", "rax", "rax") + x86("add", "rax", "rax"); };
        auto RESTORE4 = [&](int lid) {
            return x86_rsp_load64("rcx", (int)(16 * xt4 + 16))
                 + SCALE16()
                 + x86("cmp", "rcx", (long)nf4) + x86_jcc_id("jbe", lid - 5) + x86("mov32", "eax", 16L * nf4) + x86_deflabel_id(lid - 5)
                 + x86("lea", "r8", std::string("[rsp + ") + std::to_string(T4) + "]") + x86("sub", "r8", "rax")
                 + FOR(0, xt4, [&](int j) { int k = xt4 - 1 - j;
                       return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(gk4[nf4 + k])) + x86("mov", GQ(gk4[nf4 + k], 0), "rax")
                            + x86_rsp_load64("rax", 16 * k + 8) + x86("mov", GQ(gk4[nf4 + k], 8), "rax"); })
                 + FOR(0, nf4, [&](int i) {
                       return x86("cmp", "rcx", (long)i) + x86_jcc_id("jbe", lid + i)
                            + x86("mov", "rax", R8Q(16L * nf4 + 32 + 16L * i)) + x86("note", gva_name(gk4[i])) + x86("mov", GQ(gk4[i], 0), "rax")
                            + x86("mov", "rax", R8Q(16L * nf4 + 32 + 16L * i + 8)) + x86("mov", GQ(gk4[i], 8), "rax")
                            + x86_jmp_id(lid + 30 + i)
                            + x86_deflabel_id(lid + i)
                            + x86("mov", "rax", R8Q(16L * i)) + x86("note", gva_name(gk4[i])) + x86("mov", GQ(gk4[i], 0), "rax")
                            + x86("mov", "rax", R8Q(16L * i + 8)) + x86("mov", GQ(gk4[i], 8), "rax")
                            + x86_deflabel_id(lid + 30 + i); }); };
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
                           return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(gk4[nf4 + k])) + x86("mov", GQ(gk4[nf4 + k], 0), "rax")
                                + x86_rsp_load64("rax", 16 * k + 8) + x86("mov", GQ(gk4[nf4 + k], 8), "rax"); })
                     + FOR(0, nf4, [&](int i) {
                           return x86("cmp", "rdx", (long)i) + x86_jcc_id("jbe", lid + i)
                                + x86("mov", "rax", SIGQ(24 + 8L * i)) + x86("add", "rax", "r8")
                                + x86("mov", "rax", "[rax + 0]") + x86("note", gva_name(gk4[i])) + x86("mov", GQ(gk4[i], 0), "rax")
                                + x86("mov", "rax", SIGQ(24 + 8L * i)) + x86("add", "rax", "r8")
                                + x86("mov", "rax", "[rax + 8]") + x86("mov", GQ(gk4[i], 8), "rax")
                                + x86_jmp_id(lid + 30 + i)
                                + x86_deflabel_id(lid + i)
                                + x86("mov", "rax", EXTQ(16L * i).c_str()) + x86("note", gva_name(gk4[i])) + x86("mov", GQ(gk4[i], 0), "rax")
                                + x86("mov", "rax", EXTQ(16L * i + 8).c_str()) + x86("mov", GQ(gk4[i], 8), "rax")
                                + x86_deflabel_id(lid + 30 + i); }); };
            return x86("comment", "IR_DEFINE role 4: SIG s66 per-DEFINE shim (alpha=swap-by-map, gamma/omega=restore-by-map, CONSTANT frame)")
                 + IF(inl5, x86_jmp_id(245))
                 + x86("commentrule", std::string(119, '-'))
                 + IF(!inl5, x86_alpha())
                 + x86_def_ext(emit_label_intern(la.c_str()))
                 + x86("sub", "rsp", F4)
                 + FOR(0, xt4, [&](int k) {
                       return x86("note", gva_name(gk4[nf4 + k])) + x86("mov", "rax", GQ(gk4[nf4 + k], 0)) + x86_rsp_store64(16 * k, "rax")
                            + x86("mov", "rax", GQ(gk4[nf4 + k], 8)) + x86_rsp_store64(16 * k + 8, "rax")
                            + x86("mov", GQ(gk4[nf4 + k], 0), (long)DT_SNUL) + x86("mov", GQ(gk4[nf4 + k], 8), (long)0); })
                 + x86_rsp_store64(16 * xt4 + 16, "rcx")
                 + x86("mov", "rdx", SIGQ(0))
                 + R8AT()
                 + FOR(0, nf4, [&](int i) {
                       return x86("cmp", "rdx", (long)i) + x86_jcc_id("jbe", 10 + i)
                            + x86("mov", "rdi", SIGQ(24 + 8L * i)) + x86("add", "rdi", "r8")
                            + x86("mov", "rax", "[rdi + 0]") + x86("note", gva_name(gk4[i])) + x86("mov", "rsi", GQ(gk4[i], 0)) + x86("mov", GQ(gk4[i], 0), "rax") + x86("mov", "[rdi + 0]", "rsi")
                            + x86("mov", "rax", "[rdi + 8]") + x86("mov", "rsi", GQ(gk4[i], 8)) + x86("mov", GQ(gk4[i], 8), "rax") + x86("mov", "[rdi + 8]", "rsi")
                            + x86_jmp_id(41 + i)
                            + x86_deflabel_id(10 + i)
                            + x86("note", gva_name(gk4[i])) + x86("mov", "rax", GQ(gk4[i], 0)) + x86("mov", EXTQ(16L * i).c_str(), "rax")
                            + x86("mov", "rax", GQ(gk4[i], 8)) + x86("mov", EXTQ(16L * i + 8).c_str(), "rax")
                            + x86("mov", GQ(gk4[i], 0), (long)DT_SNUL) + x86("mov", GQ(gk4[i], 8), (long)0)
                            + x86_deflabel_id(41 + i); })
                 + x86("lea", "rcx", "extlbl", (uint64_t)(uintptr_t)lbl_b)
                 + x86("lea", "rax", "extlbl", (uint64_t)(uintptr_t)lbl_o)
                 + IF(fnrbp() == 2, x86("comment", "s64 RSP-ONLY WRITER (see the s58 arm's full comment — unchanged under SIG)")
                             + x86("push", "rax")
                             + x86("push", "rcx"))
                 + x86("jmp", "[rip@cell + __]", body_cell, blb.c_str())
                 + x86_def_ext(lbl_b)
                 + x86("note", gva_name(rgx)) + x86("mov", "rdi", GQ(rgx, 0)) + x86("mov", "rsi", GQ(rgx, 8))
                 + FRESTORE(80)
                 + x86("mov", "rcx", SIGQ(8))
                 + x86("add", "rsp", F4)
                 + x86("mov", "rax", "rdi") + x86("mov", "rdx", "rsi")
                 + x86("jmp", "rcx")
                 + x86_def_ext(lbl_o)
                 + FRESTORE(150)
                 + x86("mov", "rcx", SIGQ(16))
                 + x86("add", "rsp", F4)
                 + x86("mov32", "eax", (long)DT_FAIL) + x86("xor", "edx", "edx")
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
             + x86("mov", "r8", "rsp") + x86("sub", "r8", 16L * nf4)
             + SCALE16()
             + x86("cmp", "rcx", (long)nf4) + x86_jcc_id("jbe", 2) + x86("mov32", "eax", 16L * nf4) + x86_deflabel_id(2)
             + x86("sub", "rsp", T4 + 16L * nf4)
             + x86("add", "rsp", "rax")
             + FOR(0, xt4, [&](int k) {
                   return x86("note", gva_name(gk4[nf4 + k])) + x86("mov", "rax", GQ(gk4[nf4 + k], 0)) + x86_rsp_store64(16 * k, "rax")
                        + x86("mov", "rax", GQ(gk4[nf4 + k], 8)) + x86_rsp_store64(16 * k + 8, "rax")
                        + x86("mov", GQ(gk4[nf4 + k], 0), (long)DT_SNUL) + x86("mov", GQ(gk4[nf4 + k], 8), (long)0); })
             + x86_rsp_store64(16 * xt4 + 16, "rcx")
             + FOR(0, nf4, [&](int i) {
                   return x86("cmp", "rcx", (long)i) + x86_jcc_id("jbe", 10 + i)
                        + x86("mov", "rax", R8Q(16L * nf4 + 32 + 16L * i)) + x86("note", gva_name(gk4[i])) + x86("mov", "rdx", GQ(gk4[i], 0)) + x86("mov", GQ(gk4[i], 0), "rax") + x86("mov", R8Q(16L * nf4 + 32 + 16L * i).c_str(), "rdx")
                        + x86("mov", "rax", R8Q(16L * nf4 + 32 + 16L * i + 8)) + x86("mov", "rdx", GQ(gk4[i], 8)) + x86("mov", GQ(gk4[i], 8), "rax") + x86("mov", R8Q(16L * nf4 + 32 + 16L * i + 8).c_str(), "rdx")
                        + x86_jmp_id(41 + i)
                        + x86_deflabel_id(10 + i)
                        + x86("note", gva_name(gk4[i])) + x86("mov", "rax", GQ(gk4[i], 0)) + x86("mov", R8Q(16L * i).c_str(), "rax")
                        + x86("mov", "rax", GQ(gk4[i], 8)) + x86("mov", R8Q(16L * i + 8).c_str(), "rax")
                        + x86("mov", GQ(gk4[i], 0), (long)DT_SNUL) + x86("mov", GQ(gk4[i], 8), (long)0)
                        + x86_deflabel_id(41 + i); })
             + x86("lea", "rcx", "extlbl", (uint64_t)(uintptr_t)lbl_b)
             + x86("lea", "rax", "extlbl", (uint64_t)(uintptr_t)lbl_o)
             + IF(fnrbp() == 2, x86("comment", "s64 RSP-ONLY WRITER (Lon challenge: zero RBP): push the 16B {gamma,omega} pair at TOS — [rsp+0]=gamma [rsp+8]=omega, body entered at P-16 (16-parity kept).  NO anchor register: the floaters find the pair by the DEPTH-INVARIANCE LAW — control transfers only at depth-neutral statement boundaries; MATCH banks its own mark in the r12 arena; the alpha-sub/omega-add pairing releases statement temporaries.  A statement shape that leaks (the s58 -16 census class) breaks the law and dies loud at the floater's jmp — under this arm the red set IS the leak census.")
                         + x86("push", "rax")
                         + x86("push", "rcx"))
             + x86("jmp", "[rip@cell + __]", body_cell, blb.c_str())
             + x86_def_ext(lbl_b)
             + x86("note", gva_name(rgx)) + x86("mov", "rdi", GQ(rgx, 0)) + x86("mov", "rsi", GQ(rgx, 8))
             + RESTORE4(80)
             + x86("mov32", "eax", T4 + 32 + 16L * nf4)
             + x86("cmp", "rcx", (long)nf4) + x86_jcc_id("jbe", 3) + SCALE16() + x86("add", "rax", T4 + 32) + x86_deflabel_id(3)
             + x86("mov", "rcx", R8Q(16L * nf4 + 16))
             + x86("add", "rsp", "rax")
             + x86("mov", "rax", "rdi") + x86("mov", "rdx", "rsi")
             + x86("jmp", "rcx")
             + x86_def_ext(lbl_o)
             + RESTORE4(150)
             + x86("mov32", "eax", T4 + 32 + 16L * nf4)
             + x86("cmp", "rcx", (long)nf4) + x86_jcc_id("jbe", 4) + SCALE16() + x86("add", "rax", T4 + 32) + x86_deflabel_id(4)
             + x86("mov", "rcx", R8Q(16L * nf4 + 24))
             + x86("add", "rsp", "rax")
             + x86("mov32", "eax", (long)DT_FAIL) + x86("xor", "edx", "edx")
             + x86("jmp", "rcx")
             + IF(inl5, x86_deflabel_id(245))
             + IF(!inl5, x86_gamma());
    }
    if (role == 1 || role == 2 || role == -1 ) {
        uint64_t _rtn_fp; { void (*_f)(int) = rt_kw_set_rtntype_role; _rtn_fp = (uint64_t)(uintptr_t)(void *)_f; }
        std::string rtn_set = x86("mov", "edi", (long)role) + x86("call", "rt_kw_set_rtntype_role", _rtn_fp);
        if (fnrbp() == 2)
            return x86("comment", role == 1 ? "IR_DEFINE RETURN floater (s64 RSP-ONLY: pop {gamma,omega} pair at TOS — depth IS the anchor)" :
                                   role == 2 ? "IR_DEFINE FRETURN floater (s64 RSP-ONLY: skip gamma, pop omega — depth IS the anchor)" :
                                               "IR_DEFINE NRETURN floater (s64 RSP-ONLY: pop gamma — by-name result)")
                 + x86_alpha()
                 + rtn_set
                 + (role == 2 ? x86("add", "rsp", (long)8) + x86("pop", "rcx")
                              : x86("pop", "rcx") + x86("add", "rsp", (long)8))
                 + x86("jmp", "rcx");
        return x86("comment", role == 1 ? "IR_DEFINE RETURN floater (s58: BOMB — coming-out frozen)" :
                               role == 2 ? "IR_DEFINE FRETURN floater (s58: BOMB — coming-out frozen)" :
                                           "IR_DEFINE NRETURN floater (s58: BOMB — coming-out frozen)")
             + x86_alpha()
             + (role == 1 ? x86_bomb("BOMB-RETURN: descent complete, coming-out frozen (s58 RSP-only) — UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor")
              : role == 2 ? x86_bomb("BOMB-FRETURN: descent complete, coming-out frozen (s58 RSP-only) — UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor")
                          : x86_bomb("BOMB-NRETURN: descent complete, coming-out frozen (s58 RSP-only) — UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"));
    }
    int c2np = 0, c2nsave = 0, c2res_gk = -1; int c2gk[64]; long c2nargs = (long)_.op_arg_slot_n;
    int elig = bb_scc_probe(_.op_sval, (int)c2nargs, &c2np, &c2nsave, c2gk, &c2res_gk);
    for (int i = 0; elig && i < (int)c2nargs; i++) if (_.op_arg_slot[i] < 0) elig = 0;
    if (elig && !bb_scc_handoff_room()) elig = 0;
    if (!elig) { bb_scc_handoff_pending_clear(); return x86("comment", "IR_DEFINE role 0: site not SCC-eligible (or an arg slot unresolved, or no handoff room) — pass-through; the staged call box runs its full merged body")
             + x86_alpha()
             + x86_gamma(); }
    long sb = 16L * (long)c2nsave;
    uint64_t slim_fp; { long (*fp)(const char *, int, int) = rt_proc_call_open_slim; slim_fp = (uint64_t)(uintptr_t)(void *)fp; }
    std::string s = x86("comment", "IR_DEFINE role 0 (CALL2BB slice 2): own sub-rsp carve, save-set spill (GVA -> own slots), open_slim, staged args -> NV globals; outcome rides rax into the staged call box")
         + x86_alpha()
         + x86_zclaim(sb)
         + FOR(0, c2nsave, [&](int k) {
               return x86("note", gva_name(c2gk[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(c2gk[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)c2gk[k] * 16)) + x86_rsp_store64(16 * k, "rax")
                    + x86("note", gva_name(c2gk[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(c2gk[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)c2gk[k] * 16 + 8)) + x86_rsp_store64(16 * k + 8, "rax"); })
         + x86_scan_sync_out()
         + x86_ro_load_q("rdi", 0)
         + x86("mov32", "esi", (long)c2np)
         + x86("mov32", "edx", c2nargs)
         + x86("call", "rt_proc_call_open_slim", slim_fp)
         + x86_scan_sync_in_rr()
         + x86("test", "rax", "rax")
         + x86("jne", L(1))
         + x86_zrelease(sb)
         + x86("jmp", L(2))
         + x86("def", L(1))
         + FOR(0, (int)c2nargs, [&](int i) { int slot = _.op_arg_slot[i];
               return x86("mov", "rax", FRQB(slot, (int)sb))
                    + x86("note", gva_name(c2gk[i])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(c2gk[i], 0) : ABSQ(RT_GVA_VA + (unsigned long)c2gk[i] * 16), "rax")
                    + x86("mov", "rax", FRQB(slot + 8, (int)sb))
                    + x86("note", gva_name(c2gk[i])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(c2gk[i], 8) : ABSQ(RT_GVA_VA + (unsigned long)c2gk[i] * 16 + 8), "rax"); })
         + x86("mov32", "eax", 1L)
         + x86("def", L(2))
         + x86_gamma()
         + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");
    if (!bb_scc_handoff_arm()) return x86_alpha() + x86_bomb("IR_DEFINE role 0: handoff arm failed after the room check passed (impossible unless the drive deposit raced)");
    return s;
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
std::string bb_define() { int r = g_emit.op_define_role; if (r == 6) return bb_define_bind(); if (r == 7) return bb_define_activate(); return bb_define_sr(); }
