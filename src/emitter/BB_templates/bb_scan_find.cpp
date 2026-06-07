#include <string>
#include <cstring>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_scan_find_str() {
    if (!PLATFORM_X86 || !(g_descr_flat_chain && _.op_off >= 0 && _.op_name1 && _.op_name1[0] && strlen(_.op_name1) <= 32)) return x86_bomb("bb_scan_find: unhandled (needs nonempty literal needle <=32 + descr flat-chain slot)");
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                         + x86("comment", "BOX ICN IR_SCAN_FIND find(s1) [ICN-SCAN-10 fstranl.r function{*} SCAN GENERATOR: cursor[r12+cur]<-δ; L0: cursor>Δ-m->ω; unrolled literal"
                           " byte-compare s[cursor+i]==s1[i] (miss->L1 advance); all match -> {DT_I,cursor+1}->slot->γ (cursor persists); L1: cursor++ ->L0; β RE-PUMPS: cursor++ ->L0]"))
         + x86("mov", FRQ(_.op_off + 16), "r14")
         + x86("def", L(0))
         + x86("mov", "rax", FRQ(_.op_off + 16))
         + x86("mov", "rcx", "r15")
         + x86("sub", "rcx", (long)strlen(_.op_name1))
         + x86("cmp", "rax", "rcx")
         + x86("jg",  "ω")
         + FOR(0, (int)strlen(_.op_name1), [&](int i) -> std::string {
               return IF(i == 0, x86("mov", "rcx", "rax"))
                    + IF(i != 0, x86("mov", "rcx", "rax") + x86("add", "rcx", (long)i))
                    + x86("movzx", "esi", "[r13+rcx]")
                    + x86("cmp64", "rsi", (long)(unsigned char)_.op_name1[i])
                    + x86("jne", L(1)); })
         + x86("mov", FRQ(_.op_off), (long)DT_I)
         + x86("add", "rax", (long)1)
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86("jmp", "γ")
         + x86("def", L(1))
         + x86("inc", FRQ(_.op_off + 16))
         + x86("jmp", L(0))
         + x86("def", "β")
         + x86("inc", FRQ(_.op_off + 16))
         + x86("jmp", L(0));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_scan_find(IR_t * pBB) { (void)pBB; x86_begin(); bb_emit_x86(bb_scan_find_str()); }
