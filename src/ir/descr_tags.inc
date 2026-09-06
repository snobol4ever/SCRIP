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
/* The NAMETRAP descriptor low word, PACKED, because hand-fusing it is how the TAG-3 renumber survived three
 * greps and still shipped a broken build: rtx_icnsub.s carried `mov rax, 0x200000009` in FOUR places -- the
 * tag was not a recognisable immediate at all, it was a nibble inside a 64-bit literal, so no reviewer and no
 * pattern for "a bare tag constant" could see it. (slen 2 << 32) | DT_N. Use THIS, never the packed number. */
#define DT_NAMETRAP_LO (((2) << 32) | DT_N)
/* MOD_OP SENTINELS FOR src/runtime/rtx/*.s (row descr-stamp-asm-mints). mod_op names WHICH OPERATION
 * minted a descriptor; the C-side compiler stamps mod_op = (IR_* opcode) + 1 at COMPILE TIME
 * (bb_lit_scalar.cpp lit_tag_imm), because a codegen template knows which program statement it is
 * emitting. The 14 rtx/*.s files are NOT per-program codegen -- they are ONE hand-written runtime
 * shared by every compiled program, so there is no per-call IR node to read. What IS knowable at
 * ASSEMBLY TIME is WHICH RUNTIME FUNCTION mints a given descriptor, and that identity never changes
 * call to call, so it costs ZERO extra instructions to fold into the mint's existing immediate (every
 * site below already builds its return with one mov/or against a small constant; ORing in a fixed
 * byte does not widen the encoding). Values start at 130, one past IR_OP_COUNT (129, IR.h) real ops
 * (mod_op range 1-129), so a stamp reader can tell "compiler-stamped IR op" from "hand-written
 * runtime mint" on sight. src_node is NOT stamped by any of these (stays 0, DESCR_SRC_NODE_UNSTAMPED):
 * there is no per-call source-program node to name here, and mod_op alone being nonzero already
 * distinguishes a runtime-stamped descriptor from the true all-zero DESCR_MOD_OP_UNSTAMPED/
 * DESCR_SRC_NODE_UNSTAMPED pair that means "nothing touched this." NOT wired into kind_names[]
 * (out of this row's scope -- a debug printer indexing kind_names[mod_op-1] needs its own bounds
 * check for ANY externally-derived index regardless; left as a named follow-up in the FINDING). */
#define MOD_OP_RT_FAILDESCR            130   /* rtx_misc.s    rt_faildescr           */
#define MOD_OP_RT_ADD                  131   /* rtx_arith.s   rt_add                 */
#define MOD_OP_RT_SUB                  132   /* rtx_arith.s   rt_sub                 */
#define MOD_OP_RT_MUL                  133   /* rtx_arith.s   rt_mul                 */
#define MOD_OP_RT_SIZE_D               134   /* rtx_icnagg.s  rt_size_d              */
#define MOD_OP_RT_LIST_BANG_AT         135   /* rtx_icnagg.s  rt_list_bang_at        */
#define MOD_OP_RT_PROC_VALUE           136   /* rtx_icncall.s rt_proc_value          */
#define MOD_OP_RT_GEN_SPINE_PASS_OMEGA 137   /* rtx_icngen.s  rt_gen_spine_pass_ω    */
#define MOD_OP_RT_PL_DOP_UNIFY         138   /* rtx_plunify.s rt_pl_dop_unify        */
#define MOD_OP_RT_SUBSCRIPT_VAR        139   /* rtx_icnsub.s  rt_subscript_var       */
#define MOD_OP_RT_STR_CONCAT_D         140   /* rtx_str.s     str_concat_d           */
#define MOD_OP_RT_COERCE_NUM2_D        141   /* rtx_icnnum.s  rt_coerce_num2_d       */
#define MOD_OP_RT_MATCH_REPLACE        142   /* rtx_match.s   rt_match_replace       */
#define MOD_OP_RT_PL_THROW_RAISE       143   /* rtx_plunify.s rt_pl_throw_raise      */
#define MOD_OP_RT_PL_EXIST_RAISE       144   /* rtx_plunify.s rt_pl_exist_raise      */
#define MOD_OP_RT_PL_CATCH_HANDLE      145   /* rtx_plunify.s rt_pl_catch_handle     */
#define MOD_OP_RT_PL_DB_ALIVE          146   /* rtx_plunify.s rt_pl_dop_db_alive     */
#define MOD_OP_RT_PL_AX_ZGUARD         147   /* rtx_plunify.s rt_pl_dop_ax_zguard    */
#define MOD_OP_RT_PL_CHAR_GUARD        148   /* rtx_plunify.s rt_pl_dop_char_guard   */
#define MOD_OP_RT_PL_BETWEEN_GUARD     149   /* rtx_plunify.s rt_pl_dop_between_guard */
#define MOD_OP_RT_PL_STREAM_GUARD      150   /* rtx_plunify.s rt_pl_dop_stream_guard */
#define MOD_OP_RT_PL_CURSTREAM_GUARD   151   /* rtx_plunify.s rt_pl_dop_curstream_guard */
#define MOD_OP_RT_PL_ANUM_GUARD2       152   /* rtx_plunify.s rt_pl_dop_anum_guard2  */
#define MOD_OP_RT_PL_ANUM_GUARD3       153   /* rtx_plunify.s rt_pl_dop_anum_guard3  */
#define MOD_OP_RT_PL_ANUM_GUARD5       154   /* rtx_plunify.s rt_pl_dop_anum_guard5  */
#define MOD_OP_RT_PL_NB_GETVAL_GUARD   155   /* rtx_plunify.s rt_pl_dop_nb_getval_guard */
#define MOD_OP_RT_PL_AX_EGUARD         156   /* rtx_plunify.s rt_pl_dop_ax_eguard    */
#define MOD_OP_RT_PL_IS_V              157    /* rtx_plunify.s rt_pl_dop_is_v         */
#define MOD_OP_RT_PL_CMP_LT           158    /* rtx_plunify.s rt_pl_dop_cmp_lt      */
#define MOD_OP_RT_PL_CMP_GT           159    /* rtx_plunify.s rt_pl_dop_cmp_gt      */
#define MOD_OP_RT_PL_CMP_LE           160    /* rtx_plunify.s rt_pl_dop_cmp_le      */
#define MOD_OP_RT_PL_CMP_GE           161    /* rtx_plunify.s rt_pl_dop_cmp_ge      */
#define MOD_OP_RT_PL_CMP_EQ           162    /* rtx_plunify.s rt_pl_dop_cmp_eq      */
#define MOD_OP_RT_PL_CMP_NE           163    /* rtx_plunify.s rt_pl_dop_cmp_ne      */
#define MOD_OP_RT_PL_AX_ADD           164    /* rtx_plunify.s rt_pl_dop_ax_add     */
#define MOD_OP_RT_PL_AX_SUB           165    /* rtx_plunify.s rt_pl_dop_ax_sub     */
#define MOD_OP_RT_PL_AX_MUL           166    /* rtx_plunify.s rt_pl_dop_ax_mul     */
#define MOD_OP_RT_PL_AX_DIV           167    /* rtx_plunify.s rt_pl_dop_ax_div     */
#define MOD_OP_RT_PL_AX_IDIV          168    /* rtx_plunify.s rt_pl_dop_ax_idiv    */
#define MOD_OP_RT_PL_AX_MOD           169    /* rtx_plunify.s rt_pl_dop_ax_mod     */
#define MOD_OP_RT_PL_AX_REM           170    /* rtx_plunify.s rt_pl_dop_ax_rem     */
#define MOD_OP_RT_PL_AX_FPOW          171    /* rtx_plunify.s rt_pl_dop_ax_fpow    */
#define MOD_OP_RT_PL_AX_POW           172    /* rtx_plunify.s rt_pl_dop_ax_pow     */
#define MOD_OP_RT_PL_AX_MIN           173    /* rtx_plunify.s rt_pl_dop_ax_min     */
#define MOD_OP_RT_PL_AX_MAX           174    /* rtx_plunify.s rt_pl_dop_ax_max     */
#define MOD_OP_RT_PL_AX_GCD           175    /* rtx_plunify.s rt_pl_dop_ax_gcd     */
#define MOD_OP_RT_PL_AX_XOR           176    /* rtx_plunify.s rt_pl_dop_ax_xor     */
#define MOD_OP_RT_PL_AX_SHR           177    /* rtx_plunify.s rt_pl_dop_ax_shr     */
#define MOD_OP_RT_PL_AX_SHL           178    /* rtx_plunify.s rt_pl_dop_ax_shl     */
#define MOD_OP_RT_PL_AX_BAND          179    /* rtx_plunify.s rt_pl_dop_ax_band    */
#define MOD_OP_RT_PL_AX_BOR           180    /* rtx_plunify.s rt_pl_dop_ax_bor     */
#define MOD_OP_RT_PL_AX_NEG           181    /* rtx_plunify.s rt_pl_dop_ax_neg     */
#define MOD_OP_RT_PL_AX_POS           182    /* rtx_plunify.s rt_pl_dop_ax_pos     */
#define MOD_OP_RT_PL_AX_ABS           183    /* rtx_plunify.s rt_pl_dop_ax_abs     */
#define MOD_OP_RT_PL_AX_SIGN          184    /* rtx_plunify.s rt_pl_dop_ax_sign    */
#define MOD_OP_RT_PL_AX_TRUNC         185    /* rtx_plunify.s rt_pl_dop_ax_trunc   */
#define MOD_OP_RT_PL_AX_INTG          186    /* rtx_plunify.s rt_pl_dop_ax_intg    */
#define MOD_OP_RT_PL_AX_FLT           187    /* rtx_plunify.s rt_pl_dop_ax_flt     */
#define MOD_OP_RT_PL_AX_FLOOR         188    /* rtx_plunify.s rt_pl_dop_ax_floor   */
#define MOD_OP_RT_PL_AX_CEIL          189    /* rtx_plunify.s rt_pl_dop_ax_ceil    */
#define MOD_OP_RT_PL_AX_ROUND         190    /* rtx_plunify.s rt_pl_dop_ax_round   */
#define MOD_OP_RT_PL_AX_SQRT          191    /* rtx_plunify.s rt_pl_dop_ax_sqrt    */
#define MOD_OP_RT_PL_AX_MSB           192    /* rtx_plunify.s rt_pl_dop_ax_msb     */
#define MOD_OP_RT_PL_AX_BNOT          193    /* rtx_plunify.s rt_pl_dop_ax_bnot    */
#define MOD_OP_RT_PL_AX_SIN           194    /* rtx_plunify.s rt_pl_dop_ax_sin     */
#define MOD_OP_RT_PL_AX_COS           195    /* rtx_plunify.s rt_pl_dop_ax_cos     */
#define MOD_OP_RT_PL_AX_ATAN          196    /* rtx_plunify.s rt_pl_dop_ax_atan    */
#define MOD_OP_RT_PL_AX_LOG           197    /* rtx_plunify.s rt_pl_dop_ax_log     */
#define MOD_OP_RT_PL_AX_EXP           198    /* rtx_plunify.s rt_pl_dop_ax_exp     */
#define MOD_OP_RT_PL_AX_FIP           199    /* rtx_plunify.s rt_pl_dop_ax_fip     */
#define MOD_OP_RT_PL_AX_FFP           200    /* rtx_plunify.s rt_pl_dop_ax_ffp     */
#define MOD_OP_RT_PL_AX_PI            201    /* rtx_plunify.s rt_pl_dop_ax_pi      */
#define MOD_OP_RT_PL_AX_E             202    /* rtx_plunify.s rt_pl_dop_ax_e       */
#define MOD_OP_RT_PL_EXIST_KEY_RAISE 203
#endif
