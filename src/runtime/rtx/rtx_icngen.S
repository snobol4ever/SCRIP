/* rtx_icngen.S -- ICON-RTX: the Icon generator-spine family. The &level counter trio.
 *
 * CONTRACT: .github/ARCH-ICON-RTX.md. Ladder: .github/GOAL-ICON-RTX.md. Ledger: .github/RTX-CLAIMS.md.
 * Gate: SCRIP_RTX_ICNGEN (eleventh family gate -- a ledger event, recorded in RTX-CLAIMS.md).
 *
 * WHY THESE THREE, AND WHY FIRST: Lon's directive is COMPLETENESS ("replace the C runtime with asm,
 * one at a time, small to large"), so the ordering key is BODY SIZE, not dynamic share. These are the
 * three smallest unported live Icon runtime bodies in the tree -- one C statement each:
 *     rt_gen_spine_pass_γ(v) { rt_k_level--; return v; }
 *     rt_gen_spine_pass_ω()  { rt_k_level--; return FAILDESCR; }
 *     rt_gen_spine_resume_enter() { rt_k_level++; }
 * rt.c's own comment calls them "strict leaves in the s22 sense -- no frame, no transfer, one counter
 * and a marshal", which is exactly the shape that ports without judgement calls.
 *
 * STEP 0, ALL SIX CHECKS MEASURED s214-ICN (not inherited -- s212's law: re-run an inherited grep):
 *   (a) live definitions: rt.c:1259 / 1261 / 1263.
 *   (b) SPELLING ROUND-TRIPS: the Greek codepoints assemble and nm back BYTE-IDENTICAL to the C
 *       object's symbols (21 bytes, cmp clean). This check is not ceremony -- the first inventory
 *       sweep this session used an ASCII-only regex and made all four Greek runtime symbols INVISIBLE,
 *       reproducing ARCH-ICON-RTX §5(ii)'s trap exactly.
 *   (c) LINKAGE, READ FROM THE CASE (s209's rule): rt_k_level is uppercase D in rt.o and lowercase d
 *       in libscrip_rt.so => link-local hidden => DIRECT [rip + rt_k_level], NO @GOTPCREL. Measured.
 *   (d) EXECUTES AND SCALES, via LD_PRELOAD interposer over a 1..N generator, N -> 4N:
 *          rt_gen_spine_resume_enter  200 -> 800  EXACTLY 4x   SCALES
 *          rt_gen_spine_pass_γ    199 -> 799  EXACTLY 4x   SCALES
 *          rt_gen_spine_pass_ω      1 ->   1  FLAT (one terminating fail per exhausted generator,
 *                                                which is the CORRECT semantics, not a cold symbol)
 *   (e) not already asm: confirmed with a UTF-8-aware sweep of every .S (the ASCII sweep lied here too).
 *   (f) all three appear as `call sym@PLT` 118 times each in a COMPILER sweep of 316 Icon programs
 *       (RULES.md: sweep the compiler, never the frozen corpus artifacts -- s213's finding).
 *
 * CANONICAL SEMANTICS (ARCH-ICON-RTX §8 step 1 -- read before porting, not after):
 *   refs/icon-master/src/runtime/invoke.r:210      k_level++   on procedure invocation
 *   refs/icon-master/src/runtime/interp.r:791/867/988  --k_level  on return / unwind
 *   refs/icon-master/src/runtime/interp.r:960     ++k_level   with the comment "adjust procedure level" <= THIS is
 *       the canonical resume-enter case that rt_gen_spine_resume_enter models.
 *   refs/icon-master/src/runtime/keyword.r:308-314  keyword{1} level => returns C_integer k_level
 *   refs/jcon-master/jcon/iKeyword.java:112         declareKey("level", level)
 * So &level is a STRICT +-1 int counter with NO latitude: the asm is a transcription, and any drift is
 * a silently wrong &level, not a slow one. Arizona inits k_level to 0 (init.r:58) where SCRIP inits to
 * 1 -- that base offset is SCRIP's own convention and is NOT touched here.
 *
 * NO SPEED CLAIM IS MADE OR IMPLIED. What is removed is the -O0 frame ceremony (push rbp / mov rbp,rsp
 * / load / add / store / pop) around a single memory increment. Per the s208 inbox's gap #1 that is
 * PRECISELY the class of win that -O2 would also remove, so it must never be quoted as an -O2-durable
 * gain. The reason to land it is Lon's completeness directive plus correctness infrastructure.
 *
 * DESCR_t (rtx_abi.inc): 16 B, v int32 @0, slen uint32 @4, value union @8. Two INTEGER eightbytes =>
 * SysV passes n=1 in rdi:rsi and returns in rax:rdx. γ is therefore a pure pass-through marshal:
 * rdi -> rax, rsi -> rdx. FAILDESCR (descr.h:43) = { .v = DT_FAIL, .i = 0 } => rax = 99, rdx = 0.
 */
#include "rtx_abi.inc"

RTX_GATE_DEF(icngen)

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_gen_spine_resume_enter(void) -- the resume hop's +1. Canonical twin: interp.r:960. */
RTX_FUNC(rt_gen_spine_resume_enter)
    RTX_GATE(icngen, c_rt_gen_spine_resume_enter)
    inc     dword ptr [rip + rt_k_level]
    ret
RTX_ENDF(rt_gen_spine_resume_enter)

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_gen_spine_pass_γ(v) -- the success return edge: take the level back down, pass the
 * descriptor through untouched. rdi:rsi is already the incoming pair, rax:rdx is the outgoing one. */
RTX_FUNC(rt_gen_spine_pass_γ)
    RTX_GATE(icngen, c_rt_gen_spine_pass_γ)
    dec     dword ptr [rip + rt_k_level]
    mov     rax, rdi
    mov     rdx, rsi
    ret
RTX_ENDF(rt_gen_spine_pass_γ)

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_gen_spine_pass_ω(void) -- the failure return edge: level back down, synthesize FAILDESCR. */
RTX_FUNC(rt_gen_spine_pass_ω)
    RTX_GATE(icngen, c_rt_gen_spine_pass_ω)
    dec     dword ptr [rip + rt_k_level]
    mov     eax, DT_FAIL
    xor     edx, edx
    ret
RTX_ENDF(rt_gen_spine_pass_ω)

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_gen_get_fb(void) -- ICN-FR-4 ZFRAME GENERATOR RESUME: returns generator_rbp from pcall[top-1].fb,
 * set by rt_jmp_frame_lexprep2 at the generator's α prologue.  The β arm does:
 *   call rt_gen_spine_resume_enter   ; k_level++
 *   call rt_gen_get_fb               ; rax = generator_rbp
 *   jmp  [rax + cont_off]            ; jump to stored β continuation in generator frame
 * No asm shortcut: reading g_pcall (a C struct pointer) requires the C body. Always delegate. */
RTX_FUNC(rt_gen_get_fb)
    RTX_GATE(icngen, c_rt_gen_get_fb)   /* RTX disabled: jmp C twin */
    jmp c_rt_gen_get_fb                 /* RTX enabled: also use C twin (no asm shortcut) */
RTX_ENDF(rt_gen_get_fb)

/* ICN-FR-4 LAYER 3: pcall-record safe-storage wrappers (rname→caller_rbp, save_Σ→cont_ptr).
 * All four delegate to C twins — the pcall array pointer and top index live in C globals. */
RTX_FUNC(rt_gen_save_caller_rbp)
    RTX_GATE(icngen, c_rt_gen_save_caller_rbp)
    jmp c_rt_gen_save_caller_rbp
RTX_ENDF(rt_gen_save_caller_rbp)

RTX_FUNC(rt_gen_get_caller_rbp)
    RTX_GATE(icngen, c_rt_gen_get_caller_rbp)
    jmp c_rt_gen_get_caller_rbp
RTX_ENDF(rt_gen_get_caller_rbp)

RTX_FUNC(rt_gen_save_cont)
    RTX_GATE(icngen, c_rt_gen_save_cont)
    jmp c_rt_gen_save_cont
RTX_ENDF(rt_gen_save_cont)

RTX_FUNC(rt_gen_get_cont)
    RTX_GATE(icngen, c_rt_gen_get_cont)
    jmp c_rt_gen_get_cont
RTX_ENDF(rt_gen_get_cont)

/* ICN-FR-4: Wire-save (one call saves both γ+ω wires; rdi=γ, rsi=ω at call site) and wire-get functions. */
RTX_FUNC(rt_gen_save_wires)
    RTX_GATE(icngen, c_rt_gen_save_wires)
    jmp c_rt_gen_save_wires
RTX_ENDF(rt_gen_save_wires)

RTX_FUNC(rt_gen_get_gamma_wire)
    RTX_GATE(icngen, c_rt_gen_get_gamma_wire)
    jmp c_rt_gen_get_gamma_wire
RTX_ENDF(rt_gen_get_gamma_wire)

RTX_FUNC(rt_gen_get_omega_wire)
    RTX_GATE(icngen, c_rt_gen_get_omega_wire)
    jmp c_rt_gen_get_omega_wire
RTX_ENDF(rt_gen_get_omega_wire)

.section .note.GNU-stack,"",@progbits
