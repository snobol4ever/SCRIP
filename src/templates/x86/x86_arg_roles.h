/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_argroles: the call-argument ROLE annotation table (consumed only by x86_argnote in x86_asm.h,
 * which decorates emitted TEXT with `# role` comments). Two representation facts matter here:
 * (1) it WAS `static const` IN THIS HEADER, so each of the ~150 template TUs including x86_asm.h got its
 *     own 6,832-byte copy = 96.1% of the runtime's entire relocation set. Collapsed to ONE definition in
 *     x86_arg_roles.cpp on 2026-08-28 (FINDING-2026-08-28-hq_P-one-static-table-in-a-header-...).
 * (2) the surviving copy still cost 861 relocations -- one per `const char *`, since a pointer to a string
 *     literal is not a link-time constant in PIC and must be written at load time, dirtying its pages and
 *     pinning the table in .data.rel.ro. The fields are now 16-bit OFFSETS into x86_argrole_blob, which
 *     makes them genuine compile-time constants: table and blob are both .rodata, zero relocations, never
 *     written, shared between processes. Read a field with x86_argrole_str().
 * Offset 0 is the empty string, so an unused role slot needs no NULL test -- only a [0] emptiness test. */
typedef struct { unsigned short callee; unsigned short role[6]; } x86_argrole_t;
extern const char x86_argrole_blob[];
extern const x86_argrole_t x86_argroles[123];
static inline const char * x86_argrole_str(unsigned short off) { return x86_argrole_blob + off; }
