/* bb_binop.cpp — BB template for IR_BINOP (Icon arithmetic + relop apply).
   IBB-3 (Opus 4.7, 2026-05-28): arithmetic apply. IBB-8b (Opus 4.8, 2026-05-29): relop + strrel apply.

   Two apply shapes, both AG-pure (operands already on the vstack, pushed by chain predecessors):

   ARITHMETIC (BINOP_ADD..MOD, POW): rt_arith(sm_op) pops 2, pushes result, then jmp γ.
     movabs rdi, sm_op ; movabs rax, &rt_arith ; call rax ; jmp γ ; β: jmp ω   (32 bytes)
     Driven by flat_drive_binop_tree (legacy tree-shape α=lhs/β=rhs) OR as an AG-pure apply.

   RELOP / STRREL (BINOP_LT..NE numeric, BINOP_SLT..SNE string): the condition of an Icon
     if/while.  rt_acomp(tt_op) (numeric) or rt_lcomp(tt_op) (string) pops 2, pushes the result
     (value on success, FAIL on relop-false) and sets the LAST_OK flag.  The relop then jmps γ
     UNCONDITIONALLY — both ports of an AG-pure relop point at the IR_IF router (mode-2 bb_exec.c
     IR_BINOP AG-pure arm returns γ on success and ω on fail, but lower_new_If_ag wires γ==ω==
     IR_IF, so the single jmp γ here lands at the router regardless).  IR_IF (bb_if.cpp) reads
     LAST_OK and branches then/else.
       movabs rdi, tt_op ; movabs rax, &rt_acomp|&rt_lcomp ; call rax ; jmp γ ; β: jmp ω   (32 bytes)

   ICN_BINOP_* to SM_/TT_ mapping mirrors bb_binop_gen.cpp. BINOP_CONCAT routes through
   bb_lconcat (not here). */
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../../runtime/interp/gen.h"
void rt_arith(int op);
void rt_acomp(int op);
void rt_lcomp(int op);
void rt_gen_concat(void);
int  bb_slot_alloc(IR_t * nd);
int  bb_slot_alloc16(IR_t * nd);
DESCR_t str_concat_d(DESCR_t a, DESCR_t b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gen_to_sm(int64_t op) {
    switch (op) {
    case BINOP_ADD: return (int)SM_ADD;
    case BINOP_SUB: return (int)SM_SUB;
    case BINOP_MUL: return (int)SM_MUL;
    case BINOP_DIV: return (int)SM_DIV;
    case BINOP_MOD: return (int)SM_MOD;
    case BINOP_POW: return (int)SM_EXP;
    default: return -1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gen_is_numrel(int64_t op) { return op >= BINOP_LT  && op <= BINOP_NE; }
static int gen_is_strrel(int64_t op) { return op >= BINOP_SLT && op <= BINOP_SNE; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gen_rel_to_tt(int64_t op) {
    switch (op) {
    case BINOP_LT:  return (int)TT_LT;   case BINOP_LE:  return (int)TT_LE;
    case BINOP_GT:  return (int)TT_GT;   case BINOP_GE:  return (int)TT_GE;
    case BINOP_EQ:  return (int)TT_EQ;   case BINOP_NE:  return (int)TT_NE;
    case BINOP_SLT: return (int)TT_LLT;  case BINOP_SLE: return (int)TT_LLE;
    case BINOP_SGT: return (int)TT_LGT;  case BINOP_SGE: return (int)TT_LGE;
    case BINOP_SEQ: return (int)TT_LEQ;  case BINOP_SNE: return (int)TT_LNE;
    default: return (int)TT_EQ;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_binop_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — IR_BINOP");
    int64_t op = pBB ? pBB->ival : -1;
    /* GZ-3 (GROUND ZERO 3, 2026-05-31): stackless integer binop over two READ-ONLY constant operands.   */
    /* Both operands are IR_LIT_I — compile-time constants. Per the RO-IP-relative + ONE-REGISTER FRAME   */
    /* FACT RULES: the two int64 values are sealed as RO data INSIDE this box's own blob (after the jmps, */
    /* never executed) and read `[rip+disp]` (disp = emit-time constant, data+access share the blob — NO  */
    /* patch, NO absolute address, NO value stack). The result is the box's first READ-WRITE state, so it */
    /* is stored into a per-sequence frame slot `[r12+off]` (ζ=r12), off claimed via bb_slot_alloc; the    */
    /* consumer (write) recovers off via bb_slot_get(pBB). Deterministic single-shot (both operands const) */
    /* → α computes+stores+jmp γ ; β jmp ω (no resume). Grounded in test_icon.c mult (mult_V=a*b). Only    */
    /* ADD/SUB share the `48 0x 05` second-operand encoding; other ops fall through to the rt_arith arm.   */
    if (MEDIUM_BINARY && pBB && pBB->α && pBB->β && pBB->α->t == IR_LIT_I && pBB->β->t == IR_LIT_I
        && (op == BINOP_ADD || op == BINOP_SUB)) {
        int     off = bb_slot_alloc(pBB);
        int64_t v1  = pBB->α->ival;
        int64_t v2  = pBB->β->ival;
        const char *op2 = (op == BINOP_ADD) ? "\x48\x03\x05" : "\x48\x2B\x05";
        /*   off  bytes                       asm                                                          */
        /*   0    48 8B 05 <u32 d1=25>        mov rax,[rip+d1]      (rip-base=7; v1@32; d1=32-7=25)        */
        /*   7    48 0X 05 <u32 d2=26>        add|sub rax,[rip+d2]  (rip-base=14; v2@40; d2=40-14=26)      */
        /*   14   49 89 84 24 <u32 off>       mov [r12+off],rax     (store result into the ζ frame slot)   */
        /*   22   E9 <rel32 → γ>              jmp γ                 ← γ patch at 23                         */
        /*   27   (β defined) E9 <rel32 → ω>  β: jmp ω              ← β-def 27, ω patch 28                  */
        /*   32   <u64 v1>                    sealed RO operand 1   (reached only by [rip+25])              */
        /*   40   <u64 v2>                    sealed RO operand 2   (reached only by [rip+26])              */
        /*   48   end                                                                                       */
        bin = { {23, 27, 28}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(3, "\x48\x8B\x05") + u32le(25u)
             + bytes(3, op2)            + u32le(26u)
             + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)off)
             + bytes(1, "\xE9")         + u32le(0)
             + bytes(1, "\xE9")         + u32le(0)
             + u64le((uint64_t)v1)
             + u64le((uint64_t)v2);
    }
    if (MEDIUM_TEXT && pBB && pBB->α && pBB->β && pBB->α->t == IR_LIT_I && pBB->β->t == IR_LIT_I
        && (op == BINOP_ADD || op == BINOP_SUB)) {
        /* GZ-3 (GROUND ZERO 3), mode-4 TEXT twin of the BINARY int ADD/SUB arm below: both operands are */
        /* READ-ONLY int constants → emit them to .rodata, read IP-relative, compute, and store the      */
        /* result into the per-sequence ζ frame slot [r12+off] (off claimed via bb_slot_alloc, so the    */
        /* consumer's bb_slot_get hits). No value stack. Single-shot (β→ω).                               */
        int     off = bb_slot_alloc(pBB);
        int     nid = bb_node_id(pBB);
        std::string la = emit_fmt(".Lbinop%d_a", nid);
        std::string lb = emit_fmt(".Lbinop%d_b", nid);
        const char *mn = (op == BINOP_ADD) ? "add rax," : "sub rax,";
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX IR_BINOP %s [GZ-3 stackless RO-int → ζ slot]", op == BINOP_ADD ? "ADD" : "SUB"))
             + s_directive(".section .rodata")
             + s_directive(la + emit_fmt(": .quad %lld", (long long)pBB->α->ival))
             + s_directive(lb + emit_fmt(": .quad %lld", (long long)pBB->β->ival))
             + s_directive(".section .text")
             + s_directive(".intel_syntax noprefix")
             + s_2asm("mov rax,", "[rip + " + la + "]")
             + s_2asm(mn,         "[rip + " + lb + "]")
             + s_2asm("mov",      emit_fmt("[r12 + %d], rax", off))
             + s_2asm("jmp",      _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",      _.lbl_ω);
    }
    int is_rel = pBB && (gen_is_numrel(op) || gen_is_strrel(op));
    if (is_rel) {
        int tt   = gen_rel_to_tt(op);
        int strr = gen_is_strrel(op);
        if (MEDIUM_TEXT) {
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX IR_BINOP relop tt=%d (%s: pop 2, push result, set LAST_OK)", tt, strr ? "rt_lcomp" : "rt_acomp"))
                 + s_2asm("mov edi,", emit_fmt("%d", tt))
                 + s_2asm("call",     strr ? "rt_lcomp@PLT" : "rt_acomp@PLT")
                 + s_2asm("jmp",      _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp",      _.lbl_ω);
        }
        if (MEDIUM_BINARY) {
            /* movabs rdi, tt           (10: 48 BF + u64le)                                            */
            /* movabs rax, &rt_*comp    (10: 48 B8 + u64le)                                            */
            /* call rax                  ( 2: FF D0)                                                   */
            /* jmp γ                     ( 5: E9 + u32le)   ← γ patch at offset 23                     */
            /* β: jmp ω                  ( 5: E9 + u32le)   ← β-def at 27, ω patch at 28               */
            uint64_t fptr; { void (*fp)(int) = strr ? rt_lcomp : rt_acomp; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            bin = { {23, 27, 28}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
            return bytes(2, "\x48\xBF") + u64le((uint64_t)(uint32_t)tt)
                 + bytes(2, "\x48\xB8") + u64le(fptr)
                 + bytes(2, "\xFF\xD0")
                 + bytes(1, "\xE9")     + u32le(0)
                 + bytes(1, "\xE9")     + u32le(0);
        }
        return std::string();
    }
    if (op == BINOP_CONCAT) {
        /* GZ-4-string (GROUND ZERO 3, this session): stackless `s1 || s2` over two READ-ONLY string    */
        /* literal operands — the string analog of GZ-3's RO-int ADD/SUB. Both operands are IR_LIT_S     */
        /* compile-time constants: their bytes (NUL-terminated) are sealed RO data INSIDE this box's own  */
        /* blob (after the jmps, never executed) and their ADDRESSES are read `lea …,[rip+disp]` (disp =  */
        /* emit-time constant, data+access share the blob — NO patch, NO absolute address, NO value       */
        /* stack). str_concat_d(DESCR a, DESCR b) (gen_runtime.c — heap-allocates the joined NUL-term     */
        /* buffer) is the non-stack concat helper: SysV passes a→rdi:rsi, b→rdx:rcx and returns the 16-   */
        /* byte result DESCR in rax:rdx. The result is the box's READ-WRITE state → stored into a per-     */
        /* sequence frame DESCR slot [r12+off] (ζ=r12; 16 bytes via bb_slot_alloc16); the consumer (write) */
        /* recovers off via bb_slot_get and reads the payload ptr at [r12+off+8]. Deterministic single-    */
        /* shot (both operands const) → α computes+stores+jmp γ ; β jmp ω. Non-literal operands fall      */
        /* through to the rt_gen_concat arm below (stackless-abort until its own rung).                    */
        if (MEDIUM_BINARY && pBB && pBB->α && pBB->β && pBB->α->t == IR_LIT_S && pBB->β->t == IR_LIT_S
            && pBB->α->sval && pBB->β->sval) {
            int          off    = bb_slot_alloc16(pBB);
            const char * sa     = pBB->α->sval;
            const char * sb     = pBB->β->sval;
            int          la     = (int)strlen(sa) + 1;
            uint64_t     fptr; { DESCR_t (*fp)(DESCR_t, DESCR_t) = str_concat_d; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            /*   off  bytes                       asm                                                         */
            /*   0    BF 01 00 00 00              mov edi,1            (a.v=DT_S; slen=0 — upper cleared)      */
            /*   5    48 8D 35 <u32 d_a=50>       lea rsi,[rip+d_a]    (rip-base=12; str_a@62; d_a=62-12=50)   */
            /*   12   BA 01 00 00 00              mov edx,1            (b.v=DT_S; slen=0)                      */
            /*   17   48 8D 0D <u32 d_b>          lea rcx,[rip+d_b]    (rip-base=24; str_b@62+la; d_b=38+la)   */
            /*   24   48 B8 <u64 &str_concat_d>   movabs rax,&fn                                               */
            /*   34   FF D0                       call rax             (rax:rdx = result DESCR)                */
            /*   36   49 89 84 24 <u32 off>       mov [r12+off],rax    (DESCR eightbyte0 = v+slen)             */
            /*   44   49 89 94 24 <u32 off+8>     mov [r12+off+8],rdx  (DESCR eightbyte1 = ptr)                */
            /*   52   E9 <rel32 → γ>              jmp γ                ← γ patch at 53                         */
            /*   57   E9 <rel32 → ω>              β: jmp ω             ← β-def 57, ω patch 58                  */
            /*   62   <sa bytes + NUL>            sealed RO operand a  (str_a@62)                              */
            /*   62+la <sb bytes + NUL>           sealed RO operand b  (str_b@62+la)                           */
            bin = { {53, 57, 58}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
            return bytes(1, "\xBF") + u32le(1u)
                 + bytes(3, "\x48\x8D\x35") + u32le(50u)
                 + bytes(1, "\xBA") + u32le(1u)
                 + bytes(3, "\x48\x8D\x0D") + u32le((uint32_t)(38 + la))
                 + bytes(2, "\x48\xB8") + u64le(fptr)
                 + bytes(2, "\xFF\xD0")
                 + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)off)
                 + bytes(4, "\x49\x89\x94\x24") + u32le((uint32_t)(off + 8))
                 + bytes(1, "\xE9") + u32le(0)
                 + bytes(1, "\xE9") + u32le(0)
                 + std::string(sa, (size_t)la)
                 + std::string(sb, (size_t)(strlen(sb) + 1));
        }
        if (MEDIUM_TEXT && pBB && pBB->α && pBB->β && pBB->α->t == IR_LIT_S && pBB->β->t == IR_LIT_S
            && pBB->α->sval && pBB->β->sval) {
            /* GZ-4 (GROUND ZERO 3), mode-4 TEXT twin of the BINARY str_concat_d arm above: both operands */
            /* are READ-ONLY string literals → emit them NUL-terminated to .rodata, pass each as a 16-byte */
            /* DESCR in the SysV register pair (a={edi=DT_S:rsi=ptr}, b={edx=DT_S:rcx=ptr}), call          */
            /* str_concat_d, and store the returned DESCR (rax:rdx) into the ζ frame slot [r12+off]/[+8]   */
            /* (16 bytes via bb_slot_alloc16). The consumer (write) reads the payload ptr at [r12+off+8].  */
            int          off = bb_slot_alloc16(pBB);
            int          nid = bb_node_id(pBB);
            std::string  la  = emit_fmt(".Lconcat%d_a", nid);
            std::string  lb  = emit_fmt(".Lconcat%d_b", nid);
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment("# BOX IR_BINOP concat [GZ-4 stackless RO-str → ζ slot DESCR]")
                 + s_directive(".section .rodata")
                 + s_directive(la + ": .string \"" + pBB->α->sval + "\"")
                 + s_directive(lb + ": .string \"" + pBB->β->sval + "\"")
                 + s_directive(".section .text")
                 + s_directive(".intel_syntax noprefix")
                 + s_2asm("mov edi,", "1")
                 + s_2asm("lea rsi,", "[rip + " + la + "]")
                 + s_2asm("mov edx,", "1")
                 + s_2asm("lea rcx,", "[rip + " + lb + "]")
                 + s_2asm("call",     "str_concat_d@PLT")
                 + s_2asm("mov",      emit_fmt("[r12 + %d], rax", off))
                 + s_2asm("mov",      emit_fmt("[r12 + %d], rdx", off + 8))
                 + s_2asm("jmp",      _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp",      _.lbl_ω);
        }
        if (MEDIUM_TEXT) {
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment("# BOX IR_BINOP concat (rt_gen_concat: pop 2, push CONCAT result, set LAST_OK)")
                 + s_2asm("call", "rt_gen_concat@PLT")
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp",  _.lbl_ω);
        }
        if (MEDIUM_BINARY) {
            /* rt_gen_concat takes no argument, but keep the proven 32-byte arith layout (dead movabs    */
            /* rdi,0 so γ/β/ω patch offsets stay {23,27,28}); rt_gen_concat ignores rdi.                 */
            /* movabs rdi,0 (10) ; movabs rax,&rt_gen_concat (10) ; call rax (2) ; jmp γ (5) ; β: jmp ω (5). */
            uint64_t fptr; { void (*fp)(void) = rt_gen_concat; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            bin = { {23, 27, 28}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
            return bytes(2, "\x48\xBF") + u64le(0)
                 + bytes(2, "\x48\xB8") + u64le(fptr)
                 + bytes(2, "\xFF\xD0")
                 + bytes(1, "\xE9")     + u32le(0)
                 + bytes(1, "\xE9")     + u32le(0);
        }
        return std::string();
    }
    int sm_op = pBB ? gen_to_sm(op) : -1;
    if (sm_op < 0) {
        if (MEDIUM_BINARY || MEDIUM_TEXT) {
            fprintf(stderr, "[IBB] FATAL bb_binop: unsupported op ival=%lld (arith + relop only)\n",
                    pBB ? (long long)pBB->ival : -1LL);
            abort();
        }
    }
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX IR_BINOP op=%d (apply: rt_arith pops 2, pushes result)", sm_op))
             + s_2asm("mov edi,", emit_fmt("%d", sm_op))
             + s_2asm("call",     "rt_arith@PLT")
             + s_2asm("jmp",      _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",      _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        /* movabs rdi, sm_op (10) ; movabs rax, &rt_arith (10) ; call rax (2) ; jmp γ (5) ; β: jmp ω (5). */
        /* Total 32. Patch: 23 → γ, 27 → β-def, 28 → ω. Same layout as bb_lit_scalar IR_LIT_I path.     */
        uint64_t fptr; { void (*fp)(int) = rt_arith; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bin = { {23, 27, 28}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(2, "\x48\xBF") + u64le((uint64_t)(uint32_t)sm_op)
             + bytes(2, "\x48\xB8") + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(1, "\xE9")     + u32le(0)
             + bytes(1, "\xE9")     + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_binop(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_binop_str(pBB, bin), bin);
}
