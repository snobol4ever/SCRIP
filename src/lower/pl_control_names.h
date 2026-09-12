#ifndef PL_CONTROL_NAMES_H
#define PL_CONTROL_NAMES_H
#include <string.h>
static inline int pl_pi_is_control(const char * nm, int ar) {
    static const char * const any_arity[] = { "call", "findall", "bagof", "setof", "forall", "catch", "throw", "halt", "assert", "asserta", "assertz",
        "retract", "retractall", "abolish", "clause", "current_predicate", "predicate_property", "dynamic", "discontiguous", "multifile", "initialization",
        "ensure_loaded", "include", "op", "current_op", "set_prolog_flag", "current_prolog_flag", "char_conversion", "current_char_conversion", "once",
        "ignore", "not", "setup_call_cleanup", "call_cleanup", "atom_to_term", "open", "close", "set_input", "set_output", "current_input", "current_output",
        "at_end_of_stream", "set_stream_position", "get_char", "get_code", "get_byte", "peek_char", "peek_code", "peek_byte", "put_char", "put_code", "put_byte",
        "write_term", "read_term", "read", "write", "writeq", "write_canonical", "print", "nl", "flush_output", "unify_with_occurs_check", "subsumes_term",
        "compare", "keysort", "sort", "msort", "length", "functor", "arg", "copy_term", "term_variables", "atom_codes", "atom_chars", "atom_length",
        "atom_concat", "sub_atom", "char_code", "number_codes", "number_chars", "var", "nonvar", "atom", "number", "integer", "float", "atomic", "compound",
        "callable", "is_list", "ground", "acyclic_term", "repeat", "true", "fail", "false", "is", "succ", "plus", "number_vars", "numbervars", "stream_property",
        "current_stream", "consult", "tab", NULL };
    static const char * const ops[] = { "!", ",", ";", "->", "*->", "\\+", ":-", "?-", "=", "\\=", "==", "\\==", "@<", "@=<", "@>", "@>=", "=..", "<", ">", "=<", ">=", "=:=", "=\\=", "|", NULL };
    if (!nm) return 0;
    (void) ar;
    for (int i = 0; any_arity[i]; i++) if (!strcmp(nm, any_arity[i])) return 1;
    for (int i = 0; ops[i]; i++) if (!strcmp(nm, ops[i])) return 1;
    return 0;
}
#endif
