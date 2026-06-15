/* ATTIC: dead code excised from src/parser/icon/icon_runtime.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
const char *cset_complement(const char *cs) {
    if (!cs) cs = "";
    if (str_arena_pos + 128 > 65536) str_arena_pos = 0;
    char *out = icn_str_arena + str_arena_pos;
    int n = 0;
    for (int c = 1; c < 128; c++) {
        int found = 0;
        for (int i = 0; cs[i]; i++) { if ((unsigned char)cs[i] == (unsigned)c) { found = 1; break; } }
        if (!found) out[n++] = (char)c;
    }
    out[n] = '\0';
    str_arena_pos += n + 1;
    return out;
}
