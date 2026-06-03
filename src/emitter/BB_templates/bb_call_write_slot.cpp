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
void rt_pop_write_int_nl(void);
void rt_pop_write_any_nl(void);
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
    IR_t * a0 = pBB->α;
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
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment("# BOX IR_CALL write(op) [GZ-7 flat-chain slot -> rt_write_any_nl]")
             + s_2asm("mov", emit_fmt("rdi, [r12+%d]", off))
             + s_2asm("mov", emit_fmt("rsi, [r12+%d]", off + 8))
             + s_2asm("call", "rt_write_any_nl@PLT")
             + s_2asm("jmp", _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp", beta_tgt ? beta_tgt->name : _.lbl_ω);
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_write_binop_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    IR_t * a0 = pBB->α;
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
        std::string tail = s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                         + s_2asm("jmp", beta_tgt && beta_tgt->name[0] ? beta_tgt->name : _.lbl_ω);
        if (a0->t == IR_BINOP && a0->ival == BINOP_CONCAT)
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_2asm("mov rdi,", emit_fmt("[r12 + %d]", off + 8))
                 + s_2asm("call",     "rt_write_strz_nl@PLT")
                 + s_2asm("jmp",      _.lbl_γ) + tail;
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_2asm("mov rdi,", emit_fmt("[r12 + %d]", off))
             + s_2asm("call",     "rt_write_int_nl@PLT")
             + s_2asm("jmp",      _.lbl_γ) + tail;
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_write_legacy_str(IR_t * pBB, int arg_is_any) {
    if (!PLATFORM_X86) return std::string();
    bb_label_t * beta_tgt = bb_call_write_beta_target();
    const char * trailer_sym = arg_is_any ? "rt_pop_write_any_nl@PLT" : "rt_pop_write_int_nl@PLT";
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_2asm("call", trailer_sym)
             + s_2asm("jmp",  _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",  _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        uint64_t fptr;
        if (arg_is_any) { void (*fp)(void) = rt_pop_write_any_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        else            { void (*fp)(void) = rt_pop_write_int_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        std::string s;
        s += x86("call", arg_is_any ? "rt_pop_write_any_nl" : "rt_pop_write_int_nl", fptr);
        s += x86("jmp", PORT_GAMMA); s += x86("def", PORT_BETA);
        if (beta_tgt == _.lbl_ω_p) s += x86("jmp", PORT_OMEGA);
        else { s += x86_pair_jmp(0); }
        return s;
    }
    return std::string();
}
