#include <string>
#include <cstring>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
#include "descr.h"
int  bb_slot_claim(int bytes);
int  bb_slot_alloc16(IR_t * nd);
DESCR_t IR_interp_once(IR_graph_t * bbg);
DESCR_t IR_interp_resume(IR_graph_t * bbg);
void    bb_reset(IR_graph_t * bbg);
DESCR_t NV_SET_fn(const char * name, DESCR_t val);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define MAPGREP_MAX 4096
static int64_t  s_mg_vals[MAPGREP_MAX];
static int      s_mg_n;
static uint64_t s_mg_vals_ptr;
static char     s_mg_lbl[64];
static int      s_mg_cursoff;
static int      s_mg_resoff;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int          mgN()      { return s_mg_n; }
static inline uint64_t     mgValsP()  { return s_mg_vals_ptr; }
static inline const char * mgLbl()    { return s_mg_lbl; }
static inline int          mgCurs()   { return s_mg_cursoff; }
static inline int          mgRes()    { return s_mg_resoff; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_mapgrep() {
    x86_begin();
    if (PLATFORM_X86) return x86("label",    _.lbl_α)
         + x86("comment",   "IR_MAP/IR_GREP (materialized)")
         + IF(MEDIUM_TEXT,
               x86("directive", ".section .rodata")
             + x86("directive", std::string(mgLbl()) + ": .quad "
                 + (mgN() > 0
                     ? [&]{ std::string q; for (int i = 0; i < mgN(); i++) q += (i ? std::string(", ") : std::string("")) + std::to_string((long long)s_mg_vals[i]); return q; }()
                     : std::string("0")))
             + x86("directive", ".section .text")
             + x86("directive", ".intel_syntax noprefix"))
         + x86("def",       L(0))
         + x86("mov",       "rcx", FRQ(mgCurs()))
         + x86("cmp64",     "rcx", (long)mgN())
         + x86("jge",       "ω")
         + x86("lea",       "rdx", "[rip + __]", mgValsP(), mgLbl())
         + x86("mov",       "rsi", "[rdx + rcx*8]")
         + x86("mov",       FRQ(mgRes()), (long)6)
         + x86("mov",       FRQ(mgRes() + 8), "rsi")
         + x86("inc",       FRQ(mgCurs()))
         + x86("jmp",       "γ")
         + x86("def",       "β")
         + x86("jmp",       L(0));
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_mapgrep_prepare(IR_t * nd) {
    int is_grep = (nd->op == IR_GREP);
    IR_graph_t * src_sg  = (IR_graph_t *)(intptr_t) IR_EXEC(nd).counter;
    IR_graph_t * body_sg = (IR_graph_t *)(intptr_t) IR_LIT(nd).ival;
    if (!src_sg || !body_sg) {
        fprintf(stderr, "[RK] FATAL bb_mapgrep: missing source/body sub-graph (map/grep)\n");
        abort();
    }
    int n = 0;
    bb_reset(src_sg);
    DESCR_t sv = IR_interp_once(src_sg);
    int safety = src_sg->n * 256 + 4096;
    while (!IS_FAIL_fn(sv) && safety-- > 0) {
        NV_SET_fn("_", sv);
        bb_reset(body_sg);
        DESCR_t bv = IR_interp_once(body_sg);
        DESCR_t keep;
        int emit_one = 1;
        if (is_grep) { if (IS_FAIL_fn(bv)) emit_one = 0; keep = sv; }
        else         { keep = IS_FAIL_fn(bv) ? (DESCR_t){ DT_SNUL, 0, { 0 } } : bv; }
        if (emit_one) {
            if (keep.v != DT_I && keep.v != DT_SNUL) {
                fprintf(stderr, "[RK] FATAL bb_mapgrep: materialized value is not integer (v=%d); map/grep native arm supports integer sequences only\n", (int)keep.v);
                abort();
            }
            if (n >= MAPGREP_MAX) { fprintf(stderr, "[RK] FATAL bb_mapgrep: sequence exceeds %d elements\n", MAPGREP_MAX); abort(); }
            s_mg_vals[n++] = (keep.v == DT_I) ? keep.i : 0;
        }
        sv = IR_interp_resume(src_sg);
    }
    s_mg_n        = n;
    s_mg_vals_ptr = (uint64_t)(uintptr_t)(const void *)s_mg_vals;
    snprintf(s_mg_lbl, sizeof s_mg_lbl, ".Lmapgrep%d_vals", _.nid);
    s_mg_resoff  = bb_slot_alloc16(nd);
    s_mg_cursoff = bb_slot_claim(8);
}
