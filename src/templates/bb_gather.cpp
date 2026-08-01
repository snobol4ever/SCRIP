#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
#define GATHER_MAX_TAKES 256
static struct { int64_t vals[GATHER_MAX_TAKES]; int n; uint64_t vals_ptr; char lbl[64]; int cursoff; int resoff; } s_g;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_gather() {
    x86_begin();
    if (PLATFORM_X86) return x86_alpha()
         + x86("comment",   "IR_GATHER")
         + IF(MEDIUM_TEXT,
               x86("directive", ".section .rodata")
             + x86("directive", std::string(s_g.lbl) + ": .quad "
                 + (s_g.n > 0
                     ? [&]{ std::string q; for (int i = 0; i < s_g.n; i++) q += (i ? std::string(", ") : std::string("")) + std::to_string((long long)s_g.vals[i]); return q; }()
                     : std::string("0")))
             + x86("directive", ".section .text")
             + x86("directive", ".intel_syntax noprefix"))
         + x86("def",       L(0))
         + x86("mov",       "rcx", FRQ(s_g.cursoff))
         + x86("cmp64",     "rcx", (long)s_g.n)
         + x86_omega("jge")
         + x86("lea",       "rdx", "[rip + __]", s_g.vals_ptr, s_g.lbl)
         + x86("mov",       "rsi", "[rdx + rcx*8]")
         + x86("mov",       FRQ(s_g.resoff), (long)DT_I)
         + x86("mov",       FRQ(s_g.resoff + 8), "rsi")
         + x86("inc",       FRQ(s_g.cursoff))
         + x86_gamma()
         + x86_beta()
         + x86("jmp",       L(0));
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_gather_prepare(IR_t *nd) {
    int n = (int)_.op_ival;
    IR_graph_t **subs = (IR_graph_t **)(intptr_t)_.op_counter;
    int ok = (n >= 0 && n <= GATHER_MAX_TAKES && (n == 0 || subs != NULL));
    for (int i = 0; ok && i < n; i++) {
        IR_t *lf = subs[i] ? subs[i]->entry : NULL;
        if (!lf || lf->op != IR_LIT_INTEGER) { ok = 0; break; }
        s_g.vals[i] = IR_LIT(lf).ival;
    }
    if (!ok) {
        fprintf(stderr, "[RK] FATAL bb_gather: gather requires 0..%d literal-int take payloads; n=%d\n", GATHER_MAX_TAKES, n);
        abort();
    }
    s_g.n        = n;
    s_g.vals_ptr = (uint64_t)(uintptr_t)(const void *)s_g.vals;
    snprintf(s_g.lbl, sizeof s_g.lbl, ".Lgather%d_vals", _.nid);
    (void)nd;
    fprintf(stderr, "[TE] GOUGE bb_gather_prepare: emit-time slot allocation is ERADICATED (TMP-ERADICATE) — gather awaits its Raku GZ#5 rebuild grant\n");
    abort();
}
