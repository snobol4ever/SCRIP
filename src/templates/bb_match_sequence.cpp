#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string seq_dispatch_chain(long N, int base, int lo) { std::string r; for (long i = lo; i < N; i++) r += x86("cmp", "eax", (int)i) + x86("je", PAIR((int)(base + i))); return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZB-FC-3b (ARCH-ZETA S13 Tier C, SEQUENCE -- design in zeta_storage.c fc_seq_*): the FORTH arm owns NO CELL.
 * S10c's port invariant already puts rsp at exactly the depth each sigma/phi transition needs, and the SOURCE
 * element determines the target statically, so seq_i and the saved delta are DEAD -- the LIFO stack position
 * IS the sequence position.  The driver re-points the inside edges (fc_seq_sigma_tgt/fc_seq_phi_tgt); what is
 * left here is pure wiring: alpha enters element 0; na_s (last element's sigma landing) is an EMPTY glue that
 * falls into S.gamma; S.beta resumes the RIGHTMOST element (always N-1 -- never a runtime choice, since after
 * a successful match all N elements are suspended); na_f (element 0's phi landing) is EMPTY and falls into
 * S.omega.  op_fc_bytes stays 0, so the central alpha/omega hook is dormant: ZERO rsp motion is emitted. */
static inline int sfc() { return x86_port_mode() == ZC_PORT_FORTH && _.op_fc_seq; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_sequence() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (sfc())
        return x86("comment", "IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)")
             + x86_alpha()
             + x86("jmp", PAIR(0))
             + x86("def", PAIR((int)(2 * _.op_ival)))
             + x86_gamma()
             + x86_beta()
             + x86("jmp", PAIR((int)(2 * _.op_ival - 1)))
             + x86("def", PAIR((int)(2 * _.op_ival + 1)))
             + x86_omega();
    return _.op_off < 0
             ? x86_alpha() + x86_bomb("IR_MATCH_SEQUENCE: cursor slot not granted (zls)")
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
