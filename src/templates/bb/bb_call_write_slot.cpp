#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "../runtime/builtins/gen.h"
void rt_write_any_nl(DESCR_t d);
void rt_write_strz_nl(const char *s);
void rt_write_int_nl(int64_t v);
int  bb_slot_get(IR_t * nd);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static bb_label_t * bb_call_write_beta_target() {
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++)
        if (g_emit.xa_bb_emit_pair_define[i] == _.lbl_β_p && g_emit.xa_bb_emit_pair_jmp[i])
            return g_emit.xa_bb_emit_pair_jmp[i];
    return _.lbl_ω_p;
}
#define bcws_slot(off, beta_tgt) (x86_alpha() \
     + x86("mov", "rdi", FRQ(off)) \
     + x86("mov", "rsi", FRQ((off) + 8)) \
     + x86("call", "rt_write_any_nl", (uint64_t)(uintptr_t)(void*)rt_write_any_nl) \
     + x86_gamma() \
     + x86_beta() \
     + ((beta_tgt) == _.lbl_ω_p ? x86_omega() : x86_pair_jmp(0)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_write_slot_str(IR_t * pBB) {
    return bcws_slot(bb_slot_get(ir_call_arg(_.node, 0)), bb_call_write_beta_target());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define bcws_binop_concat(off, beta_tgt) (x86_alpha() \
     + x86("mov", "rdi", FRQ(off)) \
     + x86("mov", "rsi", FRQ((off) + 8)) \
     + x86("call", "rt_write_any_nl", (uint64_t)(uintptr_t)(void*)rt_write_any_nl) \
     + x86_gamma() \
     + x86_beta() \
     + ((beta_tgt) == _.lbl_ω_p ? x86_omega() : x86_pair_jmp(0)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define bcws_binop_int(off, beta_tgt) (x86_alpha() \
     + x86("mov", "rdi", FRQ(off)) \
     + x86("call", "rt_write_int_nl", (uint64_t)(uintptr_t)(void*)rt_write_int_nl) \
     + x86_gamma() \
     + x86_beta() \
     + ((beta_tgt) == _.lbl_ω_p ? x86_omega() : x86_pair_jmp(0)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_write_binop_str(IR_t * pBB) {
    if (bb_slot_get(ir_call_arg(_.node, 0)) < 0) { fprintf(stderr, "[GZ-3] FATAL bb_call_write_binop: write(binop) — slot miss\n"); abort(); }
    return (ir_call_arg(_.node, 0)->op == IR_BINOP && binop_is_concat((long)IR_LIT(ir_call_arg(_.node, 0)).ival))
         ? bcws_binop_concat(bb_slot_get(ir_call_arg(_.node, 0)), bb_call_write_beta_target())
         : bcws_binop_int(bb_slot_get(ir_call_arg(_.node, 0)), bb_call_write_beta_target());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_write_legacy_str(IR_t * pBB, int arg_is_any) {
    (void)pBB; (void)arg_is_any;
    return x86_bomb("bb_call_write_legacy: write(non-slot arg) not yet supported — awaits bb_var tier");
}
