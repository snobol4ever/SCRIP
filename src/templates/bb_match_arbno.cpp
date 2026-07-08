#include <string>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" void * rt_zcol_push(void ** ptr_cell, int * cap_cell, int i, long elem_sz);
extern "C" void * rt_zls_alloc(long bytes);
extern "C" void   rt_zls_release(void *fb);
extern "C" void   rt_zls_arbno_step1_store(void *p);
extern "C" void * rt_zls_arbno_step1_load(void);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* BB-OWNED-ζ STEP 1 (Lon pivot, this session).  x86_arbno_role0_alloc() — gated on ZC_SELFLOAD_ALLOC, fully
 * inert otherwise.  Fires at role 0's own α entry (the one point a fresh ARBNO activation begins) and stores
 * the resulting block pointer via rt_zls_arbno_step1_store (zeta_alloc.c) — a single runtime-side static
 * carrier, NOT slot-packed (role 0's own zls_field grant is exactly 16B, "3x4B + pad" meaning 4 bytes of pad
 * at +12..+15, not 8 — an 8-byte pointer does not fit there without spilling into whatever field is granted
 * next in the whole-program slot layout).  Carries the pointer to role 2's matching free WITHOUT repointing
 * r12 itself — r12 stays exactly as the outer function's single static prologue set it, so every sibling box
 * interleaved with this ARBNO activation is completely unaffected.  This deliberately does NOT yet make role
 * 0/1/2's own [r12+off] reads/writes land inside the allocated block (that's the harder producer/consumer
 * r12-sharing problem the design doc's own open sub-question flags, ZB-ACT-1 territory) -- this slice proves
 * ONLY the alloc-discipline: one alloc per fresh role-0 activation, one matching free at the provably-true
 * exit (role 2), counts balance, nothing corrupts.  HONEST SCOPE (see zeta_alloc.c for the full note):
 * correct for SEQUENTIAL re-entry only, not yet nested/concurrent activations of the same node. */
static std::string x86_arbno_role0_alloc() {
    if (x86_selfload_mode() != ZC_SELFLOAD_ALLOC) return std::string();
    return x86_align_enter()
         + x86("mov",  "rdi", 4096L)
         + x86("call", "rt_zls_alloc", (uint64_t)(uintptr_t)(void *)(void * (*)(long))rt_zls_alloc)
         + x86("mov",  "rdi", "rax")
         + x86("call", "rt_zls_arbno_step1_store", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_zls_arbno_step1_store)
         + x86_align_leave();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_arbno_role2_free() — mirror of the above, at role 2's true exit.  Loads the carrier pointer and frees
 * it.  Gated identically; inert unless ZC_SELFLOAD_ALLOC mode is active.  NOTE: op_omega_is_death's own gate
 * in x86_jmp (x86_asm.h) ALSO fires for role 2's jmp "ω" — this template-level free and that central-hook
 * free are NOT the same call and would double-free if both fired.  This rung uses this template-level call
 * as the actual free (since it needs the carrier-load step anyway, which the central hook doesn't have
 * access to); the central x86_jmp hook's op_omega_is_death gate is left in place unused for future
 * generalization (a construct whose true-exit-carrier IS reachable centrally, once one exists) but does NOT
 * currently double-fire here because role 2 is IR_MATCH_ARBNO phase 2, and phase 2's op_off resolves to
 * role-0's OWN node (drive_value_slot(own) in emit_drive's IR_MATCH_ARBNO case) — the central hook's
 * op_omega_is_death check does not know to call rt_zls_arbno_step1_load, so it would try to free r12 itself
 * (wrong pointer entirely) if ZC_SELFLOAD_ALLOC's x86_jmp branch were left enabled unconditionally. VERIFY
 * BEFORE TRUSTING COUNTS: this session's plan disables the central x86_jmp free path for this experiment (see
 * the follow-up edit) so only this template-level, carrier-aware free actually fires. */
static std::string x86_arbno_role2_free() {
    if (x86_selfload_mode() != ZC_SELFLOAD_ALLOC) return std::string();
    return x86("call", "rt_zls_arbno_step1_load", (uint64_t)(uintptr_t)(void *)(void * (*)(void))rt_zls_arbno_step1_load)
         + x86_align_enter()
         + x86("mov",  "rdi", "rax")
         + x86("call", "rt_zls_release", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_zls_release)
         + x86_align_leave();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ONE BODY, FLAVOR-INVISIBLE (Lon directive 2026-07-08 session 3, same removal as bb_match_arb.cpp — see the
 * fuller note there): the former ZC_PORT_ALLOC arms of roles 0/1/2 (state-in-ZLS2-block, hand-placed
 * push/load/pop) are DELETED; a template never branches on a flavor axis.  Under SCRIP_ZETA_PORT=2 the ZLS2
 * frame protocol is INJECTED at these roles' own port emissions by x86_port_hook (x86_asm.h), driven by
 * per-role zls2_geom grants (zeta_storage.c, slot = op_off+16, K = 32): role 0 BUMP|RESTORE — x86("def","α")
 * bumps a per-activation frame, x86("def","β") (EXTEND, a backtrack arrival: everything downstream has
 * failed, and v1 never re-opens completed iterations, so nothing below this frame is live) restores the
 * cursor to it; role 1 no grant (forward-flow landing, its jmp ω is the internal alias to F); role 2 RELEASE —
 * its single x86("jmp","ω") is STATICALLY this construct's true death (role knowledge, NOT the
 * op_omega_is_death chain classifier the L(9) note below records as broken), so the hook releases role 0's
 * frame there (unchain + release_to(block+K), which also reclaims any stray body frames below it).  The
 * activation STATE stays in the static graph-frame quad in every mode; the state-in-block re-entrancy design
 * retires with the deleted arms (nothing exercisable lost — same-node nesting needs DP-7, the recorded HONEST
 * SCOPE caveat).  The v1 quad hazard under re-entry remains exactly the documented PLAIN-mode ARBNO casualty
 * it always was.  x86_zls2_push_call/pop_call remain in x86_asm.h — the hook's arms are their callers now. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZB-5 SN4-PAT ARBNO.  Six roles share IR_MATCH_ARBNO; _.op_phase = IR_LIT.ival (NAMING NOTE, Lon 2026-07-05:
 * "phase" is a misnomer — this is a box-ROLE discriminator; rename is a future housekeeping rung).
 * v1 (deterministic body, roles 0/1/2): _.op_off = the role-0 node's zls slot (16B ZK_RAW: +0 entry δ,
 * +4 last-yield δ, +8 cur_before, +12 pad).
 *   role 0 (G, generator) — α: entry=δ, yield=δ, jmp γ (the null yield; SPITBOL shortest-first).
 *                           β: δ=yield (restore — the right neighbour may have consumed cursor before
 *                           failing), cur_before=δ, jmp ω — G's ω is the BODY-ENTRY edge, repurposed as the
 *                           β-continuation; the construct's real fail exit lives on role 2.
 *   role 1 (K, ok)        — body-success landing: δ==cur_before → jmp ω (=F, the 4/28 zero-advance rule),
 *                           else yield=δ, jmp γ (yield one more iteration).
 *   role 2 (F, exhaust)   — δ=entry, jmp ω (outer fail).  Defines a β ALIAS: body leaves stamp their fail
 *                           edges β-wards (sno_ω_to + generator-kind opcode), so both entries land here.
 * v2 (generator body, roles 3/4/5): per-iteration COLLECTION (ARCH-ZETA-LOCAL-STORAGE.md section 5f).
 * _.op_off = the role-3 node's 32B owner quad {+0 entry, +4 yield, +8 i, +12 cap, +16 ptr, +24 pad};
 * _.op_sa = min_off (the body subgraph's lowest granted slot offset); _.op_sb = elem_sz (16B header
 * {+0 prev_rZ, +8 cur_before zero-extended} + the body's contiguous slot range, 16-rounded).  While a body
 * iteration runs, rZ (r12) is REPOINTED to elem+16-min_off so body boxes' [r12+off] land inside the
 * element — the header is then readable at [r12+min_off-16]/[r12+min_off-8].  RELOAD LAW: the element
 * address is recomputed from (ptr,i) at every owner port, never cached across a push.
 *   role 3 (G) — α: entry=δ, yield=δ, i=0 (ptr/cap PERSIST for reuse across anchor retries), jmp γ (null
 *                yield).  β (EXTEND): δ=yield; rt_zcol_push(&ptr,&cap,i,elem_sz) → zeroed element i;
 *                write prev_rZ + cur_before into its header; repoint rZ; jmp ω (= body entry, as v1).
 *   role 4 (K) — body-success landing, runs REPOINTED: read cur_before + prev_rZ from the header, restore
 *                rZ; δ==cur_before → jmp ω (=F.α: a zero-advance WAY is refused, resume THIS element's
 *                body β for its next way); else i++, yield=δ, jmp γ.
 *   role 5 (F) — α (from K zero-advance, rZ already restored, i uncounted = the element's index): resume
 *                element i.  β (body-fail, runs REPOINTED): restore rZ from the header; i==0 → exhaust
 *                (δ=entry, jmp ω = outer fail); else i-- and resume element i.  Resume = recompute
 *                elem=ptr+i*elem_sz, repoint rZ, jmp γ — F.γ is stamped β-wards at the body's
 *                first-allocated (rightmost) leaf, the body's resume surface. */
std::string bb_match_arbno() {
    x86_begin();
    if (getenv("SCRIP_ARBNO_PHASE_TRACE")) fprintf(stderr, "[ARBNO-PHASE] op_phase=%d op_off=%d op_sa=%d op_sb=%d\n", (int)_.op_phase, _.op_off, _.op_sa, _.op_sb);
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0) return x86("def", "α") + x86_bomb("IR_MATCH_ARBNO: cursor slot not granted (zls)");
    if ((int)_.op_phase == 0) {
        return x86("comment", "IR_MATCH_ARBNO gen")
             + x86("def",     "α")
             + x86_arbno_role0_alloc()
             + x86("mov", FR(_.op_off), "r14d")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("mov", "r14d", FR(_.op_off + 4))
             + x86("mov", FR(_.op_off + 8), "r14d")
             + x86("jmp", "ω");
    }
    if ((int)_.op_phase == 1) {
        return x86("comment", "IR_MATCH_ARBNO ok")
             + x86("def",     "α")
             + x86("mov", "eax", FR(_.op_off + 8))
             + x86("cmp", "r14d", "eax")
             + x86("je",  L(0))
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86("jmp", "γ")
             + x86("def", L(0))
             + x86("jmp", "ω");
    }
    if ((int)_.op_phase == 2) {
        return x86("comment", "IR_MATCH_ARBNO exhaust")
             + x86("def",     "α")
             + x86("def", "β")
             + x86("mov", "r14d", FR(_.op_off))
             + x86_arbno_role2_free()
             /* BB-OWNED-ζ STEP 1, SINGLE-EXIT LABEL (Claude, this session, per Lon's ask): L(9) marks the ONE
              * instruction address that is this ARBNO node's provable true death -- the jmp immediately
              * following it is the same "jmp ω" every other true-exit uses (the port CONTRACT is unchanged:
              * this activation's real exit still goes through omega, same as any other node's), but this def
              * gives anything auditing the emitted stream (a disassembler, a future central hook, a person)
              * one unambiguous, greppable address for "ARBNO role 2 died here" that does NOT depend on the
              * op_omega_is_death chain-membership classification -- which this session found DOES NOT
              * correctly distinguish role 2's death from role 0/1's internal omega-reuse in at least one real
              * case (IR_MATCH_HEAD landing inside the same codegen_flat_chain_body window as this ARBNO's own
              * roles, making omega_resolved=1 for role 2 too, contradicting the STANDING POINTER's own claim
              * that only role 2/5 sets the flag -- see SCRIP_OMEGA_DIAG trace this session, emit.cpp ~1514).
              * No new port is added (RULES.md: FOUR PORTS = FOUR GREEK NAMES ALWAYS, no synonyms) -- this is
              * an ordinary local integer label via the existing L(n) mechanism (same as L(0) in phase 1 and in
              * bb_match_arb.cpp), not a fifth port; the jmp target is still literally "ω". */
             + x86("def", L(9))
             + x86("jmp", "ω");
    }
    if (_.op_sa < 0 || _.op_sb <= 0) return x86("def", "α") + x86_bomb("IR_MATCH_ARBNO v2: COLLECTION geometry not staged (zls_arbno_geom)");
    if ((int)_.op_phase == 3)
        return x86("comment", "IR_MATCH_ARBNO2 gen")
             + x86("def",     "α")
             + x86("mov", FR(_.op_off), "r14d")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86("mov", FR(_.op_off + 8), 0L)
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("mov", "r14d", FR(_.op_off + 4))
             + x86("lea", "rdi", FR(_.op_off + 16))
             + x86("lea", "rsi", FR(_.op_off + 12))
             + x86("mov", "edx", FR(_.op_off + 8))
             + x86("mov", "rcx", (long)_.op_sb)
             + x86_align_enter()
             + x86("call", "rt_zcol_push", (uint64_t)(uintptr_t)(void *)(void * (*)(void **, int *, int, long))rt_zcol_push)
             + x86_align_leave()
             + x86("mov", RDQ("rax", 0), x86_zr())
             + x86("mov", "ecx", "r14d")
             + x86("mov", RDQ("rax", 8), "rcx")
             + x86("lea", x86_zr(), RDQ("rax", 16 - _.op_sa))
             + x86("jmp", "ω");
    if ((int)_.op_phase == 4)
        return x86("comment", "IR_MATCH_ARBNO2 ok")
             + x86("def",     "α")
             + x86("mov", "eax", FR(_.op_sa - 8))
             + x86("mov", "rdx", FRQ(_.op_sa - 16))
             + x86("cmp", "r14d", "eax")
             + x86("je",  L(0))
             + x86("mov", x86_zr(), "rdx")
             + x86("mov", "eax", FR(_.op_off + 8))
             + x86("add", "eax", 1L)
             + x86("mov", FR(_.op_off + 8), "eax")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86("jmp", "γ")
             + x86("def", L(0))
             + x86("mov", x86_zr(), "rdx")
             + x86("jmp", "ω");
    return x86("comment", "IR_MATCH_ARBNO2 pop/exhaust")
         + x86("def",     "α")
         + x86("jmp", L(1))
         + x86("def", "β")
         + x86("mov", "rdx", FRQ(_.op_sa - 16))
         + x86("mov", x86_zr(), "rdx")
         + x86("mov", "eax", FR(_.op_off + 8))
         + x86("test", "eax", "eax")
         + x86("jz",  L(2))
         + x86("sub", "eax", 1L)
         + x86("mov", FR(_.op_off + 8), "eax")
         + x86("def", L(1))
         + x86("mov", "rax", FRQ(_.op_off + 16))
         + x86("mov", "ecx", FR(_.op_off + 8))
         + x86("mov", "rdx", (long)_.op_sb)
         + x86("imul", "rcx", "rdx")
         + x86("add", "rax", "rcx")
         + x86("lea", x86_zr(), RDQ("rax", 16 - _.op_sa))
         + x86("jmp", "γ")
         + x86("def", L(2))
         /* BB-OWNED-ζ STEP 1 (Claude, this session): L(2) here is role 5's OWN single true-exit -- the i==0
          * unwind-complete case (reached only via the "jz L(2)" test above when the per-iteration COLLECTION
          * has been fully popped). Parallel to role 2's new L(9) marker above: this is the ALREADY-EXISTING
          * label for that reason, not a new one, since renumbering an in-use L(n) here would be pure noise.
          * v2 does NOT currently call a role2_free()-equivalent at this point -- v2's per-iteration state
          * lives in the rt_zcol_push COLLECTION (a different, already-working mechanism per this session's
          * earlier tracing), not the STEP 1 single-carrier alloc/free pair; wiring the two together, if ever
          * warranted, is separate future work, not attempted here. */
         + x86("mov", "r14d", FR(_.op_off))
         + x86("jmp", "ω");
}
