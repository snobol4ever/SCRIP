#include <string>
#include <stdint.h>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t rt_subscript_var(DESCR_t base, DESCR_t idx);
extern DESCR_t rt_subscript_var_strict(DESCR_t base, DESCR_t idx);
extern DESCR_t rt_subscript_var_container_only(DESCR_t base, DESCR_t idx);
extern DESCR_t rt_subscript_var_container_only_strict(DESCR_t base, DESCR_t idx);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum { ARBLK_LO = 0, ARBLK_HI = 4, ARBLK_NDIM = 8, ARBLK_DATA = 32 };
#define LVCK_CONLY() (_.op_sval && !strcmp(_.op_sval, "lv-check-container-only"))
#define LVCK_FN() ((uint64_t)(uintptr_t)(void *)(LVCK_CONLY() ? (_.op_strict ? rt_subscript_var_container_only_strict : rt_subscript_var_container_only) \
                                                              : (_.op_strict ? rt_subscript_var_strict : rt_subscript_var)))
#define LVCK_LD(r, k, w) (_.op_zres \
                           ? x86("note", ZOPN(k)) \
                           + x86("mov", r, ZOPQ(k, w)) \
                           : x86("mov", r, FRQ((k ? _.op_sa : _.op_a_slot) + w)))
#define LVCK_RES() (LVCK_LD("rax", 0, 0) \
                  + (_.op_zres \
                      ? x86("note", ZRESN()) \
                      + x86("mov", ZRES(0), "rax") \
                      : x86("mov", FRQ(_.op_off), "rax")) \
                  + LVCK_LD("rax", 0, 8) \
                  + (_.op_zres \
                      ? x86("note", ZRESN()) \
                      + x86("mov", ZRES(8), "rax") \
                      : x86("mov", FRQ(_.op_off + 8), "rax")))
#define LVCK_KEEP() ((_.op_zres \
                      ? x86("note", ZRESN()) \
                      + x86("mov", ZRES(0), "rax") \
                      : x86("mov", FRQ(_.op_off), "rax")) \
                   + (_.op_zres \
                      ? x86("note", ZRESN()) \
                      + x86("mov", ZRES(8), "rdx") \
                      : x86("mov", FRQ(_.op_off + 8), "rdx")))
#define LVCK_NEEDS_SLOTS() (!_.op_zres && (_.op_off < 0 || _.op_a_slot < 0 || _.op_sa < 0))
static const char * lvck_sym(void) {
    return LVCK_CONLY() ? (_.op_strict ? "rt_subscript_var_container_only_strict" : "rt_subscript_var_container_only")
                        : (_.op_strict ? "rt_subscript_var_strict" : "rt_subscript_var");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_subscript_lvck() {
    return IF(LVCK_NEEDS_SLOTS(),
               x86_alpha()
             + x86_bomb("bb_subscript lv-check: needs own slot + base/index operand slots"))
         + IF(!LVCK_NEEDS_SLOTS(),
               x86("comment", "IR_SUBSCRIPT x[i] lv-check: the subject's subscript fails or errs before the object runs; passes the base through")
             + x86_alpha()
             + LVCK_LD("rdi", 0, 0)
             + LVCK_LD("rsi", 0, 8)
             + x86("cmp",     "dil", (long)DT_T)
             + x86("jne", L(0))
             + x86("test",    "rsi", "rsi")
             + x86("jne", L(2))
             + x86("jmp", L(1))
             + x86("def", L(0))
             + x86("cmp",     "dil", (long)DT_A)
             + x86("jne", L(1))
             + x86("test",    "rsi", "rsi")
             + x86("je", L(1))
             + LVCK_LD("rdx", 1, 0)
             + x86("cmp",     "dl", (long)DT_I)
             + x86("jne", L(1))
             + x86("mov",     "eax", RDD("rsi", ARBLK_NDIM))
             + x86("cmp",     "eax", (long)1)
             + x86("jne", L(1))
             + x86("mov",     "rax", RDQ("rsi", ARBLK_DATA))
             + x86("test",    "rax", "rax")
             + x86("je", L(1))
             + LVCK_LD("rcx", 1, 8)
             + x86("mov",     "eax", RDD("rsi", ARBLK_LO))
             + x86("movsxd",  "rax", "eax")
             + x86("cmp",     "rcx", "rax")
             + x86("jl", L(1))
             + x86("mov",     "eax", RDD("rsi", ARBLK_HI))
             + x86("movsxd",  "rax", "eax")
             + x86("cmp",     "rcx", "rax")
             + x86("jg", L(1))
             + x86("jmp", L(2))
             + x86("def", L(1))
             + LVCK_LD("rdi", 0, 0)
             + LVCK_LD("rsi", 0, 8)
             + LVCK_LD("rdx", 1, 0)
             + LVCK_LD("rcx", 1, 8)
             + x86("call",    lvck_sym(), LVCK_FN())
             + x86("cmp",     "al", (long)DT_FAIL)
             + x86_omega("je")
             + LVCK_KEEP()
             + x86_rt_gc_poll()
             + x86("def", L(2))
             + LVCK_RES()
             + x86_gamma()
             + x86_beta_trampoline());
}
