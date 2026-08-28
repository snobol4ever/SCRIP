/* rtx_icnagg.s -- RTX-8-ICN: the Icon aggregate family. rt_size_d + rt_list_bang_at.
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
/* ⛔ DT_DATA IS NOT DEFINED HERE. It arrives from descr_tags.inc via rtx_abi.inc (s230: "THE tag numbers live there and ONLY there"). A local `#define DT_DATA 100` stood here and SHADOWED that include, baking cmp $0x64 into dat_field_get while every C site compared $0x70 -- one binary, two values for one tag. The guard is a RANGE test (v < DT_DATA -> C), and DT_FAIL is 0x68 = 104, so the stale boundary sat BELOW DT_FAIL and admitted a FAIL descriptor into the record walk. Deleted s237; do not reintroduce a tag literal in this file. */

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
    cmp     dil, DT_SNUL
    je      .Lsz_zero
    cmp     dil, DT_S
    jne     c_rt_size_d
    mov     rax, rdi
    shr     rax, 32                     /* rax = slen */
    test    eax, eax
    je      c_rt_size_d                 /* slen == 0 => let C choose empty-vs-strlen */
    cmp     eax, -1                     /* slen == 0xFFFFFFFF => CSET, not a string */
    je      c_rt_size_d                 /* IS_CSET_fn is tested BEFORE DT_S in the C body */
    mov     rdx, rax                    /* value eightbyte = the length */
    mov     eax, DT_I | (MOD_OP_RT_SIZE_D << 8)   /* tag eightbyte = DT_I, slen 0; stamped (row descr-stamp-asm-mints) */
    ret
.Lsz_zero:
    xor     edx, edx
    mov     eax, DT_I | (MOD_OP_RT_SIZE_D << 8)   /* stamped (row descr-stamp-asm-mints) */
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
    mov     eax, DT_FAIL | (MOD_OP_RT_LIST_BANG_AT << 8)   /* stamped (row descr-stamp-asm-mints) */
    xor     edx, edx
    add     rsp, 24
    ret
RTX_ENDF(rt_list_bang_at)

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* dat_field_get(fname, obj) -- the Icon record field READ, r.f. RTX-8c-ICN, s216-ICN.
 *
 * REUSES THE ICNAGG GATE. It is the same aggregate family, so this is NOT an eleventh gate and NOT a
 * gate ledger event -- the same ruling s214 applied when rt_str_coerce joined SCRIP_RTX_ICNREL.
 *
 * DEFINED IN src/driver/driver_data.c, NOT src/runtime/. That is why a size sweep restricted to
 * src/runtime/**\/*.c cannot see it; mine did not, and the LIVE CURSOR's own 117-site figure is what
 * caught it. The renamed C body therefore lives in the driver TU, not beside the other c_* bodies.
 *
 * WHAT IS DELETED, AND IT IS NOT -O0 CEREMONY: the exported wrapper's only real work is the callee
 * data_field_ptr, whose body is a LINEAR SCAN over blk->fields calling libc strcmp ONCE PER FIELD
 * until it hits. So the cost is O(field index) with a genuine call per step. This port ABSORBS that
 * scan and replaces the per-field strcmp with an inline byte compare -- a deleted libc call per
 * field, removable at ANY -O level, the same shape as RTX-6's deleted strtoll and RTX-6b's strcmp.
 * data_field_ptr's C body STAYS: it is exported (nm: T) and has other C callers (dat_field_set,
 * rt_field_var, by_name_dispatch), so this absorbs it WITHOUT deleting it -- s211's ruling for
 * rt_parse_num_d applied to an exported callee instead of a static one.
 *
 * step 0(g), MEASURED not assumed: bb_field_get.cpp emits `call dat_field_get@PLT` with NO inline
 * tag guard (it only tests eax == 99 AFTER the call), so the guard steers nothing and the CHEAP arm
 * is dominant -- the s214 rt_str_coerce regime, a third time. Interposer over the field workload:
 * 80,000 arrivals / 80,000 non-fail == 100% take the cell-hit arm ported here.
 * step 0(d): 80,000 -> 320,000 at N -> 4N, exactly 4x, SCALES 1:1 with the workload.
 *
 * ARM SPLIT. Ported: inst.v >= DT_DATA, inst.u != 0, blk != 0, nfields > 0, fields != 0, and a field
 * name that MATCHES. Everything else tail-jumps to C, which owns the "WHAT" pseudo-field, the
 * rt_str_method fallback and FAILDESCR. The MISS path costs a duplicated scan (C re-runs
 * data_field_ptr from the top) -- accepted deliberately, because it is measured at 0% of arrivals
 * and duplicating a cold scan is cheaper to reason about than threading a partial result into C.
 *
 * OFFSETS MEASURED EMPIRICALLY THIS SESSION (gcc offsetof probe vs runtime/core/core.h), never
 * copied from prose -- s202's rule. DESCR_t 16B: v@0 slen@4 value@8. DATBLK_t 40B: name@0
 * nfields@8 fields@16. DATINST_t 24B: type@0 fields@8 id@16. DT_DATA=100, DT_FAIL=99.
 *
 * ABI: rdi = fname. obj arrives as the descriptor PAIR, rsi = (slen << 32) | v and rdx = value, so
 * esi IS obj.v and rdx IS obj.u with no unpacking owed. Returns the pair rax = (slen << 32) | v,
 * rdx = value. NO pin is touched: rdi/rdx/rcx/rsi/rax/r8/r9/r10/r11 are all the RTX working set,
 * and this routine is a true leaf on the ported arm -- no call, no stack, no red zone. */
RTX_FUNC(dat_field_get)
    RTX_GATE(icnagg, c_dat_field_get)
    cmp     sil, DT_DATA                /* C: inst.v < DT_DATA -> NULL cell, signed compare on int v */
    jl      c_dat_field_get
    test    rdx, rdx                    /* C: !inst.u */
    je      c_dat_field_get
    mov     r8, [rdx]                   /* r8 = blk = inst.u->type            DATINST_t.type   @0 */
    test    r8, r8                      /* C: !blk */
    je      c_dat_field_get
    mov     ecx, [r8 + 8]               /* ecx = blk->nfields                 DATBLK_t.nfields @8 */
    test    ecx, ecx
    jle     c_dat_field_get             /* empty record: C returns NULL cell and takes its own tail */
    mov     r9, [r8 + 16]               /* r9 = blk->fields (char **)         DATBLK_t.fields  @16 */
    test    r9, r9
    je      c_dat_field_get
    xor     r10d, r10d                  /* i = 0 */
.Ldfg_field:
    mov     r11, [r9 + r10*8]           /* r11 = blk->fields[i] */
    test    r11, r11
    je      .Ldfg_next                  /* C: `blk->fields[i] &&` -- a NULL slot is skipped, not a hit */
    xor     eax, eax                    /* k = 0; inline strcmp(blk->fields[i], fname) */
.Ldfg_byte:
    movzx   r8d, byte ptr [r11 + rax]
    cmp     r8b, byte ptr [rdi + rax]
    jne     .Ldfg_next
    test    r8b, r8b                    /* equal AND both NUL => strcmp == 0 */
    je      .Ldfg_hit
    inc     rax
    jmp     .Ldfg_byte
.Ldfg_next:
    inc     r10d
    cmp     r10d, ecx
    jl      .Ldfg_field
    jmp     c_dat_field_get             /* scan miss: WHAT / rt_str_method / FAILDESCR are all C's */
.Ldfg_hit:
    mov     r8, [rdx + 8]               /* r8 = inst.u->fields (DESCR_t *)    DATINST_t.fields @8 */
    test    r8, r8
    je      c_dat_field_get
    shl     r10, 4                      /* i * sizeof(DESCR_t) == i * 16 */
    add     r8, r10                     /* r8 = &inst.u->fields[i] == the cell */
    mov     rax, [r8]                   /* C: return *cell -- verbatim, including a stored DT_FAIL */
    mov     rdx, [r8 + 8]
    ret
RTX_ENDF(dat_field_get)

.section .note.GNU-stack,"",@progbits
