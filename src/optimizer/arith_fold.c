#include "arith_fold.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
_Static_assert(1, "GOAL-OPTIMIZER.md:40: parked out of the build, references GZ#5-amputated IR_ARITH/IR_ATOM/IR_LOGICVAR; re-seat only if those opcodes return");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int gz_arith_const_eval(const IR_t *nd, long *out) {
    if (!nd) return 0;
    if (nd->op == IR_LIT_INTEGER) { *out = (long)IR_LIT(nd).ival; return 1; }
    const IR_t *a0 = ir_pair_arg(nd, 0), *a1 = ir_pair_arg(nd, 1);
    if (nd->op != IR_ARITH || !a0) return 0;
    const char *op = IR_LIT(nd).sval ? IR_LIT(nd).sval : "+";
    if (!a1) {
        long a = 0;
        if (!gz_arith_const_eval(a0, &a)) return 0;
        if (strcmp(op,"-")==0) { *out = -a; return 1; }
        if (strcmp(op,"+")==0) { *out =  a; return 1; }
        if (strcmp(op,"abs")==0) { *out = (a<0)?-a:a; return 1; }
        if (strcmp(op,"sign")==0) { *out = (a>0)?1:(a<0)?-1:0; return 1; }
        if (strcmp(op,"truncate")==0||strcmp(op,"integer")==0) { *out = a; return 1; }
        if (strcmp(op,"msb")==0) { if (a<=0) return 0; int r=0; long v=a; while(v>1){v>>=1;r++;} *out=r; return 1; }
        return 0;
    }
    long a = 0, b = 0;
    if (!gz_arith_const_eval(a0, &a) || !gz_arith_const_eval(a1, &b)) return 0;
    if (strcmp(op,"+")==0) { *out = a+b; return 1; }
    if (strcmp(op,"-")==0) { *out = a-b; return 1; }
    if (strcmp(op,"*")==0) { *out = a*b; return 1; }
    if (strcmp(op,"/")==0) { if (!b) return 0; *out = a/b; return 1; }
    if (strcmp(op,"//")==0) { if (!b) return 0; *out = a/b; return 1; }
    if (strcmp(op,"mod")==0||strcmp(op,"rem")==0) { if (!b) return 0; *out = a%b; return 1; }
    if (strcmp(op,"/\\")==0) { *out = a&b; return 1; }
    if (strcmp(op,"\\/")==0) { *out = a|b; return 1; }
    if (strcmp(op,"xor")==0) { *out = a^b; return 1; }
    if (strcmp(op,">>")==0) { *out = (b>=0&&b<64)?(a>>b):0; return 1; }
    if (strcmp(op,"<<")==0) { *out = (b>=0&&b<64)?(a<<b):0; return 1; }
    if (strcmp(op,"max")==0) { *out = (a>b)?a:b; return 1; }
    if (strcmp(op,"min")==0) { *out = (a<b)?a:b; return 1; }
    if (strcmp(op,"gcd")==0) { long x=a<0?-a:a,y=b<0?-b:b; while(y){long t=y;y=x%y;x=t;} *out=x; return 1; }
    if (strcmp(op,"^")==0||strcmp(op,"**")==0) { if (b<0) return 0; long r=1; for(long i=0;i<b;i++) r*=a; *out=r; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_arith_lit_coerce(const IR_t *nd, long *out) {
    if (!nd) return 0;
    if (nd->op == IR_LIT_INTEGER) { *out = (long)IR_LIT(nd).ival; return 1; }
    if (nd->op == IR_LIT_STRING) {
        const char *s = IR_LIT(nd).sval ? IR_LIT(nd).sval : ""; while (*s == ' ') s++; const char *p = s; if (*p == '+' || *p == '-') p++;
        const char *d = p; while (*d >= '0' && *d <= '9') d++; const char *e = d; while (*e == ' ') e++;
        if (*e == '\0') { *out = (d > p) ? strtol(s, (char **)0, 10) : 0L; return 1; }
        return 0;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int gz_arith_float_eval(const IR_t *nd, double *out) {
    if (!nd) return 0;
    if (nd->op == IR_LIT_REAL) { *out = IR_LIT(nd).dval; return 1; }
    if (nd->op == IR_LIT_INTEGER) { *out = (double)IR_LIT(nd).ival; return 1; }
    if (nd->op == IR_ATOM && IR_LIT(nd).sval) {
        const char *a = IR_LIT(nd).sval;
        if (strcmp(a,"pi")==0) { *out = 3.141592653589793; return 1; } if (strcmp(a,"e")==0) { *out = 2.718281828459045; return 1; }
        if (strcmp(a,"inf")==0||strcmp(a,"infinity")==0) { *out = 1.0/0.0; return 1; }
        return 0;
    }
    if (nd->op != IR_ARITH || !IR_LIT(nd).sval) return 0;
    const char *op = IR_LIT(nd).sval;
    const IR_t *a0 = ir_pair_arg(nd, 0), *a1 = ir_pair_arg(nd, 1);
    if (!a0 && !a1) {
        if (strcmp(op,"pi")==0) { *out = 3.141592653589793; return 1; }
        if (strcmp(op,"e")==0)  { *out = 2.718281828459045; return 1; }
        if (strcmp(op,"inf")==0||strcmp(op,"infinity")==0) { *out = 1.0/0.0; return 1; }
        return 0;
    }
    if (!a0) return 0;
    double a = 0.0;
    if (!gz_arith_float_eval(a0, &a)) return 0;
    if (!a1) {
        if (strcmp(op,"sqrt")==0)  { *out = sqrt(a);  return 1; }
        if (strcmp(op,"sin")==0)   { *out = sin(a);   return 1; }
        if (strcmp(op,"cos")==0)   { *out = cos(a);   return 1; }
        if (strcmp(op,"tan")==0)   { *out = tan(a);   return 1; }
        if (strcmp(op,"asin")==0)  { *out = asin(a);  return 1; }
        if (strcmp(op,"acos")==0)  { *out = acos(a);  return 1; }
        if (strcmp(op,"atan")==0)  { *out = atan(a);  return 1; }
        if (strcmp(op,"exp")==0)   { *out = exp(a);   return 1; }
        if (strcmp(op,"log")==0)   { if (a<=0) return 0; *out = log(a); return 1; }
        if (strcmp(op,"float")==0) { *out = a; return 1; }
        if (strcmp(op,"float_integer_part")==0) { *out = (a>=0)?floor(a):ceil(a); return 1; }
        if (strcmp(op,"float_fractional_part")==0) { double ip; *out = modf(a, &ip); return 1; }
        if (strcmp(op,"abs")==0)     { *out = a<0?-a:a; return 1; }
        if (strcmp(op,"sign")==0)    { *out = a>0?1.0:a<0?-1.0:0.0; return 1; }
        return 0;
    }
    double b = 0.0;
    if (!gz_arith_float_eval(a1, &b)) return 0;
    if (strcmp(op,"+")==0)   { *out = a+b; return 1; }
    if (strcmp(op,"-")==0)   { *out = a-b; return 1; }
    if (strcmp(op,"*")==0)   { *out = a*b; return 1; }
    if (strcmp(op,"/")==0)   { if (!b) return 0; *out = a/b; return 1; }
    if (strcmp(op,"**")==0||strcmp(op,"^")==0) { *out = pow(a,b); return 1; }
    if (strcmp(op,"atan")==0) { *out = atan2(a,b); return 1; }
    if (strcmp(op,"max")==0)  { *out = a>b?a:b; return 1; }
    if (strcmp(op,"min")==0)  { *out = a<b?a:b; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int gz_arith_var_plus_const(const IR_t *nd, int *var_slot, const char **op_out, long *c_out) {
    if (!nd) return 0;
    if (nd->op == IR_LOGICVAR) { *var_slot = (int)IR_LIT(nd).ival; *op_out = NULL; *c_out = 0; return 1; }
    const IR_t *p0 = ir_pair_arg(nd, 0), *p1 = ir_pair_arg(nd, 1);
    if (nd->op != IR_ARITH || !IR_LIT(nd).sval || !p0 || !p1) return 0;
    const char *op = IR_LIT(nd).sval;
    if (strcmp(op,"+")==0||strcmp(op,"-")==0||strcmp(op,"*")==0||strcmp(op,"mod")==0||strcmp(op,"rem")==0||
        strcmp(op,"/\\")==0||strcmp(op,"\\/")==0||strcmp(op,"xor")==0||strcmp(op,">>")==0||strcmp(op,"<<")==0||strcmp(op,"//")==0||strcmp(op,"div")==0||strcmp(op,"/")==0) {
        if (p0->op == IR_LOGICVAR && p1->op == IR_LIT_INTEGER) {
            *var_slot = (int)IR_LIT(p0).ival; *op_out = op; *c_out = (long)IR_LIT(p1).ival; return 1;
        }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int gz_arith_var_bivar(const IR_t *nd, int *slot1, int *slot2, const char **op_out) {
    const IR_t *b0 = ir_pair_arg(nd, 0), *b1 = ir_pair_arg(nd, 1);
    if (!nd || nd->op != IR_ARITH || !IR_LIT(nd).sval || !b0 || !b1) return 0;
    const char *op = IR_LIT(nd).sval;
    if (strcmp(op,"+")==0||strcmp(op,"-")==0||strcmp(op,"*")==0||strcmp(op,"mod")==0||strcmp(op,"rem")==0||
        strcmp(op,"/\\")==0||strcmp(op,"\\/")==0||strcmp(op,"xor")==0||strcmp(op,">>")==0||strcmp(op,"<<")==0||strcmp(op,"//")==0||strcmp(op,"div")==0||strcmp(op,"/")==0) {
        if (b0->op == IR_LOGICVAR && b1->op == IR_LOGICVAR) {
            *slot1 = (int)IR_LIT(b0).ival; *slot2 = (int)IR_LIT(b1).ival; *op_out = op; return 1;
        }
    }
    return 0;
}
