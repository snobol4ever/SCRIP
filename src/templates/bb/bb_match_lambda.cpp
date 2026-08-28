#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐⭐ IR_MATCH_LAMBDA -- the epsilon box of a pattern lambda (row lang-lambda-pattern-primitives, Lon 2026-08-28).
   `LAMBDA(expr)` / `Λ(expr)` is the IMMEDIATE form: when the cursor passes, the expression is evaluated; SNOBOL4
   failure of the expression fails the match at that point, success matches the null string and consumes nothing.
   ⛔ THE EXPRESSION IS NOT EVALUATED HERE, AND THAT IS THE WHOLE POINT OF THE ROW.  The lowerer splices the
   expression's own lowered subgraph into the pattern chain ahead of this box and wires its edges: expression
   success -> this box's alpha, expression failure -> the pattern's own omega.  So the expression is EMITTED CODE
   reached by a wired jump, exactly like every other box, and pays none of the `*fn()` callout ceremony that
   `TT_DEFER` pays -- that shape lowers to SNO$MKEXPR carrying the expression as a SOURCE STRING resolved BY NAME
   at match time, which is the cost centre this primitive was minted to delete.
   ⭐ SO THE BOX ITSELF IS DELIBERATELY EMPTY, AND ITS EMPTINESS IS LOAD-BEARING, NOT LAZINESS.  It exists to give
   the construct its four ports: the expression subgraph has gamma and omega of its own but no BETA, and without a
   real box a backtrack arriving here would land in the middle of an expression's code rather than on a port.
   Backtracking through an IMMEDIATE lambda must NOT re-run the expression (reference `_backend_pure.py` class
   Lambda-capital: it yields once and simply stops on re-entry), so beta recedes past it -- which is precisely
   what x86_beta_trampoline() does. Consuming nothing means no cursor arithmetic: r14 is untouched by design. */
std::string bb_match_lambda() {
    return x86("comment", "IR_MATCH_LAMBDA immediate — epsilon; the expression ran on the wired edge into alpha")
         + x86_alpha()
         + x86_gamma()
         + x86_beta_trampoline();
}
