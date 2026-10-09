#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
extern "C++" int pas_elem_kind(void);
#define PE_LO 0
#define PE_HI 4
#define PE_DATA 32
#define PE_COLD 200
#define PE_FAIL 201
#define PE_DONE 202
#define PE_JOIN 203
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pe_core(const std::string & b0, const std::string & b8, const std::string & i0, const std::string & i8, const std::string & v0, const std::string & v8, int set) {
    return x86("mov", "rax", b0.c_str())
         + x86("cmp", "al", (long)DT_A)
         + x86("jne", L(PE_COLD))
         + x86("mov", "rsi", b8.c_str())
         + x86("test", "rsi", "rsi")
         + x86("jz", L(PE_COLD))
         + x86("mov", "rax", i0.c_str())
         + x86("cmp", "al", (long)DT_I)
         + x86("jne", L(PE_COLD))
         + x86("mov", "rcx", i8.c_str())
         + x86("mov", "eax", RDD("rsi", PE_LO))
         + x86("movsxd", "rax", "eax")
         + x86("mov", "edx", RDD("rsi", PE_HI))
         + x86("movsxd", "rdx", "edx")
         + x86("cmp", "rcx", "rax")
         + x86("jl", L(PE_FAIL))
         + x86("cmp", "rcx", "rdx")
         + x86("jg", L(PE_FAIL))
         + x86("sub", "rcx", "rax")
         + x86("shl", "rcx", 4L)
         + x86("mov", "rdx", RDQ("rsi", PE_DATA))
         + x86("add", "rcx", "rdx")
         + IF(!set, x86("mov", "rax", RDQ("rcx", 0)))
         + IF(!set, x86("mov", "rdx", RDQ("rcx", 8)))
         + IF(set, x86("mov", "rax", v0.c_str()))
         + IF(set, x86("mov", "rdx", v8.c_str()))
         + IF(set, x86("mov", RDQ("rcx", 0), "rax"))
         + IF(set, x86("mov", RDQ("rcx", 8), "rdx"))
         + IF(set, x86("mov", "rax", b0.c_str()))
         + IF(set, x86("mov", "rdx", b8.c_str()))
         + x86("jmp", L(PE_DONE))
         + x86("def", L(PE_FAIL))
         + x86("mov32", "eax", (long)DT_FAIL)
         + x86("xor", "edx", "edx")
         + x86("def", L(PE_DONE));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string pas_elem_zd_fast(void) {
    return x86("comment", "PAS-ELEM inline element access under ZD: guard, bounds, one indexed load or store, operands read in place")
         + x86("note", ZOPN(0))
         + x86("note", ZOPN(1))
         + IF(pas_elem_kind() == 2, x86("note", ZOPN(2)))
         + pe_core(ZOPQ(0, 0), ZOPQ(0, 8), ZOPQ(1, 0), ZOPQ(1, 8), pas_elem_kind() == 2 ? ZOPQ(2, 0) : "", pas_elem_kind() == 2 ? ZOPQ(2, 8) : "", pas_elem_kind() == 2)
         + x86("cmp", "al", (long)DT_FAIL)
         + x86_omega("je")
         + x86("note", ZRESN())
         + x86("mov", ZRES(0), "rax")
         + x86("note", ZRESN())
         + x86("mov", ZRES(8), "rdx")
         + x86_gamma()
         + x86("def", L(PE_COLD));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string pas_elem_flat_fast(int argbase, int resoff) {
    return x86("comment", "PAS-ELEM inline element access: guard, bounds, one indexed load or store")
         + pe_core(FRQ(argbase), FRQ(argbase + 8), FRQ(argbase + 16), FRQ(argbase + 24),
                   pas_elem_kind() == 2 ? FRQ(argbase + 32) : "", pas_elem_kind() == 2 ? FRQ(argbase + 40) : "", pas_elem_kind() == 2)
         + x86("mov", FRQ(resoff), "rax")
         + x86("mov", FRQ(resoff + 8), "rdx")
         + x86("jmp", L(PE_JOIN))
         + x86("def", L(PE_COLD));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string pas_elem_flat_join(void) { return x86("def", L(PE_JOIN)); }
