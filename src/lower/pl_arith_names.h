#ifndef PL_ARITH_NAMES_H
#define PL_ARITH_NAMES_H
#include <string.h>
static inline const char * pl_ax_suffix_of(const char * s, int ar) {
    if (!s) return (const char *)0;
    if (ar == 2) {
        if (!strcmp(s, "+")) return "add"; if (!strcmp(s, "-")) return "sub"; if (!strcmp(s, "*")) return "mul";
        if (!strcmp(s, "/")) return "div"; if (!strcmp(s, "//")) return "idiv"; if (!strcmp(s, "div")) return "idiv";
        if (!strcmp(s, "mod")) return "mod"; if (!strcmp(s, "rem")) return "rem"; if (!strcmp(s, "**")) return "fpow"; if (!strcmp(s, "^")) return "pow";
        if (!strcmp(s, "min")) return "min"; if (!strcmp(s, "max")) return "max"; if (!strcmp(s, "gcd")) return "gcd"; if (!strcmp(s, "xor")) return "xor";
        if (!strcmp(s, ">>")) return "shr"; if (!strcmp(s, "<<")) return "shl"; if (!strcmp(s, "/\\")) return "band"; if (!strcmp(s, "\\/")) return "bor";
        return (const char *)0;
    }
    if (ar == 1) {
        if (!strcmp(s, "-")) return "neg"; if (!strcmp(s, "+")) return "pos"; if (!strcmp(s, "abs")) return "abs"; if (!strcmp(s, "sign")) return "sign";
        if (!strcmp(s, "truncate")) return "trunc"; if (!strcmp(s, "integer")) return "intg"; if (!strcmp(s, "float")) return "flt";
        if (!strcmp(s, "floor")) return "floor"; if (!strcmp(s, "ceiling")) return "ceil"; if (!strcmp(s, "round")) return "round";
        if (!strcmp(s, "sqrt")) return "sqrt"; if (!strcmp(s, "msb")) return "msb"; if (!strcmp(s, "\\")) return "bnot"; if (!strcmp(s, "sin")) return "sin"; if (!strcmp(s, "cos")) return "cos";
        if (!strcmp(s, "atan")) return "atan"; if (!strcmp(s, "log")) return "log"; if (!strcmp(s, "exp")) return "exp";
        if (!strcmp(s, "float_integer_part")) return "fip"; if (!strcmp(s, "float_fractional_part")) return "ffp";
        return (const char *)0;
    }
    if (ar == 0) { if (!strcmp(s, "pi")) return "pi"; if (!strcmp(s, "e")) return "e"; return (const char *)0; }
    return (const char *)0;
}
#endif
