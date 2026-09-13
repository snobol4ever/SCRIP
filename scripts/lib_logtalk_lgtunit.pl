% lib_logtalk_lgtunit.pl -- THE PLAIN-PROLOG SHIM FOR lgtunit'S TEST-HARNESS PREDICATES.
% Row prolog-logtalk-iso-3268-conformance-cases-have-no-runner (hq_R, 2026-09-12). Read by
% test_prolog_logtalk_suite.sh and prepended to every generated case program.
%
% WHY IT EXISTS. corpus/packages/prolog/logtalk_iso grades a Prolog system against ISO/IEC 13211-1 without
% needing Logtalk installed, because every tested goal sits inside {...}, Logtalk's escape to plain Prolog.
% ⛔ BUT THE SURROUNDING SCAFFOLDING IS NOT PLAIN PROLOG: 1141 of the 3617 cases call an lgtunit framework
% predicate as ^^name(...), and 450 of those BIND the variable that the case's own expectation then calls:
%
%     test(iso_cut_0_04, true(Assertion)) :-
%         ^^set_text_output(''), \+ {(twice(_), !, write('Forwards '), fail)},
%         ^^text_output_assertion('C Forwards ', Assertion).
%
% Strip the ^^ calls (the obvious move, and the one the spike made) and Assertion is unbound, so the
% expectation raises instantiation_error and the case grades RED. ⭐ THAT RED IS MANUFACTURED BY THE
% HARNESS AND IS INDISTINGUISHABLE ON THE BOARD FROM A REAL ISO DEFECT -- 450 of them, 12% of the suite,
% every one of them in the stream and term-output families we are least sure of. So the helpers are
% re-implemented here in plain ISO Prolog rather than deleted.
%
% ⛔⭐ EVERY SHIM CALL IS WRAPPED IN lgt_h/1 AND A FAILURE OR ERROR INSIDE THE SHIM IS NOT THE CASE'S FAULT.
% lgt_h/1 converts both into throw('$lgt_harness'(Why)), which the runner buckets as UNGRADED AND NAMES --
% never PASS, never FAIL. This is the whole discipline of the file: a harness that cannot set a case up
% must say so in its own voice, because a harness defect wearing the case's name is a false measurement of
% the language, and a false measurement is worse than a missing one (ARCH-PROGRAM-LEDGER: UNKNOWN is not
% ZERO). ⭐ The reverse direction matters just as much and is easy to miss: the shim must never SUCCEED
% vacuously either, which is why lgt_h/1 treats a silent failure as a harness error rather than letting
% the conjunction fail into a plausible `false` verdict.
%
% ⛔ WHAT IS DELIBERATELY NOT HERE, and why naming it beats faking it: the binary-stream family
% (set_binary_input/output, create_binary_file, check_binary_input, binary_output_assertion,
% clean_binary_*) and stream_position/1. Their cases are reported UNGRADED with the helper named, so the
% work owed is readable off the board instead of being buried in a red count.
%
% ⛔ THE CAPTURE IS A FILE, NOT A MEMORY BUFFER, and the restore is via a saved handle, NEVER via the alias
% `user_output`. Measured on scrip 2026-09-12: set_output(user_output) did NOT restore the default sink, so
% every later write -- including the runner's own verdict line -- went on landing in the capture file and
% the case scored fail with an empty board line. flush_output/1 called on an ALIAS likewise fails silently
% (not an error, a failure), which took a working shim to a silent no-output run. Both are why lgt_cap_out/1
% and lgt_prev_out/1 hold real stream handles.

:- dynamic(lgt_prev_out/1).
:- dynamic(lgt_prev_in/1).
:- dynamic(lgt_cap_out/1).
:- dynamic(lgt_cap_in/1).

% lgt_h(+Goal) -- run one harness step. Failure and error alike become a NAMED harness refusal.
lgt_h(G) :- ( catch(G, E, throw('$lgt_harness'(error(G, E)))) -> true ; throw('$lgt_harness'(failed(G))) ).

% Logtalk LIBRARY objects a case uses as a HELPER, never as the subject: list::member/2 is the standard
% library's member/2 and nothing about it is under test, so it is provided here and the message is rewritten
% to it. A message this shim does NOT provide leaves the case UNGRADED-and-named rather than parse-error RED.
lgt_member(X, [X| _]).
lgt_member(X, [_| T]) :- lgt_member(X, T).

lgt_read_chars(S, L) :- get_char(S, C), ( C == end_of_file -> L = [] ; L = [C|T], lgt_read_chars(S, T) ).
lgt_file_to_atom(F, A) :- open(F, read, S), lgt_read_chars(S, Cs), close(S), atom_chars(A, Cs).
lgt_file_to_atom(F, Opts, A) :- open(F, read, S, Opts), lgt_read_chars(S, Cs), close(S), atom_chars(A, Cs).
lgt_atom_to_file(F, A) :- open(F, write, S), write(S, A), close(S).
lgt_atom_to_file(F, Opts, A) :- open(F, write, S, Opts), write(S, A), close(S).

% ---- output capture ---------------------------------------------------------------------------------
lgt_close_out :- ( retract(lgt_cap_out(S)) -> catch(close(S), _, true) ; true ),
                 ( retract(lgt_prev_out(P)) -> catch(set_output(P), _, true) ; true ).
lgt_set_text_output(Contents) :-
    current_output(Prev), assertz(lgt_prev_out(Prev)),
    open('lgt_capture_out.txt', write, S), assertz(lgt_cap_out(S)),
    ( Contents == '' -> true ; write(S, Contents) ), set_output(S).
lgt_set_text_output(Alias, Contents) :- lgt_set_text_output(Alias, Contents, []).
lgt_set_text_output(Alias, Contents, Opts) :-
    open('lgt_capture_out.txt', write, S, [alias(Alias)|Opts]), assertz(lgt_cap_out(S)),
    ( Contents == '' -> true ; write(S, Contents) ).
% lgtunit's text_output_contents/1 yields a LIST OF CHARACTERS, not an atom -- every caller in the suite
% asserts over it with subsumes(['1','.','0'| _], Contents), which an atom can never satisfy. The atom form
% belongs to the ASSERTION helpers, which compare against the expected atom, so the two are kept apart here.
lgt_text_output_chars(Cs) :- lgt_close_out, open('lgt_capture_out.txt', read, S), lgt_read_chars(S, Cs), close(S).
lgt_text_output_contents(Contents) :- lgt_text_output_chars(Contents).
lgt_text_output_contents(_Alias, Contents) :- lgt_text_output_chars(Contents).
lgt_text_output_assertion(Expected, Assertion) :-
    lgt_text_output_chars(Cs), atom_chars(Text, Cs), Assertion = (Text == Expected).
lgt_text_output_assertion(Alias, Expected, Assertion) :-
    lgt_text_output_contents(Alias, Cs), atom_chars(Text, Cs), Assertion = (Text == Expected).
lgt_text_output_assertion(_Alias, Expected, Opts, Assertion) :-
    lgt_close_out, lgt_file_to_atom('lgt_capture_out.txt', Opts, Text), Assertion = (Text == Expected).
lgt_check_text_output(Expected) :- lgt_text_output_chars(Cs), atom_chars(Text, Cs), Text == Expected.
lgt_check_text_output(Alias, Expected) :- lgt_text_output_contents(Alias, Cs), atom_chars(Text, Cs), Text == Expected.
lgt_suppress_text_output :- lgt_set_text_output('').
lgt_clean_text_output :- lgt_close_out, catch(lgt_delete('lgt_capture_out.txt'), _, true).

% ---- input ------------------------------------------------------------------------------------------
lgt_close_in :- ( retract(lgt_cap_in(S)) -> catch(close(S), _, true) ; true ),
                ( retract(lgt_prev_in(P)) -> catch(set_input(P), _, true) ; true ).
lgt_set_text_input(Contents) :-
    lgt_atom_to_file('lgt_capture_in.txt', Contents),
    current_input(Prev), assertz(lgt_prev_in(Prev)),
    open('lgt_capture_in.txt', read, S), assertz(lgt_cap_in(S)), set_input(S).
lgt_set_text_input(Alias, Contents) :- lgt_set_text_input(Alias, Contents, []).
lgt_set_text_input(Alias, Contents, Opts) :-
    lgt_atom_to_file('lgt_capture_in.txt', Opts, Contents),
    open('lgt_capture_in.txt', read, S, [alias(Alias)|Opts]), assertz(lgt_cap_in(S)).
% ⛔ check_text_input reads what is LEFT on the stream -- it is the suite's way of asserting how far a
% reader consumed, so it must NOT re-open the file from the start.
lgt_check_text_input(Expected) :- current_input(S), lgt_read_chars(S, Cs), atom_chars(Text, Cs), Text == Expected.
lgt_check_text_input(Alias, Expected) :- lgt_read_chars(Alias, Cs), atom_chars(Text, Cs), Text == Expected.
lgt_text_input_assertion(Expected, Assertion) :-
    current_input(S), lgt_read_chars(S, Cs), atom_chars(Text, Cs), Assertion = (Text == Expected).
lgt_text_input_assertion(Alias, Expected, Assertion) :-
    lgt_read_chars(Alias, Cs), atom_chars(Text, Cs), Assertion = (Text == Expected).
lgt_clean_text_input :- lgt_close_in, catch(lgt_delete('lgt_capture_in.txt'), _, true).

% ---- files ------------------------------------------------------------------------------------------
% ⛔ file_path/2 resolves against the CASE'S OWN DIRECTORY, which the runner copies into the per-case
% working directory -- so the plain name IS the path. Never an absolute path built here: a case that
% creates a file must not be able to write outside its own scratch directory.
lgt_file_path(Name, Name).
lgt_create_text_file(File, Contents) :- lgt_atom_to_file(File, Contents).
lgt_check_text_file(File, Expected) :- lgt_file_to_atom(File, Text), Text == Expected.
lgt_clean_file(File) :- catch(lgt_delete(File), _, true).
lgt_delete(File) :- ( catch(delete_file(File), _, fail) -> true ; true ).

% ---- streams ----------------------------------------------------------------------------------------
% A CLOSED stream handle, which is what the permission_error/existence_error cases need to be handed.
lgt_closed_output_stream(S, Opts) :- open('lgt_closed.txt', write, S, Opts), close(S).
lgt_closed_input_stream(S, Opts) :- lgt_atom_to_file('lgt_closed.txt', ''), open('lgt_closed.txt', read, S, Opts), close(S).

% ---- term relations ---------------------------------------------------------------------------------
% ⛔ NOT subsumes_term/2 AND NOT =@=/2 -- neither exists in this engine (existence_error, measured
% 2026-09-12), and a shim that leans on a builtin the system under test is missing grades the HARNESS.
% Both are defined here from copy_term/2 + numbervars/3, which are present, so the relation the suite
% asserts is computed the same way whatever the engine is missing.
lgt_variant(A, B) :- copy_term(A, A1), copy_term(B, B1), numbervars(A1, 0, N), numbervars(B1, 0, M), N == M, A1 == B1.
lgt_subsumes(General, Specific) :- \+ \+ ( copy_term(Specific, S1), numbervars(S1, 0, _), General = S1 ).
lgt_assertion(G) :- call(G).
lgt_assertion(_Label, G) :- call(G).
