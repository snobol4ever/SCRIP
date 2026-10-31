/*  ansi_term.pl -- SWI-Prolog's ansi_format/3,4 as SCRIP runs it (cqo 2026-10-31, SWI library/test_ansi_term). The generator embeds it in
    LOCAL mode: no SWI reader, no renames, its helpers '$ansi_'-named by hand. It is parsed only when a program names one of its exports.
    THE MODEL is swipl 9.0.4's library(ansi_term) (/usr/lib/swi-prolog/library/ansi_term.pl): on a stream whose tty property is true while
    the color_term flag is true, ansi_format writes ESC [ <codes> m, the formatted text and ESC [ 0 m, each escape written without moving
    the stream's line position so a column stop (~t~4|) counts only the visible text; otherwise it is format/3. The SGR code table and
    the colour names are swipl's. Left out, as SCRIP has no message system to theme: the message_property/console_color class lookup
    (a class is its own attribute list), hyperlinks, ansi_get_color/2 and the message hooks. The table and the clause shapes are
    derived from swipl's file, whose notice follows.

    Part of SWI-Prolog. Author: Jan Wielemaker. Copyright (c) 2010-2023, VU University Amsterdam, CWI, Amsterdam,
    SWI-Prolog Solutions b.v. All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
    conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
       in the documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
    NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
    THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
    OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */
:- module(ansi_term, [ ansi_format/3, ansi_format/4 ]).

ansi_format(Attr, Format, Args) :- ansi_format(current_output, Attr, Format, Args).

ansi_format(Stream, Class, Format, Args) :-
    stream_property(Stream, tty(true)),
    current_prolog_flag(color_term, true),
    !,
    '$ansi_codes'(Class, Codes, []),
    atomic_list_concat(Codes, ;, Code),
    '$ansi_keep_lpos'(Stream, format(Stream, '\e[~wm', [Code])),
    format(Stream, Format, Args),
    '$ansi_keep_lpos'(Stream, format(Stream, '\e[0m', [])),
    flush_output(Stream).
ansi_format(Stream, _, Format, Args) :- format(Stream, Format, Args).

'$ansi_keep_lpos'(S, G) :- catch(line_position(S, L), _, fail), !, call(G), set_stream(S, line_position(L)).
'$ansi_keep_lpos'(_, G) :- call(G).

'$ansi_codes'(X, _, _) :- var(X), !, throw(error(instantiation_error, _)).
'$ansi_codes'([], C, C) :- !.
'$ansi_codes'([H|T], C0, C) :- !, '$ansi_codes'(H, C0, C1), '$ansi_codes'(T, C1, C).
'$ansi_codes'(A, C0, C) :- ( '$ansi_sgr'(A, Code) -> ( is_list(Code) -> '$ansi_app'(Code, C, C0) ; C0 = [Code|C] ) ; throw(error(domain_error(sgr_code, A), _)) ).

'$ansi_app'([], C, C).
'$ansi_app'([H|T], C, [H|R]) :- '$ansi_app'(T, C, R).

'$ansi_sgr'(reset, 0).
'$ansi_sgr'(bold, 1).
'$ansi_sgr'(faint, 2).
'$ansi_sgr'(italic, 3).
'$ansi_sgr'(underline, 4).
'$ansi_sgr'(blink(slow), 5).
'$ansi_sgr'(blink(rapid), 6).
'$ansi_sgr'(negative, 7).
'$ansi_sgr'(conceal, 8).
'$ansi_sgr'(crossed_out, 9).
'$ansi_sgr'(font(primary), 10) :- !.
'$ansi_sgr'(font(N), C) :- C is 10+N.
'$ansi_sgr'(fraktur, 20).
'$ansi_sgr'(underline(double), 21).
'$ansi_sgr'(intensity(normal), 22).
'$ansi_sgr'(fg(Name), C) :- ( '$ansi_color'(Name, N) -> C is N+30 ; '$ansi_rgb'(Name, R, G, B) -> '$ansi_sgr'(fg(R,G,B), C) ).
'$ansi_sgr'(bg(Name), C) :- !, ( '$ansi_color'(Name, N) -> C is N+40 ; '$ansi_rgb'(Name, R, G, B) -> '$ansi_sgr'(bg(R,G,B), C) ).
'$ansi_sgr'(framed, 51).
'$ansi_sgr'(encircled, 52).
'$ansi_sgr'(overlined, 53).
'$ansi_sgr'(ideogram(underline), 60).
'$ansi_sgr'(right_side_line, 60).
'$ansi_sgr'(ideogram(underline(double)), 61).
'$ansi_sgr'(right_side_line(double), 61).
'$ansi_sgr'(ideogram(overlined), 62).
'$ansi_sgr'(left_side_line, 62).
'$ansi_sgr'(ideogram(stress_marking), 64).
'$ansi_sgr'(-X, Code) :- '$ansi_off'(X, Code).
'$ansi_sgr'(hfg(Name), C) :- '$ansi_color'(Name, N), C is N+90.
'$ansi_sgr'(hbg(Name), C) :- !, '$ansi_color'(Name, N), C is N+100.
'$ansi_sgr'(fg8(Name), [38,5,N]) :- '$ansi_color8'(Name, N).
'$ansi_sgr'(bg8(Name), [48,5,N]) :- '$ansi_color8'(Name, N).
'$ansi_sgr'(fg(R,G,B), [38,2,R,G,B]) :- between(0, 255, R), between(0, 255, G), between(0, 255, B).
'$ansi_sgr'(bg(R,G,B), [48,2,R,G,B]) :- between(0, 255, R), between(0, 255, G), between(0, 255, B).

'$ansi_off'(italic_and_franktur, 23).
'$ansi_off'(underline, 24).
'$ansi_off'(blink, 25).
'$ansi_off'(negative, 27).
'$ansi_off'(conceal, 28).
'$ansi_off'(crossed_out, 29).
'$ansi_off'(framed, 54).
'$ansi_off'(overlined, 55).

'$ansi_color8'(h(Name), N) :- !, '$ansi_color'(Name, N0), N is N0+8.
'$ansi_color8'(Name, N) :- atom(Name), !, '$ansi_color'(Name, N).
'$ansi_color8'(N, N) :- between(0, 255, N).

'$ansi_color'(black, 0).
'$ansi_color'(red, 1).
'$ansi_color'(green, 2).
'$ansi_color'(yellow, 3).
'$ansi_color'(blue, 4).
'$ansi_color'(magenta, 5).
'$ansi_color'(cyan, 6).
'$ansi_color'(white, 7).
'$ansi_color'(default, 9).

'$ansi_rgb'(Name, R, G, B) :- atom_codes(Name, [0'#,R1,R2,G1,G2,B1,B2]), '$ansi_hex'(R1, R2, R), '$ansi_hex'(G1, G2, G), '$ansi_hex'(B1, B2, B).
'$ansi_rgb'(Name, R, G, B) :- atom_codes(Name, [0'#,R1,G1,B1]), '$ansi_hex'(R1, R), '$ansi_hex'(G1, G), '$ansi_hex'(B1, B).

'$ansi_hex'(D1, D2, V) :- code_type(D1, xdigit(V1)), code_type(D2, xdigit(V2)), V is 16*V1+V2.
'$ansi_hex'(D1, V) :- code_type(D1, xdigit(V1)), V is 16*V1+V1.
