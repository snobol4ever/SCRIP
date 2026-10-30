#include <string>
#include <string.h>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "descr.h"
DESCR_t rt_pl_ax_cold(DESCR_t *, int, const char *);
DESCR_t rt_pl_cmp_cold(DESCR_t *, int, const char *);
DESCR_t rt_pl_is_cold(DESCR_t *, int);
DESCR_t rt_pl_zguard_cold(DESCR_t *, int);
DESCR_t rt_pl_anum_cold(DESCR_t *, int);
DESCR_t rt_pl_type_cold(DESCR_t *, int, const char *);
DESCR_t rt_pl_atop_cold(DESCR_t *, int, const char *);
int rtx_pl_unify(DESCR_t *, DESCR_t *);
void rt_pl_tr_refuse(const char *);
void rt_pl_db_decls_install(const long *, void *);
DESCR_t rt_pl_dop_unify_raise(DESCR_t *, DESCR_t *);
#include "rt/gc_heap.h"
}
#include "rt/rt_pl_trail.h"
#include "x86_asm.h"
#include "bb_pl_cell.h"
#include "bb_pl_leaf_kinds.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_L_COLD 190
#define PL_L_OK 180
#define PL_L_FAIL 195
#define PLA() (_.op_call_argbase)
#define PLR() (_.op_call_resoff)
#define PLN() ((int)_.op_ival)
#define PLK() (_.op_pl_kind)
#define PLS() (_.op_pl_sub)
#define PL_L_ZF 248
#define PL_L_ZX 249
#define PLAQ(off) (_.op_zres ? RDQ("rsp", (off)) : FRQ(PLA() + (off)))
#define PL_GAMMA() (_.op_zres ? x86("jmp", L(PL_L_ZX)) : x86_gamma())
#define PL_OMEGA(cc) (_.op_zres ? x86((cc), L(PL_L_ZF)) : x86_omega(cc))
#define PL_TRAMP() IF(!_.op_zres, x86_beta_trampoline())
#define PL_TAG_DT(c) ((c) == 'I' ? (long)DT_I : (c) == 'R' ? (long)DT_R : (c) == 'B' ? (long)DT_BIG : (c) == 'S' ? (long)DT_S : (c) == 'A' ? (long)DT_PLATOM : (long)DT_PLREF)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_OPSTR(reg) ( \
      x86("directive", ".section .rodata") \
    + x86("directive", (std::string(_.op_pl_oplbl) + ": .string \"" + _.op_pl_op + "\"").c_str()) \
    + x86("directive", ".section .text") \
    + x86("directive", ".intel_syntax noprefix") \
    + x86("lea", (reg), "[rip + __]", (uint64_t)(uintptr_t)_.op_pl_op, _.op_pl_oplbl) \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_deref(const char * r, int b, int l_unb, int l_cold, int uniform) {
    return x86("def", L(b))
         + x86("mov", "eax", RDD(r, 0))
         + x86("cmp", "al", (long)DT_PLVAR)
         + x86("je", L(b + 1))
         + x86("cmp", "al", (long)DT_N)
         + x86("jne", L(b + 2))
         + x86("mov", "ecx", RDD(r, 4))
         + x86("cmp", "ecx", (long)1)
         + x86("jne", L(l_cold))
         + x86("def", L(b + 1))
         + x86("mov", "rcx", RDQ(r, 8))
         + x86("test", "rcx", "rcx")
         + x86("jz", L(l_cold))
         + x86("cmp", "rcx", r)
         + x86("je", L(l_unb))
         + x86("mov", r, "rcx")
         + x86("jmp", L(b))
         + x86("def", L(b + 2))
         + IF(uniform, x86("cmp", "al", (long)DT_SNUL)
                     + x86("je", L(l_unb))
                     + x86("cmp", "al", (long)DT_FAIL)
                     + x86("je", L(l_unb)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_set(const char * tag, const char * val) {
    return IF(!_.op_zres, x86("mov", FRQ(PLR()), tag)
                        + x86("mov", FRQ(PLR() + 8), val))
         + IF(_.op_zres && strcmp(val, "rdx") != 0, x86("mov", "rdx", val))
         + IF(_.op_zres && strcmp(tag, "rax") != 0, x86("mov", "rax", tag));
}
static std::string pl_set(long tag, const char * val) {
    return IF(!_.op_zres, x86("mov", FRQ(PLR()), tag)
                        + x86("mov", FRQ(PLR() + 8), val))
         + IF(_.op_zres && strcmp(val, "rdx") != 0, x86("mov", "rdx", val))
         + IF(_.op_zres, x86("mov32", "eax", tag));
}
static std::string pl_set(long tag, long val) {
    return IF(!_.op_zres, x86("mov", FRQ(PLR()), tag)
                        + x86("mov", FRQ(PLR() + 8), val))
         + IF(_.op_zres, x86("mov32", "eax", tag)
                       + x86("mov32", "edx", val));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_cold_call(const char * sym, void * fp, int narg, int has_op) {
    return emit_seq({ x86("lea", "rdi", PLAQ(0))
                    + x86("mov32", "esi", (long)narg),
                      IF(has_op, PL_OPSTR("rdx")),
                      x86("call", sym, (uint64_t)(uintptr_t)fp),
                      _.op_zres ? std::string() : x86("mov", FRQ(PLR()), "rax")
                                                + x86("mov", FRQ(PLR() + 8), "rdx"),
                      _.op_zres ? x86_rt_gc_poll_res(x86_cap_raise(std::string())) : x86_rt_gc_poll_fails(),
                      _.op_zres ? x86("jmp", L(PL_L_ZX))
                                : x86("cmp", "al", (long)DT_FAIL)
                                + x86_omega("je") });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define pl_ok_store() ( \
      x86("def", L(PL_L_OK)) + pl_set((long)DT_I, 1L) + PL_GAMMA() \
)
#define pl_fail_store() ( \
      x86("def", L(PL_L_FAIL)) \
    + IF(!_.op_zres, pl_set((long)DT_FAIL, 0L) + x86_omega()) \
    + IF(_.op_zres, x86("jmp", L(PL_L_ZF))) \
)
#define pl_tail() IF(!_.op_zres, x86_gamma() + x86_beta() + x86_omega())
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_ax() {
    return IF(PLN() == 2 && PLS() != PLAX_OTHER,
               x86("lea", "rdi", PLAQ(0)) + pl_deref("rdi", 100, PL_L_COLD, PL_L_COLD, 0)
             + x86("cmp", "al", (long)DT_I)
             + x86("jne", L(PL_L_COLD))
             + x86("mov", "rdx", RDQ("rdi", 8))
             + x86("lea", "rsi", PLAQ(16)) + pl_deref("rsi", 110, PL_L_COLD, PL_L_COLD, 0)
             + x86("cmp", "al", (long)DT_I)
             + x86("jne", L(PL_L_COLD))
             + x86("mov", "rcx", RDQ("rsi", 8))
             + x86("mov", "rax", "rdx")
             + IF(PLS() == PLAX_ADD, x86("add", "rax", "rcx")
             + x86("jo", L(PL_L_COLD)))
             + IF(PLS() == PLAX_SUB, x86("sub", "rax", "rcx")
             + x86("jo", L(PL_L_COLD)))
             + IF(PLS() == PLAX_MUL, x86("imul", "rax", "rcx")
             + x86("jo", L(PL_L_COLD)))
             + IF(PLS() == PLAX_IDIV || PLS() == PLAX_MOD || PLS() == PLAX_REM,
                   x86("test", "rcx", "rcx")
                 + x86("jz", L(PL_L_COLD))
                 + x86("cmp", "rcx", (long)-1)
                 + x86("je", L(PL_L_COLD))
                 + x86("cqo")
                 + x86("idiv", "rcx"))
             + IF(PLS() == PLAX_REM, x86("mov", "rax", "rdx"))
             + IF(PLS() == PLAX_MOD,
                   x86("mov", "rax", "rdx")
                 + x86("test", "rax", "rax")
                 + x86("jz", L(120))
                 + x86("mov", "rdx", "rax")
                 + x86("xor", "rdx", "rcx")
                 + x86("jns", L(120))
                 + x86("add", "rax", "rcx")
                 + x86("def", L(120)))
             + pl_set((long)DT_I, "rax") + PL_GAMMA())
         + x86("def", L(PL_L_COLD)) + pl_cold_call("rt_pl_ax_cold", (void *)rt_pl_ax_cold, PLN(), 1) + pl_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_cmp() {
    return x86("lea", "rdi", PLAQ(0)) + pl_deref("rdi", 100, PL_L_COLD, PL_L_COLD, 0)
         + x86("cmp", "al", (long)DT_I)
         + x86("jne", L(PL_L_COLD))
         + x86("mov", "rdx", RDQ("rdi", 8))
         + x86("lea", "rsi", PLAQ(16)) + pl_deref("rsi", 110, PL_L_COLD, PL_L_COLD, 0)
         + x86("cmp", "al", (long)DT_I)
         + x86("jne", L(PL_L_COLD))
         + x86("mov", "rcx", RDQ("rsi", 8))
         + x86("cmp", "rdx", "rcx")
         + x86(_.op_pl_jcc, L(PL_L_FAIL))
         + pl_set((long)DT_I, "rdx") + PL_GAMMA()
         + pl_fail_store()
         + x86("def", L(PL_L_COLD)) + pl_cold_call("rt_pl_cmp_cold", (void *)rt_pl_cmp_cold, 2, 1) + pl_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_is() {
    return x86("lea", "rdi", PLAQ(16)) + pl_deref("rdi", 100, PL_L_COLD, PL_L_COLD, 0)
         + x86("cmp", "al", (long)DT_I)
         + x86("je", L(120))
         + x86("cmp", "al", (long)DT_R)
         + x86("je", L(120))
         + x86("cmp", "al", (long)DT_BIG)
         + x86("je", L(120))
         + x86("jmp", L(PL_L_COLD))
         + x86("def", L(120))
         + x86("mov", "rax", RDQ("rdi", 0))
         + x86("mov", "rdx", RDQ("rdi", 8))
         + IF(!_.op_zres, pl_set("rax", "rdx"))
         + IF(_.op_zres, x86("mov", PLAQ(16), "rax")
                       + x86("mov", PLAQ(24), "rdx"))
         + x86("lea", "rdi", PLAQ(0))
         + x86("lea", "rsi", PLAQ(16))
         + x86("call", "rtx_pl_unify", (uint64_t)(uintptr_t)(void *)rtx_pl_unify)
         + x86("test", "eax", "eax") + PL_OMEGA("jz")
         + IF(_.op_zres, x86("mov", "rax", PLAQ(16))
                       + x86("mov", "rdx", PLAQ(24))) + PL_GAMMA()
         + x86("def", L(PL_L_COLD))
         + x86("lea", "rdi", PLAQ(0))
         + x86("mov32", "esi", (long)2)
         + x86("call", "rt_pl_is_cold", (uint64_t)(uintptr_t)(void *)rt_pl_is_cold)
         + x86("mov", PLAQ(16), "rax")
         + x86("mov", PLAQ(24), "rdx")
         + IF(!_.op_zres, pl_set("rax", "rdx")) + x86_rt_gc_poll_fails()
         + x86("cmp", "al", (long)DT_FAIL) + PL_OMEGA("je")
         + x86("lea", "rdi", PLAQ(16))
         + x86("jmp", L(120)) + pl_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_tag_in(const char * tags, int l_yes) {
    return FOR(0, (int)strlen(tags), [&](int k) { return x86("cmp", "al", PL_TAG_DT(tags[k]))
                                                       + x86("je", L(l_yes)); });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_type() {
    return x86("lea", "rdi", PLAQ(0)) + pl_deref("rdi", 100, 130, PL_L_COLD, 1)
         + IF(PLS() == PLTYPE_VAR,
               x86("jmp", L(PL_L_FAIL))
             + x86("def", L(130))
             + x86("jmp", L(PL_L_OK)))
         + IF(PLS() == PLTYPE_NONVAR,
               x86("jmp", L(PL_L_OK))
             + x86("def", L(130))
             + x86("jmp", L(PL_L_FAIL)))
         + IF(PLS() == PLTYPE_TAGS,
               pl_tag_in(_.op_pl_tags, PL_L_OK)
             + IF(_.op_pl_rat, x86("cmp", "al", (long)DT_DATA))
             + IF(_.op_pl_rat, x86("je", L(PL_L_COLD)))
             + x86("def", L(130))
             + x86("jmp", L(PL_L_FAIL)))
         + pl_ok_store() + pl_fail_store()
         + x86("def", L(PL_L_COLD)) + pl_cold_call("rt_pl_type_cold", (void *)rt_pl_type_cold, 1, 1) + pl_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_atop() {
    return x86("lea", "rdi", PLAQ(0)) + pl_deref("rdi", 100, 130, PL_L_COLD, 1)
         + x86("cmp", "al", (long)DT_I)
         + x86("jne", L(PL_L_COLD))
         + x86("mov", "rdx", RDQ("rdi", 8))
         + x86("lea", "rsi", PLAQ(16)) + pl_deref("rsi", 110, PL_L_COLD, PL_L_COLD, 1)
         + x86("cmp", "al", (long)DT_I)
         + x86("jne", L(PL_L_COLD))
         + x86("mov", "rcx", RDQ("rsi", 8))
         + x86("cmp", "rdx", "rcx")
         + x86(_.op_pl_jcc, L(PL_L_FAIL))
         + x86("jmp", L(PL_L_OK))
         + x86("def", L(130))
         + x86("lea", "rsi", PLAQ(16)) + pl_deref("rsi", 120, 135, PL_L_COLD, 1)
         + x86("jmp", L(PL_L_COLD))
         + x86("def", L(135))
         + IF(PLS() == PLATOP_EQ,
               x86("cmp", "rdi", "rsi")
             + x86("jne", L(PL_L_FAIL))
             + x86("jmp", L(PL_L_OK)))
         + IF(PLS() == PLATOP_NE,
               x86("cmp", "rdi", "rsi")
             + x86("je", L(PL_L_FAIL))
             + x86("jmp", L(PL_L_OK)))
         + IF(PLS() == PLATOP_OTHER, x86("jmp", L(PL_L_COLD)))
         + pl_ok_store() + pl_fail_store()
         + x86("def", L(PL_L_COLD)) + pl_cold_call("rt_pl_atop_cold", (void *)rt_pl_atop_cold, 2, 1) + pl_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_zguard() {
    return x86("lea", "rdi", PLAQ(0)) + pl_deref("rdi", 100, PL_L_COLD, PL_L_COLD, 0)
         + x86("cmp", "al", (long)DT_I)
         + x86("jne", L(PL_L_COLD))
         + x86("mov", "rcx", RDQ("rdi", 8))
         + x86("test", "rcx", "rcx")
         + x86("jz", L(PL_L_COLD))
         + pl_ok_store()
         + x86("def", L(PL_L_COLD)) + pl_cold_call("rt_pl_zguard_cold", (void *)rt_pl_zguard_cold, 2, 0) + pl_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PLA_UNB(b) ((b) + 3)
#define PLA_NEXT(b) ((b) + 4)
static std::string pl_anum_arg(int rule, int aoff, int b) {
    return x86("lea", "rdi", PLAQ(aoff)) + pl_deref("rdi", b, PLA_UNB(b), PL_L_COLD, 1)
         + IF(rule == PLR_TEXT || rule == PLR_UNB_OR_TEXT, pl_tag_in("SA", PLA_NEXT(b)) + x86("jmp", L(PL_L_COLD)))
         + IF(rule == PLR_NUM, pl_tag_in("IRB", PLA_NEXT(b)) + x86("jmp", L(PL_L_COLD)))
         + IF(rule == PLR_TEXT_OR_NUM, pl_tag_in("SAIRB", PLA_NEXT(b)) + x86("jmp", L(PL_L_COLD)))
         + IF(rule == PLR_INT0 || rule == PLR_UNB_OR_INT0,
               x86("cmp", "al", (long)DT_I)
             + x86("jne", L(PL_L_COLD))
             + x86("mov", "rcx", RDQ("rdi", 8))
             + x86("test", "rcx", "rcx")
             + x86("js", L(PL_L_COLD))
             + x86("jmp", L(PLA_NEXT(b))))
         + IF(rule == PLR_INTCODE,
               x86("cmp", "al", (long)DT_I)
             + x86("jne", L(PL_L_COLD))
             + x86("mov", "rcx", RDQ("rdi", 8))
             + x86("cmp", "rcx", (long)0x10FFFF)
             + x86("ja", L(PL_L_COLD))
             + x86("jmp", L(PLA_NEXT(b))))
         + IF(rule == PLR_COMP,
               x86("cmp", "al", (long)DT_PLREF)
             + x86("jne", L(PL_L_COLD))
             + x86("jmp", L(PLA_NEXT(b))))
         + IF(rule == PLR_UNB, x86("jmp", L(PL_L_COLD)))
         + IF(rule == PLR_ANY || rule == PLR_NONVAR, x86("jmp", L(PLA_NEXT(b))))
         + x86("def", L(PLA_UNB(b)))
         + IF(rule == PLR_UNB || rule == PLR_UNB_OR_INT0 || rule == PLR_UNB_OR_TEXT, x86("jmp", L(PLA_NEXT(b))))
         + IF(!(rule == PLR_UNB || rule == PLR_UNB_OR_INT0 || rule == PLR_UNB_OR_TEXT), x86("jmp", L(PL_L_COLD)))
         + x86("def", L(PLA_NEXT(b)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_anum() {
    return IF(_.op_pl_rules != (const int *)0,
               FOR(0, PLN() - 1, [](int i) { return IF(_.op_pl_rules[i] != PLR_ANY, pl_anum_arg(_.op_pl_rules[i], 16 * (i + 1), 100 + 10 * i)); })
             + x86("jmp", L(PL_L_OK)) + pl_ok_store())
         + x86("def", L(PL_L_COLD)) + pl_cold_call("rt_pl_anum_cold", (void *)rt_pl_anum_cold, PLN(), 0) + pl_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_mkc() {
    return IF(!_.op_pl_fid_ok,
               x86_bomb("PL-MKC: the functor operand of a $mkc is the id the lowerer interned (an IR_LIT_INTEGER); the R1.3 re-intern fallback is deleted (the cto's check 4, 2026-10-03)"))
         + IF(_.op_pl_fid_ok,
               x86("mov32", "edi", (long)HB_DVEC)
             + x86("mov32", "esi", (long)(16 * (PLN() > 1 ? PLN() - 1 : 1)))
             + x86("call", "rt_gcheap_alloc", (uint64_t)(uintptr_t)(void *)rt_gcheap_alloc)
             + x86_movabs_r64("rcx", (uint64_t)DT_PLREF | ((uint64_t)(uint32_t)_.op_pl_fid << 32))
             + x86("note",
                 "the result cell is parked in argv[0] (the functor literal's cell, dead once its id is baked above) across the "
                     "poll and the kid loop: the box's result slot may be one of the kids' argv cells")
             + x86("mov", PLAQ(0), "rcx")
             + x86("mov", PLAQ(8), "rax")
             + x86_rt_gc_poll_or(PL_OMEGA("je"))
             + x86("mov", "r10", PLAQ(8))
             + x86("lea", "r9", PLAQ(16))
             + x86("mov", "r11", (long)(PLN() - 1))
             + x86("def", L(100))
             + x86("test", "r11", "r11")
             + x86("jz", L(102))
             + x86("mov", "rdi", "r9")
             + PL_DEREF(103, 104, 105, 106)
             + PL_UNBOUND(107, 108)
             + x86("mov", "rax", RDQ("rdi", 0))
             + x86("mov", "rdx", RDQ("rdi", 8))
             + x86("mov", RDQ("r10", 0), "rax")
             + x86("mov", RDQ("r10", 8), "rdx")
             + x86("jmp", L(101))
             + x86("def", L(107))
             + x86("note", "an unbound kid already on the heap (at or below rsp, the unifier's own test) is LINKED from the block, never rebound: binding the older heap variable to the newer "
                           "cell would move the variable on every term built over it and its standard order would follow the build order")
             + x86("cmp", "rdi", "rsp")
             + x86("ja", L(113))
             + x86("mov", RDQ("r10", 0), (long)DT_PLVAR)
             + x86("mov", RDQ("r10", 8), "rdi")
             + x86("jmp", L(101))
             + x86("def", L(113))
             + x86("mov", RDQ("r10", 0), (long)DT_PLVAR)
             + x86("mov", RDQ("r10", 8), "r10")
             + PL_TRAIL(109, 111, 160, 170)
             + x86("mov", RDQ("rdi", 0), (long)DT_PLVAR)
             + x86("mov", RDQ("rdi", 8), "r10")
             + x86("def", L(101))
             + x86("add", "r9", 16L)
             + x86("add", "r10", 16L)
             + x86("sub", "r11", 1L)
             + x86("jmp", L(100))
             + x86("def", L(102))
             + x86("mov", "rax", PLAQ(0))
             + x86("mov", "rdx", PLAQ(8))
             + IF(!_.op_zres, pl_set("rax", "rdx"))
             + PL_GAMMA()
             + x86("def", L(111))
             + x86("mov", "rdi", "r12")
             + x86("call", "rt_pl_tr_refuse", (uint64_t)(uintptr_t)(void *)rt_pl_tr_refuse)
             + PL_TRAMP());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_dbdecls() {
    return IF(_.op_pl_db_n < 2 || !_.op_pl_db_words, x86_bomb("$db_decls: the declaration table could not be read from the lowerer at emit time"))
         + IF(!(_.op_pl_db_n < 2 || !_.op_pl_db_words),
               x86_lea_id("rdi", 120) + x86("mov", "rsi", "r14")
             + x86("call", "rt_pl_db_decls_install", (uint64_t)(uintptr_t)(void *)rt_pl_db_decls_install)
             + x86_rt_gc_poll_or(PL_OMEGA("je"))
             + pl_set((long)DT_I, 1L)
             + PL_GAMMA()
             + x86("def", L(120))
             + x86(".quad", (uint64_t)(int64_t)_.op_pl_db_words[0]) + FOR(1, _.op_pl_db_n, [](int i) { return
             x86(".quad", (uint64_t)(int64_t)_.op_pl_db_words[i]); })
             + PL_TRAMP());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_unify() {
    return x86("lea", "rdi", PLAQ(0))
         + x86("lea", "rsi", PLAQ(16))
         + x86("call", "rtx_pl_unify", (uint64_t)(uintptr_t)(void *)rtx_pl_unify)
         + x86("test", "eax", "eax")
         + x86("jz", L(PL_L_COLD))
         + pl_set((long)DT_I, 1L) + PL_GAMMA()
         + x86("def", L(PL_L_COLD))
         + x86("note", "occurs_check error (the trail header's mode word reads 2): the leaf's failure is re-run on the cx road, which builds the ball, unwinds what it bound and arms r15")
         + x86("mov", "rdx", "r12")
         + x86("and", "rdx", (long)~(PL_TR_ARENA_BYTES - 1))
         + x86("cmp", RDQ("rdx", PL_TR_OCHECK_OFF), 2L)
         + x86("jne", L(PL_L_FAIL))
         + x86("lea", "rdi", PLAQ(0))
         + x86("lea", "rsi", PLAQ(16))
         + x86("call", "rt_pl_dop_unify_raise", (uint64_t)(uintptr_t)(void *)rt_pl_dop_unify_raise) + x86_rt_gc_poll_fails()
         + x86("jmp", L(PL_L_FAIL))
         + pl_fail_store() + pl_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string pl_leaf_inline_arm() {
    return IF(PLK() == PLK_MKC, pl_arm_mkc())
         + IF(PLK() == PLK_DBDECLS, pl_arm_dbdecls())
         + IF(PLK() == PLK_UNIFY, pl_arm_unify())
         + IF(PLK() == PLK_AX, pl_arm_ax())
         + IF(PLK() == PLK_CMP, pl_arm_cmp())
         + IF(PLK() == PLK_IS, pl_arm_is())
         + IF(PLK() == PLK_TYPE, pl_arm_type())
         + IF(PLK() == PLK_ATOP, pl_arm_atop())
         + IF(PLK() == PLK_ZGUARD, pl_arm_zguard())
         + IF(PLK() == PLK_ANUM, pl_arm_anum());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string pl_leaf_zd_tail() {
    return x86("def", L(PL_L_ZF))
         + x86("mov32", "eax", (long)DT_FAIL)
         + x86("xor", "edx", "edx")
         + x86("def", L(PL_L_ZX));
}
