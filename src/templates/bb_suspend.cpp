#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_suspend() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_sa < 0)
        return x86_bomb("bb_suspend: no expr-value slot (needs descr flat-chain producer)");
    std::string s;
    s += x86("comment", "IR_SUSPEND yield+resume");
    s += x86("label", _.lbl_α);
    if (_.op_sb >= 0 && _.lbl_t1_p) {
        if (!MEDIUM_BINARY) {
            char load_β[128];
            snprintf(load_β, sizeof load_β,
                " lea rax, [rip + %s]\n mov qword ptr [r12 + %d], rax\n",
                _.lbl_t1_p->name, _.op_sb);
            s += std::string(load_β);
        } else {
            uint32_t slotv = (uint32_t)(unsigned)_.op_sb;
            s += x86_Lrec(x86_b3(0x48, 0x8D, 0x05)) + x86_Jrec(X86T_TGT1);
            std::string mov_insn; mov_insn += (char)0x49; mov_insn += (char)0x89; mov_insn += (char)0x84; mov_insn += (char)0x24;
            mov_insn += (char)(slotv); mov_insn += (char)(slotv>>8); mov_insn += (char)(slotv>>16); mov_insn += (char)(slotv>>24);
            s += x86_Lrec(mov_insn);
        }
    }
    s += x86("mov", "rax", FRQ(_.op_sa));
    s += x86("mov", FRQ(0), "rax");
    s += x86("mov", "rax", FRQ(_.op_sa + 8));
    s += x86("mov", FRQ(8), "rax");
    s += x86("jmp", "γ");
    s += x86("def", "β");
    s += _.lbl_t0 ? x86_jmp_tgt(X86T_TGT0) : x86("jmp", "ω");
    return s;
}
