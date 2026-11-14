#include "ir/descr_tags.h"
#include "ir/descr_tags_dt_platom.h"
#include "ir/descr_tags_descr_sassert.h"
#include "ir/descr.h"
DESCR_SASSERT(DT_SNUL == 0, "DT_SNUL must stay 0: bulk memset init mints null strings for free");
DESCR_SASSERT((DT_I & DT_NUMERIC_BIT) && (DT_R & DT_NUMERIC_BIT), "NUMERIC is bit0");
DESCR_SASSERT(!(DT_S & DT_NUMERIC_BIT) && !(DT_SNUL & DT_NUMERIC_BIT), "strings never read numeric");
DESCR_SASSERT(!(DT_I & DT_REAL_BIT) && (DT_R & DT_REAL_BIT), "REAL is bit2");
DESCR_SASSERT(((DT_I & DT_R) != DT_I) && ((DT_I & DT_R) & DT_NUMERIC_BIT), "DT_I must NOT be a subset of DT_R (or BOTH INT dies) yet must share NUMERIC (or BOTH NUMERIC dies)");
DESCR_SASSERT(((DT_SNUL | DT_S) & (DT_NOTSTR_MASK & 0xFF)) == 0,
    "SNUL|S must vanish under the 8-bit string mask -- every .S string-family test reads only the tag byte, never the 32-bit word (that word's bits 8-31 carry the DESCR provenance stamp, see ARCH-SN"
    "OBOL4-RTX.md sec9)");
DESCR_SASSERT((DT_I & (DT_NOTSTR_MASK & 0xFF)) && (DT_R & (DT_NOTSTR_MASK & 0xFF)), "numerics must NOT read as string under the 8-bit mask");
DESCR_SASSERT(!(DT_FAIL & DT_NUMERIC_BIT) && (DT_FAIL & (DT_NOTSTR_MASK & 0xFF)), "DT_FAIL must read as neither numeric nor string under the 8-bit mask");
DESCR_SASSERT(!(DT_BOOL & DT_NUMERIC_BIT) && (DT_BOOL & (DT_NOTSTR_MASK & 0xFF)),
    "DT_BOOL must read as neither numeric nor string under the 8-bit mask (R-654): a Bool renders True/False, and only a Raku-own arm may coerce it to 1/0");
DESCR_SASSERT(!(DT_ORDER & DT_NUMERIC_BIT) && (DT_ORDER & (DT_NOTSTR_MASK & 0xFF)),
    "DT_ORDER must read as neither numeric nor string under the 8-bit mask, same shape as DT_BOOL above: a Raku Order renders Less/Same/More, and only a Raku-own arm may coerce it to -1/0/1");
DESCR_SASSERT(!(DT_DATA & DT_NUMERIC_BIT) && !(DT_DATA_STRIDE & DT_NUMERIC_BIT), "DATA base and stride must leave NUMERIC clear so no user datatype enters the arith fast path");
DESCR_SASSERT((RTX_NOT_HANDLED & 7) == 7 && (DT_DATA_STRIDE & 7) == 0 && (DT_DATA & 7) == 0,
    "RTX_NOT_HANDLED is the byte a verdict-road leaf returns in al and never the low byte of a tag: every tag's low three bits read 0, 2, 3 or 5");
DESCR_SASSERT(DT_FAIL < DT_DATA, "the v >= DT_DATA range tests require every fixed tag below DT_DATA");
DESCR_SASSERT(DT_RAW > DT_ORDER,
    "DT_RAW is the value-never-a-pointer stack-cell code of ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 6.1 (CTO-65; its twin, the frame-marker tag, went with the marker scan at STEP B): every code a"
    "bove DT_ORDER is a cell the collector never visits a" "s a value, and the three open-ended v >= DT_DATA tests (driver_data.c, core.c, by_name_dispatch.c) carry a v < DT_RAW bound for that reason"
    );
DESCR_SASSERT(!(DT_RAW & DT_NUMERIC_BIT) && (DT_RAW & (DT_NOTSTR_MASK & 0xFF)), "DT_RAW reads as neither numeric nor string under the 8-bit mask, like DT_FAIL");
DESCR_SASSERT(DT_T - DT_A == 8, "rtx_icnsub.s array+table share one subscript range guard");
DESCR_SASSERT(DT_PLATOM > DT_CPLX && DT_PLATOM < 0x100 && !(DT_PLATOM & DT_NUMERIC_BIT) && (DT_PLATOM & (DT_NOTSTR_MASK & 0xFF)),
    "DT_PLATOM is the Prolog atom, {v, slen 0, i = atom id} (ARCH-PROLOG-BB-REWRITE.md section 3, rung R1): it replaced the builder form that wore DT_A, "
    "the SNOBOL4/Icon ARRAY tag, with an integer payload -- the collector's DT_A arm read atom id 0x8e as an ARBLK pointer (row prolog-an-atom-wears-the-array-tag). "
    "It is safe under the collector because gc_type_says_ref is an explicit switch whose default returns 0, NOT because it sits above DT_ORDER (DT_CPLX sits above "
    "DT_ORDER and is a ref); IS_DATA_TAG_fn's v < DT_RAW bound excludes it; and it reads neither numeric nor string under the 8-bit mask, like DT_FAIL");
DESCR_SASSERT(DT_PCELL > DT_EXTL && DT_PCELL < 0x100 && !(DT_PCELL & DT_NUMERIC_BIT) && (DT_PCELL & (DT_NOTSTR_MASK & 0xFF)) && (DT_PCELL & 7) == 0,
    "DT_PCELL is the Pascal pointer to a cell, {v, slen = element index (signed 32), ptr = the address of the cell holding element 0} (ARCH-PASCAL-REDESIGN.md R5 amended (1)): it is neither numeric "
    "nor string, " "sits above DT_ORDER so the v >= DT_DATA tests exclude it by their v < DT_RAW bound, and the collector visits it as the DT_S interior-pointer arm (MK_PLAIN_I)");
DESCR_SASSERT(DT_XR > DT_PCELL && DT_XR < 0x100 && !(DT_XR & DT_NUMERIC_BIT) && (DT_XR & (DT_NOTSTR_MASK & 0xFF)) && (DT_XR & 7) == 0,
    "DT_XR is the Pascal Extended (ARCH-PASCAL-REDESIGN.md R6 amended, CTO-486), an x87 80-bit value held INLINE: {v, slen = sign and exponent in the low 16 bits and the upper 16 bits zero, "
    "i = the 64-bit mantissa with its explicit integer bit}; it is neither numeric nor string under the 8-bit masks (only the __pas_xr_ arms read it, behind the static type lower has), "
    "it bears no pointer so the collector never visits it (MK class 0, gc_tag_bears_ptr 0), and every __pas_xr_ arm writes the whole 16-byte cell");
DESCR_SASSERT(sizeof(DESCR_t) == 16, "DESCR_t is a SysV register-pair (rax:rdx) INTEGER-class return; 17+ bytes flips it to MEMORY class across 4,009 lines of asm");
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
DESCR_SASSERT(ICN_FIELD_PLAIN > 0xFFFF && ICN_FIELD_PLAIN < 0x7FFFFFFF,
    "an Icon record-field variable node carries the field's 1-based position in IR_t.seal; the lowerer ORs ICN_FIELD_PLAIN into it where the variable's only consumer is a store "
    "(an assignment or an augmented assignment), so rt_field_var_strict_at answers the bare DT_N slen 1 cell and builds no VCELL and no Rec.field name string -- name() is never asked "
    "of such a node; the bit sits above any record's field count and inside the 32-bit mov that loads the seal.");
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
DESCR_SASSERT(PROCVAL_SLEN != PROCVAL_BUILTIN_SLEN, "the two procedure-value markers must differ, or proc(s,0) is indistinguishable from a global of the same name");
DESCR_SASSERT(PROCVAL_BUILTIN_SLEN != 0xFFFFFFFFu && PROCVAL_BUILTIN_SLEN != 0xFFFFFFFDu,
    "slen sentinels share ONE 32-bit space across unrelated headers and nothing collects them: 0xFFFFFFFF is CSETVAL, "
    "0xFFFFFFFD is core.h RT_CONVE_CHAIN_MARK. PROCVAL_BUILTIN_SLEN was first written 0xFFFFFFFD, every board stayed "
    "green, and type(proc(name,0)) silently answered EXPRESSION -- grep the whole 0xFFFFFFFx space before adding one");
DESCR_SASSERT(PROCVAL_SLEN == 0xFFFFFFFEu,
    "PROCVAL_SLEN is HAND-COPIED in src/runtime/rtx/rtx_icncall.s, which mints the ordinary form in asm where no assert "
    "in this header can see it; this arm pins the C side so a silent divergence needs two deliberate edits, not one");
DESCR_SASSERT(PROCVAL_EXTERNAL_SLEN != PROCVAL_SLEN && PROCVAL_EXTERNAL_SLEN != PROCVAL_BUILTIN_SLEN && PROCVAL_EXTERNAL_SLEN != 0xFFFFFFFFu && PROCVAL_EXTERNAL_SLEN != 0xFFFFFFFDu,
    "PROCVAL_EXTERNAL_SLEN joins the ONE shared 0xFFFFFFFx sentinel space this header's PROCVAL_BUILTIN_SLEN assert already warns about; 0xFFFFFFFB was free when it was minted and this arm keeps it "
    "free");
DESCR_SASSERT(sizeof(CVSPINE_t) == 16,
    "CVSPINE_t is the two-register answer rt_call_value_spine_prep and rt_call_apply_spine_prep hand bb_call_value: fn in rax is the callee box entry and 0 declines, act0 in rdx is the word the box "
    "banks in callgen.act +0 BEFORE it transfers -- 0 when the callee may be re-driven on beta (a jmp-entry generator; the box still writes 1 at its first gamma, which is the pre-2026-09-19 behaviou"
    "r unchanged) and 2 when it may not (a plain procedure returns once, so beta fails forward instead of resuming a frame the callee already released). MEASURED (2026-09-19, row gc-the-five-c-t"
    "o-bb-entries-outside-rt-c-go-to-zero): opening a plain procedure on the spine WITHOUT this word hangs every redo -- `every x := f()` and a failing comparison each loop forever at 10 s timeout i"
    "n both modes -- because beta's `cmp rax,1` cannot tell a suspended generator from a returned procedure, while a SINGLE call is byte-correct. That is why the old rt_proc_is_generator refusal was"
    " not a policy. Two longs so the pair rides rax:rdx with no memory traffic, the shape rt_call_next_t and rt_prim_next_t already use. 3 | cells<<8 | ftc<<32 is a region-frame (n2)"
    " generator the Icon block prep opened for bb_call_value's n2 road (hq_icon 2026-10-14): before it every such call ran on a pthread of its own.");
