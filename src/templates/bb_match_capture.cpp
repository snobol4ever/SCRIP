#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" long rt_cap_open(const char *varname, int saved_delta, int cur_delta, int is_imm);
extern "C" void *rt_frame_prep(void *fb, long fbytes);
extern "C" void rt_cap_finish(DESCR_t fret);
extern "C" void rt_cap_push(void *slot, int delta);
extern "C" void rt_cap_pop(void *slot);
extern "C" int rt_cap_top(void *slot);
extern "C" const char *g_dcap_top;
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZB-FC-3c (ARCH-ZETA S13 Tier C, CAPTURES; plan of record = the s47 COND-CROSS-BOX-READ finding): when
 * LOWER registered the capture (linear inner, no ARBNO either direction), SAVE owns a 16-byte rsp cell --
 * alpha stores delta at cell+0 (the hook's sub rsp,16 has already run at the alpha define), beta emits
 * NOTHING (omega's hook pop is the release, S10b FORTH law), the rt_cap software array never called -- and
 * COND/IMM read that delta CROSS-BOX at [rsp + fp(inner)]: by S10c (cells pop at omega not gamma) the whole
 * inner subtree is still suspended at the yield, so the displacement is a compile-time constant, exactly
 * ALTERNATE's rspd(op_fc_fpmax+4) mechanism.  COND/IMM are ZERO-CELL (no hook motion); their read sits
 * OUTSIDE any align/anchor window (align_enter pushes 16 and masks rsp -- an rspd read inside it would be
 * wrong; eax survives the window's pushes into the later mov esi,eax).  sfc/cfc off = the flat rt_cap array
 * path VERBATIM (degrade never die); the pend/rbp-dcap machinery is identical on both paths (F2: by-value). */
static inline int  sfc()      { return x86_fc_on(); }
static inline int  cfc()      { return x86_port_mode() == ZC_PORT_FORTH && _.op_fc_disp >= 0; }
static inline const char * rspd(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "dword ptr [rsp + %d]", off); return b[i]; }
std::string bb_match_capture() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    return (_.op_off < 0)
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_ASSIGN: capture stack slot not promoted (flat_drive_capture)") )
         : !(_.op_sval ? _.op_sval : "")[0]
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_ASSIGN: empty capture variable name") )
         : (int)_.op_phase == 0 && sfc()
         ? ( x86("comment", "IR_MATCH_CAPTURE_SAVE fc cell")
           + x86_alpha()
           + x86("mov",  rspd(0), "r14d")
           + x86_gamma()
           + x86_beta()
           + x86_omega() )
         : (int)_.op_phase == 0
         ? ( x86("comment", "IR_MATCH_CAPTURE_SAVE push")
           + x86_alpha()
           + x86("lea",  "rdi", FR(_.op_off))
           + x86("mov",  "esi", "r14d")
           + x86_align_enter()
           + x86("call", "rt_cap_push", (uint64_t)(uintptr_t)(void *)(void (*)(void *, int))rt_cap_push)
           + x86_align_leave()
           + x86_gamma()
           + x86_beta()
           + x86("lea",  "rdi", FR(_.op_off))
           + x86_align_enter()
           + x86("call", "rt_cap_pop", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_cap_pop)
           + x86_align_leave()
           + x86_omega() )
         : (int)_.op_phase == 1
         /* rbp-dcap COND (Lon 2026-07-13: "should not call a C function when all it must do is increment RBP
          * and decrement RBP").  γ: write the 24B pend entry {varname@0, saved_delta@8, len@16} at [rbp] and
          * bump — three qword stores, no packing (no shl/or encoders needed: 32-bit reg movs zero-extend).
          * β: sub rbp,24, UNGUARDED — sound because within-alternative failure cascades transit the boxes
          * (balanced, the Python LIFO theorem) and the alternative-SWITCH bypass is bulk-restored by
          * bb_match_alternate's own rbp mark (SZ-2c gap, ported inline).  The residual rt_cap_top call is the
          * SAVE-stack array read — ZB-FC-3c's named kill, NOT this rung's (F3, CAPTURE-SPINE finding). */
         ? ( x86("comment", "IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)")
           + x86_alpha()
           + IF(cfc(),  x86("mov", "eax", rspd((int)_.op_fc_disp)))
           + IF(!cfc(), x86_align_enter()
                      + x86("lea",  "rdi", FR(_.op_off))
                      + x86("call", "rt_cap_top", (uint64_t)(uintptr_t)(void *)(int (*)(void *))rt_cap_top)
                      + x86_align_leave())
           + x86("lea",  "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), (strtab_label(b, sizeof b, (_.op_sval ? _.op_sval : "")), b))
           + x86("mov",  RDQ("rbp", 0), "rcx")
           + x86("mov",  "esi", "eax")
           + x86("mov",  RDQ("rbp", 8), "rsi")
           + x86("mov",  "edx", "r14d")
           + x86("sub",  "edx", "eax")
           + x86("mov",  RDQ("rbp", 16), "rdx")
           + x86("add",  "rbp", (long)24)
           + x86_gamma()
           + x86_beta()
           + x86("sub",  "rbp", (long)24)
           + x86_omega() )
         : ( x86("comment", "IR_MATCH_CAPTURE_IMM")
           + x86_alpha()
           + IF(cfc(),  x86("mov", "eax", rspd((int)_.op_fc_disp)))
           + x86_anchor_enter()
           + IF(!cfc(), x86("lea",  "rdi", FR(_.op_off))
                      + x86("call", "rt_cap_top", (uint64_t)(uintptr_t)(void *)(int (*)(void *))rt_cap_top))
           + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), (strtab_label(b, sizeof b, (_.op_sval ? _.op_sval : "")), b))
           + x86("mov",  "esi", "eax")
           + x86("mov",  "edx", "r14d")
           + x86("mov",  "ecx", (long)1)
           + x86("call", "rt_cap_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, int, int, int))rt_cap_open)
           + x86("test", "rax", "rax")
           + x86("je",   L(1))
           /* rbp-dcap mirror-out: this *VAR transfer may run a nested match whose head loads g_dcap_top; a
            * stale-low mirror would let its pends overwrite live entries above.  Match-family boxes only —
            * never the shared x86_frame_sink (non-SNOBOL graphs carry a non-cursor rbp). */
           + x86("mov",  "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_dcap_top, "g_dcap_top")
           + x86("mov",  RDQ("rcx", 0), "rbp")
           + x86_frame_sink()
           + x86_frame_base("rdi")
           + x86("mov",  "rsi", "rax")
           + x86("call", "rt_frame_prep", (uint64_t)(uintptr_t)(void *)(void *(*)(void *, long))rt_frame_prep)
           + x86_frame_base("rdi")
           + x86("xor", "esi", "esi")
           + x86("call", "rax")
           + x86("mov",  "rdi", "rax")
           + x86("mov",  "rsi", "rdx")
           + x86_frame_unsink()
           + x86("call", "rt_cap_finish", (uint64_t)(uintptr_t)(void *)(void (*)(DESCR_t))rt_cap_finish)
           + x86("def",  L(1))
           + x86_anchor_leave()
           + x86_gamma()
           + x86_beta()
           + x86_omega() );
}
