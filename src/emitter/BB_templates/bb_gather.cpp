#include <string>
#include <cstring>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
int  bb_slot_claim(int bytes);
int  bb_slot_alloc16(IR_t * nd);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
#define GATHER_MAX_TAKES 256
static int64_t  s_gather_vals[GATHER_MAX_TAKES];
static int      s_gather_n;
static uint64_t s_gather_vals_ptr;
static char     s_gather_lbl[64];
static int      s_gather_cursoff;
static int      s_gather_resoff;
/*--------------------------------------------------------------------------------------------------------------------*/
static inline int          gatherN() { return s_gather_n; }
static inline uint64_t     valsPtr() { return s_gather_vals_ptr; }
static inline const char * valsLbl() { return s_gather_lbl; }
static inline int          cursoff() { return s_gather_cursoff; }
static inline int          resoff()  { return s_gather_resoff; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string gather_vals_rodata() {
    std::string q = std::string(valsLbl()) + ":";
    if (gatherN() > 0) {
        q += " .quad ";
        for (int i = 0; i < gatherN(); i++) q += emit_fmt("%s%lld", (i ? ", " : ""), (long long)s_gather_vals[i]);
    } else {
        q += " .quad 0";
    }
    return s_directive(".section .rodata") + s_directive(q)
         + s_directive(".section .text") + s_directive(".intel_syntax noprefix");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_gather_str() {
    if (!PLATFORM_X86) return std::string();
    return IF(MEDIUM_TEXT,
               s_1asm(std::string(_.lbl_α) + ":")
             + s_comment(emit_fmt("# BOX IR_GATHER n=%d [x86() self-encoding, ζ-frame cursor]", gatherN()))
             + gather_vals_rodata())
           + x86("def",    L(0))
           + x86("mov",    "rcx", FRQ(cursoff()))
           + x86("cmp64",  "rcx", (long)gatherN())
           + x86("jge",    PORT_OMEGA)
           + x86("lea",    "rdx", "[rip + __]", valsPtr(), valsLbl())
           + x86("mov",    "rsi", "rdx", "rcx")
           + x86("mov",    FRQ(resoff()), (long)6)
           + x86("mov",    FRQ(resoff() + 8), "rsi")
           + x86("inc",    FRQ(cursoff()))
           + x86("jmp",    PORT_GAMMA)
           + x86("def",    PORT_BETA)
           + x86("jmp",    L(0));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_gather(IR_t * pBB) {
    if (!PLATFORM_X86) { return; }
    int n = (int)_.op_ival;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t)pBB->counter;
    int ok = (n >= 0 && n <= GATHER_MAX_TAKES && (n == 0 || subs != NULL));
    for (int i = 0; ok && i < n; i++) {
        IR_t * lf = subs[i] ? subs[i]->entry : NULL;
        if (!lf || lf->t != IR_LIT_I) { ok = 0; break; }
        s_gather_vals[i] = lf->ival;
    }
    if (!ok) {
        fprintf(stderr, "[RK] FATAL bb_gather: gather requires 0..%d literal-int take payloads (FLAT-take model); n=%d\n",
                GATHER_MAX_TAKES, n);
        abort();
    }
    s_gather_n        = n;
    s_gather_vals_ptr = (uint64_t)(uintptr_t)(const void *)s_gather_vals;
    snprintf(s_gather_lbl, sizeof(s_gather_lbl), ".Lgather%d_vals", _.nid);
    x86_begin();
    s_gather_resoff  = bb_slot_alloc16(pBB);
    s_gather_cursoff = bb_slot_claim(8);
    bb_emit_x86(bb_gather_str());
}
