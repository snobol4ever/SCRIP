# rk_grammar_dotws — G3-2: non-capturing subrule <.name> (here <.ws>). No Match tree yet, so <.name>
# expands like <name>; the point is the syntax + a user-defined ws token mediating inter-atom space.
grammar G {
    token ws  { \s }
    token a   { foo }
    token b   { bar }
    token TOP { <a> <.ws> <b> }
}
say G.parse("foo bar");
say G.parse("foobar");
