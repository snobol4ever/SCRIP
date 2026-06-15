/* ATTIC: dead code excised from src/emitter/emit_term_build.cpp (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_op_floaty(const char *fn) {
    static const char *f[] = { "sqrt", "sin", "cos", "tan", "asin", "acos", "atan", "exp", "log",
                               "float", "float_integer_part", "float_fractional_part",
                               "truncate", "round", "ceiling", "floor", "integer", "/", NULL };
    for (int i = 0; f[i]; i++) if (!strcmp(fn, f[i])) return 1;
    return 0;
}
