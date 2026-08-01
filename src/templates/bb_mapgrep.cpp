#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "descr.h"
void    bb_reset(IR_graph_t * bbg);
DESCR_t NV_SET_fn(const char * name, DESCR_t val);
}
#include "x86_asm.h"
#define MAPGREP_MAX 4096
static int64_t s_mg_vals[MAPGREP_MAX];
static struct { int n; uint64_t vals_ptr; char lbl[64]; int cursoff; int resoff; } s_mg;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_mapgrep() {
    x86_begin();
    if (PLATFORM_X86) return x86_alpha()
         + x86("comment",   "IR_MAP/IR_GREP (materialized)")
         + IF(MEDIUM_TEXT,
               x86("directive", ".section .rodata")
             + x86("directive", std::string(s_mg.lbl) + ": .quad "
                 + (s_mg.n > 0
                     ? [&]{ std::string q; for (int i = 0; i < s_mg.n; i++) q += (i ? std::string(", ") : std::string("")) + std::to_string((long long)s_mg_vals[i]); return q; }()
                     : std::string("0")))
             + x86("directive", ".section .text")
             + x86("directive", ".intel_syntax noprefix"))
         + x86("def",       L(0))
         + x86("mov",       "rcx", FRQ(s_mg.cursoff))
         + x86("cmp64",     "rcx", (long)s_mg.n)
         + x86_omega("jge")
         + x86("lea",       "rdx", "[rip + __]", s_mg.vals_ptr, s_mg.lbl)
         + x86("mov",       "rsi", "[rdx + rcx*8]")
         + x86("mov",       FRQ(s_mg.resoff), (long)DT_I)
         + x86("mov",       FRQ(s_mg.resoff + 8), "rsi")
         + x86("inc",       FRQ(s_mg.cursoff))
         + x86_gamma()
         + x86_beta()
         + x86("jmp",       L(0));
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_mapgrep_prepare(IR_t * nd) {
    (void)nd;
    fprintf(stderr, "[NO-IR-INTERP] bb_mapgrep_prepare: map/grep materialized via the IR interpreter (deleted); native Raku map/grep emission pending\n");
    abort();
}
