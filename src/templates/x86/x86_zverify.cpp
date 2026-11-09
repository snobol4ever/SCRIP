#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "templates/x86/x86_zverify.h"
#include "templates/x86/x86_zeta.h"
#include "emitter/emit_medium.h"
#include "ir/ir_node.h"
#include "emitter/emit_state.h"
#include "emitter/emit_regime.h"
#include "ir/ir_node_index.h"
#include "templates/x86/x86_call.h"
#include "templates/x86/x86_frame.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_ops.h"
#include "ir/ir_graph.h"
#include "planner/frame_layout.h"
extern "C" std::string bb_zdp_probe(long op, long node, long port, long expect, long want_rbp);
extern "C" void rt_zdp_ev(void);
extern "C" void rt_zdp_sm_init(void);
extern "C" int bb_node_id(IR_t * nd);
extern "C" int emit_zrow_adead(const IR_t * nd);
int x86_key_unbacked(int off) { return (g_emit_cfg && (g_emit_cfg->pf.keys_only || (g_emit_cfg->pf.home_frame && off >= g_emit_cfg->pf.kbase))) ? 1 : 0; }
int x86_zop_regime(int off) { if (x86_fc_hit(off)) return 2; return x86_fb_data() ? 3 : 4; }
static void x86_zop_note(int r) { if (r < 1 || r > 5) return; _.zop_seen |= (1 << r); }
static void x86_zsm_frame_addr(int off, int cls, int rsp_form) { emit_zsm_frame_addr(g_emit_cfg, _.node, off, cls, rsp_form); }
static int x86_zop_operand_hit(int k, int off, int wide_ok) {
    int s = _.op_zslot[k];
    int wide = wide_ok && off > s + 8 && (off - s) % 8 == 0 && _.op_zkown[k] >= 0;
    return s >= 0 && (off == s || off == s + 8 || (wide && off < s + _.op_zkown[k]));
}
const char * x86_zop(int off, int q, int bump) {
    static char b[16][48]; static int i; i = (i + 1) & 15;
    if (off >= ZST_BIAS / 2 && !_.op_zdef && x86_resumable_frame()) { x86_zop_note(2); x86_zsm_frame_addr(-1, 2, 0); snprintf(b[i], 48, "%s ptr [rbp + %d]", q ? "qword" : "dword", off - ZST_BIAS);
        return b[i]; }
    if (off >= ZST_BIAS / 2) { x86_zop_note(2); snprintf(b[i], 48, "%s ptr [rsp# + %d]", q ? "qword" : "dword", _.op_zabs + (off - ZST_BIAS) + bump); return b[i]; }
    if (_.op_zres && _.op_res_alias >= 0 && (off == _.op_res_alias || off == _.op_res_alias + 8)) { int d = off - _.op_res_alias;
        if (_.op_xf_off != -1) x86_zsm_frame_addr(-1, _.op_fres ? 2 : 3, 0);
        return _.op_xf_off != -1 ? (q ? RDQ("rbp", _.op_xf_off + d) : RDD("rbp", _.op_xf_off + d)) : (q ? ZRES_SPINE(d) : ZCELLD(d)); }
    for (int w = 0; w < 2; w++) for (int k = 0; _.op_zres && k < _.op_zslot_n && k < _.op_zcap; k++) if (x86_zop_operand_hit(k, off, w)) { int d = off - _.op_zslot[k];
        if (_.op_zread_xf[k] != -1) { x86_zsm_frame_addr(-1, _.op_zfres[k] ? 2 : 3, 0);
            return q ? RDQ("rbp", _.op_zread_xf[k] + d) : RDD("rbp", _.op_zread_xf[k] + d); }
        if (_.op_zread_fr[k] >= 0) return x86_zop(_.op_zread_fr[k] + d, q, bump);
        return x86_zref(_.op_zread[k] + d, q); }
    if (_.op_zres && _.node && x86_key_unbacked(off)) { int own = _.op_own, kw = _.op_kown;
        if (own >= 0 && off >= own && off < own + kw) return q ? ZRES_SPINE(off - own) : ZCELLD(off - own); }
    int r = x86_zop_regime(off); x86_zop_note(r); int eff, spine;
    if (r != 2 && x86_key_unbacked(off) && _.op_zres && _.op_xa > 0 && off >= _.op_xk && off < _.op_xk + _.op_xw) return q ? RDQ("rbp", off - _.op_xk - _.op_xa) : RDD("rbp", off - _.op_xk - _.op_xa);
    if (r != 2 && x86_key_unbacked(off) && _.op_zres) { if (_.op_xd >= 0 && off >= _.op_xk && off < _.op_xk + _.op_xw) return q ? ZCELL(_.op_xd + off - _.op_xk) : ZCELLD(_.op_xd + off - _.op_xk); }
    if (r != 2 && x86_key_unbacked(off) && _.op_zres && _.node && emit_zrow_adead(_.node)) return q ? ZRES_SPINE(0) : ZCELLD(0);
    if (r != 2 && x86_key_unbacked(off))
        { fprintf(stderr,
            "FATAL x86_zop: offset %d is a key with no frame bytes behind it, addressed through the frame -- a keys-only graph, or a key past a home-frame graph's resident region (zls_key_unbacked, "
            "R-1864/1884) op=%d regime=%d zres=%d own=%d k_own=%d\n",
            off, _.node ? (int) _.node->op : -1, r, _.op_zres, _.node ? _.op_own : -1, _.node ? _.op_kown : -1);
            fprintf(stderr, "FATAL x86_zop: at node %s own=%d k_own=%d op_sa=%d op_off=%d\n", _.node ? bb_op_name(_.node->op) : "NONE", _.node ? _.op_own : -1, _.node ? _.op_kown : -1,
                _.op_sa, _.op_off);
            abort(); }
    if (r != 2 && icn_frame_rbp()) { x86_zsm_frame_addr(off, -1, 0); return q ? RDQ("rbp", off - gen_frame_top()) : RDD("rbp", off - gen_frame_top()); }
    if (r == 2) { if (getenv("SCRIP_ZOP_DIAG")) fprintf(stderr, "[ZOP] off=%d op_fc_base=%d bump=%d computed=%d\n", off, _.op_fc_base, bump, off - _.op_fc_base + bump);
        eff = off - _.op_fc_base + bump; spine = 1; }
    else if (x86_resumable_frame() && x86_hb_on() && !x86_fb_data() && !_.op_stmt_dyn) { x86_zsm_frame_addr(off, -1, 0); x86_zop_note(3);
        snprintf(b[i], 48, "%s ptr [rbp + %d]", q ? "qword" : "dword", off); return b[i]; }
    else if (g_emit.zframe_graph && g_emit.frame_retained && x86_hb_on() && !_.op_stmt_dyn) { x86_zsm_frame_addr(off, -1, 0); x86_zop_note(3);
        snprintf(b[i], 48, "%s ptr [rbp + %d]", q ? "qword" : "dword", off); return b[i]; }
    else if (bump && !x86_fb_data() && !_.op_stmt_dyn) { x86_zsm_frame_addr(off, -1, 1); eff = x86_frame_off(off) + bump; spine = 1; }
    else if (_.op_zf_unpinned && _.op_zres && !x86_fb_data() && !_.op_stmt_dyn) { x86_zsm_frame_addr(off, -1, 1); eff = off + _.op_zabs; spine = 1; }
    else { x86_zsm_frame_addr(off, -1, 0); eff = off + ((x86_fb_data() || _.op_stmt_dyn) ? 0 : bump); spine = 0; }
    if (spine) snprintf(b[i], 48, "%s ptr [rsp# + %d]", q ? "qword" : "dword", eff);
    else snprintf(b[i], 48, "%s%d]", q ? x86_fr64_prefix() : x86_fr32_prefix(), eff);
    return b[i];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_zop_base(int off, int q, int bump, const char * base) {
    const char * a = x86_zop(off, q, bump); const char * p = strstr(a, "[rsp# + "); if (!p) p = strstr(a, "[rsp + ");
    if (!p) return std::string(a);
    return std::string(q ? "qword ptr [" : "dword ptr [") + base + " + " + std::to_string(atoi(p + (p[4] == '#' ? 8 : 7))) + "]";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * x86_ztos(int off, int q) {
    static char b[16][48]; static int i; i = (i + 1) & 15; x86_zop_note(5);
    snprintf(b[i], 48, "%s ptr [rsp + %d]", q ? "qword" : "dword", off + _.op_zdepth);
    return b[i];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * ZTOS(int off) { return x86_ztos(off, 1); }
static const char * ZTOSD(int off) { return x86_ztos(off, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * x86_zref(int off, int q) {
    static char b[16][48]; static int i; i = (i + 1) & 15;
    snprintf(b[i], 48, "%s ptr [rsp# + %d]", q ? "qword" : "dword", off);
    return b[i];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x86_zdp_on() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZDP_TEARDOWN"); v = (e && *e == '1') ? 1 : 0; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_zdp_probe_at(int port) {
    if (!x86_zdp_on()) return std::string();
    if (port != X86P_ALPHA && port != X86P_BETA) return std::string();
    long expect = (port == X86P_ALPHA) ? (long)_.op_zdp_ad : (long)_.op_zdp_bd;
    long want_rbp = _.op_zdp_rbp ? ((port == X86P_BETA) ? 1L : 2L) : 0L;
    return bb_zdp_probe((long)_.op_node_kind, (long)_.nid, (port == X86P_ALPHA) ? 1L : 2L, expect, want_rbp);
}
int x86_zdp_rbp_on() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZSM"); v = (e && *e == '1') ? 1 : 0; if (v) rt_zdp_sm_init(); } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x86_zdp_rbp_frames() {
    if (!_.op_zdp_rbp) return 0;
    if (_.op_node_kind == (int)IR_MATCH_BEGIN) return 1;
    if (_.op_node_kind == (int)IR_MATCH_DEFER) return (_.op_seal == 1) ? 1 : 0;
    return 0;
}
static long x86_zsm_owner(const IR_t * nd) { if (!nd || (nd->op != IR_MATCH_END && nd->op != IR_MATCH_REPLACE) || nd->n_operands < 1 || !nd->operands[0] || nd->operands[0]->op != IR_MATCH_BEGIN)
    return 0L;
    return (long)bb_node_id(nd->operands[0]); }
std::string x86_zsm_ev(int kind) {
    long own = x86_zsm_owner(_.node);
    return x86("comment", "ZSM")
         + x86("push", "rax") + x86("push", "rax")
         + x86("push", "rdi") + x86("push", "rsi") + x86("push", "rdx") + x86("push", "rcx")
         + x86("push", "r8") + x86("push", "r9") + x86("push", "r10") + x86("push", "r11")
         + x86("mov", "rdx", "rsp")
         + x86("add", "rdx", 80L)
         + x86("mov", "rdi", (long)_.nid)
         + x86("mov", "rsi", "rbp")
         + (own > 0L ? x86_movabs_r64("rcx", (uint64_t)(kind | ((long)_.op_node_kind << 8)) | ((uint64_t)own << 24)) : x86("mov", "rcx", (long)(kind | ((long)_.op_node_kind << 8))))
         + x86("mov", "r8", (!_.op_zslid && !_.flat_jmp_entry) ? (long)_.op_zdepth : -1L)
         + x86_movabs_r64("r9", (uint64_t)emit_zsm_word(_.node, _.op_zres, _.op_subj_cell, own) | ((uint64_t)(_.op_xrec & 12) << 50))
         + x86("call_bare", "rt_zdp_ev", (uint64_t)(uintptr_t)(void *)rt_zdp_ev)
         + x86("pop", "r11") + x86("pop", "r10") + x86("pop", "r9") + x86("pop", "r8")
         + x86("pop", "rcx") + x86("pop", "rdx") + x86("pop", "rsi") + x86("pop", "rdi")
         + x86("pop", "rax") + x86("pop", "rax");
}
std::string x86_zsm_disc(long restore) {
    return x86("comment", "ZSM disc")
         + x86("push", "rax") + x86("push", "rax")
         + x86("push", "rdi") + x86("push", "rsi") + x86("push", "rdx") + x86("push", "rcx")
         + x86("push", "r8") + x86("push", "r9") + x86("push", "r10") + x86("push", "r11")
         + x86("mov", "rdx", "rsp")
         + x86("add", "rdx", 80L)
         + x86("mov", "rdi", (long)_.nid)
         + x86("mov", "rsi", "rbp")
         + x86("mov", "rcx", (long)(10 | ((long)_.op_node_kind << 8)))
         + x86("mov", "r8", _.op_zslid ? -1L : restore)
         + x86("mov", "r9", 0L)
         + x86("call_bare", "rt_zdp_ev", (uint64_t)(uintptr_t)(void *)rt_zdp_ev)
         + x86("pop", "r11") + x86("pop", "r10") + x86("pop", "r9") + x86("pop", "r8")
         + x86("pop", "rcx") + x86("pop", "rdx") + x86("pop", "rsi") + x86("pop", "rdi")
         + x86("pop", "rax") + x86("pop", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x86_zsm_all() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZSM_ALL"); v = (e && *e == '1') ? 1 : 0; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_zdp_rbp_at(int port) {
    if (!x86_zdp_rbp_on()) return std::string();
    if (x86_zdp_rbp_frames()) { if (port == X86P_ALPHA) return x86_zsm_ev(1); if (port == X86P_BETA) return x86_zsm_ev(2); return std::string(); }
    if (!x86_zsm_all()) return std::string();
    if (port == X86P_ALPHA) return x86_zsm_ev(5);
    if (port == X86P_BETA) return x86_zsm_ev(6);
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_zdp_rbp_pre_alpha() {
    if (!x86_zdp_rbp_on() || !(x86_zdp_rbp_frames() || x86_zsm_all())) return std::string();
    return x86_zsm_ev(9);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_zdp_rbp_omega_at(int port) {
    if (!x86_zdp_rbp_on()) return std::string();
    if (port != X86P_OMEGA) return std::string();
    if (x86_zdp_rbp_frames()) return x86_zsm_ev(3);
    if (x86_zsm_all()) return x86_zsm_ev(7);
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_zdp_rbp_gamma_at(int port) {
    if (!x86_zdp_rbp_on()) return std::string();
    if (port != X86P_GAMMA) return std::string();
    if (x86_zdp_rbp_frames()) return x86_zsm_ev(4);
    if (x86_zsm_all()) return x86_zsm_ev(8);
    return std::string();
}
std::string x86_beta_zchk() {
    if (!getenv("SCRIP_ZD_RTCHECK") || !_.node || _.node->op != IR_TO || !x86_resumable_frame()) return std::string();
    int e = emit_zrow_xabs(_.node); if (e == (-2147483647 - 1)) return std::string();
    return x86("lea", "r11", RDQ("rbp", -e)) + x86("cmp", "rsp", "r11") + x86("je", L(986)) + (MEDIUM_BINARY ? x86_Lrec(x86_b2(0x0F, 0x0B)) : x86_recn("ud2") + "\n") + x86("def", L(986));
}
std::string x86_alpha_zchk() {
    if (_.node && emit_zrow_adead(_.node)) return x86_bomb("alpha of a beta-only box: the plan found no alpha landing on its run (ADEAD)");
    if (!_.node || !x86_resumable_frame()) return std::string();
    int e = emit_zrow_zchk(_.node); if (e == (-2147483647 - 1)) return std::string();
    return x86("lea", "r11", RDQ("rbp", -e)) + x86("cmp", "rsp", "r11") + x86("je", L(987)) + (MEDIUM_BINARY ? x86_Lrec(x86_b2(0x0F, 0x0B)) : x86_recn("ud2") + "\n") + x86("def", L(987));
}
std::string x86_step_zchk() { return MEDIUM_BINARY ? x86_Lrec(x86_b2(0x0F, 0x0B)) : x86_recn("ud2") + "\n"; }
