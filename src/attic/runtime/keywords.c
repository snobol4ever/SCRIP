/* ATTIC: dead code excised from src/runtime/keywords.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
int kw_assign(const char *kw, DESCR_t val) {
    if (!strcmp(kw, "pos")) {
        long n = to_int(val);
        int slen = scan_subj ? (int)strlen(scan_subj) : 0;
        long norm;
        if (n == 0)      norm = slen + 1;
        else if (n < 0)  norm = slen + 1 + n;
        else             norm = n;
        if (norm < 1 || norm > slen + 1) return 0;
        scan_pos = norm;
        return 1;
    }
    if (!strcmp(kw, "subject")) {
        const char *s = VARVAL_fn(val); if (!s) s = "";
        scan_subj = GC_strdup(s);
        scan_pos = 1;
        return 1;
    }
    if (!strcmp(kw,"error"))  { g_error  = to_int(val); return 1; }
    if (!strcmp(kw,"trace"))  { g_trace  = to_int(val); return 1; }
    if (!strcmp(kw,"dump"))   { g_dump   = to_int(val); return 1; }
    if (!strcmp(kw,"random")) {
        g_random = to_int(val);
        bb_rnd_seed = (unsigned long)g_random;
        return 1;
    }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int kw_can_assign(const char *kw, DESCR_t val) {
    if (!strcmp(kw, "pos")) {
        long n = to_int(val);
        int slen = scan_subj ? (int)strlen(scan_subj) : 0;
        long norm;
        if (n == 0)      norm = slen + 1;
        else if (n < 0)  norm = slen + 1 + n;
        else             norm = n;
        return (norm >= 1 && norm <= slen + 1) ? 1 : 0;
    }
    return 1;
}
