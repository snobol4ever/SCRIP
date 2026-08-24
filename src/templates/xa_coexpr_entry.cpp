#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string xa_coexpr_body_lea(const char * dst) {
    if (!_.lbl_t0)
        return x86_bomb("xa_coexpr_body_lea: coexpression body-entry target (t0 port) not threaded -- codegen_flat_chain_body's IR_CREATE resolution "
                        "must set g_emit.lbl_t0_p to the body-entry α-label before bb_create runs");
    return x86_lea_tgt(dst, X86T_TGT0);
}
