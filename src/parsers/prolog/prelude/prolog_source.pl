/*  prolog_source.pl -- the one predicate of SWI-Prolog's library(prolog_source) that read_term's subterm_positions feeds: valid_term_position/2
    (hq_packages 2026-10-24, ceo CEO-1959, ARCH-PROLOG-DATABASE.md sec 11).  SCRIP's own text, the logic of swipl's library/prolog_source.pl
    valid_term_position/2,4 line for line (its single-sided-unification rules, its SWI position vocabulary: From-To, string_position,
    brace_term_position, list_position, term_position, dict_position, key_value_position, parentheses_term_position, quasi_quotation_position)
    with the helpers renamed '$vtp_'-style by hand and the det/1 declarations dropped.  The rest of library(prolog_source) -- the source reader,
    prolog_read_source_term and its hooks -- is not here.
*/
:- module(prolog_source, [valid_term_position/2]).
valid_term_position(Term, TermPos) :-
    '$vtp'(0, 0x7fffffffffffffff, Term, TermPos).
'$vtp'(OuterFrom, OuterTo, _Term, TermPos),
        var(TermPos),
        OuterFrom =< OuterTo => true.
'$vtp'(OuterFrom, OuterTo, Var, From-To),
        var(Var),
        '$vtp_ft'(OuterFrom, OuterTo, From, To) => true.
'$vtp'(OuterFrom, OuterTo, Atom, From-To),
        atom(Atom),
        '$vtp_ft'(OuterFrom, OuterTo, From, To) => true.
'$vtp'(OuterFrom, OuterTo, Number, From-To),
        number(Number),
        '$vtp_ft'(OuterFrom, OuterTo, From, To) => true.
'$vtp'(OuterFrom, OuterTo, [], From-To),
        '$vtp_ft'(OuterFrom, OuterTo, From, To) => true.
'$vtp'(OuterFrom, OuterTo, String, string_position(From,To)),
        (   string(String)
        ->  true
        ;   is_of_type(codes, String)
        ->  true
        ;   is_of_type(chars, String)
        ->  true
        ;   atom(String)
        ),
        '$vtp_ft'(OuterFrom, OuterTo, From, To) => true.
'$vtp'(OuterFrom, OuterTo, {Arg}, brace_term_position(From,To,ArgPos)),
        '$vtp_ft'(OuterFrom, OuterTo, From, To) =>
    '$vtp'(From, To, Arg, ArgPos).
'$vtp'(OuterFrom, OuterTo, [Hd|Tl], list_position(From,To,ElemsPos,none)),
        '$vtp_ft'(OuterFrom, OuterTo, From, To) =>
    '$vtp_tail'([Hd|Tl], _HdPart, []),
    maplist('$vtp_2', [Hd|Tl], ElemsPos).
'$vtp'(OuterFrom, OuterTo, [Hd|Tl], list_position(From, To, ElemsPos, TailPos)),
        '$vtp_ft'(OuterFrom, OuterTo, From, To) =>
    '$vtp_tail'([Hd|Tl], HdPart, Tail),
    maplist('$vtp_4'(From,To), HdPart, ElemsPos),
    '$vtp_2'(Tail, TailPos).
'$vtp'(OuterFrom, OuterTo, Term, term_position(From,To, FFrom,FTo,SubPos)),
        '$vtp_ft'(OuterFrom, OuterTo, From, To) =>
    compound_name_arguments(Term, Name, Arguments),
    '$vtp_2'(Name, FFrom-FTo),
    maplist('$vtp_4'(From,To), Arguments, SubPos).
'$vtp'(OuterFrom, OuterTo, Dict, dict_position(From,To,TagFrom,TagTo,KeyValuePosList)),
        '$vtp_ft'(OuterFrom, OuterTo, From, To) =>
    dict_pairs(Dict, Tag, Pairs),
    '$vtp_2'(Tag, TagFrom-TagTo),
    foldl('$vtp_dict'(From,To), Pairs, KeyValuePosList, []).
'$vtp'(OuterFrom, OuterTo, Term, parentheses_term_position(From,To,ContentPos)),
        '$vtp_ft'(OuterFrom, OuterTo, From, To) =>
    '$vtp'(From, To, Term, ContentPos).
'$vtp'(OuterFrom, OuterTo, _Term, quasi_quotation_position(From,To,SyntaxTerm,SyntaxPos,_ContentPos)),
        '$vtp_ft'(OuterFrom, OuterTo, From, To) =>
    '$vtp'(From, To, SyntaxTerm, SyntaxPos).
'$vtp_2'(Term, Pos) :-
    valid_term_position(Term, Pos).
'$vtp_4'(From, To, Term, Pos) :-
    '$vtp'(From, To, Term, Pos).
'$vtp_ft'(OuterFrom, OuterTo, From, To) :-
    integer(OuterFrom),
    integer(OuterTo),
    integer(From),
    integer(To),
    OuterFrom =< OuterTo,
    From =< To,
    OuterFrom =< From,
    To =< OuterTo.
'$vtp_dict'(OuterFrom, OuterTo, Key-Value, KeyValuePosList0, KeyValuePosList1) :-
    selectchk(key_value_position(From,To,SepFrom,SepTo,Key,KeyPos,ValuePos), KeyValuePosList0, KeyValuePosList1),
    '$vtp_ft'(OuterFrom, OuterTo, From, To),
    '$vtp_ft'(OuterFrom, OuterTo, SepFrom, SepTo),
    SepFrom >= OuterFrom,
    '$vtp'(From, SepFrom, Key, KeyPos),
    '$vtp'(SepTo, To, Value, ValuePos).
'$vtp_tail'([X|Xs], HdPart, Tail) =>
    HdPart = [X|HdPart2],
    '$vtp_tail'(Xs, HdPart2, Tail).
'$vtp_tail'(Tail0, HdPart, Tail) =>
    HdPart = [],
    Tail0 = Tail.
