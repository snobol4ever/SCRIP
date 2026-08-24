/* rtx_icnsub.S — RTX-24-ICN. Contract: .github/ARCH-ICON-RTX.md. READ IT FIRST.
 *
 * SYMBOL: rt_subscript_var  (C of record: src/runtime/pattern_match.c c_rt_subscript_var)
 * GATE:   SCRIP_RTX_ICNSUB — the FOURTEENTH family gate. Icon-allocated by the ledger
 *         (RTX-CLAIMS.md: 177 Icon static sites, RELEASED to ICON-RTX at s214).
 *
 * WHY THIS SYMBOL, MEASURED, NOT ASSUMED (step 0, all six checks + (d2)):
 *   0(a) live at pattern_match.c:1025 . 0(e) NOT already asm (grep --include=*.S = 0) .
 *   0(f) exported T in libscrip_rt.so, and bb_subscript.cpp emits ONE unconditional call .
 *   0(d) DYNAMIC AND IT SCALES: 500,000 entries at N=500k and 2,000,000 at N=2M — exactly
 *        4x at N->4N, measured with an LD_PRELOAD interposer (the sanctioned instrument;
 *        util_rtx_arm_census.sh is STRUCTURALLY BLIND here because it splits sym vs c_sym
 *        and an UNPORTED symbol has no c_ half — recorded because the census reporting
 *        "zero entries" for a symbol that in fact takes 2,000,000 reads as a refusal).
 *   0(g) NO INLINE TAG GUARD in bb_subscript.cpp (no tag cmp, no je, before the call) =>
 *        the RTX-6b regime, not RTX-6's: the callee's own dispatch is the whole story and
 *        the textually-first arms may genuinely be live. s211's rule, applied and recorded.
 *   (d2) WINDOW DOMINANCE, THE s221 RULE, DISCHARGED BY WALL CLOCK: the grading window
 *        (bench_icnsub_list_dispatch.icn, 8M constant-size list reads, result discarded)
 *        runs 904/940/952/920 ms; the SAME loop with the subscript deleted and the identical
 *        arithmetic retained runs 96/96/91/93 ms. => the subscript is ~836 of ~930 ms, i.e.
 *        ~90% of the window, arms non-overlapping, spread 1.05x each, clears MIN_MS=800.
 *   ⛔ AND ONE DISCARDED INSTRUMENT, RECORDED SO IT IS NOT RE-BUILT: an rdtsc self-cost
 *        interposer attributed 813,676,867 cycles (~271 ms) of INCLUSIVE time to this symbol
 *        inside a program whose whole uninstrumented run is 36 ms. A measurement that
 *        exceeds wall time is not a measurement. Its convenient "SELF = 91.8% of inclusive"
 *        was DISCARDED, not reported, and the differential above replaced it. Per §8's
 *        "A SILENT PROBE IS A QUESTION, NOT AN ANSWER", one that lies loudly is worse.
 *
 * WHAT IS PORTED: the DT_DATA LIST arm with an INTEGER subscript, and only that.
 *   Under exactly that shape the C body provably reduces to: validate the list view,
 *   normalise a possibly-negative index, bounds-check, carve a VCELL, fill seven fields,
 *   and return NAMETRAP. Two costs are ELIMINATED rather than skipped:
 *     (1) THE strcmp. rt_list_view (pattern_match.c:21, static inline) calls
 *         strcmp(fields[2].s, "list") on EVERY subscript, plus strcmp(type->fields[0],
 *         "frame_elems") on every cache miss. At RT_OPT=-O0 those are real calls. Here they
 *         are one 4-byte compare + one NUL test, and a qword+dword compare, inline.
 *         ⭐ This is the RTX-13-ICN by-name disease showing up in a SECOND family: SCRIP
 *         discriminates a list from a record by comparing a STRING at run time, per access.
 *     (2) THE -O0 FRAME. Two 16-byte descriptor spills, `DESCR_t bvar = base` as a stack
 *         copy, six reloaded tag compares (IS_VARREF, DT_A, DT_T, DT_DATA, then DT_DATA
 *         AGAIN inside rt_list_view), and ~20 spilled loads/stores around them.
 *   The two remaining callees are ALREADY ASM and are deliberately still called, not
 *   duplicated: to_int (LEAF, rt_asm_helpers.S) is bypassed entirely for DT_I because the
 *   value is already in a register; rt_agg_alloc (ALLOC, rtx_alloc.S) is CALLED, because it
 *   is itself a gated tail-fusion into rt_gcheap_alloc's bump and re-emitting that bump here
 *   would fork the allocator for a second owner. ⛔ THE HONEST CONSEQUENCE, STATED BEFORE
 *   MEASURING: the allocation is NOT removed, so this rung is the RTX-4/RTX-8d shape in
 *   miniature and its ceiling is bounded by the carve's share. EXPECTATION ON RECORD:
 *   1.3-1.8x on the graded window. Anything at or below 1.05x is a NULL and must be
 *   recorded as one, not explained away.
 *
 * ⭐⭐ THE REAL DEFECT, WHICH THIS PORT DOES NOT FIX AND MUST NOT BE READ AS FIXING:
 *   `t := L[i]` in an RVALUE context allocates a 72-byte VCELL_t purely to NAME a cell, and
 *   bb_subscript's consumer immediately rt_derefs it and discards it. 8,000,000 reads =>
 *   8,000,000 allocations, none of them needed. Canonical Icon does not allocate to fetch
 *   a list element (refs/icon-master/src/runtime/oasgn.r / fstruct.r). That is a DESIGN
 *   rung of the RTX-14-ICN class (an rvalue subscript arm that returns the VALUE and never
 *   mints a VCELL), it is worth multiples of this port, and it is filed as RTX-25-ICN.
 *
 * ⭐ SUPERSEDED IN PART: the list below is RTX-24's ORIGINAL scope, kept verbatim
 * because it records what was true when the gate was minted. DT_A landed at
 * RTX-28, DT_T/DT_S at RTX-26, DT_T/DT_I at RTX-29, DT_S substring at RTX-27,
 * and the table MISS at RTX-30. Read each arm's own header for current scope.
 *
 * WHAT IS DELIBERATELY NOT PORTED (C keeps sole ownership):
 *   DT_A arrays . DT_T tables (tbl_key_str + table_find_pair + rt_ws_strdup_c) .
 *   DT_S/DT_SNUL string subscripting . every VARREF base (base.v == DT_N, which the
 *   DT_DATA test rejects by construction, so IS_VARREF needs no separate test here) .
 *   non-integer subscripts (DT_S "3", DT_R) . the rt_list_view REJECT path, which must
 *   reach subscript_get . a null instance or null element vector. Each falls through to
 *   c_rt_subscript_var with rdi/rsi/rdx/rcx UNTOUCHED, so the tail jump stages no
 *   arguments and IS the byte-identity proof path.
 *
 * SEMANTICS THIS ARM MUST NOT BREAK (pattern_match.c:1045-1053, transcribed exactly):
 *   i = (int)to_int(idx);  if (i < 0) i = n + i + 1;  if (!elems || i < 1 || i > n) FAIL;
 *   ⚠ NOTE THE ASYMMETRY AND DO NOT "TIDY" IT: the wrap is `i < 0`, NOT `i <= 0`, so
 *   i == 0 does NOT wrap — it falls to the bounds test and FAILS. (The DT_S arm eleven
 *   lines below uses `i <= 0`, which DOES wrap zero. Two arms, two rules, one function.)
 *   The cast is to `int` (32-bit) BEFORE the compare, so a 64-bit index truncates; `mov
 *   eax, ecx` reproduces that truncation deliberately.
 *
 * REGISTER CONTRACT: touches only the RTX working set (rax rcx rdx rsi rdi r8 r9 r10 r11).
 *   ⭐ Σ/δ/Δ (r13/r14/r15) are NOT read or written — this symbol is off the scan path, so
 *   ARCH §2's Icon-specific live-subject warning does not bind here. rbx/___/rsp untouched.
 *   No FR()/FRQ() appears: this is a .S runtime routine with no ζ frame of its own.
 *
 * ARGS (SysV, two 16-byte all-INTEGER structs — ARCH §2):
 *   base -> rdi = (base.slen << 32) | base.v , rsi = base.u   (DATINST_t *)
 *   idx  -> rdx = (idx.slen  << 32) | idx.v  , rcx = idx.i
 * RETURN: rax = (slen << 32) | v , rdx = payload
 *   success => rax = (2 << 32) | DT_N , rdx = VCELL_t *   [NAMETRAP, descr.h:44]
 *   failure => rax = DT_FAIL          , rdx = 0           [FAILDESCR, descr.h:43]
 *
 * STRUCT OFFSETS BELOW ARE PROBED, NOT REMEMBERED (offsetof, this session), and every one
 * is pinned by a _Static_assert in rtx_init.c. A drift here links fine, allocates fine, and
 * silently returns a cell pointing at the wrong element — the exact failure mode the
 * HB_AGGV and DT_E asserts were minted for.
 */
#include "rtx_abi.inc"

RTX_GATE_DEF(icnsub)

#define DATINST_TYPE      0      /* DATINST_t.type   (DATBLK_t *)  */
#define DATINST_FIELDS    8      /* DATINST_t.fields (DESCR_t *)   */
#define DATBLK_NFIELDS    8      /* DATBLK_t.nfields (int)         */
#define DATBLK_FIELDS    16      /* DATBLK_t.fields  (char **)     */
#define VCELL_CELLP       0
#define VCELL_TBL         8
#define VCELL_KEY        16
#define VCELL_KEY_D      24
#define VCELL_SV         40
#define VCELL_POS        56
#define VCELL_LEN        64
#define VCELL_SIZE       72
#define DESCR_SIZE       16
/* ⛔ DT_DATA IS NOT DEFINED HERE -- it arrives from descr_tags.inc via rtx_abi.inc (s230: "THE tag numbers live there and ONLY there"). The rationale that stood here ("rtx_abi.inc's tag list STOPS AT DT_FAIL 99 and has no DT_DATA") WAS TRUE WHEN WRITTEN AND WAS FALSIFIED BY s230, but the `#define DT_DATA 100` it justified was never removed, so it SHADOWED the include. 100 was chosen as "just above DT_FAIL 99"; DT_FAIL is really 0x68 = 104, so the boundary sat BELOW it. Measured in the shipped .so at s237: dat_field_get compared $0x64 while every C site compared $0x70. The equality arms here (.Lsub_bail) simply never matched, so RTX-24's list fast path was silently dead. Deleted s237; do not reintroduce a tag literal in this file. */
/* ⛔ Precomputed BYTE offsets into the instance field vector, NOT written as
 * k*DESCR_SIZE: GNU as Intel syntax parses `X*Y` inside brackets as index*scale,
 * so [r9 + 0*DESCR_SIZE] is an INDEX REGISTER 0 with scale 16 and fails to
 * assemble rather than folding to [r9]. Literals here, arithmetic in the name. */
#define FIELD0_V          0      /* fields[0].v   -- elems vector tag  */
#define FIELD0_P          8      /* fields[0].ptr -- elems vector      */
#define FIELD1_P         24      /* fields[1].i   -- element count n   */
#define FIELD2_V         32      /* fields[2].v   -- genus tag type    */
#define FIELD2_P         40      /* fields[2].s   -- genus tag bytes   */
/* RTX-26-ICN: the DT_T arm's layout. TBPAIR_t is {char *key; DESCR_t key_descr;
 * DESCR_t val; TBPAIR_t *next;} and TBBLK_t opens with buckets[256], so the
 * bucket vector is at offset ZERO and indexes as [tb + h*8]. All five pinned by
 * _Static_assert in rtx_init.c. ⚠ DT_T IS defined (rtx_abi.inc:63) unlike
 * DT_DATA -- checked, not assumed, because s222 proved an undefined tag compare
 * assembles cleanly as a relocation and admits the wrong datatype silently. */
#define TBPAIR_KEY        0      /* TBPAIR_t.key   (char *)        */
#define TBPAIR_VAL       24      /* TBPAIR_t.val   (DESCR_t)       */
#define TBPAIR_NEXT      40      /* TBPAIR_t.next  (TBPAIR_t *)    */
#define TBBLK_BUCKETS     0      /* TBBLK_t.buckets[TABLE_BUCKETS] */
#define TBL_HASH_SEED  5381      /* _tbl_hash: djb2-xor, aggregates.c:78 */
/* RTX-28-ICN: the DT_A arm's layout. ARBLK_t is {int lo, hi; int ndim; int lo2,
 * hi2; int proto_bare; const char *proto; DESCR_t *data; long id;} = 48 bytes,
 * so `proto` lands at 24 by natural 8-alignment and `data` at 32. ⚠ DT_A IS
 * defined (rtx_abi.inc:62) -- checked, not assumed, same reason as DT_T. All
 * four offsets pinned by _Static_assert in rtx_init.c. */
#define ARBLK_LO          0      /* ARBLK_t.lo     (int)           */
#define ARBLK_HI          4      /* ARBLK_t.hi     (int)           */
#define ARBLK_NDIM        8      /* ARBLK_t.ndim   (int)           */
#define ARBLK_DATA       32      /* ARBLK_t.data   (DESCR_t *)     */

/*-----------------------------------------------------------------------------
 * DESCR_t rt_subscript_var(DESCR_t base, DESCR_t idx)
 *
 * FRAME: 56 bytes. ⛔ ALIGNMENT IS LOAD-BEARING AND 56 IS NOT ARBITRARY. Entry
 * rsp == 8 (mod 16); SysV requires rsp == 0 (mod 16) AT a call; 8 - 56 == -48 == 0
 * (mod 16). Adjustments must be == 8 (mod 16): 8/24/40/56 work, 16/32/48 do NOT.
 *   [rsp+ 0] original rdi   [rsp+16] original rdx      [rsp+32] elems
 *   [rsp+ 8] original rsi   [rsp+24] original rcx      [rsp+40] i-1 (dword)
 *   [rsp+56 .. rsp+87] RTX-29 integer-key scratch (32 B). ⛔ 56 -> 88 AT RTX-29,
 *   and 88 IS NOT ARBITRARY EITHER: it is the same congruence class as 56
 *   (88 == 8 mod 16), so 8 - 88 == -80 == 0 (mod 16) and the SysV alignment
 *   invariant above is preserved unchanged. All eight adjustment sites were
 *   enumerated by grep before the resize; slots 0..47 kept their offsets so
 *   RTX-24/26/27/28 are byte-untouched by it.
 * The four originals are saved so EVERY bail can restore them and tail-jump to C
 * with its arguments byte-identical to what our caller staged -- the byte-identity
 * proof path survives the fact that this arm must call rt_deref before it can even
 * tell whether it applies.
 */
RTX_FUNC(rt_subscript_var)
    RTX_GATE(icnsub, c_rt_subscript_var)

    /* Cheap rejects first, BEFORE any frame, so the common bail costs four
     * compares and a tail jump with the arguments never touched. */
    cmp     dl, DT_I                   /* DT_I  -> RTX-24's DT_DATA list arm   */
    je      .Lsub_tag_ok
    cmp     dl, DT_S                   /* DT_S  -> RTX-26's DT_T table arm     */
    jne     c_rt_subscript_var
.Lsub_tag_ok:
    /* RTX-28: the DT_A array arm branches off HERE, ahead of the DT_N gate,
     * because a SNOBOL4 array subscript arrives with the base ALREADY DEREF'D --
     * MEASURED s224: base.v=4 (DT_A) base.slen=0, never the DT_N varref shape.
     * The gate below would reject it, which is exactly why this arm measured
     * 200,001 entries / 200,001 bailed / 0 commits before it existed. It is
     * RTX-24's lesson inverted: that arm had to stop rejecting varrefs, this one
     * had to stop requiring them. */
    cmp     dil, DT_A
    jne     .Lsub_not_array
    cmp     dl, DT_I                   /* array arm takes DT_I subscripts only  */
    jne     c_rt_subscript_var
    jmp     .Lsub_array
.Lsub_not_array:
    /* RTX-31: a SNOBOL4 TABLE base ALSO arrives ALREADY DEREF'D, same shape as RTX-28's
     * DT_A above -- MEASURED (seat01, 2026-08-24, table_access.sno, gdb hit-count breakpoints):
     * base.v=24 (DT_T) on 10,500/10,500 c_rt_subscript_var entries (every T[I]=v write), and
     * ZERO of those ever reached .Lsub_table_int (RTX-29) below -- the DT_N gate this arm used
     * to fall through to rejected every one of them, so every table WRITE paid a full C call
     * for work this mints in ten instructions. See perf-table-subscript-fastpath task LEDGER. */
    cmp     dil, DT_T
    jne     .Lsub_not_table_direct
    cmp     dl, DT_I                   /* this fast mint covers the DT_I-key traffic (T[I]);
                                        * a DT_S key still bails to C, same as RTX-26 already does */
    jne     c_rt_subscript_var
    jmp     .Lsub_table_direct
.Lsub_not_table_direct:
    cmp     dil, DT_N                   /* base must be the VARREF shape        */
    jne     c_rt_subscript_var
    mov     rax, rdi
    shr     rax, 32                     /* rax = base.slen                      */
    cmp     eax, 1                      /* ...and specifically the slen==1       */
    jne     c_rt_subscript_var          /*    cell-pointer form (MEASURED live)  */
    test    rsi, rsi                    /* IS_VARREF's `slen==1 && ptr` clause   */
    je      c_rt_subscript_var

    sub     rsp, 88
    mov     [rsp + 0], rdi
    mov     [rsp + 8], rsi
    mov     [rsp + 16], rdx
    mov     [rsp + 24], rcx

    call    rt_deref                    /* base = rt_deref(base); already asm    */
    /* The SUBSCRIPT tag selects the arm, and it is read from the spill rather
     * than a live register because rt_deref returns in rax:rdx and clobbers rdx.
     * MEASURED s223: t["k"] arrives base.v=9 slen=1 idx.v=DT_S; L[3] arrives
     * base.v=9 slen=1 idx.v=DT_I. Two arms, one prologue, disjoint selectors. */
    cmp     dword ptr [rsp + 16], DT_S
    je      .Lsub_table
    /* Past here the SUBSCRIPT is DT_I, so the deref'd BASE selects the arm:
     * DT_S -> RTX-27's substring-trap arm, DT_DATA -> RTX-24's list arm. */
    cmp     al, DT_S
    je      .Lsub_string
    cmp     al, DT_T                   /* RTX-29: DT_T with a DT_I key           */
    je      .Lsub_table_int
    cmp     al, DT_DATA                /* only the list/record family here      */
    jne     .Lsub_bail
    mov     rsi, rdx                    /* rsi = base.u (DATINST_t *)            */
    test    rsi, rsi
    je      .Lsub_bail

    /* --- rt_list_view, inlined: the type-level half ------------------------- */
    mov     r8, [rsi + DATINST_TYPE]    /* r8 = t = base.u->type                 */
    test    r8, r8
    je      .Lsub_bail
    cmp     dword ptr [r8 + DATBLK_NFIELDS], 3
    jl      .Lsub_bail                  /* t->nfields < 3                        */
    mov     r9, [r8 + DATBLK_FIELDS]    /* r9 = t->fields (char **)              */
    test    r9, r9
    je      .Lsub_bail
    mov     r10, [r9]                   /* r10 = t->fields[0]                    */
    test    r10, r10
    je      .Lsub_bail
    /* strcmp(t->fields[0], "frame_elems") == 0, inline: 12 bytes with the NUL.  */
    mov     rax, 0x6c655f656d617266     /* "frame_el"                            */
    cmp     [r10], rax
    jne     .Lsub_bail
    cmp     dword ptr [r10 + 8], 0x00736d65  /* "ems\0"                          */
    jne     .Lsub_bail

    /* --- rt_list_view, inlined: the instance-level half --------------------- */
    mov     r9, [rsi + DATINST_FIELDS]  /* r9 = base.u->fields (DESCR_t *)       */
    test    r9, r9
    je      .Lsub_bail
    cmp     dword ptr [r9 + FIELD2_V], DT_S   /* the GENUS TAG must be a string  */
    jne     .Lsub_bail
    mov     r10, [r9 + FIELD2_P]
    test    r10, r10
    je      .Lsub_bail
    cmp     dword ptr [r10], 0x7473696c      /* "list"                           */
    jne     .Lsub_bail
    cmp     byte ptr [r10 + 4], 0            /* ...and the NUL, so not "listen"  */
    jne     .Lsub_bail
    cmp     dword ptr [r9 + FIELD0_V], DT_DATA
    jne     .Lsub_bail                  /* elems would be NULL => C's FAIL path  */
    mov     r8, [r9 + FIELD0_P]         /* r8 = elems (DESCR_t *)                */
    test    r8, r8
    je      .Lsub_bail
    mov     [rsp + 32], r8
    mov     r11d, dword ptr [r9 + FIELD1_P]  /* r11d = n = (int)fields[1].i       */

    /* --- index normalisation and bounds, C's exact rules ------------------- */
    mov     eax, dword ptr [rsp + 24]   /* i = (int)to_int(idx); DT_I => the     */
                                        /*   payload IS the value, truncated to  */
                                        /*   32 bits exactly as the C cast does. */
    test    eax, eax
    jns     .Lsub_nowrap
    lea     eax, [rax + r11 + 1]        /* i < 0  => i = n + i + 1  (NOT i <= 0) */
.Lsub_nowrap:
    cmp     eax, 1
    jl      .Lsub_fail                  /* i < 1                                 */
    cmp     eax, r11d
    jg      .Lsub_fail                  /* i > n                                 */
    dec     eax                         /* the 0-based element index             */
    mov     [rsp + 40], eax

    /* --- carve the VCELL. rt_agg_alloc is already asm (ALLOC gate); call it,  */
    /*     do not fork the bump allocator for a second owner.                   */
    xor     edi, edi                    /* kind = 0                              */
    mov     esi, VCELL_SIZE             /* n    = sizeof(VCELL_t)                */
    call    rt_agg_alloc                /* rax = VCELL_t * (C does not null-check
                                         *   either, so neither do we)           */

    /* --- fill the seven fields, in the C body's own order ------------------ */
    movsxd  rcx, dword ptr [rsp + 40]
    shl     rcx, 4                      /* * sizeof(DESCR_t)                     */
    add     rcx, [rsp + 32]
    mov     [rax + VCELL_CELLP], rcx    /* vc->cellp = &elems[i - 1]             */
    mov     qword ptr [rax + VCELL_TBL], 0
    mov     qword ptr [rax + VCELL_KEY], 0
    mov     r8, [rsp + 16]
    mov     r9, [rsp + 24]
    mov     [rax + VCELL_KEY_D], r8     /* vc->key_d = idx (tag word)            */
    mov     [rax + VCELL_KEY_D + 8], r9 /*            ... (payload)              */
    mov     qword ptr [rax + VCELL_SV], DT_FAIL | (MOD_OP_RT_SUBSCRIPT_VAR << 8)   /* vc->sv = FAILDESCR, stamped (row descr-stamp-asm-mints) */
    mov     qword ptr [rax + VCELL_SV + 8], 0
    mov     qword ptr [rax + VCELL_POS], 0
    mov     qword ptr [rax + VCELL_LEN], 0

    add     rsp, 88
    mov     rdx, rax                    /* rdx = vc                              */
    mov     rax, DT_NAMETRAP_LO | (MOD_OP_RT_SUBSCRIPT_VAR << 8)   /* NAMETRAP, stamped (row descr-stamp-asm-mints) */
    ret

/*-----------------------------------------------------------------------------
 * RTX-26-ICN — THE DT_T TABLE ARM, keyed by a DT_S subscript.
 *
 * The C of record (pattern_match.c c_rt_subscript_var, the base.v == DT_T arm)
 * reduces, under exactly this shape, to: ks = idx.s  (tbl_key_str's DT_S case is
 * a bare `return kd.s` -- no buffer, no formatting) ; e = table_find_pair(tb,ks) ;
 * carve a VCELL ; cellp = &e->val, tbl = tb, key = 0. TWO CALLS ARE ELIMINATED
 * rather than skipped, and at RT_OPT=-O0 both are real calls, per subscript:
 *   (1) _tbl_hash  (aggregates.c:78, static but NOT inlined at -O0) -- here it is
 *       a 6-instruction loop, no frame, no call.
 *   (2) strcmp     -- here an inline byte compare that stops on the first
 *       mismatch or the shared NUL.
 * ⭐ This is the RTX-13-ICN by-name disease in a THIRD family: a table lookup
 *    that hashes and then strcmps a key string on every single access. RTX-24
 *    found it in the list arm (two strcmps for "list"/"frame_elems"); the table
 *    arm cannot delete the compare the way RTX-24 did -- a hash table's key
 *    compare is semantic, not a type tag -- so this arm makes it cheap instead.
 *
 * ⛔ THE MISS MUST TRAP, NOT FAIL. On a miss the C body does NOT fail: it mints
 *    a key-INSERT trap (cellp = 0, key = rt_ws_strdup_c(ks)), which allocates
 *    and copies. Returning FAILDESCR here would silently break `t[k] := v` on a
 *    fresh key. ⭐ UPDATED AT RTX-30: the chain-exhausted edge no longer bails --
 *    it falls to .Lsub_tbl_miss, which mints that trap in asm. The rule it was
 *    protecting is unchanged and is now enforced there; read that arm's header
 *    before touching either. Every OTHER edge on this path still bails.
 * ⛔ THE GUARD IS NARROWER THAN THE C, DELIBERATELY. C reaches its DT_T arm for
 *    any base that derefs to DT_T; this arm additionally requires the
 *    VARREF(slen==1) prologue it shares with RTX-24. That is safe by
 *    construction (anything else bails) and it is what the traffic actually is:
 *    2,000,001 of 2,000,001 arrivals measured base.v=9 slen=1.
 */
.Lsub_table:
    /* ⛔⭐ s262 STAND-DOWN, AND IT IS OBSOLESCENCE, NOT A TODO.  RTX-26/29 existed to find the entry
     * for a DT_T subscript so it could build a VCELL holding `cellp = &e->val`.  Lon ruled that
     * address out of existence -- "we should never have in our code a place that depends on that
     * pointer not moving" -- so rt_subscript_var no longer looks the key up AT ALL: it mints a cell
     * naming (tbl, key_descr) and re-resolves on use.  There is nothing left here to accelerate.
     * The C path this bails to is FASTER than what these arms did, because the whole probe is gone.
     * ⛔ The arms below are also structurally dead: they walk a CHAIN (`e = e->next`) and hash a
     * STRINGIFIED key, and neither exists any more -- a bucket is a sorted contiguous array of
     * 48-byte entries and the hash is per-datatype.  A wrong-shape walk here would read arbitrary
     * memory and still assemble and still link, which is why this is a hard jump and not a comment.
     * ⭐ The hashed lookup itself now lives in rtx_table.S (RTX-TBL), where it belongs. */
    jmp     .Lsub_bail
    cmp     al, DT_T
    jne     .Lsub_bail
    mov     rsi, rdx                    /* rsi = tb (TBBLK_t *)                  */
    test    rsi, rsi
    je      .Lsub_bail
    mov     rdi, [rsp + 24]             /* rdi = idx.s, the key bytes            */
    test    rdi, rdi
    je      .Lsub_bail                  /* C substitutes ""; do not emulate it    */

    /* --- _tbl_hash, inlined: h = 5381; while (*k) h = h*33 ^ (uchar)*k++ ------ */
.Lsub_hash_init:                        /* RTX-29 re-enters here with rdi=key,rsi=tb */
    mov     eax, TBL_HASH_SEED
    mov     r8, rdi
.Lsub_hash:
    movzx   ecx, byte ptr [r8]
    test    cl, cl
    je      .Lsub_hash_done
    mov     r9d, eax
    shl     r9d, 5
    add     r9d, eax                    /* h * 33 == (h << 5) + h, 32-bit wrap   */
    xor     r9d, ecx
    mov     eax, r9d
    inc     r8
    jmp     .Lsub_hash
.Lsub_hash_done:
    movzx   eax, al                     /* h % TABLE_BUCKETS, and 256 makes that */
                                        /*   the low byte -- pinned by assert    */
    mov     r10, [rsi + rax*8 + TBBLK_BUCKETS]   /* e = tb->buckets[h]           */

.Lsub_chain:
    test    r10, r10
    je      .Lsub_tbl_miss              /* RTX-30: MINT the key-insert trap here  */
    mov     r11, [r10 + TBPAIR_KEY]
    test    r11, r11
    je      .Lsub_bail
    mov     r8, rdi                     /* r8 walks the probe key                */
.Lsub_cmp:
    movzx   ecx, byte ptr [r11]
    movzx   edx, byte ptr [r8]
    cmp     cl, dl
    jne     .Lsub_chain_next
    test    cl, cl
    je      .Lsub_hit                   /* both hit NUL together == strcmp 0     */
    inc     r11
    inc     r8
    jmp     .Lsub_cmp
.Lsub_chain_next:
    mov     r10, [r10 + TBPAIR_NEXT]
    jmp     .Lsub_chain

.Lsub_hit:
    lea     rcx, [r10 + TBPAIR_VAL]     /* &e->val                               */
    mov     [rsp + 32], rsi             /* tb and &e->val must survive the carve */
    mov     [rsp + 40], rcx             /*   -- rsi/r10 are both caller-saved    */
    xor     edi, edi                    /* kind = 0                              */
    mov     esi, VCELL_SIZE
    call    rt_agg_alloc                /* already asm (ALLOC gate); do not fork  */
                                        /*   the bump allocator for a 2nd owner   */
    mov     rcx, [rsp + 40]
    mov     [rax + VCELL_CELLP], rcx    /* vc->cellp = &e->val                   */
    mov     rcx, [rsp + 32]
    mov     [rax + VCELL_TBL], rcx      /* vc->tbl   = tb                        */
    mov     qword ptr [rax + VCELL_KEY], 0        /* found => no strdup'd key    */
    mov     r8, [rsp + 16]
    mov     r9, [rsp + 24]
    mov     [rax + VCELL_KEY_D], r8     /* vc->key_d = idx (tag word)            */
    mov     [rax + VCELL_KEY_D + 8], r9 /*            ... (payload)              */
    mov     qword ptr [rax + VCELL_SV], DT_FAIL | (MOD_OP_RT_SUBSCRIPT_VAR << 8)   /* vc->sv = FAILDESCR, stamped (row descr-stamp-asm-mints) */
    mov     qword ptr [rax + VCELL_SV + 8], 0
    mov     qword ptr [rax + VCELL_POS], 0
    mov     qword ptr [rax + VCELL_LEN], 0
    add     rsp, 88
    mov     rdx, rax                    /* rdx = vc                              */
    mov     rax, DT_NAMETRAP_LO | (MOD_OP_RT_SUBSCRIPT_VAR << 8)   /* NAMETRAP, stamped (row descr-stamp-asm-mints) */
    ret

/*-----------------------------------------------------------------------------
 * RTX-30-ICN — THE TABLE MISS / KEY-INSERT TRAP.
 *
 * C of record, the `else` half of the DT_T arm (pattern_match.c):
 *     vc->cellp = 0;  vc->tbl = tb;  vc->key = rt_ws_strdup_c(ks);
 * then the shared tail: key_d = idx, sv = FAILDESCR, pos = 0, len = 0, NAMETRAP.
 *
 * ⛔⛔ IT MINTS A TRAP; IT DOES **NOT** FAIL. C's miss does not return FAILDESCR
 * and neither may this. A miss arm that failed would read CORRECT on every rvalue
 * workload and silently break `t[k] := v` on a FRESH key -- s223 finding 3, and
 * the reason RTX-26/29 deliberately left this edge to C rather than guess at it.
 * The trap is what `t[k] := v` lands in: rt_assign_var's `vc->cellp == 0 &&
 * vc->tbl` arm calls table_set_descr_keyown(vc->tbl, vc->key, vc->key_d, val),
 * which TAKES OWNERSHIP of the strdup'd key. Hence the copy is mandatory, not
 * defensive: the caller's `ks` may be our own stack scratch (the RTX-29 path).
 *
 * ⭐ CANONICAL ICON AGREES, AND THAT IS WHY THE INSERT IS NOT DONE HERE.
 * refs/icon-master/src/runtime/oref.r, operator [] subsc, the `table:` case:
 * *"Return a table element trapped variable representing the result; defer
 * actual lookup until later."* -- it allocates a b_tvtbl and returns; the chain
 * link happens in tvtbl_asgn (oasgn.r), which re-runs memb() at WRITE time. So
 * an eager bucket insert here would not merely be an optimisation, it would be
 * the WRONG SEMANTICS in both SCRIP's model and Icon's: `t[k]` alone must not
 * grow the table. This arm therefore allocates and names, and links NOTHING.
 *
 * ⭐ ONE ARM SERVES BOTH KEY SHAPES. It is reached from .Lsub_hash_init, so a
 * DT_S key (rdi = idx.s, the caller's bytes) and an RTX-29 DT_I key (rdi = our
 * [rsp+56..87] scratch) both arrive as a NUL-terminated string in rdi and both
 * strdup correctly. `t[i] := v` on a fresh integer key is covered by the same
 * six instructions as `t["k"] := v`.
 *
 * ⛔ GC SAFETY, AND IT IS NOT INCIDENTAL -- TWO CALLS, ONE LIVE UNTENDED CELL:
 *  (1) THE FIELD ORDER IS C's, EXACTLY: cellp and tbl are stored BEFORE the
 *      strdup call, key after it. rt_ws_strdup_c allocates and may collect, and
 *      gc_heap.c's HB_AGGV walk dereferences vc->key, vc->tbl and vc->cellp on
 *      every live VCELL it meets. The carve is fully zeroed (gc_heap.c: HB_AGGV
 *      takes the full memset arm, not the DT_S/HB_WSC 32-byte tail elide), so a
 *      collection landing mid-arm sees key=0 (skipped), cellp=0 (skipped) and a
 *      real tb -- coherent at every instruction boundary. Writing key first, or
 *      carving after the strdup, would both be defensible in C and are NOT
 *      equivalent here.
 *  (2) vc IS SPILLED TO [rsp+40] ACROSS THE STRDUP, not held in a register. The
 *      collector's root set is the conservative C-stack scan; -O0 C gets this
 *      spill from the compiler and this arm must do it by hand or the only
 *      reference to a live cell is a caller-saved register the callee may reuse.
 *
 * FRAME: the existing 88-byte frame, three slots, all already free on this path:
 *   [rsp+32] tb   [rsp+40] vc (across the strdup)   [rsp+48] the key bytes
 * ⭐ [rsp+48] was the 8-byte hole between RTX-24's slots (0..47) and RTX-29's
 * scratch (56..87) -- enumerated by grep before use, zero prior readers or
 * writers. No frame resize, so RTX-24/26/27/28/29 are byte-untouched by this.
 *
 * ⚠ THE CEILING IS THE ALLOCATION, AND IT IS NOW DOUBLE. This arm removes the
 * -O0 frame, tbl_key_str and table_find_pair's hash+walk, but it CALLS BOTH
 * rt_agg_alloc AND rt_ws_strdup_c -- two allocations per subscript where every
 * previously-landed arm had one. EXPECTATION ON RECORD, STATED BEFORE MEASURING:
 * 1.05-1.20x, and OVERLAPPING is the expected outcome, not a disappointment.
 * RTX-28 measured 1.160x with ONE allocation and the cheapest dispatch in the
 * function; this has more dispatch to remove and twice the allocation floor.
 * ⛔ Do NOT expect RTX-29's 1.296x. If it lands at or below 1.05x it is a NULL
 * and must be recorded as one. rt_ws_alloc is separately marked DO-NOT-TAKE:
 * this rung CALLS it, it does not port it.
 */
.Lsub_tbl_miss:
    mov     [rsp + 32], rsi             /* tb must survive both calls            */
    mov     [rsp + 48], rdi             /* the key bytes; rt_agg_alloc takes rdi */
    xor     edi, edi                    /* kind = 0                              */
    mov     esi, VCELL_SIZE             /* n    = sizeof(VCELL_t)                */
    call    rt_agg_alloc                /* already asm (ALLOC gate); do not fork  */
                                        /*   the bump allocator for a 2nd owner   */
    mov     [rsp + 40], rax             /* vc: GC-visible on the C stack across   */
                                        /*     the strdup below. See note (2).    */
    mov     qword ptr [rax + VCELL_CELLP], 0   /* MISS => no cell exists yet     */
    mov     rcx, [rsp + 32]
    mov     [rax + VCELL_TBL], rcx      /* vc->tbl = tb, BEFORE the strdup       */
    mov     rdi, [rsp + 48]
    call    rt_ws_strdup_c              /* rax = the owned copy of the key       */
    mov     rcx, [rsp + 40]             /* rcx = vc                              */
    mov     [rcx + VCELL_KEY], rax      /* vc->key = rt_ws_strdup_c(ks)          */
    mov     r8, [rsp + 16]
    mov     r9, [rsp + 24]
    mov     [rcx + VCELL_KEY_D], r8     /* vc->key_d = idx (tag word)            */
    mov     [rcx + VCELL_KEY_D + 8], r9 /*            ... (payload)              */
    mov     qword ptr [rcx + VCELL_SV], DT_FAIL | (MOD_OP_RT_SUBSCRIPT_VAR << 8)   /* vc->sv = FAILDESCR, stamped (row descr-stamp-asm-mints) */
    mov     qword ptr [rcx + VCELL_SV + 8], 0
    mov     qword ptr [rcx + VCELL_POS], 0
    mov     qword ptr [rcx + VCELL_LEN], 0
    add     rsp, 88
    mov     rdx, rcx                    /* rdx = vc                              */
    mov     rax, DT_NAMETRAP_LO | (MOD_OP_RT_SUBSCRIPT_VAR << 8)   /* NAMETRAP, stamped (row descr-stamp-asm-mints) */
    ret

/*-----------------------------------------------------------------------------
 * RTX-27-ICN — THE DT_S SUBSTRING-TRAP ARM, DT_I subscript, lvalue-shaped.
 *
 * C of record: the `(base.v == DT_S || base.v == DT_SNUL) && IS_VARREF_fn(bvar)`
 * arm. It mints a one-character substring trapped variable: cellp/tbl/key all 0,
 * sv = bvar, pos = i, len = 1.
 *
 * ⛔ THREE THINGS THIS ARM DELIBERATELY DOES NOT DO, EACH ONE A BAIL:
 *  (1) DT_SNUL. C would take slen from strlen("") and then fail every index, so
 *      the arm is pure ceremony; bail rather than reproduce a guaranteed FAIL.
 *  (2) slen == 0. C falls back to `strlen(sp)`. Porting a strlen call to keep a
 *      slow path slow buys nothing; the traffic has slen != 0 (RTX-16 populates
 *      it for string literals). Bail.
 *  (3) A NULL byte pointer with a nonzero slen. C substitutes ""; that shape is
 *      incoherent and unmeasured, so it is C's to keep.
 *
 * ⛔ `vc->sv = bvar`, THE **ORIGINAL** BASE, NOT THE DEREF'D ONE. The trap has to
 *    name the VARIABLE so a write-back can reach it; storing the deref'd string
 *    descriptor would produce a trap over a temporary and lose the assignment
 *    silently. bvar is [rsp+0]/[rsp+8] -- the reason the prologue spills all four
 *    arguments rather than just the two it needs to bail with.
 *
 * ⚠ THE WRAP RULE IS `i <= 0`, NOT `i < 0`. RTX-24's list arm wraps on `i < 0`.
 *   Two arms, two rules, one function, and the difference is load-bearing: at
 *   i == 0 the list arm leaves 0 and fails the `i < 1` test, while this arm maps
 *   0 to slen+1 and fails the `i > slen` test. Both fail, by different routes.
 *   DO NOT UNIFY THEM (s222's standing instruction, reconfirmed by reading the C).
 *
 * ⚠ 64-BIT INDEX, unlike RTX-24. C casts `(long)to_int(idx)` here and
 *   `(int)to_int(idx)` in the list arm, so this arm loads the full qword.
 */
.Lsub_string:
    mov     r8, rax
    shr     r8, 32                      /* r8 = base.slen (zero-extended)        */
    test    r8d, r8d
    je      .Lsub_bail                  /* slen == 0 => C's strlen path; bail (2) */
    test    rdx, rdx
    je      .Lsub_bail                  /* NULL bytes => C's ""; bail (3)         */
    mov     rax, [rsp + 24]             /* i = (long)to_int(idx), full qword      */
    test    rax, rax
    jg      .Lsub_str_nowrap
    lea     rax, [rax + r8 + 1]         /* i <= 0 => i = slen + 1 + i             */
.Lsub_str_nowrap:
    cmp     rax, 1
    jl      .Lsub_fail                  /* i < 1                                  */
    cmp     rax, r8
    jg      .Lsub_fail                  /* i > slen                               */
    mov     [rsp + 40], rax             /* pos must survive the carve             */
    xor     edi, edi
    mov     esi, VCELL_SIZE
    call    rt_agg_alloc
    mov     qword ptr [rax + VCELL_CELLP], 0
    mov     qword ptr [rax + VCELL_TBL], 0
    mov     qword ptr [rax + VCELL_KEY], 0
    mov     r8, [rsp + 16]
    mov     r9, [rsp + 24]
    mov     [rax + VCELL_KEY_D], r8     /* vc->key_d = idx                       */
    mov     [rax + VCELL_KEY_D + 8], r9
    mov     r8, [rsp + 0]               /* vc->sv = bvar, the ORIGINAL varref    */
    mov     r9, [rsp + 8]
    mov     [rax + VCELL_SV], r8
    mov     [rax + VCELL_SV + 8], r9
    mov     rcx, [rsp + 40]
    mov     [rax + VCELL_POS], rcx      /* vc->pos = i                           */
    mov     qword ptr [rax + VCELL_LEN], 1        /* one character                */
    add     rsp, 88
    mov     rdx, rax
    mov     rax, DT_NAMETRAP_LO | (MOD_OP_RT_SUBSCRIPT_VAR << 8)   /* NAMETRAP, stamped (row descr-stamp-asm-mints) */
    ret

/*-----------------------------------------------------------------------------
 * RTX-28-ICN — THE DT_A ARRAY ARM, keyed by a DT_I subscript.
 *
 * The C of record (pattern_match.c c_rt_subscript_var, the base.v == DT_A arm)
 * is four lines and its index rule is THE THIRD DISTINCT ONE IN THIS FUNCTION:
 *
 *     off = i - a->lo;  if (off < 0 || off >= a->hi - a->lo + 1) FAIL
 *
 * ⛔⛔ THERE IS NO NEGATIVE-INDEX WRAP HERE. The DT_DATA list arm wraps on
 * `i < 0`, the DT_S string arm wraps on `i <= 0`, and this arm wraps NOT AT ALL
 * -- a negative subscript on a 1-based array simply goes out of range and fails.
 * Three arms, three rules, one function. The goal file's standing instruction
 * about the first two applies with more force now: DO NOT "TIDY" THESE.
 *
 * This arm needs NO rt_deref call (the base arrives deref'd) and NO frame until
 * the carve, so every reject costs a handful of compares and a tail jump.
 *
 * ⚠ ndim != 1 BAILS. The C body does not consult ndim at all, so a 2-D array
 * subscripted with ONE index takes C's flat 1-D path. That is C's behaviour and
 * bailing preserves it exactly -- a bail re-enters the same C body -- while
 * keeping this arm's arithmetic honest about what it has actually proven.
 * FRAME: 56 bytes, same alignment reasoning as the entry block above; only the
 * three slots this arm reads back across the rt_agg_alloc call are written,
 * because this arm has NO post-frame bail.
 *   [rsp+16] idx tag word   [rsp+24] idx payload   [rsp+32] &a->data[off]
 */
.Lsub_array:
    test    rsi, rsi
    je      c_rt_subscript_var          /* a == NULL => C's FAILDESCR            */
    cmp     dword ptr [rsi + ARBLK_NDIM], 1
    jne     c_rt_subscript_var          /* 2-D: leave C's flat path alone        */
    mov     eax, ecx                    /* i = (int)to_int(idx); DT_I => the     */
                                        /*   payload IS the value, truncated to  */
                                        /*   32 bits exactly as the C cast does. */
    mov     r10d, dword ptr [rsi + ARBLK_LO]
    sub     eax, r10d                   /* off = i - a->lo                       */
    js      .Lsub_arr_fail              /* off < 0                               */
    mov     r8d, dword ptr [rsi + ARBLK_HI]
    sub     r8d, r10d
    inc     r8d                         /* a->hi - a->lo + 1                     */
    cmp     eax, r8d
    jge     .Lsub_arr_fail              /* off >= extent                         */
    mov     r9, [rsi + ARBLK_DATA]
    test    r9, r9
    je      c_rt_subscript_var          /* C forms NULL+off*16; do not emulate   */
    movsxd  rax, eax
    shl     rax, 4                      /* * sizeof(DESCR_t)                     */
    add     rax, r9                     /* &a->data[off]                         */

    sub     rsp, 88
    mov     [rsp + 16], rdx
    mov     [rsp + 24], rcx
    mov     [rsp + 32], rax

    /* --- carve the VCELL. rt_agg_alloc is already asm (ALLOC gate); call it,  */
    /*     do not fork the bump allocator for a second owner.                   */
    xor     edi, edi                    /* kind = 0                              */
    mov     esi, VCELL_SIZE             /* n    = sizeof(VCELL_t)                */
    call    rt_agg_alloc                /* rax = VCELL_t * (C does not null-check
                                         *   either, so neither do we)           */

    /* --- fill the seven fields, in the C body's own order ------------------ */
    mov     rcx, [rsp + 32]
    mov     [rax + VCELL_CELLP], rcx    /* vc->cellp = &a->data[off]             */
    mov     qword ptr [rax + VCELL_TBL], 0
    mov     qword ptr [rax + VCELL_KEY], 0
    mov     r8, [rsp + 16]
    mov     r9, [rsp + 24]
    mov     [rax + VCELL_KEY_D], r8     /* vc->key_d = idx (tag word)            */
    mov     [rax + VCELL_KEY_D + 8], r9 /*            ... (payload)              */
    mov     qword ptr [rax + VCELL_SV], DT_FAIL | (MOD_OP_RT_SUBSCRIPT_VAR << 8)   /* vc->sv = FAILDESCR, stamped (row descr-stamp-asm-mints) */
    mov     qword ptr [rax + VCELL_SV + 8], 0
    mov     qword ptr [rax + VCELL_POS], 0
    mov     qword ptr [rax + VCELL_LEN], 0

    add     rsp, 88
    mov     rdx, rax                    /* rdx = vc                              */
    mov     rax, DT_NAMETRAP_LO | (MOD_OP_RT_SUBSCRIPT_VAR << 8)   /* NAMETRAP, stamped (row descr-stamp-asm-mints) */
    ret

/* The array arm's out-of-range exit. Distinct from .Lsub_fail below because it
 * fires BEFORE the frame exists and must not adjust rsp. */
.Lsub_arr_fail:
    mov     eax, DT_FAIL | (MOD_OP_RT_SUBSCRIPT_VAR << 8)   /* stamped (row descr-stamp-asm-mints) */
    xor     edx, edx
    ret

/*-----------------------------------------------------------------------------
 * RTX-31-ICN -- THE DT_T TABLE ARM, BASE ALREADY DEREF'D (`T[I]`, SNOBOL4 shape).
 *
 * C of record, pattern_match.c c_rt_subscript_var's base.v==DT_T arm (the ONLY arm that
 * matters here -- IS_VARREF_fn(base) is false for a bare DT_T value, descr.h:79, so C's
 * own rt_deref-if-varref step is a no-op and execution goes straight to this mint):
 *     vc->cellp = 0; vc->tbl = tb; vc->key = 0; vc->key_d = idx; vc->sv = FAILDESCR;
 *     vc->pos = 0; vc->len = 0; return NAMETRAP(vc);
 * NO LOOKUP, NO STRINGIFY -- s262 (Lon) forbade cellp=&e->val outright ("we should never
 * have in our code a place that depends on that pointer not moving"), so the C mint for a
 * table subscript is already just seven field stores. This arm is that mint in asm.
 *
 * ⛔ WHY THIS ARM DID NOT EXIST: nothing above tested dil==DT_T, so every SNOBOL4 table
 * write fell through the DT_N ("must be a VARREF") gate and bailed to the full C call --
 * paying call/ret + IS_VARREF_fn + the base.v if-cascade for a mint C itself does in seven
 * stores. RTX-29 (.Lsub_table_int, below) was reachable only via that same DT_N gate, so it
 * NEVER ran for this traffic either -- confirmed by hit-count, not inferred: 0/0 on
 * table_access.sno, all 10,500 subscript-writes bailed to C instead. RTX-29 stays as-is;
 * this is a new, additional arm, not a replacement.
 *
 * ⚠ DT_S-KEYED TABLES STILL BAIL, DELIBERATELY. Reusing RTX-26's own already-standing
 * decision (see the s262 stand-down comment on .Lsub_table above): a hashed string-key
 * lookup is semantic work this arm does not attempt to shortcut. Only the DT_I-key mint
 * -- which needs no lookup at all, hit or miss -- is fast-pathed here.
 *
 * FRAME: 24 bytes (8 mod 16, same alignment rule as every other arm in this function):
 *   [rsp+0] tb   [rsp+8] idx tag word   [rsp+16] idx payload
 */
.Lsub_table_direct:
    test    rsi, rsi
    je      c_rt_subscript_var          /* tb == NULL => C's FAILDESCR (unreached in practice) */
    sub     rsp, 24
    mov     [rsp + 0], rsi              /* tb survives the rt_agg_alloc call     */
    mov     [rsp + 8], rdx              /* idx tag word                          */
    mov     [rsp + 16], rcx             /* idx payload                           */

    xor     edi, edi                    /* kind = 0                              */
    mov     esi, VCELL_SIZE             /* n    = sizeof(VCELL_t)                */
    call    rt_agg_alloc                /* already asm (ALLOC gate); do not fork  */
                                        /*   the bump allocator for a 2nd owner   */

    mov     qword ptr [rax + VCELL_CELLP], 0    /* vc->cellp = 0 -- no lookup, matches C exactly */
    mov     rcx, [rsp + 0]
    mov     [rax + VCELL_TBL], rcx      /* vc->tbl = tb                          */
    mov     qword ptr [rax + VCELL_KEY], 0      /* vc->key = 0                   */
    mov     r8, [rsp + 8]
    mov     r9, [rsp + 16]
    mov     [rax + VCELL_KEY_D], r8     /* vc->key_d = idx (tag word)            */
    mov     [rax + VCELL_KEY_D + 8], r9 /*            ... (payload)              */
    mov     qword ptr [rax + VCELL_SV], DT_FAIL | (MOD_OP_RT_SUBSCRIPT_VAR << 8)   /* vc->sv = FAILDESCR, stamped (row descr-stamp-asm-mints) */
    mov     qword ptr [rax + VCELL_SV + 8], 0
    mov     qword ptr [rax + VCELL_POS], 0
    mov     qword ptr [rax + VCELL_LEN], 0

    add     rsp, 24
    mov     rdx, rax                    /* rdx = vc                              */
    mov     rax, DT_NAMETRAP_LO | (MOD_OP_RT_SUBSCRIPT_VAR << 8)   /* NAMETRAP, stamped (row descr-stamp-asm-mints) */
    ret

/*-----------------------------------------------------------------------------
 * RTX-29-ICN — THE DT_T TABLE ARM KEYED BY A DT_I SUBSCRIPT (`t[3]`).
 *
 * MEASURED s224: `t[i]` took 400,008 entries / 400,008 bailed / 0 commits. It is
 * ICON-NATIVE traffic (unlike RTX-28's arrays, which are SNOBOL4's) and it was
 * bailing because the entry block routes to .Lsub_table only on a DT_S subscript,
 * so an integer key fell through every arm to .Lsub_bail.
 *
 * The C of record is aggregates.c tbl_key_str's `case DT_I`, which is a
 * HAND-ROLLED itoa, not an snprintf:
 *
 *     "\001i" [ '-' ] decimal-digits NUL
 *
 * ⛔ THE MAGNITUDE TRICK MUST BE PRESERVED EXACTLY. C computes the negative
 * magnitude as `(unsigned long long)(-(v + 1)) + 1ull` specifically so that
 * INT64_MIN does not overflow. A single `neg rax` is EXACTLY equivalent: neg of
 * INT64_MIN yields INT64_MIN, whose UNSIGNED value is 2^63 -- the correct
 * magnitude. Same bytes, one instruction, and the edge is safe rather than
 * accidentally safe.
 *
 * ⭐ THE KEY IS BUILT BACKWARDS from the high end of the scratch slab, so the
 * digits fall out of the div loop in the order they are stored and NO reversal
 * buffer and no second pass are needed -- the C does `do{}while` into t[] then
 * reverses; going backwards makes the reverse free. The hash does not care where
 * the string starts, only that rdi points at its first byte and it is NUL-
 * terminated, so this arm then RE-ENTERS .Lsub_hash_init and reuses RTX-26's
 * whole hash + chain + strcmp + hit machinery unchanged. Worst case is
 * '\001'+'i'+'-'+20 digits+NUL = 24 bytes, inside the 32-byte slab.
 *
 * ⚠ A MISS still falls to .Lsub_bail, which is C's key-INSERT trap -- see the
 * RTX-26 header. That is why `t[i] := v` on a FRESH integer key is correct here:
 * it bails, C mints the trap, and the assignment has somewhere to land. An arm
 * returning FAILDESCR would read correct on every rvalue workload and silently
 * break lvalue insert (s223 finding 3).
 */
.Lsub_table_int:
    mov     rsi, rdx                    /* rsi = tb (TBBLK_t *)                  */
    test    rsi, rsi
    je      .Lsub_bail
    lea     r8, [rsp + 87]              /* one PAST the last usable scratch byte */
    mov     byte ptr [r8], 0            /* the NUL, written first                */
    mov     rax, [rsp + 24]             /* the full 64-bit idx payload           */
    xor     r11d, r11d                  /* r11 = "was negative"                  */
    test    rax, rax
    jns     .Lsub_ti_digits
    mov     r11d, 1
    neg     rax                         /* magnitude; INT64_MIN-safe, see above  */
.Lsub_ti_digits:
    mov     r9d, 10
.Lsub_ti_loop:
    xor     edx, edx                    /* div reads rdx:rax -- must clear rdx    */
    div     r9                          /* rax = u/10, rdx = u%10 (unsigned)      */
    add     dl, '0'
    dec     r8
    mov     [r8], dl
    test    rax, rax
    jnz     .Lsub_ti_loop               /* C's do/while: at least one digit       */
    test    r11d, r11d
    je      .Lsub_ti_tag
    dec     r8
    mov     byte ptr [r8], '-'
.Lsub_ti_tag:
    dec     r8
    mov     byte ptr [r8], 'i'
    dec     r8
    mov     byte ptr [r8], 1            /* the \001 discriminator                */
    mov     rdi, r8                     /* rdi = first byte of the key           */
    jmp     .Lsub_hash_init             /* reuse RTX-26's walk verbatim          */

.Lsub_fail:
    add     rsp, 88
    mov     eax, DT_FAIL | (MOD_OP_RT_SUBSCRIPT_VAR << 8)   /* stamped (row descr-stamp-asm-mints) */
    xor     edx, edx
    ret

.Lsub_bail:
    mov     rdi, [rsp + 0]              /* restore the caller's exact arguments  */
    mov     rsi, [rsp + 8]
    mov     rdx, [rsp + 16]
    mov     rcx, [rsp + 24]
    add     rsp, 88
    jmp     c_rt_subscript_var
RTX_ENDF(rt_subscript_var)

/* Non-executable stack marker: without it ld marks the whole .so RWX-stack. */
.section .note.GNU-stack,"",@progbits
