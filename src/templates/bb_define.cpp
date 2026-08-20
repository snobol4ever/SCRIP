#include <string>
#include <cstdint>
#include <cstdio>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "ab_abi.h"
#include "pin_va.h"
#include "rt.h"
extern int64_t kw_fnclevel;
extern int g_monitor_bin;
/* RTX-FUNC-1 α-inline reach set.  Σ/Σlen come from emit.h; these five are the ones no header in this TU declares.  g_pl_trail is declared as a byte array deliberately — the template needs only its
 * ADDRESS plus the pinned `top` offset, never its layout, and no header exports pl_trail_t; resolution.c's _Static_assert is the guard that keeps PL_TRAIL_TOP_OFF honest across a struct change. */
extern int rt_g_want_name;
extern int rt_g_ret_by_name;
extern int * const rt_k_level_p;
extern char g_pl_trail[];
#define PL_TRAIL_TOP_OFF 32
void *rt_proc_get_fn(const char *name);   /* RTX-FUNC-0: binary body-jmp target — proc JIT fn pointer, registered by the driver proc loop before bb_ab_emit_nodes fires */
void mon_emit_call_bin(const char *fname);
void mon_emit_return_bin(const char *fname, DESCR_t retval);
const char *rt_define_query(const char *, int *, int *, int *, void **);
void rt_define_site(const char *, const char *, int, int, int, void *);
}
#include "x86_asm.h"
#define AB_TC_REG   "r8"
#define AB_TC_REG_D "r8d"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR_DEFINE — LADDER AB (2026-08-09 AB-2): per-DEFINE ACTIVATION BLOCK.                                                                                                                      */
/* SPITBOL manual Ch.8 pp.102-106: DEFINE'd function saves fname/formals/locals on a pushdown stack                                                                                                   */
/* at entry (here: an ___ frame) and restores them on RETURN/FRETURN/NRETURN.  Ch.16: &FNCLEVEL++                                                                                                     */
/* at call, -- at return.  ABI frozen in contracts/ab_abi.h (ONE AUTHORITY).                                                                                                                          */
/*                                                                                                                                                                                                    */
/* AB-2 DELIVERS:                                                                                                                                                                                     */
/*   α  = push ___; mov ___,rsp; sub rsp,K;                                                                                                                                                           */
/*        fill meta frame: wires (rcx/rdx from call site), entry-rsp, ANCHOR link/update;                                                                                                             */
/*        call rt_ab_enter_env(___) → Σ/wn/vtmark snapshot + k_level++;                                                                                                                              */
/*        save-set spill (GVA→frame); null GVA cells;                                                                                                                                                 */
/*        monitor call tap (g_monitor_bin guard + RO fname + mon_emit_call_bin);                                                                                                                       */
/*        jmp [fn_cell$<FN>] → lands at <FN>_act_α which IS here, after DEFINE store at AB-3.                                                                                                        */
/*        (body-jmp is the fn_cell indirect so AB-3 can flip the cell; static-direct fold at AB-5.)                                                                                                   */
/*   β  = 3-way on cl (AB_TC_RETURN/NRETURN/FRETURN):                                                                                                                                                */
/*        RETURN/NRETURN: result rax:rdx pre-restore (NRETURN: deref CALLER-SIDE post-restore);                                                                                                       */
/*        stash result → call rt_ab_leave_env(___, result, is_fail) → restore rax:rdx;                                                                                                               */
/*        restore save-set from frame; write &RTNTYPE;                                                                                                                                                */
/*        monitor return tap (g_monitor_bin guard + RO fname + mon_emit_return_bin(fname, result));                                                                                                   */
/*        unlink ANCHOR; LEAVE; jmp γ-wire (RETURN/NRETURN) or ω-wire (FRETURN).                                                                                                                     */
/*   fn_cell$<FN> = .data quad init'd to &rt_ab_undef_fn_stub (AB-3 stores &<FN>_act_α).                                                                                                            */
/*   Killswitch SCRIP_AB=0 → byte-identical legacy (lowerer gate; template unreached).                                                                                                               */
/*                                                                                                                                                                                                    */
/* DUAL ARM (anchor guard): if [RT_AB_ANCHOR]==0 on RETURN/FRETURN/NRETURN floater arrival,                                                                                                           */
/* fall through to legacy rt_flat_ret_snap path — so AB-2 and legacy calls coexist until AB-3.                                                                                                       */
/*                                                                                                                                                                                                    */
/* op_sval = fname (compile-time constant string)                                                                                                                                                     */
/* op_ival = nsave = 1 + nformals + nlocals                                                                                                                                                           */
/* op_arg_slot[k] = GVA index of save-set member k {fname, formal0..np-1, local0..nl-1}; -1 = no GVA */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define AB_FNCELL_MAX 1024   /* R-1 s94: 256 -> 1024.  The ONE allocator now also serves the TINY cross-chain cells alpha$<FN> / body$<ENTRY> (x86_jmp_via_cell): 3 cells per DEFINE, and 100func.sno alone has 100 DEFINEs */
static void * g_ab_fn_cells[AB_FNCELL_MAX];
static int    g_ab_fn_cell_n = 0;
static char   g_ab_fn_names[AB_FNCELL_MAX][64];
/* AB-3a: the ONE slot allocator, shared by the block template and the role-2 bind so bind-before-block ordering (main chain emits first, blocks post-chain) is immaterial — first request by fname allocates and initialises to the undef stub. */
static int bb_ab_slot_for(const char * fname) {
    for (int i = 0; i < g_ab_fn_cell_n; i++) if (!strncmp(g_ab_fn_names[i], fname, sizeof g_ab_fn_names[0] - 1)) return i;
    if (g_ab_fn_cell_n >= AB_FNCELL_MAX) { fprintf(stderr, "FATAL bb_ab_slot_for: cell table full (%d) at '%s' -- raise AB_FNCELL_MAX (R-1 s94: the old arm aliased slot 0 SILENTLY, the corruption class this abort replaces)\n", AB_FNCELL_MAX, fname); abort(); }
    int idx = g_ab_fn_cell_n++;
    snprintf(g_ab_fn_names[idx], sizeof g_ab_fn_names[idx], "%s", fname);
    g_ab_fn_cells[idx] = (void *)(uintptr_t)rt_ab_undef_fn_stub;
    return idx;
}
static void * bb_ab_cell_addr(const char * fname) { return (void *)&g_ab_fn_cells[bb_ab_slot_for(fname)]; }
extern "C" void * bb_ab_fn_cell_ptr(const char * fname) { return bb_ab_cell_addr(fname); }   /* AB-3b: non-static accessor for call-site template (bb_call_proc_staged.cpp) — same slot the block and bind use, ONE allocator */

extern "C" const char * bb_ab_sym_name(const char * nm) {   /* D-18a: asm_sym_name HOISTED from scrip.c:87 (driver static, unlinkable from the .so) — the driver's copy is now a wrapper on THIS one; same one-authority move as bb_ab_seal_entry_cells below. */
    static char b[256]; int j = 0;
    for (const char * c = nm ? nm : ""; *c && j < 250; c++) { unsigned char u = (unsigned char) *c;
        if ((u >= 'A' && u <= 'Z') || (u >= 'a' && u <= 'z') || (u >= '0' && u <= '9') || u == '_' || u == '$' || u == '.') b[j++] = (char) u;
        else j += snprintf(b + j, (size_t)(256 - j), "$%02X", u); }
    b[j] = 0; return b;
}
extern "C" void bb_ab_seal_entry_cells(const char * pname, void * fnbase, int alpha_face) {   /* D-18a (s161, HQ): m3_seal_entry_cells HOISTED VERBATIM from the driver (scrip.c) into the allocator's own home so the RUNTIME fragment compiler can seal its thunks' cells too — the driver cannot be called from libscrip_rt, and a second spelling in runtime_eval.c would be the s68/s70 spelled-twice disease on the exact function whose comment says "ONE allocator, one name".  The B1b witness (probe/b1/b1_eval_pattern_defer_call): eval_thunks_emit_from registered EXPR$ thunk procs and set their fns but never filled alpha$<thunk>, so every x86_jmp_via_cell call site inside an EVAL-compiled fragment jumped through rt_ab_undef_fn_stub — error 22 at match time, IN BOTH MODES — which is beauty's grammar-build wall (semantic.inc builds every rule pattern via EVAL). */
    extern int emit_label_lookup_offset(const char *);
    if (!pname || !fnbase) return;
    char lbl[300], cell[300];
    if (alpha_face) { snprintf(lbl, sizeof lbl, "%s_\xce\xb1", pname); snprintf(cell, sizeof cell, "alpha$%s", pname); }
    else            { snprintf(lbl, sizeof lbl, "LBL__%s",  bb_ab_sym_name(pname)); snprintf(cell, sizeof cell, "body$%s",  pname); }
    int off = emit_label_lookup_offset(lbl); if (off < 0) { if (getenv("SCRIP_SEAL_DIAG")) fprintf(stderr, "[SEAL] MISS lbl=%s cell=%s\n", lbl, cell); return; }   /* SCRIP_SEAL_DIAG=1: print the fill/miss ledger -- the instrument that convicted the unsealed alpha$EXPR$ cells (D-18) */
    *(void **)bb_ab_fn_cell_ptr(cell) = (void *)((char *)fnbase + off);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_define_activate() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    long   nsave    = (long)_.op_ival;
    long   nformals = (long)_.op_ab_nformals;   /* AB-3b: formals are save-set slots [1..nformals]; call site installs actuals there before jmp fn_cell; null loop must skip them */
    const char * fname = _.op_sval ? _.op_sval : "?";
    long   K     = ab_frame_k(nsave) + 8;   /* s53 PARITY: ab_frame_k is 8 mod 16 BY DESIGN for the old push-world (push 8 + K = 16-mult); with no push, +8 makes the carve a true 16-multiple so entry parity is PRESERVED and every interior C call sits correctly aligned.  All slot math below derives from K/BASE, so the shift is uniform. */
    long   BASE  = K - 8;   /* record base = rsp+BASE == entry−8, the old frame-pointer address bit-for-bit */
    /* ── fn_cell: allocate/record storage ── */
    void ** fn_cell_ptr = (void **)0;
    std::string fn_cell_lbl = std::string("fn_cell$") + fname;
    if (MEDIUM_BINARY) fn_cell_ptr = (void **)bb_ab_cell_addr(fname);   /* AB-3a: allocation moved into the ONE allocator (bb_ab_slot_for) so the role-2 bind and the block agree on the slot by fname regardless of emission order. */
    /* ── α ────────────────────────────────────────────────────────────────────────────────────── */
    std::string s =
        /* fn_cell .data (TEXT only; binary: runtime static array) */
        x86("directive", std::string(".section .data"))
      + x86("directive", std::string(".align 8"))
      + x86("directive", fn_cell_lbl + std::string(":"))
      + x86("directive", std::string(".quad rt_ab_undef_fn_stub"))
      + x86("directive", std::string(".section .text"))
      + x86("directive", std::string(".intel_syntax noprefix"))
        /* α label + RSP-only carve */
      + x86_alpha()
      + x86("sub",  "rsp", K)                 /* RSP-ONLY s53: α = sub rsp,K alone.  ab_frame_k gives K ≡ 8 mod 16, so entry(≡8)−K ≡ 0 — every interior C call 16-aligned with NO push.  The record base is the ADDRESS entry−8 == rsp+BASE (BASE=K−8), bit-identical to the old frame pointer; slots spelled rsp+BASE+off at α (rsp stable: align_enter is a no-op in this regime).  β re-adopts the base from ACT-ANCHOR into a scratch GPR. */
        /* store γ/ω wires (arrive in rcx/rdx from call site — lea contract) */
      + x86("mov", RDQ("rsp", BASE + AB_OFF_GW), "rcx")
      + x86("mov", RDQ("rsp", BASE + AB_OFF_WW), "rdx")
        /* entry rsp = rsp + K (this carve undone) */
      + x86("lea", "rax", RDQ("rsp", K))
      + x86("mov", RDQ("rsp", BASE + AB_OFF_ERSP), "rax")
        /* link ACT-ANCHOR: prev = [RT_AB_ANCHOR]; [RT_AB_ANCHOR] = base; store prev in frame */
      + x86("mov", "rax", ABSQ(RT_AB_ANCHOR))
      + x86("mov", RDQ("rsp", BASE + AB_OFF_ANCHOR), "rax")
      + x86("lea", "rdx", RDQ("rsp", BASE))   /* rdx free: ω wire already stored */
      + x86("mov", ABSQ(RT_AB_ANCHOR), "rdx")
        /* store β address in frame so the shared floater can reach it via anchor chain */
      + x86("lea", "rax", L(1))               /* LEA rip-relative to β label (L(1) string → XK_ILBL → x86_lea_rip_id) */
      + x86("mov", RDQ("rsp", BASE + AB_OFF_BADDR), "rax")
        /* RTX-FUNC-1: rt_ab_enter_env INLINED — the five operations of rt.c:511 emitted here, zero C crossings.  Order and semantics are byte-for-byte the C body's: Σ snapshot, Σlen snapshot,
         * wn snapshot-and-clear, vtmark = g_pl_trail.top, rt_k_level++ then kw_fnclevel = rt_k_level-1.  Scratch is rax (symbol address) + rcx (value) — strictly INSIDE the rax/rcx/rdx/rsi/rdi
         * set the C call already clobbered here, so no live value changes hands.  Every global is reached through x86("[rip@got + __]") = @GOTPCREL in TEXT / movabs in BINARY: five of the six are
         * GLOBAL/DEFAULT and dynsym-exported; rt_k_level was GLOBAL/HIDDEN and is promoted at rt.c:396 in this same commit because a hidden symbol cannot be linked from an m4 executable at all.
         * Each int→64-bit frame slot goes through a 32-bit load into the "e"-name (dword ptr, zero-extending) + movsxd reg,reg, reproducing the C's (int64_t) sign-extension EXACTLY.  ⛔ Do NOT
         * collapse that pair to x86("mov","rcx",RDD(...)): load32's TEXT arm prints its dst verbatim, so a 64-bit dst spells `mov rcx, dword ptr [..]` (gas size mismatch) while BINARY silently
         * encodes a correct 32-bit load — the both-medium divergence class, caught at source here rather than in the m4 build. */
      + x86("note", std::string("RTX-FUNC-1 inline enter_env"))
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&Σ, "Σ")
      + x86("mov", "rcx", RDQ("rax", 0))
      + x86("mov", RDQ("rsp", BASE + AB_OFF_SIGMA), "rcx")
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&Σlen, "Σlen")
      + x86("mov", "ecx", RDD("rax", 0))
      + x86("movsxd", "rcx", "ecx")
      + x86("mov", RDQ("rsp", BASE + AB_OFF_SIGMALEN), "rcx")
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_want_name, "rt_g_want_name")
      + x86("mov", "ecx", RDD("rax", 0))
      + x86("movsxd", "rcx", "ecx")
      + x86("mov", RDQ("rsp", BASE + AB_OFF_WN), "rcx")
      + x86("mov", RDD("rax", 0), (long)0)                     /* rt_g_want_name = 0 — the C clears it in the same breath it snapshots it */
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)g_pl_trail, "g_pl_trail")
      + x86("mov", "ecx", RDD("rax", PL_TRAIL_TOP_OFF))        /* rt_value_trail_mark() is { return g_pl_trail.top; } — resolution.c:31; offset pinned by that file's _Static_assert */
      + x86("movsxd", "rcx", "ecx")
      + x86("mov", RDQ("rsp", BASE + AB_OFF_VTMARK), "rcx")
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
      + x86("mov", "rax", RDQ("rax", 0))                       /* rt_k_level stays HIDDEN for the in-.so asm's PC32 reach — emitted code goes through the exported pointer (rt.c:396) */
      + x86("add", RDD("rax", 0), (long)1)                     /* rt_k_level++ */
      + x86("mov", "ecx", RDD("rax", 0))
      + x86("movsxd", "rcx", "ecx")
      + x86("sub", "rcx", (long)1)
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&kw_fnclevel, "kw_fnclevel")
      + x86("mov", RDQ("rax", 0), "rcx")                       /* kw_fnclevel = rt_k_level - 1 (int64_t cell) */
        /* save-set: spill each GVA cell into RBP-relative frame slot */
      + FOR(0, (int)nsave, [&](int k) -> std::string {
            int gk  = (k < (int)_.op_arg_slot_n) ? _.op_arg_slot[k] : -1;
            int ot  = ab_save_off(nsave, k);
            int ov  = ab_save_off(nsave, k) + 8;
            if (gk >= 0) {
                return x86("note", gva_name(gk))
                     + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, 0) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16))
                     + x86("mov", RDQ("rsp", (int)BASE + ot), "rax")
                     + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, 8) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + 8))
                     + x86("mov", RDQ("rsp", (int)BASE + ov), "rax");
            }
            return x86("mov", RDQ("rsp", (int)BASE + ot), (long)0)
                 + x86("mov", RDQ("rsp", (int)BASE + ov), (long)0);
        })
        /* null GVA cells: result cell (k=0) and locals (k>nformals) only.
         * Formals (k=1..nformals) are SKIPPED — the AB-3b call site installs actual arg values there
         * before jumping fn_cell$<FN>.  Pre-existing callers (non-AB path) reach this block through
         * fn_cell which still holds rt_ab_undef_fn_stub until DEFINE runs, so the non-AB path
         * never lands here; the ab_bind path writes INC_act_α and the call site installs args first.
         * ⛔ ALIASED FNAME — manual Ch.8 accumulator idiom, DEFINE('max(max,x)'), used throughout corpus/lib/math.sno:
         * when the function name is ALSO a formal, the result variable and that formal are ONE name and share ONE
         * GVA cell (Ch.19 DEFINE: the result is "a variable with the same name as the function").  The call site has
         * already installed the actual into that cell, and per the oracle that actual IS the value at entry and the
         * result when no assignment fires (a1(11) -> 11).  Nulling k=0 here destroys the argument.  Detect the
         * collision by GVA INDEX — names are resolved to cells by this point, so identity is an integer compare, not
         * a string one — and skip the null.  Restore is untouched: the spill loop above still saves the caller's
         * OUTER value out of that same cell, so the outer binding comes back on return exactly as before. */
      + FOR(0, (int)nsave, [&](int k) -> std::string {
            if (k >= 1 && k <= (int)nformals) return std::string();   /* AB-3b: skip formals; call site pre-installs actuals */
            int gk = (k < (int)_.op_arg_slot_n) ? _.op_arg_slot[k] : -1;
            if (k == 0 && gk >= 0) { for (int f = 1; f <= (int)nformals && f < (int)_.op_arg_slot_n; f++) if (_.op_arg_slot[f] == gk) return std::string(); }   /* fname aliased to a formal: cell holds the actual */
            if (gk >= 0) {
                return x86("note", gva_name(gk))
                     + x86("xor",  "eax", "eax")
                     + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, 0) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16),     "rax")
                     + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, 8) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + 8), "rax");
            }
            return std::string();
        })
        /* monitor CALL tap — g_monitor_bin ADDRESS both-medium (BINARY movabs / TEXT @GOTPCREL); fname from RO slot 0 */
      + x86_load_got("rax", "g_monitor_bin", (uint64_t)(uintptr_t)(void *)&g_monitor_bin)
      + x86("mov",    "rax", RDQ("rax", 0))    /* load int value; rax != 0 iff monitor active */
      + x86("test",   "rax", "rax")
      + x86("je",  L(2))             /* skip monitor tap */
      + x86_align_enter()
      + x86_ro_load_q("rdi", 0)     /* rdi = address of sealed fname string (RO slot 0) */
      + x86("call", "mon_emit_call_bin", (uint64_t)(uintptr_t)(void *)mon_emit_call_bin)
      + x86_align_leave()
      + x86("def", L(2))
        /* body-jmp: jmp FN__<FN>  (AB-3b/AB-5 static-direct fold: proc body label always known at emit time; s112 spelling).
         * fn_cell$FN holds &FN_act_α (written by DEFINE's AB-3a residual action) and is the CALL-SITE target —
         * using it here would be a self-loop (fn_cell → INC_act_α = this block).  Direct label is both media:
         * TEXT: jmp FN__INC (resolved by the assembler); BINARY: movabs rax, rt_proc_get_fn(fname); jmp rax
         * (the proc is already JIT-compiled before bb_ab_emit_nodes runs; the fn pointer is registered by the
         * driver's proc loop; cross-session x86_jmp_lblptr is NOT used — FN__INC lives in a different label
         * pool that was reset when the proc's emit_chain session ended). */
      + [&]() -> std::string {
            char blbl[128];
            snprintf(blbl, sizeof blbl, "FN__%s", fname);
            return x86("jmp_fn", blbl, (uint64_t)(uintptr_t)rt_proc_get_fn(fname));   /* medium-retire s170: the two arms were one transfer ("go to FN's body") written twice -- BINARY by the registered live address, TEXT by the label gas resolves.  Both coordinates go to the sealed encoder now; the not-registered bomb fallback moved there with the BINARY arm it belongs to. */
        }()
      + x86_gamma()   /* dead after jmp; present for box structure */
    /* ── β — 3-way dispatch on cl (AB_TYPECODE_REG) ──────────────────────────────────────────── */
      + x86("def", L(1))   /* β: L(1) */
        /* Save type-code into r8 immediately — r8 is dead at β entry (was argreg at call site only).
         * r8 survives all C calls and the LEAVE via the RTCC veneer; we read it after LEAVE for the final dispatch. */
      + x86("movzx", AB_TC_REG, "cl")    /* type code: 0=RETURN 1=NRETURN 2=FRETURN.  R10/R11-ERAD: r8 unconditionally.  r9 is RT_GVA_VA under RTCC_GLOBAL_R9_GVA and r10/r11 are the reserved wire pair rGamma/rOmega since D-1 made PASS-THRU the only blob linkage, so both prior spellings collided; r8 is veneer-preserved (RTCC slot 5, offset 40) exactly as r10 was, and the conditional fork is deleted so one authority serves both RTCC settings. */
        /* ADOPT THE FRAME (gdb conviction 2026-08-10): β arrives from the shared floater with the RETURNING STATEMENT's rbp, not this frame's — measured 0x88 below the anchor on the noarg repro.
         * Every access below (result stash, leave_env frame arg, save-set restore, GW/WW/prev loads, LEAVE) is rbp-relative, so without this adopt β works a FOREIGN frame: γ wire loaded 0 → jmp 0
         * (rip=0 crash), prev-anchor loaded dead stack garbage → anchor ← rt_ab_enter_env+107 (both gdb-measured).  The anchor is still linked here (unlink is below) and IS this frame's base. */
      + x86("mov", "rsi", ABSQ(RT_AB_ANCHOR))   /* RSP-ONLY s53: adopt the base into a SCRATCH GPR (rsi is dead at β), never a frame register; re-adopted after every C call from the still-linked anchor */
        /* FRETURN: result is irrelevant; skip stash; call leave_env(rbp, FAILDESCR, 1) */
      + x86("cmp", AB_TC_REG_D, (long)AB_TC_FRETURN)
      + x86("je",  L(3))
        /* RETURN / NRETURN: the RESULT is the CURRENT value of the fname GVA cell (save-set k=0), read BEFORE the restore loop puts the saved pre-call value back — manual Ch.8: the value of the
         * function is the value of the fname variable AT RETURN TIME.  rax:rdx at β entry are statement residue, NOT the result (gdb/output conviction 2026-08-10: stashing them returned null on
         * every RETURN micro and fed garbage descrs to leave_env on NRETURN → SIGABRT).  Plain ABSQ form deliberately — β's restore loop below is ABSQ-only; the movzx r9 above already conflicts
         * with RTCC_GLOBAL_R9_GVA's r9=GVA-base claim, flagged in the cursor, so no GVARQ here until that coordination is ruled. */
      + [&]() -> std::string { int gk0 = (0 < (int)_.op_arg_slot_n) ? _.op_arg_slot[0] : -1;
            if (gk0 < 0) return x86("xor", "eax", "eax") + x86("xor", "edx", "edx");
            return x86("note", gva_name(gk0))
                 + x86("mov", "rax", ABSQ(RT_GVA_VA + (unsigned long)gk0 * 16))
                 + x86("mov", "rdx", ABSQ(RT_GVA_VA + (unsigned long)gk0 * 16 + 8)); }()
      + x86("mov", RDQ("rsi", AB_OFF_RES0), "rax")
      + x86("mov", RDQ("rsi", AB_OFF_RES1), "rdx")
        /* RTX-FUNC-2: rt_ab_leave_env FAST PATH.  Two guards, BOTH provable no-op conditions read straight out of the C (ARCH §7 step 0(f-pre) — the shape whose falsifiability is knowable before the
         * asm exists), so the fast arm is not an approximation of the C, it is the C with the dead work removed:
         *   (a) g_pl_trail.top == [rbp+AB_OFF_VTMARK] ⇒ rt_value_trail_tidy_dead_window is a PROVEN no-op: its loop is `for (r = mark; r < top; r++)` (resolution.c) which never iterates when
         *       mark == top, and its only store is then `top = w = mark`, i.e. writing back the value already there.  Not "usually cheap" — structurally nothing.
         *   (b) rt_g_ret_by_name == 0 ⇒ rt_nret_fix (rt.c:755) collapses to `rt_g_want_name = wn; return r` — the by-name deref arm is unreachable.
         * Either guard failing takes the SLOW arm, which is the untouched C call, so the C body remains the fallback AND the bisection oracle exactly as ruling 3 requires.  Fast arm restores Σ/Σlen,
         * decrements k_level, republishes kw_fnclevel and rt_g_want_name, and hands back the stashed result in rax:rdx — the same post-conditions the call leaves behind.  r9 (type code) untouched. */
      + x86("note", std::string("RTX-FUNC-2 leave_env fast-path guards"))
      + x86("mov", "rcx", RDQ("rsi", AB_OFF_VTMARK))
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)g_pl_trail, "g_pl_trail")
      + x86("mov", "eax", RDD("rax", PL_TRAIL_TOP_OFF))
      + x86("movsxd", "rax", "eax")
      + x86("cmp", "rax", "rcx")
      + x86("jne", L(7))                                       /* trail grew inside this activation → real tidy owed → C */
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_ret_by_name, "rt_g_ret_by_name")
      + x86("mov", "eax", RDD("rax", 0))
      + x86("cmp", "eax", (long)0)
      + x86("jne", L(7))                                       /* a by-name return is pending → nret_fix has real work → C */
        /* ── FAST ARM ── */
      + x86("mov", "rcx", RDQ("rsi", AB_OFF_SIGMA))
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&Σ, "Σ")
      + x86("mov", RDQ("rax", 0), "rcx")
      + x86("mov", "rcx", RDQ("rsi", AB_OFF_SIGMALEN))
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&Σlen, "Σlen")
      + x86("mov", RDD("rax", 0), "ecx")                       /* Σlen is int — dword store, matching the C's assignment width */
      + x86("mov", "rcx", RDQ("rsi", AB_OFF_WN))
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_want_name, "rt_g_want_name")
      + x86("mov", RDD("rax", 0), "ecx")                       /* rt_nret_fix's tail: rt_g_want_name = wn */
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
      + x86("mov", "rax", RDQ("rax", 0))
      + x86("mov", "ecx", RDD("rax", 0))
      + x86("movsxd", "rcx", "ecx")
      + x86("sub", "rcx", (long)1)                             /* rcx = new k_level; ecx aliases its low half, so the store below writes the decremented value */
      + x86("mov", RDD("rax", 0), "ecx")                       /* rt_k_level-- .  ⛔ NOT x86("sub", RDD(..), 1): "sub" has NO XK_REGDISP32/XK_IMM dispatch arm and returns the EMPTY STRING — the
                                                               * decrement vanished from both media and &FNCLEVEL climbed 0,1,2,3 instead of unwinding (caught by the witness, not by the benchmarks,
                                                               * which stayed oracle-exact throughout).  Third instance of this class in this file's history after x86("leave") and x86("mov",RDD,imm);
                                                               * "add" happens to have the arm, which is exactly why the α side above works and this one did not. */
      + x86("sub", "rcx", (long)1)
      + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&kw_fnclevel, "kw_fnclevel")
      + x86("mov", RDQ("rax", 0), "rcx")
      + x86("mov", "rax", RDQ("rsi", AB_OFF_RES0))             /* result passes through unchanged — nret_fix is identity under guard (b) */
      + x86("mov", "rdx", RDQ("rsi", AB_OFF_RES1))
      + x86("jmp", L(8))
        /* ── SLOW ARM — the untouched C crossing ── */
      + x86("def", L(7))
      + x86_align_enter()
      + x86("mov", "rdi", "rsi")                     /* arg1 = base (read BEFORE rsi is repurposed) */
      + x86("mov", "rsi", RDQ("rdi", AB_OFF_RES0))   /* result.v */
      + x86("mov", "rdx", RDQ("rdi", AB_OFF_RES1))   /* result.i */
      + x86("xor", "ecx", "ecx")                     /* is_fail=0 */
      + x86("call", "rt_ab_leave_env", (uint64_t)(uintptr_t)(void *)rt_ab_leave_env)
      + x86_align_leave()
      + x86("mov", "rsi", ABSQ(RT_AB_ANCHOR))        /* re-adopt: anchor is still linked (unlink is below) */
      + x86("def", L(8))
        /* rax:rdx = nret-fixed result from leave_env */
      + x86("mov", RDQ("rsi", AB_OFF_RES0), "rax")   /* re-stash: monitor tap reads from frame */
      + x86("mov", RDQ("rsi", AB_OFF_RES1), "rdx")
      + x86("jmp", L(4))
      + x86("def", L(3))             /* FRETURN path */
      + x86_align_enter()
      + x86("mov", "rdi", "rsi")
      + x86("xor", "esi", "esi")     /* result.v = 0 */
      + x86("xor", "edx", "edx")     /* result.i = 0 */
      + x86("mov", "ecx", (long)1)   /* is_fail=1 */
      + x86("call", "rt_ab_leave_env", (uint64_t)(uintptr_t)(void *)rt_ab_leave_env)
      + x86_align_leave()
      + x86("mov", "rsi", ABSQ(RT_AB_ANCHOR))        /* re-adopt after the C call */
      + x86("mov", RDQ("rsi", AB_OFF_RES0), "rax")   /* stash FAILDESCR for monitor */
      + x86("mov", RDQ("rsi", AB_OFF_RES1), "rdx")
      + x86("def", L(4))             /* common β tail */
        /* restore save-set: GVA cells ← frame (rcx is scratch; rax:rdx stashed in frame) */
      + FOR(0, (int)nsave, [&](int k) -> std::string {
            int gk  = (k < (int)_.op_arg_slot_n) ? _.op_arg_slot[k] : -1;
            int ot  = ab_save_off(nsave, k);
            int ov  = ab_save_off(nsave, k) + 8;
            if (gk >= 0) {
                return x86("note", gva_name(gk))
                     + x86("mov", "rcx", RDQ("rsi", (int)0 + ot))
                     + x86("mov", ABSQ(RT_GVA_VA + (unsigned long)gk * 16),     "rcx")
                     + x86("mov", "rcx", RDQ("rsi", (int)0 + ov))
                     + x86("mov", ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + 8), "rcx");
            }
            return std::string();
        })
        /* load γ/ω wires and prev ACT-ANCHOR from frame BEFORE LEAVE tears it down */
      + x86("mov", "r10", RDQ("rsi", AB_OFF_GW))     /* γ wire */
      + x86("mov", "r11", RDQ("rsi", AB_OFF_WW))     /* ω wire */
      + x86("mov", "rcx", RDQ("rsi", AB_OFF_ANCHOR)) /* prev ACT-ANCHOR */
        /* monitor RETURN tap — frame still live; result is in frame RES0/RES1; address both-medium per the CALL tap */
      + x86_load_got("rax", "g_monitor_bin", (uint64_t)(uintptr_t)(void *)&g_monitor_bin)
      + x86("mov",    "rax", RDQ("rax", 0))
      + x86("test",   "rax", "rax")
      + x86("je",  L(5))
      + x86_align_enter()
      + x86_ro_load_q("rdi", 0)                       /* rdi = sealed fname ptr */
      + x86("mov", "rsi", RDQ("rsi", AB_OFF_RES0))   /* result.v */
      + x86("mov", "rdx", RDQ("rsi", AB_OFF_RES1))   /* result.i */
      + x86("call", "mon_emit_return_bin", (uint64_t)(uintptr_t)(void *)mon_emit_return_bin)
      + x86_align_leave()
      + x86("def", L(5))
      + x86("mov", "rsi", ABSQ(RT_AB_ANCHOR))        /* re-adopt: the monitor tap used rsi as arg2 and C clobbered rcx */
        /* restore result to rax:rdx */
      + x86("mov", "rax", RDQ("rsi", AB_OFF_RES0))
      + x86("mov", "rdx", RDQ("rsi", AB_OFF_RES1))
      + x86("mov", "rcx", RDQ("rsi", AB_OFF_ANCHOR)) /* prev, re-read through the base */
        /* unlink ACT-ANCHOR: [RT_AB_ANCHOR] ← rcx (prev) */
      + x86("mov", ABSQ(RT_AB_ANCHOR), "rcx")
        /* LEAVE — restores rsp = rbp, pops rbp; frame is gone after this.  Spelled as the mov/pop PAIR, not x86("leave"): there is NO "leave" arm in
         * x86_asm.h's dispatch, so x86("leave") emitted NOTHING in BOTH media — the callee frame was never torn down.  Neither encoder here touches
         * flags (as leave does not) and both are proven in-tree (x86_srf_floater; bb_glue_framed).  Adding a real "leave" encoder is the 1-byte form
         * but x86_asm.h is NOT-CONCURRENCY-SAFE — Lon routes that seat; this pair is byte-equivalent in effect and stays inside the template. */
      + x86("mov", "rsp", RDQ("rsi", AB_OFF_ERSP))   /* RSP-ONLY absolute unwind: ERSP slot = entry rsp, stored at α — one mov, no frame register, exact ω of the α carve at ANY foreign depth */
        /* dispatch on r9 (type-code saved before any C call, survives LEAVE):
         *   RETURN / NRETURN (r9 != 2) → γ wire in r10
         *   FRETURN          (r9 == 2) → ω wire in r11 */
      + x86("cmp", AB_TC_REG_D, (long)AB_TC_FRETURN)
      + x86("je",  L(6))
      + x86("jmp", "r10")            /* RETURN / NRETURN → γ */
      + x86("def", L(6))
      + x86("jmp", "r11")            /* FRETURN → ω */
      + x86_deflabel(X86P_GAMMA)     /* AB-3a FINDING-FIX (pre-existing, measured this session): x86_gamma() above REFERENCES <FN>_act_γ but nothing DEFINED it — every DEFINE-bearing program failed to LINK in m4 at HEAD (undefined reference, ld exit 1; pristine denominator identical), invisible to the matrix (no matrix probe carries a DEFINE) and to the AB gates as run.  Dead structural landing like ω below; AB-3b's site wires are the real continuations. */
      + x86_deflabel(X86P_OMEGA)     /* ω port label define only — never reached; dispatch above is exhaustive */
                /* RO fname string slot 0 — used by both monitor taps */
      + x86_ro_seal_str(0, fname);
    /* mode-3 fn_cell residual store */
    if (fn_cell_ptr) {   /* medium-retire s170: the MEDIUM_BINARY conjunct was already redundant -- fn_cell_ptr is assigned in the BINARY arm above and nowhere else, so it is non-null in BINARY only. */
        *fn_cell_ptr = (void *)(uintptr_t)rt_ab_undef_fn_stub;
    }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* bb_ab_emit_nodes — post-main-chain sweep: emit one IR_DEFINE block per DEFINE fold.                                                                                                         */
/* Called from scrip.c after both m3 and m4 main-chain emission.  Saves/restores g_emit and related                                                                                                   */
/* globals so the sweep is side-effect-free from the caller's perspective.                                                                                                                             */
static std::string bb_define_bind() {
    /* ⭐⭐⭐ DEFINE-SITE s57 (Lon in-chat: "the code for DEFINE comes directly after the statement comment ... move it there. This is shared code"): this box IS the DEFINE statement's emitted body in the
     * ONE shared chain — the constant-folded registration executes exactly where SPITBOL says DEFINE executes (manual Ch.8: "the DEFINE function must be executed for the definition to occur"), replacing
     * the m4 startup hoist (scrip.c per-proc emit_textf loop, now skipped for dyn_scope) and the bare skip.  ONE crossing: rt_define_site(name, params_csv, np, nf, fb, fn) — idempotent for the m3 case
     * (registry already populated at compile; fn==same → refresh, NO redefined poison) and the full registration for the m4 executable's first pass; a genuine re-DEFINE (different fn) sets redefined per
     * existing semantics.  Emit-time data comes from rt_define_query(fname) — the in-process registry is populated in BOTH modes before main-chain emission (driver register loops precede emit_chain), and
     * in m4 the proc blobs (which set frame_bytes/fn in-process) are emitted before the main chain, so the baked constants are final.  fn operand: named-symbol lea `[rip + FN__<FN>]` in TEXT (assembler
     * resolves); in BINARY x86_load_ro's movabs arm bakes the ptr — the real slab fn in m3 (set before main emit), and the leaf's fn==existing check makes it a refresh either way.  R10 sanctioned RO-load
     * divergence; ONE body, no medium branch on the registration half. */
    /* AB-3a (retained, SCRIP_AB=1 only): the fn_cell$<FN> <- &<FN>_act_α store — the α label is fname-derived (bb_ab_emit_nodes lbl override), TEXT rip-lea + GOT cell, BINARY neutralized to a relay (the
     * C-store at block-emit time IS the bind; movabs cannot forward-patch).  K=0 transparent spine box; trailer = the bb_goto relay shape (alpha + pair_loop). */
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    const char * fname = _.op_sval ? _.op_sval : "?";
    std::string albl = std::string(fname) + "_act_\xce\xb1";
    std::string clbl = std::string("fn_cell$") + fname;
    static int _ab = -1; if (_ab < 0) { const char * _e = getenv("SCRIP_AB"); _ab = (_e && *_e == '1') ? 1 : 0; }
    int _np = 0, _nf = 0, _fb = 0; void * _fn = 0; const char * _csv = rt_define_query(fname, &_np, &_nf, &_fb, &_fn);
    uint64_t _site_fp; { void (*fp)(const char *, const char *, int, int, int, void *) = rt_define_site; _site_fp = (uint64_t)(uintptr_t)(void *)fp; }
    std::string blbl = _.lbl_t0 ? std::string(_.lbl_t0) : (std::string("FN__") + fname);   /* NATURAL-LABEL (Lon s114 in-chat: "you can not register these FUNCTIONS at the beginning of the program, it must happen at the DEFINE"; supersedes the s112 FN__ face): the fn registered by rt_define_site is the body-entry statement's NATURAL port label (n<uid>_statement_begin_α, deposited into lbl_t0 by the bind dispatch from the dentry stamp) — the pre-main FN__ relay face is deleted with the stub blob.  Fallback spelling serves only the m3/BINARY path, where the name operand is unused (movabs bakes _fn from the registry). */
    std::string reg = x86("comment", "DEFINE-SITE s57: constant-folded registration AT the statement (shared chain)")
         + x86_ro_load_q("rdi", 0)
         + x86_ro_load_q("rsi", 1)
         + x86("mov32", "edx", (long)_np)
         + x86("mov32", "ecx", (long)_nf)
         + x86("mov32", "r8d", (long)_fb)
         + x86("lea", "r9", std::string("[rip + __]"), (uint64_t)(uintptr_t)_fn, blbl.c_str())
         + x86_scan_sync_out()
         + x86("call", "rt_define_site", _site_fp)
         + x86_scan_sync_in_rr();
    std::string seals = x86_ro_seal_str(0, fname) + x86_ro_seal_str(1, _csv ? _csv : "");
    if (!_ab) return x86_alpha() + reg + x86_pair_loop() + seals;
    /* RTX-FUNC-0 BIND-NEUTRALIZE (BINARY): the lea's [rip + __] 5-arg form routes to x86_load_ro which in
     * BINARY bakes the ptr argument (0 here) as a movabs immediate — there is NO forward-patch for it, so at
     * DEFINE-time this store wrote 0 over the correct address the bb_ab_emit_nodes posthook C-store had
     * already placed (measured: cell probe good, then jmp 0 at first call).  BINARY bind is now a transparent
     * relay — the C-store at block-emit time IS the bind.  TEXT keeps the runtime store: gas resolves
     * <FN>_act_α at assembly time and the fn_cell$<FN> .data slot via GOT, both correct as-is. */
    if (MEDIUM_BINARY) return x86_alpha() + reg + x86_pair_loop() + seals;
    return x86_alpha()
         + reg
         + x86("lea", "rax", std::string("[rip + __]"), (uint64_t)0, albl.c_str())   /* α address by NAME (TEXT): renders [rip + <FN>_act_α], resolved by the assembler.  The bare 3-arg string form is SILENTLY SWALLOWED by the encoder (measured: both leas vanished from the .s, r11 garbage, wild store, segv) — the 5-arg __ form is the sanctioned named-symbol spelling. */
         + x86("mov", "r11", std::string("[rip@got + __]"), (uint64_t)0, clbl.c_str())   /* TEXT cell = &fn_cell$<FN> via the GOT load, the 145-line precedent verbatim */
         + x86("mov", RDQ("r11", 0), "rax")
         + x86_pair_loop()
         + seals;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_ab_emit_nodes(IR_graph_t *g, int gva_active)
{
    if (!g || g->ab_n <= 0) return;
    extern int g_gva_active;
    extern IR_graph_t *g_emit_cfg;
    extern int gva_index_of(const char *);
    sm_emit_t saved_emit   = g_emit;
    IR_graph_t *saved_cfg  = g_emit_cfg;
    int         saved_gva  = g_gva_active;
    g_gva_active = gva_active;
    /* Local storage for per-function label name strings — kept alive until bb_emit_x86 consumes them */
    char ab_lbl_α[128], ab_lbl_β[128], ab_lbl_γ[128], ab_lbl_ω[128];
    for (int i = 0; i < g->ab_n; i++) {
        IR_t *nd = g->ab_nodes[i];
        if (!nd || nd->op != IR_DEFINE) continue;
        g_emit         = saved_emit;
        g_emit_cfg     = g;
        g_emit.op_sval = IR_LIT(nd).sval;
        g_emit.op_ival = (long)nd->n_operands;
        g_emit.op_ab_nformals = nd->seal;   /* RTX-FUNC bug#2 fix: dispatch emit.cpp:1008 is the ONLY other setter and role-1 blocks never reach it ("rides ab_nodes[], never reaches this dispatch in-chain") -- without this copy the null loop reads nformals=0 and wipes formal N (two-probe conviction: lower seal=1, template 0) */
        g_emit.op_beta_dead = 0;   /* β has inbound edges from the floaters */
        /* Set unique per-function label names (ROMAN_act_α / _β / _γ / _ω) */
        const char *fn = g_emit.op_sval ? g_emit.op_sval : "unknown";
        snprintf(ab_lbl_α, sizeof ab_lbl_α, "%s_act_\xce\xb1", fn);
        snprintf(ab_lbl_β, sizeof ab_lbl_β, "%s_act_\xce\xb2", fn);
        snprintf(ab_lbl_γ, sizeof ab_lbl_γ, "%s_act_\xce\xb3", fn);
        snprintf(ab_lbl_ω, sizeof ab_lbl_ω, "%s_act_\xcf\x89", fn);
        g_emit.lbl_α = ab_lbl_α;
        g_emit.lbl_β = ab_lbl_β;
        g_emit.lbl_γ = ab_lbl_γ;
        g_emit.lbl_ω = ab_lbl_ω;
        /* RTX-FUNC-0: BINARY port defines/jumps route through the POINTER fields (x86_portlbl), not the name
         * strings — stale saved_emit pointers meant the α define landed on the previous node's label and
         * INC_act_α was interned-but-never-defined (bind-fix probe: def=0 off=-1).  Intern by the same names
         * so D-records and the bind-fix agree on ONE bb_label_t per port. */
        g_emit.lbl_α_p = emit_label_intern(ab_lbl_α);
        g_emit.lbl_β_p = emit_label_intern(ab_lbl_β);
        g_emit.lbl_γ_p = emit_label_intern(ab_lbl_γ);
        g_emit.lbl_ω_p = emit_label_intern(ab_lbl_ω);
        /* 599601e (PT-2 Defect B) heap-backed op_arg_slot (int*+cap/n, emit.h:586) and migrated the in-chain fill
         * sites but missed this one: the fixed-array sizeof idiom silently became 8/4=2 AND the posthook snapshot
         * restore (g_emit = saved_emit) carries op_arg_slot as NULL -> store at k=0 SEGV'd every SCRIP_AB=1 run at
         * EMIT time (gdb: rdx=0 at :324; dark under the default-OFF gate).  Reserve per node AFTER the restore. */
        int _ab_n = (int)nd->n_operands;
        drive_arg_slots_reserve(_ab_n);
        for (int k = 0; k < _ab_n; k++) {
            const char *nm = nd->operands[k] ? IR_LIT(nd->operands[k]).sval : (const char *)0;
            g_emit.op_arg_slot[k] = (nm && gva_active) ? gva_index_of(nm) : -1;
        }
        g_emit.op_arg_slot_n = _ab_n;
        bb_emit_x86(bb_define_activate());
        /* RTX-FUNC-0 BIND-FIX: in BINARY (m3), x86_load_ro baked 0 as the forward-reference placeholder for
         * [rip + INC_act_α] in bb_define_bind() — movabs is an immediate, not a patchable rel32, so the label
         * address was never resolved.  The label pool persists for the whole emitter session (one bb_emit_begin
         * per emit_chain call), so INC_act_α is now defined (just emitted above).  Look it up and write the
         * absolute JIT address (bb_emit_buf + offset) directly into the fn_cell.  The runtime bind still runs
         * (its MOV stores this same address again) — idempotent and harmless.  TEXT (m4): bind resolves via
         * gas/ld at link time; no C-side store needed.  ONE AUTHORITY: bb_ab_cell_addr is the allocator. */
        if (MEDIUM_BINARY) {
            extern bb_buf_t bb_emit_buf;
            bb_label_t *al = emit_label_intern(ab_lbl_α);
            void **cell = (void **)bb_ab_cell_addr(fn);
            if (al && bb_label_defined(al) && cell)
                *cell = (void *)(bb_emit_buf + al->offset);
        }
    }
    g_emit     = saved_emit;
    g_emit_cfg = saved_cfg;
    g_gva_active = saved_gva;
}
#include <string>
#include <cstdint>
#include "emit.h"
extern "C" int bb_tiny_shim_ok(const char *, int);   /* s59 ONE-AUTHORITY: shim emits iff the shared site predicate (bb_call_proc_staged.cpp) says so — a jmp <fn>_alpha can never dangle */
static int fnrbp(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_FN_RBP"); v = e ? atoi(e) : 2; if (v == 1) v = 2; if (v < 0 || v > 2) v = 2; } return v; }   /* ⭐⭐⭐ S-2 (RBP-EARN s78, Lon in-chat: "remove RBP's used by FUNCTION linkage"): DEFAULT FLIPPED 1 -> 2 and the RBP-FUNCTION arm RETIRED -- `=1` now ALIASES to 2 so an inherited env or script cannot resurrect a deleted regime.  THE GATE s63/s64 SET WAS "when the =2 board == the =1 board, flip and delete", and S-1 (FLOATER-CUT, this session) is what closed it: the =2 arm was 11 programs short ONLY because every DEFINE-bearing statement leaked its goto-out depth and the RBP arm's `mov rsp,rbp` was silently absorbing it -- measured 136 -> 146 PASS on the 165-program two-arm sweep once the statement released its own K.  RBP's remaining job is the MATCH INTERIOR alone (ARBNO zeta-FRAMEs + the mrbp match frame); FUNCTION linkage now rides the RSP depth-invariance law exactly as the s64 writer/reader pair always claimed it could.  `=0` (the s58 BOMB descent instrument) is UNTOUCHED and still reachable. */
static int fnsig(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_FN_SIG"); v = (e && *e == '0') ? 0 : 1; } return v; }   /* SIG s66: MUST agree with bcps_fnsig (bb_call_proc_staged.cpp) — same env read, same default, two coherent worlds never mixed */   /* s63/s64 FUNCTION-linkage arms: 0 = s58 BOMB floaters (descent instrument) · 1 = RBP bracket (s63, proven) · 2 = RSP-ONLY (s64, Lon's challenge): the shim pushes the 16B {γ,ω} pair at TOS and the floaters find it by DEPTH-INVARIANCE — no anchor register; sound iff every statement boundary in the body is depth-neutral, so under =2 a leaking statement shape pops junk as a code address and dies LOUD at the jmp: the failure list IS the leak census, program-granular.  One predicate gates writer + readers, drift-proof. */
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "ab_abi.h"
#include "pin_va.h"
long  rt_proc_call_open_slim(const char *name, int np, int nargs);
int   bb_scc_probe(const char *fname, int nargs, int *np_out, int *nsave_out, int *gk_out, int *res_gk_out);
int   bb_scc_handoff_arm(void);
int   bb_scc_handoff_room(void);
int   rt_proc_nformals(const char *);
void  bb_scc_handoff_pending_clear(void);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR_DEFINE — SN4-FLAT-PROC (s176) functional linkage family.  SPITBOL manual Ch.8: a DEFINE'd function saves the fname variable, formals, and locals on a pushdown stack at entry and restores
 * them when the body transfers to the reserved labels RETURN (yield fname's value), FRETURN (signal failure), or NRETURN (Ch.9: yield the NAME held in fname).  In the flat regime the body statements
 * live ONLY in the one main chain, so the return labels are program-wide FLOATER boxes and the save/restore protocol rides the pcall record; roles discriminate on op_ival:
 *   3 WIRE-ADOPT — first box of a DEFINE stub blob, runs right after the jmp-entry prologue: ___ = this activation's base, the prologue parked outside-γ at [___+kt-24], outside-ω at [___+kt-16],
 *     caller ___ at [___+kt-8] (xa_flat header contract), and blob-entry rsp = ___+kt (pre-carve).  Marshal all four into rt_flat_wire_adopt, which writes the OPEN pcall record's wire quad — the way
 *     home from any depth.  γ continues to the IR_GOTO_DEFERRED entry transfer.
 *   1 RETURN floater / 2 FRETURN floater — reached as ordinary labels through the registry (any goto, incl. $-computed).  rt_flat_ret_snap PEEKS (never pops) the open record's wire quad into a static
 *     buffer {γ-wire@0, ω-wire@8, rsp@16, ___@24} and returns its address; the tail restores ___/rsp and jmps the port's wire with rax:rdx riding untouched, landing exactly on the wire the opener
 *     installed (rt_proc_enter's landings on the C path) — the landing's epilogue leaf then performs the pop + name-restore + result protocol VERBATIM, so semantics are byte-identical to the old
 *     extracted-body exit.  Discarded intermediate C frames (rt_chain_enter / rt_goto_transfer) hold only callee-saved pins that are global invariants in emitted code, so the wholesale rsp restore is
 *     sound.  Level-0 transfer and wire-less activations die loudly inside the leaf.
 *   0 site OPEN — reserved for the emitted flat call site (next slice); bombs if reached. */
static std::string bb_define_sr() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    long role = (long)_.op_ival;
    int inl5 = (role == 5) ? 1 : 0; if (inl5) role = 4;   /* SHIM-AT-STATEMENT (Lon s114 in-chat: "Move all the SHIM code emission directly after the spot where the DEFINE statement is"): role 5 = the role-4 shim emitted INLINE by the bind dispatch at the DEFINE statement — no box ports (the bind owns the node), skip-wrapped so inline flow jumps over it (entered only by name at <FN>_α), and the body transfer targets _.lbl_t0 VERBATIM (the natural n<uid>_statement_begin_α deposited from the dentry stamp) instead of LBL__<entry>.  m4 TEXT only; the m3 stub-blob role-4 arm is byte-identical. */
    if (role == 3) {
        /* ⛔⭐⭐⭐ LON RULING s54 (in-chat, verbatim substance): "delete all of that global variables. GONE. DONE.
         * We do not do that here. We have registers R10 and R11 for linkage, and plan on using the stack to
         * store things, not global variables."  The rt_flat_wire_adopt call — which banked the wire quad into
         * the g_pcall global array — is DELETED.  Wires ride rΓ=r10 · rΩ=r11 (LADDER WREG assignment, locked
         * s12).  Entry-rsp banking moves to the SPINE (stack-resident record), not a parallel global — that
         * half is the next seat's WREG-3 shape; until it lands, RETURN-from-interior-growth is KNOWN BROKEN
         * (Lon: "worry not about breakage").  git revert is the undo. */
        /* ⭐ FUNCTION LINKAGE s55 (Lon in-chat): "Can you not use R10 and R11 for success and fail return address
         * for FUNCTION linkage?  Just like any BB BLOB interface."  YES — the call sites now speak r10/r11 DIRECTLY
         * (bb_glue_pass_wires_blob), so the rcx/rdx→r10/r11 adopt hop is DELETED.  This box emits nothing: the wire
         * pair arrives already seated.  rcx/rdx remain the chain contract (rt_chain_enter, EVAL/CODE) only. */
        /* ⭐⭐⭐ SHIM DELETED s58 (Lon in-chat: "Remove stupid TEST_shim and make it real. Do not use g_call_args, instead push on the stack via RSP.").  The <FN>_shim and its g_call_args transport
         * are GONE: tiny sites (bb_call_proc_staged TINY-REAL arm) now carry the whole going-in protocol themselves — save-set spill to an RSP pushdown block, direct actual→formal GVA install,
         * locals/result NULVCL, wires, one jmp to the body α.  This stub blob remains only for the goto-transfer customers (EVAL/CODE/$X) flowing α→γ into IR_GOTO_DEFERRED. */
        return x86("comment", "IR_DEFINE wire-adopt (s58: EMPTY — shim deleted, tiny sites carry the pushdown protocol; s55: sites set r10/r11 directly)")
             + x86_alpha()
             + x86_gamma();
    }
    if (role == 4) {
        /* ⭐⭐⭐ TINY-REAL s58 SHIM (Lon in-chat: "Remove stupid TEST_shim and make it real... push on the stack via RSP"; "the SHIM... swaps all the globals with actuals, and save the globals in those
         * same stack locations"; "push val, push val, then push 2 if two args are passed. And the shim handles the 4 parameters from the 2 it was given and extends the stack in place").  Emitted ONCE
         * per DEFINE, AT the DEFINE statement (role-4 IR_DEFINE spliced by the lowerer; unreachable inline — the statement's goto skips it; entered only by name).  THE PROTOCOL — site pushes
         * {K}{succ,fail conts}{actual_i at [32+i*16]} and jmps <fn>_alpha with r10/r11 UNTOUCHED (they are the ENCLOSING activation's return ports and ride the record).  alpha: read K, extend the
         * block in place to full arity (max carve + give back K*16 — no shl encoder needed), spill+null locals/result into its own tail, bank the enclosing wires + a K copy, then per formal i ONE
         * cmp/jbe: supplied ⇒ SWAP actual↔old-global in the site's own slot (manual Ch.8 p.104 pushdown), missing ⇒ old-global into its extension slot + formal←null; set r10=<fn>_beta r11=<fn>_omega
         * (the floaters stay bare jmp r10/r11) and jmp the body α.  beta: capture the result from the result-name cell BEFORE restoring, mirror-restore every cell, re-establish the enclosing wires
         * (recursion is LIFO by construction — this is what kills the measured return-ladder), fetch the site cont, release the CONSTANT frame (T+32+nf*16, since D+K=nf), jmp cont.  omega: same with
         * FAILDESCR.  Record from alpha's post-carve rsp: [0..extra*16) locals+result olds | [extra*16]={r10,r11} | [+16]={K} || [T..T+D*16) missing-formal olds | {K}{conts}{K supplied actuals}.
         * Addressing base r8 = extension base (= site rsp − nf*16): missing_i=[r8+i*16] (valid iff i>=K), count=[r8+nf*16], conts=[+16,+24], supplied_i=[r8+nf*16+32+i*16] — every disp positive+static.
         * Over-arity (K>nf, manual: extras evaluated+ignored) stays on the classic arm this rung — owed.  TEXT-only this seat (m3 cross-chain body-α, same owed class as fold slice-2).  Hatch
         * SCRIP_NO_TINY=1 empties this box (sites fall to slim which never jmps here). */
        const char * fn4 = _.op_sval; const char * en4 = _.lbl_t0 ? _.lbl_t0 : fn4;
        int np4 = 0, ns4 = 0, rg4 = -1; int gk4[64];
        int ok4 = (fn4 && en4 && bb_tiny_shim_ok(fn4, 0)) ? bb_scc_probe(fn4, 0, &np4, &ns4, gk4, &rg4) : 0;   /* R-1 s94 (Fable 5): BOTH MEDIA -- the MEDIUM_TEXT conjunct is lifted; the shim's faces are same-chain extlbl, its body transfer rides the body$<ENTRY> cell (x86_jmp_via_cell) */
        int nf4 = ok4 ? rt_proc_nformals(fn4) : 0;
        if (!(ok4 && nf4 >= 0 && nf4 <= np4 && nf4 <= 29)) return inl5 ? x86("comment", "role 5: shim declined inline (hatch or probe/formals shape) — sites fall to the slim arm") :   /* nf<=29: L-id budget (one-byte ids, [0,250)); wider DEFINEs decline to slim */ (x86("comment", "IR_DEFINE role 4: shim declined (hatch, non-TEXT, or probe/formals shape) — sites fall to the slim arm") + x86_alpha() + x86_gamma());
        int xt4 = ns4 - nf4;   /* extra = locals + unshadowed result name (probe layout: gk[0..np)=formals then locals, gk[np..ns)=result iff unshadowed) */
        long T4 = 16L * xt4 + 32;   /* tail: extras' olds + {r10,r11} + {K,spare} */
        int rgx = rg4 < 0 ? 0 : rg4;
        auto GQ = [&](int gk, int w) { return (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, w) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + (unsigned long)w); };
        auto R8Q = [&](long d) { return std::string("[r8 + ") + std::to_string(d) + "]"; };
        std::string la = std::string(fn4) + "_\xce\xb1", lb = std::string(fn4) + "_\xce\xb3", lo = std::string(fn4) + "_\xcf\x89";   /* s58 Lon: the RETURN landing is the activation-blob GAMMA (success exit protocol), not a backtrack — named accordingly.  s112 Lon in-chat: port labels spell the Greek letters (<FN>_α/_γ/_ω), never the English words */
        std::string blb = inl5 ? std::string(en4) : (std::string("LBL__") + en4);   /* BARE-CHAIN (Lon s62) + s112 rename: the body chain's one label IS the rt key spelling, LBL__<entry>.  NATURAL-LABEL (Lon s114): under role 5, en4 arrives as the ALREADY-RESOLVED natural port label (n<uid>_statement_begin_α from the dentry stamp) and is the whole spelling — no prefix. */
        const struct bb_label_t * lbl_b = emit_label_intern(lb.c_str()); const struct bb_label_t * lbl_o = emit_label_intern(lo.c_str());   /* R-1 s94: the gamma/omega faces interned ONCE -- lea (extlbl) and def share the object, both media */
        uint64_t body_cell = (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("body$") + en4).c_str());   /* R-1 s94: filled by the m3 driver after main seals (LBL__ registration twin); TEXT never reads it */
        auto SCALE16 = [&]() { return x86("mov", "rax", "rcx") + x86("add", "rax", "rax") + x86("add", "rax", "rax") + x86("add", "rax", "rax") + x86("add", "rax", "rax"); };   /* rax = K*16, no shl encoder */
        /* s63 fnrbp: SCRIP_FN_RBP=0 restores the s58 BOMB floaters (the descent instrument) and the frameless shim tail — one predicate, both halves of the bracket, drift-proof */
        auto RESTORE4 = [&](int lid) {   /* shared beta/omega restore body: needs rsp at tail base; leaves rcx=K r8=ext-base; clobbers rax; preserves rdi/rsi; lid = per-instantiation label-id base (beta/omega emit this twice in ONE box — ids must not collide) */
            return x86_rsp_load64("rcx", (int)(16 * xt4 + 16))
                 + SCALE16()
                 + x86("cmp", "rcx", (long)nf4) + x86_jcc_id("jbe", lid - 5) + x86("mov32", "eax", 16L * nf4) + x86_deflabel_id(lid - 5)   /* r8 = site_rsp - nf*16 in ALL arities: clamp like alpha */
                 + x86("lea", "r8", std::string("[rsp + ") + std::to_string(T4) + "]") + x86("sub", "r8", "rax")
                 + FOR(0, xt4, [&](int j) { int k = xt4 - 1 - j;
                       return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(gk4[nf4 + k])) + x86("mov", GQ(gk4[nf4 + k], 0), "rax")
                            + x86_rsp_load64("rax", 16 * k + 8) + x86("mov", GQ(gk4[nf4 + k], 8), "rax"); })
                 + FOR(0, nf4, [&](int i) {
                       return x86("cmp", "rcx", (long)i) + x86_jcc_id("jbe", lid + i)
                            + x86("mov", "rax", R8Q(16L * nf4 + 32 + 16L * i)) + x86("note", gva_name(gk4[i])) + x86("mov", GQ(gk4[i], 0), "rax")
                            + x86("mov", "rax", R8Q(16L * nf4 + 32 + 16L * i + 8)) + x86("mov", GQ(gk4[i], 8), "rax")
                            + x86_jmp_id(lid + 30 + i)
                            + x86_deflabel_id(lid + i)
                            + x86("mov", "rax", R8Q(16L * i)) + x86("note", gva_name(gk4[i])) + x86("mov", GQ(gk4[i], 0), "rax")
                            + x86("mov", "rax", R8Q(16L * i + 8)) + x86("mov", GQ(gk4[i], 8), "rax")
                            + x86_deflabel_id(lid + 30 + i); })
                 + x86_rsp_load64("r10", 16 * xt4) + x86_rsp_load64("r11", 16 * xt4 + 8); };
        if (fnsig()) {
            /* ⭐⭐⭐ SIG s66 SHIM (Lon in-chat: the site publishes "a STATIC MAP of how to reach them all" and the shim "can REACH DOWN the stack himself and avoid the middle man pushes").  Protocol:
             * site sets rcx = &sig {[0]=K, [8]=γcont, [16]=ωcont, [24+8i]=off_i entry-rsp-relative} and jmps here; the actuals never moved — they sit in the CALLER'S OWN operand cells at [entry+off_i],
             * and entry rsp IS the reference point (= rsp at the site's jmp = this frame's base + F4).  WHAT DIES vs the s58 record shim: the K-dependent max-carve + clamp + give-back (was recomputed on
             * ALL THREE faces α/γ/ω), the dual r8 derivations, and the two-arm release constant — the frame is a per-DEFINE CONSTANT F4 = T4 + nf*16 (extras+wires+sigbank+pad, then nf extension slots
             * used only for missing formals), and the release is one immediate.  The swap stays the manual-Ch.8 pushdown-by-swap, relocated: supplied formal i ⇒ SWAP [entry+off_i] ↔ formal-GVA (old
             * formal parks in the caller's own cell, restored one-way at γ/ω exactly as the record copy was); missing ⇒ old formal into extension slot [rsp+T4+16i], formal←null.  Over-arity: i≥nf sig
             * entries are simply never read; the extras stay caller-resident and statement_end releases them with every other operand cell.  Recursion is LIFO by construction (each activation swaps its
             * OWN caller's cells and restores them before that caller resumes).  Registers: rcx=sig (banked at [16*xt4+16], replacing the K bank), rdx=K, r8=entry rsp, rdi/rsi swap scratch in α (r11
             * scratch in γ/ω before its reload); result rides rdi:rsi across the γ restore as before.  SCRIP_FN_SIG=0 restores the s58 record shim below, byte-identical. */
            long F4 = T4 + 16L * nf4;
            auto SIGQ = [&](long d) { return std::string("[rcx + ") + std::to_string(d) + "]"; };
            auto EXTQ = [&](long d) { return std::string("[rsp + ") + std::to_string(T4 + d) + "]"; };
            auto R8AT = [&]() { return x86("lea", "r8", std::string("[rsp + ") + std::to_string(F4) + "]"); };
            auto FRESTORE = [&](int lid) {   /* shared γ/ω: sig from bank, K, r8=entry, extras reversed, formals via sig offsets (supplied) or extension (missing); leaves rcx=sig; r11 scratch pre-reload */
                return x86_rsp_load64("rcx", (int)(16 * xt4 + 16))
                     + x86("mov", "rdx", SIGQ(0))
                     + R8AT()
                     + FOR(0, xt4, [&](int j) { int k = xt4 - 1 - j;
                           return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(gk4[nf4 + k])) + x86("mov", GQ(gk4[nf4 + k], 0), "rax")
                                + x86_rsp_load64("rax", 16 * k + 8) + x86("mov", GQ(gk4[nf4 + k], 8), "rax"); })
                     + FOR(0, nf4, [&](int i) {
                           return x86("cmp", "rdx", (long)i) + x86_jcc_id("jbe", lid + i)
                                + x86("mov", "r11", SIGQ(24 + 8L * i)) + x86("add", "r11", "r8")
                                + x86("mov", "rax", "[r11 + 0]") + x86("note", gva_name(gk4[i])) + x86("mov", GQ(gk4[i], 0), "rax")
                                + x86("mov", "rax", "[r11 + 8]") + x86("mov", GQ(gk4[i], 8), "rax")
                                + x86_jmp_id(lid + 30 + i)
                                + x86_deflabel_id(lid + i)
                                + x86("mov", "rax", EXTQ(16L * i).c_str()) + x86("note", gva_name(gk4[i])) + x86("mov", GQ(gk4[i], 0), "rax")
                                + x86("mov", "rax", EXTQ(16L * i + 8).c_str()) + x86("mov", GQ(gk4[i], 8), "rax")
                                + x86_deflabel_id(lid + 30 + i); })
                     + x86_rsp_load64("r10", 16 * xt4) + x86_rsp_load64("r11", 16 * xt4 + 8); };
            return x86("comment", "IR_DEFINE role 4: SIG s66 per-DEFINE shim (alpha=swap-by-map, gamma/omega=restore-by-map, CONSTANT frame)")
                 + IF(inl5, x86_jmp_id(245))   /* SHIM-AT-STATEMENT (Lon s114): inline flow jumps OVER the shim — the statement's own trailer continues after the skip landing; the shim is entered only by name */
                 + x86("commentrule", std::string(119, '-'))   /* Lon s116 in-chat: line-break rule directly before the <FN>_Î± label */
                 + IF(!inl5, x86_alpha())
                 + x86_def_ext(emit_label_intern(la.c_str()))
                 + x86("sub", "rsp", F4)
                 + FOR(0, xt4, [&](int k) {
                       return x86("note", gva_name(gk4[nf4 + k])) + x86("mov", "rax", GQ(gk4[nf4 + k], 0)) + x86_rsp_store64(16 * k, "rax")
                            + x86("mov", "rax", GQ(gk4[nf4 + k], 8)) + x86_rsp_store64(16 * k + 8, "rax")
                            + x86("mov", GQ(gk4[nf4 + k], 0), (long)DT_SNUL) + x86("mov", GQ(gk4[nf4 + k], 8), (long)0); })
                 + x86_rsp_store64(16 * xt4, "r10") + x86_rsp_store64(16 * xt4 + 8, "r11") + x86_rsp_store64(16 * xt4 + 16, "rcx")
                 + x86("mov", "rdx", SIGQ(0))
                 + R8AT()
                 + FOR(0, nf4, [&](int i) {
                       return x86("cmp", "rdx", (long)i) + x86_jcc_id("jbe", 10 + i)
                            + x86("mov", "rdi", SIGQ(24 + 8L * i)) + x86("add", "rdi", "r8")
                            + x86("mov", "rax", "[rdi + 0]") + x86("note", gva_name(gk4[i])) + x86("mov", "rsi", GQ(gk4[i], 0)) + x86("mov", GQ(gk4[i], 0), "rax") + x86("mov", "[rdi + 0]", "rsi")
                            + x86("mov", "rax", "[rdi + 8]") + x86("mov", "rsi", GQ(gk4[i], 8)) + x86("mov", GQ(gk4[i], 8), "rax") + x86("mov", "[rdi + 8]", "rsi")
                            + x86_jmp_id(41 + i)
                            + x86_deflabel_id(10 + i)
                            + x86("note", gva_name(gk4[i])) + x86("mov", "rax", GQ(gk4[i], 0)) + x86("mov", EXTQ(16L * i).c_str(), "rax")
                            + x86("mov", "rax", GQ(gk4[i], 8)) + x86("mov", EXTQ(16L * i + 8).c_str(), "rax")
                            + x86("mov", GQ(gk4[i], 0), (long)DT_SNUL) + x86("mov", GQ(gk4[i], 8), (long)0)
                            + x86_deflabel_id(41 + i); })
                 + x86("lea", "r10", "extlbl", (uint64_t)(uintptr_t)lbl_b)
                 + x86("lea", "r11", "extlbl", (uint64_t)(uintptr_t)lbl_o)
                 + IF(fnrbp() == 1, x86("comment", "s63 RBP-FUNCTION WRITER (see the s58 arm's full comment — unchanged under SIG)")
                             + x86("sub", "rsp", (long)8)
                             + x86("push", "r11")
                             + x86("push", "r10")
                             + x86("push", "rbp")
                             + x86("mov", "rbp", "rsp"))
                 + IF(fnrbp() == 2, x86("comment", "s64 RSP-ONLY WRITER (see the s58 arm's full comment — unchanged under SIG)")
                             + x86("push", "r11")
                             + x86("push", "r10"))
                 + x86("jmp", "[rip@cell + __]", body_cell, blb.c_str())
                 + x86_def_ext(lbl_b)
                 + x86("note", gva_name(rgx)) + x86("mov", "rdi", GQ(rgx, 0)) + x86("mov", "rsi", GQ(rgx, 8))
                 + FRESTORE(80)
                 + x86("mov", "rcx", SIGQ(8))
                 + x86("add", "rsp", F4)
                 + x86("mov", "rax", "rdi") + x86("mov", "rdx", "rsi")
                 + x86("jmp", "rcx")
                 + x86_def_ext(lbl_o)
                 + FRESTORE(150)
                 + x86("mov", "rcx", SIGQ(16))
                 + x86("add", "rsp", F4)
                 + x86("mov32", "eax", (long)DT_FAIL) + x86("xor", "edx", "edx")
                 + x86("jmp", "rcx")
                 + IF(inl5, x86_deflabel_id(245))
                 + IF(!inl5, x86_gamma());
        }
        return x86("comment", "IR_DEFINE role 4: TINY-REAL s58 per-DEFINE shim (alpha=swap/extend, beta/omega=restore)")
             + IF(inl5, x86_jmp_id(245))   /* SHIM-AT-STATEMENT (Lon s114): same skip wrap as the SIG arm */
             + x86("commentrule", std::string(119, '-'))   /* Lon s116 in-chat: line-break rule directly before the <FN>_Î± label */
             + IF(!inl5, x86_alpha())
             + x86_def_ext(emit_label_intern(la.c_str()))
             + x86_rsp_load64("rcx", 0)
             + x86("mov", "r8", "rsp") + x86("sub", "r8", 16L * nf4)
             + SCALE16()
             + x86("cmp", "rcx", (long)nf4) + x86_jcc_id("jbe", 2) + x86("mov32", "eax", 16L * nf4) + x86_deflabel_id(2)   /* over-arity (K>nf, manual: extras evaluated+ignored): no extension exists — clamp the give-back to nf*16 */
             + x86("sub", "rsp", T4 + 16L * nf4)
             + x86("add", "rsp", "rax")
             + FOR(0, xt4, [&](int k) {
                   return x86("note", gva_name(gk4[nf4 + k])) + x86("mov", "rax", GQ(gk4[nf4 + k], 0)) + x86_rsp_store64(16 * k, "rax")
                        + x86("mov", "rax", GQ(gk4[nf4 + k], 8)) + x86_rsp_store64(16 * k + 8, "rax")
                        + x86("mov", GQ(gk4[nf4 + k], 0), (long)DT_SNUL) + x86("mov", GQ(gk4[nf4 + k], 8), (long)0); })
             + x86_rsp_store64(16 * xt4, "r10") + x86_rsp_store64(16 * xt4 + 8, "r11") + x86_rsp_store64(16 * xt4 + 16, "rcx")
             + FOR(0, nf4, [&](int i) {
                   return x86("cmp", "rcx", (long)i) + x86_jcc_id("jbe", 10 + i)
                        + x86("mov", "rax", R8Q(16L * nf4 + 32 + 16L * i)) + x86("note", gva_name(gk4[i])) + x86("mov", "rdx", GQ(gk4[i], 0)) + x86("mov", GQ(gk4[i], 0), "rax") + x86("mov", R8Q(16L * nf4 + 32 + 16L * i).c_str(), "rdx")
                        + x86("mov", "rax", R8Q(16L * nf4 + 32 + 16L * i + 8)) + x86("mov", "rdx", GQ(gk4[i], 8)) + x86("mov", GQ(gk4[i], 8), "rax") + x86("mov", R8Q(16L * nf4 + 32 + 16L * i + 8).c_str(), "rdx")
                        + x86_jmp_id(41 + i)
                        + x86_deflabel_id(10 + i)
                        + x86("note", gva_name(gk4[i])) + x86("mov", "rax", GQ(gk4[i], 0)) + x86("mov", R8Q(16L * i).c_str(), "rax")
                        + x86("mov", "rax", GQ(gk4[i], 8)) + x86("mov", R8Q(16L * i + 8).c_str(), "rax")
                        + x86("mov", GQ(gk4[i], 0), (long)DT_SNUL) + x86("mov", GQ(gk4[i], 8), (long)0)
                        + x86_deflabel_id(41 + i); })
             + x86("lea", "r10", "extlbl", (uint64_t)(uintptr_t)lbl_b)
             + x86("lea", "r11", "extlbl", (uint64_t)(uintptr_t)lbl_o)
             + IF(fnrbp() == 1, x86("comment", "s63 RBP-FUNCTION WRITER (Lon: ONE frame, FUNCTION linkage): pin the return point.  Frame [rbp+0]=enclosing rbp (recursion chain, LIFO)  [rbp+8]=gamma  [rbp+16]=omega  [rbp+24]=pad (reserved: RESULT base).  32B keeps C-call 16-alignment parity for the whole body.  rbp is SysV callee-saved: survives every C crossing; the ONLY other rbp writers product-wide are nested instances of this same bracket.  Readers: the RETURN/FRETURN/NRETURN floaters — mov rsp,rbp restores the UNKNOWN body depth to alpha-end depth P, pops the frame, jmps the banked landing; gamma/omega are pure functions of rsp==P (RESTORE4 rederives rcx=K and r8 from rsp).  Interior carves stay SILENT-NO-WHACK: nothing between here and the floater releases anything, the frame pop discards it wholesale.")
                         + x86("sub", "rsp", (long)8)
                         + x86("push", "r11")
                         + x86("push", "r10")
                         + x86("push", "rbp")
                         + x86("mov", "rbp", "rsp"))
             + IF(fnrbp() == 2, x86("comment", "s64 RSP-ONLY WRITER (Lon challenge: zero RBP): push the 16B {gamma,omega} pair at TOS — [rsp+0]=gamma [rsp+8]=omega, body entered at P-16 (16-parity kept).  NO anchor register: the floaters find the pair by the DEPTH-INVARIANCE LAW — control transfers only at depth-neutral statement boundaries; MATCH banks its own mark in the r12 arena; the alpha-sub/omega-add pairing releases statement temporaries.  A statement shape that leaks (the s58 -16 census class) breaks the law and dies loud at the floater's jmp — under this arm the red set IS the leak census.")
                         + x86("push", "r11")
                         + x86("push", "r10"))
             + x86("jmp", "[rip@cell + __]", body_cell, blb.c_str())
             + x86_def_ext(lbl_b)
             + x86("note", gva_name(rgx)) + x86("mov", "rdi", GQ(rgx, 0)) + x86("mov", "rsi", GQ(rgx, 8))
             + RESTORE4(80)
             + x86("mov32", "eax", T4 + 32 + 16L * nf4)
             + x86("cmp", "rcx", (long)nf4) + x86_jcc_id("jbe", 3) + SCALE16() + x86("add", "rax", T4 + 32) + x86_deflabel_id(3)   /* over-arity release: T+32+K*16 */
             + x86("mov", "rcx", R8Q(16L * nf4 + 16))
             + x86("add", "rsp", "rax")
             + x86("mov", "rax", "rdi") + x86("mov", "rdx", "rsi")
             + x86("jmp", "rcx")
             + x86_def_ext(lbl_o)
             + RESTORE4(150)
             + x86("mov32", "eax", T4 + 32 + 16L * nf4)
             + x86("cmp", "rcx", (long)nf4) + x86_jcc_id("jbe", 4) + SCALE16() + x86("add", "rax", T4 + 32) + x86_deflabel_id(4)
             + x86("mov", "rcx", R8Q(16L * nf4 + 24))
             + x86("add", "rsp", "rax")
             + x86("mov32", "eax", (long)DT_FAIL) + x86("xor", "edx", "edx")
             + x86("jmp", "rcx")
             + IF(inl5, x86_deflabel_id(245))
             + IF(!inl5, x86_gamma());
    }
    if (role == 1 || role == 2 || role == -1 /* NRETURN */) {
        /* ⛔⭐⭐⭐ LON RULING s54: globals GONE.  The RT_AB_ANCHOR global read and the rt_flat_ret_snap
         * (g_pcall peek) legacy path are DELETED from the floaters.  RETURN/NRETURN = jmp r10 (γ wire);
         * FRETURN = jmp r11 (ω wire) — LADDER WREG blob-exit spelling, now the floater spelling too.
         * ⛔ KNOWN BROKEN until WREG-3 lands: (a) rsp is NOT restored to entry depth here — the FORTH
         * paired-release discipline must hold interior-to-exit, and defers that survive on the spine break
         * it; (b) r10/r11 are SysV caller-saved — every C crossing in the body clobbers them until the
         * per-activation spine capture (WREG-3's law: pendings capture {r10,r11} at push, β restores).
         * Lon: "worry not about breakage."  The save-set restore stays caller-side (role 0's own rsp slots
         * + the landing epilogue) — already stack-resident. */
        /* ⭐⭐⭐ s58 RSP-ONLY DESCENT MODE (Lon in-chat: "put BOMBS in RETURN, FRETURN, and NRETURN... We want to run the entire test suite in RSP only mode... be left with only all the spots that
         * need RBP" — maybe RBP, maybe something else; the bombs make every UNKNOWN-STACK-DEPTH spot self-evident).  The coming-out side is FROZEN: RETURN arrives at a depth no one can know without a
         * frame anchor (the measured −16 class), so the jmp-r10/r11 wires are replaced by loud named bombs.  Reaching the RETURN bomb IS the descent test passing: args installed, body ran, transfer
         * arrived.  git revert is the undo when the depth mechanism lands. */
        if (fnrbp() == 1)
            /* s63 RBP-FUNCTION READERS (Lon: "RETURN, FRETURN, and NRETURN can restore an unknown stack depth and immediately return to the SHIM at the proper stack depth by POPPING the RBP stack frame").
             * rbp = innermost activation's pin (the shim's writer); [rbp+0]=enclosing rbp  [rbp+8]=gamma  [rbp+16]=omega  [rbp+24]=pad.  After the full 32B pop rsp == alpha-end depth P — exactly what
             * gamma/omega's rsp-derived RESTORE4 + constant release assume.  The floaters are program-wide singletons and discover the callee DYNAMICALLY through rbp — this is the whole point of the frame.
             * NRETURN rides gamma (manual: success by name; the result cell holds a NAME descr, deref is the consumer's business — 1013 is the witness).  rcx is free here (gamma reloads K itself).
             * ⛔ KNOWN EDGE: a top-level :(RETURN) with no active frame jumps through whatever rbp holds — pre-rung it bombed; SPITBOL errors there too ("&FNCLEVEL==0" class).  Accepted, noted. */
            return x86("comment", role == 1 ? "IR_DEFINE RETURN floater (s63: pop the RBP frame, land on gamma)" :
                                   role == 2 ? "IR_DEFINE FRETURN floater (s63: pop the RBP frame, land on omega)" :
                                               "IR_DEFINE NRETURN floater (s63: pop the RBP frame, land on gamma — by-name result)")
                 + x86_alpha()
                 + x86("mov", "rsp", "rbp")
                 + x86("pop", "rbp")
                 + (role == 2 ? x86("add", "rsp", (long)8) + x86("pop", "rcx") + x86("add", "rsp", (long)8)
                              : x86("pop", "rcx") + x86("add", "rsp", (long)16))
                 + x86("jmp", "rcx");
        if (fnrbp() == 2)
            /* s64 RSP-ONLY READERS: by the depth-invariance law rsp HERE == P-16 (body-entry), so the pair is AT TOS.
             * [rsp+0]=gamma [rsp+8]=omega; pop/skip to land at P — exactly what gamma/omega's rsp-derived RESTORE4 +
             * constant release assume.  Recursion: each nested call pushed its own pair deeper, LIFO by stack discipline.
             * A leaking body arrives BELOW P-16 and jmps junk — loud, named, the census working as designed. */
            return x86("comment", role == 1 ? "IR_DEFINE RETURN floater (s64 RSP-ONLY: pop {gamma,omega} pair at TOS — depth IS the anchor)" :
                                   role == 2 ? "IR_DEFINE FRETURN floater (s64 RSP-ONLY: skip gamma, pop omega — depth IS the anchor)" :
                                               "IR_DEFINE NRETURN floater (s64 RSP-ONLY: pop gamma — by-name result)")
                 + x86_alpha()
                 + (role == 2 ? x86("add", "rsp", (long)8) + x86("pop", "rcx")
                              : x86("pop", "rcx") + x86("add", "rsp", (long)8))
                 + x86("jmp", "rcx");
        return x86("comment", role == 1 ? "IR_DEFINE RETURN floater (s58: BOMB — coming-out frozen)" :
                               role == 2 ? "IR_DEFINE FRETURN floater (s58: BOMB — coming-out frozen)" :
                                           "IR_DEFINE NRETURN floater (s58: BOMB — coming-out frozen)")
             + x86_alpha()
             + (role == 1 ? x86_bomb("BOMB-RETURN: descent complete, coming-out frozen (s58 RSP-only) — UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor")
              : role == 2 ? x86_bomb("BOMB-FRETURN: descent complete, coming-out frozen (s58 RSP-only) — UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor")
                          : x86_bomb("BOMB-NRETURN: descent complete, coming-out frozen (s58 RSP-only) — UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"));
    }
    /* role 0 — CALL2BB slice 2 (Lon s21x-c: "Have each BB allocate its RESULT value… its LOCAL STORAGE needs… by one instruction, decrement RSP"; "DEFINE, when CONSTANT FOLDED, emits exactly TWO BBs:
     * an IR_DEFINE and an IR_CALL").  THE CALL-SITE SAVE/INSTALL BOX: this box's LOCALS are the save-set slots — carved by its OWN single `sub rsp` (never a whole-graph carve, never ___-indexed),
     * offsets slide from RSP.  Body = the BP-7 SCC prefix migrated out of bb_call_proc_staged: (1) spill the DEFINE save-set's old GVA cell values (fname/formals/locals per manual Ch.8 p.104 + Ch.19
     * DEFINE: "saved prior to function entry, and restored upon function return") into the own slots; (2) open_slim — ALL runtime guards re-checked before ANY side effect, so a 0 return needs nothing
     * undone (the spilled copies are pure reads, released by the decline's add); (3) committed: install the staged args into the formals' NV GLOBAL cells (SNOBOL4's one namespace — no ζ in the call
     * process, Lon SCOPE LAW).  The outcome rides rax into the adjacent staged call box (1 = record open + block LIVE across the edge for its landings to restore+release; 0 = declined + block released
     * → the call box's classic fallback at base depth).  Handoff via bb_scc_handoff_set = the emit-order contract (sr0.γ → call, chain-adjacent); non-eligible sites and any unresolved arg slot fall to
     * a pass-through so the call box keeps its full merged body — behavior identical to the gate-off world at those sites.  FRQB(slot, sb) self-compensates the live carve (the 083 FLATDISP-LIVE-BUMP
     * law); the fc_hit window arm lives inside FRQB, self-disabling here since DRIVE_FILL reset this node's grant fields. */
    int c2np = 0, c2nsave = 0, c2res_gk = -1; int c2gk[64]; long c2nargs = (long)_.op_arg_slot_n;
    int elig = bb_scc_probe(_.op_sval, (int)c2nargs, &c2np, &c2nsave, c2gk, &c2res_gk);
    for (int i = 0; elig && i < (int)c2nargs; i++) if (_.op_arg_slot[i] < 0) elig = 0;
    if (elig && !bb_scc_handoff_room()) elig = 0;   /* pending missing (drive shape surprise) or table full: decline safely — the call box keeps its merged body */
    if (!elig) { bb_scc_handoff_pending_clear(); return x86("comment", "IR_DEFINE role 0: site not SCC-eligible (or an arg slot unresolved, or no handoff room) — pass-through; the staged call box runs its full merged body")
             + x86_alpha()
             + x86_gamma(); }
    long sb = 16L * (long)c2nsave;
    uint64_t slim_fp; { long (*fp)(const char *, int, int) = rt_proc_call_open_slim; slim_fp = (uint64_t)(uintptr_t)(void *)fp; }
    std::string s = x86("comment", "IR_DEFINE role 0 (CALL2BB slice 2): own sub-rsp carve, save-set spill (GVA -> own slots), open_slim, staged args -> NV globals; outcome rides rax into the staged call box")
         + x86_alpha()
         + x86_zclaim(sb)
         + FOR(0, c2nsave, [&](int k) {
               return x86("note", gva_name(c2gk[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(c2gk[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)c2gk[k] * 16)) + x86_rsp_store64(16 * k, "rax")
                    + x86("note", gva_name(c2gk[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(c2gk[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)c2gk[k] * 16 + 8)) + x86_rsp_store64(16 * k + 8, "rax"); })
         + x86_scan_sync_out()
         + x86_ro_load_q("rdi", 0)
         + x86("mov32", "esi", (long)c2np)
         + x86("mov32", "edx", c2nargs)
         + x86("call", "rt_proc_call_open_slim", slim_fp)
         + x86_scan_sync_in_rr()
         + x86("test", "rax", "rax")
         + x86("jne", L(1))
         + x86_zrelease(sb)
         + x86("jmp", L(2))
         + x86("def", L(1))
         + FOR(0, (int)c2nargs, [&](int i) { int slot = _.op_arg_slot[i];
               return x86("mov", "rax", FRQB(slot, (int)sb))
                    + x86("note", gva_name(c2gk[i])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(c2gk[i], 0) : ABSQ(RT_GVA_VA + (unsigned long)c2gk[i] * 16), "rax")
                    + x86("mov", "rax", FRQB(slot + 8, (int)sb))
                    + x86("note", gva_name(c2gk[i])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(c2gk[i], 8) : ABSQ(RT_GVA_VA + (unsigned long)c2gk[i] * 16 + 8), "rax"); })
         + x86("mov32", "eax", 1L)
         + x86("def", L(2))
         + x86_gamma()
         + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");
    if (!bb_scc_handoff_arm()) return x86_alpha() + x86_bomb("IR_DEFINE role 0: handoff arm failed after the room check passed (impossible unless the drive deposit raced)");
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_nreturn_mark() {
    extern int rt_g_ret_by_name;
    return x86_alpha()
         + x86("comment", "NRETURN floater: by-name mark (manual p.133); rt_nret_fix in the call epilogue reads+clears it; depth-agnostic — zero [rsp+K], zero calls; glue continues at RETURN")
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_ret_by_name, "rt_g_ret_by_name")
         + x86("mov", RDD("rax", 0), (long)1)
         + x86_gamma();
}

/*-----------------------------------------------------------------------------------------------------------------------*/
std::string bb_define() { int r = g_emit.op_define_role; if (r == 6) return bb_define_bind(); if (r == 7) return bb_define_activate(); return bb_define_sr(); }   /* ONE IR + ONE BB (Lon s116 in-chat): the single DEFINE template.  Role deposited by the dispatch: 0..4 = manual-Ch.8 save/restore citizens exactly as before (0 CALL2BB site, 1 RETURN floater, 2 FRETURN floater, 3 wire-adopt, 4 per-DEFINE shim), 5 = shim inline at the DEFINE statement, 6 = the constant-folded bind (rt_define_site), 7 = the per-DEFINE activation block, 8 = RESERVED dynamic arm (DEFINE(variable) -> call rt_define; lands with the runtime leaf). */
