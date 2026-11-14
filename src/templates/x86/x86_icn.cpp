#include <cstdint>
#include <string>
#include "templates/x86/x86_icn.h"
#include "emitter/emit_state_em_03_sh_api.h"
#include "templates/x86/x86_gc.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_ops.h"
#include "ir/descr.h"
#include "ir/descr_tags_dt_platom.h"
#include "ir/ir_graph.h"
extern "C" uint64_t *rt_co_env(void);
extern "C" int64_t rt_cvpos_pos(DESCR_t v, int64_t len);
extern "C" int core_icn_int_ok_d(DESCR_t v);
extern "C" int core_icn_error(int code, DESCR_t v);
extern "C" IcnSubjRegs_t rt_icn_subject_coerce(uint64_t lo, uint64_t hi, uint64_t mode);
extern "C" WindowPos rt_icn_window_pos(uint64_t lo, uint64_t hi);
static std::string x86_scan_window_pos(int slot, int is_j, int k, int kfail);
std::string x86_scan_env_store() {
    return x86("call", "rt_co_env", (uint64_t)(uintptr_t)(void *)rt_co_env)
         + x86("mov", "[rax + 0]", "r13")
         + x86("mov", "[rax + 8]", "r14")
         + x86("mov", "[rax + 16]", "r15");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_scan_env_load_rr() {
    return x86("push", "rax") + x86("push", "rdx")
         + x86("call", "rt_co_env", (uint64_t)(uintptr_t)(void *)rt_co_env)
         + x86("mov", "r14", "[rax + 8]")
         + x86("mov", RDQ("rax", 0), 0L)
         + x86("mov", RDQ("rax", 8), 0L)
         + x86("mov", RDQ("rax", 16), 0L)
         + x86("pop", "rdx") + x86("pop", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_gen_regime() {
    return g_emit_cfg && g_emit_cfg->cell_frame;
}
std::string x86_icn_pos_set(int a, int strict, int k) {
    return x86("mov", "rax", FRQ(a))
         + x86("cmp", "eax", (long)DT_I)
         + x86("jne", L(k))
         + x86("mov", "rax", FRQ(a + 8))
         + x86("mov", "rcx", "r15")
         + x86("add", "rcx", 1L)
         + x86("cmp", "rax", "rcx")
         + x86_omega("jg")
         + x86("mov", "rdx", "rax")
         + x86("add", "rdx", "r15")
         + x86_omega("js")
         + x86("test", "rax", "rax")
         + x86("jg", L(k + 1))
         + x86("add", "rax", "rcx")
         + x86("jmp", L(k + 1))
         + x86("def", L(k))
         + (strict ? x86("mov", "rdi", FRQ(a))
                   + x86("mov", "rsi", FRQ(a + 8))
                   + x86("call", "core_icn_int_ok_d", (uint64_t)(uintptr_t)(void *)core_icn_int_ok_d)
                   + x86_rt_gc_poll()
                   + x86("test", "eax", "eax")
                   + x86("jne", L(k + 2))
                   + x86("mov", "edi", 101L)
                   + x86("mov", "rsi", FRQ(a))
                   + x86("mov", "rdx", FRQ(a + 8))
                   + x86("call", "core_icn_error", (uint64_t)(uintptr_t)(void *)core_icn_error)
                   + x86_rt_gc_poll()
                   + x86_omega()
                   + x86("def", L(k + 2))
                 : std::string())
         + x86("mov", "rdi", FRQ(a))
         + x86("mov", "rsi", FRQ(a + 8))
         + x86("mov", "rdx", "r15")
         + x86("call", "rt_cvpos_pos", (uint64_t)(uintptr_t)(void *)rt_cvpos_pos)
         + x86("test", "rax", "rax")
         + x86_omega("je")
         + x86("def", L(k + 1))
         + x86("mov", "r14", "rax")
         + x86("sub", "r14", 1L);
}
std::string x86_icn_subject_set(int a, int mode, int k) {
    return x86("mov", "rax", FRQ(a))
         + x86("cmp", "eax", (long)DT_S)
         + x86("jne", L(k))
         + x86("shr", "rax", 32L)
         + x86("test", "eax", "eax")
         + x86("je", L(k))
         + x86("cmp", "eax", -1L)
         + x86("je", L(k))
         + x86("mov", "r13", FRQ(a + 8))
         + x86("mov", "r15d", "eax")
         + x86("jmp", L(k + 1))
         + x86("def", L(k))
         + x86("mov", "rdi", FRQ(a))
         + x86("mov", "rsi", FRQ(a + 8))
         + x86("mov", "edx", (long)mode)
         + x86("call", "rt_icn_subject_coerce", (uint64_t)(uintptr_t)(void *)rt_icn_subject_coerce)
         + x86_rt_gc_poll_rec_subject_new()
         + x86("test", "rax", "rax")
         + x86_omega("je")
         + x86("mov", "r13", "rax")
         + x86("mov", "r15", "rdx")
         + x86("def", L(k + 1))
         + x86("xor", "r14d", "r14d");
}
static std::string x86_scan_window_default(int is_j, int sa, int bank, int k) {
    return is_j ? x86("mov", "rax", "rcx")
                : x86("mov", "rax", 1L)
                + x86("mov", "rdx", FRQ(sa))
                + x86("cmp", "dl", (long)DT_SNUL)
                + x86("jne", L(k))
                + x86("mov", "rax", FRQ(bank + 16))
                + x86("add", "rax", 1L)
                + x86("def", L(k));
}
static std::string x86_scan_window_pos(int slot, int is_j, int k, int kfail, int sa, int bank) {
    if (slot < 0) return x86_scan_window_default(is_j, sa, bank, k + 8);
    return x86("mov", "rax", FRQ(slot))
         + x86("cmp", "al", (long)DT_SNUL)
         + x86("jne", L(k))
         + x86_scan_window_default(is_j, sa, bank, k + 8)
         + x86("jmp", L(k + 2))
         + x86("def", L(k))
         + x86("cmp", "al", (long)DT_I)
         + x86("jne", L(k + 1))
         + x86("mov", "rax", FRQ(slot + 8))
         + x86("jmp", L(k + 3))
         + x86("def", L(k + 1))
         + x86("mov", "rdi", FRQ(slot))
         + x86("mov", "rsi", FRQ(slot + 8))
         + x86("call", "rt_icn_window_pos", (uint64_t)(uintptr_t)(void *)rt_icn_window_pos)
         + x86_rt_gc_poll()
         + x86("test", "rdx", "rdx")
         + x86("je", L(kfail))
         + x86("mov", "rcx", "r15")
         + x86("add", "rcx", 1L)
         + x86("def", L(k + 3))
         + x86("test", "rax", "rax")
         + x86("jg", L(k + 4))
         + x86("add", "rax", "rcx")
         + x86("def", L(k + 4))
         + x86("cmp", "rax", 1L)
         + x86("jl", L(kfail))
         + x86("cmp", "rax", "rcx")
         + x86("jg", L(kfail))
         + x86("def", L(k + 2));
}
std::string x86_scan_window(int wi, int wj, int bank, int k, int sa) {
    return x86("mov", "rcx", "r15")
         + x86("add", "rcx", 1L)
         + x86_scan_window_pos(wi, 0, k, k + 5, sa, bank)
         + x86("mov", "r14", "rax")
         + x86("mov", "rcx", "r15")
         + x86("add", "rcx", 1L)
         + x86_scan_window_pos(wj, 1, k + 10, k + 5, sa, bank)
         + x86("mov", "rdx", "rax")
         + x86("mov", "rax", "r14")
         + x86("cmp", "rax", "rdx")
         + x86("jle", L(k + 6))
         + x86("mov", "rcx", "rax")
         + x86("mov", "rax", "rdx")
         + x86("mov", "rdx", "rcx")
         + x86("def", L(k + 6))
         + x86("sub", "rax", 1L)
         + x86("mov", "r14", "rax")
         + x86("sub", "rdx", 1L)
         + x86("mov", "r15", "rdx")
         + x86("jmp", L(k + 7))
         + x86("def", L(k + 5))
         + x86("mov", "r13", FRQ(bank + 8))
         + x86("mov", "r14", FRQ(bank + 16))
         + x86("mov", "r15", FRQ(bank + 24))
         + x86_omega()
         + x86("def", L(k + 7));
}
std::string x86_icn_subject_result(int off) {
    return x86("mov", "eax", "r15d")
         + x86("shl", "rax", 32L)
         + x86("or", "rax", (long)DT_S)
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "r13");
}
