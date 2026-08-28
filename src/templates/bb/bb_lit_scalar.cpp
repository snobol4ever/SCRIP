#include <string>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
void rt_icn_cset_register(const char *ptr, int len);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t blsc_bits(double d, uint64_t b = 0) { memcpy(&b, &d, 8); return b; }
static int descr_stamp_on(void) {
    static int on = -1;
    if (on < 0)
        { const char * e = getenv("SCRIP_DESCR_STAMP"); on = (e && *e == '1') ? 1 : 0; }
    return on;
}
static long lit_tag_imm(long base_tag) {
    if (!descr_stamp_on()) return base_tag;
    return base_tag
         | ((_.node ? (long)_.node->op + 1 : (long)DESCR_MOD_OP_UNSTAMPED) << 8)
         | (((_.nid <= 0 || _.nid > 0xFFFE) ? (long)DESCR_SRC_NODE_OVERFLOW : (long)_.nid) << 16);
}
static inline const char * ls_rq(int w) { return _.op_zres ? ZRES(w) : FRQ(_.op_off + w); }
static inline const char * ls_rd(int w) { return _.op_zres ? ZRESD(w) : FR(_.op_off + w); }
static inline std::string ls_dual(int w) {
    return (_.op_zres && _.op_off >= 0 && g_emit_cfg && g_emit_cfg->pl_cells_graph)
         ? x86("mov", "rax", ZRES(w))
         + x86("mov", FRQ(_.op_off + w), "rax")
         : std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_lit_scalar() {
    return _.op_node_kind == (int)IR_LIT_INTEGER && (_.op_off >= 0 || _.op_zres)
         ? x86("comment", "IR_LIT_INTEGER")
             + x86_alpha()
             + x86("note",   ZRESN())
             + x86("mov",    ls_rq(0), lit_tag_imm((long)DT_I))
             + x86("mov",    "rax", ROQ(0))
             + x86("note",   ZRESN())
             + x86("mov",    ls_rq(8), "rax")
             + ls_dual(0) + ls_dual(8)
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",    L(0))
             + x86(".quad",  (uint64_t)_.op_ival)
         : _.op_node_kind == (int)IR_LIT_STRING && (_.op_off >= 0 || _.op_zres)
         ? x86("comment", "IR_LIT_STRING")
             + x86_alpha()
             + x86("note",   ZRESN())
             + x86("mov",    ls_rq(0), lit_tag_imm((long)DT_S))
             + x86("note",   ZRESN())
             + (_.op_a_node_kind >= 0
                 /* icn-cset-embedded-nul-four-layer-gap layer 4 (string side): a literal carrying a
                    length operand (see lower_icon.c icn_attach_lit_len) may contain embedded \0 --
                    strlen(_.op_sval) would undercount it. BINARY bakes the TRUE length (its bytes come
                    from a direct pointer into the compiler's own live buffer, never re-serialized, so
                    length and content already agree). TEXT still emits strlen() here on purpose: the
                    .string directive below re-escapes through a NUL-terminated scan (x86_asm_str_escape)
                    and truncates at the same byte today, so baking the true length there would claim
                    more bytes than the assembled buffer actually holds -- a real out-of-bounds read, not
                    a fix. Deliberate, documented MODES-MAY-DIVERGE arm; TEXT-side byte emission is a
                    separate, tracked follow-up. Literals with no length operand (every other frontend)
                    take neither arm and fall through to the original unconditional strlen() below. */
                 ? IF_M3(x86("mov", ls_rd(4), (long)_.op_a_ival_sg))
                 + IF_M4(x86("mov", ls_rd(4), (long)(_.op_sval ? strlen(_.op_sval) : 0)))
                 : x86("mov",    ls_rd(4), (long)(_.op_sval ? strlen(_.op_sval) : 0)))
             + x86("mov",    "rax", ROQ(0))
             + x86("note",   ZRESN())
             + x86("mov",    ls_rq(8), "rax")
             + ls_dual(0) + ls_dual(8)
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",    L(0))
             + x86(".quad",  LS(0), _.op_sval ? _.op_sval : "")
             + x86("label",  LS(0))
             + x86(".string", _.op_sval ? _.op_sval : "")
         : _.op_node_kind == (int)IR_LIT_NAME && (_.op_off >= 0 || _.op_zres)
         ? x86("comment", "IR_LIT_NAME")
             + x86_alpha()
             + x86("note",   ZRESN())
             + x86("mov",    ls_rq(0), lit_tag_imm((long)DT_N))
             + x86("note",   ZRESN())
             + x86("mov",    ls_rd(4), (long)0)
             + x86("mov",    "rax", ROQ(0))
             + x86("note",   ZRESN())
             + x86("mov",    ls_rq(8), "rax")
             + ls_dual(0) + ls_dual(8)
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",    L(0))
             + x86(".quad",  LS(0), _.op_sval ? _.op_sval : "")
             + x86("label",  LS(0))
             + x86(".string", _.op_sval ? _.op_sval : "")
         : _.op_node_kind == (int)IR_LIT_CHARSET && (_.op_off >= 0 || _.op_zres)
         ? x86("comment", "IR_LIT_CHARSET")
             + x86_alpha()
             + x86("note",   ZRESN())
             + x86("mov",    ls_rq(0), lit_tag_imm((long)DT_S))
             + x86("note",   ZRESN())
             + x86("mov",    ls_rd(4), (long)-1)
             + x86("mov",    "rax", ROQ(0))
             + x86("note",   ZRESN())
             + x86("mov",    ls_rq(8), "rax")
             + ls_dual(0) + ls_dual(8)
             /* icn-cset-embedded-nul-four-layer-gap layer 4/5 (charset side): .slen stays the CSETVAL
                -1 identity sentinel above (load-bearing in >=10 runtime sites, never a length itself);
                the TRUE member count -- known here only when lower_icon.c attached one via
                icn_attach_lit_len (Icon literals only; every other IR_LIT_CHARSET producer does not
                exist) -- is registered by pointer so kw_cset_len/c_rt_size_d resolve it instead of
                falling to strlen(), which undercounts a set containing byte 0. BINARY-only (IF_M3):
                in BINARY the baked ROQ(0) address already points at the compiler's own correct,
                embedded-NUL-safe buffer, so registering here is exact. TEXT is deliberately left
                unregistered -- its .string bytes below are still NUL-truncated by x86_asm_str_escape
                (a separate, tracked follow-up), so a TEXT-mode registration would claim a length the
                assembled buffer does not actually hold; letting kw_cset_len miss and fall through to
                strlen() keeps TEXT exactly as self-consistent (short pointer, short reported length)
                as it was before this row. */
             + (_.op_a_node_kind >= 0
                 ? IF_M3(x86("push", "rax") + x86("push", "rdx")
                       + x86("mov", "rdi", ROQ(0))
                       + x86("mov", "rsi", (long)_.op_a_ival_sg)
                       + x86("call", "rt_icn_cset_register", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, int))rt_icn_cset_register)
                       + x86("pop", "rdx") + x86("pop", "rax"))
                 : std::string())
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",    L(0))
             + x86(".quad",  LS(0), _.op_sval ? _.op_sval : "")
             + x86("label",  LS(0))
             + x86(".string", _.op_sval ? _.op_sval : "")
         : _.op_node_kind == (int)IR_LIT_REAL && (_.op_off >= 0 || _.op_zres)
         ? x86("comment", "IR_LIT_REAL")
             + x86_alpha()
             + x86("note",   ZRESN())
             + x86("mov",    ls_rq(0), lit_tag_imm((long)DT_R))
             + x86("mov",    "rax", ROQ(0))
             + x86("note",   ZRESN())
             + x86("mov",    ls_rq(8), "rax")
             + ls_dual(0) + ls_dual(8)
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",    L(0))
             + x86(".quad",  blsc_bits(_.op_dval))
         : x86("comment", "IR_LIT_scalar")
             + x86_alpha()
             + x86_gamma()
             + x86_beta_trampoline();
}
