#ifndef RK_OPNAME_H
#define RK_OPNAME_H
#include <stddef.h>
#include <string.h>
#include <stdio.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void rk_op_canon_base(const char *cat, const char *op, char *out, size_t n) {
    if (!out || n == 0) return; if (!cat) cat = "infix"; if (!op) op = "";
    int pos = snprintf(out, n, "R%s_", cat);
    for (const char *c = op; *c && pos + 2 < (int)n; c++) { pos += snprintf(out + pos, n - (size_t)pos, "%02x", (unsigned char)*c); }
    out[(pos < (int)n) ? pos : (int)n - 1] = 0;
}
#endif
