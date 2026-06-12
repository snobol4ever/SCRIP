#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
#include "emit_bb.h"
DESCR_t rt_call_arr(const char * fn, DESCR_t * args, int nargs);
int bb_slot_alloc16(IR_t * nd);
int bb_slot_get(IR_t * nd);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_fn_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn = _.op_sval ? _.op_sval : "";
    int nargs = (int) _.op_ival;
    int resoff = bb_slot_alloc16(pBB);
    int argbase = resoff + 16;
    for (int i = 0; i < nargs; i++) {
        IR_t * ai = ir_call_arg(pBB, i);
        bb_slot_alloc16(ai ? ai : pBB);
    }
    std::string s = x86("label", _.lbl_α)
                  + x86("comment", emit_fmt("BOX IR_CALL %s(...) -> rt_call_arr [slot-based, FAIL->ω]", fn));
    for (int i = 0; i < nargs; i++) {
        IR_t * ai = ir_call_arg(pBB, i);
        int src = ai ? bb_slot_get(ai) : -1;
        int dst = argbase + i * 16;
        if (src >= 0) {
            if (MEDIUM_TEXT) {
                s += x86("ins2", "mov", emit_fmt("rax, qword ptr [r12+%d]", src));
                s += x86("ins2", "mov", emit_fmt("qword ptr [r12+%d], rax", dst));
                s += x86("ins2", "mov", emit_fmt("rax, qword ptr [r12+%d]", src + 8));
                s += x86("ins2", "mov", emit_fmt("qword ptr [r12+%d], rax", dst + 8));
            } else {
                s += x86_frame_load64("rax", src);
                s += x86_frame_store64(dst, "rax");
                s += x86_frame_load64("rax", src + 8);
                s += x86_frame_store64(dst + 8, "rax");
            }
        } else {
            if (MEDIUM_TEXT) {
                s += x86("ins2", "mov", emit_fmt("qword ptr [r12+%d], 6", dst));
                s += x86("ins2", "mov", emit_fmt("qword ptr [r12+%d], 0", dst + 8));
            } else {
                s += x86("mov", FRQ(dst), (long)6);
                s += x86("mov", FRQ(dst + 8), (long)0);
            }
        }
    }
    if (MEDIUM_TEXT) {
        std::string fl = emit_fmt(".Lcallfn_rk%d", g_flat_node_id++);
        s += x86("directive", ".section .rodata")
           + x86("directive", (fl + ": .string \"" + fn + "\"").c_str())
           + x86("directive", ".section .text")
           + x86("directive", ".intel_syntax noprefix");
        s += x86("ins2", "lea",  emit_fmt("rdi, [rip+%s]", fl.c_str()));
        s += x86("ins2", "lea",  emit_fmt("rsi, [r12+%d]", argbase));
        s += x86("ins2", "mov",  emit_fmt("edx, %d", nargs));
        s += x86("ins2", "call", "rt_call_arr@PLT");
        s += x86("ins2", "mov",  emit_fmt("[r12+%d], rax", resoff));
        s += x86("ins2", "mov",  emit_fmt("[r12+%d], rdx", resoff + 8));
        s += x86("ins2", "cmp",  "eax, 99");
        s += x86("ins2", "je",   _.lbl_ω);
        s += x86("ins2", "jmp",  _.lbl_γ);
        s += x86("Lins1", emit_fmt("%s:", _.lbl_β), "");
        s += x86("ins2", "jmp",  _.lbl_ω);
    } else {
        uint64_t fptr; { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_arr; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)fn);
        s += x86_frame_lea("rsi", argbase);
        s += x86("mov32", "edx", (long)nargs);
        s += x86_call_ro("rt_call_arr", fptr);
        s += x86_frame_store64(resoff, "rax");
        s += x86_frame_store64(resoff + 8, "rdx");
        s += x86("cmp", "eax", (long)99);
        s += x86("je", PORT_OMEGA);
        s += x86("jmp", PORT_GAMMA);
        s += x86("def", PORT_BETA);
        s += x86("jmp", PORT_OMEGA);
    }
    return s;
}
