#include "descr.h"
#include "core.h"
#include "gc_heap.h"
#include "pin_va.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_misc;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_alloc;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_str;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_call;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_leaf;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_arith;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_icnvar;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_icnnum;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_icnrel;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_icnagg;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_match;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_icngen;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_icncall;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_icnsub;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_plcall;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_plunify;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned char rtx_env_on(const char *name, unsigned char dflt) { const char *e = getenv(name); if (!e || !*e) return dflt; return (unsigned char)(e[0] != '0'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((constructor)) static void rtx_gates_init(void) { rtx_gate_misc = rtx_env_on("SCRIP_RTX_MISC", 1); rtx_gate_alloc = rtx_env_on("SCRIP_RTX_ALLOC", 1); rtx_gate_str = rtx_env_on("SCRIP_RTX_STR", 1); rtx_gate_call = rtx_env_on("SCRIP_RTX_CALL", 1); rtx_gate_leaf = rtx_env_on("SCRIP_RTX_LEAF", 1); rtx_gate_arith = rtx_env_on("SCRIP_RTX_ARITH", 1); rtx_gate_icnvar = rtx_env_on("SCRIP_RTX_ICNVAR", 1); rtx_gate_icnnum = rtx_env_on("SCRIP_RTX_ICNNUM", 1); rtx_gate_icnrel = rtx_env_on("SCRIP_RTX_ICNREL", 1); rtx_gate_icnagg = rtx_env_on("SCRIP_RTX_ICNAGG", 1); rtx_gate_match = rtx_env_on("SCRIP_RTX_MATCH", 1); rtx_gate_icngen = rtx_env_on("SCRIP_RTX_ICNGEN", 1); rtx_gate_icncall = rtx_env_on("SCRIP_RTX_ICNCALL", 1); rtx_gate_icnsub = rtx_env_on("SCRIP_RTX_ICNSUB", 1); rtx_gate_plcall = rtx_env_on("SCRIP_RTX_PLCALL", 1); rtx_gate_plunify = rtx_env_on("SCRIP_RTX_PLUNIFY", 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int Σlen; extern uint32_t g_cap_gen; extern uint32_t g_cap_gen_next;
_Static_assert(sizeof(g_cap_gen) == 4 && sizeof(g_cap_gen_next) == 4, "rtx_match.S stores g_cap_gen/g_cap_gen_next with `dword ptr` because they sit ADJACENT at +0x0/+0x4 in pattern_match.o; if either widens, that dword store truncates and an 8-byte store would clobber the sibling generation well -- links fine, passes short tests (s218)");
_Static_assert(sizeof(Σlen) == 4, "rtx_match.S stores Σlen with `dword ptr` (rt_match_enter, rt_match_ctx_restore); Σlen is `int` and has a neighbour at +0x14 in stmt_exec.o -- a qword store there is the s217 store-width class");
/* s220 CORRECTION TO THE s218 ITEM AS WORDED: the s218 cursor asked for a _Static_assert on the g_cap_gen/g_cap_gen_next
 * and Σ/Σlen ADJACENCIES, and that is why it stayed undone for three sessions -- C cannot express it. offsetof works
 * only inside an aggregate; two independent globals have no statically-known relative address, so no _Static_assert
 * can name their distance. WHAT IS EXPRESSIBLE IS THE WIDTH, and width is the whole of what the adjacency endangers:
 * the hazard is never "they moved apart", it is "a store wider than the target reached the neighbour." The two asserts
 * above pin that statically; scripts/test_gate_rtx_store_width.sh (s219b) enforces the same property dynamically from
 * ELF symbol sizes, which additionally catches a store to a global these asserts do not name. Item closed as CORRECTED,
 * not as done-as-asked. */
_Static_assert(RT_DCAP_TOP == 0x70000000UL, "rtx_match.S's rt_match_enter hardcodes RTX_DCAP_TOP_VA 0x70000000 as an absolute disp32 to inline the rt_dcap_lazy_init test; pin_va.h moved the pin -- the asm would test a DEAD page, read 0 forever, and call the initializer on EVERY match instead of once");
extern __attribute__((visibility("hidden"))) int g_repl_trace;
_Static_assert(sizeof(g_repl_trace) == 4, "rtx_match.S SLICE 9 tests g_repl_trace with `cmp dword ptr`; if it widens, the compare reads half the flag and the asm would run the hot arm while tracing is on -- silently swallowing every [REPL] line the flag exists to produce");
_Static_assert(offsetof(DESCR_t, v) == 0 && offsetof(DESCR_t, slen) == 4 && offsetof(DESCR_t, s) == 8, "rtx_match.S SLICE 9 reads *replp field-by-field as [r9+0]/[r9+4]/[r9+8] and mints its result descriptor as rsi = (nlen << 32) | DT_S with rdx = buf; descr.h drifted -- the replacement's datatype/length would be read from the wrong words and the assignment would link fine and write a malformed descriptor");
_Static_assert(DT_SNUL == 0x00,  "rtx_abi.inc hardcodes DT_SNUL 0; descr.h drifted -- the asm tag compares would link fine and silently mis-compare");
_Static_assert(DT_S    == 0x02,  "rtx_abi.inc hardcodes DT_S 1; descr.h drifted -- update src/runtime/rtx/rtx_abi.inc to match");
_Static_assert(DT_P    == 0x08,  "rtx_abi.inc hardcodes DT_P 3; descr.h drifted -- str_concat_d's pattern guard would stop routing to pat_cat");
_Static_assert(DT_X    == 0x58, "rtx_abi.inc hardcodes DT_X 15; descr.h drifted -- str_concat_d's pattern guard would stop routing to pat_cat");
_Static_assert(HB_AGGV == 206, "rtx_alloc.S hardcodes HB_AGGV 206; gc_heap.h drifted -- rt_agg_alloc would tag every aggregate cell with the WRONG block type, which links fine, allocates fine, and corrupts the GC's precise-visit classification silently");
_Static_assert(DT_FAIL == 0x68, "rtx_abi.inc hardcodes DT_FAIL 99; descr.h drifted -- FAILDESCR precedence in the null-identity arm would break");
_Static_assert(offsetof(VCELL_t, cellp) == 0, "rtx_icnvar.S hardcodes VCELL_t.cellp at offset 0; descr.h drifted -- the NAMETRAP fast arm would store a DESCR_t through the wrong member, which links fine and corrupts silently");
_Static_assert(DT_E == 0x38, "rtx_icncall.S hardcodes DT_E 11; descr.h drifted -- rt_proc_value would mint procedure values with the WRONG TAG, which links fine and silently changes procedure identity rather than crashing");
_Static_assert(DT_DATA == 0x70 && DT_S == 0x02 && DT_I == 0x03 && DT_N == 0x28, "rtx_icnsub.S hardcodes the DT_DATA/DT_S/DT_I/DT_N tags; descr.h drifted -- the list-arm guard would admit the wrong datatype and mint a VCELL over a non-list, which links fine and corrupts silently");
_Static_assert(sizeof(DESCR_t) == 16, "rtx_icnsub.S shifts the element index left by 4 to scale by sizeof(DESCR_t); descr.h drifted -- vc->cellp would point BETWEEN elements");
_Static_assert(sizeof(VCELL_t) == 72, "rtx_icnsub.S passes sizeof(VCELL_t)=72 to rt_agg_alloc; descr.h drifted -- the carve would be short and the field stores would run off the end of the cell");
_Static_assert(offsetof(VCELL_t, tbl) == 8 && offsetof(VCELL_t, key) == 16 && offsetof(VCELL_t, key_d) == 24 && offsetof(VCELL_t, sv) == 40 && offsetof(VCELL_t, pos) == 56 && offsetof(VCELL_t, len) == 64, "rtx_icnsub.S hardcodes the VCELL_t field offsets; descr.h drifted -- the list arm would fill the wrong members, which links fine and yields a cell naming the wrong storage");
_Static_assert(offsetof(DATINST_t, type) == 0 && offsetof(DATINST_t, fields) == 8, "rtx_icnsub.S hardcodes DATINST_t.type/.fields; core.h drifted -- the inlined rt_list_view would read the genus tag from the wrong word");
_Static_assert(DT_T == 0x18, "rtx_icnsub.S RTX-26 table arm guards on DT_T; descr.h drifted -- the arm would admit the wrong datatype and read a bucket vector out of a non-table, which links fine and corrupts silently");
_Static_assert(sizeof(TBPAIR_t) == 48 && offsetof(TBPAIR_t, key) == 0 && offsetof(TBPAIR_t, val) == 24 && offsetof(TBPAIR_t, next) == 40, "rtx_icnsub.S RTX-26 hardcodes TBPAIR_t.key/.val/.next; core.h drifted -- the chain walk would compare the wrong bytes or name the wrong cell");
_Static_assert(offsetof(TBBLK_t, buckets) == 0, "rtx_icnsub.S RTX-26 indexes the bucket vector as [tb + h*8] with no displacement; core.h drifted -- every lookup would start from the wrong word");
_Static_assert(TABLE_BUCKETS == 256, "rtx_icnsub.S RTX-26 folds _tbl_hash s % TABLE_BUCKETS into movzx eax,al; core.h drifted -- the fold is only valid for 256 and would select the wrong bucket");
_Static_assert(DT_A == 0x10, "rtx_icnsub.S RTX-28 array arm guards on DT_A AND branches on it AHEAD of the DT_N varref gate; descr.h drifted -- the arm would admit the wrong datatype and index a non-array, which links fine and corrupts silently");
_Static_assert(sizeof(ARBLK_t) == 48 && offsetof(ARBLK_t, lo) == 0 && offsetof(ARBLK_t, hi) == 4 && offsetof(ARBLK_t, ndim) == 8 && offsetof(ARBLK_t, data) == 32, "rtx_icnsub.S RTX-28 hardcodes ARBLK_t.lo/.hi/.ndim/.data; core.h drifted -- the bounds test would read the wrong ints and the cell address would be computed off the wrong member, which links fine and hands back a pointer into arbitrary memory");
_Static_assert(offsetof(DATBLK_t, nfields) == 8 && offsetof(DATBLK_t, fields) == 16, "rtx_icnsub.S hardcodes DATBLK_t.nfields/.fields; core.h drifted -- the frame_elems check would compare the wrong bytes and admit a record as a list");
