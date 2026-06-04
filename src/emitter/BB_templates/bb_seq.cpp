/* bb_seq.cpp — BB template for BB_SEQ (Icon/Raku compound-stmt sequence).
   RK-BB-2 step 6, GOAL-RAKU-BB. One file per BB kind per RULES.md. x86 only.

   BB_SEQ children form a γ-chain rooted at seq->α (per bb_exec.c BB_SEQ case + lower_icn.c
   proc-body wiring). Each child statement's γ AND ω point to the NEXT stmt's entry (Icon
   compound semantics — success or failure both advance, no backtracking across stmts).

   For Raku gather-bodies (BB_LANG_RKU), this is the multi-yield driver. The body is a
   sequence of BB_SUSPEND nodes (one per take()). The SM_BB_SWITCH wrapping this SEQ pumps
   it: α drives stmt0, β resumes whichever stmt last yielded, ω terminates when drained.

   ⛔ MULTI-YIELD STATE: a single SM_BB_SWITCH β-entry label has to know which child
   yielded last. We carry a `resume_addr` slot in .data — a quad holding the address to
   jmp to on next β. Init=0 (sentinel for "fresh, go to child 0 α"). Each child's γ-out
   is wrapped by a per-stmt fixup that writes `resume_addr = next_child_α` (or seq_ω-tail
   for the last yielder) BEFORE jumping to the outer γ. The outer β-entry loads
   resume_addr and indirect-jumps to it.

   Port wiring per child stmt_k (k in [0, n-1], last = n-1):
     stmt_k.α  =  .Lseq{id}_s{k}_α   (fresh entry)
     stmt_k.β  =  .Lseq{id}_s{k}_β   (resume — currently unused; β re-entries flow via
                                       resume_addr, not via the child's own β label)
     stmt_k.γ  =  .Lseq{id}_s{k}_γ   (success yield — fixup writes resume_addr for the
                                       NEXT child, then jmps to _.lbl_γ outer)
     stmt_k.ω  =  next stmt α  if k<last  else  _.lbl_ω outer
                                       (BB_SUSPEND's β does jmp-ω, so on resume we slide
                                       to the NEXT child's α — its first-yield path)

   ⛔ ENTRY DISPATCH: stmt_k.α is reached two ways:
     (1) from the outer SEQ.α on fresh entry (resume_addr==0) — but only stmt0
     (2) from stmt_{k-1}.ω after stmt_{k-1}'s β fired (BB_SUSPEND β→ω)

   So the outer α-entry needs ONE-TIME path-to-stmt0_α (resume_addr is 0 initially); after
   stmt0 yields, resume_addr is set to whatever the fixup needs. On β-entry we always do
   indirect jump. The 0-init resume_addr is a problem if the indirect-jump path ever
   reads it before stmt0 sets it — but the SM_BB_SWITCH already gates this with its own
   .byte flag (fresh→fall through to α here; β-loop→jmp _.lbl_β). So our outer β path is
   only reached AFTER our outer α has fired and a child has set resume_addr.

   LITERAL FAST-PATH ONLY: this implementation handles BB_SEQ children that are BB_SUSPEND
   nodes (i.e. Raku gather bodies). Other compositions (Icon/Snocone proc bodies with mixed
   stmt kinds) currently fall through to bb_stub via emit_core's group case (this template
   is reached only when emit_core peels BB_SEQ off — see step 5).

   FACT RULE: every byte emitted via s_* / bytes() — no seg_byte, SL_B, sl_emit_one, or
   emit_standard_blob. PEERS RULE: no fields added to BB_t. */
#include <string>
#include <string.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Count the γ-chain length rooted at α. */
static int seq_chain_len(BB_t * head) {
    int n = 0;
    for (BB_t * c = head; c; c = c->γ) n++;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Emit one child's four-port x86 inline with caller-chosen labels. Returns the box's emitted */
/* TEXT as a string. Mirrors bb_binop_gen.cpp emit_child_box.                                  */
static std::string emit_child_box_seq(BB_t * child,
                                      bb_label_t * la, bb_label_t * lg,
                                      bb_label_t * lo, bb_label_t * lb) {
    g_emit.lbl_α = la->name; g_emit.lbl_α_p = la;
    g_emit.lbl_γ = lg->name; g_emit.lbl_γ_p = lg;
    g_emit.lbl_ω = lo->name; g_emit.lbl_ω_p = lo;
    g_emit.lbl_β = lb->name; g_emit.lbl_β_p = lb;
    char * c = walk_bb_node_str_c(child);
    std::string s = c ? c : "";
    if (c) free(c);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_seq_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        int id   = bb_node_id(pBB);
        int n    = seq_chain_len(pBB->α);

        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — BB_SEQ");

        /* Non-Raku-gather BB_SEQ (n==0, or any context other than Raku gather body): fall back to     */
        /* the passthrough shape — α→γ, β→ω. Walk g_emit.xa_bb_emit_pair_* populated by               */
        /* flat_drive_seq's n==0 branch (EMIT_PAIR_JMP(γ) + EMIT_PAIR_DEF_JMP(β, ω)). Same FACT-clean  */
        /* shape as bb_conj.cpp — each byte literally produced here, bin sites/labels registered    */
        /* inline so the audit recognizes substantive emission.                                       */
        if (n == 0) {
            if (MEDIUM_TEXT) {
                std::string s;
                for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
                    if (g_emit.xa_bb_emit_pair_define[i]) s += emit_fmt("%s:\n", g_emit.xa_bb_emit_pair_define[i]->name);
                    if (g_emit.xa_bb_emit_pair_jmp[i])    s += s_1asm(emit_fmt("jmp %s", g_emit.xa_bb_emit_pair_jmp[i]->name));
                }
                return s;
            }
            if (MEDIUM_BINARY) {
                std::string b;
                for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
                    if (g_emit.xa_bb_emit_pair_define[i]) {
                        bin.sites.push_back((int)b.size());
                        bin.labels.push_back(g_emit.xa_bb_emit_pair_define[i]);
                        bin.is_def.push_back(true);
                    }
                    if (g_emit.xa_bb_emit_pair_jmp[i]) {
                        b += bytes(1, "\xE9");
                        bin.sites.push_back((int)b.size());
                        bin.labels.push_back(g_emit.xa_bb_emit_pair_jmp[i]);
                        bin.is_def.push_back(false);
                        b += u32le(0);
                    }
                }
                return b;
            }
        }

        if (MEDIUM_TEXT) {
            /* Save outer labels — child emission will overwrite g_emit.lbl_* via emit_child_box_seq. */
            const char * outer_α = _.lbl_α;
            const char * outer_γ = _.lbl_γ;
            const char * outer_ω = _.lbl_ω;
            const char * outer_β = _.lbl_β;
            bb_label_t * outer_γ_p = _.lbl_γ_p;
            bb_label_t * outer_ω_p = _.lbl_ω_p;

            /* IBB GROUND-ZERO RESET (Opus 4.7, 2026-05-28): structural shape detection.            */
            /* The original BB_SEQ template was the Raku-gather multi-yield driver — every child  */
            /* γ yields out through outer γ, awaiting outer β to resume the next child. That       */
            /* semantics is wrong for non-yielding sequences (Icon proc body, SNOBOL4 stmt list):  */
            /* each statement's γ should fall through to the NEXT statement's α, only the LAST    */
            /* statement's γ goes to outer γ. Detect the case structurally: walk the γ-chain of   */
            /* children; if ANY child is BB_SUSPEND we're in gather-driver territory. Otherwise    */
            /* we're a flat in-order sequence.                                                     */
            int has_suspend = 0;
            for (BB_t * c = pBB->α; c; c = c->γ) {
                if (c->t == BB_SUSPEND) { has_suspend = 1; break; }
            }

            /* Pre-build the per-child label structs so we can chain γ/ω. */
            std::vector<bb_label_t> Lα(n), Lγ(n), Lω(n), Lβ(n);
            for (int k = 0; k < n; k++) {
                emit_label_initf(&Lα[k], ".Lseq%d_s%d_α", id, k);
                emit_label_initf(&Lγ[k], ".Lseq%d_s%d_γ", id, k);
                emit_label_initf(&Lω[k], ".Lseq%d_s%d_ω", id, k);
                emit_label_initf(&Lβ[k], ".Lseq%d_s%d_β", id, k);
            }

            if (!has_suspend) {
                /* FLAT IN-ORDER SEQUENCE shape.                                                   */
                /*   outer_α: jmp s0_α                                                             */
                /*   sK emitted with lbl_α=Lα[k], lbl_γ=Lα[k+1] (chain), lbl_ω=outer_ω             */
                /*   LAST stmt's lbl_γ=outer_γ                                                      */
                /*   outer_β: jmp outer_ω  (no backtracking)                                        */
                std::string head =
                      s_directive(".intel_syntax noprefix")
                    + s_1asm(std::string(outer_α) + ":")
                    + s_comment(emit_fmt("# BOX BB_SEQ(n=%d, flat in-order sequence — no SUSPEND children)", n))
                    + s_1asm(emit_fmt("jmp .Lseq%d_s0_α", id));

                std::string body;
                BB_t * cur = pBB->α;
                for (int k = 0; k < n && cur; k++, cur = cur->γ) {
                    bb_label_t * child_γ_target = (k + 1 < n) ? &Lα[k + 1] : outer_γ_p;
                    body += emit_child_box_seq(cur, &Lα[k], child_γ_target, outer_ω_p, &Lβ[k]);
                }

                std::string tail =
                      s_L1asm(std::string(outer_β) + ":", "")
                    + s_comment("# flat seq: no backtracking — β -> outer ω")
                    + s_1asm(std::string("jmp ") + outer_ω);

                g_emit.lbl_α = outer_α;
                g_emit.lbl_γ = outer_γ; g_emit.lbl_γ_p = outer_γ_p;
                g_emit.lbl_ω = outer_ω; g_emit.lbl_ω_p = outer_ω_p;
                g_emit.lbl_β = outer_β;

                return head + body + tail;
            }

            /* ─── GATHER-DRIVER shape (legacy, Raku-gather; reached when any child is BB_SUSPEND) */
            std::string resume_slot = emit_fmt(".Lseq%d_resume", id);
            std::string seq_α       = std::string(outer_α);
            std::string seq_β       = std::string(outer_β);

            std::string head =
                  s_directive(".section .data")
                + s_directive(resume_slot + ": .quad 0")
                + s_directive(".section .text")
                + s_directive(".intel_syntax noprefix")
                + s_1asm(seq_α + ":")
                + s_comment(emit_fmt("# BOX BB_SEQ(n=%d, gather multi-yield driver)", n))
                + s_1asm(emit_fmt("jmp .Lseq%d_s0_α", id))
                + s_L1asm(seq_β + ":", "")
                + s_comment("# β-resume: indirect-jmp to resume_addr (set by last yielder)")
                + s_2asm("lea rax,", "[rip + " + resume_slot + "]")
                + s_2asm("mov rax,", "[rax]")
                + s_2asm("jmp",      "rax");

            std::string body;
            BB_t * cur = pBB->α;
            for (int k = 0; k < n && cur; k++, cur = cur->γ) {
                /* Each child uses its own α/γ/ω/β labels. Note: child's ω becomes the NEXT child's    */
                /* α (or outer ω if last) by SUBSTITUTING the ω-label PASSED into the child template.   */
                bb_label_t * child_ω_target;
                static bb_label_t alias_last_ω;
                if (k + 1 < n) {
                    /* Make Lω[k] act as an alias for Lα[k+1]: don't define Lω[k]; just emit the       */
                    /* child with lbl_ω = Lα[k+1] directly. */
                    child_ω_target = &Lα[k + 1];
                } else {
                    /* Last stmt: child's ω is outer ω. */
                    child_ω_target = outer_ω_p;
                }
                body += emit_child_box_seq(cur, &Lα[k], &Lγ[k], child_ω_target, &Lβ[k]);
                /* Fixup block after child k's γ-label: when child k's body emits `jmp <its γ>`,         */
                /* execution lands here (right after the child's emitted code, where we DEFINE Lγ[k]    */
                /* if the child didn't already). bb_suspend emits its OWN lbl_γ: label before yielding   */
                /* via rt_push_int — and it writes the label into the child's emitted asm. So Lγ[k]    */
                /* IS already defined inside the child's emitted text. We then need a fixup ONLY if the */
                /* child's jmp γ would land us at a label that's already been emitted INSIDE the child. */
                /* But bb_suspend's γ-path is `jmp _.lbl_γ` (the value passed in via emit_child_box_seq, */
                /* which is Lγ[k]'s name). So execution falls into whatever code we put AT the address  */
                /* of Lγ[k]. bb_suspend itself doesn't emit a `Lγ[k]:` definition — its emitted code   */
                /* contains `mov rdi,N; call rt_push_int@PLT; jmp <γ_name>`. So Lγ[k] is UNDEFINED      */
                /* inside the child's text. WE define it here — between this child's text and the next  */
                /* child's text. The fixup writes resume_addr := <next-α or seq-done> then jmps outer γ. */
                std::string nxt_addr;
                if (k + 1 < n) {
                    nxt_addr = std::string(Lα[k + 1].name);
                } else {
                    /* After last yielder, next β-entry should drain via outer ω. We point resume_addr   */
                    /* at a tiny "done" trampoline label that jmps outer ω. */
                    nxt_addr = emit_fmt(".Lseq%d_done", id);
                }
                body += s_L1asm(std::string(Lγ[k].name) + ":", "")
                      + s_comment(emit_fmt("# fixup: set resume_addr = %s ; then γ-out", nxt_addr.c_str()))
                      + s_2asm("lea rax,", "[rip + " + resume_slot + "]")
                      + s_2asm("lea rcx,", "[rip + " + nxt_addr + "]")
                      + s_2asm("mov [rax],", "rcx")
                      + s_1asm(std::string("jmp ") + outer_γ);
            }

            /* Done trampoline — resume_addr lands here after the last yield. */
            std::string tail =
                  s_L1asm(emit_fmt(".Lseq%d_done:", id), "")
                + s_comment("# SEQ drained — jmp outer ω")
                + s_1asm(std::string("jmp ") + outer_ω);

            /* Restore outer label state (best-effort — the SM_BB_SWITCH caller doesn't read these     */
            /* further after walk_bb_node_str_c returns; this is housekeeping for any sibling emission). */
            g_emit.lbl_α = outer_α;
            g_emit.lbl_γ = outer_γ; g_emit.lbl_γ_p = outer_γ_p;
            g_emit.lbl_ω = outer_ω; g_emit.lbl_ω_p = outer_ω_p;
            g_emit.lbl_β = outer_β;

            return head + body + tail;
        }

        if (MEDIUM_BINARY) {
            /* IBB-3 mode-3 (Opus 4.7, 2026-05-28 follow-up). The driver flat_drive_seq populated   */
            /* g_emit.xa_bb_emit_pair_*[] with the SEQ's own glue pairs (β-def + jmp to outer ω)    */
            /* before EMIT_PAIR_FILL → walk_bb_node → us. Walk the arrays, emit \xE9 + u32le(0)     */
            /* per jmp pair, build bin.sites/labels/is_def. Mirrors bb_conj.cpp's EP-pair loop.   */
            std::string b;
            for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
                if (g_emit.xa_bb_emit_pair_define[i]) {
                    bin.sites.push_back((int)b.size());
                    bin.labels.push_back(g_emit.xa_bb_emit_pair_define[i]);
                    bin.is_def.push_back(true);
                }
                if (g_emit.xa_bb_emit_pair_jmp[i]) {
                    b += bytes(1, "\xE9");
                    bin.sites.push_back((int)b.size());
                    bin.labels.push_back(g_emit.xa_bb_emit_pair_jmp[i]);
                    bin.is_def.push_back(false);
                    b += u32le(0);
                }
            }
            return b;
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* M3-RK-NOINTERP-1d (Opus 4.8, 2026-05-29). Raw-x86 gather multi-yield driver for the mode-3   */
/* SM_BB_INVOKE → walk_bb_node path, where g_emit.xa_bb_emit_pair_*[] is UNPOPULATED (no         */
/* flat_drive_seq ran) and the MEDIUM_BINARY pair-loop above would emit nothing — leaving the    */
/* wrapper's outer β label (.Lbbinv%d_β) unresolved. This is the raw-bytes mirror of the         */
/* MEDIUM_TEXT gather-driver (the has_suspend branch above). Children (BB_SUSPEND/BB_FAIL) carry  */
/* their own working MEDIUM_BINARY arms; we emit only the seq spine (α fan-out, β resume          */
/* indirect-jump, per-child γ fixups, done trampoline) directly into the shared bb_emit_buf,      */
/* interleaving walk_bb_node(child, NULL) calls — the same emit-in-place idiom the SM_BB_INVOKE    */
/* wrapper itself uses. The TEXT driver's .data resume quad becomes a malloc'd slot (the scratch  */
/* page has no .data section); intermediate port labels are malloc'd so their pointers survive    */
/* into the wrapper's bb_emit_end (which resolves all patches after bb_seq returns). The wrapper-  */
/* owned outer ports γ/ω/β are referenced via _.lbl_*_p (stack-stable across that bb_emit_end).   */
/* Returns 1 if it handled emission, 0 to fall through to the bb_seq_str path. FACT/PEERS clean.  */
static int bb_seq_gather_binary(BB_t * pBB) {
    if (!(PLATFORM_X86 && MEDIUM_BINARY)) return 0;
    int n = seq_chain_len(pBB->α);
    if (n == 0) return 0;
    int has_suspend = 0;
    for (BB_t * c = pBB->α; c; c = c->γ) if (c->t == BB_SUSPEND) { has_suspend = 1; break; }
    if (!has_suspend) return 0;
    int id = bb_node_id(pBB);
    uint64_t * resume_slot = (uint64_t *)calloc(1, sizeof(uint64_t));
    if (!resume_slot) return 0;
    bb_label_t * Lα = (bb_label_t *)calloc((size_t)n, sizeof(bb_label_t));
    bb_label_t * Lγ = (bb_label_t *)calloc((size_t)n, sizeof(bb_label_t));
    bb_label_t * Lω = (bb_label_t *)calloc((size_t)n, sizeof(bb_label_t));
    bb_label_t * Lβ = (bb_label_t *)calloc((size_t)n, sizeof(bb_label_t));
    bb_label_t * Ldone = (bb_label_t *)calloc(1, sizeof(bb_label_t));
    if (!Lα || !Lγ || !Lω || !Lβ || !Ldone) { return 0; }
    for (int k = 0; k < n; k++) {
        emit_label_initf(&Lα[k], ".Lseq%d_s%d_α", id, k);
        emit_label_initf(&Lγ[k], ".Lseq%d_s%d_γ", id, k);
        emit_label_initf(&Lω[k], ".Lseq%d_s%d_ω", id, k);
        emit_label_initf(&Lβ[k], ".Lseq%d_s%d_β", id, k);
    }
    emit_label_initf(Ldone, ".Lseq%d_done", id);
    bb_label_t * outer_γ_p = _.lbl_γ_p;
    bb_label_t * outer_ω_p = _.lbl_ω_p;
    bb_label_t * outer_β_p = _.lbl_β_p;
    bb_emit_byte(0xE9); bb_emit_patch_rel32(&Lα[0]);
    bb_label_define(outer_β_p);
    bb_emit_byte(0x48); bb_emit_byte(0xB8); bb_emit_u64((uint64_t)(uintptr_t)resume_slot);
    bb_emit_byte(0x48); bb_emit_byte(0x8B); bb_emit_byte(0x00);
    bb_emit_byte(0xFF); bb_emit_byte(0xE0);
    BB_t * cur = pBB->α;
    for (int k = 0; k < n && cur; k++, cur = cur->γ) {
        bb_label_t * child_ω = (k + 1 < n) ? &Lα[k + 1] : outer_ω_p;
        bb_label_define(&Lα[k]);
        _.lbl_α = Lα[k].name; _.lbl_α_p = &Lα[k];
        _.lbl_γ = Lγ[k].name; _.lbl_γ_p = &Lγ[k];
        _.lbl_ω = child_ω->name; _.lbl_ω_p = child_ω;
        _.lbl_β = Lβ[k].name; _.lbl_β_p = &Lβ[k];
        walk_bb_node(cur, NULL);
        bb_label_t * nxt = (k + 1 < n) ? &Lα[k + 1] : Ldone;
        bb_label_define(&Lγ[k]);
        bb_emit_byte(0x48); bb_emit_byte(0xB8); bb_emit_u64((uint64_t)(uintptr_t)resume_slot);
        bb_emit_byte(0x48); bb_emit_byte(0x8D); bb_emit_byte(0x0D); bb_emit_patch_rel32(nxt);
        bb_emit_byte(0x48); bb_emit_byte(0x89); bb_emit_byte(0x08);
        bb_emit_byte(0xE9); bb_emit_patch_rel32(outer_γ_p);
    }
    bb_label_define(Ldone);
    bb_emit_byte(0xE9); bb_emit_patch_rel32(outer_ω_p);
    _.lbl_γ_p = outer_γ_p; _.lbl_ω_p = outer_ω_p; _.lbl_β_p = outer_β_p;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_seq(BB_t * pBB) {
    if (bb_seq_gather_binary(pBB)) return;
    bb_bin_t bin;
    bb_emit_asm_result(bb_seq_str(pBB, bin), bin);
}
