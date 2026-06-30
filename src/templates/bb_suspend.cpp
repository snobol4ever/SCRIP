#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
/* IR_SUSPEND — Icon `suspend EXPR do BODY` inside a user-defined generator procedure.
   α: copy the EXPR value DESCR into frame[0]=[r12+0], store this suspend's β into the
   resume-pointer slot (so the next proc_gen_β indirect-jmps here), then jmp γ (= yield).
   β (resume): jump into the DO-BODY (lbl_t0) or to proc ω if bare suspend (lbl_t0==NULL).
   The resume-pointer slot (op_sb) is written BEFORE yielding; proc_gen_β does
   jmp qword ptr [r12+resume_slot] to resume at the right β each time. */
std::string bb_suspend() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_sa < 0)
        return x86_bomb("bb_suspend: no expr-value slot (needs descr flat-chain producer)");

    std::string s;
    s += x86("comment", "IR_SUSPEND yield+resume");
    s += x86("label", _.lbl_α);

    /* Store this suspend's β into the resume-pointer slot BEFORE yielding.
       On next proc_gen_β entry, the indirect jmp lands at this suspend's β.
       TEXT:   lea rax, [rip + lbl_t1]  /  mov qword ptr [r12 + op_sb], rax
       BINARY: 48 8D 05 <rel32(lbl_t1)>  /  49 89 84 24 <slot_u32>
       In binary the rel32 must be patched via a J-record pointing at port X86T_TGT1. */
    if (_.op_sb >= 0 && _.lbl_t1_p) {
        if (!MEDIUM_BINARY) {
            char load_β[128];
            snprintf(load_β, sizeof load_β,
                " lea rax, [rip + %s]\n mov qword ptr [r12 + %d], rax\n",
                _.lbl_t1_p->name, _.op_sb);
            s += std::string(load_β);
        } else {
            /* lea rax, [rip + tgt1]  →  L\x03 0x48 0x8D 0x05  J\x05  (TGT1 = port 5)
               mov [r12 + slot], rax  →  L\x08 0x49 0x89 0x84 0x24 <slot_u32 LE> */
            uint32_t slotv = (uint32_t)(unsigned)_.op_sb;
            s += x86_Lrec(x86_b3(0x48, 0x8D, 0x05)) + x86_Jrec(X86T_TGT1);
            std::string mov_insn; mov_insn += (char)0x49; mov_insn += (char)0x89; mov_insn += (char)0x84; mov_insn += (char)0x24;
            mov_insn += (char)(slotv); mov_insn += (char)(slotv>>8); mov_insn += (char)(slotv>>16); mov_insn += (char)(slotv>>24);
            s += x86_Lrec(mov_insn);
        }
    }

    /* Copy expr DESCR into frame[0] (the caller reads the yielded value there) */
    s += x86("mov", "rax", FRQ(_.op_sa));
    s += x86("mov", FRQ(0), "rax");
    s += x86("mov", "rax", FRQ(_.op_sa + 8));
    s += x86("mov", FRQ(8), "rax");

    /* Yield: jump to proc γ (returns eax=1 to caller) */
    s += x86("jmp", "γ");

    /* β (resume entry): jump to do-body α, or to proc ω if bare suspend */
    s += x86("def", "β");
    s += _.lbl_t0 ? x86_jmp_tgt(X86T_TGT0) : x86("jmp", "ω");

    return s;
}
