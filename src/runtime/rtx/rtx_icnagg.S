/* rtx_icnagg.S -- RTX-8-ICN: the Icon aggregate family. rt_size_d + rt_list_bang_at.
 *
 * CONTRACT: .github/ARCH-ICON-RTX.md. Ladder: .github/GOAL-ICON-RTX.md. Ledger: .github/RTX-CLAIMS.md.
 * Gate: SCRIP_RTX_ICNAGG (tenth family gate -- a ledger event).
 * Both symbols are Icon-EXCLUSIVE rows marked FREE, so no ownership arbitration is owed.
 *
 * WHY THESE TWO (step 0(d), measured s213 over 296 programs that were PROVEN to have run):
 *   rt_list_bang_at  21086 arrivals, scales 102000 -> 408000 at N -> 4N (exactly 4x)
 *   rt_size_d        19632 arrivals, scales   4000 ->  16000 at N -> 4N (exactly 4x)
 * Both confirmed SCALING, which is what 0(d) requires; rt_proc_value was rejected here despite 7505
 * arrivals because it measures FLAT 1 -> 1 (the setup-only signature that trapped rt_call_arr).
 *
 * DESCR_t (rtx_abi.inc): 16 bytes, v int32 at 0, slen uint32 at 4, value union at 8.
 * Two INTEGER eightbytes => SysV passes and returns it in a register PAIR:
 *   ARG n=1: rdi = (slen << 32) | v   rsi = value      RETURN: rax = (slen << 32) | v   rdx = value
 * rt_size_d's C signature takes the descriptor ALREADY SPLIT as (uint64_t lo, uint64_t hi), which is
 * bit-identical to that pair, so rdi/rsi carry it unchanged and no repacking is owed.
 */
#include "rtx_abi.inc"

#define DT_DATA  100

RTX_GATE_DEF(icnagg)

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_size_d(lo, hi) -- Icon's *x. The C body (rt.c) tests IS_FAIL, then DT_SNUL, DT_T, IS_CSET,
 * DT_S, then DT_DATA, then falls to a VARVAL_fn + strlen tail. Only two arms are taken in asm and
 * both are exact, tag-discriminated, and allocation-free:
 *   DT_SNUL -> INTVAL(0)               (C: r.v = DT_I, r.i = 0)
 *   DT_S with a NON-ZERO slen -> INTVAL(slen)   (C: n = v.slen ? v.slen : strlen(v.s))
 * The slen == 0 case is NOT taken here: C must decide between "empty string" and "length unknown,
 * call strlen", and getting that wrong is a silent wrong answer, not a slow one. Everything else --
 * DT_T tables, csets, DT_DATA records and lists, and the VARVAL tail -- tail-jumps to C untouched.
 * The DT_DATA/list arm is deliberately left in C for this rung: it needs the three-field frame_elems
 * / frame_size / gen_type walk, which is RTX-8b's subject, not a leaf. */
RTX_FUNC(rt_size_d)
    RTX_GATE(icnagg, c_rt_size_d)
    cmp     edi, DT_SNUL
    je      .Lsz_zero
    cmp     edi, DT_S
    jne     c_rt_size_d
    mov     rax, rdi
    shr     rax, 32                     /* rax = slen */
    test    eax, eax
    je      c_rt_size_d                 /* slen == 0 => let C choose empty-vs-strlen */
    cmp     eax, -1                     /* slen == 0xFFFFFFFF => CSET, not a string */
    je      c_rt_size_d                 /* IS_CSET_fn is tested BEFORE DT_S in the C body */
    mov     rdx, rax                    /* value eightbyte = the length */
    mov     eax, DT_I                   /* tag eightbyte = DT_I, slen 0 */
    ret
.Lsz_zero:
    xor     edx, edx
    mov     eax, DT_I
    ret
RTX_ENDF(rt_size_d)

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_list_bang_at(obj, idx) -- the !L element fetch. The C body is a thin wrapper:
 *     DESCR_t out; if (list_bang_at(obj, idx, &out)) return out; return FAILDESCR;
 * list_bang_at itself stays C for this rung (it is the three-FIELD_GET_fn walk, RTX-8b's subject).
 * What is removed here is the wrapper's own -O0 ceremony plus the stack round-trip of `out`: the C
 * spills a 16-byte local, passes its address, then reloads it into the return pair. The asm keeps
 * `out` in the red zone and loads the pair directly, so the wrapper costs one call and two moves.
 * Red zone (SysV 128 bytes below rsp) is safe: this is a leaf apart from the call, and the call is
 * made AFTER the red-zone slot is no longer needed as scratch -- we hand list_bang_at a real stack
 * slot instead, since it writes through the pointer while its own frame is live. */
RTX_FUNC(rt_list_bang_at)
    RTX_GATE(icnagg, c_rt_list_bang_at)
    sub     rsp, 24                     /* 16 for out + 8 to realign (entry rsp == 8 mod 16) */
    mov     rcx, rsp                    /* &out */
    call    list_bang_at@PLT            /* (obj lo=rdi, obj hi=rsi, idx=rdx, out=rcx) */
    test    eax, eax
    je      .Lbang_fail
    mov     rax, [rsp]                  /* out.v | out.slen<<32 */
    mov     rdx, [rsp + 8]              /* out.value */
    add     rsp, 24
    ret
.Lbang_fail:
    mov     eax, DT_FAIL
    xor     edx, edx
    add     rsp, 24
    ret
RTX_ENDF(rt_list_bang_at)

.section .note.GNU-stack,"",@progbits
