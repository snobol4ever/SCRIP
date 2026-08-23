#include "descr.h"
#include "core.h"
#include "gc_heap.h"
#include "pin_va.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_table;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_misc;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_alloc;
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_str;
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
extern __attribute__((visibility("hidden"))) unsigned char rtx_gate_plunify;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned char rtx_env_on(const char *name, unsigned char dflt) { const char *e = getenv(name); if (!e || !*e) return dflt; return (unsigned char)(e[0] != '0'); }
static unsigned char rtx_mon_dflt(void) { const char *m = getenv("MONITOR_BIN"); return (unsigned char)((m && *m && m[0] != '0') ? 0 : 1); }
__attribute__((constructor)) static void rtx_gates_init(void) { unsigned char d = rtx_mon_dflt(); rtx_gate_misc = rtx_env_on("SCRIP_RTX_MISC", d); rtx_gate_alloc = rtx_env_on("SCRIP_RTX_ALLOC", d); rtx_gate_str = rtx_env_on("SCRIP_RTX_STR", d); rtx_gate_leaf = rtx_env_on("SCRIP_RTX_LEAF", d); rtx_gate_arith = rtx_env_on("SCRIP_RTX_ARITH", d); rtx_gate_icnvar = rtx_env_on("SCRIP_RTX_ICNVAR", d); rtx_gate_icnnum = rtx_env_on("SCRIP_RTX_ICNNUM", d); rtx_gate_icnrel = rtx_env_on("SCRIP_RTX_ICNREL", d); rtx_gate_icnagg = rtx_env_on("SCRIP_RTX_ICNAGG", d); rtx_gate_match = rtx_env_on("SCRIP_RTX_MATCH", d); rtx_gate_icngen = rtx_env_on("SCRIP_RTX_ICNGEN", d); rtx_gate_icncall = rtx_env_on("SCRIP_RTX_ICNCALL", d); rtx_gate_icnsub = rtx_env_on("SCRIP_RTX_ICNSUB", d); rtx_gate_plunify = rtx_env_on("SCRIP_RTX_PLUNIFY", d);
  rtx_gate_table = rtx_env_on("SCRIP_RTX_TABLE", d);
  /*⛔ THE TWO KILLSWITCHES INTERLOCK, AND THEY MUST.  SCRIP_TBL_TYPED=0 makes aggregates.c hash the STRINGIFIED key (tbl_key_str then djb2) instead of hashing by
     datatype; rtx_table.S implements only the typed algorithms.  Leaving the ASM gate armed in that mode would put the ASM and the C in DIFFERENT BUCKETS for the
     same key -- a silent, data-dependent miss, which is the exact hazard the s262 rewrite removed everywhere else by construction.  So the C-side killswitch
     disarms the ASM too, and the A/B stays honest. */
  { const char *tt = getenv("SCRIP_TBL_TYPED"); if (tt && *tt == (char)48) rtx_gate_table = 0; } }
extern int Σlen; extern uint32_t g_cap_gen; extern uint32_t g_cap_gen_next;
_Static_assert(sizeof(g_cap_gen) == 4 && sizeof(g_cap_gen_next) == 4, "rtx_match.S stores g_cap_gen/g_cap_gen_next with `dword ptr` because they sit ADJACENT at +0x0/+0x4 in pattern_match.o; if either widens, that dword store truncates and an 8-byte store would clobber the sibling generation well -- links fine, passes short tests (s218)");
_Static_assert(sizeof(Σlen) == 4, "rtx_match.S stores Σlen with `dword ptr` (rt_match_enter, rt_match_ctx_restore); Σlen is `int` and has a neighbour at +0x14 in stmt_exec.o -- a qword store there is the s217 store-width class");
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
_Static_assert(sizeof(TBPAIR_t) == 48 && offsetof(TBPAIR_t, key) == 0 && offsetof(TBPAIR_t, key_descr) == 8 && offsetof(TBPAIR_t, val) == 24 && offsetof(TBPAIR_t, hkey) == 40, "rtx_icnsub.S RTX-26 hardcodes TBPAIR_t.key/.key_descr/.val/.hkey; core.h drifted -- the bucket search would compare the wrong bytes or name the wrong cell.  ⛔ .next RETIRED s262: buckets are sorted contiguous arrays, and those 8 bytes now carry the (datatype,value) sort key");
_Static_assert(offsetof(TBBLK_t, buckets) == 0 && offsetof(TBBLK_t, nbuck) == 8 && sizeof(TBBUCK_t *) == 8 && offsetof(TBBUCK_t, len) == 0 && offsetof(TBBUCK_t, cap) == 4 && offsetof(TBBUCK_t, ent) == 8,
               "rtx_table.S loads TBBLK_t.buckets (the vector) and TBBLK_t.nbuck (the mask) from fixed offsets and reads {len,cap,ent[]} out of the bucket block; core.h drifted -- every lookup would start from the wrong word.  "
               "⛔ THE BUCKET COUNT IS PER-TABLE SINCE s263, sized from the program's TABLE(n) estimate, so the .S MUST load the mask and must never bake one: a literal was correct only while every table had 256 buckets");
/*⛔ THE COMPILE-TIME TABLE_BUCKETS ASSERT IS RETIRED (s263).  There is no longer one bucket count to pin: each table sizes its own vector from its TABLE(n)
  estimate, the power-of-two invariant is established by _tbl_nbuck_for and by _tbl_rehash doubling, and rtx_table.S LOADS the mask instead of baking it.
  What replaced the guarantee is the offset assert above -- if TBBLK_t.nbuck ever moves, the .S reads a mask out of the wrong word and the build breaks. */
_Static_assert(DT_A == 0x10, "rtx_icnsub.S RTX-28 array arm guards on DT_A AND branches on it AHEAD of the DT_N varref gate; descr.h drifted -- the arm would admit the wrong datatype and index a non-array, which links fine and corrupts silently");
_Static_assert(sizeof(ARBLK_t) == 48 && offsetof(ARBLK_t, lo) == 0 && offsetof(ARBLK_t, hi) == 4 && offsetof(ARBLK_t, ndim) == 8 && offsetof(ARBLK_t, data) == 32, "rtx_icnsub.S RTX-28 hardcodes ARBLK_t.lo/.hi/.ndim/.data; core.h drifted -- the bounds test would read the wrong ints and the cell address would be computed off the wrong member, which links fine and hands back a pointer into arbitrary memory");
_Static_assert(offsetof(DATBLK_t, nfields) == 8 && offsetof(DATBLK_t, fields) == 16, "rtx_icnsub.S hardcodes DATBLK_t.nfields/.fields; core.h drifted -- the frame_elems check would compare the wrong bytes and admit a record as a list");
