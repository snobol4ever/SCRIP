#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t icn_field_get(const char *fname, DESCR_t obj);
extern DESCR_t icn_field_get_at(const char *fname, DESCR_t obj, long at1);
extern DESCR_t rt_field_var(const char *fname, DESCR_t obj);
extern DESCR_t rt_field_var_strict(const char *fname, DESCR_t obj);
extern DESCR_t rt_field_var_strict_at(const char *fname, DESCR_t obj, long at1);
}
#include "x86_asm.h"
#define RO_SEAL_STR(n, s) \
    (x86("def", L(n)) \
   + x86(".quad", LS(n), (s)) \
   + x86("label", LS(n)) \
   + x86(".string", (s)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define FIELD_GET_ARGS() \
    (IF(_.op_zres, \
         x86("mov", "rdi", ROQ(0)) \
       + x86("note", ZOPN(0)) \
       + x86("mov", "rsi", ZOPQ(0, 0)) \
       + x86("note", ZOPN(0)) \
       + x86("mov", "rdx", ZOPQ(0, 8))) \
   + IF(!_.op_zres, \
         x86("mov", "rdi", ROQ(0)) \
       + x86("mov", "rsi", FRQ(_.op_a_slot)) \
       + x86("mov", "rdx", FRQ(_.op_a_slot + 8))))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string field_get_hinted(const char *at_name, void *at_addr, const char *plain_name, void *plain_addr) {
    return x86("mov", "ecx", (long)_.op_seal)
         + x86("def", L(1))
         + FIELD_GET_ARGS()
         + x86("test", "ecx", "ecx")
         + x86("je", L(2))
         + x86("call", at_name, (uint64_t)(uintptr_t)at_addr)
         + x86("cmp", "al", (long)RTX_NOT_HANDLED)
         + x86("jne", L(4))
         + x86("xor", "ecx", "ecx")
         + x86("jmp", L(1))
         + x86("def", L(4))
         + x86_rt_gc_poll_rec_res()
         + x86("jmp", L(3))
         + x86("def", L(2))
         + x86("call", plain_name, (uint64_t)(uintptr_t)plain_addr)
         + x86_rt_gc_poll_rec_res()
         + x86("def", L(3));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string field_get_call() {
    return (_.op_node_kind == IR_FIELD_VAR)
         ? (_.op_strict && _.op_seal > 0
            ? field_get_hinted("rt_field_var_strict_at", (void *)rt_field_var_strict_at, "rt_field_var_strict", (void *)rt_field_var_strict)
            : _.op_strict
            ? FIELD_GET_ARGS() + x86("call", "rt_field_var_strict", (uint64_t)(uintptr_t)(void *)rt_field_var_strict) + x86_rt_gc_poll_rec_res()
            : FIELD_GET_ARGS() + x86("call", "rt_field_var", (uint64_t)(uintptr_t)(void *)rt_field_var) + x86_rt_gc_poll_rec_res())
         : (_.op_seal > 0)
         ? field_get_hinted("icn_field_get_at", (void *)icn_field_get_at, "icn_field_get", (void *)icn_field_get)
         : FIELD_GET_ARGS() + x86("call", "icn_field_get", (uint64_t)(uintptr_t)(void *)icn_field_get) + x86_rt_gc_poll_rec_res();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_field_get() {
    return IF(_.op_zres,
               x86("comment", (_.op_node_kind == IR_FIELD_VAR) ? "IR_FIELD_GET lv zd" : "IR_FIELD_GET zd")
             + x86_alpha()
             + field_get_call()
             + x86("cmp", "al", std::to_string((long)DT_FAIL))
             + x86_omega("je")
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             + RO_SEAL_STR(0, _.op_sval ? _.op_sval : ""))
         + IF(!_.op_zres && (_.op_off < 0 || _.op_a_slot < 0), x86_alpha() + x86_bomb("bb_field_get: needs own slot + object operand slot"))
         + IF(!_.op_zres && !(_.op_off < 0 || _.op_a_slot < 0),
               x86("comment", (_.op_node_kind == IR_FIELD_VAR) ? "IR_FIELD_GET lv" : "IR_FIELD_GET")
         + x86_alpha()
         + field_get_call()
         + x86("cmp", "al", std::to_string((long)DT_FAIL))
         + x86_omega("je")
         + x86("mov", FRQ(_.op_off), "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline()
         + RO_SEAL_STR(0, _.op_sval ? _.op_sval : ""));
}
