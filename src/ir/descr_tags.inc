/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*
 * descr_tags.inc -- THE DTYPE_t TAG NUMBERS, AND THE ONLY PLACE ANY ASSEMBLY MAY LEARN THEM. Minted s230.
 *
 * ⛔ WHY THIS FILE EXISTS, SO THE DEFECT IT CLOSES IS NOT RE-EARNED A FOURTH TIME. GOAL-DESCR-TAG-ENCODING.md
 * section 5 asserted "The asm is SYMBOLIC ... No hand-edited immediates." That claim has now been falsified
 * THREE times, each by a different file, each discovered only after it had already cost something:
 *   rtx_arith.s  -- an unsigned `cmp eax,DT_S; ja` range trick, valid only while SNUL==0 && S==1  (found s229)
 *   rtx_match.s  -- literal 99 and 1 compared against tags in rt_dcap_step's arm selection        (found s229)
 *   rt_asm_helpers.S -- literal 9 (DT_N), 99 (DT_FAIL), 6 (DT_I) in rt_deref and to_int           (found s230)
 * The first two were fixed by editing them in place, which fixes the instances and leaves the CLASS open --
 * and the class promptly produced a third instance that cost a full build + crosscheck cycle (58 regressions,
 * m3 276->218) before it was bracketed. The reason the third file kept escaping is structural, not careless:
 * it is AT&T syntax, so it CANNOT include rtx_abi.inc, because rtx_abi.inc opens with `.intel_syntax noprefix`.
 * The constants were therefore unreachable from it by construction, and hand-encoding was the only option.
 *
 * ⭐ THIS FILE CARRIES NO SYNTAX DIRECTIVE AND NO C CONSTRUCT -- only #define. That is the whole point: it is
 * includable from Intel asm, from AT&T asm, and from C alike, so no future file has an EXCUSE to hand-encode.
 * A `.S` that needs a tag includes THIS; it must never re-spell a number.
 *
 * ⚠ TWO SOURCES REMAIN BY DESIGN, AND THEY ARE GATED, NOT TRUSTED: src/contracts/descr.h holds the DTYPE_t
 * ENUM (C needs a real enum type; the preprocessor cannot make one), and this file holds the numbers for the
 * assembler. scripts/util_tag_layout_verify.py compares them value-by-value and fails on any drift, and also
 * greps every .S for bare tag-shaped immediates. Do not "simplify" by letting either side drift.
 *----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef DESCR_TAGS_INC
#define DESCR_TAGS_INC
#define DT_NUMERIC_BIT 0x01
#define DT_CHARS_BIT   0x02
#define DT_REAL_BIT    0x04
#define DT_NOTSTR_MASK 0xFFFFFFFD
#define DT_SNUL  0x00
#define DT_S     0x02
#define DT_I     0x03
#define DT_R     0x05
#define DT_P     0x08
#define DT_A     0x10
#define DT_T     0x18
#define DT_C     0x20
#define DT_N     0x28
#define DT_K     0x30
#define DT_E     0x38
#define DT_FH    0x40
#define DT_PLVAR 0x48
#define DT_PLREF 0x50
#define DT_X     0x58
#define DT_BLK   0x60
#define DT_FAIL  0x68
#define DT_DATA  0x70
#define DT_BIG   0x78
#define DT_CO    0x80
#define DT_BOOL  0x88
#define DT_ORDER 0x90
#define DT_RAW   0x98
#define DT_MAP   0xA0
#define DT_CPLX  0xA8
/* The NAMETRAP descriptor low word, PACKED, because hand-fusing it is how the TAG-3 renumber survived three
 * greps and still shipped a broken build: rtx_icnsub.s carried `mov rax, 0x200000009` in FOUR places -- the
 * tag was not a recognisable immediate at all, it was a nibble inside a 64-bit literal, so no reviewer and no
 * pattern for "a bare tag constant" could see it. (slen 2 << 32) | DT_N. Use THIS, never the packed number. */
#define DT_NAMETRAP_LO (((2) << 32) | DT_N)
/* ⛔⭐ mint_op IS GONE (Lon 2026-09-18, in-chat to the cfo, on reading this file's own history: "Remove mint_op
 * since it is derivative"). IT HELD THE MINTING BB/IR OP IN ONE BYTE, and the op is RECOVERABLE FROM THE NODE ID --
 * `scrip --dump-bb` prints an id, a kind and a stmt for every box -- so the tree was storing one fact twice and
 * spending a byte of a 16-byte ABI-pinned struct to do it. THE BYTE WENT TO src_node, WHICH IS NOW 24 BITS.
 * ⭐ THAT ALSO ANSWERS THE OBJECTION RECORDED HERE UNDER CEO-719, which is why this is not a re-walk of a dead
 * proposal: the dead proposal was folding the OP STAMP INTO src_node, and it died on "src_node cannot hold the stamp
 * because it is the BB node id and 16 bits is already too small". This is the opposite move -- the op stamp is
 * DELETED rather than folded, and the freed byte CURES the 16-bit shortage that objection named. MEASURED HEADROOM:
 * beauty.sno is the densest program in the corpus at 12812 boxes over 618 lines, 20.7 boxes per line, which was 19.6
 * percent of the old 16-bit field and is 0.08 percent of the new one; the whole graded SNOBOL4 suite at that density
 * would be about 402000 boxes, 2.4 percent of a 24-bit space.
 * ⛔ WHAT IS STILL HOMELESS AND MUST NOT BE FOLDED IN HERE: the 123 MOD_OP_RT_* ids, one per hand-written rtx/*.s
 * routine that mints a descriptor. They were a SECOND, ORTHOGONAL DIMENSION -- "Like BB_CALL, which call?" -- because
 * one bb_call dispatches to rt_add, rt_concat, rt_size and dozens more with the BB identical every time, and 118 of
 * the 123 have no IR op counterpart by name. Lon accepted that loss in those terms ("We can do without that
 * granularity for now until we figure something else"). IT NEEDS ITS OWN STORAGE AND IT IS NOT DERIVABLE FROM THE
 * NODE ID, which is exactly why it did not go the way mint_op just did. */
#endif
