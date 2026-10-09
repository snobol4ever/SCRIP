#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" long rt_cap_open(const char *varname, int saved_delta, int cur_delta, int is_imm);
extern "C" long rt_cap_open_plain(const char *varname, int saved_delta, int cur_delta, int is_imm);
extern "C" void *rt_proc_open_fn(void);
extern "C" DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0, long touched);
extern "C" DESCR_t rt_proc_call_epilogue_ω(long touched);
extern "C" long rt_cap_land_γ(DESCR_t frame0, long word, void *rec);
extern "C" long rt_cap_land_ω(long word, void *rec);
extern "C" long rt_cap_open_gva(DESCR_t *cell, int saved_delta, int cur_delta, const char *varname);
extern "C" int is_protected_pat_name(const char *name);
extern "C" int gva_name_hidden(const char *name);
extern "C" int g_gva_active;
extern "C" void * bb_dstar_rec_addr(const char * star);
extern "C" const char * bb_ab_sym_name(const char * nm);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define havehome() (_.op_zres || _.op_cap_anchor || _.op_off >= 0)
#define cap_name_plain() (_.op_sval && _.op_sval[0] && _.op_sval[0] != '*')
#define cap_open_sym() (cap_name_plain() ? "rt_cap_open_plain" : "rt_cap_open")
#define cap_open_fp() ( \
      cap_name_plain() \
    ? (const void *)(long (*)(const char *, int, int, int))rt_cap_open_plain \
    : (const void *)(long (*)(const char *, int, int, int))rt_cap_open \
)
#define cap_gva() (sn4_cap_gva() && g_gva_active && _.op_gva_k >= 0 && cap_name_plain() && !is_protected_pat_name(_.op_sval))
#define CAPO_RECORD_BYTES 64L
#define capo_carve() (cap_name_plain() ? std::string() : x86("sub", "rsp", CAPO_RECORD_BYTES))
#define capo_free() (cap_name_plain() ? std::string() : x86("add", "rsp", CAPO_RECORD_BYTES))
#define capo_arg(reg64, reg32) ( \
      cap_name_plain() \
    ? ((reg32) ? x86("xor", (reg32), (reg32)) : std::string()) \
    : x86("mov", (reg64), "rsp") \
)
#define gva_cell_addr(k) (std::string("[" RTCC_GVA_REG " + ") + std::to_string((k) * 16) + "]")
static std::string cap_imm_gva(const std::string & homeop) {
    return x86("comment", "IR_MATCH_CAPTURE_IMM gva")
         + x86_alpha()
         + x86("mov", "eax", homeop.c_str())
         + x86_anchor_enter()
         + x86("note", gva_note(_.op_gva_k))
         + ((g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? x86("lea", "rdi", gva_cell_addr(_.op_gva_k).c_str()) :
         x86("mov", "rdi", (long)(RT_GVA_VA + (unsigned long)_.op_gva_k * 16)))
         + x86("mov", "esi", "eax")
         + x86("mov", "edx", "r14d")
         + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)_.op_sval, x86_strtab_lbl(_.op_sval).c_str())
         + x86("call", "rt_cap_open_gva", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t *, int, int, const char *))rt_cap_open_gva)
         + x86_rt_gc_poll()
         + x86_anchor_leave()
         + x86_gamma()
         + x86_beta_trampoline();
}
static std::string cap_cond_target_rcx() {
    return IF(cap_gva() && gva_name_hidden(_.op_sval),
               x86("note", gva_note(_.op_gva_k))
                        + ((g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? x86("lea", "rcx", gva_cell_addr(_.op_gva_k).c_str())
                                                            : x86_movabs_r64("rcx", (uint64_t)(RT_GVA_VA + (unsigned long)_.op_gva_k * 16))))
         + IF(!(cap_gva() && gva_name_hidden(_.op_sval)) && (_.op_sval && _.op_sval[0] == '*' && _.op_sval[1]),
               x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)bb_dstar_rec_addr(_.op_sval), (std::string(".Ldstar_") + bb_ab_sym_name(_.op_sval)).c_str()))
         + IF(!(cap_gva() && gva_name_hidden(_.op_sval)) && !(_.op_sval && _.op_sval[0] == '*' && _.op_sval[1]),
               x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), x86_strtab_lbl((_.op_sval ? _.op_sval : "")).c_str()));
}
#define writehome() (_.op_zres ? ZRESD(0) : FR(_.op_off))
#define readhome() (_.op_zres ? ZOPD(1, 0) : FR(_.op_off))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_capture() {
    x86_begin();
    return (_.op_off < 0)
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_ASSIGN: capture stack slot not promoted (flat_drive_capture)") )
         : !(_.op_sval ? _.op_sval : "")[0]
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_ASSIGN: empty capture variable name") )
         : (int)_.op_phase == 0 && _.op_frame_need && _.op_cap_frame_off != -1
         ? ( x86("comment", "IR_MATCH_CAPTURE_SAVE")
           + x86_alpha()
           + x86("mov", CFC(0), "r14d")
           + x86_gamma()
           + x86_beta_trampoline() )
         : (int)_.op_phase == 0 && _.op_frame_need
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_SAVE: hazard crosses a DEFER-unsafe boundary but op_cap_frame_off is unavailable (no enclosing MATCH_BEGIN in scope, or SCRIP_MATCH_RBP=0) -- "
                      "CAPTURE never pushes its own activation frame (s88 revert: the s81/s83 own-transient-rbp arm crossed over with an ARBNO nested in its own span and corrupted the yield "
                      "cursor, D11). Honest refuse, matching bb_match_arbno.cpp's identical boundary bomb.")
           + x86_beta_trampoline() )
         : (int)_.op_phase == 0 && havehome()
         ? ( x86("comment", "IR_MATCH_CAPTURE_SAVE")
           + x86_alpha()
           + x86("mov", writehome(), "r14d")
           + x86_gamma()
           + x86_beta_trampoline() )
         : (int)_.op_phase == 0
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_SAVE: no home -- neither a ζ-SPINE cell (op_zres) nor a ζ-STANDING slot (frame_need_of: DEFER-hazard / ALT-arm "
                      "classes); classifier and ZD plan disagree on this node -- the legacy C rt_cap_push fallback is deliberately not rebuilt (s83)")
           + x86_beta_trampoline() )
         : (int)_.op_phase == 1 && _.op_sval && _.op_sval[0] == '*' && !sn4_nret_cap()
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_COND: computed-name (*VAR/NRETURN) target not yet rebuilt -- blocked on the :(NRETURN) lowering bug (s82), see this file's header comment")
           + x86_beta() )
         : (int)_.op_phase == 1 && _.op_frame_need && _.op_cap_frame_off != -1
         ? ( x86("comment", "IR_MATCH_CAPTURE_COND")
           + x86_alpha()
           + x86("mov", "eax", CFC(0))
           + cap_cond_target_rcx()
           + x86("mov", RDQ("r12", 0), "rcx")
           + x86("mov", "esi", "eax")
           + x86("mov", RDQ("r12", 8), "rsi")
           + x86("mov", "edx", "r14d")
           + x86("sub", "edx", "eax")
           + x86("mov", RDQ("r12", 16), "rdx")
           + x86("add", "r12", (long)24)
           + x86_gamma()
           + x86_beta()
           + x86("sub", "r12", (long)24)
           + x86_omega() )
         : (int)_.op_phase == 1 && _.op_frame_need
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_COND: hazard crosses a DEFER-unsafe boundary but op_cap_frame_off is unavailable -- CAPTURE never pushes its own activation frame (s88 revert), see "
                      "IR_MATCH_CAPTURE_SAVE's bomb for the full rationale.")
           + x86_beta() )
         : (int)_.op_phase == 1 && havehome()
         ? ( x86("comment", "IR_MATCH_CAPTURE_COND")
           + x86_alpha()
           + x86("mov", "eax", readhome())
           + cap_cond_target_rcx()
           + x86("mov", RDQ("r12", 0), "rcx")
           + x86("mov", "esi", "eax")
           + x86("mov", RDQ("r12", 8), "rsi")
           + x86("mov", "edx", "r14d")
           + x86("sub", "edx", "eax")
           + x86("mov", RDQ("r12", 16), "rdx")
           + x86("add", "r12", (long)24)
           + x86_gamma()
           + x86_beta()
           + x86("sub", "r12", (long)24)
           + x86_omega() )
         : (int)_.op_phase == 1
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_COND: no home -- neither a ζ-SPINE cell (op_zres) nor a ζ-STANDING slot (frame_need_of: DEFER-hazard / ALT-arm "
                      "classes); classifier and ZD plan disagree on this node -- the legacy C rt_cap_top fallback is deliberately not rebuilt (s83)")
           + x86_beta() )
         : (_.op_sval && _.op_sval[0] == '*' && !sn4_nret_cap())
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_IMM: computed-name (*VAR/NRETURN) target not yet rebuilt -- blocked on the :(NRETURN) lowering bug (s82), see this file's header comment") )
         : (int)_.op_phase == 2 && _.op_frame_need && _.op_cap_frame_off != -1 && cap_gva()
         ? cap_imm_gva(CFC(0))
         : (int)_.op_phase == 2 && _.op_frame_need && _.op_cap_frame_off != -1
         ? ( x86("comment", "IR_MATCH_CAPTURE_IMM")
           + x86_alpha()
           + x86("mov", "eax", CFC(0))
           + x86_anchor_enter()
           + capo_carve()
           + x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), x86_strtab_lbl((_.op_sval ? _.op_sval : "")).c_str())
           + x86("mov", "esi", "eax")
           + x86("mov", "edx", "r14d")
           + x86("mov", "ecx", (long)1)
           + capo_arg("r8", (const char *)0)
           + x86("call", cap_open_sym(), (uint64_t)(uintptr_t)cap_open_fp())
           + x86_rt_gc_poll()
           + (cap_fail_retreat()
              ? (x86("cmp", "rax", (long)-1)
               + x86("je", L(4)))
              : std::string())
           + x86("test", "rax", "rax")
           + x86("je", L(1))
         + bb_glue_enter_c2bb(10, 2, 3)
           + x86("def", L(2))
           + capo_arg("rcx", "ecx")
           + x86("call", "rt_cap_land_γ", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t, long, void *))rt_cap_land_γ)
           + x86_rt_gc_poll()
           + x86("jmp", L(1))
           + x86("def", L(3))
           + capo_arg("rsi", "esi")
           + x86("call", "rt_cap_land_ω", (uint64_t)(uintptr_t)(void *)(long (*)(long, void *))rt_cap_land_ω)
           + x86("push", "rax")
           + x86("push", "rax")
           + x86_rt_gc_poll()
           + x86("pop", "rax")
           + x86("pop", "rax")
           + (cap_fail_retreat() ? (x86("cmp", "rax", (long)-1)
           + x86("je", L(4))) : std::string())
           + x86("def", L(1))
           + capo_free()
           + x86_anchor_leave()
           + x86_gamma()
           + x86_beta_trampoline()
           + (cap_fail_retreat() ? (x86("def", L(4)) + capo_free() + x86_anchor_leave() + x86_omega()) : std::string()) )
         : (int)_.op_phase == 2 && _.op_frame_need
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_IMM: hazard crosses a DEFER-unsafe boundary but op_cap_frame_off is unavailable -- CAPTURE never pushes its own activation frame (s88 revert), see "
                      "IR_MATCH_CAPTURE_SAVE's bomb for the full rationale.")
           + x86_beta_trampoline() )
         : havehome() && cap_gva()
         ? cap_imm_gva(readhome())
         : havehome()
         ? ( x86("comment", "IR_MATCH_CAPTURE_IMM")
           + x86_alpha()
           + x86("mov", "eax", readhome())
           + x86_anchor_enter()
           + capo_carve()
           + x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), x86_strtab_lbl((_.op_sval ? _.op_sval : "")).c_str())
           + x86("mov", "esi", "eax")
           + x86("mov", "edx", "r14d")
           + x86("mov", "ecx", (long)1)
           + capo_arg("r8", (const char *)0)
           + x86("call", cap_open_sym(), (uint64_t)(uintptr_t)cap_open_fp())
           + x86_rt_gc_poll()
           + (cap_fail_retreat()
              ? (x86("cmp", "rax", (long)-1)
               + x86("je", L(4)))
              : std::string())
           + x86("test", "rax", "rax")
           + x86("je", L(1))
         + bb_glue_enter_c2bb(10, 2, 3)
           + x86("def", L(2))
           + capo_arg("rcx", "ecx")
           + x86("call", "rt_cap_land_γ", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t, long, void *))rt_cap_land_γ)
           + x86_rt_gc_poll()
           + x86("jmp", L(1))
           + x86("def", L(3))
           + capo_arg("rsi", "esi")
           + x86("call", "rt_cap_land_ω", (uint64_t)(uintptr_t)(void *)(long (*)(long, void *))rt_cap_land_ω)
           + x86("push", "rax")
           + x86("push", "rax")
           + x86_rt_gc_poll()
           + x86("pop", "rax")
           + x86("pop", "rax")
           + (cap_fail_retreat() ? (x86("cmp", "rax", (long)-1)
           + x86("je", L(4))) : std::string())
           + x86("def", L(1))
           + capo_free()
           + x86_anchor_leave()
           + x86_gamma()
           + x86_beta_trampoline()
           + (cap_fail_retreat() ? (x86("def", L(4)) + capo_free() + x86_anchor_leave() + x86_omega()) : std::string()) )
         : ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_IMM: no home -- neither a ζ-SPINE cell (op_zres) nor a ζ-STANDING slot (frame_need_of: DEFER-hazard / ALT-arm "
                      "classes); classifier and ZD plan disagree on this node -- the legacy C rt_cap_top fallback is deliberately not rebuilt (s83)")
           + x86_beta_trampoline() );
}
