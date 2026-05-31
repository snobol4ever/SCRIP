# rk_grammar_subrule — G3-2 first milestone: <subrule> references compose via registry expansion.
# Single subrule, concatenation <a><b>, and alternation-in-subrule grouping (foo|bar). token = ws
# insignificant, anchored whole-string. Match -> matched text; no match -> Nil (stringifies empty).
grammar Greet {
    token hi  { hello }
    token TOP { <hi> }
}
say Greet.parse("hello");
say Greet.parse("nope");
grammar Seq {
    token a   { foo }
    token b   { bar }
    token TOP { <a> <b> }
}
say Seq.parse("foobar");
say Seq.parse("fooqux");
grammar Alt {
    token x   { cat | dog }
    token TOP { <x> s }
}
say Alt.parse("cats");
say Alt.parse("dogs");
say Alt.parse("fishs");
