/* bb_unop.cpp — grouped BB template for Icon value-producing unary operators.
   Handles IR_UNOP (op in pBB->ival: TT_MNS/TT_PLS/TT_SIZE/TT_NONNULL) and the
   legacy split kinds IR_NEG/IR_POS/IR_NONNULL/IR_NULL_TEST/IR_NOT/IR_SIZE.

   GZ-11+ STACKLESS ARMS (under g_icn_flat_chain) — grounded per CONSULT-CANONICAL-SOURCES:
   jcon ir_a_Unop / ir_a_Not, Icon ovalue.r (nonnull, null), omisc.r (size), oarith.r (neg/pos).

   ── IR_UNOP mux (TT_MNS / TT_PLS) ───────────────────────────────────────────────────────────
   jcon ir_a_Unop: "-"/"+"/…∈funcs → single-result, resume fails. Icon oarith.r neg(x)/number(x).
   Operand DESCR at [r12+sa]:[r12+sa+8]; read int payload [r12+sa+8], neg(TT_MNS)/pass(TT_PLS),
   write {DT_I,result} to own slot [r12+off]:[r12+off+8]. β → ω.

   ── IR_NOT (split kind, jcon ir_a_Not) ───────────────────────────────────────────────────────
   FLAT-CHAIN PORT TOPOLOGY: jcon wires operand.success → outside this box's ω, operand.failure
   → this box itself (on the operand's ω port). In the flat BFS the NOT box is reached ONLY when
   the operand FAILED. α = "operand just failed → succeed with NULVCL → γ". No value slot to
   inspect — the operand is a control-flow box (relop etc.) with no DESCR slot. Just produce
   NULVCL into own slot and jmp γ. β → ω by the four-port contract.
   Byte layout (BINARY, 34 bytes):
     0: 49 C7 84 24 <u32 off> 00 00 00 00   mov qword [r12+off],0    (DT_SNUL=0) — 12 bytes
    12: 49 C7 84 24 <u32 off+8> 00 00 00 00 mov qword [r12+off+8],0  (payload=0) — 12 bytes
    24: E9 <rel32 γ>                         jmp γ      (γ patch at 25)           —  5 bytes
    29: E9 <rel32 ω>                         β: jmp ω   (β-def 29, ω patch at 30) —  5 bytes
    34: end

   ── IR_NONNULL (\E, Icon ovalue.r {0,1} nonnull) ─────────────────────────────────────────────
   fail or DT_SNUL → ω; else copy operand DESCR to own slot → γ. Consumes operand value slot.
   Byte layout (BINARY, 70 bytes):
     0: cmp[r12+sa],99 — 8; je ω@10 — 6; cmp[r12+sa],0 — 8; je ω@24 — 6;
    28: mov rax,[r12+sa] — 8; mov [r12+off],rax — 8; mov rax,[r12+sa+8] — 8; mov [r12+off+8],rax — 8;
    60: jmp γ@61 — 5; β-def@65: jmp ω@66 — 5. Total 70.

   ── IR_NULL_TEST (/E, Icon ovalue.r {0,1} null) ──────────────────────────────────────────────
   fail or non-null → ω; DT_SNUL → NULVCL to own slot → γ. Consumes operand value slot.
   Byte layout (BINARY, 62 bytes):
     0: cmp[r12+sa],99 — 8; je ω@10 — 6; cmp[r12+sa],0 — 8; jne ω@24 — 6;
    28: mov qword [r12+off],0 — 12; mov qword [r12+off+8],0 — 12;
    52: jmp γ@53 — 5; β-def@57: jmp ω@58 — 5. Total 62.

   ── IR_SIZE (*E, Icon omisc.r operator{1} * size) ────────────────────────────────────────────
   Single-result. Call rt_icn_size_d(lo=rdi, hi=rsi) → rax:rdx = {DT_I, len}. Consumes slot.
   Byte layout (BINARY, 54 bytes):
     0: mov rdi,[r12+sa] — 8; mov rsi,[r12+sa+8] — 8; movabs rax,fn — 10; call rax — 2;
    28: mov [r12+off],rax — 8; mov [r12+off+8],rdx — 8; jmp γ@45 — 5; β@49: jmp ω@50 — 5. Total 54.

   ── IR_NEG / IR_POS split kinds ──────────────────────────────────────────────────────────────
   Same as TT_MNS/TT_PLS: read int payload [r12+sa+8], neg/pass, write {DT_I,result} to own slot.

   LEGACY VSTACK ARM: falls through if off-chain or sa<0. rt_unop_* helpers ABORT (vstack gone).
   Legacy bytes (BINARY, 22 total):
       0: 48 B8 <u64le fn>   movabs rax, &rt_unop_<op>
      10: FF D0              call rax
      12: E9 <u32 γ>         jmp γ   (γ patch at 13)
      17: E9 <u32 ω>         β: jmp ω (β-def 17, ω patch 18)
      22: end
*/
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "ast.h"
#include "descr.h"
int rt_unop_neg(void);
int rt_unop_pos(void);
int rt_unop_nonnull(void);
int rt_unop_null_test(void);
int rt_unop_not(void);
int rt_unop_size(void);
int  bb_slot_get(IR_t * nd);
int  bb_slot_alloc16(IR_t * nd);
struct DESCR_t rt_icn_size_d(uint64_t lo, uint64_t hi);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *bb_unop_helper_name(IR_t * pBB) {
    if (!pBB) return "rt_unop_not";
    switch (pBB->t) {
    case IR_NEG:        return "rt_unop_neg";
    case IR_POS:        return "rt_unop_pos";
    case IR_NONNULL:    return "rt_unop_nonnull";
    case IR_NULL_TEST:  return "rt_unop_null_test";
    case IR_NOT:        return "rt_unop_not";
    case IR_SIZE:       return "rt_unop_size";
    default:            return "rt_unop_not";
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t bb_unop_helper_ptr(IR_t * pBB) {
    int (*fp)(void) = rt_unop_not;
    if (pBB) switch (pBB->t) {
        case IR_NEG:       fp = rt_unop_neg;       break;
        case IR_POS:       fp = rt_unop_pos;       break;
        case IR_NONNULL:   fp = rt_unop_nonnull;   break;
        case IR_NULL_TEST: fp = rt_unop_null_test; break;
        case IR_NOT:       fp = rt_unop_not;       break;
        case IR_SIZE:      fp = rt_unop_size;      break;
        default:           fp = rt_unop_not;       break;
    }
    return (uint64_t)(uintptr_t)(void*)fp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_unop_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — IR_UNOP");
    /* ── GZ-11+ IR_UNOP mux: TT_MNS (-x) / TT_PLS (+x) ───────────────────────────────────────────────── */
    /* jcon ir_a_Unop ("-"∈funcs → single-result) + Icon oarith.r neg(x)/number(x). Operand DESCR at      */
    /* [r12+sa]:[r12+sa+8]; read int payload [r12+sa+8], neg (TT_MNS) or pass (TT_PLS), write {DT_I,res}  */
    /* to own slot [r12+off]:[r12+off+8]. β → ω.                                                          */
    if (g_icn_flat_chain && pBB && pBB->α && (pBB->ival == (int64_t)TT_MNS || pBB->ival == (int64_t)TT_PLS)) {
        int sa = bb_slot_get(pBB->α);
        if (sa >= 0) {
            int off = bb_slot_alloc16(pBB);
            int is_neg = (pBB->ival == (int64_t)TT_MNS);
            if (MEDIUM_BINARY) {
                /*   0  49 8B 84 24 <u32 sa+8>       mov rax,[r12+sa+8]    (operand int payload) — 8 bytes */
                /*   8  (TT_MNS) 48 F7 D8            neg rax                                     — L bytes */
                /*  8+L 49 C7 84 24 <u32 off> <DT_I>  mov qword [r12+off], DT_I (DESCR lo)      — 12 bytes*/
                /*  ... 49 89 84 24 <u32 off+8>       mov [r12+off+8], rax  (DESCR hi: payload)  —  8 bytes*/
                /*  ... E9 <rel32 γ>                  jmp γ   (γ patch at 29+L)                            */
                /*  ... E9 <rel32 ω>                  β: jmp ω (β-def at 33+L)                             */
                std::string opb = is_neg ? bytes(3, "\x48\xF7\xD8") : std::string();
                int L      = (int)opb.size();
                int p_jmpg = 8 + L + 12 + 8;
                int p_jmpb = p_jmpg + 5;
                bin = { {p_jmpg + 1, p_jmpb, p_jmpb + 1}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
                return bytes(4, "\x49\x8B\x84\x24") + u32le((uint32_t)(sa + 8))
                     + opb
                     + bytes(4, "\x49\xC7\x84\x24") + u32le((uint32_t)off) + u32le((uint32_t)DT_I)
                     + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)(off + 8))
                     + bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0);
            }
            if (MEDIUM_TEXT) {
                std::string s =
                       s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment(emit_fmt("# BOX IR_UNOP %s [GZ-11+ stackless slot→slot DESCR]", is_neg ? "NEG" : "POS"))
                     + s_2asm("mov", emit_fmt("rax, [r12+%d]", sa + 8));
                if (is_neg) s += s_1asm("neg rax");
                s +=   s_2asm("mov", emit_fmt("qword ptr [r12+%d], %d", off, (int)DT_I))
                     + s_2asm("mov", emit_fmt("[r12+%d], rax", off + 8))
                     + s_2asm("jmp", _.lbl_γ)
                     + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                     + s_2asm("jmp", _.lbl_ω);
                return s;
            }
        }
    }
    /* ── GZ-11+ split-kind stackless arms (g_icn_flat_chain only) ─────────────────────────────────────── */
    if (g_icn_flat_chain && pBB) {
        /* ── IR_NOT (jcon ir_a_Not): port-inversion node — reached from operand.ω (fail). Produce NULVCL.*/
        /* No slot read: the operand is a control-flow box (relop/etc.) with no DESCR slot. Simply write  */
        /* {DT_SNUL=0, payload=0} to own slot and jmp γ. β → ω per four-port contract.                   */
        if (pBB->t == IR_NOT) {
            int off = bb_slot_alloc16(pBB);
            if (MEDIUM_BINARY) {
                bin = { {25, 29, 30}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
                return bytes(4, "\x49\xC7\x84\x24") + u32le((uint32_t)off)       + u32le(0)
                     + bytes(4, "\x49\xC7\x84\x24") + u32le((uint32_t)(off + 8)) + u32le(0)
                     + bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0);
            }
            if (MEDIUM_TEXT) {
                return s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment("# BOX IR_NOT [GZ-11+ flat-chain: operand.ω(fail) → NULVCL → γ]")
                     + s_2asm("mov", emit_fmt("qword ptr [r12+%d], 0", off))
                     + s_2asm("mov", emit_fmt("qword ptr [r12+%d], 0", off + 8))
                     + s_2asm("jmp", _.lbl_γ)
                     + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                     + s_2asm("jmp", _.lbl_ω);
            }
        }
        /* ── NONNULL / NULL_TEST / SIZE / NEG / POS: value-consuming — operand has a slot. ────────────── */
        int sa = pBB->α ? bb_slot_get(pBB->α) : -1;
        if (sa >= 0) {
            int off = bb_slot_alloc16(pBB);
            /* ── IR_NONNULL (\E, Icon ovalue.r {0,1} nonnull): fail/null → ω; else copy DESCR → γ.      */
            if (pBB->t == IR_NONNULL) {
                if (MEDIUM_BINARY) {
                    bin = { {10, 24, 61, 65, 66}, {_.lbl_ω_p, _.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, false, false, true, false} };
                    return bytes(4, "\x41\x83\xBC\x24") + u32le((uint32_t)sa) + bytes(1, "\x63")
                         + bytes(2, "\x0F\x84") + u32le(0)
                         + bytes(4, "\x41\x83\xBC\x24") + u32le((uint32_t)sa) + bytes(1, "\x00")
                         + bytes(2, "\x0F\x84") + u32le(0)
                         + bytes(4, "\x49\x8B\x84\x24") + u32le((uint32_t)sa)
                         + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)off)
                         + bytes(4, "\x49\x8B\x84\x24") + u32le((uint32_t)(sa + 8))
                         + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)(off + 8))
                         + bytes(1, "\xE9") + u32le(0)
                         + bytes(1, "\xE9") + u32le(0);
                }
                if (MEDIUM_TEXT) {
                    return s_1asm(emit_fmt("%s:", _.lbl_α))
                         + s_comment("# BOX IR_NONNULL [GZ-11+ stackless: fail/null → ω; copy DESCR → γ]")
                         + s_2asm("cmp", emit_fmt("dword ptr [r12+%d], 99", sa))
                         + s_2asm("je",  _.lbl_ω)
                         + s_2asm("cmp", emit_fmt("dword ptr [r12+%d], 0", sa))
                         + s_2asm("je",  _.lbl_ω)
                         + s_2asm("mov", emit_fmt("rax, [r12+%d]", sa))
                         + s_2asm("mov", emit_fmt("[r12+%d], rax", off))
                         + s_2asm("mov", emit_fmt("rax, [r12+%d]", sa + 8))
                         + s_2asm("mov", emit_fmt("[r12+%d], rax", off + 8))
                         + s_2asm("jmp", _.lbl_γ)
                         + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                         + s_2asm("jmp", _.lbl_ω);
                }
            }
            /* ── IR_NULL_TEST (/E, Icon ovalue.r {0,1} null): fail/non-null → ω; DT_SNUL → NULVCL → γ. */
            if (pBB->t == IR_NULL_TEST) {
                if (MEDIUM_BINARY) {
                    bin = { {10, 24, 53, 57, 58}, {_.lbl_ω_p, _.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, false, false, true, false} };
                    return bytes(4, "\x41\x83\xBC\x24") + u32le((uint32_t)sa) + bytes(1, "\x63")
                         + bytes(2, "\x0F\x84") + u32le(0)
                         + bytes(4, "\x41\x83\xBC\x24") + u32le((uint32_t)sa) + bytes(1, "\x00")
                         + bytes(2, "\x0F\x85") + u32le(0)
                         + bytes(4, "\x49\xC7\x84\x24") + u32le((uint32_t)off)       + u32le(0)
                         + bytes(4, "\x49\xC7\x84\x24") + u32le((uint32_t)(off + 8)) + u32le(0)
                         + bytes(1, "\xE9") + u32le(0)
                         + bytes(1, "\xE9") + u32le(0);
                }
                if (MEDIUM_TEXT) {
                    return s_1asm(emit_fmt("%s:", _.lbl_α))
                         + s_comment("# BOX IR_NULL_TEST [GZ-11+ stackless: fail/non-null → ω; null → NULVCL → γ]")
                         + s_2asm("cmp", emit_fmt("dword ptr [r12+%d], 99", sa))
                         + s_2asm("je",  _.lbl_ω)
                         + s_2asm("cmp", emit_fmt("dword ptr [r12+%d], 0", sa))
                         + s_2asm("jne", _.lbl_ω)
                         + s_2asm("mov", emit_fmt("qword ptr [r12+%d], 0", off))
                         + s_2asm("mov", emit_fmt("qword ptr [r12+%d], 0", off + 8))
                         + s_2asm("jmp", _.lbl_γ)
                         + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                         + s_2asm("jmp", _.lbl_ω);
                }
            }
            /* ── IR_SIZE (*E, Icon omisc.r operator{1} size): call rt_icn_size_d(lo,hi) → {DT_I,len}.   */
            if (pBB->t == IR_SIZE) {
                uint64_t fptr; { struct DESCR_t (*fp)(uint64_t,uint64_t) = rt_icn_size_d; fptr = (uint64_t)(uintptr_t)(void*)fp; }
                if (MEDIUM_BINARY) {
                    bin = { {45, 49, 50}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
                    return bytes(4, "\x49\x8B\xBC\x24") + u32le((uint32_t)sa)
                         + bytes(4, "\x49\x8B\xB4\x24") + u32le((uint32_t)(sa + 8))
                         + bytes(2, "\x48\xB8")          + u64le(fptr)
                         + bytes(2, "\xFF\xD0")
                         + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)off)
                         + bytes(4, "\x49\x89\x94\x24") + u32le((uint32_t)(off + 8))
                         + bytes(1, "\xE9") + u32le(0)
                         + bytes(1, "\xE9") + u32le(0);
                }
                if (MEDIUM_TEXT) {
                    return s_1asm(emit_fmt("%s:", _.lbl_α))
                         + s_comment("# BOX IR_SIZE [GZ-11+ stackless: rt_icn_size_d(DESCR) → {DT_I,len}]")
                         + s_2asm("mov", emit_fmt("rdi, [r12+%d]", sa))
                         + s_2asm("mov", emit_fmt("rsi, [r12+%d]", sa + 8))
                         + s_2asm("call", "rt_icn_size_d@PLT")
                         + s_2asm("mov", emit_fmt("[r12+%d], rax", off))
                         + s_2asm("mov", emit_fmt("[r12+%d], rdx", off + 8))
                         + s_2asm("jmp", _.lbl_γ)
                         + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                         + s_2asm("jmp", _.lbl_ω);
                }
            }
            /* ── IR_NEG / IR_POS split kinds: same as TT_MNS/TT_PLS above — read int payload, neg/pass. */
            if (pBB->t == IR_NEG || pBB->t == IR_POS) {
                int is_neg = (pBB->t == IR_NEG);
                if (MEDIUM_BINARY) {
                    std::string opb = is_neg ? bytes(3, "\x48\xF7\xD8") : std::string();
                    int L      = (int)opb.size();
                    int p_jmpg = 8 + L + 12 + 8;
                    int p_jmpb = p_jmpg + 5;
                    bin = { {p_jmpg + 1, p_jmpb, p_jmpb + 1}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
                    return bytes(4, "\x49\x8B\x84\x24") + u32le((uint32_t)(sa + 8))
                         + opb
                         + bytes(4, "\x49\xC7\x84\x24") + u32le((uint32_t)off) + u32le((uint32_t)DT_I)
                         + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)(off + 8))
                         + bytes(1, "\xE9") + u32le(0)
                         + bytes(1, "\xE9") + u32le(0);
                }
                if (MEDIUM_TEXT) {
                    std::string s =
                           s_1asm(emit_fmt("%s:", _.lbl_α))
                         + s_comment(emit_fmt("# BOX IR_%s [GZ-11+ stackless split-kind slot→slot DESCR]", is_neg ? "NEG" : "POS"))
                         + s_2asm("mov", emit_fmt("rax, [r12+%d]", sa + 8));
                    if (is_neg) s += s_1asm("neg rax");
                    s +=   s_2asm("mov", emit_fmt("qword ptr [r12+%d], %d", off, (int)DT_I))
                         + s_2asm("mov", emit_fmt("[r12+%d], rax", off + 8))
                         + s_2asm("jmp", _.lbl_γ)
                         + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                         + s_2asm("jmp", _.lbl_ω);
                    return s;
                }
            }
        }
    }
    /* ── LEGACY VSTACK ARM — off-chain or sa<0; rt_unop_* helpers ABORT (vstack removed GZ-3). ─────────*/
    const char *hn = bb_unop_helper_name(pBB);
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX IR_UNOP (%s legacy — vstack; ABORT in stackless build)", hn))
             + s_2asm("call", emit_fmt("%s@PLT", hn))
             + s_2asm("jmp",  _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",  _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        uint64_t fptr = bb_unop_helper_ptr(pBB);
        bin = { {13, 17, 18}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(2, "\x48\xB8") + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(1, "\xE9")     + u32le(0)
             + bytes(1, "\xE9")     + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_unop(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_unop_str(pBB, bin), bin); }
