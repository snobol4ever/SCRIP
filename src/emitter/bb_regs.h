#pragma once
/*====================================================================================================================================================================================================*/
/* bb_regs.h — THE locked x86-64 register convention for BB-native emission. SINGLE SOURCE the three concurrent sessions (SNOBOL4 / Icon / Prolog) reference so no box can diverge on which register
   holds what. Origin: the GOAL-*-BB FACT RULE "X86-64 REGISTER / SUBJECT-MODEL CONVENTION" (R12=ζ RATIFIED 2026-05-30), ratified 2026-05-31 (Lon) as the winner over the SMX-4-era register stories in
   REGISTER-LAYOUT.md (r12 = SM value-stack TOS; r13/r14/r15 = free) and the RULES.md ICON-STACKLESS "r13 = SM-state register" label — both obsolete because SMX-4 deleted the SM engine, so there is no SM
   value-stack and no SM state. Casing carries meaning: UPPERCASE = the fixed whole/bound; lowercase = the moving position.
       R13 = Σ  subject BASE ptr         the fixed whole string
       R14 = δ  CURSOR                   the moving scan position
       R15 = Δ  subject LENGTH / END     the fixed bound  (folds the retired Ω/Σlen — one quantity, one register)
       R12 = ζ  BB-local RW FRAME base   [r12+off]; NOT a value stack. ONE load at each BB-BLOB sequence BEGIN (amortized across its boxes; switches per sequence); survives C calls (callee-saved).
       R10 =    per-BLOB DATA-block ptr  lea r10,[rip+data]; constant inside one BLOB; caller-saved (FLAT model). NOTE: the BROKERED model (RULES.md) uses r10 as the broker's CURRENT-NODE ptr — fork TBD.
       rbx =    DESCR BASE POINTER for the dual-width DESCR mode (Lon 2026-05-31): 8-byte DESCR (32-bit) / 16-byte DESCR (64-bit); a concurrent session is doing the 32-bit conversion.
       rbp =    variable NAME/HASH-table base — RESERVED. GET/SET are C CALLS (NV_GET_fn/NV_SET_fn) FOR NOW; inlining as [rbp+slot] x86 asm is a future optimization. NB rbp/r13 can't zero-disp: [rbp]->[rbp+0].
   γ-success packing: rax = σ ptr (transient Σ+δ), rdx = δ int (spec_t). Changing this convention is LOCKSTEP: update this header, the 3 GOAL files' FACT RULE table, and REGISTER-LAYOUT.md in one commit.        */
/*====================================================================================================================================================================================================*/
/* TEXT-arm (GAS) register-name operands — boxes write BBREG_* not literal "r12"/"r13", so the convention lives in exactly one place. */
#define BBREG_ZETA      "r12"
#define BBREG_SIGMA     "r13"
#define BBREG_CURSOR    "r14"
#define BBREG_LIMIT     "r15"
#define BBREG_DATA      "r10"
#define BBREG_BASE      "rbx"
#define BBREG_HASH      "rbp"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* BINARY-arm raw x86 register numbers (ModRM.reg field / opcode+r selector / REX.B,.R extension bit). r12-r15 set REX.B/.R; rbx=3 and r10 (REX.B + index 2). */
#define BBREGN_ZETA     12
#define BBREGN_SIGMA    13
#define BBREGN_CURSOR   14
#define BBREGN_LIMIT    15
#define BBREGN_DATA     10
#define BBREGN_BASE     3
#define BBREGN_HASH     5
