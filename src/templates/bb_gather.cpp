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
static int64_t  s_gather_vals[GATHER_MAX_TAKES];
static int      s_gather_n;
static uint64_t s_gather_vals_ptr;
static char     s_gather_lbl[64];
static int      s_gather_cursoff;
static int      s_gather_resoff;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int          gatherN()  { return s_gather_n; }
static inline uint64_t     valsPtr()  { return s_gather_vals_ptr; }
static inline const char * valsLbl()  { return s_gather_lbl; }
static inline int          cursoff()  { return s_gather_cursoff; }
static inline int          resoff()   { return s_gather_resoff; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_gather() {
    x86_begin();
    if (PLATFORM_X86) return x86("def",     "α")
         + x86("comment",   "IR_GATHER")
         + IF(MEDIUM_TEXT,
               x86("directive", ".section .rodata")
             + x86("directive", std::string(valsLbl()) + ": .quad "
                 + (gatherN() > 0
                     ? [&]{ std::string q; for (int i = 0; i < gatherN(); i++) q += (i ? std::string(", ") : std::string("")) + std::to_string((long long)s_gather_vals[i]); return q; }()
                     : std::string("0")))
             + x86("directive", ".section .text")
             + x86("directive", ".intel_syntax noprefix"))
         + x86("def",       L(0))
         + x86("mov",       "rcx", FRQ(cursoff()))
         + x86("cmp64",     "rcx", (long)gatherN())
         + x86("jge",       "ω")
         + x86("lea",       "rdx", "[rip + __]", valsPtr(), valsLbl())
         + x86("mov",       "rsi", "[rdx + rcx*8]")
         + x86("mov",       FRQ(resoff()), (long)6)
         + x86("mov",       FRQ(resoff() + 8), "rsi")
         + x86("inc",       FRQ(cursoff()))
         + x86("jmp",       "γ")
         + x86("def",       "β")
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
        s_gather_vals[i] = IR_LIT(lf).ival;
    }
    if (!ok) {
        fprintf(stderr, "[RK] FATAL bb_gather: gather requires 0..%d literal-int take payloads; n=%d\n", GATHER_MAX_TAKES, n);
        abort();
    }
    s_gather_n        = n;
    s_gather_vals_ptr = (uint64_t)(uintptr_t)(const void *)s_gather_vals;
    snprintf(s_gather_lbl, sizeof s_gather_lbl, ".Lgather%d_vals", _.nid);
    (void)nd;
    fprintf(stderr, "[TE] GOUGE bb_gather_prepare: emit-time slot allocation is ERADICATED (TMP-ERADICATE) — gather awaits its Raku GZ#5 rebuild grant\n");
    abort();
}
