#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" void * rt_zcol_push(void ** ptr_cell, int * cap_cell, int i, long elem_sz);
#include "x86_asm.h"
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
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0) return x86_bomb("IR_MATCH_ARBNO: cursor slot not granted (zls)");
    if ((int)_.op_phase == 0)
        return x86("comment", "IR_MATCH_ARBNO gen")
             + x86("label",   _.lbl_α)
             + x86("mov", FR(_.op_off), "r14d")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("mov", "r14d", FR(_.op_off + 4))
             + x86("mov", FR(_.op_off + 8), "r14d")
             + x86("jmp", "ω");
    if ((int)_.op_phase == 1)
        return x86("comment", "IR_MATCH_ARBNO ok")
             + x86("label",   _.lbl_α)
             + x86("mov", "eax", FR(_.op_off + 8))
             + x86("cmp", "r14d", "eax")
             + x86("je",  L(0))
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86("jmp", "γ")
             + x86("def", L(0))
             + x86("jmp", "ω");
    if ((int)_.op_phase == 2)
        return x86("comment", "IR_MATCH_ARBNO exhaust")
             + x86("label",   _.lbl_α)
             + x86("def", "β")
             + x86("mov", "r14d", FR(_.op_off))
             + x86("jmp", "ω");
    if (_.op_sa < 0 || _.op_sb <= 0) return x86_bomb("IR_MATCH_ARBNO v2: COLLECTION geometry not staged (zls_arbno_geom)");
    if ((int)_.op_phase == 3)
        return x86("comment", "IR_MATCH_ARBNO2 gen")
             + x86("label",   _.lbl_α)
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
             + x86("push", "rbp")
             + x86("mov",  "rbp", "rsp")
             + x86("and",  "rsp", -16L)
             + x86("call", "rt_zcol_push", (uint64_t)(uintptr_t)(void *)(void * (*)(void **, int *, int, long))rt_zcol_push)
             + x86("mov",  "rsp", "rbp")
             + x86("pop",  "rbp")
             + x86("mov", RDQ("rax", 0), "r12")
             + x86("mov", "ecx", "r14d")
             + x86("mov", RDQ("rax", 8), "rcx")
             + x86("lea", "r12", RDQ("rax", 16 - _.op_sa))
             + x86("jmp", "ω");
    if ((int)_.op_phase == 4)
        return x86("comment", "IR_MATCH_ARBNO2 ok")
             + x86("label",   _.lbl_α)
             + x86("mov", "eax", FR(_.op_sa - 8))
             + x86("mov", "rdx", FRQ(_.op_sa - 16))
             + x86("cmp", "r14d", "eax")
             + x86("je",  L(0))
             + x86("mov", "r12", "rdx")
             + x86("mov", "eax", FR(_.op_off + 8))
             + x86("add", "eax", 1L)
             + x86("mov", FR(_.op_off + 8), "eax")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86("jmp", "γ")
             + x86("def", L(0))
             + x86("mov", "r12", "rdx")
             + x86("jmp", "ω");
    return x86("comment", "IR_MATCH_ARBNO2 pop/exhaust")
         + x86("label",   _.lbl_α)
         + x86("jmp", L(1))
         + x86("def", "β")
         + x86("mov", "rdx", FRQ(_.op_sa - 16))
         + x86("mov", "r12", "rdx")
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
         + x86("lea", "r12", RDQ("rax", 16 - _.op_sa))
         + x86("jmp", "γ")
         + x86("def", L(2))
         + x86("mov", "r14d", FR(_.op_off))
         + x86("jmp", "ω");
}
