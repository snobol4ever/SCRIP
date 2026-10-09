#include <string>
#include <cstdint>
#include <cstring>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_gva_active;
DESCR_t NV_SET_fn(const char * name, DESCR_t val);
DESCR_t NV_GET_fn(const char * name);
struct DESCR_t rt_keyword_pos_set(struct DESCR_t v);
struct DESCR_t rt_keyword_pos_set_strict(struct DESCR_t v);
typedef struct { uint64_t ptr; uint64_t len; } RevKwSubjRegs_t;
RevKwSubjRegs_t rt_keyword_subject_set(uint64_t lo, uint64_t hi);
RevKwSubjRegs_t rt_keyword_subject_set_strict(uint64_t lo, uint64_t hi);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RAG_BAD() (!(_.op_a_slot >= 0 && _.op_sc >= 0 && _.op_off >= 0))
#define RAG_KW() ((_.op_sval && !strcmp(_.op_sval, "&pos")) || (_.op_sval && !strcmp(_.op_sval, "&subject")))
std::string bb_rev_assign_global() {
    return IF(RAG_BAD(), x86_alpha() + x86_bomb("bb_rev_assign_global: g<-v needs rhs slot + save slot + own slot"))
         + IF(!RAG_BAD() && RAG_KW(),
               x86("comment", "IR_REV_ASSIGN &pos/&subject <- v: the keyword is set through its setter (the cursor and subject registers follow, an out-of-range &pos fails) and restored on recede")
             + x86_alpha()
             + IF((_.op_sval && !strcmp(_.op_sval, "&pos")), x86("mov", FRQ(_.op_sc), (long)DT_I)
                      + x86("mov", "rax", "r14")
                      + x86("add", "rax", (long)1)
                      + x86("mov", FRQ(_.op_sc + 8), "rax"))
             + IF((_.op_sval && !strcmp(_.op_sval, "&subject")), x86("mov", "rdi", ROQ(0))
                       + x86("call", "NV_GET_fn", (uint64_t)(uintptr_t)(void *)NV_GET_fn)
                       + x86("mov", FRQ(_.op_sc), "rax")
                       + x86("mov", FRQ(_.op_sc + 8), "rdx")
                       + x86_rt_gc_poll())
             + x86("mov", "rdi", FRQ(_.op_a_slot))
             + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
             + IF((_.op_sval && !strcmp(_.op_sval, "&pos")),
                  x86("call", (_.op_strict ? "rt_keyword_pos_set_strict" : "rt_keyword_pos_set"), (uint64_t)(uintptr_t)(void *)(_.op_strict ? rt_keyword_pos_set_strict : rt_keyword_pos_set))
                      + x86("cmp", "al", (long)DT_FAIL)
                      + x86_omega("je")
                      + x86("mov", FRQ(_.op_off), "rax")
                      + x86("mov", FRQ(_.op_off + 8), "rdx")
                      + x86_rt_gc_poll()
                      + x86("mov", "r14", "rdx")
                      + x86("sub", "r14", (long)1))
             + IF((_.op_sval && !strcmp(_.op_sval, "&subject")), x86("call", (_.op_strict ? "rt_keyword_subject_set_strict" : "rt_keyword_subject_set"),
                             (uint64_t)(uintptr_t)(void *)(_.op_strict ? rt_keyword_subject_set_strict : rt_keyword_subject_set))
                       + x86("test", "rax", "rax")
                       + x86_omega("je")
                       + x86("mov", FRQ(_.op_off), (long)DT_S)
                       + x86("mov", FRQ(_.op_off + 8), "rax")
                       + x86_rt_gc_poll()
                       + x86("mov", "rax", FRQ(_.op_off + 8))
                       + x86("mov", "r13", "rax")
                       + x86("mov", "r15", "rdx")
                       + x86("mov", "r14", (long)0))
             + x86_gamma()
             + x86_beta()
             + IF((_.op_sval && !strcmp(_.op_sval, "&pos")), x86("mov", "r14", FRQ(_.op_sc + 8))
                      + x86("sub", "r14", (long)1)
                      + x86_scan_sync_out_force())
             + IF((_.op_sval && !strcmp(_.op_sval, "&subject")), x86("mov", "rdi", FRQ(_.op_sc))
                       + x86("mov", "rsi", FRQ(_.op_sc + 8))
                       + x86("call", (_.op_strict ? "rt_keyword_subject_set_strict" : "rt_keyword_subject_set"),
                             (uint64_t)(uintptr_t)(void *)(_.op_strict ? rt_keyword_subject_set_strict : rt_keyword_subject_set))
                       + x86("mov", FRQ(_.op_off), (long)DT_S)
                       + x86("mov", FRQ(_.op_off + 8), "rax")
                       + x86_rt_gc_poll()
                       + x86("mov", "rax", FRQ(_.op_off + 8))
                       + x86("mov", "r13", "rax")
                       + x86("mov", "r15", "rdx")
                       + x86("mov", "r14", (long)0))
             + x86_omega()
             + IF((_.op_sval && !strcmp(_.op_sval, "&subject")), x86("def", L(0))
                       + x86(".quad", LS(0), _.op_sval)
                       + x86("label", LS(0))
                       + x86(".string", _.op_sval)))
         + IF(!RAG_BAD() && !RAG_KW(),
               IF(g_gva_active && _.op_gva_k >= 0,
              x86("comment", "IR_REV_ASSIGN g<-v gva: save the global into the frame save slot, store the rhs, restore on recede")
            + x86_alpha()
            + x86("note", gva_note(_.op_gva_k))
            + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 0) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 0))
            + x86("note", gva_note(_.op_gva_k))
            + x86("mov", "rdx", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8))
            + x86("mov", FRQ(_.op_sc), "rax")
            + x86("mov", FRQ(_.op_sc + 8), "rdx")
            + x86("mov", "rcx", FRQ(_.op_a_slot))
            + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
            + x86("note", gva_note(_.op_gva_k))
            + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 0) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 0), "rcx")
            + x86("note", gva_note(_.op_gva_k))
            + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8), "rsi")
            + x86("mov", FRQ(_.op_off), "rcx")
            + x86("mov", FRQ(_.op_off + 8), "rsi")
            + x86_gamma()
            + x86_beta()
            + x86("mov", "rax", FRQ(_.op_sc))
            + x86("mov", "rdx", FRQ(_.op_sc + 8))
            + x86("note", gva_note(_.op_gva_k))
            + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 0) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 0), "rax")
            + x86("note", gva_note(_.op_gva_k))
            + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8), "rdx")
            + x86_omega())
         + IF(!(g_gva_active && _.op_gva_k >= 0),
              x86("comment", "IR_REV_ASSIGN g<-v by name: NV_GET to save, NV_SET to store, NV_SET again on recede")
            + x86_alpha()
            + x86("mov", "rdi", ROQ(0))
            + x86("call", "NV_GET_fn", (uint64_t)(uintptr_t)(void *)NV_GET_fn)
            + x86("mov", FRQ(_.op_sc), "rax")
            + x86("mov", FRQ(_.op_sc + 8), "rdx")
            + x86_rt_gc_poll()
            + x86("mov", "rsi", FRQ(_.op_a_slot))
            + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
            + x86("mov", "rdi", ROQ(0))
            + x86("rtcc_wb")
            + x86("call_bare", "NV_SET_fn", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(const char *, DESCR_t))NV_SET_fn)
            + x86("rtcc_rl")
            + x86_rt_gc_poll()
            + x86("mov", "rcx", FRQ(_.op_a_slot))
            + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
            + x86("mov", FRQ(_.op_off), "rcx")
            + x86("mov", FRQ(_.op_off + 8), "rsi")
            + x86_gamma()
            + x86_beta()
            + x86("mov", "rsi", FRQ(_.op_sc))
            + x86("mov", "rdx", FRQ(_.op_sc + 8))
            + x86("mov", "rdi", ROQ(0))
            + x86("rtcc_wb")
            + x86("call_bare", "NV_SET_fn", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(const char *, DESCR_t))NV_SET_fn)
            + x86("rtcc_rl")
            + x86_rt_gc_poll()
            + x86_omega()
            + x86("def", L(0))
            + x86(".quad", LS(0), _.op_sval)
            + x86("label", LS(0))
            + x86(".string", _.op_sval)));
}
