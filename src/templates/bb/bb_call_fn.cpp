#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
extern DESCR_t rt_call_arr_bl_strict(const char *, DESCR_t *, int, int);
extern DESCR_t rt_call_arr_bl_sn4(const char *, DESCR_t *, int, int);
#include "bb_template_common.h"
#include "bb_templates.h"
DESCR_t rt_call_arr(const char * fn, DESCR_t * args, int nargs);
DESCR_t rt_call_arr_bl(const char * fn, DESCR_t * args, int nargs, int bidlen);
extern "C" int zls_g_resume(const IR_graph_t *);
extern "C" {
#include "builtin_ids.h"
}
static int bid_bake_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_BID_BAKE"); v = (e && *e == '0') ? 0 : 1; } return v; }
static long bid_bake_of(const char * fn) { if (!bid_bake_on() || !fn) return -1L; size_t n = strlen(fn); if (n > 0xFFFFu) return -1L; return (long)(((unsigned long)n << 16) | (unsigned long)(unsigned)bid_of(fn, (unsigned)n)); }
DESCR_t rt_pl_dop_unify_ci(DESCR_t * args, long long imm);
DESCR_t rt_pl_dop_unify_cs(DESCR_t * args, const char * cs);
int bb_slot_get(IR_t * nd);
void bb_slot_register(IR_t * nd, int off);
}
#include "x86_asm.h"
std::string marshal_call_arg(IR_t * lf, IR_graph_t * sg, int aoff, IR_t * owner, int idx);
void * dop_direct_fp(const char * fn, int64_t narg, const char ** sym);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" {
typedef struct { long fn; long how; } rk_next_t;
extern rk_next_t rk_method_open(DESCR_t * args, int nargs);
extern DESCR_t rk_method_land_γ(DESCR_t frame0, long word);
extern DESCR_t rk_method_land_ω(long word);
extern long rk_iter_open(DESCR_t * args, int nargs, DESCR_t * cur);
extern rk_next_t rk_iter_step(DESCR_t * cur);
extern long rk_iter_land_γ(DESCR_t frame0, long word, DESCR_t * cur);
extern long rk_iter_land_ω(long word, DESCR_t * cur);
extern DESCR_t rk_iter_finish(DESCR_t * cur);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcfn_opens_as_method(const char * fn, int nargs) { return (fn && nargs >= 2 && !strcmp(fn, "meth_call")) ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcfn_method_open_enter(int base, int decl_id, int join_id) {
    return x86("comment", "THE RAKU METHOD ROAD OPENS AND LANDS RATHER THAN BEING ENTERED FROM C (hq_raku 2026-09-20, row raku-the-method-road-..., CEO-979). Lon 2026-09-20 07:4x, verbatim: a BB can just call any C function and use its return data to jump to the next BB, in that way only BBs jump to other BBs, but after a C call. The old road entered the callee box from C seven frames deep -- rt_call_arr_bl -> rt_call_arr_bl_s -> rt_call_arr_impl -> try_call_builtin_by_name_bl_s -> script_try_call_builtin_by_name -> invoke_method_proc -> rt_call_proc_descr -> rt_proc_enter -- and all seven stayed on the stack for the whole life of the callee. This mirrors bb_glue_prim_open_enter exactly: rdi = the marshalled argv block, esi = its count; rk_method_open returns rax = the callee entry (0 = IT DECLINED, which is not a failure -- the unchanged rt_call_arr_bl road follows this label) and rdx = the packed protocol word. The poll is the sanctioned safe point at the EMITTED return of an allocating runtime call (the open runs the callee prologue) and it is the _word form because rdx is an INTEGER protocol word and rax a code pointer, neither a collected-heap block. EACH LAND POLLS WITH x86_rt_gc_poll_rec_res AND THAT CHOICE IS FORCED: a land RETURNS A DESCR whose second word is a collected-heap pointer, so the sigma_word form that is right after the open would keep it RAW below the poll floor -- the defect x86_rt_gc_poll_rec_sigma_pair was written against. 6.5c writes the rax:rdx pair as the record's ONE cell and hands it to the poll as its shield array, so rt_gc_visit_descr relocates it BY ITS TYPE FIELD, a typed visit that survives the F6 step-3 deletion of the frame sweep. I first left both lands unpolled, reasoning that the caller's store-then-poll at the join covers them; util_gc_census safe-points caught it as unpolled 126 -> 128 and the catch is right, because between a land's return and that store the DESCR is live in registers across an allocating return with no safe point of its own.")
         + x86("call", "rk_method_open", (uint64_t)(uintptr_t)(void *)rk_method_open)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("test", "rax", "rax")
         + x86_jcc_id("jz", decl_id)
         + bb_glue_enter_c2bb(base, base + 5, base + 6)
         + x86_deflabel_id(base + 5)
         + x86("call", "rk_method_land_γ", (uint64_t)(uintptr_t)(void *)rk_method_land_γ)
         + x86_rt_gc_poll_rec_res()
         + x86_jmp_id(join_id)
         + x86_deflabel_id(base + 6)
         + x86("call", "rk_method_land_ω", (uint64_t)(uintptr_t)(void *)rk_method_land_ω)
         + x86_rt_gc_poll_rec_res()
         + x86_jmp_id(join_id);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcfn_opens_as_iter(const char * fn, int nargs) { return (fn && nargs == 3 && !strcmp(fn, "meth_call")) ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcfn_iter_open_enter(int base, const std::string & cellsq, const std::string & curq, int nargs, int decl_id, int join_id) {
    return x86("comment", "THE BOX OWNS THE LOOP AND THE C CODE IS FOUR LEAVES WITH A CURSOR (hq_raku 2026-09-20, ceo CEO-986 phase 0). Lon, verbatim: so make a plan to get those constructs converted to a sequence of BB's -- under his FACT RULE CEO-985 that logic lives in emitted boxes, not in C. WHAT THIS REPLACES, measured by backtrace on a five-element .reduce witness and not inferred: the fold ran inside script_try_call_builtin_by_name and entered the block's box FROM C once per element, seven C frames deep -- rt_call_arr_bl -> rt_call_arr_bl_s -> rt_call_arr_impl -> try_call_builtin_by_name_bl_s -> script_try_call_builtin_by_name(meth_call) -> script_try_call_builtin_by_name(__rk_arr_reduce) -> rt_call_proc_descr -> rt_proc_enter -- and all seven stayed on the stack for the whole life of the block, n-1 times. Now alpha opens the cursor, rk_iter_step is the next leaf plus the per-element open, the transfer is the tree's ONE entry glue, and the land is the emit. THE FOUR LEAVES ARE open, step(=next+open), land(=emit) and finish, and map/reduce/first/sort differ ONLY inside rk_iter_emit and in whether the land answers STOP -- which is why this is one box for four constructs rather than four rewrites. THE GC ANSWER, WHICH IS PHASE 0's REAL DELIVERABLE AND NOT A DETAIL: THE CURSOR IS ONE TYPED DESCR IN A FRAME CELL THE MAP ALREADY COVERS -- this node's own result slot, kind DESCR in the typed frame layout -- and its state lives inside a block of an ALREADY-VISITED type (DT_A, gc_visit_arblk). So no new heap type, no new visitor, NO NEW GLOBAL and no collector change: a C global would have been both a new global under the no-new-globals law and an unrooted holder under Lon's five clauses, which is the pair of mistakes g_redisp made. Positions inside the cursor are the remaining-subject DESCR rather than a raw scan pointer, and gc_heap.c:1110 relocates an interior string pointer by preserving its offset, so the cursor survives a collection BY CONSTRUCTION rather than by a sweep. AND IT GENERALISES, WHICH IS THE SEQUENCING TRAP THE ceo NAMED: a construct whose state does not fit the four cells widens the SAME one visited block (phase 2's sort keeps its element vector there), so the protocol does not get rewritten at the nested loop. WHY A COLLECTION CANNOT HAPPEN INSIDE A LEAF, so only the leaf BOUNDARIES need rooting: rt_gcheap_alloc never collects -- it carves, grows, and arms g_gc_pending -- and the collection happens at the emitted poll (CEO-812, verified in the allocator). THE OPEN DECLINES (rax == 0) FOR EVERYTHING IT IS NOT PROVABLY EXACT ON and the unchanged rt_call_arr_bl road follows the decline label: a non-reduce method, a non-block argument, a data-type invocant, a callee that is not a plain jmp-entry box, an empty array and a single-element array (which make no per-element call at all and stay on the arm that already answers them). THE POLLS ARE THE SANCTIONED SAFE POINTS AT THE EMITTED RETURN OF AN ALLOCATING RUNTIME CALL: the _sigma_word form after the open, the step and each land, because rax is a code pointer or an integer verdict and rdx an integer protocol word -- neither a collected-heap block -- and the _rec_res form after finish, because THAT return IS a DESCR whose second word is a heap pointer, the distinction x86_rt_gc_poll_rec_sigma_pair was written against.")
         + x86("lea", "rdi", cellsq.c_str())
         + x86("mov32", "esi", (long)nargs)
         + x86("lea", "rdx", curq.c_str())
         + x86("call", "rk_iter_open", (uint64_t)(uintptr_t)(void *)rk_iter_open)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("test", "rax", "rax")
         + x86_jcc_id("jz", decl_id)
         + x86_deflabel_id(base + 7)
         + x86("lea", "rdi", curq.c_str())
         + x86("call", "rk_iter_step", (uint64_t)(uintptr_t)(void *)rk_iter_step)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("test", "rax", "rax")
         + x86_jcc_id("jz", base + 9)
         + bb_glue_enter_c2bb(base, base + 5, base + 6)
         + x86_deflabel_id(base + 5)
         + x86("lea", "rcx", curq.c_str())
         + x86("call", "rk_iter_land_γ", (uint64_t)(uintptr_t)(void *)rk_iter_land_γ)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("test", "rax", "rax")
         + x86_jcc_id("jz", base + 7)
         + x86_jmp_id(base + 9)
         + x86_deflabel_id(base + 6)
         + x86("lea", "rsi", curq.c_str())
         + x86("call", "rk_iter_land_ω", (uint64_t)(uintptr_t)(void *)rk_iter_land_ω)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("test", "rax", "rax")
         + x86_jcc_id("jz", base + 7)
         + x86_deflabel_id(base + 9)
         + x86("lea", "rdi", curq.c_str())
         + x86("call", "rk_iter_finish", (uint64_t)(uintptr_t)(void *)rk_iter_finish)
         + x86_rt_gc_poll_rec_res()
         + x86_jmp_id(join_id);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcfn_result_slot(IR_t * nd) {
    { int _s = nd ? zls_off(nd) : -1; if (_s >= 0) { if (bb_slot_get(nd) < 0) bb_slot_register(nd, _s); return _s; } }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_fn_str(IR_t * pBB) {
    const char * fn = _.op_sval ? _.op_sval : "";
    int nargs = (int) _.op_ival;
    if (_.op_zres) {
        std::string s = x86_alpha()
                      + x86("comment", std::string("BOX IR_CALL ZD-7 ") + fn + "(...) -> rt_call_arr [ZD: args from ZOPQ, result to ZRES]");
        if (nargs > 0) {
            s += x86("sub", "rsp", (long)(nargs * 16));
            for (int i = 0; i < nargs; i++) {
                s += x86("mov", "r8", ZOPQ(i, nargs * 16 + 0));
                s += x86("mov", x86_zref(i * 16 + 0, 1), "r8");
                s += x86("mov", "r8", ZOPQ(i, nargs * 16 + 8));
                s += x86("mov", x86_zref(i * 16 + 8, 1), "r8");
            }
        }
        const char * zdsym = 0; void * zdfp = dop_direct_fp(fn, (int64_t)nargs, &zdsym);
        if (!zdfp && _.node && _.node->seal == IR_SEAL_CALL_DET_LEAF) return x86_alpha() + x86_bomb("bb_call_fn: the lowerer sealed this call as a det leaf and dop_direct_fp does not know the callee -- the narrowing is REFUSED for a callee the registry does not know");
        if (zdfp) {
            s += x86("comment", (std::string("PL-REGAIN-2 direct det leaf under ZD: ") + zdsym + " (no by-name dispatch)").c_str());
            s += x86_reg_disp32_lea64("rdi", "rsp", 0);
            s += x86("mov32", "esi", (long)nargs);
            s += x86("call", zdsym, (uint64_t)(uintptr_t)zdfp);
        } else {
        int _mopen = bcfn_opens_as_method(fn, nargs);
        if (_mopen) { s += x86_reg_disp32_lea64("rdi", "rsp", 0) + x86("mov32", "esi", (long)nargs) + bcfn_method_open_enter(20, 28, 29); s += x86_deflabel_id(28); }
        {
            std::string fl = std::string(".L") + x86_boxkind() + "_rkfnzd" + std::to_string(g_flat_node_id++);
            s += x86("directive", ".section .rodata");
            s += x86("directive", (fl + ": .string \"" + fn + "\"").c_str());
            s += x86("directive", ".section .text");
            s += x86("directive", ".intel_syntax noprefix");
            s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
        }
        if (nargs > 0) s += x86_reg_disp32_lea64("rsi", "rsp", 0);
        else           s += x86("xor", "esi", "esi");
        s += x86("mov32", "edx", (long)nargs);
        s += x86("mov32", "ecx", bid_bake_of(fn));
        s += x86("call", ((_.op_strict == 2) ? "rt_call_arr_bl_sn4" : _.op_strict ? "rt_call_arr_bl_strict" : "rt_call_arr_bl"), (uint64_t)(uintptr_t)(void *)((_.op_strict == 2) ? rt_call_arr_bl_sn4 : _.op_strict ? rt_call_arr_bl_strict : rt_call_arr_bl));
        if (_mopen) s += x86_deflabel_id(29);
        }
        if (nargs > 0) s += x86("add", "rsp", (long)(nargs * 16));
        { int _wpop_save = _.op_wpop; int _zgpop_save = _.op_zgpop; if (_.op_sb) { _.op_wpop = 0; _.op_zgpop = 0; }
        s += x86("cmp", "al", (long)DT_FAIL);
        s += x86_omega("je");
        s += x86("note", ZRESN()) + x86("mov", ZRES(0), "rax");
        s += x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx");
        if (_.op_sb) { s += x86_omega(); _.op_wpop = _wpop_save; _.op_zgpop = _zgpop_save; return s; } _.op_wpop = _wpop_save; _.op_zgpop = _zgpop_save; }
        s += x86_rt_gc_poll();
        s += x86_gamma();
        s += x86_beta_trampoline();
        return s;
    }
    int resoff = bcfn_result_slot(pBB);
    if (resoff < 0) return x86_alpha() + x86_bomb("bb_call_fn: no LOWER slot grant (TMP-ERADICATE)");
    if (_.node && nargs > _.node->n_operands) return x86_alpha() + x86_bomb("bb_call_fn: arg count exceeds LOWER grant (TMP-ERADICATE)");
    int argbase = zls_argv_off(pBB); if (argbase < 0) argbase = resoff + 16;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    std::string s = x86_alpha()
                  + x86("comment", std::string("BOX IR_CALL ") + fn + "(...) -> rt_call_arr [operand-marshal, FAIL->ω]");
    const char * dsym = 0; void * dfp = dop_direct_fp(fn, (int64_t)nargs, &dsym);
    if (!dfp && _.node && _.node->seal == IR_SEAL_CALL_DET_LEAF) return x86_alpha() + x86_bomb("bb_call_fn: the lowerer sealed this call as a det leaf and dop_direct_fp does not know the callee -- the narrowing is REFUSED for a callee the registry does not know");
    int cui = -1; long long cival = 0; const char * csval = 0;
    if (dfp && nargs == 2 && !strcmp(fn, "$unify") && !getenv("SCRIP_NO_CU")) {
        for (int i = 0; i < 2 && cui < 0; i++) {
            IR_t * lf = (subs && subs[i]) ? subs[i]->entry : ir_call_arg(pBB, i);
            if (!lf) continue;
            if (lf->op == IR_LIT_INTEGER) { cui = i; cival = (long long)IR_LIT(lf).ival; }
            else if (lf->op == IR_LIT_STRING && IR_LIT(lf).sval) { cui = i; csval = IR_LIT(lf).sval; }
        }
    }
    if (cui >= 0) {
        int vi = 1 - cui;
        s += marshal_call_arg((subs && subs[vi]) ? subs[vi]->entry : ir_call_arg(pBB, vi), (subs && subs[vi]) ? subs[vi] : NULL, argbase, _.node, vi);
    } else {
        for (int i = nargs - 1; i >= 0; i--)
            s += marshal_call_arg((subs && subs[i]) ? subs[i]->entry : ir_call_arg(pBB, i), (subs && subs[i]) ? subs[i] : NULL, argbase + i * 16, _.node, i);
    }
    if (dfp && cui >= 0) {
        s += x86("comment", (std::string("PL-REGAIN-5 const head-unify leaf: ") + (csval ? "rt_pl_dop_unify_cs" : "rt_pl_dop_unify_ci") + " (const in reg, one-operand marshal)").c_str());
        s += x86("lea", "rdi", FRQ(argbase));
        if (csval) {
            s += x86("mov", "rsi", ROQ(cui * 2));
            s += x86_jmp_id(cui * 2 + 1);
            s += x86_ro_seal_str(cui * 2, csval);
            s += x86_deflabel_id(cui * 2 + 1);
            s += x86("call", "rt_pl_dop_unify_cs", (uint64_t)(uintptr_t)(void *)rt_pl_dop_unify_cs);
        } else {
            s += x86_movabs_r64("rsi", (uint64_t)cival);
            s += x86("call", "rt_pl_dop_unify_ci", (uint64_t)(uintptr_t)(void *)rt_pl_dop_unify_ci);
        }
    } else if (dfp) {
        s += x86("comment", (std::string("PL-REGAIN-2 direct det leaf: ") + dsym + " (no by-name dispatch)").c_str());
        s += x86("lea", "rdi", FRQ(argbase));
        s += x86("mov32", "esi", (long)nargs);
        s += x86("call", dsym, (uint64_t)(uintptr_t)dfp);
    } else {
        int _mopen = bcfn_opens_as_method(fn, nargs);
        int _iopen = bcfn_opens_as_iter(fn, nargs);
        if (_mopen) { s += x86("lea", "rdi", FRQ(argbase)) + x86("mov32", "esi", (long)nargs) + bcfn_method_open_enter(20, 28, 29); s += x86_deflabel_id(28); }
        if (_iopen) { std::string _cq = FRQ(argbase); std::string _rq = FRQ(resoff); s += bcfn_iter_open_enter(40, _cq, _rq, nargs, 48, 29); s += x86_deflabel_id(48); }
        std::string fl = std::string(".L") + x86_boxkind() + "_rkfn" + std::to_string(g_flat_node_id++);
        s += x86("directive", ".section .rodata");
        s += x86("directive", (fl + ": .string \"" + fn + "\"").c_str());
        s += x86("directive", ".section .text");
        s += x86("directive", ".intel_syntax noprefix");
        s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
        s += x86("lea", "rsi", FRQ(argbase));
        s += x86("mov32", "edx", (long)nargs);
        s += x86("rtcc_wb");
        s += x86("mov32", "ecx", bid_bake_of(fn));
        s += x86("call_bare", ((_.op_strict == 2) ? "rt_call_arr_bl_sn4" : _.op_strict ? "rt_call_arr_bl_strict" : "rt_call_arr_bl"), (uint64_t)(uintptr_t)(void *)((_.op_strict == 2) ? rt_call_arr_bl_sn4 : _.op_strict ? rt_call_arr_bl_strict : rt_call_arr_bl));
        s += x86("rtcc_rl");
        if (_mopen || _iopen) s += x86_deflabel_id(29);
    }
    s += x86("mov", FRQ(resoff), "rax");
    s += x86("mov", FRQ(resoff + 8), "rdx");
    s += x86("cmp", "al", (long)DT_FAIL);
    s += x86_omega("je");
    s += x86_rt_gc_poll();
    s += x86_gamma();
    s += x86_beta();
    s += x86_omega();
    return s;
}
