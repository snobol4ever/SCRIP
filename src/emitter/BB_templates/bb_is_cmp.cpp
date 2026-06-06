#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_is_cmp_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
            if (strcmp(fn, "is") == 0 && pBB->α && pBB->α->t == IR_LOGICVAR && pBB->β
                && (pBB->β->t == IR_ARITH || pBB->β->t == IR_LIT_I || pBB->β->t == IR_LIT_F
                    || pBB->β->t == IR_LOGICVAR || pBB->β->t == IR_ATOM)) {
                std::string b;
                /* sub rsp, 16              48 83 EC 10                                              */
                b += bytes(4, "\x48\x83\xEC\x10");
                /* mov rdi, lhs_bb          48 BF [8]                                                */
                b += bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)(void*)pBB->α);
                /* mov rsi, rhs_bb          48 BE [8]                                                */
                b += bytes(2, "\x48\xBE") + u64le((uint64_t)(uintptr_t)(void*)pBB->β);
                /* movabs rax, &rt_is_eval; call rax     48 B8 [8] FF D0                          */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_is_eval) + bytes(2, "\xFF\xD0");
                /* add rsp, 16              48 83 C4 10                                              */
                b += bytes(4, "\x48\x83\xC4\x10");
                /* test eax, eax            85 C0                                                   */
                b += bytes(2, "\x85\xC0");
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
            if (pBB->α && pBB->β &&
                (strcmp(fn,"==")==0   || strcmp(fn,"\\==")==0 || strcmp(fn,"@<")==0 || strcmp(fn,"@>")==0
              || strcmp(fn,"@=<")==0  || strcmp(fn,"@>=")==0
              || strcmp(fn,"=:=")==0  || strcmp(fn,"=\\=")==0 || strcmp(fn,"<")==0  || strcmp(fn,">")==0
              || strcmp(fn,"=<")==0   || strcmp(fn,"<=")==0   || strcmp(fn,">=")==0)) {
                IR_t *a0 = pBB->α, *a1 = pBB->β;
                int  k0 = (int)a0->t,    k1 = (int)a1->t;
                long i0 = (long)a0->ival, i1 = (long)a1->ival;
                const char *s0 = (k0 == IR_ATOM) ? a0->sval : NULL;
                const char *s1 = (k1 == IR_ATOM) ? a1->sval : NULL;
                int is_arith = (strcmp(fn,"=:=")==0 || strcmp(fn,"=\\=")==0 || strcmp(fn,"<")==0
                             || strcmp(fn,">")==0   || strcmp(fn,"=<")==0   || strcmp(fn,"<=")==0 || strcmp(fn,">=")==0);
                void *callee = is_arith ? (void*)rt_arith_cmp : (void*)rt_term_cmp;
                std::string b;
                /* sub rsp, 16              48 83 EC 10                                               */
                b += bytes(4, "\x48\x83\xEC\x10");
                /* mov rdi, op (fn name)    48 BF [8]                                                */
                b += bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)fn);
                /* mov esi, k0              BE [4]                                                   */
                b += bytes(1, "\xBE") + u32le((uint32_t)k0);
                /* mov rdx, i0              48 BA [8]                                                */
                b += bytes(2, "\x48\xBA") + u64le((uint64_t)i0);
                /* mov rcx, s0 (or xor ecx,ecx)   48 B9 [8] / 31 C9                                 */
                if (s0) b += bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)s0);
                else    b += bytes(2, "\x31\xC9");
                /* mov r8d, k1              41 B8 [4]                                                */
                b += bytes(2, "\x41\xB8") + u32le((uint32_t)k1);
                /* mov r9, i1               49 B9 [8]                                                */
                b += bytes(2, "\x49\xB9") + u64le((uint64_t)i1);
                /* mov rax, s1 (or xor eax,eax) ; mov [rsp+0], rax   48 B8[8]/31 C0 ; 48 89 04 24    */
                if (s1) b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)s1);
                else    b += bytes(2, "\x31\xC0");
                b += bytes(4, "\x48\x89\x04\x24");
                /* movabs rax, &callee; call rax   48 B8 [8] FF D0                                  */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)callee) + bytes(2, "\xFF\xD0");
                /* add rsp, 16              48 83 C4 10                                              */
                b += bytes(4, "\x48\x83\xC4\x10");
                /* test eax, eax            85 C0                                                   */
                b += bytes(2, "\x85\xC0");
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
    }
    if (MEDIUM_TEXT) {
    std::string succ_back = x86("ins2", "jmp", _.lbl_γ)
                          + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_γ);
    (void)succ_back;
        if (strcmp(fn, "is") == 0 && pBB->α && pBB->α->t == IR_LOGICVAR && pBB->β) {
            const IR_t *rhs = pBB->β;
            int is_const = rhs->t == IR_ATOM && rhs->sval
                        && (!strcmp(rhs->sval, "pi") || !strcmp(rhs->sval, "e"));
            int is_floaty = 0;
            if (rhs->t == IR_ARITH) {
                const char *rop = rhs->sval ? rhs->sval : "+";
                is_floaty = bb_op_floaty(rop)
                         || (rhs->α && rhs->α->t == IR_LIT_F)
                         || (rhs->β && rhs->β->t == IR_LIT_F);
            }
            if (is_const || is_floaty) {
                int      dst_slot = (int)pBB->α->ival;
                int      lk = -1, rk = -1;
                long     li = 0,  ri = 0;
                uint64_t lb = 0,  rb = 0;
                if (rhs->t == IR_ARITH) {
                    const IR_t *L = rhs->α, *R = rhs->β;
                    if (L) { lk = (int)L->t; li = (long)L->ival;
                             double d = (L->t == IR_LIT_F) ? L->dval : 0.0; memcpy(&lb, &d, 8); }
                    if (R) { rk = (int)R->t; ri = (long)R->ival;
                             double d = (R->t == IR_LIT_F) ? R->dval : 0.0; memcpy(&rb, &d, 8); }
                }
                std::string load_op = _.bb_op_lbl
                    ? x86("ins2", "lea rsi,", emit_fmt("[rip + %s]", _.bb_op_lbl))
                    : x86("ins2", "xor", "esi, esi");
                return hdr
                     + x86("ins2", "sub", "rsp, 8")
                     + x86("ins2", "mov edi,", emit_fmt("%d", dst_slot))
                     + load_op
                     + x86("ins2", "mov edx,", emit_fmt("%d", lk))
                     + x86("ins2", "mov rcx,", emit_fmt("%ld", li))
                     + x86("ins2", "mov rax,", emit_fmt("%llu", (unsigned long long)lb))
                     + x86("ins2", "movq", "xmm0, rax")
                     + x86("ins2", "mov r8d,", emit_fmt("%d", rk))
                     + x86("ins2", "mov r9,",  emit_fmt("%ld", ri))
                     + x86("ins2", "mov rax,", emit_fmt("%llu", (unsigned long long)rb))
                     + x86("ins2", "movq", "xmm1, rax")
                     + x86("ins2", "call", "rt_is_f@PLT")
                     + x86("ins2", "add", "rsp, 8")
                     + x86("ins2", "test", "eax, eax")
                     + x86("ins2", "je", _.lbl_ω)
                     + x86("ins2", "jmp", _.lbl_γ)
                     + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
        }
        if (strcmp(fn, "is") == 0 && pBB->α && pBB->β && pBB->β->t == IR_ARITH
            && pBB->α->t == IR_LOGICVAR && pBB->β->α && pBB->β->β) {
            /* V-2 (2026-05-27): `Var is L op R` for the common binary-arith RHS. Flatten operands at  */
            /* emit time into serializable scalars (kind+value) — NO cross-process IR_t pointers. The  */
            /* op string was interned by bb_prepare into _.bb_op_lbl. Call rt_is(dst_slot,     */
            /* op, lk, li, rk, ri) which evaluates + unifies; branch eax → γ (ok) / ω (fail). The       */
            /* template owns ALL bytes incl. both four-port jmps; rt_is carries no port logic.       */
            int   dst_slot = (int)pBB->α->ival;
            IR_t *L = pBB->β->α, *R = pBB->β->β;
            int   lk = (int)L->t, rk = (int)R->t;
            long  li = (long)L->ival, ri = (long)R->ival;
            std::string load_op = _.bb_op_lbl
                ? x86("ins2", "lea rsi,", emit_fmt("[rip + %s]", _.bb_op_lbl))
                : x86("ins2", "xor", "esi, esi");
            return hdr
                 + x86("ins2", "mov edi,", emit_fmt("%d", dst_slot))
                 + load_op
                 + x86("ins2", "mov edx,", emit_fmt("%d", lk))
                 + x86("ins2", "mov rcx,", emit_fmt("%ld", li))
                 + x86("ins2", "mov r8d,", emit_fmt("%d", rk))
                 + x86("ins2", "mov r9,",  emit_fmt("%ld", ri))
                 + x86("ins2", "call", "rt_is@PLT")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je", _.lbl_ω)
                 + x86("ins2", "jmp", _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if (strcmp(fn, "is") == 0 && pBB->α && pBB->β && pBB->β->t == IR_ARITH
            && pBB->α->t == IR_LIT_I && pBB->β->α && pBB->β->β) {
            long  lval = (long)pBB->α->ival;
            IR_t *L = pBB->β->α, *R = pBB->β->β;
            int   lk = (int)L->t, rk = (int)R->t;
            long  li = (long)L->ival, ri = (long)R->ival;
            std::string load_op = _.bb_op_lbl
                ? x86("ins2", "lea rsi,", emit_fmt("[rip + %s]", _.bb_op_lbl))
                : x86("ins2", "xor", "esi, esi");
            return hdr
                 + x86("ins2", "mov rdi,", emit_fmt("%ld", lval))
                 + load_op
                 + x86("ins2", "mov edx,", emit_fmt("%d", lk))
                 + x86("ins2", "mov rcx,", emit_fmt("%ld", li))
                 + x86("ins2", "mov r8d,", emit_fmt("%d", rk))
                 + x86("ins2", "mov r9,",  emit_fmt("%ld", ri))
                 + x86("ins2", "call", "rt_is_lint@PLT")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je", _.lbl_ω)
                 + x86("ins2", "jmp", _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if (strcmp(fn, "is") == 0 && pBB->α && pBB->β && pBB->β->t == IR_ARITH
            && pBB->α->t == IR_LOGICVAR && pBB->β->α && !pBB->β->β) {
            int   dst_slot = (int)pBB->α->ival;
            IR_t *L = pBB->β->α;
            int   lk = (int)L->t;
            long  li = (long)L->ival;
            std::string load_op = _.bb_op_lbl
                ? x86("ins2", "lea rsi,", emit_fmt("[rip + %s]", _.bb_op_lbl))
                : x86("ins2", "xor", "esi, esi");
            return hdr
                 + x86("ins2", "mov edi,", emit_fmt("%d", dst_slot))
                 + load_op
                 + x86("ins2", "mov edx,", emit_fmt("%d", lk))
                 + x86("ins2", "mov rcx,", emit_fmt("%ld", li))
                 + x86("ins2", "mov r8d,", "-1")     /* unary sentinel — rk=-1 */
                 + x86("ins2", "mov r9,",  "0")      /* ri=0 (unused for unary) */
                 + x86("ins2", "call", "rt_is@PLT")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je", _.lbl_ω)
                 + x86("ins2", "jmp", _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if (pBB->α && pBB->β
            && (pBB->α->t == IR_STRUCT || pBB->β->t == IR_STRUCT)
            && (strcmp(fn,"==")==0 || strcmp(fn,"\\==")==0 || strcmp(fn,"@<")==0 || strcmp(fn,"@>")==0
             || strcmp(fn,"@=<")==0 || strcmp(fn,"@>=")==0)) {
            char op_lbl[64]; strtab_label(op_lbl, sizeof op_lbl, fn);
            return hdr
                 + x86("ins2", "sub", "rsp, 16")
                 + emit_build_compound_term(pBB->α)
                 + x86("ins2", "mov", "qword ptr [rsp + 0], rax")
                 + emit_build_compound_term(pBB->β)
                 + x86("ins2", "mov", "rdx, rax")
                 + x86("ins2", "mov", "rsi, qword ptr [rsp + 0]")
                 + x86("ins2", "lea rdi,", emit_fmt("[rip + %s]", op_lbl))
                 + x86("ins2", "call", "rt_term_cmp_terms@PLT")
                 + x86("ins2", "add", "rsp, 16")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if (pBB->α && pBB->β &&
            (strcmp(fn,"==")==0   || strcmp(fn,"\\==")==0 || strcmp(fn,"@<")==0 || strcmp(fn,"@>")==0
          || strcmp(fn,"@=<")==0  || strcmp(fn,"@>=")==0
          || strcmp(fn,"=:=")==0  || strcmp(fn,"=\\=")==0 || strcmp(fn,"<")==0  || strcmp(fn,">")==0
          || strcmp(fn,"=<")==0   || strcmp(fn,"<=")==0   || strcmp(fn,">=")==0)) {
            IR_t *a0 = pBB->α, *a1 = pBB->β;
            int  k0 = (int)a0->t,    k1 = (int)a1->t;
            long i0 = (long)a0->ival, i1 = (long)a1->ival;
            char op_lbl[64]; strtab_label(op_lbl, sizeof op_lbl, fn);
            char s0lbl[64]; s0lbl[0] = 0;
            char s1lbl[64]; s1lbl[0] = 0;
            if (k0 == IR_ATOM && a0->sval) strtab_label(s0lbl, sizeof s0lbl, a0->sval);
            if (k1 == IR_ATOM && a1->sval) strtab_label(s1lbl, sizeof s1lbl, a1->sval);
            int is_arith = (strcmp(fn,"=:=")==0 || strcmp(fn,"=\\=")==0 || strcmp(fn,"<")==0
                         || strcmp(fn,">")==0   || strcmp(fn,"=<")==0   || strcmp(fn,"<=")==0 || strcmp(fn,">=")==0);
            const char *callee = is_arith ? "rt_arith_cmp@PLT" : "rt_term_cmp@PLT";
            return hdr
                 + x86("ins2", "sub", "rsp, 16")
                 + x86("ins2", "lea rdi,", emit_fmt("[rip + %s]", op_lbl))
                 + x86("ins2", "mov esi,",  emit_fmt("%d",  k0))
                 + x86("ins2", "mov rdx,",  emit_fmt("%ld", i0))
                 + (s0lbl[0] ? x86("ins2", "lea rcx,", emit_fmt("[rip + %s]", s0lbl)) : x86("ins2", "xor", "ecx, ecx"))
                 + x86("ins2", "mov r8d,",  emit_fmt("%d",  k1))
                 + x86("ins2", "mov r9,",   emit_fmt("%ld", i1))
                 + (s1lbl[0] ? x86("ins2", "lea rax,", emit_fmt("[rip + %s]", s1lbl)) : x86("ins2", "xor", "eax, eax"))
                 + x86("ins2", "mov", "qword ptr [rsp + 0], rax")
                 + x86("ins2", "call", callee)
                 + x86("ins2", "add", "rsp, 16")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
    }
    return std::string();
}
