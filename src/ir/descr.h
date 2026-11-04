#ifndef DESCR_H
#define DESCR_H
#include <stdint.h>
#include <stddef.h>
#include "ir/descr_tags.h"
#ifdef __cplusplus
extern "C" {
#endif
struct _ARBLK_t;
struct _TBBLK_t;
struct _DATINST_t;
typedef struct DESCR_t {
    uint8_t v;
    uint8_t src_node0, src_node1, src_node2;
    uint32_t slen;
    union { char *s; int64_t i; double r; void *p; struct _ARBLK_t *arr; struct _TBBLK_t *tbl; struct _DATINST_t *u; void *ptr; };
} DESCR_t;
DESCR_SASSERT(sizeof(DESCR_t) == 16, "DESCR_t is a SysV register-pair (rax:rdx) INTEGER-class return; 17+ bytes flips it to MEMORY class across 4,009 lines of asm");
#define DESCR_SRC_NODE_UNSTAMPED 0u
#define DESCR_SRC_NODE_OVERFLOW 0xFFFFFFu
DESCR_SASSERT(offsetof(DESCR_t, v) == 0 && offsetof(DESCR_t, src_node0) == 1 && offsetof(DESCR_t, src_node2) == 3 && offsetof(DESCR_t, slen) == 4,
    "the tag word is {v:1, src_node:3, slen:4} and emitted code mints it as ONE 32-bit immediate: " "emit.cpp's lit_prepare ORs the node id in at <<8, which on little-endian x86-64 lands in "
    "src_node[0..2] exactly. Move either field and every stamped literal mints a wrong node id silently.");
DESCR_SASSERT(DESCR_SRC_NODE_OVERFLOW == 0xFFFFFFu && DESCR_SRC_NODE_UNSTAMPED == 0u,
    "src_node is 24 bits (Lon 2026-09-18, in-chat: mint_op is REMOVED because the IR op is DERIVABLE from "
    "the node id -- scrip --dump-bb maps id to kind and statement -- so the byte it held belongs to the id "
    "instead. 0 stays UNSTAMPED and an id past the field SATURATES to OVERFLOW rather than wrapping, "
    "because a wrapped id aliases two nodes and yields a CONFIDENTLY WRONG attribution, which is strictly "
    "worse for a debugging tool than no attribution. MEASURED HEADROOM: beauty.sno is the densest program "
    "in the corpus at 12812 boxes over 618 lines, 19.6 percent of the OLD 16-bit field and 0.08 percent of " "this one.");
static inline __attribute__((always_inline)) uint32_t descr_src_node(DESCR_t d) { return (uint32_t)d.src_node0 | ((uint32_t)d.src_node1 << 8) | ((uint32_t)d.src_node2 << 16); }
static inline __attribute__((always_inline)) void descr_set_src_node(DESCR_t *d, uint32_t id) {
    if (id > DESCR_SRC_NODE_OVERFLOW) id = DESCR_SRC_NODE_OVERFLOW;
    d->src_node0 = (uint8_t)id;
    d->src_node1 = (uint8_t)(id >> 8);
    d->src_node2 = (uint8_t)(id >> 16);
}
#define FAILDESCR ((DESCR_t){ .v = DT_FAIL, .i = 0 })
#define NAMETRAP(vc_) ((DESCR_t){ .v = DT_N, .slen = 2, .p = (void *)(vc_) })
#define ICN_FIELD_PLAIN 0x10000
DESCR_SASSERT(ICN_FIELD_PLAIN > 0xFFFF && ICN_FIELD_PLAIN < 0x7FFFFFFF,
    "an Icon record-field variable node carries the field's 1-based position in IR_t.seal; the lowerer ORs ICN_FIELD_PLAIN into it where the variable's only consumer is a store "
    "(an assignment or an augmented assignment), so rt_field_var_strict_at answers the bare DT_N slen 1 cell and builds no VCELL and no Rec.field name string -- name() is never asked "
    "of such a node; the bit sits above any record's field count and inside the 32-bit mov that loads the seal.");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) int IS_FAIL_fn(DESCR_t v) { return v.v == DT_FAIL; }
static inline __attribute__((always_inline)) int IS_DATA_TAG_fn(uint8_t v) { return v >= DT_DATA && v < DT_RAW; }
static inline __attribute__((always_inline)) int IS_NAMETRAP_fn(DESCR_t v) { return v.v == DT_N && v.slen == 2; }
static inline __attribute__((always_inline)) int IS_VARREF_fn(DESCR_t v) { return v.v == DT_N && (v.slen == 2 || (v.slen == 1 && v.ptr) || (v.slen == 0 && v.s && *v.s)); }
static inline __attribute__((always_inline)) int IS_KWNAME_fn(DESCR_t v) { return v.v == DT_N && v.slen == 0 && v.s && v.s[0] == '&' && v.s[1] == '&'; }
#define DATA_INST_SLEN 0u
#define DATA_ELEMS_SLEN 1u
DESCR_SASSERT(DATA_INST_SLEN != DATA_ELEMS_SLEN,
    "DT_DATA was a UNION with nothing in the descriptor to arbitrate it: the .u arm is a DATINST_t (a record instance) and the "
    ".ptr arm is a raw DESCR_t element vector (an Icon list's frame_elems backing store), and until 2026-09-18 BOTH were minted "
    "with slen 0, so a reader that guessed wrong read a DATBLK_t* out of the first element of a descriptor array -- a SILENT "
    "WRONG ANSWER, not a crash, and the collector had to string-compare a field name to tell them apart. This is DT_N's own "
    "scheme (slen 0 char*, 1 DESCR_t*, 2 VCELL_t*) applied to the tag that needed it; the two values are PINNED here so a future " "third meaning cannot quietly reuse one.");
DESCR_SASSERT(DATA_ELEMS_SLEN != 0xFFFFFFFFu && DATA_ELEMS_SLEN != 0xFFFFFFFDu && DATA_ELEMS_SLEN != 0xFFFFFFFEu && DATA_ELEMS_SLEN != 0xFFFFFFFCu && DATA_ELEMS_SLEN != 0xFFFFFFFBu,
    "a DT_DATA discriminator is PER-TAG, so reusing a small value another tag already uses is fine (DT_N slen 1 is NAMEPTR and is "
    "unrelated to this); what it must not do is land in the 0xFFFFFFFx range that the PROCVAL_BUILTIN_SLEN assert above records as "
    "ONE space shared across unrelated headers with nothing collecting it.");
static inline __attribute__((always_inline)) int IS_DATA_INST_fn(DESCR_t v) { return v.v == DT_DATA && v.slen == DATA_INST_SLEN; }
static inline __attribute__((always_inline)) int IS_DATA_ELEMS_fn(DESCR_t v) { return v.v == DT_DATA && v.slen == DATA_ELEMS_SLEN; }
#define DATA_ELEMS(p_) ((DESCR_t){ .v = DT_DATA, .slen = DATA_ELEMS_SLEN, .ptr = (void *)(p_) })
#define FHVAL(idx_) ((DESCR_t){ .v = DT_FH, .i = (int64_t)(idx_) })
#define COERCE_ERR_FAILURE_CONVERTIBLE 0x1000000L
#define COERCE_KEEP_INT 0x2000000L
#define COERCE_OP_SHIFT 32
#define COERCE_OP_TAG(bcode) (((long)((bcode) + 1)) << COERCE_OP_SHIFT)
#define COERCE_OP_OF(codes) ((int)(((codes) >> COERCE_OP_SHIFT) & 0xffff) - 1)
#define COERCE_OP_SELF_RIGHT (1L << 48)
#define COERCE_OP_UNARY_NEG (1L << 49)
#define COERCE_OP_UNARY_POS (1L << 50)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) int IS_FH_fn(DESCR_t v) { return v.v == DT_FH; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PROCVAL_SLEN 0xFFFFFFFEu
#define PROCVAL_BUILTIN_SLEN 0xFFFFFFFCu
DESCR_SASSERT(PROCVAL_SLEN != PROCVAL_BUILTIN_SLEN, "the two procedure-value markers must differ, or proc(s,0) is indistinguishable from a global of the same name");
DESCR_SASSERT(PROCVAL_BUILTIN_SLEN != 0xFFFFFFFFu && PROCVAL_BUILTIN_SLEN != 0xFFFFFFFDu,
    "slen sentinels share ONE 32-bit space across unrelated headers and nothing collects them: 0xFFFFFFFF is CSETVAL, "
    "0xFFFFFFFD is core.h RT_CONVE_CHAIN_MARK. PROCVAL_BUILTIN_SLEN was first written 0xFFFFFFFD, every board stayed "
    "green, and type(proc(name,0)) silently answered EXPRESSION -- grep the whole 0xFFFFFFFx space before adding one");
DESCR_SASSERT(PROCVAL_SLEN == 0xFFFFFFFEu,
    "PROCVAL_SLEN is HAND-COPIED in src/runtime/rtx/rtx_icncall.s, which mints the ordinary form in asm where no assert "
    "in this header can see it; this arm pins the C side so a silent divergence needs two deliberate edits, not one");
#define PROCVAL_EXTERNAL_SLEN 0xFFFFFFFBu
DESCR_SASSERT(PROCVAL_EXTERNAL_SLEN != PROCVAL_SLEN && PROCVAL_EXTERNAL_SLEN != PROCVAL_BUILTIN_SLEN && PROCVAL_EXTERNAL_SLEN != 0xFFFFFFFFu && PROCVAL_EXTERNAL_SLEN != 0xFFFFFFFDu,
    "PROCVAL_EXTERNAL_SLEN joins the ONE shared 0xFFFFFFFx sentinel space this header's PROCVAL_BUILTIN_SLEN assert already warns about; 0xFFFFFFFB was free when it was minted and this arm keeps it "
    "free");
typedef struct _EXTFN_t { void *fn; char name[]; } EXTFN_t;
typedef struct _CVSPINE_t { long fn; long act0; } CVSPINE_t;
DESCR_SASSERT(sizeof(CVSPINE_t) == 16,
    "CVSPINE_t is the two-register answer rt_call_value_spine_prep and rt_call_apply_spine_prep hand bb_call_value: fn in rax is the callee box entry and 0 declines, act0 in rdx is the word the box "
    "banks in callgen.act +0 BEFORE it transfers -- 0 when the callee may be re-driven on beta (a jmp-entry generator; the box still writes 1 at its first gamma, which is the pre-2026-09-19 behaviou"
    "r unchanged) and 2 when it may not (a plain procedure returns once, so beta fails forward instead of resuming a frame the callee already released). MEASURED (ceo 2026-09-19, row gc-the-five-c-t"
    "o-bb-entries-outside-rt-c-go-to-zero): opening a plain procedure on the spine WITHOUT this word hangs every redo -- `every x := f()` and a failing comparison each loop forever at 10 s timeout i"
    "n both modes -- because beta's `cmp rax,1` cannot tell a suspended generator from a returned procedure, while a SINGLE call is byte-correct. That is why the old rt_proc_is_generator refusal was"
    " not a policy. Two longs so the pair rides rax:rdx with no memory traffic, the shape rt_call_next_t and rt_prim_next_t already use. 3 | cells<<8 | ftc<<32 is a region-frame (n2)"
    " generator the Icon block prep opened for bb_call_value's n2 road (hq_icon 2026-10-14): before it every such call ran on a pthread of its own.");
static inline __attribute__((always_inline)) int IS_PROCVAL_EXTERNAL_fn(DESCR_t v) { return v.v == DT_E && v.slen == PROCVAL_EXTERNAL_SLEN; }
#define PROCVAL_EXTERNAL(blk_) ((DESCR_t){ .v = DT_E, .slen = PROCVAL_EXTERNAL_SLEN, .s = (blk_)->name })
#define PROCVAL_EXT_FN(v_) (((EXTFN_t *)(void *)((char *)(v_).s - offsetof(EXTFN_t, name)))->fn)
static inline __attribute__((always_inline)) int IS_PROCVAL_fn(DESCR_t v) { return v.v == DT_E && (v.slen == PROCVAL_SLEN || v.slen == PROCVAL_BUILTIN_SLEN || v.slen == PROCVAL_EXTERNAL_SLEN); }
static inline __attribute__((always_inline)) int IS_PROCVAL_BUILTIN_fn(DESCR_t v) { return v.v == DT_E && v.slen == PROCVAL_BUILTIN_SLEN; }
#define PROCVAL_BUILTIN(name_) ((DESCR_t){ .v = DT_E, .slen = PROCVAL_BUILTIN_SLEN, .s = (char *)(name_) })
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) uint32_t descr_cstrlen(const char *s_) { return s_ ? (uint32_t)__builtin_strlen(s_) : 0u; }
const char *kw_cset_empty(void);
#define CSET_BLK_HDR 40
#define CSET_BLK_LEN_OFF (-40)
#define CSET_BLK_MAGIC_OFF (-36)
#define CSET_BLK_BITS_OFF (-32)
#define CSET_BLK_MAGIC 0xC5E7B175u
static inline __attribute__((always_inline)) int cset_blk_len(const char *p) { int32_t n; __builtin_memcpy(&n, p + CSET_BLK_LEN_OFF, 4); return (int)n; }
static inline __attribute__((always_inline)) const unsigned char *cset_blk_bits(const char *p) { return (const unsigned char *)(p + CSET_BLK_BITS_OFF); }
static inline __attribute__((always_inline)) size_t descr_slen(DESCR_t d) {
    if (d.v == DT_S) { if (d.slen == 0xFFFFFFFFu) return d.s ? (size_t)cset_blk_len(d.s) : 0; return (size_t)d.slen; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_cstr_materialize(DESCR_t d);
static inline __attribute__((always_inline)) const char *rt_cstr_d(DESCR_t d) {
    if (d.v != DT_S || !d.s || !d.slen || d.slen == 0xFFFFFFFFu) return d.s ? d.s : "";
    return d.s[d.slen] ? rt_cstr_materialize(d) : d.s;
}
#define NULVCL ((DESCR_t){ .v = DT_SNUL, .slen = 0, .s = "" })
#define STRVAL(s_) __extension__({ char *_sv_ = (char *)(s_); (DESCR_t){ .v = DT_S, .slen = descr_cstrlen(_sv_), .s = _sv_ }; })
#define BSTRVAL(s_, len_) ((DESCR_t){ .v = DT_S, .slen = (uint32_t)(len_), .s = (s_) })
#define INTVAL(i_) ((DESCR_t){ .v = DT_I, .i = (i_) })
#define REALVAL(r_)((DESCR_t){ .v = DT_R, .r = (r_) })
#define CSETVAL(s_) ((DESCR_t){ .v = DT_S, .slen = 0xFFFFFFFFu, .s = (s_) })
#define NAMEPTR(dp_) ((DESCR_t){ .v = DT_N, .slen = 1, .ptr = (void*)(dp_) })
static inline __attribute__((always_inline)) DESCR_t NAME_AS_STR_fn(DESCR_t d) { return (d.v == DT_N && d.slen == 0 && d.s && !IS_KWNAME_fn(d)) ? STRVAL(d.s) : d; }
#define NAMEVAL(s_) ((DESCR_t){ .v = DT_N, .slen = 0, .s = (char *)(s_) })
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) int IS_NULL_fn(DESCR_t v) { return v.v == DT_SNUL || (v.v == DT_S && v.slen == 0 && (!v.s || !*v.s)); }
static inline __attribute__((always_inline)) int IS_STR_fn(DESCR_t v) { return v.v == DT_S || v.v == DT_SNUL; }
static inline __attribute__((always_inline)) int IS_INT_fn(DESCR_t v) { return v.v == DT_I; }
static inline __attribute__((always_inline)) int IS_REAL_fn(DESCR_t v) { return v.v == DT_R; }
static inline __attribute__((always_inline)) int IS_CSET_fn(DESCR_t v) { return v.v == DT_S && v.slen == 0xFFFFFFFFu; }
static inline __attribute__((always_inline)) int IS_DATA_fn(DESCR_t v) { return v.v == DT_DATA; }
#ifdef __cplusplus
}
#endif
#endif
