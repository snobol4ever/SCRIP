#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
DESCR_t rt_call_arr(const char * fn, DESCR_t * args, int nargs);
DESCR_t rt_call_arr_bl(const char * fn, DESCR_t * args, int nargs, int bidlen);
extern "C" int zls_g_resume(const IR_graph_t *);
extern "C" {
#include "builtin_ids.h"
}
static int bid_bake_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_BID_BAKE"); v = (e && *e == '0') ? 0 : 1; } return v; }
static long bid_bake_of(const char * fn) { if (!bid_bake_on() || !fn) return -1L; size_t n = strlen(fn); if (n > 0xFFFFu) return -1L; return (long)(((unsigned long)n << 16) | (unsigned long)(unsigned)bid_of(fn, (unsigned)n)); }
DESCR_t rt_pl_dop_unify_ci(DESCR_t * args, long long imm);
DESCR_t rt_pl_dop_unify_cs(DESCR_t * args, const char * cs);
int bb_slot_get(IR_t * nd);
void bb_slot_register(IR_t * nd, int off);
}
#include "x86_asm.h"
std::string marshal_call_arg(IR_t * lf, IR_graph_t * sg, int aoff, IR_t * owner, int idx);
void * dop_direct_fp(const char * fn, int64_t narg, const char ** sym);
extern "C" char g_pl_trail[];
extern "C" char g_hp_fr[];
extern "C" uint32_t g_plw_dot_sl;
extern "C" int g_plw_cellws_on;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string sink_deref(const char * reg, int lh, int ld, int ln2) {
    return x86_deflabel_id(lh)
         + x86("mov", "eax", (std::string("dword ptr [") + reg + " + 0]").c_str())
         + x86("cmp", "al", (long)DT_N)  + x86_jcc_id("jne", ln2 + 1)
         + x86("mov", "esi", (std::string("dword ptr [") + reg + " + 4]").c_str())
         + x86("mov", "rax", (std::string("[") + reg + " + 8]").c_str())
         + x86("test", "rax", "rax")   + x86_jcc_id("je", ld)
         + x86("cmp", "esi", (long)1)  + x86_jcc_id("jne", ln2)
         + x86("mov", reg, "rax")
         + x86_jmp_id(lh)
         + x86_deflabel_id(ln2)
         + x86("cmp", "esi", (long)2)  + x86_jcc_id("jne", ld)
         + x86("mov", "rax", "[rax + 0]")
         + x86("test", "rax", "rax")   + x86_jcc_id("je", ld)
         + x86("mov", reg, "rax")
         + x86_jmp_id(lh)
         + x86_deflabel_id(ln2 + 1)
         + x86("cmp", "al", (long)DT_PLVAR) + x86_jcc_id("jne", ld)
         + x86("mov", "rax", (std::string("[") + reg + " + 8]").c_str())
         + x86("test", "rax", "rax")   + x86_jcc_id("je", ld)
         + x86("cmp", "rax", reg)      + x86_jcc_id("je", ld)
         + x86("mov", reg, "rax")
         + x86_jmp_id(lh)
         + x86_deflabel_id(ld);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string sink_unb(const char * reg, int lyes, int lno) {
    return x86("mov", "eax", (std::string("dword ptr [") + reg + " + 0]").c_str())
         + x86("cmp", "al", (long)DT_SNUL)  + x86_jcc_id("je", lyes)
         + x86("cmp", "al", (long)DT_FAIL) + x86_jcc_id("je", lyes)
         + x86("cmp", "al", (long)DT_PLVAR) + x86_jcc_id("jne", lno)
         + x86("mov", "rax", (std::string("[") + reg + " + 8]").c_str())
         + x86("cmp", "rax", reg)      + x86_jcc_id("je", lyes)
         + x86_jmp_id(lno);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string sink_trailpush(const char * creg, int lslow) {
    return x86("lea", "r12", "[rip + __]", (uint64_t)(uintptr_t)g_pl_trail, "g_pl_trail")
         + x86("mov", "rcx", "[r12 + 0]")
         + x86("test", "rcx", "rcx")   + x86_jcc_id("je", lslow)
         + x86("mov", "eax", "dword ptr [r12 + 32]")
         + x86("mov32", "esi", (long)24)
         + x86("imul", "rsi", "rax")
         + x86("mov", "rax", "[r12 + 24]")
         + x86("sub", "rax", (long)24)
         + x86("cmp", "rsi", "rax")    + x86_jcc_id("ja", lslow)
         + x86("add", "rcx", "rsi")
         + x86("mov", "[rcx + 0]", creg)
         + x86("mov", "rax", (std::string("[") + creg + " + 0]").c_str())
         + x86("mov", "[rcx + 8]", "rax")
         + x86("mov", "rax", (std::string("[") + creg + " + 8]").c_str())
         + x86("mov", "[rcx + 16]", "rax")
         + x86("mov", "eax", "dword ptr [r12 + 32]")
         + x86("add", "eax", (long)1)
         + x86("mov", "dword ptr [r12 + 32]", "eax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string sink_cp16(const char * dst, const char * src) {
    return x86("mov", "rax", (std::string("[") + src + " + 0]").c_str())
         + x86("mov", (std::string("[") + dst + " + 0]").c_str(), "rax")
         + x86("mov", "rax", (std::string("[") + src + " + 8]").c_str())
         + x86("mov", (std::string("[") + dst + " + 8]").c_str(), "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string sink_tp_nc(const char * creg) {
    return x86("mov", "rdi", "[r12 + 0]")
         + x86("mov", "eax", "dword ptr [r12 + 32]")
         + x86("mov32", "esi", (long)24)
         + x86("imul", "rsi", "rax")
         + x86("add", "rdi", "rsi")
         + x86("mov", "[rdi + 0]", creg)
         + x86("mov", "rax", (std::string("[") + creg + " + 0]").c_str())
         + x86("mov", "[rdi + 8]", "rax")
         + x86("mov", "rax", (std::string("[") + creg + " + 8]").c_str())
         + x86("mov", "[rdi + 16]", "rax")
         + x86("mov", "eax", "dword ptr [r12 + 32]")
         + x86("add", "eax", (long)1)
         + x86("mov", "dword ptr [r12 + 32]", "eax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string sink_carve48(int lslow) {
    return x86("lea", "r12", "[rip + __]", (uint64_t)(uintptr_t)g_hp_fr, "g_hp_fr")
         + x86("mov", "eax", "dword ptr [r12 + 24]")
         + x86("test", "eax", "eax")            + x86_jcc_id("je", lslow)
         + x86("mov", "rdi", "[r12 + 0]")
         + x86("mov", "rax", "[r12 + 8]")
         + x86("sub", "rax", (long)48)
         + x86("cmp", "rdi", "rax")             + x86_jcc_id("ja", lslow);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string sink_carve48_take(void) {
    return x86("lea", "r12", "[rip + __]", (uint64_t)(uintptr_t)g_hp_fr, "g_hp_fr")
         + x86("mov", "rdi", "[r12 + 0]")
         + x86("mov", "[rdi + 0]", (long)0)
         + x86("mov", "dword ptr [rdi + 8]", (long)48)
         + x86("mov", "dword ptr [rdi + 12]", (long)(209 | (1 << 16)))
         + x86("lea", "rdx", "[rdi + 16]")
         + x86("mov", "rax", "rdi")
         + x86("add", "rax", (long)48)
         + x86("mov", "[r12 + 0]", "rax")
         + x86("mov", "rax", "[r12 + 16]")
         + x86("add", "rax", (long)1)
         + x86("mov", "[r12 + 16]", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string sink_kid(const char * creg, int koff, int lunb, int lbnd, int ljoin) {
    std::string s = sink_unb(creg, lunb, lbnd);
    s += x86_deflabel_id(lunb);
    s += x86("mov", (std::string("[rdx + ") + std::to_string(koff) + "]").c_str(), (long)DT_PLVAR);
    s += x86("lea", "rax", (std::string("[rdx + ") + std::to_string(koff) + "]").c_str());
    s += x86("mov", (std::string("[rdx + ") + std::to_string(koff + 8) + "]").c_str(), "rax");
    s += x86("lea", "r12", "[rip + __]", (uint64_t)(uintptr_t)g_pl_trail, "g_pl_trail");
    s += sink_tp_nc(creg);
    s += x86("mov", (std::string("[") + creg + " + 0]").c_str(), (long)DT_PLVAR);
    s += x86("lea", "rax", (std::string("[rdx + ") + std::to_string(koff) + "]").c_str());
    s += x86("mov", (std::string("[") + creg + " + 8]").c_str(), "rax");
    s += x86_jmp_id(ljoin);
    s += x86_deflabel_id(lbnd);
    s += x86("mov", "rax", (std::string("[") + creg + " + 0]").c_str());
    s += x86("mov", (std::string("[rdx + ") + std::to_string(koff) + "]").c_str(), "rax");
    s += x86("mov", "rax", (std::string("[") + creg + " + 8]").c_str());
    s += x86("mov", (std::string("[rdx + ") + std::to_string(koff + 8) + "]").c_str(), "rax");
    s += x86_deflabel_id(ljoin);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string sink_unify2_str(int argbase, uint64_t ufp, const char * usym, int zd_rsp = -1) {
    auto ZDFRQ = [&](int n) -> const char * { return zd_rsp >= 0 ? x86_zref(zd_rsp+n, 1) : FRQ(argbase+n); };
    std::string s = x86("comment", "PL-SINK-1 inline $unify fast path: deref/bind/trail/int-eq emitted; rt_pl_dop_unify stays the slow-path oracle (bit-identical fallback, unmodified args)");
    s += (zd_rsp >= 0 ? x86_reg_disp32_lea64("rdi", "rsp", zd_rsp) : x86("lea", "rdi", FRQ(argbase)));
    s += (zd_rsp >= 0 ? x86_reg_disp32_lea64("r8",  "rsp", zd_rsp) : x86("lea", "r8",  FRQ(argbase)));
    s += sink_deref("r8", 40, 41, 55);
    s += x86("lea", "r9",  ZDFRQ(16));
    s += sink_deref("r9", 42, 43, 57);
    s += x86("cmp", "r8", "r9");
    s += x86_jcc_id("je", 51);
    s += sink_unb("r8", 44, 45);
    s += x86_deflabel_id(44);
    s += sink_unb("r9", 53, 46);
    s += x86_deflabel_id(46);
    s += sink_trailpush("r8", 53);
    s += sink_cp16("r8", "r9");
    s += x86_jmp_id(51);
    s += x86_deflabel_id(45);
    s += sink_unb("r9", 47, 48);
    s += x86_deflabel_id(47);
    s += sink_trailpush("r9", 53);
    s += sink_cp16("r9", "r8");
    s += x86_jmp_id(51);
    s += x86_deflabel_id(48);
    s += x86("mov", "ecx", "dword ptr [r8 + 0]");
    s += x86("mov", "edx", "dword ptr [r9 + 0]");
    s += x86("cmp", "cl", (long)DT_PLREF);
    s += x86_jcc_id("jne", 49);
    s += x86("cmp", "dl", (long)DT_PLREF);
    s += x86_jcc_id("je", 53);
    s += x86_jmp_id(52);
    s += x86_deflabel_id(49);
    s += x86("cmp", "dl", (long)DT_PLREF);
    s += x86_jcc_id("je", 52);
    s += x86("cmp", "cl", (long)DT_R);
    s += x86_jcc_id("je", 53);
    s += x86("cmp", "dl", (long)DT_R);
    s += x86_jcc_id("je", 53);
    s += x86("cmp", "cl", (long)DT_I);
    s += x86_jcc_id("jne", 50);
    s += x86("cmp", "dl", (long)DT_I);
    s += x86_jcc_id("jne", 50);
    s += x86("mov", "rax", "[r8 + 0]");
    s += x86("cmp64", "rax", (long)DT_I);
    s += x86_jcc_id("jne", 53);
    s += x86("mov", "rax", "[r9 + 0]");
    s += x86("cmp64", "rax", (long)DT_I);
    s += x86_jcc_id("jne", 53);
    s += x86("mov", "rax", "[r8 + 8]");
    s += x86("mov", "rsi", "[r9 + 8]");
    s += x86("cmp", "rax", "rsi");
    s += x86_jcc_id("je", 51);
    s += x86_jmp_id(52);
    s += x86_deflabel_id(50);
    s += x86("mov", "rax", "[r8 + 0]");
    s += x86("mov", "rsi", "[r9 + 0]");
    s += x86("cmp", "rax", "rsi");
    s += x86_jcc_id("jne", 53);
    s += x86("mov", "rax", "[r8 + 8]");
    s += x86("mov", "rsi", "[r9 + 8]");
    s += x86("cmp", "rax", "rsi");
    s += x86_jcc_id("jne", 53);
    s += x86_deflabel_id(51);
    s += x86("mov", "rax", "[r8 + 0]");
    s += x86("mov", "rdx", "[r8 + 8]");
    s += x86_jmp_id(54);
    s += x86_deflabel_id(52);
    s += x86("mov32", "eax", (long)DT_FAIL);
    s += x86("mov32", "edx", (long)0);
    s += x86_jmp_id(54);
    s += x86_deflabel_id(53);
    s += x86("mov32", "esi", (long)2);
    s += x86("call", usym, ufp);
    s += x86_deflabel_id(54);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string sink_unify_lst_str(int argbase, uint64_t ufp, const char * usym, int zd_rsp = -1) {
    auto ZDFRQ = [&](int n) -> const char * { return zd_rsp >= 0 ? x86_zref(zd_rsp+n, 1) : FRQ(argbase+n); };
    std::string s = x86("comment", "PL-SINK-2 inline $unify_lst READ-mode fast path: bound './2 subject + both-unbound-distinct H,T -> double bind; rt_pl_dop_unify_lst is the slow-path oracle (unmodified args)");
    s += (zd_rsp >= 0 ? x86_reg_disp32_lea64("rdi", "rsp", zd_rsp) : x86("lea", "rdi", FRQ(argbase)));
    s += (zd_rsp >= 0 ? x86_reg_disp32_lea64("r8",  "rsp", zd_rsp) : x86("lea", "r8",  FRQ(argbase)));
    s += sink_deref("r8", 60, 61, 62);
    s += sink_unb("r8", 80, 74);
    s += x86_deflabel_id(74);
    s += x86("mov", "ecx", "dword ptr [r8 + 0]");
    s += x86("cmp", "cl", (long)DT_PLREF);
    s += x86_jcc_id("jne", 73);
    s += x86("lea", "r12", "[rip + __]", (uint64_t)(uintptr_t)&g_plw_dot_sl, "g_plw_dot_sl");
    s += x86("mov", "eax", "dword ptr [r12 + 0]");
    s += x86("test", "eax", "eax");
    s += x86_jcc_id("je", 72);
    s += x86("mov", "edx", "dword ptr [r8 + 4]");
    s += x86("cmp", "eax", "edx");
    s += x86_jcc_id("jne", 73);
    s += x86("lea", "r9",  ZDFRQ(16));
    s += sink_deref("r9", 64, 65, 66);
    s += x86("lea", "rcx", ZDFRQ(32));
    s += sink_deref("rcx", 68, 69, 70);
    s += x86("cmp", "r9", "rcx");
    s += x86_jcc_id("je", 72);
    s += sink_unb("r9", 75, 72);
    s += x86_deflabel_id(75);
    s += sink_unb("rcx", 76, 72);
    s += x86_deflabel_id(76);
    s += x86("lea", "r12", "[rip + __]", (uint64_t)(uintptr_t)g_pl_trail, "g_pl_trail");
    s += x86("mov", "rdi", "[r12 + 0]");
    s += x86("test", "rdi", "rdi")   + x86_jcc_id("je", 72);
    s += x86("mov", "eax", "dword ptr [r12 + 32]");
    s += x86("mov32", "esi", (long)24);
    s += x86("imul", "rsi", "rax");
    s += x86("mov", "rax", "[r12 + 24]");
    s += x86("sub", "rax", (long)48);
    s += x86("cmp", "rsi", "rax")    + x86_jcc_id("ja", 72);
    s += x86("add", "rdi", "rsi");
    s += x86("mov", "[rdi + 0]", "r9");
    s += x86("mov", "rax", "[r9 + 0]");
    s += x86("mov", "[rdi + 8]", "rax");
    s += x86("mov", "rax", "[r9 + 8]");
    s += x86("mov", "[rdi + 16]", "rax");
    s += x86("mov", "[rdi + 24]", "rcx");
    s += x86("mov", "rax", "[rcx + 0]");
    s += x86("mov", "[rdi + 32]", "rax");
    s += x86("mov", "rax", "[rcx + 8]");
    s += x86("mov", "[rdi + 40]", "rax");
    s += x86("mov", "eax", "dword ptr [r12 + 32]");
    s += x86("add", "eax", (long)2);
    s += x86("mov", "dword ptr [r12 + 32]", "eax");
    s += x86("mov", "r12", "[r8 + 8]");
    s += x86("mov", "rax", "[r12 + 0]");
    s += x86("mov", "[r9 + 0]", "rax");
    s += x86("mov", "rax", "[r12 + 8]");
    s += x86("mov", "[r9 + 8]", "rax");
    s += x86("mov", "rax", "[r12 + 16]");
    s += x86("mov", "[rcx + 0]", "rax");
    s += x86("mov", "rax", "[r12 + 24]");
    s += x86("mov", "[rcx + 8]", "rax");
    s += x86("mov", "rax", "[r8 + 0]");
    s += x86("mov", "rdx", "[r8 + 8]");
    s += x86_jmp_id(77);
    s += x86_deflabel_id(80);
    s += x86("comment", "PL-SINK-3 inline $unify_lst WRITE mode: carve 2 kids off the PLJ frontier, join unbound args, bind subject to the './2 cell");
    s += x86("lea", "r12", "[rip + __]", (uint64_t)(uintptr_t)&g_plw_dot_sl, "g_plw_dot_sl");
    s += x86("mov", "eax", "dword ptr [r12 + 0]");
    s += x86("test", "eax", "eax");
    s += x86_jcc_id("je", 72);
    s += sink_carve48(72);
    s += x86("lea", "r12", "[rip + __]", (uint64_t)(uintptr_t)g_pl_trail, "g_pl_trail");
    s += x86("mov", "rdi", "[r12 + 0]");
    s += x86("test", "rdi", "rdi");
    s += x86_jcc_id("je", 72);
    s += x86("mov", "eax", "dword ptr [r12 + 32]");
    s += x86("mov32", "esi", (long)24);
    s += x86("imul", "rsi", "rax");
    s += x86("mov", "rax", "[r12 + 24]");
    s += x86("sub", "rax", (long)72);
    s += x86("cmp", "rsi", "rax");
    s += x86_jcc_id("ja", 72);
    s += sink_carve48_take();
    s += x86("lea", "r9", ZDFRQ(16));
    s += sink_deref("r9", 81, 82, 83);
    s += sink_kid("r9", 0, 85, 86, 87);
    s += x86("lea", "rcx", ZDFRQ(32));
    s += sink_deref("rcx", 88, 89, 90);
    s += sink_kid("rcx", 16, 92, 93, 94);
    s += x86("lea", "r12", "[rip + __]", (uint64_t)(uintptr_t)g_pl_trail, "g_pl_trail");
    s += sink_tp_nc("r8");
    s += x86("mov", "dword ptr [r8 + 0]", (long)DT_PLREF);
    s += x86("lea", "r12", "[rip + __]", (uint64_t)(uintptr_t)&g_plw_dot_sl, "g_plw_dot_sl");
    s += x86("mov", "eax", "dword ptr [r12 + 0]");
    s += x86("mov", "dword ptr [r8 + 4]", "eax");
    s += x86("mov", "[r8 + 8]", "rdx");
    s += x86("mov", "rax", "[r8 + 0]");
    s += x86_jmp_id(77);
    s += x86_deflabel_id(73);
    s += x86("mov32", "eax", (long)DT_FAIL);
    s += x86("mov32", "edx", (long)0);
    s += x86_jmp_id(77);
    s += x86_deflabel_id(72);
    s += x86("mov32", "esi", (long)3);
    s += x86("call", usym, ufp);
    s += x86_deflabel_id(77);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string sink_trail_mark_str(int argbase, uint64_t ufp, const char * usym, int zd_rsp = -1, int resoff = -1) {
    std::string s = x86("comment", "PL-SINK-8 inline $trail_mark fast path: guards prove the zh/cw mark push is a no-op, then mark = g_pl_trail.top; rt_pl_dop_trail_mark is the slow-path oracle (unmodified args)");
    if (resoff >= 0 && g_emit_cfg && g_emit_cfg->zframe_graph) {
        extern void *g_pl_zf_pending_cursor;
        s += x86("comment", "PL-FR-4 N0-SUPPRESS: if pending β-resume, lexprep2 already wrote correct trail mark — skip pl_trail_mark and use frame slot as-is");
        s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)&g_pl_zf_pending_cursor, "g_pl_zf_pending_cursor");
        s += x86("mov", "rax", "[rdi]");
        s += x86("test", "rax", "rax");
        s += x86_jcc_id("je", 102);
        { int _gres = zls_g_resume(g_emit_cfg);
          if (_gres >= 0) {
            s += x86("mov", FRQ(_gres), "rax");
          } }
        s += x86("mov", "rax", FRQ(resoff));
        s += x86("mov", "rdx", FRQ(resoff + 8));
        s += x86_jmp_id(101);
        s += x86_deflabel_id(102);
    }
    s += x86("lea", "r12", "[rip + __]", (uint64_t)(uintptr_t)&g_plw_cellws_on, "g_plw_cellws_on");
    s += x86("mov", "eax", "dword ptr [r12 + 0]");
    s += x86("test", "eax", "eax");
    s += x86_jcc_id("jne", 100);
    s += x86("lea", "r12", "[rip + __]", (uint64_t)(uintptr_t)g_pl_trail, "g_pl_trail");
    s += x86("mov", "eax", "dword ptr [r12 + 32]");
    s += x86("movsxd", "rdx", "eax");
    s += x86("mov32", "eax", (long)DT_I);
    s += x86_jmp_id(101);
    s += x86_deflabel_id(100);
    if (zd_rsp >= 0) s += x86_reg_disp32_lea64("rdi", "rsp", zd_rsp); else s += x86("lea", "rdi", FRQ(argbase));
    s += x86("mov32", "esi", (long)0);
    s += x86("call", usym, ufp);
    s += x86_deflabel_id(101);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string sink_ix_g_str(int argbase, uint64_t ufp, const char * usym, int kk, long long kival, int zd_rsp = -1) {
    auto ZDFRQ = [&](int n) -> const char * { return zd_rsp >= 0 ? x86_zref(zd_rsp+n, 1) : FRQ(argbase+n); };
    std::string s = x86("comment", "PL-SINK-4 inline $ix_g specialized guard (kk emit-time constant); rt_pl_dop_ix_g stays the slow-path oracle (unmodified args)");
    s += (zd_rsp >= 0 ? x86_reg_disp32_lea64("rdi", "rsp", zd_rsp) : x86("lea", "rdi", FRQ(argbase)));
    s += (zd_rsp >= 0 ? x86_reg_disp32_lea64("r8",  "rsp", zd_rsp) : x86("lea", "r8",  FRQ(argbase)));
    s += sink_deref("r8", 110, 111, 112);
    s += sink_unb("r8", 114, 118);
    s += x86_deflabel_id(118);
    s += x86("mov", "eax", "dword ptr [r8 + 0]");
    if (kk == 3) {
        s += x86("cmp", "al", (long)DT_PLREF) + x86_jcc_id("jne", 119);
        s += x86("lea", "r12", "[rip + __]", (uint64_t)(uintptr_t)&g_plw_dot_sl, "g_plw_dot_sl");
        s += x86("mov", "edx", "dword ptr [r12 + 0]");
        s += x86("cmp", "dl", (long)DT_SNUL)  + x86_jcc_id("je", 116);
        s += x86("mov", "esi", "dword ptr [r8 + 4]");
        s += x86("cmp", "esi", "edx")    + x86_jcc_id("jne", 115);
        s += x86_jmp_id(114);
        s += x86_deflabel_id(119);
        s += x86("cmp", "al", (long)DT_I)  + x86_jcc_id("jne", 120);
        s += x86("mov", "esi", "dword ptr [r8 + 4]");
        s += x86("cmp", "esi", (long)0)  + x86_jcc_id("jne", 114);
        s += x86_jmp_id(115);
        s += x86_deflabel_id(120);
        s += x86("cmp", "al", (long)DT_S)  + x86_jcc_id("jne", 114);
        s += x86("mov", "esi", "dword ptr [r8 + 4]");
        s += x86("cmp", "esi", (long)0)  + x86_jcc_id("jne", 114);
        s += x86("mov", "rdx", "[r8 + 8]");
        s += x86("test", "rdx", "rdx")   + x86_jcc_id("je", 114);
        s += x86_jmp_id(115);
    } else if (kk == 2) {
        s += x86("cmp", "al", (long)DT_PLREF) + x86_jcc_id("je", 115);
        s += x86("cmp", "al", (long)DT_I)  + x86_jcc_id("je", 114);
        s += x86("cmp", "al", (long)DT_S)  + x86_jcc_id("jne", 114);
        s += x86("mov", "esi", "dword ptr [r8 + 4]");
        s += x86("cmp", "esi", (long)0)  + x86_jcc_id("jne", 114);
        s += x86("mov", "rdx", "[r8 + 8]");
        s += x86("test", "rdx", "rdx")   + x86_jcc_id("je", 114);
        s += x86_jmp_id(116);
    } else {
        s += x86("cmp", "al", (long)DT_PLREF) + x86_jcc_id("je", 115);
        s += x86("cmp", "al", (long)DT_I)  + x86_jcc_id("jne", 114);
        s += x86("mov", "esi", "dword ptr [r8 + 4]");
        s += x86("cmp", "esi", (long)0)  + x86_jcc_id("jne", 114);
        s += x86_movabs_r64("rdx", (uint64_t)kival);
        s += x86("mov", "rsi", "[r8 + 8]");
        s += x86("cmp", "rsi", "rdx")    + x86_jcc_id("jne", 115);
        s += x86_jmp_id(114);
    }
    s += x86_deflabel_id(114);
    s += x86("mov32", "eax", (long)DT_I);
    s += x86("mov32", "edx", (long)1);
    s += x86_jmp_id(117);
    s += x86_deflabel_id(115);
    s += x86("mov32", "eax", (long)DT_FAIL);
    s += x86("mov32", "edx", (long)0);
    s += x86_jmp_id(117);
    s += x86_deflabel_id(116);
    s += x86("mov32", "esi", (long)3);
    s += x86("call", usym, ufp);
    s += x86_deflabel_id(117);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcfn_result_slot(IR_t * nd) {
    { int _s = nd ? zls_off(nd) : -1; if (_s >= 0) { if (bb_slot_get(nd) < 0) bb_slot_register(nd, _s); return _s; } }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_fn_str(IR_t * pBB) {
    const char * fn = _.op_sval ? _.op_sval : "";
    int nargs = (int) _.op_ival;
    if (_.op_zres) {
        std::string s = x86_alpha()
                      + x86("comment", std::string("BOX IR_CALL ZD-7 ") + fn + "(...) -> rt_call_arr [ZD: args from ZOPQ, result to ZRES]");
        if (nargs > 0) {
            s += x86("sub", "rsp", (long)(nargs * 16));
            for (int i = 0; i < nargs; i++) {
                s += x86("mov", "r8", ZOPQ(i, nargs * 16 + 0));
                s += x86("mov", x86_zref(i * 16 + 0, 1), "r8");
                s += x86("mov", "r8", ZOPQ(i, nargs * 16 + 8));
                s += x86("mov", x86_zref(i * 16 + 8, 1), "r8");
            }
        }
        bool zd_sank = false;
        if (nargs == 0 && !strcmp(fn, "$trail_mark") && !getenv("SCRIP_NO_SINK") && !getenv("SCRIP_NO_SINK8")) {
            const char * tm_sym = 0; void * tm_fp = dop_direct_fp(fn, 0, &tm_sym);
            if (tm_fp) { s += sink_trail_mark_str(0, (uint64_t)(uintptr_t)tm_fp, tm_sym, 0, _.op_off); zd_sank = true;
                if (_.op_off >= 0 && g_emit_cfg && g_emit_cfg->pl_cells_graph) { s += x86("mov", "r12", ZRES(0)); s += x86("mov", FRQ(_.op_off), "r12"); s += x86("mov", "rdi", ZRES(8)); s += x86("mov", FRQ(_.op_off + 8), "rdi"); }
            }
        }
        const char * zdsym = 0; void * zdfp = (nargs > 0) ? dop_direct_fp(fn, (int64_t)nargs, &zdsym) : (void *)0;
        if (zdfp && nargs == 2 && !strcmp(fn, "$unify") && !getenv("SCRIP_NO_SINK")) {
            s += sink_unify2_str(0, (uint64_t)(uintptr_t)zdfp, zdsym, 0);
        } else if (zdfp && nargs == 3 && !strcmp(fn, "$unify_lst") && !getenv("SCRIP_NO_SINK")) {
            s += sink_unify_lst_str(0, (uint64_t)(uintptr_t)zdfp, zdsym, 0);
        } else if (zdfp && nargs == 3 && !strcmp(fn, "$ix_g") && !getenv("SCRIP_NO_SINK") && !getenv("SCRIP_NO_SINK4")) {
            int z_ix_kk = 0; long long z_ix_kival = 0;
            if (_.node && _.node->n_operands >= 2) { IR_t * a1 = _.node->n_operands > 1 ? _.node->operands[1] : NULL; IR_t * a2 = _.node->n_operands > 2 ? _.node->operands[2] : NULL; if (a1 && a1->op == IR_LIT_INTEGER) { int kk = (int)(IR_LIT(a1).ival & 0xFF); if (kk == 3) z_ix_kk = 3; else if (kk == 2) z_ix_kk = 2; else if (kk == 1 && a2 && a2->op == IR_LIT_INTEGER) { z_ix_kk = 1; z_ix_kival = IR_LIT(a2).ival; } } }
            if (z_ix_kk > 0) { s += sink_ix_g_str(0, (uint64_t)(uintptr_t)zdfp, zdsym, z_ix_kk, z_ix_kival, 0); }
            else { s += x86("comment", (std::string("PL-REGAIN-2 direct det leaf under ZD: ") + zdsym + " (no by-name dispatch)").c_str()); s += x86_reg_disp32_lea64("rdi", "rsp", 0); s += x86("mov32", "esi", (long)nargs); s += x86("call", zdsym, (uint64_t)(uintptr_t)zdfp); }
        } else if (zdfp) {
            s += x86("comment", (std::string("PL-REGAIN-2 direct det leaf under ZD: ") + zdsym + " (no by-name dispatch)").c_str());
            s += x86_reg_disp32_lea64("rdi", "rsp", 0);
            s += x86("mov32", "esi", (long)nargs);
            s += x86("call", zdsym, (uint64_t)(uintptr_t)zdfp);
        } else if (!zd_sank) {
        {
            std::string fl = std::string(".L") + x86_boxkind() + "_rkfnzd" + std::to_string(g_flat_node_id++);
            s += x86("directive", ".section .rodata");
            s += x86("directive", (fl + ": .string \"" + fn + "\"").c_str());
            s += x86("directive", ".section .text");
            s += x86("directive", ".intel_syntax noprefix");
            s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
        }
        if (nargs > 0) s += x86_reg_disp32_lea64("rsi", "rsp", 0);
        else           s += x86("xor", "esi", "esi");
        s += x86("mov32", "edx", (long)nargs);
        s += x86("mov32", "ecx", bid_bake_of(fn));
        s += x86("call", "rt_call_arr_bl", (uint64_t)(uintptr_t)(void *)rt_call_arr_bl);
        }
        if (nargs > 0) s += x86("add", "rsp", (long)(nargs * 16));
        { int _wpop_save = _.op_wpop; int _zgpop_save = _.op_zgpop; if (_.op_sb) { _.op_wpop = 0; _.op_zgpop = 0; }
        s += x86("cmp", "al", (long)DT_FAIL);
        s += x86_omega("je");
        s += x86("note", ZRESN()) + x86("mov", ZRES(0), "rax");
        s += x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx");
        if (_.op_off >= 0 && g_emit_cfg && g_emit_cfg->pl_cells_graph) { s += x86("mov", "r12", ZRES(0)); s += x86("mov", FRQ(_.op_off), "r12"); s += x86("mov", "rdi", ZRES(8)); s += x86("mov", FRQ(_.op_off + 8), "rdi"); }
        if (_.op_sb) { s += x86_omega(); _.op_wpop = _wpop_save; _.op_zgpop = _zgpop_save; return s; } _.op_wpop = _wpop_save; _.op_zgpop = _zgpop_save; }
        s += x86_gamma();
        s += (g_emit_cfg && g_emit_cfg->pl_cells_graph) ? (x86_beta() + x86_jmp(X86P_OMEGA)) : x86_beta_trampoline();
        return s;
    }
    int resoff = bcfn_result_slot(pBB);
    if (resoff < 0) return x86_alpha() + x86_bomb("bb_call_fn: no LOWER slot grant (TMP-ERADICATE)");
    if (_.node && nargs > _.node->n_operands) return x86_alpha() + x86_bomb("bb_call_fn: arg count exceeds LOWER grant (TMP-ERADICATE)");
    int argbase = resoff + 16;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    std::string s = x86_alpha()
                  + x86("comment", std::string("BOX IR_CALL ") + fn + "(...) -> rt_call_arr [operand-marshal, FAIL->ω]");
    const char * dsym = 0; void * dfp = dop_direct_fp(fn, (int64_t)nargs, &dsym);
    int cui = -1; long long cival = 0; const char * csval = 0;
    if (dfp && nargs == 2 && !strcmp(fn, "$unify") && !getenv("SCRIP_NO_CU")) {
        for (int i = 0; i < 2 && cui < 0; i++) {
            IR_t * lf = (subs && subs[i]) ? subs[i]->entry : ir_call_arg(pBB, i);
            if (!lf) continue;
            if (lf->op == IR_LIT_INTEGER) { cui = i; cival = (long long)IR_LIT(lf).ival; }
            else if (lf->op == IR_LIT_STRING && IR_LIT(lf).sval) { cui = i; csval = IR_LIT(lf).sval; }
        }
    }
    int ix_kk = 0; long long ix_kival = 0;
    if (dfp && nargs == 3 && !strcmp(fn, "$ix_g")) {
        IR_t * a1 = (subs && subs[1]) ? subs[1]->entry : ir_call_arg(pBB, 1);
        IR_t * a2 = (subs && subs[2]) ? subs[2]->entry : ir_call_arg(pBB, 2);
        if (a1 && a1->op == IR_LIT_INTEGER) {
            int kk = (int)(((long long)IR_LIT(a1).ival) & 0xFF);
            if (kk == 3) ix_kk = 3;
            else if (kk == 2) ix_kk = 2;
            else if (kk == 1 && a2 && a2->op == IR_LIT_INTEGER) { ix_kk = 1; ix_kival = (long long)IR_LIT(a2).ival; }
        }
    }
    if (cui >= 0) {
        int vi = 1 - cui;
        s += marshal_call_arg((subs && subs[vi]) ? subs[vi]->entry : ir_call_arg(pBB, vi), (subs && subs[vi]) ? subs[vi] : NULL, argbase, _.node, vi);
    } else {
        for (int i = nargs - 1; i >= 0; i--)
            s += marshal_call_arg((subs && subs[i]) ? subs[i]->entry : ir_call_arg(pBB, i), (subs && subs[i]) ? subs[i] : NULL, argbase + i * 16, _.node, i);
    }
    if (dfp && cui >= 0) {
        s += x86("comment", (std::string("PL-REGAIN-5 const head-unify leaf: ") + (csval ? "rt_pl_dop_unify_cs" : "rt_pl_dop_unify_ci") + " (const in reg, one-operand marshal)").c_str());
        s += x86("lea", "rdi", FRQ(argbase));
        if (csval) {
            s += x86("mov", "rsi", ROQ(cui * 2));
            s += x86_jmp_id(cui * 2 + 1);
            s += x86_ro_seal_str(cui * 2, csval);
            s += x86_deflabel_id(cui * 2 + 1);
            s += x86("call", "rt_pl_dop_unify_cs", (uint64_t)(uintptr_t)(void *)rt_pl_dop_unify_cs);
        } else {
            s += x86_movabs_r64("rsi", (uint64_t)cival);
            s += x86("call", "rt_pl_dop_unify_ci", (uint64_t)(uintptr_t)(void *)rt_pl_dop_unify_ci);
        }
    } else if (dfp && nargs == 2 && !strcmp(fn, "$unify") && !getenv("SCRIP_NO_SINK")) {
        s += sink_unify2_str(argbase, (uint64_t)(uintptr_t)dfp, dsym);
    } else if (dfp && nargs == 3 && !strcmp(fn, "$unify_lst") && !getenv("SCRIP_NO_SINK")) {
        s += sink_unify_lst_str(argbase, (uint64_t)(uintptr_t)dfp, dsym);
    } else if (dfp && nargs == 0 && !strcmp(fn, "$trail_mark") && !getenv("SCRIP_NO_SINK") && !getenv("SCRIP_NO_SINK8")) {
        s += sink_trail_mark_str(argbase, (uint64_t)(uintptr_t)dfp, dsym, -1, resoff);
    } else if (dfp && nargs == 3 && !strcmp(fn, "$ix_g") && ix_kk > 0 && !getenv("SCRIP_NO_SINK") && !getenv("SCRIP_NO_SINK4")) {
        s += sink_ix_g_str(argbase, (uint64_t)(uintptr_t)dfp, dsym, ix_kk, ix_kival);
    } else if (dfp) {
        s += x86("comment", (std::string("PL-REGAIN-2 direct det leaf: ") + dsym + " (no by-name dispatch)").c_str());
        s += x86("lea", "rdi", FRQ(argbase));
        s += x86("mov32", "esi", (long)nargs);
        s += x86("call", dsym, (uint64_t)(uintptr_t)dfp);
    } else {
        std::string fl = std::string(".L") + x86_boxkind() + "_rkfn" + std::to_string(g_flat_node_id++);
        s += x86("directive", ".section .rodata");
        s += x86("directive", (fl + ": .string \"" + fn + "\"").c_str());
        s += x86("directive", ".section .text");
        s += x86("directive", ".intel_syntax noprefix");
        s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
        s += x86("lea", "rsi", FRQ(argbase));
        s += x86("mov32", "edx", (long)nargs);
        s += x86("rtcc_wb");
        s += x86("mov32", "ecx", bid_bake_of(fn));
        s += x86("call_bare", "rt_call_arr_bl", (uint64_t)(uintptr_t)(void *)rt_call_arr_bl);
        s += x86("rtcc_rl");
    }
    s += x86("mov", FRQ(resoff), "rax");
    s += x86("mov", FRQ(resoff + 8), "rdx");
    s += x86("cmp", "al", (long)DT_FAIL);
    s += x86_omega("je");
    s += x86_gamma();
    s += x86_beta();
    s += x86_omega();
    return s;
}
