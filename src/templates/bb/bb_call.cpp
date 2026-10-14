#include <string>
#include <string.h>
#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
extern std::string bb_call_bool_relop();
extern std::string bb_call_bool_slot();
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define BCR(r) (_.op_call_route == (r))
#define BCR_KNOWN() (BCR(CALL_ROUTE_DVAL2_BOMB) || BCR(CALL_ROUTE_PROC_STAGED) || BCR(CALL_ROUTE_RK_BOOL_RELOP) || BCR(CALL_ROUTE_RK_BOOL_SLOT))
#define BCR_UNKINDED() (BCR(CALL_ROUTE_BYNAME) || BCR(CALL_ROUTE_BYNAME_GEN) || BCR(CALL_ROUTE_FN))
#define BCR_FATAL() (fprintf(stderr, "[IBB] FATAL bb_call: unsupported call shape fn='%s'\n", _.op_sval ? _.op_sval : ""), abort(), std::string())
std::string bb_call() {
    return IF(BCR(CALL_ROUTE_DVAL2_BOMB), x86_alpha() + x86_bomb("CALL dval=2 descr-chain arm aborted per LANGUAGE-BLIND rule"))
         + IF(BCR(CALL_ROUTE_PROC_STAGED), bb_call_proc_staged())
         + IF(BCR(CALL_ROUTE_RK_BOOL_RELOP), bb_call_bool_relop())
         + IF(BCR(CALL_ROUTE_RK_BOOL_SLOT), bb_call_bool_slot())
         + IF(BCR_UNKINDED(), x86_alpha() + x86_bomb("bb_call: a by-name call reached the emitter as IR_CALL -- LOWER's last traversal (lower_call_forms_all) did not re-kind it by road"))
         + IF(!BCR_KNOWN() && !BCR_UNKINDED(), BCR_FATAL());
}
