#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
/* IR_REPALT — Icon repeated alternation `|e`.  These three small fragments are emitted by flat_drive_repalt around the
   internally-driven sub-expression e.  A one-bit `yielded` flag lives at [r12 + op_off + 16] (mirroring bb_limit's
   counter slot, claimed adjacent to the 16-byte result slot at op_off).  The flag toggles JCON ir_a_RepAlt's
   MoveLabel/IndirectGoto target: cleared on every fresh start of e, set the moment e yields a value.  On exhaustion
   flat_drive_repalt tests it — set means restart e (infinite repeat), clear means |e produced nothing so it fails. */
/*--------------------------------------------------------------------------------------------------------------------*/
/* Fresh-start / restart entry: yielded := 0. */
std::string bb_repalt_clear() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_REPALT yielded:=0")
         + x86("mov", FRQ(_.op_off + 16), (long)0);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* e yielded a value: copy its DESCR (type+val) into the REPALT result slot, then yielded := 1. */
std::string bb_repalt_yield() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_sa < 0) return x86_bomb("bb_repalt_yield: sub-expression value slot not materialised");
    return x86("comment", "IR_REPALT yield (copy value, yielded:=1)")
         + x86("mov", "rax", FRQ(_.op_sa))
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", "rax", FRQ(_.op_sa + 8))
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86("mov", FRQ(_.op_off + 16), (long)1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* e exhausted: load the flag and compare to 1 (flat_drive_repalt emits je-restart / jmp-ω after this). */
std::string bb_repalt_test() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_REPALT test yielded")
         + x86("mov", "rax", FRQ(_.op_off + 16))
         + x86("cmp", "rax", (long)1);
}
