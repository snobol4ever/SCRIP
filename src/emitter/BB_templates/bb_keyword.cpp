#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_descr_flat_chain;
extern int g_icn_scan_regs_live;
struct DESCR_t rt_icn_keyword_subject(void);
struct DESCR_t rt_icn_keyword_pos(void);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static const char * bkw(void) { return !_.op_sval ? "" : (_.op_sval[0] == '&' ? _.op_sval + 1 : _.op_sval); }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bkw_tail() { return x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω"); }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bkw_call_slot(const char * nm, uint64_t fptr) {
    return x86("call", nm, fptr)
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + bkw_tail();
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_keyword_str() {
    if (!PLATFORM_X86) return std::string();
    if (!(g_descr_flat_chain && _.op_off >= 0)) return x86_bomb("bb_keyword: no slot");
    if (!strcmp(bkw(), "subject")) {
        return IF(g_icn_scan_regs_live,
                   IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                                 + x86("comment", "BOX ICN IR_KEYWORD &subject [ICN-SCAN-1 reg arm: {DT_S,0,r13}->slot->γ; r13=Σ subject base]"))
                 + x86("mov", FRQ(_.op_off),     (long)DT_S)
                 + x86("mov", FRQ(_.op_off + 8), "r13")
                 + bkw_tail())
             + IF(!g_icn_scan_regs_live,
                   IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                                 + x86("comment", "BOX ICN IR_KEYWORD &subject [x86() stackless: rt_icn_keyword_subject->slot->γ]"))
                 + bkw_call_slot("rt_icn_keyword_subject", (uint64_t)(uintptr_t)(void *)rt_icn_keyword_subject));
    }
    if (!strcmp(bkw(), "pos")) {
        return IF(g_icn_scan_regs_live,
                   IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                                 + x86("comment", "BOX ICN IR_KEYWORD &pos [ICN-SCAN-1 reg arm: {DT_I, r14+1}->slot->γ; r14=δ 0-based cursor so &pos = δ+1]"))
                 + x86("mov", FRQ(_.op_off), (long)DT_I)
                 + x86("mov", "rax", "r14")
                 + x86("add", "rax", (long)1)
                 + x86("mov", FRQ(_.op_off + 8), "rax")
                 + bkw_tail())
             + IF(!g_icn_scan_regs_live,
                   IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                                 + x86("comment", "BOX ICN IR_KEYWORD &pos [x86() stackless: rt_icn_keyword_pos->slot->γ]"))
                 + bkw_call_slot("rt_icn_keyword_pos", (uint64_t)(uintptr_t)(void *)rt_icn_keyword_pos));
    }
    if (!strcmp(bkw(), "null")) {
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", "BOX ICN IR_KEYWORD &null [x86() stackless: {DT_SNUL,0}->slot->γ]"))
             + x86("mov", FRQ(_.op_off),     (long)DT_SNUL)
             + x86("mov", FRQ(_.op_off + 8), (long)0)
             + bkw_tail();
    }
    if (!strcmp(bkw(), "fail")) {
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", "BOX ICN IR_KEYWORD &fail [x86() stackless: ->ω]"))
             + x86("jmp", "ω")
             + x86("def", "β")
             + x86("jmp", "ω");
    }
    return x86_bomb("bb_keyword: unsupported keyword");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_keyword(IR_t * pBB) { (void)pBB; x86_begin(); bb_emit_x86(bb_keyword_str()); }
