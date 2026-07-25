#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
DESCR_t rt_call_arr(const char * fn, DESCR_t * args, int nargs);
DESCR_t rt_pl_dop_unify_ci(DESCR_t * args, long long imm);
DESCR_t rt_pl_dop_unify_cs(DESCR_t * args, const char * cs);
int bb_slot_get(IR_t * nd);
void bb_slot_register(IR_t * nd, int off);
}
#include "x86_asm.h"
std::string marshal_call_arg(IR_t * lf, IR_graph_t * sg, int aoff, IR_t * owner, int idx);
void * dop_direct_fp(const char * fn, int64_t narg, const char ** sym);
extern "C" char g_pl_trail[];
/* PL-SINK-1 (2026-07-24) — EMITTED $unify FAST PATH.  The data-plane leaves measured 86% of Prolog wall live in C (s141 FINDING §ARCHITECTURAL VERDICT); this sinks the hot arms of plw_unify_cells
 * (by_name_dispatch.c) into the box itself: deref chase (DT_PLVAR chain), ptr-equal, one-side bind (inline trail push + 16-byte cell copy), int==int, and a bit-identical-descr shortcut.  Every arm the
 * fast path cannot decide EXACTLY (DT_N entry forms, both-unbound join/VVB, compound recursion, floats/NaN, non-identical atoms → rt_descr_equal, trail uninitialized/full → area grow) falls into the
 * UNTOUCHED C leaf rt_pl_dop_unify with UNMODIFIED inputs, so outcomes are bit-identical by construction: the inline path either completes a whole plw arm verbatim or defers the whole call.  The GC
 * safepoint + unwind-floor discipline are alloc/throw-time concerns — the inline arms allocate nothing and cannot throw, so both stay in the leaf (the slow path).  Layout literals (tag offsets 0/+8,
 * trail base+0/cap+24/top+32, 24-byte entries) are _Static_assert-anchored beside plw_bind in by_name_dispatch.c.  Internal label ids 40..54 (marshal uses idx*2/idx*2+1 ≤ 3).  Kill switch:
 * SCRIP_NO_SINK=1 at emit time (the SCRIP_SLOT_ELIDE / SCRIP_NO_CU pattern); the SCRIP_NO_DOP / SCRIP_DOP_SKIP hatches disable it too (the sink nests inside the dfp branch), and poison-trap debug
 * sessions (SCRIP_PL_POISON_TRAP) should set SCRIP_NO_SINK since inline arms skip poison reads. */
static std::string sink_deref(const char * reg, int lh, int ld, int ln2) {
    return x86_deflabel_id(lh)
         + x86("mov", "eax", (std::string("dword ptr [") + reg + " + 0]").c_str())
         + x86("cmp", "eax", (long)9)  + x86_jcc_id("jne", ln2 + 1)
         + x86("mov", "esi", (std::string("dword ptr [") + reg + " + 4]").c_str())
         + x86("mov", "rax", (std::string("[") + reg + " + 8]").c_str())
         + x86("test", "rax", "rax")   + x86_jcc_id("je", ld)
         + x86("cmp", "esi", (long)1)  + x86_jcc_id("jne", ln2)
         + x86("mov", reg, "rax")
         + x86_jmp_id(lh)
         + x86_deflabel_id(ln2)
         + x86("cmp", "esi", (long)2)  + x86_jcc_id("jne", ld)
         + x86("mov", "rax", "[rax + 0]")
         + x86("test", "rax", "rax")   + x86_jcc_id("je", ld)
         + x86("mov", reg, "rax")
         + x86_jmp_id(lh)
         + x86_deflabel_id(ln2 + 1)
         + x86("cmp", "eax", (long)13) + x86_jcc_id("jne", ld)
         + x86("mov", "rax", (std::string("[") + reg + " + 8]").c_str())
         + x86("test", "rax", "rax")   + x86_jcc_id("je", ld)
         + x86("cmp", "rax", reg)      + x86_jcc_id("je", ld)
         + x86("mov", reg, "rax")
         + x86_jmp_id(lh)
         + x86_deflabel_id(ld);
}
static std::string sink_unb(const char * reg, int lyes, int lno) {
    return x86("mov", "eax", (std::string("dword ptr [") + reg + " + 0]").c_str())
         + x86("cmp", "eax", (long)0)  + x86_jcc_id("je", lyes)
         + x86("cmp", "eax", (long)99) + x86_jcc_id("je", lyes)
         + x86("cmp", "eax", (long)13) + x86_jcc_id("jne", lno)
         + x86("mov", "rax", (std::string("[") + reg + " + 8]").c_str())
         + x86("cmp", "rax", reg)      + x86_jcc_id("je", lyes)
         + x86_jmp_id(lno);
}
static std::string sink_trailpush(const char * creg, int lslow) {
    return x86("lea", "r10", "[rip + __]", (uint64_t)(uintptr_t)g_pl_trail, "g_pl_trail")
         + x86("mov", "r11", "[r10 + 0]")
         + x86("test", "r11", "r11")   + x86_jcc_id("je", lslow)
         + x86("mov", "eax", "dword ptr [r10 + 32]")
         + x86("mov32", "esi", (long)24)
         + x86("imul", "rsi", "rax")
         + x86("mov", "rax", "[r10 + 24]")
         + x86("sub", "rax", (long)24)
         + x86("cmp", "rsi", "rax")    + x86_jcc_id("ja", lslow)
         + x86("add", "r11", "rsi")
         + x86("mov", "[r11 + 0]", creg)
         + x86("mov", "rax", (std::string("[") + creg + " + 0]").c_str())
         + x86("mov", "[r11 + 8]", "rax")
         + x86("mov", "rax", (std::string("[") + creg + " + 8]").c_str())
         + x86("mov", "[r11 + 16]", "rax")
         + x86("mov", "eax", "dword ptr [r10 + 32]")
         + x86("add", "eax", (long)1)
         + x86("mov", "dword ptr [r10 + 32]", "eax");
}
static std::string sink_cp16(const char * dst, const char * src) {
    return x86("mov", "rax", (std::string("[") + src + " + 0]").c_str())
         + x86("mov", (std::string("[") + dst + " + 0]").c_str(), "rax")
         + x86("mov", "rax", (std::string("[") + src + " + 8]").c_str())
         + x86("mov", (std::string("[") + dst + " + 8]").c_str(), "rax");
}
static std::string sink_unify2_str(int argbase, uint64_t ufp, const char * usym) {
    std::string s = x86("comment", "PL-SINK-1 inline $unify fast path: deref/bind/trail/int-eq emitted; rt_pl_dop_unify stays the slow-path oracle (bit-identical fallback, unmodified args)");
    s += x86("lea", "rdi", FRQ(argbase));
    s += x86("lea", "r8",  FRQ(argbase));
    s += sink_deref("r8", 40, 41, 55);
    s += x86("lea", "r9",  FRQ(argbase + 16));
    s += sink_deref("r9", 42, 43, 57);
    s += x86("cmp", "r8", "r9");
    s += x86_jcc_id("je", 51);
    s += sink_unb("r8", 44, 45);
    s += x86_deflabel_id(44);
    s += sink_unb("r9", 53, 46);
    s += x86_deflabel_id(46);
    s += sink_trailpush("r8", 53);
    s += sink_cp16("r8", "r9");
    s += x86_jmp_id(51);
    s += x86_deflabel_id(45);
    s += sink_unb("r9", 47, 48);
    s += x86_deflabel_id(47);
    s += sink_trailpush("r9", 53);
    s += sink_cp16("r9", "r8");
    s += x86_jmp_id(51);
    s += x86_deflabel_id(48);
    s += x86("mov", "ecx", "dword ptr [r8 + 0]");
    s += x86("mov", "edx", "dword ptr [r9 + 0]");
    s += x86("cmp", "ecx", (long)14);
    s += x86_jcc_id("jne", 49);
    s += x86("cmp", "edx", (long)14);
    s += x86_jcc_id("je", 53);
    s += x86_jmp_id(52);
    s += x86_deflabel_id(49);
    s += x86("cmp", "edx", (long)14);
    s += x86_jcc_id("je", 52);
    s += x86("cmp", "ecx", (long)7);
    s += x86_jcc_id("je", 53);
    s += x86("cmp", "edx", (long)7);
    s += x86_jcc_id("je", 53);
    s += x86("cmp", "ecx", (long)6);
    s += x86_jcc_id("jne", 50);
    s += x86("cmp", "edx", (long)6);
    s += x86_jcc_id("jne", 50);
    s += x86("mov", "rax", "[r8 + 0]");
    s += x86("cmp64", "rax", (long)6);
    s += x86_jcc_id("jne", 53);
    s += x86("mov", "rax", "[r9 + 0]");
    s += x86("cmp64", "rax", (long)6);
    s += x86_jcc_id("jne", 53);
    s += x86("mov", "rax", "[r8 + 8]");
    s += x86("mov", "rsi", "[r9 + 8]");
    s += x86("cmp", "rax", "rsi");
    s += x86_jcc_id("je", 51);
    s += x86_jmp_id(52);
    s += x86_deflabel_id(50);
    s += x86("mov", "rax", "[r8 + 0]");
    s += x86("mov", "rsi", "[r9 + 0]");
    s += x86("cmp", "rax", "rsi");
    s += x86_jcc_id("jne", 53);
    s += x86("mov", "rax", "[r8 + 8]");
    s += x86("mov", "rsi", "[r9 + 8]");
    s += x86("cmp", "rax", "rsi");
    s += x86_jcc_id("jne", 53);
    s += x86_deflabel_id(51);
    s += x86("mov", "rax", "[r8 + 0]");
    s += x86("mov", "rdx", "[r8 + 8]");
    s += x86_jmp_id(54);
    s += x86_deflabel_id(52);
    s += x86("mov32", "eax", (long)99);
    s += x86("mov32", "edx", (long)0);
    s += x86_jmp_id(54);
    s += x86_deflabel_id(53);
    s += x86("mov32", "esi", (long)2);
    s += x86("call", usym, ufp);
    s += x86_deflabel_id(54);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcfn_result_slot(IR_t * nd) {
    { int _s = nd ? zls_off(nd) : -1; if (_s >= 0) { if (bb_slot_get(nd) < 0) bb_slot_register(nd, _s); return _s; } }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_fn_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn = _.op_sval ? _.op_sval : "";
    int nargs = (int) _.op_ival;
    int resoff = bcfn_result_slot(pBB);
    if (resoff < 0) return x86_alpha() + x86_bomb("bb_call_fn: no LOWER slot grant (TMP-ERADICATE)");
    if (_.node && nargs > _.node->n_operands) return x86_alpha() + x86_bomb("bb_call_fn: arg count exceeds LOWER grant (TMP-ERADICATE)");
    int argbase = resoff + 16;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    std::string s = x86_alpha()
                  + x86("comment", std::string("BOX IR_CALL ") + fn + "(...) -> rt_call_arr [operand-marshal, FAIL->ω]");
    const char * dsym = 0; void * dfp = dop_direct_fp(fn, (int64_t)nargs, &dsym);
    /* PL-REGAIN-5 (2026-07-19): const head-unify — when one $unify side is an emit-time LIT (int / atom-as-LIT_STRING, the lower_prolog shapes), the const rides in a REGISTER and only the other side is
     * marshaled (to args[0], where the ci/cs leaf expects it).  Eligibility is emit-time-static and rides UNDER the dop hatches (SCRIP_NO_DOP / SCRIP_DOP_SKIP=unify disable it with the leaf).  The RO
     * string uses the LIT_S arm's in-band seal idiom verbatim (both-media); the skipped arg's seal ids (cui*2, cui*2+1) are free exactly because its LIT marshal is skipped.  Producer boxes still emit;
     * only the site-side copy and the DESCR-pair operand die. */
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
        for (int i = 0; i < nargs; i++)
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
    } else if (dfp && nargs == 2 && !strcmp(fn, "$unify") && !getenv("SCRIP_NO_SINK")) {
        s += sink_unify2_str(argbase, (uint64_t)(uintptr_t)dfp, dsym);
    } else if (dfp) {
        s += x86("comment", (std::string("PL-REGAIN-2 direct det leaf: ") + dsym + " (no by-name dispatch)").c_str());
        s += x86("lea", "rdi", FRQ(argbase));
        s += x86("mov32", "esi", (long)nargs);
        s += x86("call", dsym, (uint64_t)(uintptr_t)dfp);
    } else {
        std::string fl = std::string(".Lrkfn") + std::to_string(g_flat_node_id++);
        s += x86("directive", ".section .rodata");
        s += x86("directive", (fl + ": .string \"" + fn + "\"").c_str());
        s += x86("directive", ".section .text");
        s += x86("directive", ".intel_syntax noprefix");
        s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
        s += x86("lea", "rsi", FRQ(argbase));
        s += x86("mov32", "edx", (long)nargs);
        s += x86("call", "rt_call_arr", (uint64_t)(uintptr_t)(void *)rt_call_arr);
    }
    s += x86("mov", FRQ(resoff), "rax");
    s += x86("mov", FRQ(resoff + 8), "rdx");
    s += x86("cmp", "eax", (long)99);
    s += x86_omega("je");
    s += x86_gamma();
    s += x86_beta();
    s += x86_omega();
    return s;
}
