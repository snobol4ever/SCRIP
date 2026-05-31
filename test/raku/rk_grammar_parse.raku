# rk_grammar_parse — G3-1: Grammar.parse($s) runs the TOP rule (anchored, whole-string).
# Match -> the matched text; no match -> Nil (stringifies empty). token = ratchet, ws insignificant.
grammar Greeting { token TOP { hello } }
say Greeting.parse("hello");
say Greeting.parse("hi");
grammar Digits { token TOP { \d+ } }
say Digits.parse("12345");
say Digits.parse("12a45");
