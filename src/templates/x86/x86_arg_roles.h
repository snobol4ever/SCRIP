/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_argroles WAS `static const` here -- included by every template TU (x86_asm.h's mandated
 * encoder header, ~150 of them), so each got its OWN 6,832-byte copy of this table: 122 entries x
 * 7 pointers (callee + role[6]) = 854 R_X86_64_RELATIVE relocations PER COPY, written at every
 * process start regardless of which template that TU ever emits. 150 copies = 128,100 relocations
 * = 96.1% of the runtime's ENTIRE relocation set and 93% of .data.rel.ro (FINDING-2026-08-28-hq_P-
 * one-static-table-in-a-header-is-96-percent-of-the-runtime-relocations.md). ONE definition now
 * lives in x86_arg_roles.cpp (the only .cpp in this header-only directory, for exactly this); this
 * header only declares it, with the SAME explicit bound the definition provides, so sizeof() in
 * x86_argrole_find (x86_asm.h) keeps working in every TU without seeing the initializer -- a count
 * drift between the two is a build error (array size mismatch), never a silent one. */
typedef struct { const char * callee; const char * role[6]; } x86_argrole_t;
extern const x86_argrole_t x86_argroles[122];
