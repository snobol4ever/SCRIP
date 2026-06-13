#include "bb_common.h"
extern "C" {
#include "IR_interp_state.h"
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bdisp(IR_t *pBB) {
    const char *fn = _.op_sval ? _.op_sval : "";
    std::string hdr = x86("label", _.lbl_α);
    std::string r;
    if (!(r = bb_io()).empty()) return r;
    if (!(r = bb_is_cmp_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_type_test_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_term_inspect_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_aggregate_nb()).empty()) return r;
    if (!(r = bb_atom_string()).empty()) return r;
    if (!(r = bb_term_io_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_findall_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_succ_plus_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_list_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_retract_throw_str(pBB, fn, hdr)).empty()) return r;
    if (MEDIUM_BINARY) return bytes(1, "\xE9") + u32le(0) + bytes(1, "\xE9") + u32le(0);
    return x86("comment", std::string("RESOLVE: unknown '") + fn + "' stub")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "γ");
}
std::string bb_resolve(IR_t * pBB) {
    return IF(PLATFORM_X86 && MEDIUM_MACRO_DEF, x86("comment", "no macro form — RESOLVE"))
         + IF(PLATFORM_X86 && !MEDIUM_MACRO_DEF, bdisp(pBB));
}
