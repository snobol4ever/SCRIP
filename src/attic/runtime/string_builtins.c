/* ATTIC: dead code excised from src/runtime/string_builtins.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*====================================================================================================================*/
DESCR_t SIZE_fn(DESCR_t s) {
    const char *STRVAL_fn = VARVAL_fn(s);
    return INTVAL((int64_t)utf8_strlen(STRVAL_fn));
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t INTGER_fn(DESCR_t v) {
    if (IS_INT(v))  return v;
    if (IS_REAL(v)) return INTVAL((int64_t)v.r);
    if (IS_STR(v)) {
        const char *s = v.s ? v.s : "";
        while (*s == ' ') s++;
        if (!*s) return NULVCL;
        char *end;
        long long iv = strtoll(s, &end, 10);
        while (*end == ' ') end++;
        if (*end) return NULVCL;
        return INTVAL((int64_t)iv);
    }
    return NULVCL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t real_fn(DESCR_t v) {
    if (IS_REAL(v)) return v;
    if (IS_INT(v))  return REALVAL((double)v.i);
    if (IS_STR(v)) {
        const char *s = v.s ? v.s : "";
        while (*s == ' ') s++;
        if (!*s) return NULVCL;
        char *end;
        double rv = strtod(s, &end);
        while (*end == ' ') end++;
        if (*end) return NULVCL;
        return REALVAL(rv);
    }
    return NULVCL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t string_fn(DESCR_t v) {
    return STRVAL(VARVAL_fn(v));
}
