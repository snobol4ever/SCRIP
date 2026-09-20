#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
DESCR_t str_concat_d(DESCR_t a, DESCR_t b);
DESCR_t str_concat_fracdigit_d(DESCR_t a, DESCR_t b);
DESCR_t rt_icn_lconcat_d(DESCR_t a, DESCR_t b);
}
#include "x86_asm.h"
#include <cstdlib>
#include <cstdio>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int bcs_ok() { return _.op_off >= 0 && binop_is_concat((long)_.op_ival) && _.op_sa >= 0 && _.op_sb >= 0; }
static inline const char *bcs_rt_name() { return _.op_ival == BINOP_LCONCAT ? "rt_icn_lconcat_d" : _.op_ival == BINOP_CONCAT_FRACDIGIT ? "str_concat_fracdigit_d" : "str_concat_d"; }
static inline void *bcs_rt_addr() { return _.op_ival == BINOP_LCONCAT ? (void*)rt_icn_lconcat_d : _.op_ival == BINOP_CONCAT_FRACDIGIT ? (void*)str_concat_fracdigit_d : (void*)str_concat_d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int bcs_null_side() { if (getenv("SCRIP_OPT_NULLCAT") && getenv("SCRIP_OPT_NULLCAT")[0] == '0') return -1;
    return (_.op_ival == BINOP_CONCAT_FRACDIGIT || _.op_ival == BINOP_LCONCAT) ? -1 : _.op_snul_a_ok ? 1 : _.op_snul_b_ok ? 0 : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int bcs_inline_ok() { return bcs_ok() && !_.op_zres && _.op_ival != BINOP_LCONCAT && !(getenv("SCRIP_GC_INLINE_CONCAT") && getenv("SCRIP_GC_INLINE_CONCAT")[0] == '0'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcs_inline_concat() {
    x86_begin();
    return x86("comment", "CEO-990 / Lon 2026-09-20: THE BOX ALLOCATES INSTEAD OF CALLING C TO ALLOCATE.  The guards below are every case str_concat_d handles DIFFERENTLY, so each one is a MISS to the existing call rather than a behaviour change: both operands DT_S (a pattern, a list, a real, a bignum, FAIL or a DT_DATA goes to the leaf), neither slen the 0xFFFFFFFF ask-strlen sentinel, neither slen zero (IS_NULL_fn returns the other operand's descriptor UNCHANGED, which an allocate-and-copy would not), and neither pointer null.")
         + x86("mov", "eax", FR(_.op_sa))
         + x86("cmp", "al", (long)DT_S)
         + x86("jne", L(0))
         + x86("mov", "eax", FR(_.op_sb))
         + x86("cmp", "al", (long)DT_S)
         + x86("jne", L(0))
         + x86("mov", "esi", FR(_.op_sa + 4))
         + x86("test", "esi", "esi")
         + x86("je", L(0))
         + x86("cmp", "esi", (long)0xFFFFFFFF)
         + x86("je", L(0))
         + x86("mov", "edi", FR(_.op_sb + 4))
         + x86("test", "edi", "edi")
         + x86("je", L(0))
         + x86("cmp", "edi", (long)0xFFFFFFFF)
         + x86("je", L(0))
         + x86("mov", "rax", FRQ(_.op_sa + 8))
         + x86("test", "rax", "rax")
         + x86("je", L(0))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("test", "rcx", "rcx")
         + x86("je", L(0))
         + x86("comment", "⛔ THE STRING-EXTENSION OWNER GUARD, FIVE INSTRUCTIONS, GRANTED BY LON 2026-09-20 (CEO-996b) AND IT IS THE REASON THIS BOX DOES NOT MAKE THE ACCEPTANCE BENCHMARK QUADRATIC.  str_concat_d is NOT allocate-and-copy: when a is the top-most heap block it GROWS IT IN PLACE (rt_sxt_match / rt_sxt_extend), which is what keeps `s := s || \"x\"` LINEAR.  An inline path that always allocated would make it quadratic while every gate stayed green.  ⛔ AND IT READS NO HIDDEN SYMBOL AND DEREFERENCES NOTHING: g_sxt_fr is visibility(\"hidden\") because rtx_str.s reaches it RIP-direct, so emitted code cannot read the owner in either medium, and a header read at [a.s - 8] would fault on a string literal that is not a heap block at all.  The owner is BY CONSTRUCTION the block that ENDS AT top (rt_sxt_note's own condition), and a block's end is a.s + round16(alen+1), so top - a.s - alen lands in [1,16] for the owner and outside it for everything else -- pure arithmetic over top, the pointer and the length, all three of which are already in registers.  Conservative in the safe direction: a false hit goes to the leaf, which decides.")
         + x86_gc_fr_load("r10")
         + x86("mov", "rcx", RDQ("r10", GCFR_TOP))
         + x86("sub", "rcx", "rax")
         + x86("sub", "rcx", "rsi")
         + x86("cmp", "rcx", (long)17)
         + x86("jb", L(0))
         + x86("comment", "the payload is alen + blen + 1 for the terminating NUL, exactly as rt_str_alloc(al + bl) asks for it; the bump rounds it and adds the 16-byte header, and ANY miss inside it lands on the same slow label as the guards above")
         + x86("lea", "r8", RDQ("rsi", 1))
         + x86("add", "r8", "rdi")
         + x86_gc_bump_inline_at((long)DT_S, 0)
         + x86("comment", "⛔ THE INLINE PATH DOES NOT CALL rt_sxt_note, AND THAT IS A DESIGN CHOICE MEASURED RATHER THAN AN OVERSIGHT: it does not claim ownership, so the next concat on a freshly inlined block sees a block that DOES end at top, takes the guard above to the leaf, and the runtime re-establishes the extension chain itself.  Writing the owner from emitted code would need the hidden record and is not done.")
         + x86("lea", "rdi", RDQ("rbx", 16))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "ecx", FR(_.op_sa + 4))
         + x86("rep_movsb")
         + x86("mov", "rsi", FRQ(_.op_sb + 8))
         + x86("mov", "ecx", FR(_.op_sb + 4))
         + x86("rep_movsb")
         + x86("mov", RDB("rdi", 0), (long)0)
         + x86("comment", "BSTRVAL(buf, al + bl): the tag word is DT_S with the provenance stamp ZERO and the length in the high dword, minted as two 32-bit stores because that word is {v:1, src_node:3, slen:4} (descr.h)")
         + x86("mov", "eax", FR(_.op_sa + 4))
         + x86("add", "eax", FR(_.op_sb + 4))
         + x86("mov", FR(_.op_off), (long)DT_S)
         + x86("mov", FR(_.op_off + 4), "eax")
         + x86("lea", "rax", RDQ("rbx", 16))
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86_gamma()
         + x86("def", L(0));
}
std::string bb_binop_concat_slot() {
    if (_.op_zres && bcs_null_side() >= 0) {
        const int k = bcs_null_side();
        return x86("comment", "IR_BINOP_CONCAT zd null-identity")
             + x86_alpha()
             + x86("note", ZOPN(k))
             + x86("mov",  "rax", ZOPQ(k, 0))
             + x86("note", ZOPN(k))
             + x86("mov",  "rdx", ZOPQ(k, 8))
             + x86("note", ZRESN())
             + x86("mov",  ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov",  ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    }
    if (!_.op_zres && bcs_ok() && bcs_null_side() >= 0) {
        const int s = bcs_null_side() ? _.op_sb : _.op_sa;
        return x86_alpha()
             + x86("comment", "IR_BINOP_CONCAT null-identity")
             + x86("mov", "rax", FRQ(s))
             + x86("mov", "rdx", FRQ(s + 8))
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    }
    if (_.op_zres)
        return x86("comment", "IR_BINOP_CONCAT zd")
             + x86_alpha()
             + x86("note", ZOPN(0))
             + x86("mov",  "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0))
             + x86("mov",  "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1))
             + x86("mov",  "rdx", ZOPQ(1, 0))
             + x86("note", ZOPN(1))
             + x86("mov",  "rcx", ZOPQ(1, 8))
             + x86("rtcc_wb")
             + x86("call_bare", bcs_rt_name(), (uint64_t)(uintptr_t)bcs_rt_addr())
             + x86("note", ZRESN())
             + x86("mov",  ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov",  ZRES(8), "rdx")
             + x86("rtcc_rl")
             + IF(_.op_ival == BINOP_LCONCAT, x86("note", ZRESN())
             + x86("mov", "eax", ZRESD(0))
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je"))
             + x86_gamma()
             + x86_beta_trampoline();
    return IF(bcs_ok(),
           x86_alpha()
         + x86("comment", "IR_BINOP_CONCAT")
         + IF(bcs_inline_ok(), bcs_inline_concat())
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("call_rt", bcs_rt_name(), (long)_.op_off, (uint64_t)(uintptr_t)bcs_rt_addr())
         + IF(_.op_ival == BINOP_LCONCAT, x86("mov", "eax", FR(_.op_off))
         + x86("cmp", "al", (long)DT_FAIL)
         + x86_omega("je"))
         + x86_gamma()
         + x86_beta_trampoline());
}
