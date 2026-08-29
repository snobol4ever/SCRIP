#include "prolog_builtin.h"
#include "prolog_atom.h"
#include "prolog_runtime.h"
#include "term.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
static FILE *pl_wr_fp = (FILE *)0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_wr_set_fp(FILE *f) { pl_wr_fp = f; }
static FILE *plw(void) { return pl_wr_fp ? pl_wr_fp : stdout; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_op_prec(const char *name, int arity) {
    struct { const char *n; int a; int p; } tbl[] = {
        {":-",2,1200},{";",2,1100},{"->",2,1050},{",",2,1000},
        {"=",2,700},{"\\=",2,700},{"is",2,700},{"=:=",2,700},{"=\\=",2,700},
        {"<",2,700},{">",2,700},{"=<",2,700},{">=",2,700},
        {"==",2,700},{"\\==",2,700},{"@<",2,700},{"@>",2,700},
        {"@=<",2,700},{"@>=",2,700},{"=..",2,700},
        {"+",2,500},{"-",2,500},
        {"*",2,400},{"/",2,400},{"//",2,400},{"mod",2,400},
        {"rem",2,400},{"<<",2,400},{">>",2,400},
        {"**",2,200},{"^",2,200},
        {"-",1,200},{"\\+",1,900},{"not",1,900},
        {NULL,0,0}
    };
    for (int i = 0; tbl[i].n; i++)
        if (tbl[i].a == arity && strcmp(tbl[i].n, name) == 0) return tbl[i].p;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_write(Term *t) {
    t = term_deref(t);
    if (!t) { fprintf(plw(), "[]"); return; }
    switch (t->tag) {
        case TERM_ATOM: {
            const char *name = prolog_atom_name(t->atom_id);
            if (!name) name = "?";
            int needs_quote = 0;
            if (name[0] && (isupper((unsigned char)name[0]) || name[0] == '_'))
                needs_quote = 1;
            for (const char *p = name; *p && !needs_quote; p++)
                if (!isalnum((unsigned char)*p) && *p != '_')
                    needs_quote = 1;
            if (needs_quote && name[0] != '[') {
            }
            fprintf(plw(), "%s", name);
            break;
        }
        case TERM_VAR:
            fprintf(plw(), "_G%d", t->var_slot);
            break;
        case TERM_INT:
            fprintf(plw(), "%ld", t->ival);
            break;
        case TERM_FLOAT: {
            double fv = t->fval;
            char fb[64];
            for (int prec = 15; prec <= 17; prec++) {
                snprintf(fb, sizeof fb, "%.*g", prec, fv);
                if (strtod(fb, NULL) == fv) break;
            }
            if (!strpbrk(fb, ".eEnN")) { size_t n = strlen(fb); if (n+2 < sizeof fb) { fb[n]='.'; fb[n+1]='0'; fb[n+2]='\0'; } }
            fputs(fb, plw());
            break;
        }
        case TERM_COMPOUND: {
            const char *fn = prolog_atom_name(t->compound.functor);
            if (!fn) fn = "?";
            if (strcmp(fn, "$VAR") == 0 && t->compound.arity == 1) {
                Term *n = term_deref(t->compound.args[0]);
                if (n && n->tag == TERM_INT) {
                    long num = n->ival;
                    int letter = (int)(num % 26);
                    long suffix = num / 26;
                    if (suffix == 0) fprintf(plw(), "%c", 'A' + letter);
                    else            fprintf(plw(), "%c%ld", 'A' + letter, suffix);
                    break;
                }
            }
            if (strcmp(fn, "$VARNAME") == 0 && t->compound.arity == 1) {
                Term *n = term_deref(t->compound.args[0]);
                if (n && n->tag == TERM_ATOM) {
                    const char *vn = prolog_atom_name(n->atom_id);
                    fprintf(plw(), "%s", vn ? vn : "_");
                    break;
                }
            }
            if (t->compound.functor == ATOM_DOT && t->compound.arity == 2) {
                fprintf(plw(), "[");
                pl_write(t->compound.args[0]);
                Term *tail = term_deref(t->compound.args[1]);
                while (tail && tail->tag == TERM_COMPOUND &&
                       tail->compound.functor == ATOM_DOT &&
                       tail->compound.arity == 2) {
                    fprintf(plw(), ",");
                    pl_write(tail->compound.args[0]);
                    tail = term_deref(tail->compound.args[1]);
                }
                if (tail && tail->tag == TERM_ATOM && tail->atom_id == ATOM_NIL) {
                } else {
                    fprintf(plw(), "|");
                    pl_write(tail);
                }
                fprintf(plw(), "]");
                break;
            }
            if (t->compound.arity == 1 && fn && strcmp(fn, "{}") == 0) {
                fprintf(plw(), "{"); pl_write(t->compound.args[0]); fprintf(plw(), "}");
                break;
            }
            struct { const char *name; int arity; int prec; int right_assoc; } ops[] = {
                {":-",2,1200,1}, {";",2,1100,1}, {"->",2,1050,1},
                {",",2,1000,1},
                {"=",2,700,0},{"\\=",2,700,0},{"is",2,700,0},
                {"=:=",2,700,0},{"=\\=",2,700,0},
                {"<",2,700,0},{">",2,700,0},{"=<",2,700,0},{">=",2,700,0},
                {"==",2,700,0},{"\\==",2,700,0},
                {"@<",2,700,0},{"@>",2,700,0},{"@=<",2,700,0},{"@>=",2,700,0},
                {"=..",2,700,0},
                {"+",2,500,0},{"-",2,500,0},
                {"*",2,400,0},{"/",2,400,0},{"//",2,400,0},{"mod",2,400,0},
                {"rem",2,400,0},{"<<",2,400,0},{">>",2,400,0},
                {"**",2,200,1},{"^",2,200,1},
                {"-",1,200,0},{"\\+",1,900,0},{"not",1,900,0},
                {NULL,0,0,0}
            };
            int is_op = 0;
            for (int i = 0; ops[i].name; i++) {
                if (strcmp(fn, ops[i].name) == 0 && t->compound.arity == ops[i].arity) {
                    is_op = 1;
                    if (ops[i].arity == 2) {
                        Term *larg = term_deref(t->compound.args[0]);
                        Term *rarg = term_deref(t->compound.args[1]);
                        int lp = -1, rp = -1;
                        if (larg && larg->tag == TERM_COMPOUND) {
                            const char *lfn = prolog_atom_name(larg->compound.functor);
                            if (lfn) lp = pl_op_prec(lfn, larg->compound.arity);
                        }
                        if (rarg && rarg->tag == TERM_COMPOUND) {
                            const char *rfn = prolog_atom_name(rarg->compound.functor);
                            if (rfn) rp = pl_op_prec(rfn, rarg->compound.arity);
                        }
                        int my_prec = ops[i].prec;
                        int lneed = (lp > my_prec) || (lp == my_prec && ops[i].right_assoc);
                        int rneed = (rp > my_prec) || (rp == my_prec && !ops[i].right_assoc);
                        if (lneed) fprintf(plw(), "(");
                        pl_write(t->compound.args[0]);
                        if (lneed) fprintf(plw(), ")");
                        if (isalpha((unsigned char)fn[0])) fprintf(plw(), " %s ", fn);
                        else fprintf(plw(), "%s", fn);
                        if (rneed) fprintf(plw(), "(");
                        pl_write(t->compound.args[1]);
                        if (rneed) fprintf(plw(), ")");
                    } else {
                        Term *arg = term_deref(t->compound.args[0]);
                        int ap = -1;
                        if (arg && arg->tag == TERM_COMPOUND) {
                            const char *afn = prolog_atom_name(arg->compound.functor);
                            if (afn) ap = pl_op_prec(afn, arg->compound.arity);
                        }
                        int aneed = (ap >= ops[i].prec);
                        if (isalpha((unsigned char)fn[0])) fprintf(plw(), "%s ", fn);
                        else fprintf(plw(), "%s", fn);
                        if (aneed) fprintf(plw(), "(");
                        pl_write(t->compound.args[0]);
                        if (aneed) fprintf(plw(), ")");
                    }
                    break;
                }
            }
            if (!is_op) {
                fprintf(plw(), "%s(", fn);
                for (int i = 0; i < t->compound.arity; i++) {
                    if (i) fprintf(plw(), ",");
                    pl_write(t->compound.args[i]);
                }
                fprintf(plw(), ")");
            }
            break;
        }
        case TERM_REF:
            pl_write(t->ref);
            break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int atom_needs_quoting(const char *name) {
    if (!name || !name[0]) return 1;
    if (name[0] == '[' && strcmp(name,"[]")==0) return 0;
    if (name[0] == '{' && strcmp(name,"{}")==0) return 0;
    if (isupper((unsigned char)name[0]) || name[0] == '_') return 1;
    int all_graphic = 1;
    static const char *graphic = "#&*+-./:<=>?@\\^~";
    for (const char *p = name; *p; p++)
        if (!strchr(graphic, *p)) { all_graphic = 0; break; }
    if (all_graphic) return 0;
    if (islower((unsigned char)name[0])) {
        for (const char *p = name+1; *p; p++)
            if (!isalnum((unsigned char)*p) && *p != '_') return 1;
        return 0;
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_writeq_term(Term *t) {
    t = term_deref(t);
    if (!t) { fprintf(plw(), "'[]'"); return; }
    switch (t->tag) {
        case TERM_ATOM: {
            const char *name = prolog_atom_name(t->atom_id);
            if (!name) name = "?";
            if (atom_needs_quoting(name)) {
                fputc('\'', plw());
                for (const char *p = name; *p; p++) {
                    if (*p == '\'') fputc('\'', plw());
                    fputc(*p, plw());
                }
                fputc('\'', plw());
            } else {
                fprintf(plw(), "%s", name);
            }
            break;
        }
        case TERM_VAR:
            fprintf(plw(), "_G%d", t->var_slot);
            break;
        case TERM_INT:
            fprintf(plw(), "%ld", t->ival);
            break;
        case TERM_FLOAT: {
            double fv = t->fval;
            if (fv == (long)fv && fv >= -1e15 && fv <= 1e15) fprintf(plw(), "%.1f", fv);
            else fprintf(plw(), "%g", fv);
            break;
        }
        case TERM_COMPOUND: {
            const char *fn = prolog_atom_name(t->compound.functor);
            if (!fn) fn = "?";
            if (strcmp(fn,"$VAR")==0 && t->compound.arity==1) {
                Term *n = term_deref(t->compound.args[0]);
                if (n && n->tag == TERM_INT) {
                    long num = n->ival; int letter=(int)(num%26); long suf=num/26;
                    if (suf==0) fprintf(plw(), "%c",'A'+letter); else fprintf(plw(), "%c%ld",'A'+letter,suf);
                    break;
                }
            }
            if (strcmp(fn,"$VARNAME")==0 && t->compound.arity==1) {
                Term *n = term_deref(t->compound.args[0]);
                if (n && n->tag == TERM_ATOM) {
                    const char *vn = prolog_atom_name(n->atom_id);
                    fprintf(plw(), "%s", vn ? vn : "_");
                    break;
                }
            }
            if (t->compound.functor == ATOM_DOT && t->compound.arity == 2) {
                fprintf(plw(), "["); pl_writeq_term(t->compound.args[0]);
                Term *tail = term_deref(t->compound.args[1]);
                while (tail && tail->tag==TERM_COMPOUND && tail->compound.functor==ATOM_DOT && tail->compound.arity==2) {
                    fprintf(plw(), ","); pl_writeq_term(tail->compound.args[0]);
                    tail = term_deref(tail->compound.args[1]);
                }
                if (!(tail && tail->tag==TERM_ATOM && tail->atom_id==ATOM_NIL)) { fprintf(plw(), "|"); pl_writeq_term(tail); }
                fprintf(plw(), "]"); break;
            }
            if (t->compound.arity == 1 && strcmp(fn, "{}") == 0) {
                fprintf(plw(), "{"); pl_writeq_term(t->compound.args[0]); fprintf(plw(), "}"); break;
            }
            struct { const char *name; int arity; int prec; int right_assoc; } ops[] = {
                {":-",2,1200,1},{";",2,1100,1},{"->",2,1050,1},{",",2,1000,1},
                {"=",2,700,0},{"\\=",2,700,0},{"is",2,700,0},
                {"=:=",2,700,0},{"=\\=",2,700,0},
                {"<",2,700,0},{">",2,700,0},{"=<",2,700,0},{">=",2,700,0},
                {"==",2,700,0},{"\\==",2,700,0},
                {"@<",2,700,0},{"@>",2,700,0},{"@=<",2,700,0},{"@>=",2,700,0},
                {"=..",2,700,0},{"+",2,500,0},{"-",2,500,0},
                {"*",2,400,0},{"/",2,400,0},{"//",2,400,0},{"mod",2,400,0},
                {"rem",2,400,0},{"<<",2,400,0},{">>",2,400,0},
                {"**",2,200,1},{"^",2,200,1},
                {"-",1,200,0},{"\\+",1,900,0},{"not",1,900,0},
                {NULL,0,0,0}
            };
            int is_op = 0;
            for (int i = 0; ops[i].name; i++) {
                if (strcmp(fn,ops[i].name)==0 && t->compound.arity==ops[i].arity) {
                    is_op = 1;
                    if (ops[i].arity==2) {
                        Term *la=term_deref(t->compound.args[0]),*ra=term_deref(t->compound.args[1]);
                        int lp=-1,rp=-1;
                        if(la&&la->tag==TERM_COMPOUND){const char*lf=prolog_atom_name(la->compound.functor);if(lf)lp=pl_op_prec(lf,la->compound.arity);}
                        if(ra&&ra->tag==TERM_COMPOUND){const char*rf=prolog_atom_name(ra->compound.functor);if(rf)rp=pl_op_prec(rf,ra->compound.arity);}
                        int my=ops[i].prec;
                        if((lp>my)||(lp==my&&ops[i].right_assoc)) { fprintf(plw(), "("); pl_writeq_term(t->compound.args[0]); fprintf(plw(), ")"); }
                        else pl_writeq_term(t->compound.args[0]);
                        if(isalpha((unsigned char)fn[0])) fprintf(plw(), " %s ",fn); else fprintf(plw(), "%s",fn);
                        if((rp>my)||(rp==my&&!ops[i].right_assoc)) { fprintf(plw(), "("); pl_writeq_term(t->compound.args[1]); fprintf(plw(), ")"); }
                        else pl_writeq_term(t->compound.args[1]);
                    } else {
                        Term *arg=term_deref(t->compound.args[0]); int ap=-1;
                        if(arg&&arg->tag==TERM_COMPOUND){const char*af=prolog_atom_name(arg->compound.functor);if(af)ap=pl_op_prec(af,arg->compound.arity);}
                        if(isalpha((unsigned char)fn[0])) fprintf(plw(), "%s ",fn); else fprintf(plw(), "%s",fn);
                        if(ap>=ops[i].prec){fprintf(plw(), "(");pl_writeq_term(t->compound.args[0]);fprintf(plw(), ")");}
                        else pl_writeq_term(t->compound.args[0]);
                    }
                    break;
                }
            }
            if (!is_op) {
                if (atom_needs_quoting(fn)) {
                    fputc('\'', plw());
                    for (const char *p=fn;*p;p++){if(*p=='\'')fputc('\'', plw());fputc(*p, plw());}
                    fputc('\'', plw());
                } else { fprintf(plw(), "%s",fn); }
                fprintf(plw(), "(");
                for (int i=0;i<t->compound.arity;i++){if(i)fprintf(plw(), ",");pl_writeq_term(t->compound.args[i]);}
                fprintf(plw(), ")");
            }
            break;
        }
        default: break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_writeq(Term *t) { pl_writeq_term(t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_write_canonical_term(Term *t) {
    t = term_deref(t);
    if (!t) { fprintf(plw(), "'[]'"); return; }
    switch (t->tag) {
        case TERM_ATOM: {
            const char *name = prolog_atom_name(t->atom_id);
            if (!name) name = "?";
            if (atom_needs_quoting(name)) {
                fputc('\'', plw());
                for (const char *p=name;*p;p++){if(*p=='\'')fputc('\'', plw());fputc(*p, plw());}
                fputc('\'', plw());
            } else fprintf(plw(), "%s",name);
            break;
        }
        case TERM_VAR:  fprintf(plw(), "_G%d",t->var_slot); break;
        case TERM_INT:  fprintf(plw(), "%ld",t->ival); break;
        case TERM_FLOAT: {
            double fv=t->fval;
            if(fv==(long)fv&&fv>=-1e15&&fv<=1e15) fprintf(plw(), "%.1f",fv);
            else fprintf(plw(), "%g",fv);
            break;
        }
        case TERM_COMPOUND: {
            const char *fn = prolog_atom_name(t->compound.functor);
            if (!fn) fn = "?";
            if (t->compound.functor == ATOM_DOT && t->compound.arity == 2) {
                extern int rt_pl_dialect_is_swi(void);
                if (!rt_pl_dialect_is_swi()) {
                    fprintf(plw(), "'.'("); pl_write_canonical_term(t->compound.args[0]);
                    fprintf(plw(), ","); pl_write_canonical_term(t->compound.args[1]);
                    fprintf(plw(), ")"); break;
                }
                fprintf(plw(), "["); pl_write_canonical_term(t->compound.args[0]);
                Term *tail = term_deref(t->compound.args[1]);
                while (tail && tail->tag==TERM_COMPOUND && tail->compound.functor==ATOM_DOT && tail->compound.arity==2) {
                    fprintf(plw(), ","); pl_write_canonical_term(tail->compound.args[0]);
                    tail = term_deref(tail->compound.args[1]);
                }
                if (!(tail && tail->tag==TERM_ATOM && tail->atom_id==ATOM_NIL)) { fprintf(plw(), "|"); pl_write_canonical_term(tail); }
                fprintf(plw(), "]"); break;
            }
            if (atom_needs_quoting(fn)) {
                fputc('\'', plw());
                for(const char *p=fn;*p;p++){if(*p=='\'')fputc('\'', plw());fputc(*p, plw());}
                fputc('\'', plw());
            } else fprintf(plw(), "%s",fn);
            fprintf(plw(), "(");
            for(int i=0;i<t->compound.arity;i++){
                if(i) fprintf(plw(), ",");
                pl_write_canonical_term(t->compound.args[i]);
            }
            fprintf(plw(), ")");
            break;
        }
        default: break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_write_canonical(Term *t) { pl_write_canonical_term(t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_wt_atom(const char *name, int quoted) {
    if (!name) name = "?";
    if (quoted && atom_needs_quoting(name)) { fputc('\'', plw()); for (const char *p=name;*p;p++){if(*p=='\'')fputc('\'', plw());fputc(*p, plw());} fputc('\'', plw()); }
    else fprintf(plw(), "%s", name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_wt(Term *t, int quoted, int ignore_ops, int numbervars, long max_depth, long depth) {
    t = term_deref(t);
    if (!t) { pl_wt_atom("[]", quoted); return; }
    if (max_depth > 0 && depth >= max_depth) { fprintf(plw(), "..."); return; }
    switch (t->tag) {
        case TERM_ATOM: pl_wt_atom(prolog_atom_name(t->atom_id), quoted); break;
        case TERM_VAR: fprintf(plw(), "_G%d", t->var_slot); break;
        case TERM_INT: fprintf(plw(), "%ld", t->ival); break;
        case TERM_FLOAT: {
            double fv = t->fval; char fb[64];
            for (int pr = 15; pr <= 17; pr++) { snprintf(fb, sizeof fb, "%.*g", pr, fv); if (strtod(fb, NULL) == fv) break; }
            if (!strpbrk(fb, ".eEnN")) { size_t n = strlen(fb); if (n+2 < sizeof fb) { fb[n]='.'; fb[n+1]='0'; fb[n+2]='\0'; } }
            fputs(fb, plw()); break;
        }
        case TERM_COMPOUND: {
            const char *fn = prolog_atom_name(t->compound.functor); if (!fn) fn = "?";
            if (numbervars && strcmp(fn,"$VAR")==0 && t->compound.arity==1) {
                Term *n = term_deref(t->compound.args[0]);
                if (n && n->tag == TERM_INT) { long num=n->ival; int letter=(int)(num%26); long suf=num/26; if(suf==0) fprintf(plw(), "%c",'A'+letter); else fprintf(plw(), "%c%ld",'A'+letter,suf); break; }
            }
            if (!ignore_ops && t->compound.functor == ATOM_DOT && t->compound.arity == 2) {
                fprintf(plw(), "["); pl_wt(t->compound.args[0], quoted, ignore_ops, numbervars, max_depth, depth+1);
                Term *tail = term_deref(t->compound.args[1]); long dd = depth+1;
                while (tail && tail->tag==TERM_COMPOUND && tail->compound.functor==ATOM_DOT && tail->compound.arity==2) {
                    if (max_depth > 0 && dd >= max_depth) { fprintf(plw(), "|..."); tail = (Term *)0; break; }
                    fprintf(plw(), ","); pl_wt(tail->compound.args[0], quoted, ignore_ops, numbervars, max_depth, dd+1);
                    tail = term_deref(tail->compound.args[1]); dd++;
                }
                if (tail && !(tail->tag==TERM_ATOM && tail->atom_id==ATOM_NIL)) { fprintf(plw(), "|"); pl_wt(tail, quoted, ignore_ops, numbervars, max_depth, dd); }
                fprintf(plw(), "]"); break;
            }
            struct { const char *name; int arity; int prec; int right_assoc; } ops[] = {
                {":-",2,1200,1},{";",2,1100,1},{"->",2,1050,1},{",",2,1000,1},
                {"=",2,700,0},{"\\=",2,700,0},{"is",2,700,0},{"=:=",2,700,0},{"=\\=",2,700,0},
                {"<",2,700,0},{">",2,700,0},{"=<",2,700,0},{">=",2,700,0},
                {"==",2,700,0},{"\\==",2,700,0},{"@<",2,700,0},{"@>",2,700,0},{"@=<",2,700,0},{"@>=",2,700,0},
                {"=..",2,700,0},{"+",2,500,0},{"-",2,500,0},
                {"*",2,400,0},{"/",2,400,0},{"//",2,400,0},{"mod",2,400,0},{"rem",2,400,0},{"<<",2,400,0},{">>",2,400,0},
                {"**",2,200,1},{"^",2,200,1},{"-",1,200,0},{"\\+",1,900,0},{"not",1,900,0},{NULL,0,0,0}
            };
            int is_op = 0;
            if (!ignore_ops) for (int i = 0; ops[i].name; i++) if (strcmp(fn,ops[i].name)==0 && t->compound.arity==ops[i].arity) {
                is_op = 1;
                if (ops[i].arity == 2) {
                    Term *la=term_deref(t->compound.args[0]), *ra=term_deref(t->compound.args[1]); int lp=-1, rp=-1;
                    if (la&&la->tag==TERM_COMPOUND) { const char *lf=prolog_atom_name(la->compound.functor); if(lf) lp=pl_op_prec(lf,la->compound.arity); }
                    if (ra&&ra->tag==TERM_COMPOUND) { const char *rf=prolog_atom_name(ra->compound.functor); if(rf) rp=pl_op_prec(rf,ra->compound.arity); }
                    int my = ops[i].prec;
                    if ((lp>my)||(lp==my&&ops[i].right_assoc)) { fprintf(plw(), "("); pl_wt(t->compound.args[0],quoted,ignore_ops,numbervars,max_depth,depth+1); fprintf(plw(), ")"); }
                    else pl_wt(t->compound.args[0], quoted, ignore_ops, numbervars, max_depth, depth+1);
                    if (isalpha((unsigned char)fn[0])) fprintf(plw(), " %s ", fn); else fprintf(plw(), "%s", fn);
                    if ((rp>my)||(rp==my&&!ops[i].right_assoc)) { fprintf(plw(), "("); pl_wt(t->compound.args[1],quoted,ignore_ops,numbervars,max_depth,depth+1); fprintf(plw(), ")"); }
                    else pl_wt(t->compound.args[1], quoted, ignore_ops, numbervars, max_depth, depth+1);
                } else {
                    Term *arg = term_deref(t->compound.args[0]); int ap = -1;
                    if (arg&&arg->tag==TERM_COMPOUND) { const char *af=prolog_atom_name(arg->compound.functor); if(af) ap=pl_op_prec(af,arg->compound.arity); }
                    if (isalpha((unsigned char)fn[0])) fprintf(plw(), "%s ", fn); else fprintf(plw(), "%s", fn);
                    if (ap >= ops[i].prec) { fprintf(plw(), "("); pl_wt(t->compound.args[0],quoted,ignore_ops,numbervars,max_depth,depth+1); fprintf(plw(), ")"); }
                    else pl_wt(t->compound.args[0], quoted, ignore_ops, numbervars, max_depth, depth+1);
                }
                break;
            }
            if (!is_op) {
                if (fn && fn[0] == '.' && fn[1] == 0) fprintf(plw(), "'.'"); else pl_wt_atom(fn, quoted); fprintf(plw(), "(");
                for (int i = 0; i < t->compound.arity; i++) { if(i) fprintf(plw(), ","); pl_wt(t->compound.args[i],quoted,ignore_ops,numbervars,max_depth,depth+1); }
                fprintf(plw(), ")");
            }
            break;
        }
        default: break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_write_term_opts(Term *t, int quoted, int ignore_ops, int numbervars, long max_depth) { pl_wt(t, quoted, ignore_ops, numbervars, max_depth, 0); }
static int _aid_plus=-1, _aid_minus=-1, _aid_times=-1, _aid_div=-1, _aid_mod=-1;
