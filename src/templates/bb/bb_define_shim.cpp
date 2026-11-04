#include <algorithm>
#include "runtime/core/g_runtime.h"
#include <string>
#include <cstdint>
#include <cstdio>
#include <vector>
#include "emitter/emit_knobs.h"
#include "emitter/emit_str.h"
extern "C" {
#include "emitter/emit_state.h"
#include "bb_templates.h"
#include "ir/pin_va.h"
#include "runtime/rt/rt.h"
#include "runtime/core/trace.h"
#include "runtime/rtx/rtcc.h"
extern int rt_g_want_name;
extern int rt_g_ret_by_name;
void rt_kw_set_rtntype_role(int);
void rt_define_bind_entry(const char *fname, const char *entry);
extern int * const rt_k_level_p;
void sno_trace_call(const char *fname);
void sno_trace_return(const char *fname, DESCR_t retval);
void rt_trace_call_hook_p(const char *fname, trace_pend_t *pend);
void rt_trace_return_hook_p(const char *fname, DESCR_t retval, trace_pend_t *pend, long long rstno);
void rt_trace_fail_hook_p(const char *fname, trace_pend_t *pend, long long rstno);
void rt_define_site(const char *, const char *, int, int, int, void *);
void rt_define_site_entry(const char *, const char *);
void bb_ab_seal_alpha(const char * pname, void * alpha);
void rt_shim_nv_in(const char *, DESCR_t *, const long *, const char *); DESCR_t rt_shim_nv_gamma(const char *, DESCR_t *); void rt_shim_nv_omega(const char *, DESCR_t *);
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_call.h"
#include "templates/x86/x86_frame.h"
#include "templates/x86/x86_gc.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_rsp_tracker.h"
#include "templates/x86/x86_rodata.h"
extern "C" {
#include "optimizer/gva_collect.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define fnsig() emit_knob_unless_zero("SCRIP_FN_SIG")
#define wn_park() emit_knob_unless_zero("SCRIP_WN_PARK")
template<typename F> inline std::string emit_shim9(long loff, long carve, F f) {
    return emit_shim9_after((emit_shim9_arm(loff, carve), f()));
}
#define S9(f) emit_shim9(32 + 16L * _.op_sig_xt, _.op_sig_F, (f))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define bb_fnclevel_enter() ( \
      x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p") \
    + x86("mov", "rax", RDQ("rax", 0)) \
    + x86("add", RDD("rax", 0), (long)1) \
    + x86("mov", "r11d", RDD("rax", 0)) \
    + x86("movsxd", "r11", "r11d") \
    + x86("sub", "r11", (long)1) \
    + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime") \
    + x86("mov", RDQ("rax", GRT_AT(kw_fnclevel)), "r11") \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define bb_stno_save(off) ( \
      x86("comment", "&STNO SAVE (R-1543): the CALLER's &STNO/&LINE go into this activation's own frame unit, so RETURN can put them back") \
    + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime") \
    + x86("mov", "rax", RDQ("rax", GRT_AT(stno))) \
    + x86_rsp_store64((off), "rax") \
    + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime") \
    + x86("mov", "rax", RDQ("rax", GRT_AT(line))) \
    + x86_rsp_store64((off) + 8, "rax") \
)
#define bb_stno_last_from_callee() ( \
      x86("comment", "&LASTNO/&LASTLINE ON RETURN: the returning function's statement and line become the previous ones, as SPITBOL answers them in the calling statement") \
    + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime") \
    + x86("mov", "r11", RDQ("rax", GRT_AT(stno))) \
    + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime") \
    + x86("mov", RDQ("rax", GRT_AT(lastno)), "r11") \
    + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime") \
    + x86("mov", "r11", RDQ("rax", GRT_AT(line))) \
    + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime") \
    + x86("mov", RDQ("rax", GRT_AT(lastline)), "r11") \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define bb_stno_restore(off) ( \
      x86("comment", "&STNO RESTORE (R-1543): the caller's &STNO/&LINE come back from this activation's own frame unit") \
    + bb_stno_last_from_callee() \
    + x86_rsp_load64("r11", (off)) \
    + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime") \
    + x86("mov", RDQ("rax", GRT_AT(stno)), "r11") \
    + x86_rsp_load64("r11", (off) + 8) \
    + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime") \
    + x86("mov", RDQ("rax", GRT_AT(line)), "r11") \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define bb_fnclevel_leave() ( \
      x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p") \
    + x86("mov", "rax", RDQ("rax", 0)) \
    + x86("mov", "r11d", RDD("rax", 0)) \
    + x86("movsxd", "r11", "r11d") \
    + x86("sub", "r11", (long)1) \
    + x86("mov", RDD("rax", 0), "r11d") \
    + x86("sub", "r11", (long)1) \
    + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime") \
    + x86("mov", RDQ("rax", GRT_AT(kw_fnclevel)), "r11") \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string shim_gq(int gk, int w) { return (g_runtime.abi.rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, w) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + (unsigned long)w); }
static std::string shim_r8q(long d) { return std::string("[r8 + ") + std::to_string(d) + "]"; }
static std::string shim_sigq(long d) { return std::string("[rcx + ") + std::to_string(d) + "]"; }
static std::string shim_extq(long d) { return std::string("[rsp + ") + std::to_string(_.op_sig_T + d) + "]"; }
static std::string shim_r8at() { return x86("lea", "r8", std::string("[rsp + ") + std::to_string(_.op_sig_F) + "]"); }
static int shim_rk(int j) { return _.op_sig_xt - 1 - j; }
static int shim_dupl(int k) { return _.op_sig_gk[_.op_sig_nf + k] >= 0 && std::count(_.op_sig_gk, _.op_sig_gk + _.op_sig_nf, _.op_sig_gk[_.op_sig_nf + k]) > 0; }
template<typename A, typename E> static std::string shim_chain(int base, const char * reg, A arg, E ext) {
    return IF(_.op_sig_nf > 0, FOR(0, _.op_sig_nf, [&](int i) { return x86("cmp", reg, (long)i) + x86_jcc_id("jbe", base + i) + arg(i); })
                             + x86_jmp_id(base + _.op_sig_nf)
                             + FOR(0, _.op_sig_nf, [&](int i) { return x86_deflabel_id(base + i) + ext(i); })
                             + x86_deflabel_id(base + _.op_sig_nf));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string shim_wnsave() {
    return x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_want_name, "rt_g_want_name")
         + x86("mov", "edx", RDD("rax", 0))
         + x86("movsxd", "rdx", "edx")
         + x86_rsp_store64((int)(16L * _.op_sig_xt + 24), "rdx")
         + x86("mov", RDD("rax", 0), (long)0)
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_ret_by_name, "rt_g_ret_by_name")
         + x86("mov", RDD("rax", 0), (long)0);
}
static std::string shim_wnrestore() {
    return IF(!wn_park(), x86("comment", "SCRIP_WN_PARK=0: the caller's want-name is left as the body left it (the knob's documented meaning; the park gate's fail-once arm plants it)"))
         + IF(wn_park(), x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_want_name, "rt_g_want_name")
                       + x86_rsp_load64("rdx", (int)(16L * _.op_sig_xt + 24))
                       + x86("mov", RDD("rax", 0), "edx"));
}
static std::string shim_scale16() {
    return x86("mov", "rax", "rcx")
         + x86("add", "rax", "rax")
         + x86("add", "rax", "rax")
         + x86("add", "rax", "rax")
         + x86("add", "rax", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string shim_restore4(int lid) {
    return x86_rsp_load64("rcx", (int)(16 * _.op_sig_xt + 16))
         + shim_scale16()
         + x86("cmp", "rcx", (long)_.op_sig_nf)
         + x86_jcc_id("jbe", lid - 5)
         + x86("mov32", "eax", 16L * _.op_sig_nf)
         + x86_deflabel_id(lid - 5)
         + x86("lea", "r8", std::string("[rsp + ") + std::to_string(_.op_sig_T) + "]")
         + x86("sub", "r8", "rax")
         + FOR(0, _.op_sig_xt, [&](int j) {
               return x86_rsp_load64("rax", 16 * shim_rk(j))
                    + x86("note", gva_note(_.op_sig_gk[_.op_sig_nf + shim_rk(j)]))
                    + x86("mov", shim_gq(_.op_sig_gk[_.op_sig_nf + shim_rk(j)], 0), "rax")
                    + x86_rsp_load64("rax", 16 * shim_rk(j) + 8)
                    + x86("mov", shim_gq(_.op_sig_gk[_.op_sig_nf + shim_rk(j)], 8), "rax"); })
         + shim_chain(lid, "rcx",
               [&](int i) {
                   return x86("mov", "rax", shim_r8q(16L * _.op_sig_nf + 32 + 16L * i))
                        + x86("note", gva_note(_.op_sig_gk[i]))
                        + x86("mov", shim_gq(_.op_sig_gk[i], 0), "rax")
                        + x86("mov", "rax", shim_r8q(16L * _.op_sig_nf + 32 + 16L * i + 8))
                        + x86("mov", shim_gq(_.op_sig_gk[i], 8), "rax"); },
               [&](int i) {
                   return x86("mov", "rax", shim_r8q(16L * i))
                        + x86("note", gva_note(_.op_sig_gk[i]))
                        + x86("mov", shim_gq(_.op_sig_gk[i], 0), "rax")
                        + x86("mov", "rax", shim_r8q(16L * i + 8))
                        + x86("mov", shim_gq(_.op_sig_gk[i], 8), "rax"); });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string shim_nvpush() {
    return x86("push", "rdi")
         + x86("push", "rsi")
         + x86("push", "rdx")
         + x86("push", "rcx")
         + x86("push", "r8")
         + x86("push", "r9")
         + x86("push", "r12")
         + x86("push", "rdi");
}
static std::string shim_nvpop() {
    return x86("pop", "rdi")
         + x86("pop", "r12")
         + x86("pop", "r9")
         + x86("pop", "r8")
         + x86("pop", "rcx")
         + x86("pop", "rdx")
         + x86("pop", "rsi")
         + x86("pop", "rdi");
}
static std::string shim_nvcells() {
    return x86("mov", "rsi", "rsp")
         + x86("add", "rsi", (long)((_.op_sig_T + 16L * _.op_sig_nf) + 64));
}
static std::string shim_nvin() {
    return IF(_.op_sig_nnv, x86("comment", "NV-ROAD NAMES (R-1543 chunk 2): a formal, local or result name with no island cell is swapped by a leaf into this frame's own cells, by name")
                          + shim_nvpush()
                          + x86("mov", "rdi", ROQ(232))
                          + shim_nvcells()
                          + x86_rsp_load64("rdx", (int)(16 * _.op_sig_xt + 16 + 64))
                          + x86("mov", "rcx", "rsp")
                          + x86("add", "rcx", (long)(_.op_sig_F + 64))
                          + S9([&]() { return x86("call", "rt_shim_nv_in", (uint64_t)(uintptr_t)(void *)rt_shim_nv_in) + x86_rt_gc_poll(); })
                          + shim_nvpop());
}
static std::string shim_nvgamma() {
    return IF(_.op_sig_nnv, shim_nvpush()
                          + x86("mov", "rdi", ROQ(237))
                          + shim_nvcells()
                          + S9([&]() { return x86("call", "rt_shim_nv_gamma", (uint64_t)(uintptr_t)(void *)rt_shim_nv_gamma) + x86_rt_gc_poll(); })
                          + IF(_.op_sig_rg < 0, x86_rsp_store64(56, "rax") + x86_rsp_store64(48, "rdx"))
                          + shim_nvpop());
}
static std::string shim_nvomega() {
    return IF(_.op_sig_nnv, shim_nvpush()
                          + x86("mov", "rdi", ROQ(237))
                          + shim_nvcells()
                          + S9([&]() { return x86("call", "rt_shim_nv_omega", (uint64_t)(uintptr_t)(void *)rt_shim_nv_omega) + x86_rt_gc_poll(); })
                          + shim_nvpop());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string shim_frestore(int lid) {
    return x86_rsp_load64("rcx", (int)(16 * _.op_sig_xt + 16))
         + x86("mov", "rdx", shim_sigq(0))
         + shim_r8at()
         + FOR(0, _.op_sig_xt, [&](int j) {
               return IF(_.op_sig_gk[_.op_sig_nf + shim_rk(j)] >= 0, x86_rsp_load64("rax", 16 * shim_rk(j))
                    + x86("note", gva_note(_.op_sig_gk[_.op_sig_nf + shim_rk(j)]))
                    + x86("mov", shim_gq(_.op_sig_gk[_.op_sig_nf + shim_rk(j)], 0), "rax")
                    + x86_rsp_load64("rax", 16 * shim_rk(j) + 8)
                    + x86("mov", shim_gq(_.op_sig_gk[_.op_sig_nf + shim_rk(j)], 8), "rax")); })
         + shim_chain(lid, "rdx",
               [&](int i) {
                   return IF(_.op_sig_gk[i] >= 0, x86("mov", "rax", shim_sigq(24 + 8L * i))
                        + x86("add", "rax", "r8")
                        + x86("mov", "rax", "[rax + 0]")
                        + x86("note", gva_note(_.op_sig_gk[i]))
                        + x86("mov", shim_gq(_.op_sig_gk[i], 0), "rax")
                        + x86("mov", "rax", shim_sigq(24 + 8L * i))
                        + x86("add", "rax", "r8")
                        + x86("mov", "rax", "[rax + 8]")
                        + x86("mov", shim_gq(_.op_sig_gk[i], 8), "rax")); },
               [&](int i) {
                   return IF(_.op_sig_gk[i] >= 0, x86("mov", "rax", shim_extq(16L * i).c_str())
                        + x86("note", gva_note(_.op_sig_gk[i]))
                        + x86("mov", shim_gq(_.op_sig_gk[i], 0), "rax")
                        + x86("mov", "rax", shim_extq(16L * i + 8).c_str())
                        + x86("mov", shim_gq(_.op_sig_gk[i], 8), "rax")); });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string shim_sig(int inl5) {
    return x86("comment", "IR_DEFINE role 4: SIG s66 per-DEFINE shim (alpha=swap-by-map, gamma/omega=restore-by-map, CONSTANT frame)")
         + IF(inl5, x86_jmp_id(245))
         + x86("commentrule", std::string(119, '-'))
         + IF(!inl5, x86_alpha())
         + x86_def_ext(_.op_sig_lbl_a)
         + x86("sub", "rsp", _.op_sig_F)
         + x86_rsp_mark_save()
         + shim_wnsave()
         + FOR(0, _.op_sig_xt, [&](int k) {
               return IF(!(_.op_sig_gk[_.op_sig_nf + k] < 0 || shim_dupl(k)), x86("note", gva_note(_.op_sig_gk[_.op_sig_nf + k]))
                    + x86("mov", "rax", shim_gq(_.op_sig_gk[_.op_sig_nf + k], 0))
                    + x86_rsp_store64(16 * k, "rax")
                    + x86("mov", "rax", shim_gq(_.op_sig_gk[_.op_sig_nf + k], 8))
                    + x86_rsp_store64(16 * k + 8, "rax")
                    + x86("mov", shim_gq(_.op_sig_gk[_.op_sig_nf + k], 0), (long)DT_SNUL)
                    + x86("mov", shim_gq(_.op_sig_gk[_.op_sig_nf + k], 8), (long)0)); })
         + x86_rsp_store64(16 * _.op_sig_xt + 16, "rcx")
         + x86("mov", "rdx", shim_sigq(0))
         + shim_r8at()
         + shim_chain(bb_shim_id_alpha(_.op_sig_nf), "rdx",
               [&](int i) {
                   return IF(_.op_sig_gk[i] >= 0, x86("mov", "rdi", shim_sigq(24 + 8L * i))
                        + x86("add", "rdi", "r8")
                        + x86("mov", "rax", "[rdi + 0]")
                        + x86("note", gva_note(_.op_sig_gk[i]))
                        + x86("mov", "rsi", shim_gq(_.op_sig_gk[i], 0))
                        + x86("mov", shim_gq(_.op_sig_gk[i], 0), "rax")
                        + x86("mov", "[rdi + 0]", "rsi")
                        + x86("mov", "rax", "[rdi + 8]")
                        + x86("mov", "rsi", shim_gq(_.op_sig_gk[i], 8))
                        + x86("mov", shim_gq(_.op_sig_gk[i], 8), "rax")
                        + x86("mov", "[rdi + 8]", "rsi")); },
               [&](int i) {
                   return IF(_.op_sig_gk[i] >= 0, x86("note", gva_note(_.op_sig_gk[i]))
                        + x86("mov", "rax", shim_gq(_.op_sig_gk[i], 0))
                        + x86("mov", shim_extq(16L * i).c_str(), "rax")
                        + x86("mov", "rax", shim_gq(_.op_sig_gk[i], 8))
                        + x86("mov", shim_extq(16L * i + 8).c_str(), "rax")
                        + x86("mov", shim_gq(_.op_sig_gk[i], 0), (long)DT_SNUL)
                        + x86("mov", shim_gq(_.op_sig_gk[i], 8), (long)0)); })
         + FOR(0, _.op_sig_xt, [&](int k) {
               return IF(!(_.op_sig_gk[_.op_sig_nf + k] < 0 || !shim_dupl(k)), x86("note", gva_note(_.op_sig_gk[_.op_sig_nf + k]))
                    + x86("mov", "rax", shim_gq(_.op_sig_gk[_.op_sig_nf + k], 0))
                    + x86_rsp_store64(16 * k, "rax")
                    + x86("mov", "rax", shim_gq(_.op_sig_gk[_.op_sig_nf + k], 8))
                    + x86_rsp_store64(16 * k + 8, "rax")
                    + x86("mov", shim_gq(_.op_sig_gk[_.op_sig_nf + k], 0), (long)DT_SNUL)
                    + x86("mov", shim_gq(_.op_sig_gk[_.op_sig_nf + k], 8), (long)0)); })
         + bb_fnclevel_enter()
         + bb_stno_save((int)(16 * _.op_sig_xt))
         + shim_nvin()
         + IF(x86_trace_hooks_on(), x86_load_got("rax", "g_runtime", (uint64_t)(uintptr_t)(void *)&g_runtime)
             + x86("mov", "rax", RDQ("rax", GRT_AT(trace)))
             + x86("cmp", "rax", (long)0)
             + x86_jcc_id("jg", 246)
             + x86_load_got("rax", "g_runtime", (uint64_t)(uintptr_t)(void *)&g_runtime)
             + x86("mov", "rax", RDQ("rax", GRT_AT(kw_ftrace)))
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
             + x86("sub", "rsp", 112L)
             + x86("mov", RDQ("rsp", 0), 0L)
             + x86("mov", "rdi", ROQ(232))
             + x86("mov", "rsi", "rsp")
             + S9([&]() { return x86("call", "rt_trace_call_hook_p", (uint64_t)(uintptr_t)(void *)rt_trace_call_hook_p) + x86_rt_gc_poll(); })
             + S9([&]() { return bb_glue_trace_pend_run(bb_shim_id_omega(_.op_sig_nf) + _.op_sig_nf + 2, std::string(), 1); })
             + x86("add", "rsp", 112L)
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
         + IF(x86_trace_hooks_on(), x86_load_got("rax", "g_runtime", (uint64_t)(uintptr_t)(void *)&g_runtime)
             + x86("mov", "rax", RDQ("rax", GRT_AT(trace_budget)))
             + x86("cmp", "rax", (long)0)
             + x86_jcc_id("je", 238)
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
             + S9([&]() { return x86("call", "sno_trace_call", (uint64_t)(uintptr_t)(void *)sno_trace_call) + x86_rt_gc_poll(); })
             + x86_align_leave()
             + x86("pop", "rdi")
             + x86("pop", "r12")
             + x86("pop", "r9")
             + x86("pop", "r8")
             + x86("pop", "rcx")
             + x86("pop", "rdx")
             + x86("pop", "rsi")
             + x86("pop", "rdi")
             + x86_deflabel_id(238))
         + x86_jmp_id(231)
         + x86(".quad_ro", L(232), LS(232), _.op_sval)
         + x86("label", LS(232))
         + x86(".string", _.op_sval)
         + x86_deflabel_id(231)
         + x86("lea", "rcx", "extlbl", (uint64_t)(uintptr_t)_.op_sig_lbl_b)
         + x86("lea", "rax", "extlbl", (uint64_t)(uintptr_t)_.op_sig_lbl_o)
         + x86("comment", "s64 RSP-ONLY WRITER (see the s58 arm's full comment — unchanged under SIG)")
         + x86("push", "rax")
         + x86("push", "rcx")
         + bb_define_entry_cell_data(_.op_sig_bcell, _.op_sig_blb)
         + x86("jmp_fn_cell", _.op_sig_bcell, _.op_sig_entry_cell)
         + x86_def_ext(_.op_sig_lbl_b)
         + x86_rsp_land(0L)
         + x86_gc_site_raw(X86_SITE_FN_EXIT, 7, (int)((32 + 16 * _.op_sig_xt) | (_.op_sig_F << 16)))
         + shim_nvgamma()
             + IF(_.op_sig_rg >= 0, x86("note", gva_note((_.op_sig_rg < 0 ? 0 : _.op_sig_rg)))
             + x86("mov", "rdi", shim_gq((_.op_sig_rg < 0 ? 0 : _.op_sig_rg), 0))
             + x86("mov", "rsi", shim_gq((_.op_sig_rg < 0 ? 0 : _.op_sig_rg), 8)))
         + x86("mov", "rax", "rdi")
         + x86("mov", "rdx", "rsi")
         + x86("push", "rdx")
         + x86("push", "rax")
         + IF(x86_trace_hooks_on(), x86_load_got("rax", "g_runtime", (uint64_t)(uintptr_t)(void *)&g_runtime)
             + x86("mov", "rax", RDQ("rax", GRT_AT(trace)))
             + x86("cmp", "rax", (long)0)
             + x86_jcc_id("jne", 247)
             + x86_load_got("rax", "g_runtime", (uint64_t)(uintptr_t)(void *)&g_runtime)
             + x86("mov", "rax", RDQ("rax", GRT_AT(kw_ftrace)))
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
             + x86_rsp_load64("r8", 64 + 16 * _.op_sig_xt + 8)
             + x86("sub", "rsp", 112L)
             + x86("mov", RDQ("rsp", 0), 0L)
             + x86("mov", "rcx", "rsp")
             + S9([&]() { return x86("call", "rt_trace_return_hook_p", (uint64_t)(uintptr_t)(void *)rt_trace_return_hook_p) + x86_rt_gc_poll(); })
             + S9([&]() { return bb_glue_trace_pend_run(bb_shim_id_omega(_.op_sig_nf) + _.op_sig_nf + 2 + BB_SHIM_TRACE_IDS, std::string(), 1); })
             + x86("add", "rsp", 112L)
             + x86_align_leave()
             + x86("pop", "r12")
             + x86("pop", "r9")
             + x86("pop", "r8")
             + x86("pop", "rcx")
             + x86("pop", "rdi")
             + x86("pop", "rsi")
             + IF(_.op_sig_rg >= 0, x86("note", gva_note((_.op_sig_rg < 0 ? 0 : _.op_sig_rg)))
             + x86("mov", "rdi", shim_gq((_.op_sig_rg < 0 ? 0 : _.op_sig_rg), 0))
             + x86("mov", "rsi", shim_gq((_.op_sig_rg < 0 ? 0 : _.op_sig_rg), 8)))
             + x86_deflabel_id(235))
         + IF(x86_trace_hooks_on(), x86_load_got("rax", "g_runtime", (uint64_t)(uintptr_t)(void *)&g_runtime)
             + x86("mov", "rax", RDQ("rax", GRT_AT(trace_budget)))
             + x86("cmp", "rax", (long)0)
             + x86_jcc_id("je", 239)
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
             + S9([&]() { return x86("call", "sno_trace_return", (uint64_t)(uintptr_t)(void *)sno_trace_return) + x86_rt_gc_poll(); })
             + x86_align_leave()
             + x86("pop", "r12")
             + x86("pop", "r9")
             + x86("pop", "r8")
             + x86("pop", "rcx")
             + x86("pop", "rdi")
             + x86("pop", "rsi")
             + IF(_.op_sig_rg >= 0, x86("note", gva_note((_.op_sig_rg < 0 ? 0 : _.op_sig_rg)))
             + x86("mov", "rdi", shim_gq((_.op_sig_rg < 0 ? 0 : _.op_sig_rg), 0))
             + x86("mov", "rsi", shim_gq((_.op_sig_rg < 0 ? 0 : _.op_sig_rg), 8)))
             + x86_deflabel_id(239))
         + x86_jmp_id(236)
         + x86(".quad_ro", L(237), LS(237), _.op_sval)
         + x86("label", LS(237))
         + x86(".string", _.op_sval)
         + x86_deflabel_id(236)
         + x86("pop", "rax")
         + x86("pop", "rdx")
         + shim_frestore(bb_shim_id_beta(_.op_sig_nf))
         + bb_stno_restore((int)(16 * _.op_sig_xt))
         + bb_fnclevel_leave()
         + shim_wnrestore()
         + x86("mov", "rcx", shim_sigq(8))
         + x86("add", "rsp", _.op_sig_F)
         + x86("comment", "re-stage the return value: FRESTORE above uses rax/rdx as scratch, so the pair staged for the tap is gone by here "
                          "(row 521; the d067ceae4 revert was exactly this clobber)")
         + x86("mov", "rax", "rdi")
         + x86("mov", "rdx", "rsi")
         + x86("jmp", "rcx")
         + x86_def_ext(_.op_sig_lbl_o)
         + x86_rsp_land(0L)
         + shim_nvomega()
         + IF(x86_trace_hooks_on(), x86_load_got("rax", "g_runtime", (uint64_t)(uintptr_t)(void *)&g_runtime)
             + x86("mov", "rax", RDQ("rax", GRT_AT(trace)))
             + x86("cmp", "rax", (long)0)
             + x86_jcc_id("jne", 248)
             + x86_load_got("rax", "g_runtime", (uint64_t)(uintptr_t)(void *)&g_runtime)
             + x86("mov", "rax", RDQ("rax", GRT_AT(kw_ftrace)))
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
             + x86("sub", "rsp", 112L)
             + x86("mov", RDQ("rsp", 0), 0L)
             + x86("mov", "rdi", ROQ(237))
             + x86("mov", "rsi", "rsp")
             + x86_rsp_load64("rdx", 112 + 64 + 16 * _.op_sig_xt + 8)
             + S9([&]() { return x86("call", "rt_trace_fail_hook_p", (uint64_t)(uintptr_t)(void *)rt_trace_fail_hook_p) + x86_rt_gc_poll(); })
             + S9([&]() { return bb_glue_trace_pend_run(bb_shim_id_omega(_.op_sig_nf) + _.op_sig_nf + 2 + 2 * BB_SHIM_TRACE_IDS, std::string(), 1); })
             + x86("add", "rsp", 112L)
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
         + IF(x86_trace_hooks_on(), x86_load_got("rax", "g_runtime", (uint64_t)(uintptr_t)(void *)&g_runtime)
             + x86("mov", "rax", RDQ("rax", GRT_AT(trace_budget)))
             + x86("cmp", "rax", (long)0)
             + x86_jcc_id("je", 240)
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
             + x86("mov32", "esi", (long)DT_FAIL)
             + x86("xor", "edx", "edx")
             + S9([&]() { return x86("call", "sno_trace_return", (uint64_t)(uintptr_t)(void *)sno_trace_return) + x86_rt_gc_poll(); })
             + x86_align_leave()
             + x86("pop", "rdi")
             + x86("pop", "r12")
             + x86("pop", "r9")
             + x86("pop", "r8")
             + x86("pop", "rcx")
             + x86("pop", "rdx")
             + x86("pop", "rsi")
             + x86("pop", "rdi")
             + x86_deflabel_id(240))
         + shim_frestore(bb_shim_id_omega(_.op_sig_nf))
         + bb_stno_restore((int)(16 * _.op_sig_xt))
         + bb_fnclevel_leave()
         + shim_wnrestore()
         + x86("mov", "rcx", shim_sigq(16))
         + x86("add", "rsp", _.op_sig_F)
         + x86("mov32", "eax", (long)DT_FAIL)
         + x86("xor", "edx", "edx")
         + x86("jmp", "rcx")
         + IF(inl5, x86_deflabel_id(245))
         + IF(!inl5, x86_gamma());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string shim_s58(int inl5) {
    return x86("comment", "IR_DEFINE role 4: TINY-REAL s58 per-DEFINE shim (alpha=swap/extend, beta/omega=restore)")
         + IF(inl5, x86_jmp_id(245))
         + x86("commentrule", std::string(119, '-'))
         + IF(!inl5, x86_alpha())
         + x86_def_ext(_.op_sig_lbl_a)
         + x86_rsp_load64("rcx", 0)
         + x86("mov", "r8", "rsp")
         + x86("sub", "r8", 16L * _.op_sig_nf)
         + shim_scale16()
         + x86("cmp", "rcx", (long)_.op_sig_nf)
         + x86_jcc_id("jbe", 2)
         + x86("mov32", "eax", 16L * _.op_sig_nf)
         + x86_deflabel_id(2)
         + x86("sub", "rsp", _.op_sig_T + 16L * _.op_sig_nf)
         + x86("add", "rsp", "rax")
         + shim_wnsave()
         + FOR(0, _.op_sig_xt, [&](int k) {
               return x86("note", gva_note(_.op_sig_gk[_.op_sig_nf + k]))
                    + x86("mov", "rax", shim_gq(_.op_sig_gk[_.op_sig_nf + k], 0))
                    + x86_rsp_store64(16 * k, "rax")
                    + x86("mov", "rax", shim_gq(_.op_sig_gk[_.op_sig_nf + k], 8))
                    + x86_rsp_store64(16 * k + 8, "rax")
                    + x86("mov", shim_gq(_.op_sig_gk[_.op_sig_nf + k], 0), (long)DT_SNUL)
                    + x86("mov", shim_gq(_.op_sig_gk[_.op_sig_nf + k], 8), (long)0); })
         + x86_rsp_store64(16 * _.op_sig_xt + 16, "rcx")
         + shim_chain(bb_shim_id_alpha(_.op_sig_nf), "rcx",
               [&](int i) {
                   return x86("mov", "rax", shim_r8q(16L * _.op_sig_nf + 32 + 16L * i))
                        + x86("note", gva_note(_.op_sig_gk[i]))
                        + x86("mov", "rdx", shim_gq(_.op_sig_gk[i], 0))
                        + x86("mov", shim_gq(_.op_sig_gk[i], 0), "rax")
                        + x86("mov", shim_r8q(16L * _.op_sig_nf + 32 + 16L * i).c_str(), "rdx")
                        + x86("mov", "rax", shim_r8q(16L * _.op_sig_nf + 32 + 16L * i + 8))
                        + x86("mov", "rdx", shim_gq(_.op_sig_gk[i], 8))
                        + x86("mov", shim_gq(_.op_sig_gk[i], 8), "rax")
                        + x86("mov", shim_r8q(16L * _.op_sig_nf + 32 + 16L * i + 8).c_str(), "rdx"); },
               [&](int i) {
                   return x86("note", gva_note(_.op_sig_gk[i]))
                        + x86("mov", "rax", shim_gq(_.op_sig_gk[i], 0))
                        + x86("mov", shim_r8q(16L * i).c_str(), "rax")
                        + x86("mov", "rax", shim_gq(_.op_sig_gk[i], 8))
                        + x86("mov", shim_r8q(16L * i + 8).c_str(), "rax")
                        + x86("mov", shim_gq(_.op_sig_gk[i], 0), (long)DT_SNUL)
                        + x86("mov", shim_gq(_.op_sig_gk[i], 8), (long)0); })
         + x86("lea", "rcx", "extlbl", (uint64_t)(uintptr_t)_.op_sig_lbl_b)
         + x86("lea", "rax", "extlbl", (uint64_t)(uintptr_t)_.op_sig_lbl_o)
         + x86("push", "rax")
         + x86("push", "rcx")
         + bb_define_entry_cell_data(_.op_sig_bcell, _.op_sig_blb)
         + x86("jmp_fn_cell", _.op_sig_bcell, _.op_sig_entry_cell)
         + x86_def_ext(_.op_sig_lbl_b)
         + x86_gc_site_raw(X86_SITE_FN_EXIT, 8, (int)(32 + 16 * _.op_sig_xt))
         + x86("note", gva_note((_.op_sig_rg < 0 ? 0 : _.op_sig_rg)))
         + x86("mov", "rdi", shim_gq((_.op_sig_rg < 0 ? 0 : _.op_sig_rg), 0))
         + x86("mov", "rsi", shim_gq((_.op_sig_rg < 0 ? 0 : _.op_sig_rg), 8))
         + shim_restore4(bb_shim_id_beta(_.op_sig_nf))
         + shim_wnrestore()
         + x86("mov32", "eax", _.op_sig_T + 32 + 16L * _.op_sig_nf)
         + x86("cmp", "rcx", (long)_.op_sig_nf)
         + x86_jcc_id("jbe", 3)
         + shim_scale16()
         + x86("add", "rax", _.op_sig_T + 32)
         + x86_deflabel_id(3)
         + x86("mov", "rcx", shim_r8q(16L * _.op_sig_nf + 16))
         + x86("add", "rsp", "rax")
         + x86("mov", "rax", "rdi")
         + x86("mov", "rdx", "rsi")
         + x86("jmp", "rcx")
         + x86_def_ext(_.op_sig_lbl_o)
         + shim_restore4(bb_shim_id_omega(_.op_sig_nf))
         + shim_wnrestore()
         + x86("mov32", "eax", _.op_sig_T + 32 + 16L * _.op_sig_nf)
         + x86("cmp", "rcx", (long)_.op_sig_nf)
         + x86_jcc_id("jbe", 4)
         + shim_scale16()
         + x86("add", "rax", _.op_sig_T + 32)
         + x86_deflabel_id(4)
         + x86("mov", "rcx", shim_r8q(16L * _.op_sig_nf + 24))
         + x86("add", "rsp", "rax")
         + x86("mov32", "eax", (long)DT_FAIL)
         + x86("xor", "edx", "edx")
         + x86("jmp", "rcx")
         + IF(inl5, x86_deflabel_id(245))
         + IF(!inl5, x86_gamma());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string shim_body(int inl5) {
    return !_.op_sig_ok ? (inl5 ? x86("comment", "role 5: shim refused inline (hatch or probe/formals shape) — sites fall to the slim arm")
                                : x86("comment", "IR_DEFINE role 4: shim refused (hatch, non-TEXT, or probe/formals shape) — sites fall to the slim arm")
                                  + x86_alpha()
                                  + x86_gamma())
         : fnsig() ? shim_sig(inl5)
         : _.op_sig_nnv ? x86("comment", "IR_DEFINE role 4: the legacy s58 shim has no NV road (SCRIP_FN_SIG=0) -- refused, sites fall to the slim arm") + IF(!inl5, x86_alpha() + x86_gamma())
         : shim_s58(inl5);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_define_shim() {
    x86_begin();
    return shim_body(0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_define_shim_inline() {
    x86_begin();
    return shim_body(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_define_rt_shim() {
    x86_begin();
    return shim_body(0);
}
