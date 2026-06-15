/* ATTIC: dead code excised from src/parser/prolog/prolog_builtin.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
static void pl_write_to_file(Term *t, FILE *out) {
    t = term_deref(t);
    if (!t) { fprintf(out, "[]"); return; }
    switch (t->tag) {
        case TERM_ATOM: {
            const char *name = prolog_atom_name(t->atom_id);
            fprintf(out, "%s", name ? name : "?");
            break;
        }
        case TERM_VAR:   fprintf(out, "_G%d", t->var_slot); break;
        case TERM_INT:   fprintf(out, "%ld", t->ival); break;
        case TERM_FLOAT: {
            double fv = t->fval;
            if (fv == (long)fv && fv >= -1e15 && fv <= 1e15) fprintf(out, "%.1f", fv);
            else fprintf(out, "%g", fv);
            break;
        }
        case TERM_COMPOUND: {
            const char *fn = prolog_atom_name(t->compound.functor);
            if (!fn) fn = "?";
            if (t->compound.functor == ATOM_DOT && t->compound.arity == 2) {
                fprintf(out, "["); pl_write_to_file(t->compound.args[0], out);
                Term *tail = term_deref(t->compound.args[1]);
                while (tail && tail->tag == TERM_COMPOUND &&
                       tail->compound.functor == ATOM_DOT && tail->compound.arity == 2) {
                    fprintf(out, ","); pl_write_to_file(tail->compound.args[0], out);
                    tail = term_deref(tail->compound.args[1]);
                }
                if (!(tail && tail->tag == TERM_ATOM && tail->atom_id == ATOM_NIL))
                    { fprintf(out, "|"); pl_write_to_file(tail, out); }
                fprintf(out, "]"); break;
            }
            int my_prec = pl_op_prec(fn, t->compound.arity);
            if (my_prec >= 0) {
                if (t->compound.arity == 2) {
                    Term *la = term_deref(t->compound.args[0]), *ra = term_deref(t->compound.args[1]);
                    int lp = -1, rp = -1;
                    if (la && la->tag == TERM_COMPOUND) {
                        const char *lf = prolog_atom_name(la->compound.functor);
                        if (lf) lp = pl_op_prec(lf, la->compound.arity);
                    }
                    if (ra && ra->tag == TERM_COMPOUND) {
                        const char *rf = prolog_atom_name(ra->compound.functor);
                        if (rf) rp = pl_op_prec(rf, ra->compound.arity);
                    }
                    if (lp > my_prec) { fprintf(out, "("); pl_write_to_file(t->compound.args[0], out); fprintf(out, ")"); }
                    else pl_write_to_file(t->compound.args[0], out);
                    if (isalpha((unsigned char)fn[0])) fprintf(out, " %s ", fn); else fprintf(out, "%s", fn);
                    if (rp > my_prec) { fprintf(out, "("); pl_write_to_file(t->compound.args[1], out); fprintf(out, ")"); }
                    else pl_write_to_file(t->compound.args[1], out);
                    break;
                } else if (t->compound.arity == 1) {
                    Term *arg = term_deref(t->compound.args[0]); int ap = -1;
                    if (arg && arg->tag == TERM_COMPOUND) {
                        const char *af = prolog_atom_name(arg->compound.functor);
                        if (af) ap = pl_op_prec(af, arg->compound.arity);
                    }
                    if (isalpha((unsigned char)fn[0])) fprintf(out, "%s ", fn); else fprintf(out, "%s", fn);
                    if (ap >= my_prec) { fprintf(out, "("); pl_write_to_file(t->compound.args[0], out); fprintf(out, ")"); }
                    else pl_write_to_file(t->compound.args[0], out);
                    break;
                }
            }
            fprintf(out, "%s(", fn);
            for (int i = 0; i < t->compound.arity; i++) {
                if (i) fprintf(out, ",");
                pl_write_to_file(t->compound.args[i], out);
            }
            fprintf(out, ")"); break;
        }
        default: break;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
char *pl_term_to_string(Term *t) {
    char *buf = NULL; size_t sz = 0;
    FILE *f = open_memstream(&buf, &sz);
    if (!f) return strdup("?");
    pl_write_to_file(t, f);
    fclose(f);
    return buf;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int pl_functor(Term *t, Term *name, Term *arity, Trail *tr) {
    t     = term_deref(t);
    name  = term_deref(name);
    arity = term_deref(arity);
    if (t && t->tag != TERM_VAR) {
        Term *name_term  = NULL;
        Term *arity_term = NULL;
        switch (t->tag) {
            case TERM_ATOM:
                name_term  = term_new_atom(t->atom_id);
                arity_term = term_new_int(0);
                break;
            case TERM_INT:
                name_term  = term_new_int(t->ival);
                arity_term = term_new_int(0);
                break;
            case TERM_FLOAT:
                name_term  = term_new_float(t->fval);
                arity_term = term_new_int(0);
                break;
            case TERM_COMPOUND:
                name_term  = term_new_atom(t->compound.functor);
                arity_term = term_new_int(t->compound.arity);
                break;
            default:
                return 0;
        }
        int mark = trail_mark(tr);
        if (!unify(name, name_term, tr) || !unify(arity, arity_term, tr)) {
            trail_unwind(tr, mark);
            return 0;
        }
        return 1;
    } else {
        if (!name || name->tag == TERM_VAR) return 0;
        if (!arity || arity->tag != TERM_INT) return 0;
        long ar = arity->ival;
        Term *new_t;
        if (ar == 0) {
            if (name->tag == TERM_ATOM) new_t = term_new_atom(name->atom_id);
            else if (name->tag == TERM_INT) new_t = term_new_int(name->ival);
            else return 0;
        } else {
            if (name->tag != TERM_ATOM) return 0;
            Term **args = malloc(ar * sizeof(Term *));
            for (int i = 0; i < ar; i++) args[i] = term_new_var(i);
            new_t = term_new_compound(name->atom_id, (int)ar, args);
            free(args);
        }
        int mark = trail_mark(tr);
        if (!unify(t, new_t, tr)) {
            trail_unwind(tr, mark);
            return 0;
        }
        return 1;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int pl_arg(Term *n, Term *compound, Term *arg, Trail *tr) {
    n        = term_deref(n);
    compound = term_deref(compound);
    if (!n || n->tag != TERM_INT) return 0;
    if (!compound || compound->tag != TERM_COMPOUND) return 0;
    long idx = n->ival;
    if (idx < 1 || idx > compound->compound.arity) return 0;
    int mark = trail_mark(tr);
    if (!unify(arg, compound->compound.args[idx - 1], tr)) {
        trail_unwind(tr, mark);
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static Term *make_list(int n, Term **items) {
    Term *tail = term_new_atom(ATOM_NIL);
    for (int i = n - 1; i >= 0; i--) {
        Term *args[2] = { items[i], tail };
        tail = term_new_compound(ATOM_DOT, 2, args);
    }
    return tail;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int list_len(Term *t) {
    int n = 0;
    while (1) {
        t = term_deref(t);
        if (!t) return -1;
        if (t->tag == TERM_ATOM && t->atom_id == ATOM_NIL) return n;
        if (t->tag != TERM_COMPOUND || t->compound.functor != ATOM_DOT ||
            t->compound.arity != 2) return -1;
        n++;
        t = t->compound.args[1];
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int pl_univ(Term *t, Term *list, Trail *tr) {
    t    = term_deref(t);
    list = term_deref(list);
    if (t && t->tag != TERM_VAR) {
        Term *result;
        if (t->tag == TERM_ATOM) {
            Term *items[1] = { term_new_atom(t->atom_id) };
            result = make_list(1, items);
        } else if (t->tag == TERM_INT) {
            Term *items[1] = { term_new_int(t->ival) };
            result = make_list(1, items);
        } else if (t->tag == TERM_COMPOUND) {
            int arity = t->compound.arity;
            Term **items = malloc((arity + 1) * sizeof(Term *));
            items[0] = term_new_atom(t->compound.functor);
            for (int i = 0; i < arity; i++) items[i + 1] = t->compound.args[i];
            result = make_list(arity + 1, items);
            free(items);
        } else return 0;
        int mark = trail_mark(tr);
        if (!unify(list, result, tr)) { trail_unwind(tr, mark); return 0; }
        return 1;
    } else {
        if (!list || list->tag != TERM_COMPOUND || list->compound.functor != ATOM_DOT)
            return 0;
        int len = list_len(list);
        if (len < 1) return 0;
        Term *head_item = term_deref(list->compound.args[0]);
        if (!head_item || head_item->tag != TERM_ATOM) return 0;
        int functor_id = head_item->atom_id;
        int arity = len - 1;
        Term *new_t;
        if (arity == 0) {
            new_t = term_new_atom(functor_id);
        } else {
            Term **args = malloc(arity * sizeof(Term *));
            Term *cur = term_deref(list->compound.args[1]);
            for (int i = 0; i < arity; i++) {
                args[i] = term_deref(cur->compound.args[0]);
                cur = term_deref(cur->compound.args[1]);
            }
            new_t = term_new_compound(functor_id, arity, args);
            free(args);
        }
        int mark = trail_mark(tr);
        if (!unify(t, new_t, tr)) { trail_unwind(tr, mark); return 0; }
        return 1;
    }
}
