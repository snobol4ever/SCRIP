#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int prolog_atom_intern(const char *);
void rt_pl_tr_refuse(const char *);
int rt_pl_unify_const_cold(const DESCR_t *, int64_t, int);
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
std::string bb_unify_const() {
    x86_begin();
    IR_t * nd = _.node;
    if (_.op_zres) return x86_alpha() + x86_bomb("bb_unify_const: a Prolog head is a flat-frame graph; no ZD arm exists") + x86_beta_trampoline();
    IR_t * src = (nd && nd->n_operands > 0) ? nd->operands[0] : (IR_t *)0;
    IR_t * k = (nd && nd->n_operands > 1) ? nd->operands[1] : (IR_t *)0;
    if (!src || !k || src->op != IR_VAR_REF || !IR_LIT(src).sval || (k->op != IR_LIT_ATOM && k->op != IR_LIT_INTEGER))
        return x86_alpha() + x86_bomb("bb_unify_const: operand 0 must be a named IR_VAR_REF and operand 1 an IR_LIT_ATOM or IR_LIT_INTEGER") + x86_beta_trampoline();
    int voff = bb_varslot_peek(IR_LIT(src).sval);
    if (voff < 0) return x86_alpha() + x86_bomb("bb_unify_const: the variable has no frame slot") + x86_beta_trampoline();
    int atom = (k->op == IR_LIT_ATOM);
    long ktag = atom ? (long)DT_PLATOM : (long)DT_I;
    uint64_t kval = atom ? (uint64_t)(unsigned)prolog_atom_intern(IR_LIT(k).sval ? IR_LIT(k).sval : "") : (uint64_t)IR_LIT(k).ival;
    std::string s = x86("comment", std::string("IR_UNIFY_CONST ") + pl_safe_text(IR_LIT(src).sval) + " = " + (atom ? pl_safe_text(IR_LIT(k).sval) : std::to_string((long long)IR_LIT(k).ival))
                         + ": deref the cell, compare tag and value, or bind the constant (trail test and push inline)")
                  + x86_alpha()
                  + x86("lea", "rdi", FRQ(voff))
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
