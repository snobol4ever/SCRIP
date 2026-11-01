#include "runtime/core/core.h"
#include <string>
#include "runtime/core/g_runtime.h"
#include <stdint.h>
#include "emitter/emit_str.h"
#include "emitter/emit_gc_site.h"
extern "C" {
#include "bb_templates.h"
#include "emitter/emit_state.h"
#include "runtime/rt/pl_trail_layout.h"
#include "runtime/rt/rt_pl_trail.h"
#include "bb_pl_cell.h"
#include "runtime/rt/gc_heap.h"
#include "templates/xa/xa_templates.h"
const char * bb_ab_fn_sym(const char * nm);
extern int * const rt_k_level_p;
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_call.h"
#include "templates/x86/x86_frame.h"
#include "templates/x86/x86_gc.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_rsp_tracker.h"
#include "templates/x86/x86_zeta.h"
#include "templates/x86/x86_zverify.h"
std::string xa_flat_pkt_tail_pop(const char * fb, int kt, int lsk);
#define PB_FB() (x86_hb_on() ? x86_hb() : "r11")
#define PB_N() ((int)_.op_ival)
#define PB_KT() (_.flat_frame_bytes)
#define PB_NB() (PB_KT() + 16 * (_.op_pb_ns - PB_N()))
#define RO_SEAL_STR(n, s) \
    (x86(".quad_ro", L(n), LS(n), (s)) \
   + x86("label", LS(n)) \
   + x86(".string", (s)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_lvl_add(long d) {
    return x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
         + x86("mov", "rax", RDQ("rax", 0))
         + x86("add", RDD("rax", 0), d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_jmp_proc_fn() {
    return x86("note",
        "the registry jump: the record's fn word (PROC_FN at +8, the 128-byte stride both asserted in rt.c), so a fn "
            "re-sealed at run time -- a dynamic predicate's enumerator, a redefinition -- is followed in both media")
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
         + x86("mov", "rax", RDQ("rax", GRT_AT(rt_gen_procs)))
         + x86("mov", "rax", RDQ("rax", (int)(8 + 128 * _.op_pb_idx)))
         + x86_jmp_reg("rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_jmp_callee() {
    return IF(_.op_pb_static, x86("note",
        "a static callee (ARCH-PROLOG-C-OUT-OF-THE-BOX 9.3): a direct jump to its entry label in the text, through its "
            "alpha cell in the slab (x86_jmp_via_cell, both media; the cell is sealed by rt_proc_seal_alpha when the "
                "record's fn is set and pinned, under no knob); the registry record is for a fn re-sealed at run time, "
                "which a static predicate never is (assertz on it raises permission_error)"))
         + IF(_.op_pb_static, x86("jmp", "[rip@cell + __]", (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("alpha$") + _.op_sval).c_str()), bb_ab_fn_sym(_.op_sval)))
         + IF(!_.op_pb_static, pb_jmp_proc_fn());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_block_zop(int i, int w) {
    return (i >= 0 && i < _.op_zcap && _.op_zread_xf[i] != -1) ? std::string(RDQ("rbp", _.op_zread_xf[i] + w))
                                                               : std::string(x86_zref(((i >= 0 && i < _.op_zcap) ? _.op_zread[i] : 0) + w + 16 * PB_N(), 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_block_build() {
    return x86("sub", "rsp", (long)(16 * PB_N()))
         + IF(_.op_zres, FOR(0, PB_N(), [&](int i) { return x86("note", ZOPN(i))
                                                     + x86("mov", "rax", pb_block_zop(i, 0))
                                                     + x86_rsp_store64(16 * i, "rax")
                                                     + x86("note", ZOPN(i))
                                                     + x86("mov", "rax", pb_block_zop(i, 8))
                                                     + x86_rsp_store64(16 * i + 8, "rax"); }))
         + IF(!_.op_zres, FOR(0, PB_N(), [&](int i) { return x86("note", std::string("block A") + std::to_string(i))
                                                      + x86("mov", "rax", FRQB(_.op_pb_slot[i], 16 * PB_N()))
                                                      + x86_rsp_store64(16 * i, "rax")
                                                      + x86("mov", "rax", FRQB(_.op_pb_slot[i] + 8, 16 * PB_N()))
                                                      + x86_rsp_store64(16 * i + 8, "rax"); }));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_arms(int lb, int lg) {
    return x86("def", L(lb + 3))
         + x86("mov", "r8", RDQ("rdi", 8))
         + x86("test", "r8", "r8")
         + x86("je", L(lb + 8))
         + x86("mov", "ecx", RDD("rdi", 4))
         + x86("cmp", "ecx", 2L)
         + x86("jne", L(lb + 6))
         + x86("mov", "r8", RDQ("r8", 0))
         + x86("test", "r8", "r8")
         + x86("je", L(lb + 8))
         + x86("jmp", L(lb + 6))
         + x86("def", L(lb + 5))
         + x86("mov", "r8", RDQ("rdi", 8))
         + x86("mov", "rax", "r8")
         + x86("sub", "rax", "rsp")
         + x86("lea", "rcx", RDQ(PB_FB(), PB_KT() + 16 * _.op_pb_ns))
         + x86("sub", "rcx", "rsp")
         + x86("cmp", "rax", "rcx")
         + x86("jae", L(lb + 9))
         + x86("cmp", "r8", "rdi")
         + x86("je", L(lb + 8))
         + x86("def", L(lb + 6))
         + x86("mov", "rdi", "r8")
         + x86("sub", "r9d", 1L)
         + x86("jne", L(lb + 2))
         + x86("jmp", L(199))
         + x86("def", L(lb + 8))
         + x86("mov", "rax", "rdi")
         + x86("sub", "rax", "rsp")
         + x86("lea", "rcx", RDQ(PB_FB(), PB_KT() + 16 * _.op_pb_ns))
         + x86("sub", "rcx", "rsp")
         + x86("cmp", "rax", "rcx")
         + x86("jb", L(lg))
         + x86("mov", "rax", (long)((1L << 32) | (long)DT_N))
         + x86("mov", RDQ("rsi", 0), "rax")
         + x86("mov", RDQ("rsi", 8), "rdi")
         + x86("jmp", L(lb + 7))
         + x86("def", L(lg))
         + x86("mov", "eax", RDD("rdi", 0))
         + x86("test", "eax", "eax")
         + x86("je", L(lg + 1))
         + x86("and", "eax", (long)(255 | PL_VAR_ATTV))
         + x86("cmp", "eax", (long)DT_PLVAR)
         + x86("jne", L(199))
         + x86("mov", "rax", RDQ("rdi", 8))
         + x86("cmp", "rax", "rdi")
         + x86("jne", L(199))
         + x86("def", L(lg + 1))
         + x86("test", "r13", "r13")
         + x86("jz", L(lg + 2))
         + x86("mov", "rax", RDQ("r13", PL_TR_FRAME_HI_OFF))
         + x86_raw_unpack("rax")
         + x86("cmp", "rdi", "rax")
         + x86("jb", L(lg + 2))
         + x86("mov", "rax", "r12")
         + x86("and", "rax", (long)(PL_TR_ARENA_BYTES - 1))
         + x86("cmp", "rax", (long)(PL_TR_ARENA_BYTES - PL_TR_ENTRY_BYTES))
         + x86("jae", L(lg + 3))
         + x86("mov", "rax", RDQ("rdi", 0))
         + x86("mov", "rcx", RDQ("rdi", 8))
         + x86("mov", RDQ("r12", 0), "rdi")
         + x86("mov", RDQ("r12", 8), 0L)
         + x86("mov", RDQ("r12", 16), "rax")
         + x86("mov", RDQ("r12", 24), "rcx")
         + x86("add", "r12", (long)PL_TR_ENTRY_BYTES)
         + x86_pl_tr_top_sync()
         + x86("def", L(lg + 2))
         + x86("note", "the globalized variable keeps the frame cell's serial (the same variable); a virgin frame cell (the zero DESCR) draws one, before the allocation, and rcx rides the call")
         + x86("mov", "ecx", RDD("rdi", 0))
         + x86("cmp", "cl", (long)DT_PLVAR)
         + x86("jne", L(lg + 600))
         + x86("mov", "rcx", RDQ("rdi", 0))
         + x86("jmp", L(lg + 601))
         + x86("def", L(lg + 600))
         + PL_SERIAL_DRAW("rcx")
         + x86("def", L(lg + 601))
         + x86("push", "rsi")
         + x86("push", "rdi")
         + x86("push", "r11")
         + x86("push", "rdx")
         + x86("push", "rcx")
         + x86("push", "rcx")
         + x86("mov32", "edi", (long)HB_DVEC)
         + x86("mov32", "esi", 16L)
         + x86("call", "rt_gcheap_alloc", (uint64_t)(uintptr_t)(void *)rt_gcheap_alloc)
         + x86("pop", "rcx")
         + x86("pop", "rcx")
         + x86("pop", "rdx")
         + x86("pop", "r11")
         + x86("pop", "rdi")
         + x86("pop", "rsi")
         + x86("mov", RDQ("rax", 0), "rcx")
         + x86("mov", RDQ("rax", 8), "rax")
         + x86("mov", RDQ("rdi", 0), (long)DT_PLVAR)
         + x86("mov", RDQ("rdi", 8), "rax")
         + x86_rt_gc_poll()
         + x86("jmp", L(lb + 9))
         + x86("def", L(lg + 3))
         + x86("mov", "rdi", "r12")
         + x86("call", "rt_pl_tr_refuse", (uint64_t)(uintptr_t)(void *)rt_pl_tr_refuse);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_body(int lb) {
    return x86("def", L(lb + 2))
         + x86("mov", "eax", RDD("rdi", 0))
         + x86("cmp", "al", (long)DT_N)
         + x86("je", L(lb + 3))
         + x86("test", "al", "al")
         + x86("je", L(lb + 8))
         + x86("cmp", "al", (long)DT_PLVAR)
         + x86("je", L(lb + 5))
         + x86("def", L(lb + 9))
         + x86("mov", "rax", RDQ("rdi", 0))
         + x86("mov", RDQ("rsi", 0), "rax")
         + x86("mov", "rax", RDQ("rdi", 8))
         + x86("mov", RDQ("rsi", 8), "rax")
         + x86("def", L(lb + 7));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_cell_entry(int lb) {
    return x86("mov", "eax", RDD("rsi", 0))
         + x86("cmp", "al", (long)DT_N)
         + x86("je", L(lb + 4))
         + x86("cmp", "al", (long)DT_PLVAR)
         + x86("jne", L(lb + 7))
         + x86("def", L(lb + 4))
         + x86("mov", "rdi", RDQ("rsi", 8))
         + x86("mov32", "r9d", 4L)
         + pb_body(lb);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_tail_pop_test() {
    return x86("note", "pop: this frame's own choice is the youngest only through a construct the lowering proved dead, so the call may also take the tail arm when r13 is a header "
                      "inside this frame (H, or a construct's own D1 header in its op slots) and no clause alternative is left; r13 returns to the caller's choice with the frame")
         + x86("je", L(198))
         + x86("lea", "rax", RDQ(PB_FB(), PB_KT() - 64))
         + x86("cmp", "r13", "rax")
         + x86("ja", L(199))
         + x86("cmp", "r13", PB_FB())
         + x86("jb", L(199))
         + x86("mov", "rax", RDQ(PB_FB(), PB_KT() - 56))
         + x86("test", "rax", "rax")
         + x86("jne", L(199))
         + x86("lea", "rax", RDQ(PB_FB(), PB_KT() - 64))
         + x86("cmp", "r13", "rax")
         + x86("je", L(198))
         + x86("mov", "rcx", RDQ("r13", 16))
         + x86("cmp", "rcx", "rax")
         + x86("je", L(198))
         + x86("cmp", "rcx", RDQ(PB_FB(), PB_KT() - 40))
         + x86("jne", L(199))
         + x86("def", L(198));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_tail_arm() {
    return x86("note", "last call (ARCH-PROLOG-C-OUT-OF-THE-BOX 1.4): the block is tested in place, then copied over this frame's own block and the frame released")
         + x86("lea", "r11", RDQ("rsp", 16 * PB_N() + (_.op_zres ? _.op_zabs : 0)))
         + x86("note",
        "tail-safe, per cell (ARCH-PROLOG-C-OUT-OF-THE-BOX 9.3): a variable argument is inspected from its own slot "
            "(one hop fewer than through its name cell), any other cell from the block; ONE check body, its cold arms out "
                "of line after the callee jump: a name (slen 1 the cell, slen 2 the VCELL's) is followed to its cell; a PLVAR "
                    "link INTO THIS FRAME -- rsp up to the old block's top, every cell the release kills, the spine cells below fb included -- "
                        "is followed too, a link elsewhere is a value (its pointer is "
                        "absolute and its target outlives the frame); at most four hops, a longer chain or a self link of this frame "
                            "DECLINES the tail arm; a bound cell's VALUE is copied in; an unbound cell elsewhere becomes a one-hop name; an unbound cell OF THIS FRAME "
                            "(the zero DESCR, a self PLVAR) is GLOBALIZED (ARCH-PROLOG-ACTIVATION-LIFETIME 1 (c), the WAM's put_unsafe_value): a heap cell is made, the frame "
                                "cell is bound to it under the trail test and the block carries the link; a null name or an attributed cell of this frame declines")
         + IF(PB_N() > 8, x86("mov", "rsi", "rsp"))
         + IF(PB_N() > 8, x86("lea", "rdx", RDQ("rsp", 16 * PB_N())))
         + IF(PB_N() > 8, x86("def", L(101)))
         + IF(PB_N() > 8, x86("cmp", "rsi", "rdx"))
         + IF(PB_N() > 8, x86("jae", L(110)))
         + IF(PB_N() > 8, pb_cell_entry(100))
         + IF(PB_N() > 8, x86("add", "rsi", 16L))
         + IF(PB_N() > 8, x86("jmp", L(101)))
         + IF(PB_N() > 8, x86("def", L(110)))
         + IF(PB_N() <= 8, FOR(0, PB_N(), [&](int i) { return x86("lea", "rsi", RDQ("rsp", 16 * i))
                                                        + IF(_.op_pb_vs[i] < 0, pb_cell_entry(110 + 10 * i))
                                                        + IF(_.op_pb_vs[i] >= 0, x86("lea", "rdi", FRQB(_.op_pb_vs[i], 16 * PB_N())))
                                                        + IF(_.op_pb_vs[i] >= 0, x86("mov32", "r9d", 4L))
                                                        + IF(_.op_pb_vs[i] >= 0, pb_body(110 + 10 * i)); }))
         + IF(_.flat_pkt, xa_flat_pkt_tail_pop(PB_FB(), PB_KT(), 100))
         + x86("mov", "rax", RDQ(PB_FB(), PB_KT() - 40))
         + x86("cmp", "r13", "rax")
         + IF(_.op_pb_pop, pb_tail_pop_test())
         + IF(!_.op_pb_pop, x86("jne", L(199)))
         + IF(_.op_pb_pop, x86("mov", "r13", RDQ(PB_FB(), PB_KT() - 40)))
         + IF(_.op_pb_pop, x86_pl_tr_tidy(700))
         + x86("mov", "rcx", RDQ(PB_FB(), PB_KT() - 24))
         + x86("mov", "rdx", RDQ(PB_FB(), PB_KT() - 16))
         + IF(x86_hb_on(), x86("mov", "r8", RDQ(PB_FB(), PB_KT() - 8)))
         + x86("note",
        "the new block's TOP is the old block's top (rbp+kt+16*nparams): a longer block grows DOWN over this frame's "
            "dead header, never up into the caller's spine; copied descending so a long block never overruns its own source")
         + FOR(0, PB_N(), [&](int j) { return x86("mov", "rax", RDQ("rsp", 16 * (PB_N() - 1 - j) + 8))
                                            + x86("mov", RDQ(PB_FB(), PB_NB() + 16 * (PB_N() - 1 - j) + 8), "rax")
                                            + x86("mov", "rax", RDQ("rsp", 16 * (PB_N() - 1 - j)))
                                            + x86("mov", RDQ(PB_FB(), PB_NB() + 16 * (PB_N() - 1 - j)), "rax"); })
         + x86("lea", "rsp", RDQ(PB_FB(), PB_NB()))
         + IF(x86_hb_on(), x86("mov", PB_FB(), "r8"))
         + pb_jmp_callee()
         + IF(PB_N() > 8, pb_arms(100, 300))
         + IF(PB_N() <= 8, FOR(0, PB_N(), [&](int i) { return pb_arms(110 + 10 * i, 300 + 4 * i); }))
         + x86("def", L(199));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_kill_copy(int i, int bump, int lb) {
    return x86("mov", "rax", FRQB(_.op_pb_kv[i], bump))
         + x86("mov", "rdx", FRQB(_.op_pb_kv[i] + 8, bump))
         + x86("test", "rax", "rax")
         + x86("je", L(lb + 1 + i))
         + x86("cmp", "al", (long)DT_PLVAR)
         + x86("jne", L(lb + 1 + 32 + i))
         + x86("lea", "rcx", FRQB(_.op_pb_kv[i], bump))
         + x86("cmp", "rdx", "rcx")
         + x86("je", L(lb + 1 + i))
         + x86("def", L(lb + 1 + 32 + i))
         + x86_rsp_store64(16 * i, "rax")
         + x86_rsp_store64(16 * i + 8, "rdx")
         + x86("def", L(lb + 1 + i));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_kill_zero_value(int vs, int nk, int bump, int lb) {
    return x86("mov", "rax", FRQB(vs, bump))
         + x86("test", "rax", "rax")
         + x86("je", L(lb + 65 + nk))
         + x86("cmp", "al", (long)DT_PLVAR)
         + x86("jne", L(lb + 97 + nk))
         + x86("lea", "rcx", FRQB(vs, bump))
         + x86("cmp", "rcx", FRQB(vs + 8, bump))
         + x86("je", L(lb + 65 + nk))
         + x86("def", L(lb + 97 + nk))
         + x86("mov", FRQB(vs, bump), 0L)
         + x86("mov", FRQB(vs + 8, bump), 0L)
         + x86("def", L(lb + 65 + nk));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_kill_zero_home(int vs, int bump) {
    return x86("mov", FRQB(vs, bump), 0L)
         + x86("mov", FRQB(vs + 8, bump), 0L);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_kill(int home, int bump, int lb) {
    return x86("note",
        home ? "a dead slot is not a root (ARCH-PROLOG-ACTIVATION-LIFETIME 6.4 R1): this call is a variable's first and last occurrence, so its home holds what the callee bound and "
                   "no road reads it again; under B == B0 the cell is written to the null DESCR the prologue zeroes to, no trail entry, no frame map change"
             : "a dead slot is not a root (ARCH-PROLOG-ACTIVATION-LIFETIME 6.4 R1): this call is a variable's last occurrence and not the last call; the block carries a NAME of the "
                   "cell, so under B == B0 a cell holding a value or a REF is copied into each block entry that names it and then written to the null DESCR the prologue zeroes to "
                   "(an unbound home, the zero DESCR or a self PLVAR, is left: the callee binds through the name); no trail entry, no frame map change")
         + IF(!x86_hb_on(), x86("lea", "r11", RDQ("rsp", bump + (_.op_zres ? _.op_zabs : 0))))
         + x86("mov", "rcx", RDQ(PB_FB(), PB_KT() - 40))
         + x86("cmp", "r13", "rcx")
         + x86("jne", L(lb))
         + IF(!home, FOR(0, PB_N(), [&](int i) { return IF(_.op_pb_kv[i] >= 0, pb_kill_copy(i, bump, lb)); }))
         + IF(!home, FOR(0, PB_N(), [&](int i) { return IF(_.op_pb_kvn[i] > 0, pb_kill_zero_value(_.op_pb_kv[i], _.op_pb_kvn[i], bump, lb)); }))
         + IF(home, FOR(0, PB_N(), [&](int i) { return IF(_.op_pb_khn[i] > 0, pb_kill_zero_home(_.op_pb_kh[i], bump)); }))
         + x86("def", L(lb));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_trail_act() {
    return x86("note",
                   "the act word names a retained frame: trail the slot when its frame is OLDER than the newest choice (the slot at or above [B+32]), so the unwind zeroes it when backtracking passes"
                   " this box")
         + x86("test", "rax", "rax")
         + x86("jz", L(86))
         + x86("test", "r13", "r13")
         + x86("jz", L(86))
         + x86("lea", "rcx", FRQ(_.op_pb_act))
         + x86("mov", "r8", RDQ("r13", PL_TR_FRAME_HI_OFF))
         + x86_raw_unpack("r8")
         + x86("cmp", "rcx", "r8")
         + x86("jb", L(86))
         + x86("mov", "r8", "r12")
         + x86("and", "r8", (long)(PL_TR_ARENA_BYTES - 1))
         + x86("cmp", "r8", (long)(PL_TR_ARENA_BYTES - PL_TR_ENTRY_BYTES))
         + x86("jae", L(87))
         + x86("mov", RDQ("r12", 0), "rcx")
         + x86("mov", RDQ("r12", 8), 0L)
         + x86("mov", RDQ("r12", 16), 0L)
         + x86("mov", RDQ("r12", 24), 0L)
         + x86("add", "r12", (long)PL_TR_ENTRY_BYTES)
         + x86("mov", "r8", "r12")
         + x86("and", "r8", X86_PL_TR_ARENA_MASK)
         + x86("mov", RDQ("r8", 0), "r12")
         + x86("jmp", L(86))
         + x86("def", L(87))
         + x86("mov", "rdi", "r12")
         + x86("call", "rt_pl_tr_refuse", (uint64_t)(uintptr_t)(void *)rt_pl_tr_refuse)
         + x86("def", L(86));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_rec_beta() {
    return x86_pl_ball_rule(30, 22)
         + x86("note", "beta finds rsp at this call's cell (every box of the continuation popped its own carve): a null record fails on; else pop the cell, "
                       "re-link rbp to the retained frame and resume it at its beta, rsp back where the callee left it")
         + x86("mov", "rax", x86_zref(24, 1))
         + x86("test", "rax", "rax")
         + x86("je", L(22))
         + x86("mov", "rcx", x86_zref(16, 1))
         + x86("shr", "rcx", 8L)
         + x86_add("rsp", 32)
         + x86_rsp_rec(32)
         + x86("mov", x86_hb(), "rax")
         + pb_lvl_add(1L)
         + x86_jmp_reg("rcx")
         + x86("def", L(22))
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_block_beta() {
    return x86_pl_ball_rule(30, 22)
         + x86("mov", "rax", FRQ(_.op_pb_act))
         + x86("test", "rax", "rax")
         + x86("je", L(22))
         + x86("mov", "rcx", FRQ(_.op_pb_act + 8))
         + x86("mov", FRQ(_.op_pb_act), 0L)
         + IF(_.op_pb_frs, x86_add("rsp", 16))
         + IF(_.op_pb_frs, x86_rsp_rec(16))
         + x86("mov", x86_hb(), "rax")
         + pb_lvl_add(1L)
         + x86_jmp_reg("rcx")
         + x86("def", L(22))
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pb_block_arm() {
    return x86_alpha()
         + x86("note", "the block protocol (ARCH-PROLOG-C-OUT-OF-THE-BOX 1.2): the argument block on this spine, the wires, a jump; no staged medium, no registry call, no C prologue")
         + IF(_.op_pb_fr, x86("mov", FRQ(_.op_pb_act), 0L))
         + IF(_.op_sprec, x86("note", "the null resume record (ARCH-PROLOG-ACTIVATION-LIFETIME 8.1): this call's cell holds the result and, at +16, the record its beta reads; "
                                 "a callee that leaves no choice releases to this cell, so beta finds the null record and fails on"))
         + IF(_.op_sprec, x86("mov", x86_zref(16, 1), (long)DT_RAW))
         + IF(_.op_sprec, x86("mov", x86_zref(24, 1), 0L))
         + pb_block_build()
         + IF(_.op_pb_kvt > 0, pb_kill(0, 16 * PB_N(), 400))
         + IF(_.op_pb_lco, pb_tail_arm())
         + x86_rsp_mark_save()
         + x86_lea_id("rcx", 3)
         + x86_lea_id("rdx", 4)
         + pb_jmp_callee()
         + x86("def", L(3))
         + xa_call_tail_land(-16L * PB_N())
         + IF(_.op_pb_kht > 0, pb_kill(1, 0, 530))
         + IF(_.op_pb_fr, x86("mov", FRQ(_.op_pb_act), "rax"))
         + IF(_.op_pb_fr, x86("mov", FRQ(_.op_pb_act + 8), "rdx"))
         + IF(_.op_sprec, x86("note", "a retained callee (rax = its frame, rdx = its beta) left rsp at the bottom of its live region: push this call's cell there, the "
                                 "resume record at +16 {DT_RAW | beta << 8, frame}; nothing reads the cell carved at alpha until the callee releases back to it"))
         + IF(_.op_sprec, x86("test", "rax", "rax"))
         + IF(_.op_sprec, x86("jz", L(86)))
         + IF(_.op_sprec, x86_sub("rsp", 32))
         + IF(_.op_sprec, x86_rsp_rec(-32))
         + IF(_.op_sprec, x86("shl", "rdx", 8L))
         + IF(_.op_sprec, x86("or", "rdx", (long)DT_RAW))
         + IF(_.op_sprec, x86("mov", x86_zref(16, 1), "rdx"))
         + IF(_.op_sprec, x86("mov", x86_zref(24, 1), "rax"))
         + IF(_.op_sprec, x86("def", L(86)))
         + IF(x86_resumable_frame() && _.op_pb_fr, pb_trail_act())
         + IF(_.op_pb_frs, x86("note", "a retained callee left rsp at the bottom of its live region: carve this call's result cell there, so the store lands in the call's own cell "
                                "and never in the callee's bottom cell (ARCH-PROLOG-ACTIVATION-LIFETIME 9.14 (b)); beta pops it before resuming the callee"))
         + IF(_.op_pb_frs, x86("test", "rax", "rax"))
         + IF(_.op_pb_frs, x86("jz", L(88)))
         + IF(_.op_pb_frs, x86_sub("rsp", 16))
         + IF(_.op_pb_frs, x86_rsp_rec(-16))
         + IF(_.op_pb_frs, x86("def", L(88)))
         + pb_lvl_add(-1L)
         + x86("mov", "rax", "rdi")
         + x86("mov", "rdx", "rsi")
         + x86("jmp", L(2))
         + x86("def", L(4))
         + xa_call_tail_land(-16L * PB_N())
         + IF(_.op_pb_fr, x86("mov", FRQ(_.op_pb_act), 0L))
         + pb_lvl_add(-1L)
         + xa_call_tail_fail()
         + x86("def", L(2))
         + IF(_.op_pb_fr, xa_call_tail_result_frame(_.op_pb_off))
         + xa_call_tail_fail_test()
         + IF(_.op_pb_zsp || _.op_sprec, xa_call_tail_result_zres())
         + x86_gamma()
         + x86_beta()
         + IF(_.op_pb_zsp, x86_omega())
         + IF(_.op_sprec, pb_rec_beta())
         + IF(_.op_pb_fr, pb_block_beta())
         + RO_SEAL_STR(0, _.op_sval ? _.op_sval : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_pl_block() {
    x86_begin();
    return IF(_.op_pb_off < 0, x86_bomb("bb_call_pl_block: no LOWER slot grant (TMP-ERADICATE)"))
         + IF(_.op_pb_off >= 0 && _.op_sprec && (_.op_pb_zsp || !_.op_zres || _.op_xf_off != -1), x86_alpha())
         + IF(_.op_pb_off >= 0 && _.op_sprec && (_.op_pb_zsp || !_.op_zres || _.op_xf_off != -1),
              x86_bomb("bb_call_pl_block: the plan puts this call's resume record on the spine, but the call is not a retaining spine call"))
         + IF(_.op_pb_off >= 0 && !(_.op_sprec && (_.op_pb_zsp || !_.op_zres || _.op_xf_off != -1)), pb_block_arm());
}
