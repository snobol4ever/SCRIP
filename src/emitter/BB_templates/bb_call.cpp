#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
#include "emit_bb.h"
#include "../../runtime/builtins/gen.h"
void rt_write_any_nl(DESCR_t d);
int  rt_proc_is_registered(const char *name);
int  rt_builtin_is_known(const char *name);
int  bb_slot_get(IR_t * nd);
int  bb_slot_alloc16(IR_t * nd);
int  bb_varslot(const char * name);
DESCR_t rt_call_arr(const char * fn, DESCR_t * args, int nargs);
int64_t rt_gvar_get_int(const char * name);
DESCR_t rt_gvar_get_descr(const char * name);
DESCR_t rt_call_named_proc(const char * name, DESCR_t * args, int nargs);
DESCR_t rt_call_named_proc_sl(const char * name, DESCR_t * args, int nargs, void * sl);
int  rt_proc_frame_nslots(const char * name);
int  rt_proc_decl_level(const char * name);
uint64_t rt_proc_byref_mask(const char * name);
DESCR_t * rt_gvar_cell(const char * name);
extern int g_emit_frame_caller_dl;
DESCR_t rt_proc_define(const char * spec);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string pas_sl_setup(const char * fn) {
    int callee_dl = rt_proc_decl_level(fn);
    if (g_emit_frame_caller_dl < 0 || callee_dl < 1) return x86("mov32", "ecx", (long)0);
    int h = (g_emit_frame_caller_dl + 1) - callee_dl;
    if (h < 0) h = 0;
    std::string s = x86_frame_lea("rcx", 0);
    for (int i = 0; i < h; i++) s += x86_reg_disp32_load64("rcx", "rcx", 0);
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string marshal_varparam_addr(IR_t * lf, int aoff, int idx) {
    if (!lf) return x86_bomb("marshal_varparam_addr: null arg head");
    IR_t * fin = lf; int gg = 0;
    while (fin && fin->γ && fin->γ->t != IR_SUCCEED && fin->γ->t != IR_FAIL && gg++ < 256) fin = fin->γ;
    if (fin) lf = fin;
    std::string s;
    if (lf->t == IR_VAR_FRAME) {
        int hops = (int) lf->dval;
        int voff = 16 + (int) lf->ival * 16;
        if (MEDIUM_TEXT) s += x86("comment", emit_fmt("marshal arg%d = VAR-PARAM cell addr of frame slot=%d hops=%d -> [r12+%d]", idx, (int) lf->ival, hops, aoff));
        s += x86_frame_lea("rax", 0);
        for (int h = 0; h < hops; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
        s += x86_reg_disp32_lea64("rax", "rax", voff);
    } else if (lf->t == IR_VAR_FRAME_REF) {
        int hops = (int) lf->dval;
        int voff = 16 + (int) lf->ival * 16;
        if (MEDIUM_TEXT) s += x86("comment", emit_fmt("marshal arg%d = VAR-PARAM forward cell addr from ref slot=%d hops=%d -> [r12+%d]", idx, (int) lf->ival, hops, aoff));
        s += x86_frame_lea("rax", 0);
        for (int h = 0; h < hops; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
        s += x86_reg_disp32_load64("rax", "rax", voff + 8);
    } else if (lf->t == IR_VAR && lf->sval) {
        if (MEDIUM_TEXT) {
            char b1[80]; strtab_label(b1, sizeof b1, lf->sval);
            s += x86("comment", emit_fmt("marshal arg%d = VAR-PARAM cell addr of gvar -> [r12+%d]", idx, aoff))
               + x86("ins2", "lea", emit_fmt("rdi, [rip + %s]", b1)) + x86("ins2", "call", "rt_gvar_cell@PLT");
        } else {
            uint64_t fptr; { DESCR_t * (*fp)(const char *) = rt_gvar_cell; fptr = (uint64_t)(uintptr_t)(void *) fp; }
            s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)lf->sval) + x86_call_ro("rt_gvar_cell", fptr);
        }
    } else {
        return x86_bomb("marshal_varparam_addr: var-param arg is not a variable");
    }
    if (MEDIUM_TEXT) {
        s += x86("ins2", "mov", emit_fmt("qword ptr [r12+%d], 0", aoff));
        s += x86("ins2", "mov", emit_fmt("[r12+%d], rax", aoff + 8));
    } else {
        s += x86("mov", FRQ(aoff), (long)0);
        s += x86_frame_store64(aoff + 8, "rax");
    }
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern std::string bb_call_proc_staged_str(IR_t *);
extern std::string bb_call_write_slot_str(IR_t *);
extern std::string bb_call_write_binop_str(IR_t *);
extern std::string bb_call_write_legacy_str(IR_t *, int);
extern std::string bb_call_userproc_str(IR_t *);
extern std::string bb_call_builtin_str(IR_t *);
/*--------------------------------------------------------------------------------------------------------------------*/
std::string marshal_call_arg(IR_t * lf, int aoff, IR_t * owner, int idx) {
    if (!lf) return std::string();
    if (g_gvar_flat_chain) {
        IR_t * fin = lf; int gg = 0;
        while (fin && fin->γ && fin->γ->t != IR_SUCCEED && fin->γ->t != IR_FAIL && gg++ < 256) fin = fin->γ;
        int fin_arith = (fin && fin->t == IR_BINOP && (fin->ival == BINOP_ADD || fin->ival == BINOP_SUB || fin->ival == BINOP_MUL || fin->ival == BINOP_DIV || fin->ival == BINOP_MOD));
        int opnd_ok_a = (fin_arith && fin->α && ((fin->α->t == IR_LIT_I) || (fin->α->t == IR_VAR && fin->α->sval) || (fin->α->t == IR_VAR_FRAME) || (fin->α->t == IR_VAR_FRAME_REF)));
        int opnd_ok_b = (fin_arith && fin->β && ((fin->β->t == IR_LIT_I) || (fin->β->t == IR_VAR && fin->β->sval) || (fin->β->t == IR_VAR_FRAME) || (fin->β->t == IR_VAR_FRAME_REF)));
        if (fin && fin != lf && fin_arith && opnd_ok_a && opnd_ok_b) {
            int scratch = bb_slot_alloc16(fin);
            std::string s;
            if (MEDIUM_TEXT) s += x86("comment", emit_fmt("marshal arg%d = inline gvar-arith subexpr -> [r12+%d]", idx, aoff));
            if (fin->α->t == IR_VAR) {
                if (MEDIUM_TEXT) { char b1[80]; strtab_label(b1, sizeof b1, fin->α->sval); s += x86("ins2", "lea", emit_fmt("rdi, [rip + %s]", b1)) + x86("ins2", "call", "rt_gvar_get_int@PLT"); }
                else { s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)fin->α->sval) + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *)rt_gvar_get_int); }
            } else if (fin->α->t == IR_VAR_FRAME) {
                s += x86_frame_lea("rax", 0);
                for (int h = 0; h < (int) fin->α->dval; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
                s += x86_reg_disp32_load64("rax", "rax", 16 + (int) fin->α->ival * 16 + 8);
            } else if (fin->α->t == IR_VAR_FRAME_REF) {
                s += x86_frame_lea("rax", 0);
                for (int h = 0; h < (int) fin->α->dval; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
                s += x86_reg_disp32_load64("rax", "rax", 16 + (int) fin->α->ival * 16 + 8);
                s += x86_reg_disp32_load64("rax", "rax", 8);
            } else s += x86_movabs_r64("rax", (uint64_t)fin->α->ival);
            s += x86_frame_store64(scratch, "rax");
            if (fin->β->t == IR_VAR) {
                if (MEDIUM_TEXT) { char b2[80]; strtab_label(b2, sizeof b2, fin->β->sval); s += x86("ins2", "lea", emit_fmt("rdi, [rip + %s]", b2)) + x86("ins2", "call", "rt_gvar_get_int@PLT"); }
                else { s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)fin->β->sval) + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *)rt_gvar_get_int); }
                s += x86("mov", "rcx", "rax");
            } else if (fin->β->t == IR_VAR_FRAME) {
                s += x86_frame_lea("rax", 0);
                for (int h = 0; h < (int) fin->β->dval; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
                s += x86_reg_disp32_load64("rax", "rax", 16 + (int) fin->β->ival * 16 + 8);
                s += x86("mov", "rcx", "rax");
            } else if (fin->β->t == IR_VAR_FRAME_REF) {
                s += x86_frame_lea("rax", 0);
                for (int h = 0; h < (int) fin->β->dval; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
                s += x86_reg_disp32_load64("rax", "rax", 16 + (int) fin->β->ival * 16 + 8);
                s += x86_reg_disp32_load64("rax", "rax", 8);
                s += x86("mov", "rcx", "rax");
            } else s += x86("mov", "rcx", (long)fin->β->ival);
            s += x86_frame_load64("rax", scratch);
            switch ((int)fin->ival) {
            case BINOP_ADD: s += x86("add",  "rax", "rcx"); break;
            case BINOP_SUB: s += x86("sub",  "rax", "rcx"); break;
            case BINOP_MUL: s += x86("imul", "rax", "rcx"); break;
            case BINOP_DIV: s += x86("cqo") + x86("idiv", "rcx"); break;
            case BINOP_MOD: s += x86("cqo") + x86("idiv", "rcx") + x86("mov", "rax", "rdx"); break;
            }
            s += x86("mov", FRQ(aoff), (long)6);
            s += x86_frame_store64(aoff + 8, "rax");
            return s;
        }
        if (lf->t == IR_VAR && lf->sval) {
            std::string s;
            if (MEDIUM_TEXT) {
                char b1[80]; strtab_label(b1, sizeof b1, lf->sval);
                s += x86("comment", emit_fmt("marshal arg%d = gvar read -> [r12+%d]", idx, aoff))
                   + x86("ins2", "lea", emit_fmt("rdi, [rip + %s]", b1)) + x86("ins2", "call", "rt_gvar_get_descr@PLT");
            } else {
                s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)lf->sval) + x86("call", "rt_gvar_get_descr", (uint64_t)(uintptr_t)(void *)rt_gvar_get_descr);
            }
            s += x86_frame_store64(aoff, "rax");
            s += x86_frame_store64(aoff + 8, "rdx");
            return s;
        }
        if (lf->t == IR_VAR_FRAME) {
            int hops = (int) lf->dval;
            int voff = 16 + (int) lf->ival * 16;
            std::string s = IF(MEDIUM_TEXT, x86("comment", emit_fmt("marshal arg%d = frame var slot=%d hops=%d -> [r12+%d]", idx, (int) lf->ival, hops, aoff)));
            s += x86_frame_lea("rax", 0);
            for (int h = 0; h < hops; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
            s += x86_reg_disp32_load64("rcx", "rax", voff)     + x86_frame_store64(aoff, "rcx");
            s += x86_reg_disp32_load64("rcx", "rax", voff + 8) + x86_frame_store64(aoff + 8, "rcx");
            return s;
        }
        if (lf->t == IR_VAR_FRAME_REF) {
            int hops = (int) lf->dval;
            int voff = 16 + (int) lf->ival * 16;
            std::string s = IF(MEDIUM_TEXT, x86("comment", emit_fmt("marshal arg%d = frame ref-var deref slot=%d hops=%d -> [r12+%d]", idx, (int) lf->ival, hops, aoff)));
            s += x86_frame_lea("rax", 0);
            for (int h = 0; h < hops; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
            s += x86_reg_disp32_load64("rax", "rax", voff + 8);
            s += x86_reg_disp32_load64("rcx", "rax", 0) + x86_frame_store64(aoff, "rcx");
            s += x86_reg_disp32_load64("rcx", "rax", 8) + x86_frame_store64(aoff + 8, "rcx");
            return s;
        }
    }
    if (lf->t == IR_CALL && (lf->dval == 2.0 || lf->dval == 3.0 || lf->dval == 5.0)) {
        const char * nfn = lf->sval ? lf->sval : "";
        int nn = (int) lf->ival;
        IR_graph_t ** nsubs = (IR_graph_t **)(intptr_t) lf->counter;
        int avbase = (nn > 0) ? bb_slot_alloc16(nsubs[0]->entry) : bb_slot_alloc16(lf);
        for (int j = 1; j < nn; j++) bb_slot_alloc16(nsubs[j]->entry);
        int isreg = (nfn[0] && rt_proc_is_registered(nfn));
        int nmig  = isreg && rt_proc_frame_nslots(nfn) >= 0;
        uint64_t nbrm = isreg ? rt_proc_byref_mask(nfn) : 0;
        const char * rsym = isreg ? (nmig ? "rt_call_named_proc_sl" : "rt_call_named_proc") : "rt_call_arr";
        std::string s;
        for (int j = 0; j < nn; j++) s += ((nbrm >> j) & 1ull) ? marshal_varparam_addr(nsubs[j]->entry, avbase + j * 16, j) : marshal_call_arg(nsubs[j]->entry, avbase + j * 16, lf, j);
        if (MEDIUM_TEXT) {
            std::string fl = emit_fmt(".Lcallfn%d", _.nid);
            s += x86("directive", ".section .rodata")
               + x86("directive", (fl + ": .string \"" + nfn + "\"").c_str())
               + x86("directive", ".section .text") + x86("directive", ".intel_syntax noprefix");
            s += x86("ins2", "lea", emit_fmt("rdi, [rip+%s]", fl.c_str()));
            s += x86("ins2", "lea", emit_fmt("rsi, [r12+%d]", avbase));
            s += x86("ins2", "mov", emit_fmt("edx, %d", nn));
            if (nmig) s += pas_sl_setup(nfn);
            s += x86("ins2", "call", emit_fmt("%s@PLT", rsym));
            s += x86("ins2", "mov", emit_fmt("[r12+%d], rax", aoff));
            s += x86("ins2", "mov", emit_fmt("[r12+%d], rdx", aoff + 8));
        } else if (MEDIUM_BINARY) {
            uint64_t fptr;
            if (nmig)       { DESCR_t (*fp)(const char *, DESCR_t *, int, void *) = rt_call_named_proc_sl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            else if (isreg) { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_named_proc; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            else            { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_arr;        fptr = (uint64_t)(uintptr_t)(void*)fp; }
            s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)nfn);
            s += x86_frame_lea("rsi", avbase);
            s += x86("mov32", "edx", (long)nn);
            if (nmig) s += pas_sl_setup(nfn);
            s += x86_call_ro(rsym, fptr);
            s += x86_frame_store64(aoff, "rax");
            s += x86_frame_store64(aoff + 8, "rdx");
        }
        return s;
    }
    std::string s;
    if (MEDIUM_TEXT) {
        if (lf->t == IR_LIT_I) {
            s += x86("ins2", "mov", emit_fmt("qword ptr [r12+%d], 6", aoff));
            s += x86("ins2", "movabs", emit_fmt("rax, %lld", (long long)lf->ival));
            s += x86("ins2", "mov", emit_fmt("[r12+%d], rax", aoff + 8));
        } else if (lf->t == IR_LIT_F) {
            uint64_t bits; double d = lf->dval; memcpy(&bits, &d, 8);
            s += x86("ins2", "mov", emit_fmt("qword ptr [r12+%d], 7", aoff));
            s += x86("ins2", "movabs", emit_fmt("rax, %llu", (unsigned long long)bits));
            s += x86("ins2", "mov", emit_fmt("[r12+%d], rax", aoff + 8));
        } else if (lf->t == IR_LIT_NUL) {
            s += x86("ins2", "mov", emit_fmt("qword ptr [r12+%d], 0", aoff));
            s += x86("ins2", "mov", emit_fmt("qword ptr [r12+%d], 0", aoff + 8));
        } else if (lf->t == IR_LIT_S) {
            std::string sl = emit_fmt(".Lcallarg%d_%d", _.nid, idx);
            s += x86("directive", ".section .rodata")
               + x86("directive", sl + ": .string \"" + (lf->sval ? lf->sval : "") + "\"")
               + x86("directive", ".section .text") + x86("directive", ".intel_syntax noprefix");
            s += x86("ins2", "mov", emit_fmt("qword ptr [r12+%d], 1", aoff));
            s += x86("ins2", "lea", emit_fmt("rax, [rip+%s]", sl.c_str()));
            s += x86("ins2", "mov", emit_fmt("[r12+%d], rax", aoff + 8));
        } else {
            int voff = bb_varslot(lf->sval ? lf->sval : "");
            s += x86("ins2", "mov", emit_fmt("rax, [r12+%d]", voff));
            s += x86("ins2", "mov", emit_fmt("[r12+%d], rax", aoff));
            s += x86("ins2", "mov", emit_fmt("rax, [r12+%d]", voff + 8));
            s += x86("ins2", "mov", emit_fmt("[r12+%d], rax", aoff + 8));
        }
    } else if (MEDIUM_BINARY) {
        uint32_t aoffu = (uint32_t) aoff;
        if (lf->t == IR_LIT_I || lf->t == IR_LIT_F || lf->t == IR_LIT_NUL || lf->t == IR_LIT_S) {
            uint64_t tag = (lf->t == IR_LIT_I) ? 6 : (lf->t == IR_LIT_F) ? 7 : (lf->t == IR_LIT_S) ? 1 : 0;
            s += x86("mov", FRQ(aoff), (long)tag);
            if (lf->t == IR_LIT_NUL) {
                s += x86("mov", FRQ(aoff + 8), (long)0);
            } else {
                uint64_t v;
                if (lf->t == IR_LIT_I)      v = (uint64_t)lf->ival;
                else if (lf->t == IR_LIT_F) { double d = lf->dval; memcpy(&v, &d, 8); }
                else                        v = (uint64_t)(uintptr_t)(lf->sval ? lf->sval : "");
                s += x86_load_ro("rax", "??", (uint64_t)v);
                s += x86_frame_store64(aoff + 8, "rax");
            }
        } else {
            uint32_t voff = (uint32_t) bb_varslot(lf->sval ? lf->sval : "");
            s += x86_frame_load64("rax", voff)   + x86_frame_store64(aoffu, "rax");
            s += x86_frame_load64("rax", voff+8) + x86_frame_store64(aoffu+8, "rax");
        }
    }
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_gvar_define_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    int64_t narg = _.op_ival;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    IR_t * spec = (narg > 0 && subs && subs[0]) ? subs[0]->entry : NULL;
    const char * specstr = (spec && spec->t == IR_LIT_S && spec->sval) ? spec->sval : "";
    if (MEDIUM_TEXT) {
        std::string fl = emit_fmt(".Ldefspec%d", _.nid);
        std::string s = x86("label", _.lbl_α)
            + x86("comment", "BOX IR_CALL DEFINE(spec) -> rt_proc_define [single-shot success]")
            + x86("directive", ".section .rodata")
            + x86("directive", (fl + ": .string \"" + specstr + "\"").c_str())
            + x86("directive", ".section .text") + x86("directive", ".intel_syntax noprefix");
        s += x86("ins2", "lea", emit_fmt("rdi, [rip+%s]", fl.c_str()));
        s += x86("ins2", "call", "rt_proc_define@PLT");
        s += x86("ins2", "jmp", _.lbl_γ);
        s += x86("Lins1", emit_fmt("%s:", _.lbl_β), "");
        s += x86("ins2", "jmp", _.lbl_ω);
        return s;
    }
    if (MEDIUM_BINARY) {
        uint64_t fptr; { DESCR_t (*fp)(const char *) = rt_proc_define; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        std::string s;
        s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)specstr);
        s += x86_call_ro("rt_proc_define", fptr);
        s += x86("jmp", PORT_GAMMA);
        s += x86("def", PORT_BETA);
        s += x86("jmp", PORT_OMEGA);
        return s;
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_gvar_userproc_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn   = _.op_sval ? _.op_sval : "";
    int64_t      narg = _.op_ival;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    int resoff  = bb_slot_alloc16(_.node);
    int argbase = (narg > 0) ? bb_slot_alloc16(subs[0]->entry) : resoff;
    for (int i = 1; i < (int)narg; i++) bb_slot_alloc16(subs[i]->entry);
    if (MEDIUM_TEXT) {
        std::string s = x86("label", _.lbl_α)
            + x86("comment", emit_fmt("BOX IR_CALL %s(...) -> rt_call_named_proc [four-port, FAIL->ω]", fn));
        uint64_t brm = rt_proc_byref_mask(fn);
        for (int i = 0; i < (int)narg; i++)
            s += ((brm >> i) & 1ull) ? marshal_varparam_addr(subs[i]->entry, argbase + i * 16, i) : marshal_call_arg(subs[i]->entry, argbase + i * 16, _.node, i);
        std::string fl = emit_fmt(".Lprocfn%d", _.nid);
        s += x86("directive", ".section .rodata")
           + x86("directive", (fl + ": .string \"" + fn + "\"").c_str())
           + x86("directive", ".section .text") + x86("directive", ".intel_syntax noprefix");
        s += x86("ins2", "lea", emit_fmt("rdi, [rip+%s]", fl.c_str()));
        s += x86("ins2", "lea", emit_fmt("rsi, [r12+%d]", argbase));
        s += x86("ins2", "mov", emit_fmt("edx, %lld", (long long)narg));
        int mig = rt_proc_frame_nslots(fn) >= 0;
        if (mig) s += pas_sl_setup(fn);
        s += x86("ins2", "call", mig ? "rt_call_named_proc_sl@PLT" : "rt_call_named_proc@PLT");
        s += x86("ins2", "mov", emit_fmt("[r12+%d], rax", resoff));
        s += x86("ins2", "mov", emit_fmt("[r12+%d], rdx", resoff + 8));
        s += x86("ins2", "cmp", "eax, 99");
        s += x86("ins2", "je", _.lbl_ω);
        s += x86("ins2", "jmp", _.lbl_γ);
        s += x86("Lins1", emit_fmt("%s:", _.lbl_β), "");
        s += x86("ins2", "jmp", _.lbl_ω);
        return s;
    }
    if (MEDIUM_BINARY) {
        std::string s;
        uint64_t brm = rt_proc_byref_mask(fn);
        for (int i = 0; i < (int)narg; i++)
            s += ((brm >> i) & 1ull) ? marshal_varparam_addr(subs[i]->entry, argbase + i * 16, i) : marshal_call_arg(subs[i]->entry, argbase + i * 16, _.node, i);
        int mig = rt_proc_frame_nslots(fn) >= 0;
        uint64_t fptr;
        if (mig) { DESCR_t (*fp)(const char *, DESCR_t *, int, void *) = rt_call_named_proc_sl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        else     { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_named_proc; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)fn);
        s += x86_frame_lea("rsi", argbase);
        s += x86("mov32", "edx", (long)narg);
        if (mig) s += pas_sl_setup(fn);
        s += x86_call_ro(mig ? "rt_call_named_proc_sl" : "rt_call_named_proc", fptr);
        s += x86_frame_store64(resoff, "rax");
        s += x86_frame_store64(resoff + 8, "rdx");
        s += x86("cmp", "eax", (long)99);
        s += x86("je", PORT_OMEGA);
        s += x86("jmp", PORT_GAMMA);
        s += x86("def", PORT_BETA);
        s += x86("jmp", PORT_OMEGA);
        return s;
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_byname_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn   = _.op_sval ? _.op_sval : "";
    int64_t      narg = _.op_ival;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    int resoff  = bb_slot_alloc16(_.node);
    int argbase = (narg > 0 && subs && subs[0]) ? bb_slot_alloc16(subs[0]->entry) : resoff;
    for (int i = 1; i < (int)narg; i++) if (subs && subs[i]) bb_slot_alloc16(subs[i]->entry);
    if (MEDIUM_TEXT) {
        std::string s = x86("label", _.lbl_α)
            + x86("comment", emit_fmt("BOX IR_CALL %s(...) -> rt_call_arr by-name [four-port, FAIL->ω]", fn));
        for (int i = 0; i < (int)narg; i++)
            s += marshal_call_arg(subs && subs[i] ? subs[i]->entry : NULL, argbase + i * 16, _.node, i);
        std::string fl = emit_fmt(".Lbynamefn%d", _.nid);
        s += x86("directive", ".section .rodata")
           + x86("directive", (fl + ": .string \"" + fn + "\"").c_str())
           + x86("directive", ".section .text") + x86("directive", ".intel_syntax noprefix");
        s += x86("ins2", "lea", emit_fmt("rdi, [rip+%s]", fl.c_str()));
        s += x86("ins2", "lea", emit_fmt("rsi, [r12+%d]", argbase));
        s += x86("ins2", "mov", emit_fmt("edx, %lld", (long long)narg));
        s += x86("ins2", "call", "rt_call_arr@PLT");
        s += x86("ins2", "mov", emit_fmt("[r12+%d], rax", resoff));
        s += x86("ins2", "mov", emit_fmt("[r12+%d], rdx", resoff + 8));
        s += x86("ins2", "cmp", "eax, 99");
        s += x86("ins2", "je", _.lbl_ω);
        s += x86("ins2", "jmp", _.lbl_γ);
        s += x86("Lins1", emit_fmt("%s:", _.lbl_β), "");
        s += x86("ins2", "jmp", _.lbl_ω);
        return s;
    }
    if (MEDIUM_BINARY) {
        std::string s;
        for (int i = 0; i < (int)narg; i++)
            s += marshal_call_arg(subs && subs[i] ? subs[i]->entry : NULL, argbase + i * 16, _.node, i);
        uint64_t fptr; { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_arr; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)fn);
        s += x86_frame_lea("rsi", argbase);
        s += x86("mov32", "edx", (long)narg);
        s += x86_call_ro("rt_call_arr", fptr);
        s += x86_frame_store64(resoff, "rax");
        s += x86_frame_store64(resoff + 8, "rdx");
        s += x86("cmp", "eax", (long)99);
        s += x86("je", PORT_OMEGA);
        s += x86("jmp", PORT_GAMMA);
        s += x86("def", PORT_BETA);
        s += x86("jmp", PORT_OMEGA);
        return s;
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn   = _.op_sval ? _.op_sval : "";
    int64_t      narg = _.op_ival;
    IR_t       * a0   = _.node->α;
    if (g_descr_flat_chain && _.op_dval == 2.0) return x86_bomb("IR_CALL dval=2 descr-chain arm aborted per LANGUAGE-BLIND rule");
    if (g_gvar_flat_chain && _.op_dval == 5.0) return bb_call_gvar_define_str(pBB);
    if (g_gvar_flat_chain && (_.op_dval == 2.0 || _.op_dval == 3.0) && fn && rt_proc_is_registered(fn)) return bb_call_gvar_userproc_str(pBB);
    if (g_descr_flat_chain && fn && rt_proc_is_registered(fn) && _.op_dval == 3.0) return bb_call_proc_staged_str(pBB);
    if (g_gvar_flat_chain && _.op_dval == 3.0 && fn && fn[0] && !rt_proc_is_registered(fn)) return bb_call_byname_str(pBB);
    if (g_gvar_flat_chain && _.op_dval == 2.0 && fn && fn[0] && !rt_proc_is_registered(fn) && !rt_builtin_is_known(fn)) return bb_call_byname_str(pBB);
    if (g_descr_flat_chain && fn && (!strcmp(fn, "write")) && narg == 1 && a0) {
        int off = bb_slot_get(a0);
        if (off >= 0) return bb_call_write_slot_str(pBB);
    }
    int is_write_strlit  = (fn && !strcmp(fn, "write") && narg == 1 && a0 && a0->t == IR_LIT_S && a0->sval);
    int is_write_intexpr = (fn && !strcmp(fn, "write") && narg == 1 && a0 &&
                            (a0->t == IR_BINOP || a0->t == IR_LIT_I || a0->t == IR_TO || a0->t == IR_TO_BY || a0->t == IR_ALT || a0->t == IR_BINOP_GEN || a0->t == IR_VAR ||
                             a0->t == IR_NEG || a0->t == IR_POS || a0->t == IR_NONNULL || a0->t == IR_NULL_TEST || a0->t == IR_NOT || a0->t == IR_SIZE
                                || a0->t == IR_CALL || a0->t == IR_CASE || a0->t == IR_FIELD_GET || a0->t == IR_LIST_BANG || a0->t == IR_LIMIT || a0->t == IR_IDX));
    int arg_is_any = (a0 && (a0->t == IR_VAR || a0->t == IR_BINOP || a0->t == IR_BINOP_GEN || a0->t == IR_ALT ||
                             a0->t == IR_NEG || a0->t == IR_POS || a0->t == IR_NONNULL || a0->t == IR_NULL_TEST || a0->t == IR_NOT || a0->t == IR_SIZE ||
                             a0->t == IR_CALL || a0->t == IR_CASE || a0->t == IR_FIELD_GET || a0->t == IR_LIST_BANG || a0->t == IR_LIMIT || a0->t == IR_IDX ||
                             (a0->t == IR_TO_BY && a0->sval && a0->sval[0] == 'r')));
    int is_userproc = (fn && rt_proc_is_registered(fn) && !is_write_strlit && !is_write_intexpr);
    if (is_userproc) return bb_call_userproc_str(pBB);
    int is_builtin  = (fn && rt_builtin_is_known(fn)       && !is_write_strlit && !is_write_intexpr);
    if (is_builtin)  return bb_call_builtin_str(pBB);
    if (!is_write_strlit && !is_write_intexpr) {
        fprintf(stderr, "[IBB] FATAL bb_call: unsupported call shape fn='%s' narg=%lld a0=%d\n",
                fn, (long long)narg, a0 ? (int)a0->t : -1);
        abort();
    }
    if (is_write_intexpr) {
        int arg_is_ro_binop = (a0 && (a0->t == IR_BINOP || a0->t == IR_TO || a0->t == IR_TO_BY));
        if (arg_is_ro_binop) return bb_call_write_binop_str(pBB);
        return bb_call_write_legacy_str(pBB, arg_is_any);
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_call(IR_t * pBB) { bb_emit_x86(bb_call_str(pBB)); }
