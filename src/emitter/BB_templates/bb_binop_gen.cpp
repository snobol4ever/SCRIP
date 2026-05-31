/* bb_binop_gen.cpp — BB template for IR_BINOP_GEN (generative arith/relop, cross-product).
   ICN-T-3 / ICN-M4 (2026-05-27), GOAL-ICON-BB. One file per BB kind per RULES.md.
   x86 only — IS_JVM/JS/NET/WASM arms stub (RULES.md: x86 only for now).

   IR_BINOP_GEN: nd->ival = BinopKind; nd->α = lhs operand box, nd->β = rhs operand box.
   Both operands may be generators; this box yields the full cross-product (mode-2 oracle:
   bb_exec.c case IR_BINOP_GEN, lines ~688-762). The odometer:
     state 0 (fresh, α): seed lhs (α-entry) → hold lv; seed rhs (α-entry) → hold rv; apply.
     state 1 (active, β): advance rhs (β-entry); on rhs.ω → advance lhs (β), reset rhs (α-entry),
                          re-seed rv; on lhs.ω → parent ω. Then apply.
     apply: push lv, push rv; rt_arith (arithmetic) or rt_acomp (relop); on relop-fail → advance
            and retry; on success → parent γ. (DT_I round-trip: the gate's seed generators all
            yield integers; real/string operand generality is a documented ICN-M4 follow-on.)

   ICN-XA-1: the α/β operand boxes are emitted INLINE here via walk_bb_node_str_c (the string
   variant of walk_bb_node), keeping this _str() body pure — zero emit_text_n mid-body. Every byte
   still originates in the keyed child template (bb_to etc.) reached through emit_core dispatch. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "emit_core.h"
#include "SM.h"
#include "ast.h"
#include "../runtime/interp/box_rt.h"
void rt_arith(int op);
void rt_acomp(int op);
int  rt_last_ok(void);
void rt_pop_void(void);
void rt_pop_store_i64(int64_t *slot);
void rt_push_stored_i64(const int64_t *slot);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * binop_op_name(int64_t op) {
    switch (op) {
    case BINOP_ADD: return "+";  case BINOP_SUB: return "-";  case BINOP_MUL: return "*";
    case BINOP_DIV: return "/";  case BINOP_MOD: return "%";  case BINOP_POW: return "^";
    case BINOP_LT:  return "<";  case BINOP_LE:  return "<="; case BINOP_GT:  return ">";
    case BINOP_GE:  return ">="; case BINOP_EQ:  return "=";  case BINOP_NE:  return "~=";
    default: return "?";
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Map BinopKind → is_relop + the integer arg the runtime helper expects.                          */
/* arithmetic (rt_arith): SM_ADD/SUB/MUL/DIV/MOD/EXP.   relop (rt_acomp): TT_LT/LE/GT/GE/EQ/NE.       */
static int binop_is_relop(int64_t op) {
    return (op >= BINOP_LT && op <= BINOP_NE);
}
static int binop_runtime_arg(int64_t op) {
    switch (op) {
    case BINOP_ADD: return (int)SM_ADD;  case BINOP_SUB: return (int)SM_SUB;
    case BINOP_MUL: return (int)SM_MUL;  case BINOP_DIV: return (int)SM_DIV;
    case BINOP_MOD: return (int)SM_MOD;  case BINOP_POW: return (int)SM_EXP;
    case BINOP_LT:  return (int)TT_LT;   case BINOP_LE:  return (int)TT_LE;
    case BINOP_GT:  return (int)TT_GT;   case BINOP_GE:  return (int)TT_GE;
    case BINOP_EQ:  return (int)TT_EQ;   case BINOP_NE:  return (int)TT_NE;
    default: return (int)SM_ADD;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Emit one child operand box inline, with caller-chosen γ/ω/β/α label names. Returns the box's x86   */
/* (TEXT) as a string via walk_bb_node_str_c, after pointing g_emit.lbl_* at the four fresh labels.   */
static std::string emit_child_box(IR_t * child,
                                  bb_label_t * la, bb_label_t * lg, bb_label_t * lo, bb_label_t * lb) {
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
/* Is this operand a restartable generator box (emits four ports)? Mirrors bb_exec.c                  */
/* bb_is_gen_kind_raw, seeing through a IR_ASSIGN wrapper.                                             */
static int operand_is_gen(IR_t * e) {
    if (!e) return 0;
    if (e->t == IR_ASSIGN) return operand_is_gen(e->β);
    switch (e->t) {
    case IR_TO: case IR_TO_BY: case IR_UPTO: case IR_ALT:
    case IR_BINOP_GEN: case IR_ITERATE: case IR_LIMIT: case IR_PROC_GEN:
    case IR_LIST_BANG: case IR_KEY_GEN: case IR_FIND_GEN: case IR_SEQ_GEN:
        return 1;
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Single-shot (non-generator) integer operand: synthesize a four-port box with the SAME label names  */
/* the odometer expects (la/lg/lo/lb).  α: push the literal int via rt_push_int, jmp γ.  β: jmp ω      */
/* (single-shot — exhausted after one value).  Only IR_LIT_I is handled inline today; other single-   */
/* shot kinds (IR_VAR / IR_KEYWORD / non-int literals) are a documented ICN-M4 follow-on.  Returns ""  */
/* (with *ok=0) if the operand kind is not yet inline-emittable, so the caller can stub the port.      */
static std::string synth_single_shot_box(IR_t * e, int * ok,
                                         bb_label_t * la, bb_label_t * lg, bb_label_t * lo, bb_label_t * lb) {
    *ok = 0;
    IR_t * inner = e;
    while (inner && inner->t == IR_ASSIGN) inner = inner->β;
    if (!inner || inner->t != IR_LIT_I) return std::string();
    *ok = 1;
    long v = (long)inner->ival;
    return s_1asm(emit_fmt("%s:", la->name))
         + s_comment(emit_fmt("# single-shot operand LIT_I=%ld", v))
         + s_2asm("mov rdi,", emit_fmt("%ld", v))
         + s_2asm("call", "rt_push_int@PLT")
         + s_2asm("jmp", lg->name)
         + s_L1asm(std::string(lb->name) + ":", "")
         + s_2asm("jmp", lo->name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_binop_gen_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        const char * op = binop_op_name(pBB->ival);
        if (MEDIUM_MACRO_DEF)
            return s_comment("# no macro form — BINOP_GEN");
        if (MEDIUM_BINARY) {
            /* IBB-6 (Sonnet 4.6, 2026-05-28): store/restore/apply slab for IR_BINOP_GEN odometer.  */
            /* Receives five EMIT_PAIR entries from flat_drive_binop_gen_tree:                         */
            /*   pair[0] = DEF_JMP(lhs_store, lhs_seeded)  — defines lhs_store; saves lhs; jmps     */
            /*             lhs_seeded (= rhs slab α-entry) to re-seed rhs after lhs advances         */
            /*   pair[1] = DEF_JMP(rhs_store, rhs_store)   — defines rhs_store; saves rhs; falls     */
            /*             through into apply                                                          */
            /*   pair[2] = JMP(outer_γ)  — apply succeeded                                           */
            /*   pair[3] = JMP(rhs_β)    — relop failed: advance rhs                                  */
            /*   pair[4] = DEF_JMP(lbl_β, rhs_β)  — BINOP_GEN β-define, jmps rhs_β (re-pump)        */
            /*                                                                                         */
            /* Slab sections (addresses known at emit time; &pBB->counter = lhs cache,               */
            /* &pBB->value.i = rhs cache):                                                            */
            /*   lhs_store: movabs rdi,&pBB->counter; movabs rax,&rt_pop_store_i64; call rax (22)    */
            /*              jmp lhs_seeded (5)                                                         */
            /*   rhs_store: movabs rdi,&pBB->value.i; movabs rax,&rt_pop_store_i64; call rax (22)    */
            /*   apply:     movabs rdi,&pBB->counter; movabs rax,&rt_push_stored_i64; call rax (22)  */
            /*              movabs rdi,&pBB->value.i; movabs rax,&rt_push_stored_i64; call rax (22)  */
            /*              movabs rdi,rt_arg; movabs rax,&rt_arith/rt_acomp; call rax (22)           */
            /*              [arith: jmp outer_γ (5); β-def+jmp (10)]                                  */
            /*              [relop: movabs rax,&rt_last_ok (10); call rax (2); test eax,eax (2);      */
            /*                      jne outer_γ (6); movabs rax,&rt_pop_void (10); call rax (2);     */
            /*                      jmp rhs_β (5); β-def+jmp (10)]                                   */
            int n = g_emit.xa_bb_emit_pair_n;
            if (n < 5 || !pBB) {
                bin = { {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
                return bytes(1,"\xE9")+u32le(0)+bytes(1,"\xE9")+u32le(0);
            }
            bb_label_t *lhs_store  = g_emit.xa_bb_emit_pair_define[0];
            bb_label_t *lhs_seeded = g_emit.xa_bb_emit_pair_jmp[0];
            bb_label_t *rhs_store  = g_emit.xa_bb_emit_pair_define[1];
            /* pair[1] jmp is rhs_store itself (fall-through marker) — not a real jump target */
            bb_label_t *lbl_γ_out  = g_emit.xa_bb_emit_pair_jmp[2];
            bb_label_t *lbl_rhs_β  = g_emit.xa_bb_emit_pair_jmp[3];
            bb_label_t *lbl_β_def  = g_emit.xa_bb_emit_pair_define[4];
            bb_label_t *lbl_β_jmp  = g_emit.xa_bb_emit_pair_jmp[4];
            int is_relop = binop_is_relop(pBB->ival);
            int rt_arg   = binop_runtime_arg(pBB->ival);
            uint64_t cnt_ptr = (uint64_t)(uintptr_t)&pBB->counter;
            uint64_t val_ptr = (uint64_t)(uintptr_t)&pBB->value.i;
            uint64_t fn_store; { void (*fp)(int64_t*)       = rt_pop_store_i64;    fn_store  = (uint64_t)(uintptr_t)(void*)fp; }
            uint64_t fn_push;  { void (*fp)(const int64_t*) = rt_push_stored_i64;  fn_push   = (uint64_t)(uintptr_t)(void*)fp; }
            uint64_t fn_arith; { void (*fp)(int)            = rt_arith;             fn_arith  = (uint64_t)(uintptr_t)(void*)fp; }
            uint64_t fn_acomp; { void (*fp)(int)            = rt_acomp;             fn_acomp  = (uint64_t)(uintptr_t)(void*)fp; }
            uint64_t fn_lok;   { int  (*fp)(void)           = rt_last_ok;           fn_lok    = (uint64_t)(uintptr_t)(void*)fp; }
            uint64_t fn_pv;    { void (*fp)(void)           = rt_pop_void;          fn_pv     = (uint64_t)(uintptr_t)(void*)fp; }
            /* Helper: emit `movabs rdi, ptr ; movabs rax, fn ; call rax` (22 bytes). */
            auto call22 = [&](uint64_t ptr, uint64_t fn) -> std::string {
                return bytes(2,"\x48\xBF") + u64le(ptr)
                     + bytes(2,"\x48\xB8") + u64le(fn)
                     + bytes(2,"\xFF\xD0");
            };
            std::string b;
            /* lhs_store section. */
            int lhs_store_off = (int)b.size();
            b += call22(cnt_ptr, fn_store);           /* rt_pop_store_i64(&counter) */
            b += bytes(1,"\xE9") + u32le(0);          /* jmp lhs_seeded */
            int lhs_jmp_patch = lhs_store_off + 22 + 1;
            /* rhs_store section. */
            int rhs_store_off = (int)b.size();
            b += call22(val_ptr, fn_store);            /* rt_pop_store_i64(&value.i) */
            /* apply section: re-push lhs and rhs from cache, call op. */
            b += call22(cnt_ptr, fn_push);             /* rt_push_stored_i64(&counter) */
            b += call22(val_ptr, fn_push);             /* rt_push_stored_i64(&value.i) */
            b += call22((uint64_t)(uint32_t)rt_arg, is_relop ? fn_acomp : fn_arith);
            if (!is_relop) {
                b += bytes(1,"\xE9") + u32le(0);      /* jmp outer_γ */
                int γ_patch = (int)b.size() - 4;
                b += bytes(1,"\xE9") + u32le(0);      /* β: jmp lbl_β_jmp */
                int β_def_off = (int)b.size() - 5;
                int β_jmp_patch = (int)b.size() - 4;
                bin.sites  = {lhs_store_off, lhs_jmp_patch, rhs_store_off, γ_patch, β_def_off, β_jmp_patch};
                bin.labels = {lhs_store, lhs_seeded, rhs_store, lbl_γ_out, lbl_β_def, lbl_β_jmp};
                bin.is_def = {true, false, true, false, true, false};
            } else {
                b += call22(0, fn_lok);                /* rt_last_ok() — ptr arg unused */
                /* movabs rax, fn_lok already loaded rax; we re-issue call22 but fn_lok takes void.  */
                /* Override: emit test+branch inline. */
                b.resize(b.size() - 22);               /* undo bogus call22 for fn_lok */
                uint64_t fn_lok2; { int (*fp)(void) = rt_last_ok; fn_lok2 = (uint64_t)(uintptr_t)(void*)fp; }
                b += bytes(2,"\x48\xB8") + u64le(fn_lok2) + bytes(2,"\xFF\xD0"); /* movabs rax,fn; call rax (12) */
                b += bytes(2,"\x85\xC0");              /* test eax,eax */
                b += bytes(2,"\x0F\x85") + u32le(0);  /* jne outer_γ */
                int γ_patch = (int)b.size() - 4;
                b += call22(0, fn_pv);                 /* rt_pop_void() */
                b.resize(b.size() - 22);               /* undo bogus ptr arg */
                b += bytes(2,"\x48\xB8") + u64le(fn_pv) + bytes(2,"\xFF\xD0"); /* 12 bytes */
                b += bytes(1,"\xE9") + u32le(0);       /* jmp rhs_β */
                int rβ_patch = (int)b.size() - 4;
                b += bytes(1,"\xE9") + u32le(0);       /* β: jmp lbl_β_jmp */
                int β_def_off = (int)b.size() - 5;
                int β_jmp_patch = (int)b.size() - 4;
                bin.sites  = {lhs_store_off, lhs_jmp_patch, rhs_store_off, γ_patch, rβ_patch, β_def_off, β_jmp_patch};
                bin.labels = {lhs_store, lhs_seeded, rhs_store, lbl_γ_out, lbl_rhs_β, lbl_β_def, lbl_β_jmp};
                bin.is_def = {true, false, true, false, false, true, false};
            }
            return b;
        }
        if (MEDIUM_TEXT) {
            IR_t * lhs = pBB->α;
            IR_t * rhs = pBB->β;
            if (!lhs || !rhs) {
                /* Degenerate — no operands. Wire α→γ, β→ω so the graph stays connected. */
                return s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment(emit_fmt("# BOX BINOP_GEN(op='%s') [no operands — passthrough]", op))
                     + s_2asm("jmp", _.lbl_γ)
                     + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                     + s_2asm("jmp", _.lbl_ω);
            }
            int id        = bb_node_id(pBB);
            int is_relop  = binop_is_relop(pBB->ival);
            int rt_arg    = binop_runtime_arg(pBB->ival);

            /* Save the parent's four ports — the child emits trample g_emit.lbl_*. */
            const char *p_α = _.lbl_α, *p_γ = _.lbl_γ, *p_ω = _.lbl_ω, *p_β = _.lbl_β;
            bb_label_t *p_αp = _.lbl_α_p, *p_γp = _.lbl_γ_p, *p_ωp = _.lbl_ω_p, *p_βp = _.lbl_β_p;

            /* Child boxes' four ports. Each child's γ/ω lands on a parent-owned trampoline label.    */
            /* Non-static: nested BINOP_GEN (e.g. `3 < ((1 to 3)*(1 to 2))`) recurses through          */
            /* walk_bb_node_str → bb_binop_gen reentrantly; static storage would alias across levels.  */
            bb_label_t Lla, Llg, Llo, Llb;   /* lhs box α/γ/ω/β */
            bb_label_t Lra, Lrg, Lro, Lrb;   /* rhs box α/γ/ω/β */
            emit_label_initf(&Lla, ".LbgL%d_α", id); emit_label_initf(&Llg, ".LbgL%d_γ", id);
            emit_label_initf(&Llo, ".LbgL%d_ω", id); emit_label_initf(&Llb, ".LbgL%d_β", id);
            emit_label_initf(&Lra, ".LbgR%d_α", id); emit_label_initf(&Lrg, ".LbgR%d_γ", id);
            emit_label_initf(&Lro, ".LbgR%d_ω", id); emit_label_initf(&Lrb, ".LbgR%d_β", id);

            int lhs_gen = operand_is_gen(lhs);
            int rhs_gen = operand_is_gen(rhs);
            std::string lhs_box, rhs_box;
            if (lhs_gen) {
                lhs_box = emit_child_box(lhs, &Lla, &Llg, &Llo, &Llb);
            } else {
                int ok = 0;
                lhs_box = synth_single_shot_box(lhs, &ok, &Lla, &Llg, &Llo, &Llb);
                if (!ok) {
                    return s_1asm(emit_fmt("%s:", _.lbl_α))
                         + s_comment(emit_fmt("# BOX BINOP_GEN(op='%s') [non-gen lhs kind=%d — inline TODO]", op, (int)(lhs?lhs->t:-1)))
                         + s_2asm("jmp", _.lbl_ω)
                         + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                         + s_2asm("jmp", _.lbl_ω);
                }
            }
            if (rhs_gen) {
                rhs_box = emit_child_box(rhs, &Lra, &Lrg, &Lro, &Lrb);
            } else {
                int ok = 0;
                rhs_box = synth_single_shot_box(rhs, &ok, &Lra, &Lrg, &Lro, &Lrb);
                if (!ok) {
                    return s_1asm(emit_fmt("%s:", _.lbl_α))
                         + s_comment(emit_fmt("# BOX BINOP_GEN(op='%s') [non-gen rhs kind=%d — inline TODO]", op, (int)(rhs?rhs->t:-1)))
                         + s_2asm("jmp", _.lbl_ω)
                         + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                         + s_2asm("jmp", _.lbl_ω);
                }
            }

            /* Restore parent ports for the orchestration below. */
            _.lbl_α = p_α; _.lbl_γ = p_γ; _.lbl_ω = p_ω; _.lbl_β = p_β;
            _.lbl_α_p = p_αp; _.lbl_γ_p = p_γp; _.lbl_ω_p = p_ωp; _.lbl_β_p = p_βp;

            std::string st  = emit_fmt(".Lbg%d_st", id);   /* state byte (0 fresh / 1 active) */
            std::string lv  = emit_fmt(".Lbg%d_lv", id);   /* held lhs int */
            std::string rv  = emit_fmt(".Lbg%d_rv", id);   /* held rhs int */
            std::string apply   = emit_fmt(".Lbg%d_apply", id);
            std::string advance = emit_fmt(".Lbg%d_advance", id);
            std::string seedrhs = emit_fmt(".Lbg%d_seedrhs", id);
            /* The child boxes jump to their γ/ω labels (the names we passed in Llg/Llo/Lrg/Lro).      */
            /* Those label NAMES are where our trampolines must live, so define them with EXACTLY      */
            /* those names: lhs.γ=Llg, lhs.ω=Llo, rhs.γ=Lrg, rhs.ω=Lro.                                */
            std::string lgt  = Llg.name;   /* lhs.γ trampoline */
            std::string lot  = Llo.name;   /* lhs.ω trampoline → parent ω */
            std::string rgt  = Lrg.name;   /* rhs.γ trampoline */
            std::string rot  = Lro.name;   /* rhs.ω trampoline → advance lhs */

            std::string s;
            s += s_directive(".section .data");
            s += s_directive(st + ": .byte 0");
            s += s_directive(lv + ": .quad 0");
            s += s_directive(rv + ": .quad 0");
            s += s_directive(".section .text");
            s += s_directive(".intel_syntax noprefix");

            /* α (fresh entry): state=1; enter lhs box at its α (seed the outer generator). */
            s += s_1asm(emit_fmt("%s:", p_α));
            s += s_comment(emit_fmt("# BOX BINOP_GEN(op='%s' relop=%d) cross-product odometer", op, is_relop));
            s += s_2asm("lea rax,", "[rip + " + st + "]");
            s += s_2asm("mov byte ptr [rax],", "1");
            s += s_2asm("jmp", Lla.name);

            /* β (retry entry): advance. */
            s += s_L1asm(emit_fmt("%s:", p_β), "");
            s += s_2asm("jmp", advance);

            /* lhs box inline. Its γ → lgt trampoline, ω → lot trampoline, β reachable at Llb. */
            s += lhs_box;

            /* lhs.γ trampoline: pop pushed int → lv; (re)seed rhs from its α. Both fresh-seed and    */
            /* post-advance enter rhs at α (rhs restarts for each new lhs value — the inner loop).     */
            s += s_L1asm(lgt + ":", "");
            s += s_2asm("call", "rt_vstack_pop@PLT");      /* DESCR_t 16B: v|slen->rax, union .i->rdx */
            s += s_2asm("lea rcx,", "[rip + " + lv + "]");
            s += s_2asm("mov [rcx],", "rdx");
            s += s_2asm("jmp", seedrhs);

            /* lhs.ω trampoline: outer generator exhausted → whole BINOP_GEN fails → parent ω. */
            s += s_L1asm(lot + ":", "");
            s += s_2asm("jmp", p_ω);

            /* seed rhs: enter rhs box α (its α-body resets the rhs counter). */
            s += s_L1asm(seedrhs + ":", "");
            s += s_2asm("jmp", Lra.name);

            /* rhs box inline. Its γ → rgt trampoline, ω → rot trampoline, β at Lrb. */
            s += rhs_box;

            /* rhs.γ trampoline: pop pushed int → rv; apply. */
            s += s_L1asm(rgt + ":", "");
            s += s_2asm("call", "rt_vstack_pop@PLT");      /* DESCR_t 16B: union .i in rdx */
            s += s_2asm("lea rcx,", "[rip + " + rv + "]");
            s += s_2asm("mov [rcx],", "rdx");
            s += s_2asm("jmp", apply);

            /* rhs.ω trampoline: inner generator exhausted → advance lhs (β-entry). lhs.γ re-seeds rhs. */
            s += s_L1asm(rot + ":", "");
            s += s_2asm("jmp", Llb.name);

            /* advance (β resume): try rhs β first (advance inner generator). */
            s += s_L1asm(advance + ":", "");
            s += s_2asm("jmp", Lrb.name);

            /* apply: push lv, push rv; call rt_arith/rt_acomp; test last_ok. */
            s += s_L1asm(apply + ":", "");
            s += s_2asm("lea rax,", "[rip + " + lv + "]");
            s += s_2asm("mov rdi,", "[rax]");
            s += s_2asm("call", "rt_push_int@PLT");
            s += s_2asm("lea rax,", "[rip + " + rv + "]");
            s += s_2asm("mov rdi,", "[rax]");
            s += s_2asm("call", "rt_push_int@PLT");
            s += s_2asm("mov edi,", emit_fmt("%d", rt_arg));
            s += s_2asm("call", is_relop ? "rt_acomp@PLT" : "rt_arith@PLT");
            if (is_relop) {
                /* relop: rt_acomp pushed (r or FAIL) and set last_ok. On fail, pop the FAIL and       */
                /* advance to the next pair; on success the result (rhs value) is on the stack → γ.     */
                s += s_2asm("call", "rt_last_ok@PLT");
                s += s_2asm("test eax,", "eax");
                s += s_2asm("jne", p_γ);                  /* success: result already on vstack */
                s += s_2asm("call", "rt_pop_void@PLT");   /* discard FAIL */
                s += s_2asm("jmp", advance);              /* relop failed: try next pair */
            } else {
                /* arithmetic: result on vstack; always succeeds (operand FAIL would have set last_ok=0 */
                /* but operands here are live generator values). → parent γ. */
                s += s_2asm("jmp", p_γ);
            }
            return s;
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_binop_gen(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_binop_gen_str(pBB, bin), bin);
}
