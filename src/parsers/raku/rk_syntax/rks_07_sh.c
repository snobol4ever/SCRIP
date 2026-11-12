#include "parsers/raku/rk_syntax/rks_head.h"
#include "parsers/raku/rk_syntax/rks_01_sh.h"
#include "parsers/raku/rk_syntax/rks_02_sh.h"
#include "parsers/raku/rk_syntax/rks_03_sh.h"
#include "parsers/raku/rk_syntax/rks_04_sh.h"
#include "parsers/raku/rk_syntax/rks_05_sh.h"
#include "parsers/raku/rk_syntax/rks_06_sh.h"
#include "parsers/raku/rk_syntax/rks_07_sh.h"
#include "parsers/raku/rk_syntax/rks_09_sh.h"
static int r_postfix_op_raw(RkP *p, int pos);
static int r_postfixish_at(RkP *p, int q, int sub, int meta);
static void register_user_op_x(RkP *p, int from, int to, int exported);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int prev_cp(RkP *p, int pos) {
    if (pos <= 0) return -1;
    int q = pos - 1;
    while (q > 0 && (((unsigned char) p->s[q]) & 0xC0) == 0x80) q--;
    return cp_at(p, q);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int infix_pred_ok(RkP *p, int pos, const char *s) {
    if (!strcmp(s, "-")) return ch(p, pos + 1) != '>' || at_lit(p, pos + 1, ">>");
    if (!strcmp(s, "!=")) return is_space_cp(cp_at(p, pos + 2)) || ch(p, pos + 2) == ']';
    if (!strcmp(s, "<<") || !strcmp(s, ">>")) return 0;
    if ((!strcmp(s, "+<") || !strcmp(s, "~<")) && p->in_meta) return at_lit(p, pos + 2, "<<") || ch(p, pos + 2) != '<';
    if ((!strcmp(s, "+>") || !strcmp(s, "~>")) && p->in_meta) return at_lit(p, pos + 2, ">>") || ch(p, pos + 2) != '>';
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int op_table_longest(RkP *p, int pos, const RkOp *tab, const RkOp **best) {
    int bl = 0; *best = NULL;
    for (int i = 0; tab[i].sym; i++) {
        int l = (int) strlen(tab[i].sym);
        if (l <= bl) continue;
        if (!at_lit(p, pos, tab[i].sym)) continue;
        if ((tab[i].flags & OF_WORD) && wordch_at(p, pos + l)) continue;
        if (tab == rk_infix && !infix_pred_ok(p, pos, tab[i].sym)) continue;
        bl = l; *best = &tab[i];
    }
    return bl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int user_op_longest_i(RkP *p, int pos, char cat, int *prec, int *idx) {
    int bl = 0; *idx = -1;
    for (int i = p->nuops - 1; i >= 0; i--) {
        RkUserOp *u = &p->uops[i];
        if (u->cat != cat || u->len <= bl) continue;
        if (pos + u->len > p->n || memcmp(p->s + pos, u->sym, (size_t) u->len)) continue;
        if (u->wordend && (cat == 't' || cat == 'p') && wordch_at(p, pos + u->len)) continue;
        bl = u->len; *prec = u->prec; *idx = i;
    }
    return bl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int user_op_longest(RkP *p, int pos, char cat, int *prec) { int i; return user_op_longest_i(p, pos, cat, prec, &i); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void set_op(OpInfo *o, const RkOp *r, int from, int to) {
    o->prec = r->prec; o->sub = r->sub ? r->sub : r->prec; o->assoc = r->assoc; o->flags = r->flags; o->from = from; o->to = to; o->nextterm = NT_TERMISH;
    if (r->flags & OF_COMMA) o->nextterm = NT_NULLTERMISH;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_ternary_rest(RkP *p, int pos) {
    int sg = p->goal; p->goal = GOAL_BANGBANG;
    int q = ws(p, pos);
    int e = r_EXPR(p, q, PR('i'));
    tree_t *mid = p->build ? rkb_expr(p->B, takelst(p)) : NULL;
    p->goal = sg;
    if (e < 0) panic_at(p, q, "Confused: Found ?? but no !!");
    int t = ws(p, e);
    if (at_lit(p, t, "!!")) { p->tv = mid; return t + 2; }
    if (at_lit(p, t, "::") && ch(p, t + 2) != '=') panic_at(p, t, "Please use !! rather than :: in the conditional operator");
    if (ch(p, t) == ':' && ch(p, t + 1) != '=' && !is_word_cp(cp_at(p, t + 1))) panic_at(p, t, "Please use !! rather than : in the conditional operator");
    OpInfo io;
    if (r_infix_plain(p, t, &io) >= 0) panic_at(p, t, "Precedence of %.*s is too loose to use inside ?? !!; please parenthesize", io.to - io.from, p->s + io.from);
    panic_at(p, t, "Confused: Found ?? but no !!");
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_infix_plain(RkP *p, int pos, OpInfo *o) {
    o->mid = NULL;
    const RkOp *best; int bl = op_table_longest(p, pos, rk_infix, &best);
    int uprec = 0; int uidx = -1; int ul = user_op_longest_i(p, pos, 'i', &uprec, &uidx);
    int c = ch(p, pos);
    if (at_lit(p, pos, "??") && ch(p, pos + 2) != '?') {
        if (ul > 2) goto user;
        int e = r_ternary_rest(p, pos + 2);
        o->mid = p->build ? taketv(p) : NULL;
        o->prec = PR('j'); o->sub = o->prec; o->assoc = AS_RIGHT; o->flags = OF_FIDDLY; o->from = pos; o->to = e; o->nextterm = NT_TERMISH;
        return e;
    }
    if (c == '=' && !at_lit(p, pos, "==") && !at_lit(p, pos, "=>") && !at_lit(p, pos, "=:=") && !at_lit(p, pos, "=~") && !at_lit(p, pos, "===") && ul <= 1) {
        if (at_lit(p, pos, "=~") && !at_lit(p, pos, "=~=") && !p->p5isms) panic_at(p, pos, "Unsupported use of =~ to do pattern matching; in Raku please use ~~");
        if (p->p5isms && at_lit(p, pos, "=~")) { o->prec = PR('m'); o->sub = o->prec; o->assoc = AS_LEFT; o->flags = OF_CHAIN | OF_IFFY; o->from = pos; o->to = pos + 2; o->nextterm = NT_TERMISH;
            return pos + 2; }
        int item = p->leftsigil == '$' || p->in_meta;
        o->prec = PR('i'); o->sub = item ? PR('i') : PR('e'); o->assoc = AS_RIGHT; o->flags = item ? 0 : OF_FIDDLY; o->from = pos; o->to = pos + 1; o->nextterm = NT_TERMISH;
        p->leftsigil = 0;
        return pos + 1;
    }
    if (at_lit(p, pos, "=~") && !at_lit(p, pos, "=~=") && !p->p5isms) panic_at(p, pos, "Unsupported use of =~ to do pattern matching; in Raku please use ~~");
    if (p->p5isms && ((at_lit(p, pos, "=~") && !at_lit(p, pos, "=~=")) || (at_lit(p, pos, "!~") && !at_lit(p, pos, "!~~")))) {
        o->prec = PR('m'); o->sub = o->prec; o->assoc = AS_LEFT; o->flags = OF_CHAIN | OF_IFFY; o->from = pos; o->to = pos + 2; o->nextterm = NT_TERMISH;
        return pos + 2;
    }
    if (at_lit(p, pos, "!~") && !at_lit(p, pos, "!~~") && is_space_cp(cp_at(p, pos + 2)) && !p->p5isms) panic_at(p, pos,
        "Unsupported use of !~ to do negated pattern matching; in Raku please use !~~");
    if (at_lit(p, pos, "::=")) panic_at(p, pos, "\"::=\" not yet implemented. Sorry.");
    if (at_lit(p, pos, ".=")) { o->prec = PR('v'); o->sub = PR('z'); o->assoc = AS_LEFT; o->flags = OF_FIDDLY | OF_DOTTY; o->from = pos; o->to = pos + 2; o->nextterm = NT_DOTTYOPISH; return pos + 2; }
    if (c == '.' && !at_lit(p, pos, "..") && ul <= 1) {
        int q = ws(p, pos + 1);
        if (p->in_reduce) return -1;
        if (!is_alpha_cp(cp_at(p, q))) {
            if (q != pos + 1) panic_at(p, pos, "Unsupported use of . to concatenate strings; in Raku please use ~");
            if (is_space_cp(prev_cp(p, pos))) panic_at(p, pos, "Malformed postfix call (only basic method calls that exclusively use a dot can be detached)");
            panic_at(p, pos, "Malformed postfix call");
        }
        o->prec = PR('v'); o->sub = PR('z'); o->assoc = AS_LEFT; o->flags = OF_FIDDLY | OF_DOTTY; o->from = pos; o->to = q; o->nextterm = NT_DOTTYOPISH;
        return q;
    }
    if (c == ':' && !at_lit(p, pos, ":=") && !at_lit(p, pos, "::=")) {
        if (p->invocant_ok && p->goal != GOAL_BANGBANG) {
            int n1 = cp_at(p, pos + 1);
            if (pos + 1 >= p->n || is_space_cp(n1) || is_terminator_at(p, pos + 1)) {
                p->invocant_ok = 0; p->saw_inv = 1;
                o->prec = PR('g'); o->sub = o->prec; o->assoc = AS_LIST; o->flags = 0; o->from = pos; o->to = pos + 1; o->nextterm = NT_NULLTERMISH;
                return pos + 1;
            }
        }
        return -1;
    }
    if (c == '?' && !at_lit(p, pos, "??") && !at_lit(p, pos, "?|") && !at_lit(p, pos, "?^") && !at_lit(p, pos, "?&")) {
        int q = pos + 1;
        while (q < p->n && p->s[q] != ';' && p->s[q] != ':') q++;
        if (ch(p, q) == ':') panic_at(p, pos, "Unsupported use of ? and : for the ternary conditional operator; in Raku please use ?? and !!");
        return -1;
    }
    if ((at_lit(p, pos, "<<") || at_lit(p, pos, ">>")) && !at_lit(p, pos, "<<==") && !at_lit(p, pos, "==>>") && !p->in_meta && is_space_cp(cp_at(p, pos + 2)) && ul < 2)
        panic_at(p, pos, "Unsupported use of %s to do %s shift; in Raku please use %s", c == '<' ? "<<" : ">>", c == '<' ? "left" : "right", c == '<' ? "+< or ~<" : "+> or ~>");
    if (best && !strcmp(best->sym, "..") && !p->in_meta && (ch(p, pos + 2) == ')' || ch(p, pos + 2) == ']')) panic_at(p, pos, "Please use ..* for indefinite range");
    if (ul > bl) goto user;
    if (!best) return -1;
    if (!strcmp(best->sym, ",")) p->invocant_ok = 0;
    set_op(o, best, pos, pos + bl);
    return pos + bl;
user:
    o->prec = uprec; o->sub = uprec; o->assoc = uidx >= 0 ? p->uops[uidx].assoc : AS_LEFT; o->flags = uidx >= 0 ? p->uops[uidx].flags : 0; o->from = pos; o->to = pos + ul; o->nextterm = NT_TERMISH;
    if (o->flags & OF_COMMA) o->nextterm = NT_NULLTERMISH;
    return pos + ul;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_infixish_core(RkP *p, int pos, OpInfo *o, int in_meta) {
    int sm = p->in_meta; p->in_meta = in_meta;
    int best = -1; OpInfo bo; memset(&bo, 0, sizeof bo);
    OpInfo t; memset(&t, 0, sizeof t);
    int c = cp_at(p, pos);
    if (c == '[') {
        if (ch(p, pos + 1) == '&' && (is_alpha_cp(cp_at(p, pos + 2)) || ch(p, pos + 2) == '(' || strchr(".!^:*?=~", ch(p, pos + 2)))) {
            int e = r_variable(p, pos + 1);
            if (e >= 0 && ch(p, e) == ']') { t.prec = PR('t'); t.sub = t.prec; t.assoc = AS_LEFT; t.flags = 0; t.from = pos; t.to = e + 1; t.nextterm = NT_TERMISH; if (e + 1 > best) { best = e + 1;
                bo = t; } }
        }
        if (best < 0) {
            int e = r_infixish(p, pos + 1, &t, 'b');
            if (e >= 0 && ch(p, e) == ']') { t.from = pos; t.to = e + 1; best = e + 1; bo = t; }
        }
    }
    if (c == 0xAB || c == 0xBB || at_lit(p, pos, "<<") || at_lit(p, pos, ">>")) {
        int l = (c == '<' || c == '>') ? 2 : 2;
        int e = r_infixish(p, pos + l, &t, 'h');
        if (e >= 0) {
            int d = cp_at(p, e);
            int ok = (c == 0xAB || c == 0xBB) ? (d == 0xAB || d == 0xBB) : (at_lit(p, e, "<<") || at_lit(p, e, ">>"));
            if (ok) { int f = e + 2; if (f > best) { best = f; bo = t; bo.from = pos; bo.to = f; } }
            else if (c == 0xAB || c == 0xBB) panic_at(p, e, "Missing \xc2\xab or \xc2\xbb");
        }
    }
    int neg_bad = -1; int chose_meta = 0;
    if (c == '!' && ch(p, pos + 1) != '!') {
        int e = r_infixish(p, pos + 1, &t, 'n');
        if (e >= 0) {
            int is_eq = (e == pos + 2 && ch(p, pos + 1) == '=');
            if (is_eq) { t.prec = PR('m'); t.sub = t.prec; t.assoc = AS_LEFT; t.flags = OF_CHAIN | OF_IFFY; }
            else if (!(t.flags & OF_IFFY)) neg_bad = e;
            if (e > best) { best = e; bo = t; bo.from = pos; bo.to = e; chose_meta = 1; }
        }
    }
    if ((c == 'R' || c == 'S' || c == 'X' || c == 'Z') && !wordch_at(p, pos + 1)) {
        int e = r_infixish(p, pos + 1, &t, c);
        if (e >= 0 && e > best) {
            if (c == 'X' || c == 'Z') { t.prec = PR('f'); t.sub = t.prec; t.assoc = AS_LIST; t.flags = 0; }
            best = e; bo = t; bo.from = pos; bo.to = e;
        }
    }
    if ((c == 'R' || c == 'S' || c == 'X' || c == 'Z') && wordch_at(p, pos + 1)) {
        int e = r_infixish(p, pos + 1, &t, c);
        if (e >= 0 && e > best) {
            if (c == 'X' || c == 'Z') { t.prec = PR('f'); t.sub = t.prec; t.assoc = AS_LIST; t.flags = 0; }
            best = e; bo = t; bo.from = pos; bo.to = e;
        }
    }
    int e = r_infix_plain(p, pos, &t);
    if (e >= 0 && e >= best) { best = e; bo = t; chose_meta = 0; }
    p->in_meta = sm;
    if (best < 0) return -1;
    if (neg_bad >= 0 && best == neg_bad && chose_meta && c == '!') panic_at(p, pos, "Cannot negate %.*s because it is not iffy enough", neg_bad - pos - 1, p->s + pos + 1);
    if (ch(p, best) == '=' && ch(p, best + 1) != '=' && !(bo.flags & OF_DOTTY) && best - bo.from >= 1 && !(best - bo.from == 1 && ch(p, bo.from) == '=')) {
        int prevop_is_assign = (bo.prec == PR('i'));
        if (!prevop_is_assign && !(bo.prec == PR('m') && ch(p, best - 1) == '=')) {
            if (bo.flags & OF_DIFFY) panic_at(p, pos, "Cannot make assignment out of %.*s because %s operators are diffy", best - bo.from, p->s + bo.from, "structural");
            int item = bo.prec > PR('g');
            bo.prec = PR('i'); bo.sub = item ? PR('i') : PR('e'); bo.assoc = AS_RIGHT; bo.flags = 0; bo.nextterm = NT_TERMISH;
            best++; bo.to = best;
        }
    }
    *o = bo;
    return best;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_infixish(RkP *p, int pos, OpInfo *o, int in_meta) {
    if (!in_meta && (stdstopper(p, pos) || infixstopper(p, pos))) return -1;
    if (in_meta && pos >= p->n) return -1;
    if (!in_meta && !p->in_reduce && ch(p, pos) == ':' && (is_alpha_cp(cp_at(p, pos + 1)) || ch(p, pos + 1) == '!' || ch(p, pos + 1) == '<' || ch(p, pos + 1) == '(' ||
                                                           ch(p, pos + 1) == '[' || ch(p, pos + 1) == '{' || ch(p, pos + 1) == '$' || ch(p, pos + 1) == '@' || ch(p, pos + 1) == '%' ||
                                                           ch(p, pos + 1) == '&' || is_digit_cp(cp_at(p, pos + 1)) || cp_at(p, pos + 1) == 0xAB)) {
        if (!(is_digit_cp(cp_at(p, pos + 1)) && !is_alpha_cp(cp_at(p, r_decint(p, pos + 1))))) {
            int e = r_colonpair(p, pos);
            int k = ch(p, pos + 1);
            if (e >= 0 && (k == '{' || k == '[' || k == '<' || cp_at(p, pos + 1) == 0xAB)) panic_at(p, pos, "You can't adverb %.*s", e - pos, p->s + pos);
            if (e >= 0) { o->prec = PR('i'); o->sub = o->prec; o->assoc = AS_UNARY; o->flags = OF_FAKE; o->from = pos; o->to = e; o->nextterm = NT_TERMISH; return e; }
        }
    }
    if (!in_meta && (ch(p, pos) == '{' || at_lambda(p, pos))) {
        if (p->in_reduce) return -1;
        panic_at(p, pos, "Unexpected block in infix position (missing statement control word before the expression?)");
    }
    return r_infixish_core(p, pos, o, in_meta);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_prefixish(RkP *p, int pos, int *prec) {
    if (at_lambda(p, pos) || at_lit(p, pos, "???") || at_lit(p, pos, "!!!") || at_lit(p, pos, "...")) return -1;
    const RkOp *best; int bl = op_table_longest(p, pos, rk_prefix, &best);
    int uprec = 0; int ul = user_op_longest(p, pos, 'p', &uprec);
    int e = -1;
    if (ul > bl) { e = pos + ul; *prec = uprec; }
    else if (best) {
        const char *s = best->sym;
        if (!strcmp(s, "?") && at_lit(p, pos, "??")) panic_at(p, pos, "Expected a term, but found either infix ?? or redundant prefix ?");
        if (!strcmp(s, "!") && at_lit(p, pos, "!!")) return -1;
        if (!strcmp(s, "~") && at_lit(p, pos, "~~")) panic_at(p, pos, "Expected a term, but found either infix ~~ or redundant prefix ~");
        if (!strcmp(s, "^") && at_lit(p, pos, "^^")) panic_at(p, pos, "Expected a term, but found either infix ^^ or redundant prefix ^");
        if (best->flags & OF_WORD) {
            if (!end_keyword_ok(p, pos + bl)) return -1;
            e = pos + bl;
            if (!strcmp(s, "let") || !strcmp(s, "temp")) { int c = cp_at(p, e); if (!(is_space_cp(c) || c == '#')) return -1; }
        }
        else e = pos + bl;
        *prec = best->prec;
    }
    if (e < 0) return -1;
    if (cp_at(p, e) == 0xAB) e += 2;
    else if (at_lit(p, e, "<<") && !at_lit(p, e, "<<==")) e += 2;
    return ws(p, e);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_postfix_op(RkP *p, int pos) {
    int e = r_postfix_op_raw(p, pos);
    if (e >= 0 && p->build) { memset(&p->pf, 0, sizeof p->pf); p->pf.k = 'P'; p->pf.from = pos; p->pf.to = e; p->pf.txt = ct_strndup0(p->s + pos, e - pos); }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_postfix_op_raw(RkP *p, int pos) {
    const RkOp *best; int bl = op_table_longest(p, pos, rk_postfix, &best);
    int uprec = 0; int ul = user_op_longest(p, pos, 'P', &uprec);
    if (at_lit(p, pos, "->")) {
        int c = ch(p, pos + 2);
        if (c == '[' || c == '{' || c == '(') panic_at(p, pos, "Unsupported use of ->(), ->{} or ->[] as postfix dereferencer; in Raku please use .(), .[] or .{} to deref");
        panic_at(p, pos, "Unsupported use of -> as postfix; in Raku please use either . to call a method, or whitespace to delimit a pointy block");
    }
    int c = cp_at(p, pos);
    if (c == 0x207B || c == 0x207A || c == 0xAF || c == 0x2070 || c == 0xB9 || c == 0xB2 || c == 0xB3 || (c >= 0x2074 && c <= 0x2079)) {
        int q = pos;
        if (c == 0x207B || c == 0x207A || c == 0xAF) q += cp_len(p, q);
        int d = cp_at(p, q); int n = 0;
        while (d == 0x2070 || d == 0xB9 || d == 0xB2 || d == 0xB3 || (d >= 0x2074 && d <= 0x2079)) { q += cp_len(p, q); d = cp_at(p, q); n++; }
        if (n && q - pos > bl && q - pos > ul) return q;
    }
    if (ul > bl) return pos + ul;
    if (!best) return -1;
    return pos + bl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_dotty(RkP *p, int pos) {
    if (ch(p, pos) != '.') return -1;
    int q = pos + 1;
    int c = ch(p, q);
    if (c == '.') return -1;
    if (c == '+' || c == '*' || c == '?' || c == '=' || c == '^') {
        int t = q + 1;
        if (c == '^' && ch(p, t) == '!') t++;
        int e = r_dottyop(p, t);
        if (e >= 0) { p->pf.mod = c; p->pf.from = pos; return e; }
    }
    int e = r_dottyop(p, q);
    if (e >= 0) p->pf.from = pos;
    return e;
}
int r_postfixish(RkP *p, int pos) {
    int sub = p->x_sub; p->x_sub = 0;
    if (stdstopper(p, pos)) return -1;
    memset(&p->pf, 0, sizeof p->pf);
    int wf = marked_ws_from(p, pos);
    if (wf >= 0 && wf != pos) return -1;
    int q = pos;
    if (!p->qsigil) { int u = r_unsp(p, q); if (u >= 0) q = u; else if (ch(p, q) == '\\' && q + 1 < p->n && !is_space_cp(cp_at(p, q + 1))) q++; }
    int meta = 0;
    {
        int t = q;
        if (ch(p, t) == '.') { int u = t + 1; int v = r_unsp(p, u); if (v >= 0) u = v; if (cp_at(p, u) == 0xBB || at_lit(p, u, ">>")) t = u; }
        if (cp_at(p, t) == 0xBB || (at_lit(p, t, ">>") && !(p->qsigil && ch(p, t + 2) == '('))) {
            t += 2; meta = 1;
            int v = r_unsp(p, t); if (v >= 0) t = v;
            q = t;
        }
    }
    int e = r_postfixish_at(p, q, sub, meta);
    if (e >= 0 && meta && p->build) p->pf.hyper = 1;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int r_postfixish_at(RkP *p, int q, int sub, int meta) {
    int e;
    if (p->nuops) {
        int prec; int idx; int l = user_op_longest_i(p, q, 'k', &prec, &idx);
        if (l > 0 && idx >= 0 && idx + 1 < p->nuops && p->uops[idx + 1].cat == 'K') {
            RkUserOp *cl = &p->uops[idx + 1];
            const char *su = p->ustop; int sl = p->ustop_len; int sq = p->qsigil;
            p->ustop = cl->sym; p->ustop_len = cl->len; p->qsigil = 0;
            int t = r_semilist(p, q + l);
            RkList *uin = p->last_list;
            p->ustop = su; p->ustop_len = sl; p->qsigil = sq;
            t = ws(p, t);
            if (t + cl->len <= p->n && !memcmp(p->s + t, cl->sym, (size_t) cl->len)) {
                if (p->build) { memset(&p->pf, 0, sizeof p->pf); p->pf.k = 'U'; p->pf.from = q; p->pf.to = t + cl->len; p->pf.txt = p->uops[idx].sym; p->pf.inner = uin; }
                return t + cl->len;
            }
            panic_at(p, t, "Unable to parse postcircumfix:<%s %s>; couldn't find final %s", p->uops[idx].sym, cl->sym, cl->sym);
        }
    }
    if ((e = r_postfix_op(p, q)) >= 0) return e;
    if (ch(p, q) == '.' && !is_word_cp(cp_at(p, q + 1)) && (e = r_postfix_op(p, q + 1)) >= 0) return e;
    p->x_sub = sub; e = r_postcircumfix(p, q); p->x_sub = 0;
    if (e >= 0) return e;
    if (ch(p, q) == '.' && (ch(p, q + 1) == '[' || ch(p, q + 1) == '{' || ch(p, q + 1) == '<' || ch(p, q + 1) == '(' || cp_at(p, q + 1) == 0xAB)) {
        p->x_sub = sub; e = r_postcircumfix(p, q + 1); p->x_sub = 0;
        if (e >= 0) return e;
    }
    if ((e = r_dotty(p, q)) >= 0) return e;
    if (ch(p, q) == '!' && ch(p, q + 1) != '!' && ch(p, q + 1) != '=' && ch(p, q + 1) != '~' && (e = r_methodop(p, q + 1)) >= 0) { p->pf.mod = '!'; return e; }
    if (meta && !p->qsigil) {
        if (is_space_cp(cp_at(p, q))) panic_at(p, q, "Missing postfix");
        if (is_alpha_cp(cp_at(p, q))) panic_at(p, q, "Missing dot on method call");
        panic_at(p, q, "Malformed postfix");
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bracket_ending_at(RkP *p, int end) {
    int c = prev_cp(p, end);
    return c == ')' || c == '}' || c == ']' || c == '>' || c == 0xBB;
}
int r_semilist(RkP *p, int pos) {
    int star = p->x_star; p->x_star = 0;
    int q = ws(p, pos);
    int sbk = p->bk; tree_t *sl = p->cur_list;
    p->bk = BK_ITEMS; p->cur_list = NULL;
    RkList *first = NULL; int n = 0;
    for (;;) {
        int c = ch(p, q);
        if (c == ')' || c == ']' || c == '}' || q >= p->n) break;
        if (p->ustop && q + p->ustop_len <= p->n && !memcmp(p->s + q, p->ustop, (size_t) p->ustop_len)) break;
        p->stmt_list = NULL;
        p->x_star = !n && star;
        int e = r_statement(p, q);
        p->x_star = 0;
        if (e < 0) break;
        if (!n) first = p->stmt_list;
        n++;
        q = r_eat_terminator(p, e);
        if (q < 0) { q = e; break; }
        q = ws(p, q);
    }
    p->bk = sbk; p->cur_list = sl;
    p->last_list = first; p->last_nstmts = n;
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_dottyop(RkP *p, int pos) {
    int q = pos;
    int u = r_unsp(p, q); if (u >= 0) q = u;
    int e = r_methodop(p, q);
    if (e >= 0) return e;
    if (ch(p, q) == ':') { e = r_colonpair(p, q); if (e >= 0) return e; }
    if (!is_alpha_cp(cp_at(p, q))) {
        if ((e = r_postfix_op(p, q)) >= 0) return e;
        if ((e = r_postcircumfix(p, q)) >= 0) return e;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_reduce(RkP *p, int pos) {
    if (ch(p, pos) != '[') return -1;
    int q = pos + 1;
    if (is_space_cp(cp_at(p, q)) || q >= p->n) return -1;
    int t = q; while (ch(p, t) == '[') t++;
    if (strchr("-+?~^", ch(p, t)) && ch(p, t) && (is_word_cp(cp_at(p, t + 1)) || ch(p, t + 1) == '$' || ch(p, t + 1) == '@')) return -1;
    int r = q; while (r < p->n && !is_space_cp(cp_at(p, r)) && p->s[r] != ']') r++;
    if (ch(p, r) != ']') { int x = q; int depth = 0; while (x < p->n && !is_space_cp(cp_at(p, x))) { if (p->s[x] == '[') depth++; if (p->s[x] == ']') { if (!depth) break; depth--; } x++; } if (ch(p,
        x) != ']') return -1; }
    int si = p->in_reduce; p->in_reduce = 1;
    int tri = 0;
    if (ch(p, q) == '\\') { tri = 1; q++; }
    OpInfo o;
    jmp_buf save; memcpy(save, p->jb, sizeof save);
    int e = -1;
    if (!setjmp(p->jb)) e = r_infixish(p, q, &o, tri ? 't' : 'r');
    else e = -1;
    memcpy(p->jb, save, sizeof save);
    if (e < 0 || ch(p, e) != ']') { p->in_reduce = si; return -1; }
    if (o.flags & OF_FIDDLY) panic_at(p, pos, "Cannot reduce with %.*s because %s operators are too fiddly", e - q, p->s + q, o.prec == PR('i') ? "item assignment" : "conditional");
    if ((o.flags & OF_DIFFY) && !(o.flags & OF_CHAIN)) panic_at(p, pos, "Cannot reduce with %.*s because %s operators are diffy and not chaining", e - q, p->s + q, "structural infix");
    p->in_reduce = si;
    const char *rop = p->build ? rkb_reduce_name(p->B, q, e) : NULL;
    int f = r_args(p, e + 1, 0);
    if (p->build) rkb_reduce(p->B, &p->tm, pos, f, rop, takelst(p));
    return f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int r_user_term(RkP *p, int pos) {
    int prec = 0;
    p->ucf_c = 0;
    int l = user_op_longest(p, pos, 't', &prec);
    if (l > 0) return pos + l;
    l = user_op_longest(p, pos, 'c', &prec);
    if (l > 0) {
        int q = pos + l;
        for (int i = p->nuops - 1; i >= 0; i--) {
            RkUserOp *u = &p->uops[i];
            if (u->cat != 'c' || u->len != l || memcmp(p->s + pos, u->sym, (size_t) l)) continue;
            RkUserOp *cl = (i + 1 < p->nuops && p->uops[i + 1].cat == 'C') ? &p->uops[i + 1] : NULL;
            if (!cl) break;
            const char *su = p->ustop; int sl = p->ustop_len;
            p->ustop = cl->sym; p->ustop_len = cl->len;
            int e = r_semilist(p, q);
            p->ustop = su; p->ustop_len = sl;
            e = ws(p, e);
            if (e + cl->len <= p->n && !memcmp(p->s + e, cl->sym, (size_t) cl->len)) { p->ucf_in = p->last_list; p->ucf_open = u->sym; p->ucf_c = 1; return e + cl->len; }
            panic_at(p, e, "Unable to parse expression in circumfix:<%s %s>; couldn't find final %s", u->sym, cl->sym, cl->sym);
        }
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void register_user_op(RkP *p, int from, int to) { register_user_op_x(p, from, to, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void register_user_op_x(RkP *p, int from, int to, int exported) {
    int nb = p->nuops;
    int n = to - from; const char *s = p->s + from;
    add_routine_name(p, s, n);
    static const char *const cats[] = { "infix", "prefix", "postfix", "circumfix", "postcircumfix", "term", 0 };
    for (int i = 0; cats[i]; i++) {
        int l = (int) strlen(cats[i]);
        if (n > l + 1 && !memcmp(s, cats[i], (size_t) l) && s[l] == ':') {
            int q = from + l + 1;
            if (at_lit(p, q, "sym")) q += 3;
            int c = cp_at(p, q);
            int close = c == '<' ? '>' : c == 0xAB ? 0xBB : c == '[' ? ']' : c == '(' ? ')' : c == '{' ? '}' : 0;
            if (!close) return;
            int a = q + cp_len(p, q);
            int b = a; while (b < to && cp_at(p, b) != close) { if (p->s[b] == '\\' && b + 1 < to) b++; b += cp_len(p, b); }
            if (c == '<' && at_lit(p, q, "<<")) { a = q + 2; b = a; while (b < to && !at_lit(p, b, ">>")) b++; }
            int x = a; int y = b;
            if (c == '[' || c == '(') { while (x < y && (p->s[x] == '\'' || p->s[x] == '"' || asc_space((unsigned char) p->s[x]))) x++; while (y > x && (p->s[y - 1] == '\'' || p->s[y - 1] == '"' ||
                asc_space((unsigned char) p->s[y - 1]))) y--; }
            while (x < y && asc_space((unsigned char) p->s[x])) x++;
            while (y > x && asc_space((unsigned char) p->s[y - 1])) y--;
            if (i == 0) add_user_op(p, 'i', p->s + x, y - x, PR('t'));
            else if (i == 1) add_user_op(p, 'p', p->s + x, y - x, PR('v'));
            else if (i == 2) add_user_op(p, 'P', p->s + x, y - x, PR('x'));
            else if (i == 5) { add_name_n(p, p->s + x, y - x); add_user_op(p, 't', p->s + x, y - x, 0); }
            if (i == 0 || i == 1 || i == 2 || i == 5) { if (exported) for (int k = nb; k < p->nuops; k++) p->uops[k].depth = 1000; return; }
            int sp = x; while (sp < y && !asc_space((unsigned char) p->s[sp])) sp++;
            int op2 = sp; while (op2 < y && asc_space((unsigned char) p->s[op2])) op2++;
            int pair0 = p->nuops;
            if (i == 3) { add_user_op(p, 'c', p->s + x, sp - x, 0); add_user_op(p, 'C', p->s + op2, y - op2, 0); }
            if (i == 3 && p->B && p->nuops == pair0 + 2) rkb_user_op(p->B, 'c', p->uops[pair0].sym, p->uops[pair0].len, p->s + x, y - x, 0);
            if (i == 4) { add_user_op(p, 'k', p->s + x, sp - x, 0); add_user_op(p, 'K', p->s + op2, y - op2, 0); }
            if (i == 4 && p->B && p->nuops == pair0 + 2) rkb_user_op(p->B, 'k', p->uops[pair0].sym, p->uops[pair0].len, p->s + x, y - x, 0);
            if (exported) for (int k = nb; k < p->nuops; k++) p->uops[k].depth = 1000;
            return;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void add_enum_value(RkP *p, const char *en, int enl, const char *v, int vl) {
    add_our_name(p, v, vl); p->names[p->nnames - 1].value = 1;
    if (enl > 0) {
        char *full = (char *) ct_alloc((size_t) enl + (size_t) vl + 3);
        memcpy(full, en, (size_t) enl); memcpy(full + enl, "::", 2); memcpy(full + enl + 2, v, (size_t) vl); full[enl + 2 + vl] = 0;
        add_our_name(p, full, enl + 2 + vl); p->names[p->nnames - 1].value = 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void enum_names_from(RkP *p, int from, int to, const char *en, int enl) {
    int q = from;
    int c = cp_at(p, q);
    if (c == '<' || c == 0xAB) {
        q += (at_lit(p, q, "<<") ? 2 : cp_len(p, q));
        while (q < to) {
            while (q < to && is_space_cp(cp_at(p, q))) q += cp_len(p, q);
            int s = q; while (q < to && !is_space_cp(cp_at(p, q)) && ch(p, q) != '>' && cp_at(p, q) != 0xBB) q += cp_len(p, q);
            if (q > s) add_enum_value(p, en, enl, p->s + s, q - s);
            if (ch(p, q) == '>' || cp_at(p, q) == 0xBB) break;
        }
        return;
    }
    for (q = from; q < to; ) {
        int e = r_identifier(p, q);
        if (e >= 0 && e <= to) {
            int h = hs(p, e);
            if (at_lit(p, h, "=>") || (q > from && p->s[q - 1] == ':' && (q < 2 || p->s[q - 2] != ':'))) add_enum_value(p, en, enl, p->s + q, e - q);
            if (q > from && (p->s[q - 1] == '\'' || p->s[q - 1] == '"')) add_enum_value(p, en, enl, p->s + q, e - q);
            q = e; continue;
        }
        if (ch(p, q) == '\'' || ch(p, q) == '"') { int s = q + 1; int t = s; while (t < to && p->s[t] != p->s[q]) t++; if (t > s) add_enum_value(p, en, enl, p->s + s, t - s); q = t + 1; continue; }
        q += cp_len(p, q);
    }
}
