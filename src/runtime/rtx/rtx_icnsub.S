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
 *   ARCH §2's Icon-specific live-subject warning does not bind here. rbx/rbp/rsp untouched.
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
/* ⛔ rtx_abi.inc's tag list STOPS AT DT_FAIL 99 and has no DT_DATA. Defined here
 * rather than added there, because that header is shared state with SN4-RTX and a
 * tag-list edit is a needless rebase point. ⚠ AND THE OMISSION IS A LIVE TRAP, not
 * a nuisance: with DT_DATA undefined, `cmp edi, DT_DATA` ASSEMBLES CLEANLY as a
 * relocation against an undefined external symbol -- only the memory-operand form
 * `cmp dword ptr [r9 + k], DT_DATA` is ambiguous enough to be rejected. So the
 * register compare that guards this entire arm would have linked against whatever
 * the loader resolved and admitted the wrong datatype, silently. The assembler
 * caught this by accident, one line out of two. Pinned by _Static_assert in
 * rtx_init.c. */
#define DT_DATA         100
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
 * The four originals are saved so EVERY bail can restore them and tail-jump to C
 * with its arguments byte-identical to what our caller staged -- the byte-identity
 * proof path survives the fact that this arm must call rt_deref before it can even
 * tell whether it applies.
 */
RTX_FUNC(rt_subscript_var)
    RTX_GATE(icnsub, c_rt_subscript_var)

    /* Cheap rejects first, BEFORE any frame, so the common bail costs four
     * compares and a tail jump with the arguments never touched. */
    cmp     edx, DT_I                   /* DT_I  -> RTX-24's DT_DATA list arm   */
    je      .Lsub_tag_ok
    cmp     edx, DT_S                   /* DT_S  -> RTX-26's DT_T table arm     */
    jne     c_rt_subscript_var
.Lsub_tag_ok:
    /* RTX-28: the DT_A array arm branches off HERE, ahead of the DT_N gate,
     * because a SNOBOL4 array subscript arrives with the base ALREADY DEREF'D --
     * MEASURED s224: base.v=4 (DT_A) base.slen=0, never the DT_N varref shape.
     * The gate below would reject it, which is exactly why this arm measured
     * 200,001 entries / 200,001 bailed / 0 commits before it existed. It is
     * RTX-24's lesson inverted: that arm had to stop rejecting varrefs, this one
     * had to stop requiring them. */
    cmp     edi, DT_A
    jne     .Lsub_not_array
    cmp     edx, DT_I                   /* array arm takes DT_I subscripts only  */
    jne     c_rt_subscript_var
    jmp     .Lsub_array
.Lsub_not_array:
    cmp     edi, DT_N                   /* base must be the VARREF shape        */
    jne     c_rt_subscript_var
    mov     rax, rdi
    shr     rax, 32                     /* rax = base.slen                      */
    cmp     eax, 1                      /* ...and specifically the slen==1       */
    jne     c_rt_subscript_var          /*    cell-pointer form (MEASURED live)  */
    test    rsi, rsi                    /* IS_VARREF's `slen==1 && ptr` clause   */
    je      c_rt_subscript_var

    sub     rsp, 56
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
    cmp     eax, DT_S
    je      .Lsub_string
    cmp     eax, DT_DATA                /* only the list/record family here      */
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
    mov     qword ptr [rax + VCELL_SV], DT_FAIL   /* vc->sv = FAILDESCR          */
    mov     qword ptr [rax + VCELL_SV + 8], 0
    mov     qword ptr [rax + VCELL_POS], 0
    mov     qword ptr [rax + VCELL_LEN], 0

    add     rsp, 56
    mov     rdx, rax                    /* rdx = vc                              */
    mov     rax, 0x200000009            /* NAMETRAP: (slen 2 << 32) | DT_N 9     */
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
 * ⛔ THE MISS IS NOT PORTED AND MUST BAIL, NOT FAIL. On a miss the C body does
 *    NOT fail: it mints a key-INSERT trap (cellp = 0, key = rt_ws_strdup_c(ks)),
 *    which allocates and copies. Returning FAILDESCR here would silently break
 *    `t[k] := v` on a fresh key. The chain-exhausted edge therefore tail-jumps
 *    to C with the arguments byte-identical, exactly like every other bail.
 * ⛔ THE GUARD IS NARROWER THAN THE C, DELIBERATELY. C reaches its DT_T arm for
 *    any base that derefs to DT_T; this arm additionally requires the
 *    VARREF(slen==1) prologue it shares with RTX-24. That is safe by
 *    construction (anything else bails) and it is what the traffic actually is:
 *    2,000,001 of 2,000,001 arrivals measured base.v=9 slen=1.
 */
.Lsub_table:
    cmp     eax, DT_T
    jne     .Lsub_bail
    mov     rsi, rdx                    /* rsi = tb (TBBLK_t *)                  */
    test    rsi, rsi
    je      .Lsub_bail
    mov     rdi, [rsp + 24]             /* rdi = idx.s, the key bytes            */
    test    rdi, rdi
    je      .Lsub_bail                  /* C substitutes ""; do not emulate it    */

    /* --- _tbl_hash, inlined: h = 5381; while (*k) h = h*33 ^ (uchar)*k++ ------ */
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
    je      .Lsub_bail                  /* MISS -> C's key-insert trap. See above */
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
    mov     qword ptr [rax + VCELL_SV], DT_FAIL   /* vc->sv = FAILDESCR          */
    mov     qword ptr [rax + VCELL_SV + 8], 0
    mov     qword ptr [rax + VCELL_POS], 0
    mov     qword ptr [rax + VCELL_LEN], 0
    add     rsp, 56
    mov     rdx, rax                    /* rdx = vc                              */
    mov     rax, 0x200000009            /* NAMETRAP: (slen 2 << 32) | DT_N 9     */
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
    add     rsp, 56
    mov     rdx, rax
    mov     rax, 0x200000009            /* NAMETRAP                              */
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

    sub     rsp, 56
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
    mov     qword ptr [rax + VCELL_SV], DT_FAIL   /* vc->sv = FAILDESCR          */
    mov     qword ptr [rax + VCELL_SV + 8], 0
    mov     qword ptr [rax + VCELL_POS], 0
    mov     qword ptr [rax + VCELL_LEN], 0

    add     rsp, 56
    mov     rdx, rax                    /* rdx = vc                              */
    mov     rax, 0x200000009            /* NAMETRAP: (slen 2 << 32) | DT_N 9     */
    ret

/* The array arm's out-of-range exit. Distinct from .Lsub_fail below because it
 * fires BEFORE the frame exists and must not adjust rsp. */
.Lsub_arr_fail:
    mov     eax, DT_FAIL
    xor     edx, edx
    ret

.Lsub_fail:
    add     rsp, 56
    mov     eax, DT_FAIL
    xor     edx, edx
    ret

.Lsub_bail:
    mov     rdi, [rsp + 0]              /* restore the caller's exact arguments  */
    mov     rsi, [rsp + 8]
    mov     rdx, [rsp + 16]
    mov     rcx, [rsp + 24]
    add     rsp, 56
    jmp     c_rt_subscript_var
RTX_ENDF(rt_subscript_var)

/* Non-executable stack marker: without it ld marks the whole .so RWX-stack. */
.section .note.GNU-stack,"",@progbits
