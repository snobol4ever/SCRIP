static char icn_str_arena[65536];
static int  str_arena_pos = 0;
#define ICN_STACK_MAX 256
static long icn_stack[ICN_STACK_MAX];
static int  icn_sp = 0;
long icn_retval = 0;
int  icn_failed = 0;
static char subscript_buf[2];
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *cset_union(const char *a, const char *b) {
    if (!a) a = ""; if (!b) b = "";
    if (str_arena_pos + 256 > 65536) str_arena_pos = 0;
    char *out = icn_str_arena + str_arena_pos;
    int n = 0;
    for (int i = 0; a[i]; i++) out[n++] = a[i];
    for (int j = 0; b[j]; j++) {
        int found = 0;
        for (int i = 0; a[i]; i++) { if (a[i] == b[j]) { found = 1; break; } }
        if (!found) out[n++] = b[j];
    }
    out[n] = '\0';
    str_arena_pos += n + 1;
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *cset_diff(const char *a, const char *b) {
    if (!a) a = ""; if (!b) b = "";
    if (str_arena_pos + 256 > 65536) str_arena_pos = 0;
    char *out = icn_str_arena + str_arena_pos;
    int n = 0;
    for (int i = 0; a[i]; i++) {
        int found = 0;
        for (int j = 0; b[j]; j++) { if (a[i] == b[j]) { found = 1; break; } }
        if (!found) out[n++] = a[i];
    }
    out[n] = '\0';
    str_arena_pos += n + 1;
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *cset_inter(const char *a, const char *b) {
    if (!a) a = ""; if (!b) b = "";
    if (str_arena_pos + 256 > 65536) str_arena_pos = 0;
    char *out = icn_str_arena + str_arena_pos;
    int n = 0;
    for (int i = 0; a[i]; i++) {
        for (int j = 0; b[j]; j++) { if (a[i] == b[j]) { out[n++] = a[i]; break; } }
    }
    out[n] = '\0';
    str_arena_pos += n + 1;
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *cset_canonical(const char *cs) {
    if (!cs || !*cs) return "";
    unsigned char present[256] = {0};
    for (const unsigned char *p = (const unsigned char *)cs; *p; p++) present[*p] = 1;
    int n = 0;
    for (int c = 0; c < 256; c++) if (present[c]) n++;
    if (str_arena_pos + n + 1 > 65536) str_arena_pos = 0;
    char *out = icn_str_arena + str_arena_pos;
    int bi = 0;
    for (int c = 0; c < 256; c++) if (present[c]) out[bi++] = (char)c;
    out[bi] = '\0';
    str_arena_pos += bi + 1;
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* icn-cset-embedded-nul-four-layer-gap, a fifth/newly-discovered layer found while proving the
   original four-layer fix end to end: bb_scan_{any,many,bal,upto}.cpp and bb_gcc.cpp all test cset
   membership via a per-subject-byte strchr(needle, ch) call. strchr treats ch==0 as "find the
   needle's own NUL terminator", which ALWAYS returns non-NULL -- so any subject byte that happens to
   be \x00 is reported as a member of EVERY needle cset, regardless of its actual content (confirmed:
   corpus/tests/icon/rung38_cset_embedded_nul.icn's `skips ? upto('a')` matched at the leading \x00
   instead of the real 'a'). memchr has no such special case; resolving the needle's TRUE length here
   (same kw_cset_len-then-strlen fallback as rt_scan_enter/c_rt_size_d) and bounding the search by it
   fixes both an embedded-NUL SUBJECT (this bug) and an embedded-NUL NEEDLE, for the same reason.
   Same call-site shape as strchr (rdi=needle, esi=ch as int) so every caller's null/non-null result
   check (test+je) is unchanged -- only the call target and this function differ. */
int rt_icn_cset_member(const char *needle, int ch) {
    if (!needle) return 0;
    extern int kw_cset_len(const char *);
    int kn = kw_cset_len(needle);
    int len = (kn >= 0) ? kn : (int) strlen(needle);
    return memchr(needle, (unsigned char)ch, (unsigned long)len) != 0;
}
