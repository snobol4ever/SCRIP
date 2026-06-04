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
    if (!PLATFORM_X86) return std::string();
    int off = _.op_off, cur = _.op_off + 16; const char * cs = _.op_name1;
    if (!(g_descr_flat_chain && off >= 0 && cs && cs[0] && strlen(cs) <= 32)) return std::string();
    long m = (long) strlen(cs);
    std::string cmps;
    for (long i = 0; i < m; i++) {
        if (i == 0) cmps += x86("mov", "rcx", "rax");
        else        cmps += x86("mov", "rcx", "rax") + x86("add", "rcx", (long)i);
        cmps += x86("movzx", "esi", "[r13+rcx]")
              + x86("cmp64", "rsi", (long)(unsigned char)cs[i])
              + x86("jne", L(1));
    }
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                         + x86("comment", "BOX ICN IR_SCAN_FIND find(s1) [ICN-SCAN-10 fstranl.r function{*} SCAN GENERATOR: cursor[r12+cur]<-δ; L0: cursor>Δ-m->ω; unrolled literal byte-compare s[cursor+i]==s1[i] (miss->L1 advance); all match -> {DT_I,cursor+1}->slot->γ (cursor persists); L1: cursor++ ->L0; β RE-PUMPS: cursor++ ->L0]"))
         + x86("mov", FRQ(cur), "r14")
         + x86("def", L(0))
         + x86("mov", "rax", FRQ(cur))
         + x86("mov", "rcx", "r15")
         + x86("sub", "rcx", (long)m)
         + x86("cmp", "rax", "rcx")
         + x86("jg",  PORT_OMEGA)
         + cmps
         + x86("mov", FRQ(off), (long)DT_I)
         + x86("add", "rax", (long)1)
         + x86("mov", FRQ(off + 8), "rax")
         + x86("jmp", PORT_GAMMA)
         + x86("def", L(1))
         + x86("inc", FRQ(cur))
         + x86("jmp", L(0))
         + x86("def", PORT_BETA)
         + x86("inc", FRQ(cur))
         + x86("jmp", L(0));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_scan_find(IR_t * pBB) {
    (void)pBB;
    x86_begin();
    std::string s = bb_scan_find_str();
    if (s.empty()) { bb_emit_x86(x86_bomb("bb_scan_find: unhandled (needs nonempty literal needle <=32 + descr flat-chain slot)")); return; }
    bb_emit_x86(s);
}
