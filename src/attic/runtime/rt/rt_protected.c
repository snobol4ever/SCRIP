/* ATTIC: dead code excised from src/runtime/rt/rt_protected.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
int protected_pat_name_to_sm_op(const char *name)
{
    if (!name) return -1;
    for (int i = 0; g_protected_pat_op[i].name; i++) {
        if (strcmp(name, g_protected_pat_op[i].name) == 0)
            return (int)g_protected_pat_op[i].op;
    }
    return -1;
}
