#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
DESCR_t rt_keyword_read_snobol4(const char *sval);
int rt_kw_index(const char *kw);
DESCR_t rt_kw_read_idx(int64_t idx);
const char *rt_kw_direct_sym(int idx, int *soff, const void **base);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SNOBOL4 keywords are a distinct set from Icon's and are plain single-value reads:
 * no &subject/&pos scan-register arms, no resumable-generator counter. The opcode
 * (IR_KEYWORD_SNOBOL4) selects this template; the emitter never tests a language. */
std::string bb_keyword_snobol4() {
    if (!PLATFORM_X86) return std::string();
    const int kwi = _.op_sval ? rt_kw_index(_.op_sval) : -1;   /* ⭐ KW-3 THE STATIC SLOT: the keyword's canonical block index, resolved AT EMIT TIME because `&TRIM` is a literal in the source and its index is a compile-time fact. kwi < 0 means either the killswitch is off (DEFAULT -- every arm below is then byte-identical to KW-2 by construction, which is what keeps the 529/529 md5 sweep green) or the block does not name this keyword (&ARB/&BAL/&REM/&FAIL and the pattern family), and BOTH cases fall through to the verbatim legacy name-string arm. */
    int soff = 0; const void *cbase = (const void *)0;
    const char *csym = (kwi >= 0) ? rt_kw_direct_sym(kwi, &soff, &cbase) : (const char *)0;   /* ⭐⭐⭐ KW-D (GOAL-SNOBOL4-100 line 510, Lon verbatim: "use direct references"): non-NULL = this keyword's cell is DIRECTLY ADDRESSABLE and the box emits a LOAD, not a call -- TEXT [rip+sym@GOTPCREL] then deref (the rt_anchor_g/bb_define precedent, R10's sanctioned media divergence), BINARY movabs of the in-process cell. NULL (STR family, SCRIP_KW_DIRECT=0, or a rebound block) keeps the KW-3 indexed-call arm verbatim. */
    if (_.op_zres && csym)
        return x86("comment", "IR_KEYWORD_SNOBOL4_read zd [KW-D direct cell]")
             + x86_alpha()
             + x86_load_got("rcx", csym, (uint64_t)(uintptr_t)cbase)
             + (soff ? x86("add", "rcx", (long)soff) : std::string())
             + x86("mov", "rdx", "[rcx]")
             + x86("mov", "rax", (long)DT_I)
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();   /* THE READ IS THREE INSTRUCTIONS AND ZERO CALLS: rcx <- &cell (GOT/movabs), rdx <- value, rax <- DT_I -- rax:rdx mirror rt_kw_read_idx's DESCR return convention EXACTLY (descr.h: v+slen are the FIRST eight bytes, the union the SECOND), so the two stores below are byte-identical to the call arm's and the ZD plan is unchanged (still a zero-operand K=16 leaf). Reading a live cell needs no lock and no validation -- validation is a WRITE-side fact (208/210/287/209 + KW-5, see rt_kw_write_idx, which every write still calls) -- and seeding is already done before statement one (KW-5b: core_lib_init). rcx scratch per the rtcc_anchor_cmp precedent; r10/r11 wires untouched. Port topology gamma-only, unchanged: a keyword READ never fails (manual Ch.16). */
    if (_.op_zres && kwi >= 0)
        return x86("comment", "IR_KEYWORD_SNOBOL4_read zd [KW-3 static idx]")
             + x86_alpha()
             + x86_ro_load_q("rdi", 0)
             + x86("call",    "rt_kw_read_idx", (uint64_t)(uintptr_t)(void *)rt_kw_read_idx)
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86_ro_seal_q(0, (uint64_t)(int64_t)kwi);   /* The sealed quad IS the keyword, statically present in the emitted program: one rip-relative load replaces the name-string pointer, and an O(1) array index replaces the two case-folds plus ~60-arm strcmp cascade the KW-1 census measured. Port topology is unchanged and still gamma-only -- a keyword READ always yields a value (manual Ch.16: protected keywords cannot be ASSIGNED, but reading one never fails), so there is no omega edge. The seal sits after the trampoline exactly where the legacy name tail sits: rip-relative constant data, independent of the frame base, so it neither moves nor needs compensation. */
    if (_.op_zres)
        return x86("comment", "IR_KEYWORD_SNOBOL4_read zd")
             + x86_alpha()
             + x86("mov",     "rdi", ROQ(0))
             + x86("call",    "rt_keyword_read_snobol4", (uint64_t)(uintptr_t)(void *)rt_keyword_read_snobol4)
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",     L(0))
             + x86(".quad",   LS(0), _.op_sval)
             + x86("label",   LS(0))
             + x86(".string", _.op_sval);   /* ZD-2l: a ZERO-OPERAND LEAF, the same shape as the LIT_* producers already on the cells -- zd_nops returns 0 for it and needs no edit, and zd_plan's K is 16 because it yields a value.  Only the STORE destination moves: the returned DESCR halves land in this box's own cell instead of the flat slot.  The sealed RO tail after the trampoline is carried verbatim -- it is rip-relative constant data that ROQ(0) reaches, wholly independent of the frame base, so it neither moves nor needs compensation.  Port topology is gamma-only and that is correct: a keyword READ always yields a value (manual Ch.16 -- protected keywords cannot be ASSIGNED, but reading one never fails), so there is no omega edge, matching the legacy arm below. */
    if (!(_.op_off >= 0)) return x86_alpha() + x86_bomb("bb_keyword_snobol4: no slot");
    if (csym)
        return x86("comment", "IR_KEYWORD_SNOBOL4_read [KW-D direct cell]")
             + x86_alpha()
             + x86_load_got("rcx", csym, (uint64_t)(uintptr_t)cbase)
             + (soff ? x86("add", "rcx", (long)soff) : std::string())
             + x86("mov", "rdx", "[rcx]")
             + x86("mov", "rax", (long)DT_I)
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();   /* flat-slot twin of the zd direct arm above -- same three instructions, only the store destination differs, exactly as the two call arms differ. */
    if (kwi >= 0)
        return x86("comment", "IR_KEYWORD_SNOBOL4_read [KW-3 static idx]")
             + x86_alpha()
             + x86_ro_load_q("rdi", 0)
             + x86("call",    "rt_kw_read_idx", (uint64_t)(uintptr_t)(void *)rt_kw_read_idx)
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86_ro_seal_q(0, (uint64_t)(int64_t)kwi);
    return x86("comment", "IR_KEYWORD_SNOBOL4_read")
         + x86_alpha()
         + x86("mov",     "rdi", ROQ(0))
         + x86("call",    "rt_keyword_read_snobol4", (uint64_t)(uintptr_t)(void *)rt_keyword_read_snobol4)
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline()
         + x86("def",     L(0))
         + x86(".quad",   LS(0), _.op_sval)
         + x86("label",   LS(0))
         + x86(".string", _.op_sval);
}
