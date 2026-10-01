#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int prolog_atom_intern(const char *);
int prolog_functor_arity(int);
void rt_pl_tr_refuse(const char *);
int rt_pl_unify_const_cold(const DESCR_t *, int64_t, int);
DESCR_t rt_pl_unify_struct_fresh(long);
int rt_pl_unify_value(DESCR_t *, DESCR_t *);
int bb_slot_get(IR_t * nd);
}
#include "rt/rt_pl_trail.h"
#include "x86_asm.h"
static_assert(X86_PL_TR_ENTRY_BYTES == PL_TR_ENTRY_BYTES, "pl_bind_rdi spells the trail entry size as a literal");
static_assert(X86_PL_TR_ARENA_MASK == -(long)PL_TR_ARENA_BYTES, "pl_bind_rdi spells the trail arena mask as a literal");
static_assert(PL_TR_FRAME_HI_OFF == 32, "pl_bind_rdi reads the youngest choice's frame top at [B + 32]");
static_assert(offsetof(pl_tr_entry_t, cell) == 0 && offsetof(pl_tr_entry_t, pad) == 8 && offsetof(pl_tr_entry_t, old) == 16, "pl_bind_rdi writes the trail entry as {cell, pad, old}");
static_assert(offsetof(VCELL_t, cellp) == 0, "pl_deref_rdi follows a DT_N slen 2 name through the VCELL's first word");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_deref_rdi(int l0, int lvar, int ln2, int ldone) {
    return x86("note", "deref: follow a name (slen 1: the cell; slen 2: the VCELL's cell) or a bound PLVAR to the cell that holds the value; eax = its tag word")
         + x86("def", L(l0))
         + x86("mov", "eax", RDD("rdi", 0))
         + x86("cmp", "al", (long)DT_PLVAR)
         + x86("je", L(lvar))
         + x86("cmp", "al", (long)DT_N)
         + x86("jne", L(ldone))
         + x86("mov", "rsi", RDQ("rdi", 8))
         + x86("test", "rsi", "rsi")
         + x86("jz", L(ldone))
         + x86("mov", "ecx", RDD("rdi", 4))
         + x86("cmp", "ecx", 1L)
         + x86("jne", L(ln2))
         + x86("cmp", "rsi", "rdi")
         + x86("je", L(ldone))
         + x86("mov", "rdi", "rsi")
         + x86("jmp", L(l0))
         + x86("def", L(ln2))
         + x86("cmp", "ecx", 2L)
         + x86("jne", L(ldone))
         + x86("mov", "rsi", RDQ("rsi", 0))
         + x86("test", "rsi", "rsi")
         + x86("jz", L(ldone))
         + x86("mov", "rdi", "rsi")
         + x86("jmp", L(l0))
         + x86("def", L(lvar))
         + x86("mov", "rsi", RDQ("rdi", 8))
         + x86("test", "rsi", "rsi")
         + x86("jz", L(ldone))
         + x86("cmp", "rsi", "rdi")
         + x86("je", L(ldone))
         + x86("mov", "rdi", "rsi")
         + x86("jmp", L(l0))
         + x86("def", L(ldone));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_trail_rdi(int lstore, int lrefuse) {
    return x86("note", "trail test: no choice (B = r13 = 0) records nothing; a cell below rsp (the heap) or at or above the youngest choice's frame top [B + 32] is older than the choice")
         + x86("test", "r13", "r13")
         + x86("jz", L(lstore))
         + x86("cmp", "rdi", "rsp")
         + x86("jbe", L(lstore + 1))
         + x86("mov", "rax", RDQ("r13", PL_TR_FRAME_HI_OFF))
         + x86("cmp", "rdi", "rax")
         + x86("jb", L(lstore))
         + x86("def", L(lstore + 1))
         + x86("note", "the push: {cell, 0, old} on the r12 trail, the arena's top word synced for the collector")
         + x86("mov", "rax", "r12")
         + x86("and", "rax", (long)(PL_TR_ARENA_BYTES - 1))
         + x86("cmp", "rax", (long)(PL_TR_ARENA_BYTES - PL_TR_ENTRY_BYTES))
         + x86("jae", L(lrefuse))
         + x86("mov", "rax", RDQ("rdi", 0))
         + x86("mov", "rdx", RDQ("rdi", 8))
         + x86("mov", RDQ("r12", 0), "rdi")
         + x86("mov", RDQ("r12", 8), 0L)
         + x86("mov", RDQ("r12", 16), "rax")
         + x86("mov", RDQ("r12", 24), "rdx")
         + x86("add", "r12", X86_PL_TR_ENTRY_BYTES)
         + x86_pl_tr_top_sync()
         + x86("def", L(lstore));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_trail_refuse(int lrefuse) {
    return x86("def", L(lrefuse))
         + x86("mov", "rdi", "r12")
         + x86("call", "rt_pl_tr_refuse", (uint64_t)(uintptr_t)(void *)rt_pl_tr_refuse);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_safe_text(const char * t) {
    std::string o; for (const char * p = t ? t : ""; *p && o.size() < 32; p++) o += (*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '_' ? *p : '?';
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_src_ok(IR_t * nd, IR_t ** src, long * idx) {
    if (!nd || nd->n_operands < 2) return 0;
    *src = nd->operands[0];
    { IR_t * ix = nd->operands[1]; if (!*src || !ix || ix->op != IR_LIT_INTEGER) return 0; *idx = (long)IR_LIT(ix).ival; if (*idx < 0) return 0; }
    if ((*src)->op == IR_VAR_REF) return IR_LIT(*src).sval && bb_varslot_peek(IR_LIT(*src).sval) >= 0;
    if ((*src)->op == IR_UNIFY_STRUCT) return bb_slot_get(*src) >= 0;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_src_rdi(IR_t * src, long idx) {
    if (src->op == IR_VAR_REF) return x86("lea", "rdi", FRQ(bb_varslot_peek(IR_LIT(src).sval)));
    return x86("note", "the cell is child " + std::to_string((long long)idx) + " of the parent box's compound: [parent.p + 16j]")
         + x86("mov", "rax", FRQ(bb_slot_get(src) + 8))
         + x86("lea", "rdi", RDQ("rax", 16 * idx));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_unbound_rdi(int lbind, int lbound) {
    return x86("note", "unbound after the deref: the zero DESCR, DT_FAIL, a self PLVAR (the deref stopped on it) or a self name")
         + x86("cmp", "al", 0L)
         + x86("je", L(lbind))
         + x86("cmp", "al", (long)DT_FAIL)
         + x86("je", L(lbind))
         + x86("cmp", "al", (long)DT_PLVAR)
         + x86("je", L(lbind))
         + x86("cmp", "al", (long)DT_N)
         + x86("jne", L(lbound))
         + x86("mov", "rsi", RDQ("rdi", 8))
         + x86("cmp", "rsi", "rdi")
         + x86("je", L(lbind))
         + x86("def", L(lbound));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_unify_const() {
    x86_begin();
    IR_t * nd = _.node;
    if (_.op_zres) return x86_alpha() + x86_bomb("bb_unify_const: a Prolog head is a flat-frame graph; no ZD arm exists") + x86_beta_trampoline();
    IR_t * src = (IR_t *)0; long idx = 0;
    IR_t * k = (nd && nd->n_operands > 2) ? nd->operands[2] : (IR_t *)0;
    if (!pl_src_ok(nd, &src, &idx) || !k || (k->op != IR_LIT_ATOM && k->op != IR_LIT_INTEGER))
        return x86_alpha() + x86_bomb("bb_unify_const: operands are a cell source (a named IR_VAR_REF with a frame slot, or a driven parent IR_UNIFY_STRUCT), a child index and a literal")
             + x86_beta_trampoline();
    int atom = (k->op == IR_LIT_ATOM);
    long ktag = atom ? (long)DT_PLATOM : (long)DT_I;
    uint64_t kval = atom ? (uint64_t)(unsigned)prolog_atom_intern(IR_LIT(k).sval ? IR_LIT(k).sval : "") : (uint64_t)IR_LIT(k).ival;
    std::string lhs = src->op == IR_VAR_REF ? pl_safe_text(IR_LIT(src).sval) : "child" + std::to_string((long long)idx);
    std::string what = lhs + " = " + (atom ? pl_safe_text(IR_LIT(k).sval) : std::to_string((long long)IR_LIT(k).ival));
    std::string s = x86("comment", std::string("IR_UNIFY_CONST ") + what + ": deref the cell, compare tag and value, or bind the constant (trail test and push inline)")
                  + x86_alpha()
                  + pl_src_rdi(src, idx)
                  + pl_deref_rdi(10, 11, 12, 13)
                  + x86("cmp", "al", ktag)
                  + x86("jne", L(20));
    if (!atom) s += x86("mov", "ecx", RDD("rdi", 4)) + x86("test", "ecx", "ecx") + x86("jne", L(40));
    s += x86_movabs_r64("rax", kval)
       + x86("mov", "rsi", RDQ("rdi", 8))
       + x86("cmp", "rsi", "rax")
       + x86_omega("jne")
       + x86_gamma()
       + x86("def", L(20))
       + x86("note", "unbound: the zero DESCR, DT_FAIL, a self PLVAR (the deref stopped on it) or a self name")
       + x86("cmp", "al", 0L)
       + x86("je", L(30))
       + x86("cmp", "al", (long)DT_FAIL)
       + x86("je", L(30))
       + x86("cmp", "al", (long)DT_PLVAR)
       + x86("je", L(30))
       + x86("cmp", "al", (long)DT_N)
       + x86("jne", L(25))
       + x86("mov", "rsi", RDQ("rdi", 8))
       + x86("cmp", "rsi", "rdi")
       + x86("je", L(30))
       + x86_omega()
       + x86("def", L(25));
    if (atom) s += x86("cmp", "al", (long)DT_S) + x86("je", L(40)) + x86_omega();
    else s += x86("cmp", "al", (long)DT_PLREF) + x86_omega("je") + x86("cmp", "al", (long)DT_PLATOM) + x86_omega("je") + x86("cmp", "al", (long)DT_S) + x86_omega("je") + x86("jmp", L(40));
    s += x86("def", L(30))
       + pl_trail_rdi(31, 39)
       + x86("mov", RDQ("rdi", 0), ktag)
       + x86_movabs_r64("rax", kval)
       + x86("mov", RDQ("rdi", 8), "rax")
       + x86_gamma()
       + x86("def", L(40))
       + x86("note", "cold: a legacy DT_S atom text, or a number of another kind, compared by the value service; it never binds")
       + x86_movabs_r64("rsi", kval)
       + x86("mov", "edx", (long)atom)
       + x86("call", "rt_pl_unify_const_cold", (uint64_t)(uintptr_t)(void *)rt_pl_unify_const_cold)
       + x86_rt_gc_poll()
       + x86("test", "eax", "eax")
       + x86_omega("je")
       + x86_gamma()
       + pl_trail_refuse(39)
       + x86_beta_trampoline();
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_unify_struct() {
    x86_begin();
    IR_t * nd = _.node;
    if (_.op_zres) return x86_alpha() + x86_bomb("bb_unify_struct: a Prolog head is a flat-frame graph; no ZD arm exists") + x86_beta_trampoline();
    IR_t * src = (IR_t *)0; long idx = 0;
    IR_t * f = (nd && nd->n_operands > 2) ? nd->operands[2] : (IR_t *)0;
    if (!pl_src_ok(nd, &src, &idx) || !f || f->op != IR_LIT_INTEGER)
        return x86_alpha() + x86_bomb("bb_unify_struct: operands are a cell source, a child index and an IR_LIT_INTEGER functor id") + x86_beta_trampoline();
    long fid = (long)IR_LIT(f).ival; int off = _.op_off;
    if (off < 0) return x86_alpha() + x86_bomb("bb_unify_struct: no result slot granted for the compound DESCR") + x86_beta_trampoline();
    if (prolog_functor_arity((int)fid) < 1) return x86_alpha() + x86_bomb("bb_unify_struct: the functor id names no compound functor") + x86_beta_trampoline();
    std::string what = std::string("functor#") + std::to_string((long long)fid) + " against " + (src->op == IR_VAR_REF ? pl_safe_text(IR_LIT(src).sval) : "child" + std::to_string((long long)idx));
    return x86("comment", "IR_UNIFY_STRUCT " + what + ": read mode keeps the compound DESCR in this box's own slot for its children; write mode takes a fresh block and binds the cell to it")
         + x86_alpha()
         + pl_src_rdi(src, idx)
         + pl_deref_rdi(10, 11, 12, 13)
         + x86("cmp", "al", (long)DT_PLREF)
         + x86("jne", L(20))
         + x86("mov", "ecx", RDD("rdi", 4))
         + x86("cmp", "ecx", fid)
         + x86_omega("jne")
         + x86("mov", "rax", RDQ("rdi", 0))
         + x86("mov", "rdx", RDQ("rdi", 8))
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86_gamma()
         + x86("def", L(20))
         + pl_unbound_rdi(30, 25)
         + x86_omega()
         + x86("def", L(30))
         + x86("note", "write mode: the fresh block of self-referencing cells comes from a value service (it allocates, so the DESCR is stored into the mapped slot and the poll follows)")
         + x86("note", "the cell is re-derived after the poll and bound to the block")
         + x86("mov32", "edi", fid)
         + x86("call", "rt_pl_unify_struct_fresh", (uint64_t)(uintptr_t)(void *)rt_pl_unify_struct_fresh)
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86_rt_gc_poll()
         + pl_src_rdi(src, idx)
         + pl_deref_rdi(14, 15, 16, 17)
         + pl_trail_rdi(31, 39)
         + x86("mov", "rax", FRQ(off))
         + x86("mov", "rdx", FRQ(off + 8))
         + x86("mov", RDQ("rdi", 0), "rax")
         + x86("mov", RDQ("rdi", 8), "rdx")
         + x86_gamma()
         + pl_trail_refuse(39)
         + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_unify_first() {
    x86_begin();
    IR_t * nd = _.node;
    if (_.op_zres) return x86_alpha() + x86_bomb("bb_unify_first: a Prolog head is a flat-frame graph; no ZD arm exists") + x86_beta_trampoline();
    IR_t * src = (IR_t *)0; long idx = 0;
    const char * vn = nd ? IR_LIT(nd).sval : (const char *)0;
    if (!pl_src_ok(nd, &src, &idx) || !vn)
        return x86_alpha() + x86_bomb("bb_unify_first: operand 0 is the cell source, the node's ival the child index and its sval the variable's name") + x86_beta_trampoline();
    int vo = bb_varslot_peek(vn);
    if (vo < 0) return x86_alpha() + x86_bomb("bb_unify_first: the variable has no frame slot") + x86_beta_trampoline();
    return x86("comment", std::string("IR_UNIFY_FIRST ") + pl_safe_text(vn) + " takes child" + std::to_string((long long)idx)
                            + ": a bound value is copied into the variable's slot, an unbound cell is referenced from it")
         + x86_alpha()
         + pl_src_rdi(src, idx)
         + pl_deref_rdi(10, 11, 12, 13)
         + pl_unbound_rdi(30, 25)
         + x86("mov", "rax", RDQ("rdi", 0))
         + x86("mov", "rdx", RDQ("rdi", 8))
         + x86("mov", FRQ(vo), "rax")
         + x86("mov", FRQ(vo + 8), "rdx")
         + x86_gamma()
         + x86("def", L(30))
         + x86("mov", "rax", (long)DT_PLVAR)
         + x86("mov", FRQ(vo), "rax")
         + x86("mov", FRQ(vo + 8), "rdi")
         + x86_gamma()
         + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_unify_value() {
    x86_begin();
    IR_t * nd = _.node;
    if (_.op_zres) return x86_alpha() + x86_bomb("bb_unify_value: a Prolog head is a flat-frame graph; no ZD arm exists") + x86_beta_trampoline();
    IR_t * src = (IR_t *)0; long idx = 0;
    const char * vn = nd ? IR_LIT(nd).sval : (const char *)0;
    if (!pl_src_ok(nd, &src, &idx) || !vn)
        return x86_alpha() + x86_bomb("bb_unify_value: operand 0 is the cell source, the node's ival the child index and its sval the variable's name") + x86_beta_trampoline();
    int vo = bb_varslot_peek(vn);
    if (vo < 0) return x86_alpha() + x86_bomb("bb_unify_value: the variable has no frame slot") + x86_beta_trampoline();
    return x86("comment", std::string("IR_UNIFY_VALUE ") + pl_safe_text(vn) + " = child" + std::to_string((long long)idx)
                            + ": an unbound side binds to the other inline, two atomics compare inline, anything else goes to the general-unify leaf")
         + x86_alpha()
         + pl_src_rdi(src, idx)
         + pl_deref_rdi(10, 11, 12, 13)
         + x86("mov", "r8", "rdi")
         + x86("lea", "rdi", FRQ(vo))
         + pl_deref_rdi(14, 15, 16, 17)
         + x86("mov", "rsi", "rdi")
         + x86("mov", "rdi", "r8")
         + x86("note", "rdi = the cell, rsi = the variable's cell, both dereferenced; eax = the cell's tag, ecx = the variable's")
         + x86("mov", "eax", RDD("rdi", 0))
         + x86("and", "eax", 255L)
         + x86("mov", "ecx", RDD("rsi", 0))
         + x86("and", "ecx", 255L)
         + x86("cmp", "eax", 0L)
         + x86("je", L(40))
         + x86("cmp", "eax", (long)DT_FAIL)
         + x86("je", L(40))
         + x86("cmp", "eax", (long)DT_PLVAR)
         + x86("je", L(40))
         + x86("cmp", "ecx", 0L)
         + x86("je", L(50))
         + x86("cmp", "ecx", (long)DT_FAIL)
         + x86("je", L(50))
         + x86("cmp", "ecx", (long)DT_PLVAR)
         + x86("je", L(50))
         + x86("note", "both bound: two atoms or two small integers compare inline, everything else is the leaf's")
         + x86("cmp", "eax", "ecx")
         + x86("jne", L(60))
         + x86("cmp", "eax", (long)DT_PLATOM)
         + x86("je", L(45))
         + x86("cmp", "eax", (long)DT_I)
         + x86("jne", L(60))
         + x86("mov", "edx", RDD("rdi", 4))
         + x86("mov", "ecx", RDD("rsi", 4))
         + x86("or", "edx", "ecx")
         + x86("jne", L(60))
         + x86("def", L(45))
         + x86("mov", "rax", RDQ("rdi", 8))
         + x86("mov", "rdx", RDQ("rsi", 8))
         + x86("cmp", "rax", "rdx")
         + x86_omega("jne")
         + x86_gamma()
         + x86("def", L(40))
         + x86("note", "the cell is unbound: a bound variable binds it inline; two unbound cells are the leaf's (it orders the binding)")
         + x86("cmp", "ecx", 0L)
         + x86("je", L(60))
         + x86("cmp", "ecx", (long)DT_FAIL)
         + x86("je", L(60))
         + x86("cmp", "ecx", (long)DT_PLVAR)
         + x86("je", L(60))
         + x86("cmp", "ecx", (long)DT_N)
         + x86("je", L(60))
         + x86("mov", "r8", "rsi")
         + pl_trail_rdi(31, 39)
         + x86("mov", "rax", RDQ("r8", 0))
         + x86("mov", "rdx", RDQ("r8", 8))
         + x86("mov", RDQ("rdi", 0), "rax")
         + x86("mov", RDQ("rdi", 8), "rdx")
         + x86_gamma()
         + x86("def", L(50))
         + x86("note", "the variable is unbound and the cell is bound: bind the variable's cell to the value")
         + x86("mov", "r8", "rdi")
         + x86("mov", "rdi", "rsi")
         + pl_trail_rdi(33, 39)
         + x86("mov", "rax", RDQ("r8", 0))
         + x86("mov", "rdx", RDQ("r8", 8))
         + x86("mov", RDQ("rdi", 0), "rax")
         + x86("mov", RDQ("rdi", 8), "rdx")
         + x86_gamma()
         + x86("def", L(60))
         + x86("note", "the general-unify leaf: rtx_pl_unify in R4.3; today the ctx veneer over plw_unify_cells, which can allocate through plw_bind's boxing, so it is polled")
         + x86("call", "rt_pl_unify_value", (uint64_t)(uintptr_t)(void *)rt_pl_unify_value)
         + x86_rt_gc_poll()
         + x86("test", "eax", "eax")
         + x86_omega("je")
         + x86_gamma()
         + pl_trail_refuse(39)
         + x86_beta_trampoline();
}
