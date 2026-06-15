/* ATTIC: dead code excised from src/parser/raku/re.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
int        nfa_start(const Nfa *nfa) { return nfa?nfa->start:NFA_NULL; }
int        nfa_accept(const Nfa *nfa) { return nfa?nfa->accept:NFA_NULL; }
int        nfa_ngroups(const Nfa *nfa) { return nfa?nfa->ngroups:0; }
void       nfa_group_name_copy(const Nfa *nfa, int g, char *dst64) {
    if (!dst64) return; dst64[0]='\0';
    if (!nfa||g<0||g>=MAX_GROUPS) return;
    memcpy(dst64, nfa->group_name[g], 64);
}
Nfa_state *nfa_states(Nfa *nfa) { return nfa?nfa->states:0; }
