#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
extern int64_t kw_fnclevel;
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR_FUNC_ACTIVATE — LADDER AB (2026-08-08): per-DEFINE ACTIVATION BLOCK.  SPITBOL manual Ch.8 pp.102-106: DEFINE'd function saves fname/formals/locals on a pushdown stack at entry (our RBP frame),
 * restores them on RETURN/FRETURN/NRETURN.  Ch.16: &FNCLEVEL increments at call, decrements at return.  The block is jump-target-only dead code at AB-1 (call sites UNCHANGED); AB-3 flips the sites.
 *
 * FRAME LAYOUT (RBP-relative; RBP = frame base established by push rbp / mov rbp,rsp in α):
 *   [rbp - 0x08]  caller rbp (pushed by push rbp)          <- implicit, below rbp
 *   [rbp - K]     start of frame body (sub rsp,K below rbp)
 *   Frame cells at fixed offsets from rbp (depth-immune — the entire reason for the RBP frame):
 *   AB_OFF_GW      (-0x10): γ wire (caller's success continuation) — AB-2 fills
 *   AB_OFF_WW      (-0x18): ω wire (caller's failure continuation) — AB-2 fills
 *   AB_OFF_ERSP    (-0x20): entry rsp (caller's RSP before the call) — AB-2 fills
 *   AB_OFF_ANCHOR  (-0x28): prev ACT-ANCHOR value — AB-2 fills
 *   AB_OFF_WN      (-0x30): wn / rt_g_want_name snapshot — AB-2 fills
 *   AB_OFF_VTMARK  (-0x38): value-trail mark — AB-2 fills
 *   AB_OFF_SAVE0   (-0x40): save-set[0] type word   (fname)
 *   AB_OFF_SAVE0+8 (-0x38 ... overlaps vtmark? -- AB-1: save-set base = -0x40; nsave*16 bytes below vtmark slot; revisit with ABI freeze at AB-1 contracts/ header)
 *   NOTE: exact offsets and the Sigma/Sigma_len/kl fields finalized at ABI freeze (AB-1 contracts/ header).
 *         AB-1 emits a BOMB STUB β to surface any premature entry; real β lands at AB-2.
 *
 * op_sval = function name (compile-time constant)
 * op_ival = nsave = 1 + nformals + nlocals
 * op_arg_slot[k] = GVA index of save-set member k {fname, formal0..N-1, local0..M-1}; -1 if GVA not active */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* AB frame constant offsets from RBP (ALL NEGATIVE -- cells live BELOW the pushed rbp's saved value).
 * Sized to hold 6 meta fields + nsave*16 save-set DESCRs, 16-aligned.
 * AB-1: we size for the meta slots only; the save-set portion is calculated from op_ival.
 * AB-1 ABI NOTE: this is a DRAFT layout.  The contracts/ header (AB-1 deliverable) is the ONE AUTHORITY.
 * Fields listed match the AB-0 census frame ABI block exactly. */
static const int AB_META_BYTES = 6 * 8;   /* gw + ww + entry_rsp + prev_anchor + wn + vtmark  (8B each) */
static inline long ab_frame_k(long nsave) {
    long save_bytes = nsave * 16;   /* nsave DESCRs at 16B each */
    long total = AB_META_BYTES + save_bytes;
    return ((total + 8 + 15) & ~15L) - 8;   /* 16-align ACROSS the pushed rbp per bb_glue_framed convention */
}
/* RBP-relative offsets for save-set member k (DESCR = 16B: type@+0, value@+8) */
static inline int ab_save_off(long nsave, int k) { return -(int)(AB_META_BYTES + 16 * (nsave - 1 - k) + 16); }   /* grows downward; member 0 nearest to meta block */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_func_activate() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    long nsave = (long)_.op_ival;
    const char * fname = _.op_sval ? _.op_sval : "?";
    long K = ab_frame_k(nsave);
    /* ===== α — ENTRY (AB-1: frame + save-set save + null formals + null fname + &FNCLEVEL++ + bomb before body jmp) ===== */
    std::string s = x86("comment", std::string("IR_FUNC_ACTIVATE α — ") + fname + " (AB-1: frame+save; body-jmp + β = BOMB stubs until AB-2/AB-3)")
        + x86_alpha()
        + x86("push", "rbp")
        + x86("mov",  "rbp", "rsp")
        + x86("sub",  "rsp", K)
        /* Save save-set members from their GVA cells into RBP-frame slots (depth-immune [rbp+off] reads).
         * op_arg_slot[k] = GVA index; -1 means GVA not active for this variable (fall back to null). */
        + FOR(0, (int)nsave, [&](int k) {
              int gk = (k < (int)(sizeof _.op_arg_slot / sizeof *_.op_arg_slot)) ? _.op_arg_slot[k] : -1;
              int off_t = ab_save_off(nsave, k);       /* type word offset from rbp */
              int off_v = ab_save_off(nsave, k) + 8;  /* value word offset from rbp */
              if (gk >= 0) {
                  return x86("note", gva_name(gk))
                       + x86("mov", "rax", ABSQ(RT_GVA_VA + (unsigned long)gk * 16))
                       + x86("mov", RDQ("rbp", off_t), "rax")
                       + x86("mov", "rax", ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + 8))
                       + x86("mov", RDQ("rbp", off_v), "rax");
              } else {
                  /* GVA not active: save null DESCR */
                  return x86("mov", RDQ("rbp", off_t), (long)0)
                       + x86("mov", RDQ("rbp", off_v), (long)0);
              }
          })
        /* Null the save-set members in their GVA cells (formal slots = null; fname = null).
         * Caller already staged the actual args before the jmp; AB-3 installs them after this null.
         * AB-1: we null ALL (args not yet staged through the block), so tests will show null formals until AB-3. */
        + FOR(0, (int)nsave, [&](int k) {
              int gk = (k < (int)(sizeof _.op_arg_slot / sizeof *_.op_arg_slot)) ? _.op_arg_slot[k] : -1;
              if (gk >= 0) {
                  return x86("note", gva_name(gk))
                       + x86("mov", ABSQ(RT_GVA_VA + (unsigned long)gk * 16),     (long)0)
                       + x86("mov", ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + 8), (long)0);
              }
              return std::string();
          })
        /* &FNCLEVEL++ — AB-1: direct absolute store into the legacy kw_fnclevel cell (KW-COORD: after KW-1 this becomes KWQ(FNCLEVEL_IDX)) */
        + x86("mov", "rax", ABSQ((unsigned long)(uintptr_t)&kw_fnclevel))
        + x86("mov", "rax", RDQ("rax", 0))
        + x86("add", "rax", (long)1)
        + x86("mov", "rcx", ABSQ((unsigned long)(uintptr_t)&kw_fnclevel))
        + x86("mov", RDQ("rcx", 0), "rax")
        /* AB-1 BODY-JMP STUB: bomb — no fn_cell and no body-entry wire yet (AB-2/AB-3 fill these).
         * A premature entry surfaces here rather than silently jumping wild. */
        + x86("bomb", "IR_FUNC_ACTIVATE α: body-jmp not wired yet (AB-2 installs fn_cell + body entry; AB-3 wires call sites)")
        + x86_gamma()   /* γ = dead after the bomb; present for correct box structure */
        /* ===== β — RETURN LANDING STUB (AB-2 replaces with the real 3-way dispatch) ===== */
        + x86("comment", std::string("IR_FUNC_ACTIVATE β — ") + fname + " (AB-1 stub: bomb)")
        + x86_beta()
        + x86("bomb", "IR_FUNC_ACTIVATE β: return landing not wired yet (AB-2 installs real restore + 3-way dispatch)")
        + x86_omega()   /* ω = dead after the bomb */
        /* RO label for fname string (used by AB-2 monitor tap: lea rdi,[rip+fname_ro]) */
        + x86_ro_seal_str(0, fname);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
