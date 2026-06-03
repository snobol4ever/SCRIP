#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
#include "emit_bb.h"
#include "../../runtime/builtins/gen.h"
int  bb_slot_alloc16(IR_t * nd);
DESCR_t rt_call_arr(const char * fn, DESCR_t * args, int nargs);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
extern std::string marshal_call_arg(IR_t * lf, int aoff, IR_t * owner, int idx);
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_rk_arr_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn   = pBB->sval ? pBB->sval : "";
    int64_t      narg = pBB->ival;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) pBB->counter;
    int args_ok = 1;
    for (int i = 0; i < (int)narg; i++) {
        IR_t * lf = (subs && subs[i]) ? subs[i]->entry : NULL;
        if (!lf) { args_ok = 0; break; }
        int leaf   = (lf->t == IR_LIT_I || lf->t == IR_LIT_S || lf->t == IR_LIT_F || lf->t == IR_LIT_NUL || lf->t == IR_VAR);
        int nested = (lf->t == IR_CALL && lf->dval == 2.0);
        if (!leaf && !nested) { args_ok = 0; break; }
    }
    if (!args_ok) return std::string();
    int resoff  = bb_slot_alloc16(pBB);
    int argbase = (narg > 0) ? bb_slot_alloc16(subs[0]->entry) : resoff;
    for (int i = 1; i < (int)narg; i++) bb_slot_alloc16(subs[i]->entry);
    if (MEDIUM_TEXT) {
        std::string s = s_1asm(emit_fmt("%s:", _.lbl_α))
            + s_comment(emit_fmt("# BOX IR_CALL %s(...) [RK-EMIT-2 dval=2 -> rt_call_arr]", fn));
        for (int i = 0; i < (int)narg; i++)
            s += marshal_call_arg(subs[i]->entry, argbase + i * 16, pBB, i);
        std::string fl = emit_fmt(".Lcallfn%d", bb_node_id(pBB));
        s += s_directive(".section .rodata")
           + s_directive(fl + ": .string \"" + fn + "\"")
           + s_directive(".section .text") + s_directive(".intel_syntax noprefix");
        s += s_2asm("lea", emit_fmt("rdi, [rip+%s]", fl.c_str()));
        s += s_2asm("lea", emit_fmt("rsi, [r12+%d]", argbase));
        s += s_2asm("mov", emit_fmt("edx, %lld", (long long)narg));
        s += s_2asm("call", "rt_call_arr@PLT");
        s += s_2asm("mov", emit_fmt("[r12+%d], rax", resoff));
        s += s_2asm("mov", emit_fmt("[r12+%d], rdx", resoff + 8));
        s += s_2asm("jmp", _.lbl_γ);
        s += s_L1asm(emit_fmt("%s:", _.lbl_β), "");
        s += s_2asm("jmp", _.lbl_ω);
        return s;
    }
    if (MEDIUM_BINARY) {
        std::string s;
        for (int i = 0; i < (int)narg; i++)
            s += marshal_call_arg(subs[i]->entry, argbase + i * 16, pBB, i);
        uint64_t fptr; { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_arr; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        s += x86("mov", "rdi", (uint64_t)(uintptr_t)fn);
        s += x86_frame_load64("rsi", argbase);
        s += x86("mov32", "edx", (long)narg);
        s += x86("call", "rt_call_arr", fptr);
        s += x86_frame_store64(resoff, "rax");
        s += x86_frame_store64(resoff + 8, "rdx");
        s += x86("jmp", PORT_GAMMA);
        s += x86("def", PORT_BETA);
        s += x86("jmp", PORT_OMEGA);
        return s;
    }
    return std::string();
}
