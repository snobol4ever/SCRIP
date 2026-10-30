/*  uri.pl -- SWI-Prolog's library(uri) as a Prolog layer (cqo 2026-10-30, the cto's ruling on the TreallaTests library gaps:
    "build it now as a prelude library in Prolog"; witness packages/prolog/trealla_tests misc_uri_lib).  SCRIP's own text, AGPL
    like the rest of SCRIP: not a vendored file (swipl's library(uri) is C, uri.c).  The generator embeds it in LOCAL mode: no
    SWI reader, no renames, its helpers '$uri_'-named by hand.  It is parsed only when a program names one of its exports.

    THE MODEL.  A URI is split by RFC 3986 appendix B: scheme ':' then '//' authority, the path up to '?' or '#', the search
    after '?', the fragment after '#'; a part that is absent is left unbound.  Text is worked as code lists of code points; a
    percent escape stands for a UTF-8 octet, so encoding writes each octet of a character above 127 as %XX in upper-case hex
    and decoding gathers the octets back into one code point.  The character classes are uri.c's: unreserved (ALPHA DIGIT
    - . _ ~), sub-delims, pchar = unreserved, sub-delims, : and @; a path adds /, a search and a fragment add / and ?, a query
    name or value is pchar without & = + (and with / and ?), a segment is pchar.

    MEASURED AGAINST swipl 9.0.4 (2026-10-30, the witness's sixty lines): a query name runs to '=' and a value to '&' or ';'
    ('a&b=1' parses as 'a&b'='1'), '+' in a query value decodes to a space and an encoded value writes a space as %20; a
    scheme of one letter is a drive letter, so uri_is_global('c:/x') fails; uri_normalized lower-cases the scheme and the
    host, removes dot segments, decodes an escaped unreserved character and upper-cases the other escapes, while
    iri_normalized leaves the escapes as written and uri_normalized_iri decodes the unreserved and the non-ASCII ones;
    uri_authority_components splits '[::1]:8080' at its first colon as swipl 9.0.4 does (host '[' and port ':1]:8080').
    NOT HERE, as in swipl 9.0.4: uri_edit/3.  A component term other than uri_components/5 to build from raises
    instantiation_error with the context uri:uri_components/2, as swipl does.
*/
:- module(uri, [ uri_components/2, uri_data/3, uri_data/4, uri_normalized/2, uri_normalized/3, iri_normalized/2, iri_normalized/3,
                 uri_normalized_iri/2, uri_normalized_iri/3, uri_resolve/3, uri_is_global/1, uri_query_components/2,
                 uri_authority_components/2, uri_authority_data/3, uri_encoded/3, uri_file_name/2, uri_iri/2 ]).
uri_components(U, C) :- nonvar(U), !, '$uri_codes'(U, Cs), '$uri_split'(Cs, S, A, P, Q, F), C = uri_components(S, A, P, Q, F).
uri_components(U, C) :- nonvar(C), C = uri_components(S, A, P, Q, F), !, '$uri_build'(S, A, P, Q, F, Cs), atom_codes(U, Cs).
uri_components(_, _) :- throw(error(instantiation_error, context(uri:uri_components/2, _))).
'$uri_codes'(T, Cs) :- atom(T), !, atom_codes(T, Cs).
'$uri_codes'(T, Cs) :- string(T), !, string_codes(T, Cs).
'$uri_codes'(T, Cs) :- number(T), !, number_codes(T, Cs).
'$uri_codes'(T, _) :- throw(error(type_error(atomic, T), _)).
'$uri_split'(Cs, S, A, P, Q, F) :-
    '$uri_scheme'(Cs, S, R0),
    '$uri_auth'(R0, A, R1),
    '$uri_until'(R1, [0'?, 0'#], PC, R2), atom_codes(P, PC),
    '$uri_search'(R2, Q, R3),
    '$uri_frag'(R3, F).
'$uri_scheme'(Cs, S, R) :- '$uri_until'(Cs, [0':, 0'/, 0'?, 0'#], Pre, [0':|R0]), Pre \== [], !, atom_codes(S, Pre), R = R0.
'$uri_scheme'(Cs, _, Cs).
'$uri_auth'([0'/, 0'/|R0], A, R) :- !, '$uri_until'(R0, [0'/, 0'?, 0'#], AC, R), atom_codes(A, AC).
'$uri_auth'(R, _, R).
'$uri_search'([0'?|R0], Q, R) :- !, '$uri_until'(R0, [0'#], QC, R), atom_codes(Q, QC).
'$uri_search'(R, _, R).
'$uri_frag'([0'#|R0], F) :- !, atom_codes(F, R0).
'$uri_frag'(_, _).
'$uri_until'([], _, [], []).
'$uri_until'([C|Cs], Stops, B, A) :- ( memberchk(C, Stops) -> B = [], A = [C|Cs] ; B = [C|B1], '$uri_until'(Cs, Stops, B1, A) ).
'$uri_build'(S, A, P, Q, F, Cs) :-
    ( var(S) -> C1 = [] ; '$uri_codes'(S, SC), append(SC, [0':], C1) ),
    ( var(A) -> C2 = [] ; '$uri_codes'(A, AC), append([0'/, 0'/], AC, C2) ),
    ( var(P) -> C3 = [] ; '$uri_codes'(P, PC), '$uri_enc'(path, PC, 1, C3) ),
    ( var(Q) -> C4 = [] ; '$uri_codes'(Q, QC), '$uri_enc'(search, QC, 1, QE), C4 = [0'?|QE] ),
    ( var(F) -> C5 = [] ; '$uri_codes'(F, FC), '$uri_enc'(fragment, FC, 1, FE), C5 = [0'#|FE] ),
    append([C1, C2, C3, C4, C5], Cs).
uri_data(scheme, uri_components(S, _, _, _, _), S).
uri_data(authority, uri_components(_, A, _, _, _), A).
uri_data(path, uri_components(_, _, P, _, _), P).
uri_data(search, uri_components(_, _, _, Q, _), Q).
uri_data(fragment, uri_components(_, _, _, _, F), F).
uri_data(scheme, uri_components(_, A, P, Q, F), S, uri_components(S, A, P, Q, F)).
uri_data(authority, uri_components(S, _, P, Q, F), A, uri_components(S, A, P, Q, F)).
uri_data(path, uri_components(S, A, _, Q, F), P, uri_components(S, A, P, Q, F)).
uri_data(search, uri_components(S, A, P, _, F), Q, uri_components(S, A, P, Q, F)).
uri_data(fragment, uri_components(S, A, P, Q, _), F, uri_components(S, A, P, Q, F)).
'$uri_unreserved'(C) :- C >= 0'a, C =< 0'z, !.
'$uri_unreserved'(C) :- C >= 0'A, C =< 0'Z, !.
'$uri_unreserved'(C) :- C >= 0'0, C =< 0'9, !.
'$uri_unreserved'(C) :- memberchk(C, [0'-, 0'., 0'_, 0'~]).
'$uri_allowed'(_, C) :- '$uri_unreserved'(C), !.
'$uri_allowed'(path, C) :- memberchk(C, [0'!, 0'$, 0'&, 0'', 0'(, 0'), 0'*, 0'+, 0',, 0';, 0'=, 0':, 0'@, 0'/]), !.
'$uri_allowed'(segment, C) :- memberchk(C, [0'!, 0'$, 0'&, 0'', 0'(, 0'), 0'*, 0'+, 0',, 0';, 0'=, 0':, 0'@]), !.
'$uri_allowed'(search, C) :- memberchk(C, [0'!, 0'$, 0'&, 0'', 0'(, 0'), 0'*, 0'+, 0',, 0';, 0'=, 0':, 0'@, 0'/, 0'?]), !.
'$uri_allowed'(fragment, C) :- memberchk(C, [0'!, 0'$, 0'&, 0'', 0'(, 0'), 0'*, 0'+, 0',, 0';, 0'=, 0':, 0'@, 0'/, 0'?]), !.
'$uri_allowed'(query_value, C) :- memberchk(C, [0'!, 0'$, 0'', 0'(, 0'), 0'*, 0',, 0';, 0':, 0'@, 0'/, 0'?]), !.
'$uri_enc'(_, [], _, []).
'$uri_enc'(K, [0'%, H1, H2|Cs], 1, [0'%, H1, H2|Es]) :- '$uri_hexval'(H1, _), '$uri_hexval'(H2, _), !, '$uri_enc'(K, Cs, 1, Es).
'$uri_enc'(K, [C|Cs], Keep, Es) :- C < 128, '$uri_allowed'(K, C), !, Es = [C|Es1], '$uri_enc'(K, Cs, Keep, Es1).
'$uri_enc'(K, [C|Cs], Keep, Es) :- '$uri_utf8'(C, Bs), '$uri_pct'(Bs, Es, Es1), '$uri_enc'(K, Cs, Keep, Es1).
'$uri_pct'([], T, T).
'$uri_pct'([B|Bs], [0'%, H1, H2|T0], T) :- Hi is B >> 4, Lo is B /\ 15, '$uri_hexdig'(Hi, H1), '$uri_hexdig'(Lo, H2), '$uri_pct'(Bs, T0, T).
'$uri_hexdig'(V, D) :- V < 10, !, D is 0'0 + V.
'$uri_hexdig'(V, D) :- D is 0'A + V - 10.
'$uri_hexval'(D, V) :- D >= 0'0, D =< 0'9, !, V is D - 0'0.
'$uri_hexval'(D, V) :- D >= 0'a, D =< 0'f, !, V is D - 0'a + 10.
'$uri_hexval'(D, V) :- D >= 0'A, D =< 0'F, V is D - 0'A + 10.
'$uri_utf8'(C, [C]) :- C < 0x80, !.
'$uri_utf8'(C, [B1, B2]) :- C < 0x800, !, B1 is 0xC0 \/ (C >> 6), B2 is 0x80 \/ (C /\ 0x3F).
'$uri_utf8'(C, [B1, B2, B3]) :- C < 0x10000, !, B1 is 0xE0 \/ (C >> 12), B2 is 0x80 \/ ((C >> 6) /\ 0x3F), B3 is 0x80 \/ (C /\ 0x3F).
'$uri_utf8'(C, [B1, B2, B3, B4]) :- B1 is 0xF0 \/ (C >> 18), B2 is 0x80 \/ ((C >> 12) /\ 0x3F), B3 is 0x80 \/ ((C >> 6) /\ 0x3F), B4 is 0x80 \/ (C /\ 0x3F).
'$uri_dec'(Plus, Cs, Ds) :- '$uri_octets'(Plus, Cs, Os), '$uri_unutf8'(Os, Ds).
'$uri_octets'(_, [], []).
'$uri_octets'(P, [0'%, H1, H2|Cs], [o(B)|Os]) :- '$uri_hexval'(H1, V1), '$uri_hexval'(H2, V2), !, B is V1 * 16 + V2, '$uri_octets'(P, Cs, Os).
'$uri_octets'(1, [0'+|Cs], [c(32)|Os]) :- !, '$uri_octets'(1, Cs, Os).
'$uri_octets'(P, [C|Cs], [c(C)|Os]) :- '$uri_octets'(P, Cs, Os).
'$uri_unutf8'([], []).
'$uri_unutf8'([c(C)|Os], [C|Ds]) :- !, '$uri_unutf8'(Os, Ds).
'$uri_unutf8'([o(B)|Os], [C|Ds]) :- '$uri_seq'(B, N, V0), N > 0, '$uri_cont'(N, Os, V0, C, Os1), !, '$uri_unutf8'(Os1, Ds).
'$uri_unutf8'([o(B)|Os], [B|Ds]) :- '$uri_unutf8'(Os, Ds).
'$uri_seq'(B, 0, B) :- B < 0x80, !.
'$uri_seq'(B, 1, V) :- B >= 0xC0, B < 0xE0, !, V is B /\ 0x1F.
'$uri_seq'(B, 2, V) :- B >= 0xE0, B < 0xF0, !, V is B /\ 0x0F.
'$uri_seq'(B, 3, V) :- B >= 0xF0, B < 0xF8, V is B /\ 0x07.
'$uri_cont'(0, Os, V, V, Os) :- !.
'$uri_cont'(N, [o(B)|Os], V0, V, Os1) :- B >= 0x80, B < 0xC0, V1 is (V0 << 6) \/ (B /\ 0x3F), N1 is N - 1, '$uri_cont'(N1, Os, V1, V, Os1).
uri_encoded(K, V, E) :- nonvar(V), !, '$uri_codes'(V, Cs), '$uri_comp'(K, K1), '$uri_enc'(K1, Cs, 0, Es), atom_codes(E, Es).
uri_encoded(K, V, E) :- '$uri_codes'(E, Cs), ( K == query_value -> P = 1 ; P = 0 ), '$uri_dec'(P, Cs, Ds), atom_codes(V, Ds).
'$uri_comp'(query_value, query_value) :- !.
'$uri_comp'(fragment, fragment) :- !.
'$uri_comp'(path, path) :- !.
'$uri_comp'(segment, segment) :- !.
'$uri_comp'(K, _) :- throw(error(domain_error(uri_component, K), _)).
uri_query_components(Q, L) :- nonvar(Q), !, '$uri_codes'(Q, Cs), '$uri_qparse'(Cs, L).
uri_query_components(Q, L) :- '$uri_qbuild'(L, Cs), atom_codes(Q, Cs).
'$uri_qparse'([], []) :- !.
'$uri_qparse'(Cs, [N = V|L]) :-
    ( '$uri_until'(Cs, [0'=], NC, [0'=|R0]) -> true ; throw(error(syntax_error(illegal_uri_query), _)) ),
    '$uri_until'(R0, [0'&, 0';], VC, R1),
    '$uri_dec'(1, NC, ND), atom_codes(N, ND), '$uri_dec'(1, VC, VD), atom_codes(V, VD),
    ( R1 = [_|R2] -> '$uri_qparse'(R2, L) ; L = [] ).
'$uri_qbuild'([], []).
'$uri_qbuild'([E|Es], Cs) :-
    '$uri_qpair'(E, N, V), '$uri_codes'(N, NC), '$uri_codes'(V, VC),
    '$uri_enc'(query_value, NC, 0, NE), '$uri_enc'(query_value, VC, 0, VE),
    append(NE, [0'=|VE], C0),
    ( Es == [] -> Cs = C0 ; '$uri_qbuild'(Es, R), append(C0, [0'&|R], Cs) ).
'$uri_qpair'(N = V, N, V) :- !.
'$uri_qpair'(N - V, N, V) :- !.
'$uri_qpair'(T, N, V) :- compound(T), T =.. [N, V], !.
'$uri_qpair'(T, _, _) :- throw(error(type_error(name_value, T), _)).
uri_authority_components(A, C) :- nonvar(A), !, '$uri_codes'(A, Cs), '$uri_aparse'(Cs, U, P, H, Pt), C = uri_authority(U, P, H, Pt).
uri_authority_components(A, uri_authority(U, P, H, Pt)) :-
    ( var(U) -> C1 = [] ; '$uri_codes'(U, UC), ( var(P) -> C1a = UC ; '$uri_codes'(P, PC), append(UC, [0':|PC], C1a) ), append(C1a, [0'@], C1) ),
    ( var(H) -> C2 = [] ; '$uri_codes'(H, C2) ),
    ( var(Pt) -> C3 = [] ; '$uri_codes'(Pt, PtC), C3 = [0':|PtC] ),
    append([C1, C2, C3], Cs), atom_codes(A, Cs).
'$uri_aparse'(Cs, U, P, H, Pt) :-
    ( '$uri_until'(Cs, [0'@], UI, [0'@|R]) -> '$uri_userinfo'(UI, U, P) ; R = Cs ),
    ( '$uri_until'(R, [0':], HC, [0':|PtC]) -> true ; HC = R, PtC = none ),
    ( HC == [] -> true ; atom_codes(H, HC) ),
    ( PtC == none -> true ; PtC == [] -> true ; '$uri_port'(PtC, Pt) ).
'$uri_userinfo'(UI, U, P) :- ( '$uri_until'(UI, [0':], UC, [0':|PC]) -> atom_codes(U, UC), atom_codes(P, PC) ; atom_codes(U, UI) ).
'$uri_port'(Cs, Pt) :- '$uri_digits'(Cs), !, number_codes(Pt, Cs).
'$uri_port'(Cs, Pt) :- atom_codes(Pt, Cs).
'$uri_digits'([D]) :- !, D >= 0'0, D =< 0'9.
'$uri_digits'([D|Ds]) :- D >= 0'0, D =< 0'9, '$uri_digits'(Ds).
uri_authority_data(user, uri_authority(U, _, _, _), U).
uri_authority_data(password, uri_authority(_, P, _, _), P).
uri_authority_data(host, uri_authority(_, _, H, _), H).
uri_authority_data(port, uri_authority(_, _, _, Pt), Pt).
uri_is_global(U) :- '$uri_codes'(U, Cs), '$uri_until'(Cs, [0':, 0'/, 0'?, 0'#], [C0, C1|Cr], [0':|_]), '$uri_alpha'(C0), '$uri_schemechars'([C1|Cr]).
'$uri_alpha'(C) :- C >= 0'a, C =< 0'z, !.
'$uri_alpha'(C) :- C >= 0'A, C =< 0'Z.
'$uri_schemechars'([]).
'$uri_schemechars'([C|Cs]) :- ( '$uri_alpha'(C) -> true ; C >= 0'0, C =< 0'9 -> true ; memberchk(C, [0'+, 0'-, 0'.]) ), '$uri_schemechars'(Cs).
uri_resolve(R, B, T) :-
    '$uri_codes'(R, RC), '$uri_split'(RC, Rs, Ra, Rp, Rq, Rf),
    '$uri_codes'(B, BC), '$uri_split'(BC, Bs, Ba, Bp, Bq, _),
    atom_codes(Rp, RpC), atom_codes(Bp, BpC),
    '$uri_resolve'(Rs, Ra, RpC, Rq, Bs, Ba, BpC, Bq, Ts, Ta, TpC, Tq),
    atom_codes(Tp, TpC), '$uri_build_raw'(Ts, Ta, Tp, Tq, Rf, Cs), atom_codes(T, Cs).
'$uri_resolve'(Rs, Ra, RpC, Rq, _, _, _, _, Rs, Ra, TpC, Rq) :- nonvar(Rs), !, '$uri_dots'(RpC, TpC).
'$uri_resolve'(_, Ra, RpC, Rq, Bs, _, _, _, Bs, Ra, TpC, Rq) :- nonvar(Ra), !, '$uri_dots'(RpC, TpC).
'$uri_resolve'(_, _, [], Rq, Bs, Ba, BpC, Bq, Bs, Ba, BpC, Tq) :- !, ( nonvar(Rq) -> Tq = Rq ; Tq = Bq ).
'$uri_resolve'(_, _, [0'/|Rp], Rq, Bs, Ba, _, _, Bs, Ba, TpC, Rq) :- !, '$uri_dots'([0'/|Rp], TpC).
'$uri_resolve'(_, _, RpC, Rq, Bs, Ba, BpC, _, Bs, Ba, TpC, Rq) :- '$uri_merge'(Ba, BpC, RpC, M), '$uri_dots'(M, TpC).
'$uri_merge'(Ba, [], RpC, [0'/|RpC]) :- nonvar(Ba), !.
'$uri_merge'(_, BpC, RpC, M) :- '$uri_lastslash'(BpC, Dir), append(Dir, RpC, M).
'$uri_lastslash'(Cs, Dir) :- append(Dir, Rest, Cs), last(Dir, 0'/), \+ memberchk(0'/, Rest), !.
'$uri_lastslash'(_, []).
'$uri_build_raw'(S, A, P, Q, F, Cs) :-
    ( var(S) -> C1 = [] ; atom_codes(S, SC), append(SC, [0':], C1) ),
    ( var(A) -> C2 = [] ; atom_codes(A, AC), append([0'/, 0'/], AC, C2) ),
    atom_codes(P, C3),
    ( var(Q) -> C4 = [] ; atom_codes(Q, QC), C4 = [0'?|QC] ),
    ( var(F) -> C5 = [] ; atom_codes(F, FC), C5 = [0'#|FC] ),
    append([C1, C2, C3, C4, C5], Cs).
'$uri_dots'(In, Out) :- '$uri_rds'(In, [], Rev), reverse(Rev, Segs), append(Segs, Out).
'$uri_rds'([], Acc, Acc) :- !.
'$uri_rds'([0'., 0'., 0'/|In], Acc, Out) :- !, '$uri_rds'(In, Acc, Out).
'$uri_rds'([0'., 0'/|In], Acc, Out) :- !, '$uri_rds'(In, Acc, Out).
'$uri_rds'([0'/, 0'., 0'/|In], Acc, Out) :- !, '$uri_rds'([0'/|In], Acc, Out).
'$uri_rds'([0'/, 0'.], Acc, Out) :- !, '$uri_rds'([0'/], Acc, Out).
'$uri_rds'([0'/, 0'., 0'., 0'/|In], Acc, Out) :- !, '$uri_pop'(Acc, Acc1), '$uri_rds'([0'/|In], Acc1, Out).
'$uri_rds'([0'/, 0'., 0'.], Acc, Out) :- !, '$uri_pop'(Acc, Acc1), '$uri_rds'([0'/], Acc1, Out).
'$uri_rds'([0'.], Acc, Acc) :- !.
'$uri_rds'([0'., 0'.], Acc, Acc) :- !.
'$uri_rds'([0'/|In], Acc, Out) :- !, '$uri_until'(In, [0'/], Seg, Rest), '$uri_rds'(Rest, [[0'/|Seg]|Acc], Out).
'$uri_rds'(In, Acc, Out) :- '$uri_until'(In, [0'/], Seg, Rest), '$uri_rds'(Rest, [Seg|Acc], Out).
'$uri_pop'([], []).
'$uri_pop'([_|Acc], Acc).
uri_normalized(U, N) :- '$uri_norm'(uri, U, N).
uri_normalized(U, B, N) :- uri_resolve(U, B, A), uri_normalized(A, N).
iri_normalized(U, N) :- '$uri_norm'(iri, U, N).
iri_normalized(U, B, N) :- uri_resolve(U, B, A), iri_normalized(A, N).
uri_normalized_iri(U, N) :- '$uri_norm'(niri, U, N).
uri_normalized_iri(U, B, N) :- uri_resolve(U, B, A), uri_normalized_iri(A, N).
'$uri_norm'(How, U, N) :-
    '$uri_codes'(U, Cs), '$uri_split'(Cs, S0, A0, P0, Q0, F0),
    ( var(S0) -> S = S0 ; downcase_atom(S0, S) ),
    ( var(A0) -> A = A0 ; '$uri_lowhost'(A0, A) ),
    atom_codes(P0, PC0), '$uri_dots'(PC0, PC1), '$uri_esc'(How, PC1, PC), atom_codes(P, PC),
    '$uri_escpart'(How, Q0, Q), '$uri_escpart'(How, F0, F),
    '$uri_build_raw'(S, A, P, Q, F, NC), atom_codes(N, NC).
'$uri_lowhost'(A0, A) :- atom_codes(A0, Cs), ( '$uri_until'(Cs, [0'@], UI, [0'@|H]) -> '$uri_lower'(H, HL), append(UI, [0'@|HL], L) ; '$uri_lower'(Cs, L) ), atom_codes(A, L).
'$uri_lower'([], []).
'$uri_lower'([C|Cs], [L|Ls]) :- ( C >= 0'A, C =< 0'Z -> L is C + 32 ; L = C ), '$uri_lower'(Cs, Ls).
'$uri_escpart'(_, V, V) :- var(V), !.
'$uri_escpart'(How, V0, V) :- atom_codes(V0, C0), '$uri_esc'(How, C0, C), atom_codes(V, C).
'$uri_esc'(iri, Cs, Cs) :- !.
'$uri_esc'(How, Cs, Out) :- '$uri_escn'(How, Cs, Out).
'$uri_escn'(_, [], []).
'$uri_escn'(How, [0'%, H1, H2|Cs], Out) :- '$uri_hexval'(H1, V1), '$uri_hexval'(H2, V2), !, B is V1 * 16 + V2,
    ( B < 128, '$uri_unreserved'(B) -> Out = [B|Out1], Cs1 = Cs
    ; How == niri, B >= 0xC0, '$uri_seq'(B, N, V0), N > 0, '$uri_escoct'(N, Cs, Os, Cs1), '$uri_cont'(N, Os, V0, C, []) -> Out = [C|Out1]
    ; '$uri_hexdig'(V1, U1), '$uri_hexdig'(V2, U2), Out = [0'%, U1, U2|Out1], Cs1 = Cs ),
    '$uri_escn'(How, Cs1, Out1).
'$uri_escn'(How, [C|Cs], [C|Out]) :- '$uri_escn'(How, Cs, Out).
'$uri_escoct'(0, Cs, [], Cs) :- !.
'$uri_escoct'(N, [0'%, H1, H2|Cs], [o(B)|Os], Rest) :- '$uri_hexval'(H1, V1), '$uri_hexval'(H2, V2), B is V1 * 16 + V2, N1 is N - 1, '$uri_escoct'(N1, Cs, Os, Rest).
uri_file_name(U, F) :- nonvar(U), !, uri_components(U, uri_components(file, A, P, _, _)), ( var(A) -> true ; A == '' -> true ; A == localhost ), atom_codes(P, PC), '$uri_dec'(0, PC, FC), atom_codes(F, FC).
uri_file_name(U, F) :- '$uri_codes'(F, FC0), ( FC0 = [0'/|_] -> FC = FC0 ; absolute_file_name(F, Abs), atom_codes(Abs, FC) ), '$uri_enc'(path, FC, 0, PE), append([0'f, 0'i, 0'l, 0'e, 0':, 0'/, 0'/], PE, Cs), atom_codes(U, Cs).
uri_iri(U, I) :- nonvar(U), !, '$uri_codes'(U, Cs), '$uri_iridec'(Cs, Ds), atom_codes(I, Ds).
uri_iri(U, I) :- '$uri_codes'(I, Cs), '$uri_irienc'(Cs, Es), atom_codes(U, Es).
'$uri_iridec'([], []).
'$uri_iridec'([0'%, H1, H2|Cs], [C|Ds]) :- '$uri_hexval'(H1, V1), '$uri_hexval'(H2, V2), B is V1 * 16 + V2, B >= 0xC0, '$uri_seq'(B, N, V0), N > 0, '$uri_escoct'(N, Cs, Os, Cs1), '$uri_cont'(N, Os, V0, C, []), !, '$uri_iridec'(Cs1, Ds).
'$uri_iridec'([C|Cs], [C|Ds]) :- '$uri_iridec'(Cs, Ds).
'$uri_irienc'([], []).
'$uri_irienc'([C|Cs], Es) :- C >= 128, !, '$uri_utf8'(C, Bs), '$uri_pct'(Bs, Es, Es1), '$uri_irienc'(Cs, Es1).
'$uri_irienc'([C|Cs], [C|Es]) :- '$uri_irienc'(Cs, Es).
