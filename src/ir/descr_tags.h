#ifndef DESCR_TAGS_H
#define DESCR_TAGS_H
#define DT_NUMERIC_BIT 0x01
#define DT_CHARS_BIT 0x02
#define DT_REAL_BIT 0x04
#define DT_NOTSTR_MASK 0xFFFFFFFDu
#define DT_DATA_STRIDE 8
#define RTX_NOT_HANDLED 0xFF
#define RTX_NOT_HANDLED_INT 0xFFFFFFFE
#define RTX_NOT_HANDLED_PTR 0xFFFFFFFFFFFFFFFF
typedef enum {
    DT_SNUL = 0x00, DT_S = 0x02, DT_I = 0x03, DT_R = 0x05, DT_P = 0x08, DT_A = 0x10, DT_T = 0x18, DT_C = 0x20, DT_N = 0x28, DT_K = 0x30, DT_E = 0x38, DT_FH = 0x40, DT_PLVAR = 0x48, DT_PLREF = 0x50,
        DT_X = 0x58, DT_BLK = 0x60, DT_FAIL = 0x68, DT_DATA = 0x70, DT_BIG = 0x78, DT_CO = 0x80, DT_BOOL = 0x88, DT_ORDER = 0x90, DT_RAW = 0x98, DT_CPLX = 0xA8, DT_PLATOM = 0xB0, DT_EXTL = 0xB8,
        DT_PCELL = 0xC0, DT_XR = 0xC8,
} DTYPE_t;
#ifdef __cplusplus
#define DESCR_SASSERT(c, m) static_assert(c, m)
#else
#define DESCR_SASSERT(c, m) _Static_assert(c, m)
#endif
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
#endif
