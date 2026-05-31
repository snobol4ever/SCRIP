/* bb_catch.cpp — BB template for BB_CATCH: Prolog catch/3.
   WAM-CP-10 (2026-05-28, Opus 4.7): MODE-2 owns this construct via bb_exec.c BB_CATCH executor.
   PLR-K-18 (2026-05-29): MEDIUM_BINARY arm — calls rt_pl_catch(zc_ptr) which mirrors the mode-2
   executor end-to-end (setjmp/Resolve_CatchFrame, goal_g run, longjmp recovery, rethrow on mismatch).
   Mode-4 TEXT stub deferred to WAM-CP-13.
   FACT-clean: every byte below comes from inside this function. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
/* PLR-K-18: rt_pl_catch effect helper — in-process absolute address for MEDIUM_BINARY. */
extern "C" { int rt_pl_catch(void *zc_ptr); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_catch_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — RESOLVE_CATCH");
        if (MEDIUM_BINARY) {
            /* PLR-K-18 (2026-05-29): catch/3 MEDIUM_BINARY arm.  pBB->ival holds the                  */
            /* bb_catch_state_t* sidecar (goal_g/catcher/rec_g).  Pass it as rdi; rt_pl_catch       */
            /* mirrors the mode-2 BB_CATCH executor: setjmp/Resolve_CatchFrame, runs goal_g, on          */
            /* longjmp from throw/1 restores g_resolve_env, unwinds trail, unifies catcher, runs rec_g,     */
            /* rethrows if catcher does not match.  Returns 1=γ / 0=ω.                                 */
            /* sub rsp,16 keeps rsp 16-aligned: rt_pl_catch → setjmp → glibc SSE-sensitive (cf.        */
            /* rt_pl_findall PLR-K-10 alignment SIGSEGV diagnosis).  std bin-patch tail.               */
            void *zc_ptr = (void *)(intptr_t)pBB->ival;
            std::string b;
            b += bytes(4, "\x48\x83\xEC\x10");                                           /* sub rsp, 16 */
            b += bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)zc_ptr);              /* movabs rdi, zc_ptr */
            b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_pl_catch); /* movabs rax, &rt_pl_catch */
            b += bytes(2, "\xFF\xD0");                                                    /* call rax */
            b += bytes(4, "\x48\x83\xC4\x10");                                           /* add rsp, 16 */
            b += bytes(2, "\x85\xC0");                                                    /* test eax, eax */
            /* tail: je ω | jmp γ | β: jmp ω.  The β definition is load-bearing — when a catch/3 is    */
            /* the entry node of a callee block (inner :- catch(...)) the block's redo label jmp's to  */
            /* .Lplpb%d_β; the prior {ω,γ,ω} all-false bin left it undefined → unresolved-ref abort.   */
            /* catch is semi-deterministic in this dispatch (rt_pl_catch commits to one solution), so  */
            /* the redo entry β simply fails to ω.                                                      */
            int j = (int)b.size();
            bin = { {j + 2, j + 6 + 1, j + 6 + 5, j + 6 + 5 + 1},
                    {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p},
                    {false, false, true, false} };
            return b + bytes(2, "\x0F\x84") + u32le(0)   /* je  ω */
                     + bytes(1, "\xE9")     + u32le(0)   /* jmp γ */
                     + bytes(1, "\xE9")     + u32le(0);  /* β: jmp ω */
        }
        if (MEDIUM_TEXT) {
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment("# BOX RESOLVE_CATCH (mode-4 STUB — WAM-CP-13 will implement; today fails through)")
                 + s_2asm("jmp", _.lbl_ω)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_catch(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_catch_str(pBB, bin), bin);
}
