/*  pcre.pl -- SWI-Prolog's library(pcre) over a PCRE subset matched in Prolog (cfo 2026-10-11, row prolog-library-pcre-re-foldl-6-
    and-re-split-3-are-missing-advent-demo, CEO-1660).  SCRIP's own text, AGPL like the rest of SCRIP: not a vendored file -- swipl's
    pcre.pl is a front for the C libpcre2 binding, and SCRIP links no foreign library (no allocator but its own).  The generator
    (util_gen_prolog_prelude_libs.py) embeds it in LOCAL mode: no SWI reader, no renames, its helpers '$re_'-named by hand.  It is
    parsed only when a program names one of its exports.

    THE PATTERN.  Text (string, atom or code list), or Text/Flags with Flags an atom of single-letter flags: i caseless, m multiline,
    s dotall, x extended (layout and #-comments outside classes are ignored), g global (re_replace), and the default capture type a
    atom, I integer, f float, n number, r range (Start-Length), S string, t term.  The syntax: literals; . ; [...] and [^...] with
    ranges and the class escapes; \d \D \w \W \s \S \b \B; \n \t \r \f \v \e \0 and \xHH / \x{H...}; any other escaped character is
    itself; ^ $ (\A \z \Z); groups ( ), (?: ), (?<name> ) and (?P<name> ), a name's suffix _A _I _F _N _R _S _T giving that group's
    capture type; alternation |; quantifiers * + ? {n} {n,} {n,m}, each greedy, lazy (?) or possessive (+, matched as greedy).  A
    construct outside the subset (back references, look-around, atomic groups, \p{...}) raises existence_error(re_syntax, Text).

    THE MATCH.  Leftmost, and among the matches at one start the first in PCRE's backtracking order (alternatives left to right,
    greedy repeats longest first).  Positions count characters.  A match is a dict re_match{0: Whole, 1: G1, ..., Name: Gn} -- an
    unset group is absent -- each capture converted to its type.  re_foldl/6 and re_split/3,4 walk the non-overlapping matches left
    to right; after an empty match the next search starts one character on, as libpcre2's callers do.

    MEASURED AGAINST swipl 9.0.4 (2026-10-11): the row's witness (re_foldl over \d+, re_split over \s+) and the Advent of Code day-3
    shape ((?<num>\d+)/r read as Start-Length).
*/
:- module(pcre, [ re_match/2, re_match/3, re_matchsub/3, re_matchsub/4, re_foldl/6, re_split/3, re_split/4, re_replace/4,
                  re_replace/5 ]).
re_match(Re, S) :- re_match(Re, S, []).
re_match(Re, S, Opts) :- '$re_prep'(Re, Opts, R), '$re_codes'(S, W), '$re_search'(R, W, 0, W, _, _), !.
re_matchsub(Re, S, Sub) :- re_matchsub(Re, S, Sub, []).
re_matchsub(Re, S, Sub, Opts) :- '$re_prep'(Re, Opts, R), '$re_codes'(S, W), '$re_search'(R, W, 0, W, M, _), !, '$re_sub'(R, M, Sub).
re_foldl(G, Re, S, V0, V, Opts) :- '$re_prep'(Re, Opts, R), '$re_codes'(S, W), '$re_fold'(R, G, W, W, 0, V0, V).
'$re_fold'(R, G, W, Cs, P, V0, V) :-
    (   '$re_search'(R, W, P, Cs, M, Rest)
    ->  '$re_sub'(R, M, Sub), call(G, Sub, V0, V1), '$re_step'(M, Rest, Cs1, P1), ( Cs1 == none -> V = V1 ; '$re_fold'(R, G, W, Cs1, P1, V1, V) )
    ;   V = V0
    ).
'$re_step'(m(_, c(S, E, _), _, _), Rest, Cs, P) :- ( E =:= S -> ( Rest = [_|Cs] -> P is E + 1 ; Cs = none, P = E ) ; Cs = Rest, P = E ).
re_split(Re, S, Parts) :- re_split(Re, S, Parts, []).
re_split(Re, S, Parts, Opts) :- '$re_prep'(Re, Opts, R), '$re_codes'(S, W), '$re_type'(R, 0, T), '$re_split'(R, T, W, W, 0, W, 0, Parts).
'$re_split'(R, T, W, Cs, P, FCs, From, Parts) :-
    (   '$re_search'(R, W, P, Cs, M, Rest)
    ->  M = m(_, c(S, E, SCs), _, _), '$re_cap'(T, c(From, S, FCs), Pre), '$re_cap'(T, c(S, E, SCs), Mid), Parts = [Pre, Mid|Ps],
        '$re_step'(M, Rest, Cs1, P1),
        ( Cs1 == none -> '$re_cap'(T, c(E, E, Rest), Post), Ps = [Post] ; '$re_split'(R, T, W, Cs1, P1, Rest, E, Ps) )
    ;   length(FCs, L), E is From + L, '$re_cap'(T, c(From, E, FCs), Post), Parts = [Post]
    ).
re_replace(Re, With, S, Out) :- re_replace(Re, With, S, Out, []).
re_replace(Re, With, S, Out, Opts) :-
    '$re_prep'(Re, Opts, R), '$re_codes'(S, W), '$re_codes'(With, Ws), R = re(_, _, Fl, _, _),
    ( memberchk(g, Fl) -> All = true ; All = false ), '$re_repl'(R, Ws, W, W, 0, W, 0, All, Os), string_codes(Out, Os).
'$re_repl'(R, Ws, W, Cs, P, FCs, From, All, Os) :-
    (   '$re_search'(R, W, P, Cs, M, Rest)
    ->  M = m(_, c(S, E, _), _, _), N is S - From, '$re_take'(N, FCs, Pre), '$re_subst'(Ws, M, Rep), append(Pre, Rep, Os0),
        (   All == true, '$re_step'(M, Rest, Cs1, P1), Cs1 \== none
        ->  '$re_repl'(R, Ws, W, Cs1, P1, Rest, E, All, Os1), append(Os0, Os1, Os)
        ;   append(Os0, Rest, Os)
        )
    ;   Os = FCs
    ).
'$re_subst'([], _, []).
'$re_subst'([0'\\, D|Ws], M, Out) :- D >= 0'0, D =< 0'9, !, I is D - 0'0, '$re_group_codes'(M, I, G), '$re_subst'(Ws, M, R), append(G, R, Out).
'$re_subst'([0'$, D|Ws], M, Out) :- D >= 0'0, D =< 0'9, !, I is D - 0'0, '$re_group_codes'(M, I, G), '$re_subst'(Ws, M, R), append(G, R, Out).
'$re_subst'([C|Ws], M, [C|R]) :- '$re_subst'(Ws, M, R).
'$re_group_codes'(m(_, C0, Caps, _), I, G) :- ( I =:= 0 -> C = C0 ; memberchk(I-C, Caps) -> true ; C = c(0, 0, []) ), C = c(S, E, Cs), N is E - S, '$re_take'(N, Cs, G).
/* ---- the prepared pattern: re(Ast, NGroups, Flags, Names, DefaultType) ---- */
'$re_prep'(Re, Opts, re(Ast, NG, Fl, Names, T)) :-
    ( nonvar(Re), Re = Pat/F -> atom_codes(F, FCs), '$re_flags'(FCs, Fl0, T0) ; Pat = Re, Fl0 = [], T0 = s ),
    '$re_opts'(Opts, Fl0, Fl, T0, T), '$re_codes'(Pat, PCs),
    ( memberchk(x, Fl) -> '$re_strip_x'(PCs, PCs1) ; PCs1 = PCs ),
    ( '$re_alt'(PCs1, [], 0, NG, [], Names, Ast) -> true ; throw(error(existence_error(re_syntax, Pat), _)) ).
'$re_flags'([], [], s).
'$re_flags'([C|Cs], Fl, T) :- '$re_flags'(Cs, Fl0, T0), char_code(Ch, C),
    ( memberchk(Ch-Ty, [a-a, 'I'-'I', f-f, n-n, r-r, 'S'-s, t-t]) -> Fl = Fl0, T = Ty ; Fl = [Ch|Fl0], T = T0 ).
'$re_opts'([], Fl, Fl, T, T).
'$re_opts'([O|Os], Fl0, Fl, T0, T) :- '$re_opt'(O, Fl0, Fl1, T0, T1), '$re_opts'(Os, Fl1, Fl, T1, T).
'$re_opt'(capture_type(Ty), Fl, Fl, _, T) :- !, '$re_type_name'(Ty, T).
'$re_opt'(caseless(true), Fl, [i|Fl], T, T) :- !.
'$re_opt'(multiline(true), Fl, [m|Fl], T, T) :- !.
'$re_opt'(dotall(true), Fl, [s|Fl], T, T) :- !.
'$re_opt'(extended(true), Fl, [x|Fl], T, T) :- !.
'$re_opt'(_, Fl, Fl, T, T).
'$re_type_name'(string, s).
'$re_type_name'(atom, a).
'$re_type_name'(integer, 'I').
'$re_type_name'(float, f).
'$re_type_name'(number, n).
'$re_type_name'(range, r).
'$re_type_name'(term, t).
'$re_codes'(T, Cs) :- string(T), !, string_codes(T, Cs).
'$re_codes'(T, Cs) :- atom(T), !, atom_codes(T, Cs).
'$re_codes'(T, Cs) :- is_list(T), !, ( T = [C|_], atom(C) -> atomic_list_concat(T, A), atom_codes(A, Cs) ; Cs = T ).
'$re_codes'(T, Cs) :- number(T), !, number_codes(T, Cs).
'$re_codes'(T, _) :- var(T), !, throw(error(instantiation_error, _)).
'$re_codes'(T, _) :- throw(error(type_error(text, T), _)).
'$re_strip_x'([], []).
'$re_strip_x'([0'\\, C|Cs], [0'\\, C|Os]) :- !, '$re_strip_x'(Cs, Os).
'$re_strip_x'([0'[|Cs], [0'[|Os]) :- !, '$re_strip_x_class'(Cs, Os).
'$re_strip_x'([0'#|Cs], Os) :- !, '$re_strip_x_comment'(Cs, Os).
'$re_strip_x'([C|Cs], Os) :- memberchk(C, [32, 9, 10, 13, 12, 11]), !, '$re_strip_x'(Cs, Os).
'$re_strip_x'([C|Cs], [C|Os]) :- '$re_strip_x'(Cs, Os).
'$re_strip_x_class'([], []).
'$re_strip_x_class'([0'\\, C|Cs], [0'\\, C|Os]) :- !, '$re_strip_x_class'(Cs, Os).
'$re_strip_x_class'([0']|Cs], [0']|Os]) :- !, '$re_strip_x'(Cs, Os).
'$re_strip_x_class'([C|Cs], [C|Os]) :- '$re_strip_x_class'(Cs, Os).
'$re_strip_x_comment'([], []).
'$re_strip_x_comment'([10|Cs], Os) :- !, '$re_strip_x'(Cs, Os).
'$re_strip_x_comment'([_|Cs], Os) :- '$re_strip_x_comment'(Cs, Os).
/* ---- the pattern parser: Codes0, Codes, Group0, Group, Names0, Names, Ast ---- */
'$re_alt'(Cs0, Cs, G0, G, N0, N, Ast) :-
    '$re_seq'(Cs0, Cs1, G0, G1, N0, N1, S),
    ( Cs1 = [0'||Cs2] -> '$re_alt'(Cs2, Cs, G1, G, N1, N, A2), ( A2 = alt(As) -> Ast = alt([S|As]) ; Ast = alt([S, A2]) ) ; Cs = Cs1, G = G1, N = N1, Ast = S ).
'$re_seq'(Cs0, Cs, G0, G, N0, N, seq(Items)) :- '$re_items'(Cs0, Cs, G0, G, N0, N, Items).
'$re_items'([], [], G, G, N, N, []) :- !.
'$re_items'([C|Cs], [C|Cs], G, G, N, N, []) :- ( C =:= 0'| ; C =:= 0') ), !.
'$re_items'(Cs0, Cs, G0, G, N0, N, [I|Is]) :- '$re_atom'(Cs0, Cs1, G0, G1, N0, N1, A), '$re_quant'(Cs1, Cs2, A, I), '$re_items'(Cs2, Cs, G1, G, N1, N, Is).
'$re_quant'([0'*|Cs0], Cs, A, rep(A, 0, inf, M)) :- !, '$re_lazy'(Cs0, Cs, M).
'$re_quant'([0'+|Cs0], Cs, A, rep(A, 1, inf, M)) :- !, '$re_lazy'(Cs0, Cs, M).
'$re_quant'([0'?|Cs0], Cs, A, rep(A, 0, 1, M)) :- !, '$re_lazy'(Cs0, Cs, M).
'$re_quant'([0'{|Cs0], Cs, A, rep(A, Min, Max, M)) :- '$re_int'(Cs0, Cs1, Min), ( Cs1 = [0'}|Cs2] -> Max = Min ; Cs1 = [0',, 0'}|Cs2] -> Max = inf ; Cs1 = [0',|Cs3], '$re_int'(Cs3, [0'}|Cs2], Max) ), !, '$re_lazy'(Cs2, Cs, M).
'$re_quant'(Cs, Cs, A, A).
'$re_lazy'([0'?|Cs], Cs, lazy) :- !.
'$re_lazy'([0'+|Cs], Cs, greedy) :- !.
'$re_lazy'(Cs, Cs, greedy).
'$re_int'([D|Cs0], Cs, N) :- D >= 0'0, D =< 0'9, '$re_int_more'(Cs0, Cs, D, Ds), number_codes(N, Ds).
'$re_int_more'([D|Cs0], Cs, D0, [D0|Ds]) :- D >= 0'0, D =< 0'9, !, '$re_int_more'(Cs0, Cs, D, Ds).
'$re_int_more'(Cs, Cs, D0, [D0]).
'$re_atom'([0'(, 0'?, 0':|Cs0], Cs, G0, G, N0, N, A) :- !, '$re_alt'(Cs0, [0')|Cs], G0, G, N0, N, A).
'$re_atom'([0'(, 0'?, 0'<, C|Cs0], Cs, G0, G, N0, N, group(G1, A)) :- C \== 0'=, C \== 0'!, !, '$re_name'([C|Cs0], Cs1, Nm), G1 is G0 + 1, '$re_alt'(Cs1, [0')|Cs], G1, G, [G1-Nm|N0], N, A).
'$re_atom'([0'(, 0'?, 0'P, 0'<|Cs0], Cs, G0, G, N0, N, group(G1, A)) :- !, '$re_name'(Cs0, Cs1, Nm), G1 is G0 + 1, '$re_alt'(Cs1, [0')|Cs], G1, G, [G1-Nm|N0], N, A).
'$re_atom'([0'(, 0'?|_], _, _, _, _, _, _) :- !, fail.
'$re_atom'([0'(|Cs0], Cs, G0, G, N0, N, group(G1, A)) :- !, G1 is G0 + 1, '$re_alt'(Cs0, [0')|Cs], G1, G, N0, N, A).
'$re_atom'([0'[, 0'^|Cs0], Cs, G, G, N, N, class(true, Is)) :- !, '$re_class'(Cs0, Cs, Is, first).
'$re_atom'([0'[|Cs0], Cs, G, G, N, N, class(false, Is)) :- !, '$re_class'(Cs0, Cs, Is, first).
'$re_atom'([0'.|Cs], Cs, G, G, N, N, any) :- !.
'$re_atom'([0'^|Cs], Cs, G, G, N, N, bol) :- !.
'$re_atom'([0'$|Cs], Cs, G, G, N, N, eol) :- !.
'$re_atom'([0'\\, C|Cs0], Cs, G, G, N, N, A) :- !, '$re_esc'(C, Cs0, Cs, A).
'$re_atom'([C|Cs], Cs, G, G, N, N, chr(C)) :- C =\= 0'), C =\= 0'|, C =\= 0'*, C =\= 0'+, C =\= 0'?.
'$re_name'(Cs0, Cs, Nm) :- '$re_name_codes'(Cs0, Cs, NCs), NCs \== [], atom_codes(Nm, NCs).
'$re_name_codes'([0'>|Cs], Cs, []) :- !.
'$re_name_codes'([C|Cs0], Cs, [C|Ns]) :- ( code_type(C, alnum) ; C =:= 0'_ ), !, '$re_name_codes'(Cs0, Cs, Ns).
'$re_esc'(0'd, Cs, Cs, cls(digit)) :- !.
'$re_esc'(0'D, Cs, Cs, ncls(digit)) :- !.
'$re_esc'(0'w, Cs, Cs, cls(word)) :- !.
'$re_esc'(0'W, Cs, Cs, ncls(word)) :- !.
'$re_esc'(0's, Cs, Cs, cls(space)) :- !.
'$re_esc'(0'S, Cs, Cs, ncls(space)) :- !.
'$re_esc'(0'b, Cs, Cs, wordb) :- !.
'$re_esc'(0'B, Cs, Cs, nwordb) :- !.
'$re_esc'(0'A, Cs, Cs, bos) :- !.
'$re_esc'(0'z, Cs, Cs, eos) :- !.
'$re_esc'(0'Z, Cs, Cs, eol_z) :- !.
'$re_esc'(C, Cs0, Cs, chr(X)) :- '$re_esc_code'(C, Cs0, Cs, X), !.
'$re_esc'(C, _, _, _) :- ( C >= 0'1, C =< 0'9 ; memberchk(C, [0'p, 0'P, 0'k, 0'g, 0'G, 0'K, 0'R, 0'X, 0'h, 0'H, 0'N, 0'C]) ), !, fail.
'$re_esc'(C, Cs, Cs, chr(C)).
'$re_esc_code'(0'n, Cs, Cs, 10).
'$re_esc_code'(0't, Cs, Cs, 9).
'$re_esc_code'(0'r, Cs, Cs, 13).
'$re_esc_code'(0'f, Cs, Cs, 12).
'$re_esc_code'(0'v, Cs, Cs, 11).
'$re_esc_code'(0'e, Cs, Cs, 27).
'$re_esc_code'(0'a, Cs, Cs, 7).
'$re_esc_code'(0'0, Cs, Cs, 0).
'$re_esc_code'(0'x, [0'{|Cs0], Cs, X) :- !, '$re_hex'(Cs0, [0'}|Cs], 0, X, _).
'$re_esc_code'(0'x, Cs0, Cs, X) :- '$re_hex2'(Cs0, Cs, X).
'$re_hex'([D|Cs0], Cs, X0, X, n) :- '$re_hexd'(D, V), !, X1 is X0 * 16 + V, '$re_hex'(Cs0, Cs, X1, X, _).
'$re_hex'(Cs, Cs, X, X, _).
'$re_hex2'([D1, D2|Cs], Cs, X) :- '$re_hexd'(D1, V1), '$re_hexd'(D2, V2), !, X is V1 * 16 + V2.
'$re_hex2'([D1|Cs], Cs, X) :- '$re_hexd'(D1, X).
'$re_hexd'(D, V) :- D >= 0'0, D =< 0'9, !, V is D - 0'0.
'$re_hexd'(D, V) :- D >= 0'a, D =< 0'f, !, V is D - 0'a + 10.
'$re_hexd'(D, V) :- D >= 0'A, D =< 0'F, V is D - 0'A + 10.
'$re_class'([0']|Cs], Cs, [], rest) :- !.
'$re_class'([0'\\, C|Cs0], Cs, [I|Is], _) :- !, '$re_class_esc'(C, Cs0, Cs1, I0), '$re_class_range'(I0, Cs1, Cs2, I), '$re_class'(Cs2, Cs, Is, rest).
'$re_class'([C|Cs0], Cs, [I|Is], _) :- '$re_class_range'(C, Cs0, Cs1, I), '$re_class'(Cs1, Cs, Is, rest).
'$re_class_esc'(C, Cs, Cs, cls(K)) :- memberchk(C-K, [0'd-digit, 0'w-word, 0's-space]), !.
'$re_class_esc'(C, Cs, Cs, ncls(K)) :- memberchk(C-K, [0'D-digit, 0'W-word, 0'S-space]), !.
'$re_class_esc'(0'b, Cs, Cs, 8) :- !.
'$re_class_esc'(C, Cs0, Cs, X) :- '$re_esc_code'(C, Cs0, Cs, X), !.
'$re_class_esc'(C, Cs, Cs, C).
'$re_class_range'(L, [0'-, 0'\\, E|Cs0], Cs, range(L, H)) :- integer(L), '$re_esc_code'(E, Cs0, Cs, H), !.
'$re_class_range'(L, [0'-, H|Cs], Cs, range(L, H)) :- integer(L), H \== 0'], H \== 0'\\, !.
'$re_class_range'(I, Cs, Cs, I).
/* ---- the matcher: Ast, Codes0, Pos0, Codes, Pos, Caps0, Caps; the environment E = env(Flags, Whole) ---- */
'$re_search'(re(Ast, _, Fl, _, _), W, P0, Cs0, m(W, Whole, Caps, Fl), Rest) :- '$re_try'(Ast, env(Fl, W), Cs0, P0, Whole, Caps, Rest).
'$re_try'(Ast, Env, Cs, P, c(P, E, Cs), Caps, Rest) :- '$re_m'(Ast, Env, Cs, P, Rest, E, [], Caps), !.
'$re_try'(Ast, Env, [_|Cs], P0, Whole, Caps, Rest) :- P1 is P0 + 1, '$re_try'(Ast, Env, Cs, P1, Whole, Caps, Rest).
'$re_m'(seq(Is), Env, Cs0, P0, Cs, P, C0, C) :- '$re_seqm'(Is, Env, Cs0, P0, Cs, P, C0, C).
'$re_m'(alt(As), Env, Cs0, P0, Cs, P, C0, C) :- '$re_altm'(As, Env, Cs0, P0, Cs, P, C0, C).
'$re_m'(chr(X), env(Fl, _), [Y|Cs], P0, Cs, P, C, C) :- ( X =:= Y -> true ; memberchk(i, Fl), '$re_lower'(X, L), '$re_lower'(Y, L) ), P is P0 + 1.
'$re_m'(any, env(Fl, _), [Y|Cs], P0, Cs, P, C, C) :- ( Y =\= 10 -> true ; memberchk(s, Fl) ), P is P0 + 1.
'$re_m'(cls(K), _, [Y|Cs], P0, Cs, P, C, C) :- '$re_kind'(K, Y), P is P0 + 1.
'$re_m'(ncls(K), _, [Y|Cs], P0, Cs, P, C, C) :- \+ '$re_kind'(K, Y), P is P0 + 1.
'$re_m'(class(Neg, Is), env(Fl, _), [Y|Cs], P0, Cs, P, C, C) :- ( '$re_in'(Is, Fl, Y) -> Neg == false ; Neg == true ), P is P0 + 1.
'$re_m'(group(I, A), Env, Cs0, P0, Cs, P, C0, C) :- '$re_m'(A, Env, Cs0, P0, Cs, P, C0, C1), '$re_setcap'(C1, I, c(P0, P, Cs0), C).
'$re_m'(rep(A, Min, Max, M), Env, Cs0, P0, Cs, P, C0, C) :- '$re_rep'(Min, Max, M, A, Env, Cs0, P0, Cs, P, C0, C).
'$re_m'(bol, env(Fl, W), Cs, P, Cs, P, C, C) :- ( P =:= 0 -> true ; memberchk(m, Fl), P1 is P - 1, '$re_nth0'(P1, W, 10) ).
'$re_m'(eol, env(Fl, _), Cs, P, Cs, P, C, C) :- ( Cs == [] -> true ; Cs == [10] -> true ; memberchk(m, Fl), Cs = [10|_] ).
'$re_m'(bos, _, Cs, 0, Cs, 0, C, C).
'$re_m'(eos, _, [], P, [], P, C, C).
'$re_m'(eol_z, _, Cs, P, Cs, P, C, C) :- ( Cs == [] ; Cs == [10] ), !.
'$re_m'(wordb, env(_, W), Cs, P, Cs, P, C, C) :- '$re_atb'(W, P, Cs).
'$re_m'(nwordb, env(_, W), Cs, P, Cs, P, C, C) :- \+ '$re_atb'(W, P, Cs).
'$re_seqm'([], _, Cs, P, Cs, P, C, C).
'$re_seqm'([I|Is], Env, Cs0, P0, Cs, P, C0, C) :- '$re_m'(I, Env, Cs0, P0, Cs1, P1, C0, C1), '$re_seqm'(Is, Env, Cs1, P1, Cs, P, C1, C).
'$re_altm'([A|_], Env, Cs0, P0, Cs, P, C0, C) :- '$re_m'(A, Env, Cs0, P0, Cs, P, C0, C).
'$re_altm'([_|As], Env, Cs0, P0, Cs, P, C0, C) :- '$re_altm'(As, Env, Cs0, P0, Cs, P, C0, C).
'$re_rep'(Min, Max, M, A, Env, Cs0, P0, Cs, P, C0, C) :-
    Min > 0, !, '$re_m'(A, Env, Cs0, P0, Cs1, P1, C0, C1), Min1 is Min - 1, '$re_dec'(Max, Max1), '$re_rep'(Min1, Max1, M, A, Env, Cs1, P1, Cs, P, C1, C).
'$re_rep'(_, Max, greedy, A, Env, Cs0, P0, Cs, P, C0, C) :-
    Max \== 0, '$re_m'(A, Env, Cs0, P0, Cs1, P1, C0, C1), P1 > P0, '$re_dec'(Max, Max1), '$re_rep'(0, Max1, greedy, A, Env, Cs1, P1, Cs, P, C1, C).
'$re_rep'(_, _, greedy, _, _, Cs, P, Cs, P, C, C).
'$re_rep'(_, _, lazy, _, _, Cs, P, Cs, P, C, C).
'$re_rep'(_, Max, lazy, A, Env, Cs0, P0, Cs, P, C0, C) :-
    Max \== 0, '$re_m'(A, Env, Cs0, P0, Cs1, P1, C0, C1), P1 > P0, '$re_dec'(Max, Max1), '$re_rep'(0, Max1, lazy, A, Env, Cs1, P1, Cs, P, C1, C).
'$re_dec'(inf, inf) :- !.
'$re_dec'(N, N1) :- N1 is N - 1.
'$re_setcap'([], I, V, [I-V]).
'$re_setcap'([J-W|Cs], I, V, Out) :- ( J =:= I -> Out = [I-V|Cs] ; Out = [J-W|Out1], '$re_setcap'(Cs, I, V, Out1) ).
'$re_kind'(digit, Y) :- Y >= 0'0, Y =< 0'9.
'$re_kind'(word, Y) :- ( Y >= 0'a, Y =< 0'z ; Y >= 0'A, Y =< 0'Z ; Y >= 0'0, Y =< 0'9 ; Y =:= 0'_ ), !.
'$re_kind'(space, Y) :- memberchk(Y, [32, 9, 10, 11, 12, 13]).
'$re_in'([I|Is], Fl, Y) :- ( '$re_in1'(I, Fl, Y) -> true ; '$re_in'(Is, Fl, Y) ).
'$re_in1'(range(L, H), Fl, Y) :- !, ( Y >= L, Y =< H -> true ; memberchk(i, Fl), '$re_lower'(Y, Ly), '$re_upper'(Y, Uy), ( Ly >= L, Ly =< H ; Uy >= L, Uy =< H ) ).
'$re_in1'(cls(K), _, Y) :- !, '$re_kind'(K, Y).
'$re_in1'(ncls(K), _, Y) :- !, \+ '$re_kind'(K, Y).
'$re_in1'(X, Fl, Y) :- ( X =:= Y -> true ; memberchk(i, Fl), '$re_lower'(X, L), '$re_lower'(Y, L) ).
'$re_lower'(X, L) :- ( X >= 0'A, X =< 0'Z -> L is X + 32 ; L = X ).
'$re_upper'(X, U) :- ( X >= 0'a, X =< 0'z -> U is X - 32 ; U = X ).
'$re_nth0'(I, [X|Xs], Y) :- ( I =:= 0 -> X = Y ; I1 is I - 1, '$re_nth0'(I1, Xs, Y) ).
'$re_atb'(W, P, Cs) :- ( P =:= 0 -> Before = false ; P1 is P - 1, '$re_nth0'(P1, W, B), ( '$re_kind'(word, B) -> Before = true ; Before = false ) ),
    ( Cs = [A|_], '$re_kind'(word, A) -> After = true ; After = false ), Before \== After.
/* ---- the match as a dict ---- */
'$re_sub'(re(_, NG, _, Names, T), m(_, Whole, Caps, _), Sub) :- '$re_cap'(T, Whole, V0), '$re_subs'(1, NG, Names, T, Caps, Ps), dict_pairs(Sub, re_match, [0-V0|Ps]).
'$re_subs'(I, NG, _, _, _, []) :- I > NG, !.
'$re_subs'(I, NG, Names, T, Caps, Ps) :- I1 is I + 1,
    ( memberchk(I-C, Caps) -> '$re_key'(I, Names, T, K, Ty), '$re_cap'(Ty, C, V), Ps = [K-V|Ps1] ; Ps = Ps1 ),
    '$re_subs'(I1, NG, Names, T, Caps, Ps1).
'$re_key'(I, Names, T, K, Ty) :- ( memberchk(I-Nm, Names) -> '$re_name_type'(Nm, T, K, Ty) ; K = I, Ty = T ).
'$re_name_type'(Nm, T, K, Ty) :-
    atom_codes(Nm, Cs), ( append(KCs, [0'_, C], Cs), KCs \== [], char_code(Ch, C), memberchk(Ch-Ty, ['A'-a, 'I'-'I', 'F'-f, 'N'-n, 'R'-r, 'S'-s, 'T'-t]) -> atom_codes(K, KCs) ; K = Nm, Ty = T ).
'$re_type'(re(_, _, _, _, T), _, T).
'$re_cap'(r, c(S, E, _), S-L) :- !, L is E - S.
'$re_cap'(T, c(S, E, Cs0), V) :- N is E - S, '$re_take'(N, Cs0, Cs), '$re_conv'(T, Cs, V).
'$re_conv'(s, Cs, V) :- !, string_codes(V, Cs).
'$re_conv'(a, Cs, V) :- !, atom_codes(V, Cs).
'$re_conv'('I', Cs, V) :- !, number_codes(V, Cs).
'$re_conv'(f, Cs, V) :- !, number_codes(N, Cs), V is float(N).
'$re_conv'(n, Cs, V) :- !, number_codes(V, Cs).
'$re_conv'(t, Cs, V) :- !, atom_codes(A, Cs), term_to_atom(V, A).
'$re_conv'(_, Cs, V) :- string_codes(V, Cs).
'$re_take'(0, _, []) :- !.
'$re_take'(N, [C|Cs], [C|Ts]) :- N1 is N - 1, '$re_take'(N1, Cs, Ts).
