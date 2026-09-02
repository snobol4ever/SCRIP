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
    if (_.op_sa < 0 && !_.op_zres) return x86_alpha() + x86_bomb("bb_suspend: no expr-value slot (needs descr flat-chain producer)");
    return x86("comment", _.op_zres ? "IR_SUSPEND yield+resume [PL-ZK-2 ZD: ZOPQ->ZRES]" : "IR_SUSPEND yield+resume")
         + x86_alpha()
         + (_.op_sb >= 0 && _.lbl_t1_p ? x86_lea_tgt("rax", X86T_TGT1) + x86("mov", FRQ(_.op_sb), "rax") : std::string())
         + (_.op_zres ?
                x86("note", ZRESN())
              + x86("mov", "rax", ZOPQ(0, 0))
              + x86("mov", ZRES(0), "rax")
              + x86("note", ZRESN())
              + x86("mov", "rax", ZOPQ(0, 8))
              + x86("mov", ZRES(8), "rax")
            :
                x86("mov", "rax", FRQ(_.op_sa))
              + x86("mov", FRQ(0), "rax")
              + x86("mov", "rax", FRQ(_.op_sa + 8))
              + x86("mov", FRQ(8), "rax"))
         + x86_scan_sync_out()
         + x86_gamma()
         + x86_beta()
         + x86_scan_sync_in_rr()
         + (_.lbl_t0 && _.op_suspend_stmt_uclaim > 0 ? x86_sub("rsp", _.op_suspend_stmt_uclaim) : std::string())
         + (_.lbl_t0 ? x86_jmp_tgt(X86T_TGT0) : x86_omega());
}
