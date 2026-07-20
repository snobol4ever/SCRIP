#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string seq_dispatch_chain(long N, int base, int lo) {
    return lo >= N
             ? std::string()
             : x86("cmp", "eax", (int)lo)
             + x86("je", PAIR((int)(base + lo)))
             + seq_dispatch_chain(N, base, lo + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_sequence() {
    x86_begin();
    return !PLATFORM_X86
             ? std::string()
             : (x86_port_mode() == ZC_PORT_FORTH && _.op_fc_seq)
             ? x86("comment", "IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)")
             + x86_alpha()
             + x86("jmp", PAIR(0))
             + x86("def", PAIR((int)(2 * _.op_ival)))
             + x86_gamma()
             + x86_beta()
             + x86("jmp", PAIR((int)(2 * _.op_ival - 1)))
             + x86("def", PAIR((int)(2 * _.op_ival + 1)))
             + x86_omega()
             : _.op_off < 0
             ? x86_alpha() + x86_bomb("IR_MATCH_SEQUENCE: cursor slot not granted (zls)")
             : _.op_seq_static
             ? x86("comment", "IR_MATCH_SEQ_NARY (SPD SEQ-STATIC: zero-counter, sigma/phi statically wired)")
             + x86_alpha()
             + x86("mov", FR(_.op_off), "r14d")
             + x86("jmp", PAIR(0))
             + x86("def", PAIR((int)(2 * _.op_ival)))
             + x86_gamma()
             + x86_beta()
             + x86("jmp", PAIR((int)(2 * _.op_ival - 1)))
             + x86("def", PAIR((int)(2 * _.op_ival + 1)))
             + x86_omega()
             : x86("comment", "IR_MATCH_SEQ_NARY")
             + x86_alpha()
             + x86("mov", FR(_.op_off), "r14d")
             + x86("mov", FR(_.op_off + 4), 0)
             + x86("jmp", PAIR(0))
             + x86("def", PAIR((int)(2 * _.op_ival)))
             + x86("mov", "eax", FR(_.op_off + 4))
             + x86("add", "eax", 1)
             + x86("mov", FR(_.op_off + 4), "eax")
             + seq_dispatch_chain(_.op_ival, 0, 1)
             + x86_gamma()
             + x86_beta()
             + x86("mov", FR(_.op_off + 4), (int)_.op_ival)
             + x86("def", PAIR((int)(2 * _.op_ival + 1)))
             + x86("mov", "eax", FR(_.op_off + 4))
             + x86("sub", "eax", 1)
             + x86("mov", FR(_.op_off + 4), "eax")
             + seq_dispatch_chain(_.op_ival, (int)_.op_ival, 0)
             + x86_omega();
}
