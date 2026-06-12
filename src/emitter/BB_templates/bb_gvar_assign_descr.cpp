#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
#include "descr.h"
DESCR_t NV_SET_fn(const char * name, DESCR_t val);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * dst_name()  { return _.op_sval ? _.op_sval : ""; }
static inline uint64_t     dst_addr()  { return (uint64_t)(uintptr_t)dst_name(); }
static inline const char * dst_label() { const char * l = emit_intern_str(dst_name()); if (l) return l; static char b[64]; strtab_label(b, sizeof b, dst_name()); return b; }
static inline uint64_t     fn_nv_set() { DESCR_t (*f)(const char *, DESCR_t) = NV_SET_fn; return (uint64_t)(uintptr_t)(void *)f; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_gvar_assign_descr() {
    if (PLATFORM_X86) {
        int rhs = _.op_sa;
        int off = _.op_off;
        if (!(rhs >= 0 && off >= 0)) return x86_bomb("bb_gvar_assign_descr: needs rhs slot + own slot");
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", std::string("BOX IR_ASSIGN_DESCR NV_SET_fn global write rhs_slot=") + std::to_string(rhs) + " own_slot=" + std::to_string(off) + " [RO name sealed [rip+disp]]"))
             + x86_frame_load64("rsi", rhs)
             + x86_frame_load64("rdx", rhs + 8)
             + x86_ro_load_q("rdi", 0)
             + x86("call", "NV_SET_fn", fn_nv_set())
             + x86_frame_store64(off, "rax")
             + x86_frame_store64(off + 8, "rdx")
             + x86("jmp", "\xCE\xB3")
             + x86("def", "\xCE\xB2")
             + x86("jmp", "\xCF\x89")
             + x86_ro_seal_str(0, dst_name());
    }
    return std::string();
}