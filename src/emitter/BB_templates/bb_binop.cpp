/* bb_binop.cpp — IR_BINOP router. The IR_BINOP kind has SIX distinct box shapes; each lives in
   its own one-box-one-file template. This router calls each box's _str() in priority order and
   emits the first that produces bytes (empty string = "this box does not apply, try the next").
   The per-box files (each a single Proebsting four-port box):
     bb_binop_lit_arith.cpp   GZ-3     RO const-int ADD/SUB (sealed blob operands)
     bb_binop_jct_relop.cpp   RK-EMIT-3 Raku junction-collapse relop
     bb_binop_relop.cpp       GZ-8     stackless numeric relop (slot→slot cmp)
     bb_binop_arith.cpp       GZ-9     stackless integer arithmetic (slot→slot DESCR)
     bb_binop_concat_slot.cpp GZ-11+   slot-based string concat
     bb_binop_concat_lit.cpp  GZ-4     RO literal string concat (sealed blob operands)
     bb_binop_agpure.cpp      legacy   AG-pure rt_arith/rt_acomp/rt_lcomp/rt_gen_concat fallback
   Routing order matches the original in-file fall-through: the specialized stackless arms are
   tried before the legacy vstack fallback. The macro-def short-circuit lives in the agpure box. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_lit_arith_str(IR_t * pBB, bb_bin_t & bin);
std::string bb_binop_jct_relop_str(IR_t * pBB, bb_bin_t & bin);
std::string bb_binop_relop_str(IR_t * pBB, bb_bin_t & bin);
std::string bb_binop_arith_str(IR_t * pBB, bb_bin_t & bin);
std::string bb_binop_concat_slot_str(IR_t * pBB, bb_bin_t & bin);
std::string bb_binop_concat_lit_str(IR_t * pBB, bb_bin_t & bin);
std::string bb_binop_agpure_str(IR_t * pBB, bb_bin_t & bin);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_binop(IR_t * pBB) {
    bb_bin_t bin;
    std::string s;
    s = bb_binop_lit_arith_str(pBB, bin);   if (!s.empty()) { bb_emit_asm_result(s, bin); return; }
    s = bb_binop_jct_relop_str(pBB, bin);   if (!s.empty()) { bb_emit_asm_result(s, bin); return; }
    s = bb_binop_relop_str(pBB, bin);       if (!s.empty()) { bb_emit_asm_result(s, bin); return; }
    s = bb_binop_arith_str(pBB, bin);       if (!s.empty()) { bb_emit_asm_result(s, bin); return; }
    s = bb_binop_concat_slot_str(pBB, bin); if (!s.empty()) { bb_emit_asm_result(s, bin); return; }
    s = bb_binop_concat_lit_str(pBB, bin);  if (!s.empty()) { bb_emit_asm_result(s, bin); return; }
    s = bb_binop_agpure_str(pBB, bin);      bb_emit_asm_result(s, bin);
}
