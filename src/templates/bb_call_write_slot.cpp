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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcws_slot_bin(int off, bb_label_t * beta_tgt) { uint64_t fptr; { void (*fp)(DESCR_t) = rt_write_any_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    return x86_alpha()
         + x86_frame_load64("rdi", off)
         + x86_frame_load64("rsi", off + 8)
         + x86("call", "rt_write_any_nl", fptr)
         + x86_gamma()
         + x86_beta()
         + (beta_tgt == _.lbl_ω_p ? x86_omega() : x86_pair_jmp(0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_write_slot_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    int off = bb_slot_get(ir_call_arg(_.node, 0));
    bb_label_t * beta_tgt = bb_call_write_beta_target();
    if (MEDIUM_BINARY) return bcws_slot_bin(off, beta_tgt);
    if (MEDIUM_TEXT) return x86_alpha()
                          + x86("comment", "BOX IR_CALL write(op) [GZ-7 flat-chain slot -> rt_write_any_nl]")
                          + x86("mov", "rdi", FRQ(off))
                          + x86("mov", "rsi", FRQ(off + 8))
                          + x86("call", "rt_write_any_nl@PLT")
                          + x86_gamma()
                          + x86("label", std::string(_.lbl_β))
                          + x86("label", _.lbl_β)
                          + x86("jmp", beta_tgt ? beta_tgt->name : _.lbl_ω);
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcws_binop_concat_bin(int off, bb_label_t * beta_tgt) { uint64_t fptr; { void (*fp)(DESCR_t) = rt_write_any_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    return x86_alpha()
         + x86_frame_load64("rdi", off)
         + x86_frame_load64("rsi", off + 8)
         + x86("call", "rt_write_any_nl", fptr)
         + x86_gamma()
         + x86_beta()
         + (beta_tgt == _.lbl_ω_p ? x86_omega() : x86_pair_jmp(0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcws_binop_int_bin(int off, bb_label_t * beta_tgt) { uint64_t fptr; { void (*fp)(int64_t) = rt_write_int_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    return x86_alpha()
         + x86_frame_load64("rdi", off)
         + x86("call", "rt_write_int_nl", fptr)
         + x86_gamma()
         + x86_beta()
         + (beta_tgt == _.lbl_ω_p ? x86_omega() : x86_pair_jmp(0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_write_binop_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    IR_t * a0 = ir_call_arg(_.node, 0);
    bb_label_t * beta_tgt = bb_call_write_beta_target();
    int off = bb_slot_get(a0);
    if (off < 0) { fprintf(stderr, "[GZ-3] FATAL bb_call_write_binop: write(binop) — slot miss\n"); abort(); }
    if (MEDIUM_BINARY) return (a0->op == IR_BINOP && binop_is_concat((long)IR_LIT(a0).ival))
                              ? bcws_binop_concat_bin(off, beta_tgt)
                              : bcws_binop_int_bin(off, beta_tgt);
    if (MEDIUM_TEXT) {
        std::string tail = x86("label", std::string(_.lbl_β))
                         + x86("jmp", beta_tgt && beta_tgt->name[0] ? beta_tgt->name : _.lbl_ω);
        if (a0->op == IR_BINOP && binop_is_concat((long)IR_LIT(a0).ival))
            return x86_alpha()
                 + x86("mov", "rdi", "[" + std::string(x86_zr()) + " + " + std::to_string(off) + "]")
                 + x86("mov", "rsi", "[" + std::string(x86_zr()) + " + " + std::to_string(off + 8) + "]")
                 + x86("call",     "rt_write_any_nl@PLT")
                 + x86("jmp",      _.lbl_γ) + tail;
        return x86_alpha()
             + x86("mov", "rdi", "[" + std::string(x86_zr()) + " + " + std::to_string(off) + "]")
             + x86("call",     "rt_write_int_nl@PLT")
             + x86("jmp",      _.lbl_γ) + tail;
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_write_legacy_str(IR_t * pBB, int arg_is_any) {
    (void)pBB; (void)arg_is_any;
    return x86_bomb("bb_call_write_legacy: write(non-slot arg) not yet supported — awaits bb_var tier");
}
