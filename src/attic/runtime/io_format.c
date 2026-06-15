/* ATTIC: dead code excised from src/runtime/io_format.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_str_nl(const char *s, uint32_t slen)
{
    if (s && slen) fwrite(s, 1, slen, stdout);
    fputc('\n', stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_float(double v)
{
    char fb[64]; rt_format_float(fb, sizeof fb, v); fputs(fb, stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_cstr(const char *s)
{
    if (s) fputs(s, stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_var(int slot)
{
    extern Term **g_resolve_env;
    if (!g_resolve_env || slot < 0) { fputs("_", stdout); return; }
    Term *t = g_resolve_env[slot] ? term_deref(g_resolve_env[slot]) : NULL;
    if (!t) { fputs("_", stdout); return; }
    if (t->tag == TERM_INT)   { printf("%ld", (long)t->ival); return; }
    if (t->tag == TERM_FLOAT) { char fb[64]; rt_format_float(fb, sizeof fb, t->fval); fputs(fb, stdout); return; }
    if (t->tag == TERM_ATOM)  { const char *nm = prolog_atom_name(t->atom_id);
                                 if (nm) fputs(nm, stdout); return; }
    if (t->tag == TERM_COMPOUND) { extern void pl_write(Term *); pl_write(t); return; }
    fputs("_", stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_term_ptr(void *t)
{
    extern void pl_write(Term *);
    Term *d = t ? term_deref((Term *)t) : NULL;
    if (!d) { fputs("_", stdout); return; }
    pl_write(d);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_writeq_term_ptr(void *t)
{
    extern void pl_writeq(Term *);
    Term *d = t ? term_deref((Term *)t) : NULL;
    if (!d) { fputs("_", stdout); return; }
    pl_writeq(d);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_canonical_term_ptr(void *t)
{
    extern void pl_write_canonical(Term *);
    Term *d = t ? term_deref((Term *)t) : NULL;
    if (!d) { fputs("_", stdout); return; }
    pl_write_canonical(d);
}
