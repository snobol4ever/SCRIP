#ifndef X86_ARMED_H
#define X86_ARMED_H
#include <string>
#include "emitter/emit_state_.h"
#include "ir/ir_node.h"
#include "emitter/emit_str.h"
#include "ir/stage2_sm_prog_api.h"
#include "runtime/builtins/gen_binop_f_numify_blank.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_ops.h"
#include "ir/descr_tags_dt_platom.h"
extern "C++" {
std::string bb_glue_binop_open(int base, const std::string & load, const std::string & store, long op, int ldecline, int truthy);
#define RK_ARMED_LOAD_FR() (x86("mov", "rdi", FRQ(_.op_sa)) + x86("mov", "rsi", FRQ(_.op_sa + 8)) + x86("mov", "rdx", FRQ(_.op_sb)) + x86("mov", "rcx", FRQ(_.op_sb + 8)))
#define RK_ARMED_STORE_FR() (x86("mov", FRQ(_.op_off), "rax") + x86("mov", FRQ(_.op_off + 8), "rdx"))
#define RK_ARMED_LOAD_ZD() \
    (x86("note", ZOPN(0)) + x86("mov", "rdi", ZOPQ(0, 0)) + x86("note", ZOPN(0)) + x86("mov", "rsi", ZOPQ(0, 8)) + x86("note", ZOPN(1)) + x86("mov", "rdx", ZOPQ(1, 0)) \
   + x86("note", ZOPN(1)) + x86("mov", "rcx", ZOPQ(1, 8)))
#define RK_ARMED_STORE_ZD() (x86("note", ZRESN()) + x86("mov", ZRES(0), "rax") + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx"))
#define RELOP_ARMED(truthy) IF(g_stage2.rk_infix_armed, bb_glue_binop_open(60, RK_ARMED_LOAD_FR(), RK_ARMED_STORE_FR(), (long)_.op_ival, 14, (truthy)) + x86("def", L(14)))
#define RELOP_NUMIFY_BLANK() ((_.op_strict & IR_STRICT_RELOP_NUMIFY_BLANK) != 0)
#define RELOP_NUM_TEXT() ((_.op_strict & IR_STRICT_RELOP_NUM_TEXT) != 0)
#define RELOP_OPW() ((long)_.op_ival | (RELOP_NUMIFY_BLANK() ? (long)BINOP_F_NUMIFY_BLANK : 0L) | (RELOP_NUM_TEXT() ? (long)BINOP_F_NUM_TEXT : 0L))
#define RELOP_RAISED_OMEGA() IF(RELOP_NUMIFY_BLANK(), x86_omega("js"))
#define ARITH_ARMED_FR() \
    IF(g_stage2.rk_infix_armed, \
          x86("mov", "eax", FR(_.op_sa)) \
        + x86("cmp", "al", (long)DT_DATA) \
        + x86("je", L(12)) \
        + x86("mov", "eax", FR(_.op_sb)) \
        + x86("cmp", "al", (long)DT_DATA) \
        + x86("jne", L(13)) \
        + x86("def", L(12)) \
        + bb_glue_binop_open(60, RK_ARMED_LOAD_FR(), RK_ARMED_STORE_FR(), (long)binop_base((long long)_.op_ival), 13, 0) \
        + x86("def", L(13)))
#define RK_ARMED_ZD(op, truthy) \
    IF(g_stage2.rk_infix_armed, \
          x86("note", ZOPN(0)) \
        + x86("mov", "eax", ZOPD(0, 0)) \
        + x86("cmp", "al", (long)DT_DATA) \
        + x86("je", L(12)) \
        + x86("note", ZOPN(1)) \
        + x86("mov", "eax", ZOPD(1, 0)) \
        + x86("cmp", "al", (long)DT_DATA) \
        + x86("jne", L(13)) \
        + x86("def", L(12)) \
        + bb_glue_binop_open(60, RK_ARMED_LOAD_ZD(), RK_ARMED_STORE_ZD(), (long)(op), 13, (truthy)) \
        + x86("def", L(13)))
#define ARITH_ARMED_ZD() RK_ARMED_ZD(binop_base((long long)_.op_ival), 0)
#define RELOP_ARMED_ZD(truthy) RK_ARMED_ZD(_.op_ival, (truthy))
}
#endif
