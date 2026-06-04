#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
#include "emit_bb.h"
#include "../../runtime/builtins/gen.h"
void rt_write_any_nl(DESCR_t d);
void rt_write_strz_nl(const char *s);
void rt_write_int_nl(int64_t v);
int  bb_slot_get(IR_t * nd);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static bb_label_t * bb_call_write_beta_target() {
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++)
        if (g_emit.xa_bb_emit_pair_define[i] == _.lbl_β_p && g_emit.xa_bb_emit_pair_jmp[i])
            return g_emit.xa_bb_emit_pair_jmp[i];
    return _.lbl_ω_p;
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_write_slot_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    IR_t * a0 = _.node->α;
    int off = bb_slot_get(a0);
    bb_label_t * beta_tgt = bb_call_write_beta_target();
    if (MEDIUM_BINARY) {
        uint64_t fptr; { void (*fp)(DESCR_t) = rt_write_any_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        std::string s;
        s += x86_frame_load64("rdi", off);
        s += x86_frame_load64("rsi", off + 8);
        s += x86("call", "rt_write_any_nl", fptr);
        s += x86("jmp", PORT_GAMMA);
        s += x86("def", PORT_BETA);
        if (beta_tgt == _.lbl_ω_p) { s += x86("jmp", PORT_OMEGA); }
        else { s += x86_pair_jmp(0); }
        return s;
    }
    if (MEDIUM_TEXT) {
        return x86("label", _.lbl_α)
             + x86("comment", "BOX IR_CALL write(op) [GZ-7 flat-chain slot -> rt_write_any_nl]")
             + x86("ins2", "mov", emit_fmt("rdi, [r12+%d]", off))
             + x86("ins2", "mov", emit_fmt("rsi, [r12+%d]", off + 8))
             + x86("ins2", "call", "rt_write_any_nl@PLT")
             + x86("ins2", "jmp", _.lbl_γ)
             + x86("Lins1", emit_fmt("%s:", _.lbl_β), "")
             + x86("ins2", "jmp", beta_tgt ? beta_tgt->name : _.lbl_ω);
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_write_binop_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    IR_t * a0 = _.node->α;
    bb_label_t * beta_tgt = bb_call_write_beta_target();
    int off = bb_slot_get(a0);
    if (off < 0) { fprintf(stderr, "[GZ-3] FATAL bb_call_write_binop: write(binop) — slot miss\n"); abort(); }
    if (MEDIUM_BINARY) {
        if (a0->t == IR_BINOP && a0->ival == BINOP_CONCAT) {
            uint64_t fptr; { void (*fp)(const char *) = rt_write_strz_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            std::string s;
            s += x86_frame_load64("rdi", off + 8);
            s += x86("call", "rt_write_strz_nl", fptr);
            s += x86("jmp", PORT_GAMMA); s += x86("def", PORT_BETA);
            if (beta_tgt == _.lbl_ω_p) s += x86("jmp", PORT_OMEGA);
            else { s += x86_pair_jmp(0); }
            return s;
        }
        uint64_t fptr; { void (*fp)(int64_t) = rt_write_int_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        std::string s;
        s += x86_frame_load64("rdi", off);
        s += x86("call", "rt_write_int_nl", fptr);
        s += x86("jmp", PORT_GAMMA); s += x86("def", PORT_BETA);
        if (beta_tgt == _.lbl_ω_p) s += x86("jmp", PORT_OMEGA);
        else { s += x86_pair_jmp(0); }
        return s;
    }
    if (MEDIUM_TEXT) {
        std::string tail = x86("Lins1", emit_fmt("%s:", _.lbl_β), "")
                         + x86("ins2", "jmp", beta_tgt && beta_tgt->name[0] ? beta_tgt->name : _.lbl_ω);
        if (a0->t == IR_BINOP && a0->ival == BINOP_CONCAT)
            return x86("label", _.lbl_α)
                 + x86("ins2", "mov rdi,", emit_fmt("[r12 + %d]", off + 8))
                 + x86("ins2", "call",     "rt_write_strz_nl@PLT")
                 + x86("ins2", "jmp",      _.lbl_γ) + tail;
        return x86("label", _.lbl_α)
             + x86("ins2", "mov rdi,", emit_fmt("[r12 + %d]", off))
             + x86("ins2", "call",     "rt_write_int_nl@PLT")
             + x86("ins2", "jmp",      _.lbl_γ) + tail;
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_write_legacy_str(IR_t * pBB, int arg_is_any) {
    (void)pBB; (void)arg_is_any;
    return x86_bomb("bb_call_write_legacy: write(non-slot arg) not yet supported — awaits bb_var tier");
}
