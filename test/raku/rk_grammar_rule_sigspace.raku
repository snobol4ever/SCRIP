# rk_grammar_rule_sigspace — G3-2: `rule` :sigspace. Whitespace between atoms in a `rule` body becomes
# optional whitespace (\s*); `token`/`regex` strip it. So `rule TOP { <a> <b> }` matches with or without
# spaces; the token subrules stay strict. Match -> matched text; no match -> Nil (empty).
grammar G {
    token a   { foo }
    token b   { bar }
    rule  TOP { <a> <b> }
}
say G.parse("foo bar");
say G.parse("foobar");
say G.parse("foo   bar");
say G.parse("foo baz");
