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
    (void)nd;
    fprintf(stderr, "[NO-IR-INTERP] bb_mapgrep_prepare: map/grep materialized via the IR interpreter (deleted); native Raku map/grep emission pending\n");
    abort();
}
