#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
#include "emit_bb.h"
#include "../../runtime/interp/gen.h"
void rt_write_str_nl(const char *s, uint32_t slen);
void rt_write_int_nl(int64_t v);
void rt_write_any_nl(DESCR_t d);
void rt_write_strz_nl(const char *s);
void rt_pop_write_int_nl(void);
void rt_pop_write_any_nl(void);
void rt_call_proc(const char *name, int nargs);
DESCR_t rt_icn_call_proc_descr(const char *name, int nargs);
void rt_icn_arg_stage(int idx, DESCR_t v);
int  rt_proc_is_registered(const char *name);
void rt_call_builtin(const char *name, int nargs);
int  rt_builtin_is_known(const char *name);
int  bb_slot_get(IR_t * nd);
int  bb_slot_alloc16(IR_t * nd);
int  bb_varslot(const char * name);
DESCR_t rt_rk_call_arr(const char * fn, DESCR_t * args, int nargs);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* RK-EMIT-2-NEST: marshal one dval==2.0 call-arg into [r12+aoff]. */
static std::string rk_marshal_call_arg(IR_t * lf, int aoff, IR_t * owner, int idx) {
    if (!lf) return std::string();
    if (lf->t == IR_CALL && lf->dval == 2.0) {
        const char * nfn = lf->sval ? lf->sval : "";
        int nn = (int) lf->ival;
        IR_graph_t ** nsubs = (IR_graph_t **)(intptr_t) lf->counter;
        int avbase = (nn > 0) ? bb_slot_alloc16(nsubs[0]->entry) : bb_slot_alloc16(lf);
        for (int j = 1; j < nn; j++) bb_slot_alloc16(nsubs[j]->entry);
        std::string s;
        for (int j = 0; j < nn; j++) s += rk_marshal_call_arg(nsubs[j]->entry, avbase + j * 16, lf, j);
        if (MEDIUM_TEXT) {
            std::string fl = emit_fmt(".Lrkfn%d", bb_node_id(lf));
            s += s_directive(".section .rodata")
               + s_directive(fl + ": .string \"" + nfn + "\"")
               + s_directive(".section .text") + s_directive(".intel_syntax noprefix");
            s += s_2asm("lea", emit_fmt("rdi, [rip+%s]", fl.c_str()));
            s += s_2asm("lea", emit_fmt("rsi, [r12+%d]", avbase));
            s += s_2asm("mov", emit_fmt("edx, %d", nn));
            s += s_2asm("call", "rt_rk_call_arr@PLT");
            s += s_2asm("mov", emit_fmt("[r12+%d], rax", aoff));
            s += s_2asm("mov", emit_fmt("[r12+%d], rdx", aoff + 8));
        } else if (MEDIUM_BINARY) {
            uint64_t nptr = (uint64_t)(uintptr_t) nfn;
            uint64_t fptr; { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_rk_call_arr; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            s += x86_Lrec(x86_b2(0x48,0xBF)) + x86_Lrec(u64le(nptr));
            s += x86_Lrec(x86_b4(0x49,0x8D,0xB4,0x24)) + x86_Lrec(u32le((uint32_t)avbase));
            s += x86_Lrec(x86_b1(0xBA)) + x86_Lrec(u32le((uint32_t)nn));
            s += x86_Lrec(x86_b2(0x48,0xB8)) + x86_Lrec(u64le(fptr));
            s += x86_Lrec(x86_b2(0xFF,0xD0));
            s += x86_frame_store64(aoff, "rax");
            s += x86_frame_store64(aoff + 8, "rdx");
        }
        return s;
    }
    std::string s;
    if (MEDIUM_TEXT) {
        if (lf->t == IR_LIT_I) {
            s += s_2asm("mov", emit_fmt("qword ptr [r12+%d], 6", aoff));
            s += s_2asm("movabs", emit_fmt("rax, %lld", (long long)lf->ival));
            s += s_2asm("mov", emit_fmt("[r12+%d], rax", aoff + 8));
        } else if (lf->t == IR_LIT_F) {
            uint64_t bits; double d = lf->dval; memcpy(&bits, &d, 8);
            s += s_2asm("mov", emit_fmt("qword ptr [r12+%d], 7", aoff));
            s += s_2asm("movabs", emit_fmt("rax, %llu", (unsigned long long)bits));
            s += s_2asm("mov", emit_fmt("[r12+%d], rax", aoff + 8));
        } else if (lf->t == IR_LIT_NUL) {
            s += s_2asm("mov", emit_fmt("qword ptr [r12+%d], 0", aoff));
            s += s_2asm("mov", emit_fmt("qword ptr [r12+%d], 0", aoff + 8));
        } else if (lf->t == IR_LIT_S) {
            std::string sl = emit_fmt(".Lrkarg%d_%d", bb_node_id(owner), idx);
            s += s_directive(".section .rodata")
               + s_directive(sl + ": .string \"" + (lf->sval ? lf->sval : "") + "\"")
               + s_directive(".section .text") + s_directive(".intel_syntax noprefix");
            s += s_2asm("mov", emit_fmt("qword ptr [r12+%d], 1", aoff));
            s += s_2asm("lea", emit_fmt("rax, [rip+%s]", sl.c_str()));
            s += s_2asm("mov", emit_fmt("[r12+%d], rax", aoff + 8));
        } else {
            int voff = bb_varslot(lf->sval ? lf->sval : "");
            s += s_2asm("mov", emit_fmt("rax, [r12+%d]", voff));
            s += s_2asm("mov", emit_fmt("[r12+%d], rax", aoff));
            s += s_2asm("mov", emit_fmt("rax, [r12+%d]", voff + 8));
            s += s_2asm("mov", emit_fmt("[r12+%d], rax", aoff + 8));
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
                s += x86_Lrec(x86_b2(0x48,0xB8)) + x86_Lrec(u64le(v));
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * bb_chain_terminal(IR_t * entry) {
    IR_t * n = entry; int guard = 0;
    while (n && n->γ && n->γ->t != IR_SUCCEED && n->γ->t != IR_FAIL && guard++ < 4096) n = n->γ;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Recover the beta-target from the driver's queued EMIT_PAIR. */
static bb_label_t * bb_call_beta_target() {
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++)
        if (g_emit.xa_bb_emit_pair_define[i] == _.lbl_β_p && g_emit.xa_bb_emit_pair_jmp[i])
            return g_emit.xa_bb_emit_pair_jmp[i];
    return _.lbl_ω_p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn   = pBB->sval ? pBB->sval : "";
    int64_t      narg = pBB->ival;
    IR_t       * a0   = pBB->α;
    /* RK-EMIT-2: general deterministic Raku builtin call (dval==2.0). */
    if (g_icn_flat_chain && pBB->dval == 2.0) {
        IR_graph_t ** subs = (IR_graph_t **)(intptr_t) pBB->counter;
        int args_ok = 1;
        for (int i = 0; i < (int)narg; i++) {
            IR_t * lf = (subs && subs[i]) ? subs[i]->entry : NULL;
            if (!lf) { args_ok = 0; break; }
            int leaf   = (lf->t == IR_LIT_I || lf->t == IR_LIT_S || lf->t == IR_LIT_F || lf->t == IR_LIT_NUL || lf->t == IR_VAR);
            int nested = (lf->t == IR_CALL && lf->dval == 2.0);
            if (!leaf && !nested) { args_ok = 0; break; }
        }
        if (args_ok) {
            int resoff  = bb_slot_alloc16(pBB);
            int argbase = (narg > 0) ? bb_slot_alloc16(subs[0]->entry) : resoff;
            for (int i = 1; i < (int)narg; i++) bb_slot_alloc16(subs[i]->entry);
            if (MEDIUM_TEXT) {
                std::string s = s_1asm(emit_fmt("%s:", _.lbl_α))
                    + s_comment(emit_fmt("# BOX IR_CALL %s(...) [RK-EMIT-2 dval=2 -> rt_rk_call_arr]", fn));
                for (int i = 0; i < (int)narg; i++)
                    s += rk_marshal_call_arg(subs[i]->entry, argbase + i * 16, pBB, i);
                std::string fl = emit_fmt(".Lrkfn%d", bb_node_id(pBB));
                s += s_directive(".section .rodata")
                   + s_directive(fl + ": .string \"" + fn + "\"")
                   + s_directive(".section .text") + s_directive(".intel_syntax noprefix");
                s += s_2asm("lea", emit_fmt("rdi, [rip+%s]", fl.c_str()));
                s += s_2asm("lea", emit_fmt("rsi, [r12+%d]", argbase));
                s += s_2asm("mov", emit_fmt("edx, %lld", (long long)narg));
                s += s_2asm("call", "rt_rk_call_arr@PLT");
                s += s_2asm("mov", emit_fmt("[r12+%d], rax", resoff));
                s += s_2asm("mov", emit_fmt("[r12+%d], rdx", resoff + 8));
                s += s_2asm("jmp", _.lbl_γ);
                s += s_L1asm(emit_fmt("%s:", _.lbl_β), "");
                s += s_2asm("jmp", _.lbl_ω);
                return s;
            }
            if (MEDIUM_BINARY) {
                std::string s;
                for (int i = 0; i < (int)narg; i++)
                    s += rk_marshal_call_arg(subs[i]->entry, argbase + i * 16, pBB, i);
                uint64_t nptr = (uint64_t)(uintptr_t) fn;
                uint64_t fptr; { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_rk_call_arr; fptr = (uint64_t)(uintptr_t)(void*)fp; }
                s += x86_Lrec(x86_b2(0x48,0xBF)) + x86_Lrec(u64le(nptr));
                s += x86_Lrec(x86_b4(0x49,0x8D,0xB4,0x24)) + x86_Lrec(u32le((uint32_t)argbase));
                s += x86_Lrec(x86_b1(0xBA)) + x86_Lrec(u32le((uint32_t)narg));
                s += x86_Lrec(x86_b2(0x48,0xB8)) + x86_Lrec(u64le(fptr));
                s += x86_Lrec(x86_b2(0xFF,0xD0));
                s += x86_frame_store64(resoff, "rax");
                s += x86_frame_store64(resoff + 8, "rdx");
                s += x86("jmp", PORT_GAMMA);
                s += x86("def", PORT_BETA);
                s += x86("jmp", PORT_OMEGA);
                return s;
            }
        }
    }
    /* GZ-10: flat-chain user-procedure call (dval==3.0). */
    if (g_icn_flat_chain && fn && rt_proc_is_registered(fn) && pBB->dval == 3.0) {
        int off = bb_slot_alloc16(pBB);
        IR_graph_t ** argblks = (IR_graph_t **)(intptr_t) pBB->counter;
        bb_label_t * beta_tgt = bb_call_beta_target();
        if (MEDIUM_BINARY) {
            uint64_t stage_fp; { void (*fp)(int, DESCR_t) = rt_icn_arg_stage; stage_fp = (uint64_t)(uintptr_t)(void*)fp; }
            std::string stage;
            for (int i = 0; i < (int)narg; i++) {
                IR_t * prod = bb_chain_terminal(argblks && argblks[i] ? argblks[i]->entry : NULL);
                int slot = prod ? bb_slot_get(prod) : -1;
                if (slot < 0) slot = 0;
                stage += x86_Lrec(x86_b1(0xBF)) + x86_Lrec(u32le((uint32_t)i));
                stage += x86_Lrec(x86_b4(0x49,0x8B,0xB4,0x24)) + x86_Lrec(u32le((uint32_t)slot));
                stage += x86_Lrec(x86_b4(0x49,0x8B,0x94,0x24)) + x86_Lrec(u32le((uint32_t)(slot + 8)));
                stage += x86_Lrec(x86_b2(0x48,0xB8)) + x86_Lrec(u64le(stage_fp));
                stage += x86_Lrec(x86_b2(0xFF,0xD0));
            }
            uint64_t nptr = (uint64_t)(uintptr_t)fn;
            uint64_t fptr; { DESCR_t (*fp)(const char *, int) = rt_icn_call_proc_descr; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            std::string tail;
            tail += x86_Lrec(x86_b2(0x48,0xBF)) + x86_Lrec(u64le(nptr));
            tail += x86_Lrec(x86_b1(0xBE))     + x86_Lrec(u32le((uint32_t)narg));
            tail += x86_Lrec(x86_b2(0x48,0xB8)) + x86_Lrec(u64le(fptr));
            tail += x86_Lrec(x86_b2(0xFF,0xD0));
            tail += x86_frame_store64(off, "rax");
            tail += x86_frame_store64(off + 8, "rdx");
            tail += x86_Lrec(x86_b3(0x83,0xF8,0x63));
            /* je omega, jmp gamma, def beta, jmp beta_tgt */
            tail += x86_Lrec(x86_b2(0x0F,0x84)) + x86_Jrec(PORT_OMEGA);
            tail += x86_Lrec(x86_b1(0xE9))      + x86_Jrec(PORT_GAMMA);
            /* beta is an internal label; define it + jmp beta_tgt */
            tail += x86_Drec(PORT_BETA);
            if (beta_tgt == _.lbl_ω_p) {
                tail += x86("jmp", PORT_OMEGA);
            } else {
                /* jmp to driver-supplied label via pair-loop record */
                tail += x86_Lrec(x86_b1(0xE9));
                tail += (char)'F'; tail += (char)(unsigned char)0; /* pair index 0 — the beta jmp */
            }
            return stage + tail;
        }
        if (MEDIUM_TEXT) {
            int id2 = bb_node_id(pBB);
            std::string nl = emit_fmt(".Lcall%d_pname", id2);
            std::string s = s_1asm(emit_fmt("%s:", _.lbl_α))
                           + s_comment(emit_fmt("# BOX IR_CALL %s(...) [GZ-10 user-proc mode-4]", fn))
                           + s_directive(".section .rodata")
                           + s_directive(nl + ": .string \"" + std::string(fn) + "\"")
                           + s_directive(".section .text")
                           + s_directive(".intel_syntax noprefix");
            for (int i = 0; i < (int)narg; i++) {
                IR_t * prod = bb_chain_terminal(argblks && argblks[i] ? argblks[i]->entry : NULL);
                int slot = prod ? bb_slot_get(prod) : -1;
                if (slot < 0) slot = 0;
                s += s_2asm("mov edi,",  emit_fmt("%d", i))
                  +  s_2asm("mov rsi,",  emit_fmt("[r12+%d]", slot))
                  +  s_2asm("mov rdx,",  emit_fmt("[r12+%d]", slot + 8))
                  +  s_2asm("call",      "rt_icn_arg_stage@PLT");
            }
            s += s_2asm("lea rdi,", "[rip + " + nl + "]")
              +  s_2asm("mov esi,",  emit_fmt("%d", (int)narg))
              +  s_2asm("call",      "rt_icn_call_proc_descr@PLT")
              +  s_2asm("mov",       emit_fmt("[r12+%d], rax", off))
              +  s_2asm("mov",       emit_fmt("[r12+%d], rdx", off + 8))
              +  s_2asm("cmp",       "eax, 99")
              +  s_2asm("je",        _.lbl_ω)
              +  s_2asm("jmp",       _.lbl_γ)
              +  s_L1asm(emit_fmt("%s:", _.lbl_β), "")
              +  s_2asm("jmp",       beta_tgt ? beta_tgt->name : _.lbl_ω);
            return s;
        }
    }
    /* GZ-7 flat-chain: write(E) where E is any single producer box with a slot. */
    if (g_icn_flat_chain && fn && (!strcmp(fn, "write")) && narg == 1 && a0) {
        int off = bb_slot_get(a0);
        if (off >= 0) {
            bb_label_t * beta_tgt = bb_call_beta_target();
            if (MEDIUM_BINARY) {
                uint64_t fptr; { void (*fp)(DESCR_t) = rt_write_any_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
                std::string s;
                /* mov rdi,[r12+off]; mov rsi,[r12+off+8]; movabs rax,fptr; call rax */
                s += x86_Lrec(x86_b4(0x49,0x8B,0xBC,0x24)) + x86_Lrec(u32le((uint32_t)off));
                s += x86_Lrec(x86_b4(0x49,0x8B,0xB4,0x24)) + x86_Lrec(u32le((uint32_t)(off + 8)));
                s += x86_Lrec(x86_b2(0x48,0xB8))            + x86_Lrec(u64le(fptr));
                s += x86_Lrec(x86_b2(0xFF,0xD0));
                s += x86("jmp", PORT_GAMMA);
                s += x86("def", PORT_BETA);
                if (beta_tgt == _.lbl_ω_p) { s += x86("jmp", PORT_OMEGA); }
                else { s += x86_Lrec(x86_b1(0xE9)); s += (char)'F'; s += (char)(unsigned char)0; }
                return s;
            }
            if (MEDIUM_TEXT) {
                return s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment("# BOX IR_CALL write(op) [GZ-7 flat-chain slot -> rt_write_any_nl]")
                     + s_2asm("mov", emit_fmt("rdi, [r12+%d]", off))
                     + s_2asm("mov", emit_fmt("rsi, [r12+%d]", off + 8))
                     + s_2asm("call", "rt_write_any_nl@PLT")
                     + s_2asm("jmp", _.lbl_γ)
                     + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                     + s_2asm("jmp", beta_tgt ? beta_tgt->name : _.lbl_ω);
            }
        }
    }
    /* Write-of-strlit (non-flat-chain, non-slot path): legacy R-HW / GZ-1 form. */
    int is_write_strlit = (fn && !strcmp(fn, "write") && narg == 1 && a0 && a0->t == IR_LIT_S && a0->sval);
    int is_write_intexpr = (fn && !strcmp(fn, "write") && narg == 1 && a0 &&
                            (a0->t == IR_BINOP || a0->t == IR_LIT_I || a0->t == IR_TO || a0->t == IR_TO_BY || a0->t == IR_ALT || a0->t == IR_BINOP_GEN || a0->t == IR_VAR ||
                             a0->t == IR_NEG || a0->t == IR_POS || a0->t == IR_NONNULL || a0->t == IR_NULL_TEST || a0->t == IR_NOT || a0->t == IR_SIZE || a0->t == IR_CALL || a0->t == IR_CASE || a0->t == IR_FIELD_GET || a0->t == IR_LIST_BANG || a0->t == IR_LIMIT || a0->t == IR_IDX));
    int arg_is_any = (a0 && (a0->t == IR_VAR || a0->t == IR_BINOP || a0->t == IR_BINOP_GEN || a0->t == IR_ALT ||
                             a0->t == IR_NEG || a0->t == IR_POS || a0->t == IR_NONNULL || a0->t == IR_NULL_TEST || a0->t == IR_NOT || a0->t == IR_SIZE ||
                             a0->t == IR_CALL || a0->t == IR_CASE || a0->t == IR_FIELD_GET || a0->t == IR_LIST_BANG || a0->t == IR_LIMIT || a0->t == IR_IDX ||
                             (a0->t == IR_TO_BY && a0->sval && a0->sval[0] == 'r')));
    int is_userproc = (fn && rt_proc_is_registered(fn) && !is_write_strlit && !is_write_intexpr);
    if (is_userproc) {
        if (MEDIUM_TEXT) {
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX IR_CALL %s(...) [IBB-9-6 user-proc dispatch]", fn))
                 + s_2asm("call", "rt_call_proc@PLT")
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp",  _.lbl_ω);
        }
        if (MEDIUM_BINARY) {
            uint64_t nptr = (uint64_t)(uintptr_t)fn;
            uint64_t fptr; { void (*fp)(const char *, int) = rt_call_proc; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            std::string s;
            s += x86_Lrec(x86_b2(0x48,0xBF)) + x86_Lrec(u64le(nptr));
            s += x86_Lrec(x86_b1(0xBE))     + x86_Lrec(u32le((uint32_t)narg));
            s += x86_Lrec(x86_b2(0x48,0xB8)) + x86_Lrec(u64le(fptr));
            s += x86_Lrec(x86_b2(0xFF,0xD0));
            s += x86("jmp", PORT_GAMMA);
            s += x86("def", PORT_BETA);
            s += x86("jmp", PORT_OMEGA);
            return s;
        }
        return std::string();
    }
    int is_builtin = (fn && rt_builtin_is_known(fn) && !is_write_strlit && !is_write_intexpr);
    if (is_builtin) {
        if (MEDIUM_TEXT) {
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX IR_CALL %s(...) [IBB-10 builtin dispatch]", fn))
                 + s_2asm("call", "rt_call_builtin@PLT")
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp",  _.lbl_ω);
        }
        if (MEDIUM_BINARY) {
            uint64_t nptr = (uint64_t)(uintptr_t)fn;
            uint64_t fptr; { void (*fp)(const char *, int) = rt_call_builtin; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            std::string s;
            s += x86_Lrec(x86_b2(0x48,0xBF)) + x86_Lrec(u64le(nptr));
            s += x86_Lrec(x86_b1(0xBE))     + x86_Lrec(u32le((uint32_t)narg));
            s += x86_Lrec(x86_b2(0x48,0xB8)) + x86_Lrec(u64le(fptr));
            s += x86_Lrec(x86_b2(0xFF,0xD0));
            s += x86("jmp", PORT_GAMMA);
            s += x86("def", PORT_BETA);
            s += x86("jmp", PORT_OMEGA);
            return s;
        }
        return std::string();
    }
    if (!is_write_strlit && !is_write_intexpr) {
        fprintf(stderr, "[IBB] FATAL bb_call: unsupported call shape fn='%s' narg=%lld a0=%d\n",
                fn, (long long)narg, a0 ? (int)a0->t : -1);
        abort();
    }
    if (is_write_intexpr) {
        int arg_is_ro_int   = (a0 && a0->t == IR_LIT_I);
        int arg_is_ro_binop = (a0 && (a0->t == IR_BINOP || a0->t == IR_TO || a0->t == IR_TO_BY));
        bb_label_t * beta_tgt = bb_call_beta_target();
        if (arg_is_ro_int && MEDIUM_BINARY) {
            uint64_t fptr; { void (*fp)(int64_t) = rt_write_int_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            std::string s;
            s += x86_Lrec(x86_b3(0x48,0x8B,0x3D)) + x86_Lrec(u32le(22u));
            s += x86_Lrec(x86_b2(0x48,0xB8))      + x86_Lrec(u64le(fptr));
            s += x86_Lrec(x86_b2(0xFF,0xD0));
            s += x86("jmp", PORT_GAMMA);
            s += x86("def", PORT_BETA);
            if (beta_tgt == _.lbl_ω_p) { s += x86("jmp", PORT_OMEGA); }
            else { s += x86_Lrec(x86_b1(0xE9)); s += (char)'F'; s += (char)(unsigned char)0; }
            s += x86_Lrec(u64le((uint64_t)a0->ival));
            return s;
        }
        if (arg_is_ro_int && MEDIUM_TEXT) {
            std::string sl = emit_fmt(".Lwrite_int%d", bb_node_id(pBB));
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX IR_CALL write(%lld) [GZ-2 RO-int IP-relative]", (long long)a0->ival))
                 + s_directive(".section .rodata")
                 + s_directive(sl + emit_fmt(": .quad %lld", (long long)a0->ival))
                 + s_directive(".section .text")
                 + s_directive(".intel_syntax noprefix")
                 + s_2asm("mov rdi,", "[rip + " + sl + "]")
                 + s_2asm("call",     "rt_write_int_nl@PLT")
                 + s_2asm("jmp",      _.lbl_γ);
        }
        if (arg_is_ro_binop) {
            int off = bb_slot_get(a0);
            if (off < 0) { fprintf(stderr, "[GZ-3] FATAL bb_call: write(binop) — slot miss\n"); abort(); }
            if (MEDIUM_BINARY) {
                if (a0->t == IR_BINOP && a0->ival == BINOP_CONCAT) {
                    uint64_t fptr; { void (*fp)(const char *) = rt_write_strz_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
                    std::string s;
                    s += x86_Lrec(x86_b4(0x49,0x8B,0xBC,0x24)) + x86_Lrec(u32le((uint32_t)(off + 8)));
                    s += x86_Lrec(x86_b2(0x48,0xB8)) + x86_Lrec(u64le(fptr));
                    s += x86_Lrec(x86_b2(0xFF,0xD0));
                    s += x86("jmp", PORT_GAMMA); s += x86("def", PORT_BETA);
                    if (beta_tgt == _.lbl_ω_p) s += x86("jmp", PORT_OMEGA);
                    else { s += x86_Lrec(x86_b1(0xE9)); s += (char)'F'; s += (char)(unsigned char)0; }
                    return s;
                }
                uint64_t fptr; { void (*fp)(int64_t) = rt_write_int_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
                std::string s;
                s += x86_Lrec(x86_b4(0x49,0x8B,0xBC,0x24)) + x86_Lrec(u32le((uint32_t)off));
                s += x86_Lrec(x86_b2(0x48,0xB8)) + x86_Lrec(u64le(fptr));
                s += x86_Lrec(x86_b2(0xFF,0xD0));
                s += x86("jmp", PORT_GAMMA); s += x86("def", PORT_BETA);
                if (beta_tgt == _.lbl_ω_p) s += x86("jmp", PORT_OMEGA);
                else { s += x86_Lrec(x86_b1(0xE9)); s += (char)'F'; s += (char)(unsigned char)0; }
                return s;
            }
            if (MEDIUM_TEXT) {
                std::string tail = s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                                 + s_2asm("jmp", beta_tgt && beta_tgt->name[0] ? beta_tgt->name : _.lbl_ω);
                if (a0->t == IR_BINOP && a0->ival == BINOP_CONCAT)
                    return s_1asm(emit_fmt("%s:", _.lbl_α))
                         + s_2asm("mov rdi,", emit_fmt("[r12 + %d]", off + 8))
                         + s_2asm("call",     "rt_write_strz_nl@PLT")
                         + s_2asm("jmp",      _.lbl_γ) + tail;
                return s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_2asm("mov rdi,", emit_fmt("[r12 + %d]", off))
                     + s_2asm("call",     "rt_write_int_nl@PLT")
                     + s_2asm("jmp",      _.lbl_γ) + tail;
            }
        }
        const char * trailer_sym = arg_is_any ? "rt_pop_write_any_nl@PLT" : "rt_pop_write_int_nl@PLT";
        if (MEDIUM_TEXT) {
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_2asm("call", trailer_sym)
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp",  _.lbl_ω);
        }
        if (MEDIUM_BINARY) {
            uint64_t fptr;
            if (arg_is_any) { void (*fp)(void) = rt_pop_write_any_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            else            { void (*fp)(void) = rt_pop_write_int_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            std::string s;
            s += x86_Lrec(x86_b2(0x48,0xB8)) + x86_Lrec(u64le(fptr));
            s += x86_Lrec(x86_b2(0xFF,0xD0));
            s += x86("jmp", PORT_GAMMA); s += x86("def", PORT_BETA);
            if (beta_tgt == _.lbl_ω_p) s += x86("jmp", PORT_OMEGA);
            else { s += x86_Lrec(x86_b1(0xE9)); s += (char)'F'; s += (char)(unsigned char)0; }
            return s;
        }
        return std::string();
    }
    /* write(string_literal): R-HW / GZ-1 legacy path. */
    int id = bb_node_id(pBB);
    if (MEDIUM_TEXT) {
        const char * lit = a0->sval;
        uint32_t     len = (uint32_t)strlen(lit);
        std::string  sl  = emit_fmt(".Lcall%d_str", id);
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX IR_CALL write(\"%s\") [GZ-1 strlit]", lit))
             + s_directive(".section .rodata")
             + s_directive(sl + ": .ascii \"" + lit + "\"")
             + s_directive(".section .text")
             + s_directive(".intel_syntax noprefix")
             + s_2asm("lea rdi,", "[rip + " + sl + "]")
             + s_2asm("mov esi,", emit_fmt("%u", len))
             + s_2asm("call",     "rt_write_str_nl@PLT")
             + s_2asm("jmp",      _.lbl_γ);
    }
    if (MEDIUM_BINARY) {
        const char *lit  = a0->sval;
        uint32_t    slen = (uint32_t)strlen(lit);
        uint64_t    fptr; { void (*fp)(const char *, uint32_t) = rt_write_str_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bb_label_t * beta_tgt = bb_call_beta_target();
        /* GZ-1 R-HW-2: RO string sealed in-blob, [rip+27] load. */
        std::string s;
        s += x86_Lrec(x86_b3(0x48,0x8D,0x3D)) + x86_Lrec(u32le(27u));
        s += x86_Lrec(x86_b1(0xBE))            + x86_Lrec(u32le(slen));
        s += x86_Lrec(x86_b2(0x48,0xB8))        + x86_Lrec(u64le(fptr));
        s += x86_Lrec(x86_b2(0xFF,0xD0));
        s += x86("jmp", PORT_GAMMA);
        s += x86("def", PORT_BETA);
        if (beta_tgt == _.lbl_ω_p) s += x86("jmp", PORT_OMEGA);
        else { s += x86_Lrec(x86_b1(0xE9)); s += (char)'F'; s += (char)(unsigned char)0; }
        s += x86_Lrec(std::string(lit, slen));
        return s;
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_call(IR_t * pBB) { bb_emit_x86(bb_call_str(pBB)); }
