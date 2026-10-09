#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" uint64_t rt_match_ctx_restore(uint64_t sig, uint64_t len, uint64_t cell);
typedef struct { long fn; long how; } rt_dcap_next_t;
extern "C" rt_dcap_next_t rt_dcap_end_ok_open(const char *mark, const char *top, const char *subj, void *rec);
extern "C" rt_dcap_next_t rt_dcap_land_γ(DESCR_t frame0, void *rec);
extern "C" rt_dcap_next_t rt_dcap_land_ω(void *rec);
#define DCF_RECORD_BYTES 112L
extern "C" long zvo_owner_dout(int cur_head);
#include "x86_asm.h"
#define rfc() (_.op_fc_disp >= 0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cap_name_strict(void) {
    static int v = -1;
    if (v < 0) {
        const char * e = getenv("SCRIP_CAP_NAME_STRICT");
        v = (e && *e == '0') ? 0 : 1;
    } return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define mend_bank_cursors() ( \
      IF(sn4_defer_beta_guard(), \
              x86("note", "mbc_restore") \
            + x86("mov", "rcx", "[rip@got + __]", (uint64_t)(uintptr_t)(const void *)&rtccb[0], "rtccb") \
            + x86("mov", "rax", RDQ("rbp", -48)) \
            + x86("mov", RDQ("rcx", 248), "rax")) \
    + ((_.op_dval != 0.0) \
    ? x86("note", "repl_start") \
    + x86("mov", "eax", RDD("rbp", -40)) \
    + x86("note", "repl_start") \
    + x86("mov", RDD("rbp", -36), "eax") \
    + x86("note", "repl_end") \
    + x86("mov", RDQ("rbp", -56), "r14") \
    : std::string()) \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string release_pump() {
    return std::string()
         + x86_xfer_enter()
         + x86_anchor_enter()
         + x86("sub", "rsp", DCF_RECORD_BYTES)
         + x86("note", "cas_mark")
         + x86("mov", "rdi", RDQ("rbp", -8))
         + x86("mov", "rsi", "r12")
         + x86("mov", "rdx", "r13")
         + x86("mov", "rcx", "rsp")
         + x86("call", "rt_dcap_end_ok_open", (uint64_t)(uintptr_t)(void *)(rt_dcap_next_t (*)(const char *, const char *, const char *, void *))rt_dcap_end_ok_open)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("def", L(1))
         + x86("cmp", "rax", 1L)
         + x86("jbe", L(2))
         + bb_glue_enter_c2bb(20, 8, 9)
         + x86("def", L(8))
         + x86("mov", "rdx", "rsp")
         + x86("call", "rt_dcap_land_γ", (uint64_t)(uintptr_t)(void *)(rt_dcap_next_t (*)(DESCR_t, void *))rt_dcap_land_γ)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("jmp", L(1))
         + x86("def", L(9))
         + x86("mov", "rdi", "rsp")
         + x86("call", "rt_dcap_land_ω", (uint64_t)(uintptr_t)(void *)(rt_dcap_next_t (*)(void *))rt_dcap_land_ω)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("jmp", L(1))
         + x86("def", L(2))
         + x86("add", "rsp", DCF_RECORD_BYTES)
         + x86("mov", RDQ("rsp", 0), "rax")
         + x86("note", HKN(1))
         + x86("mov", "rdi", RDQ("rbp", -16))
         + x86("note", HKN(3))
         + x86("mov", "rsi", RDQ("rbp", -32))
         + x86("lea", "rdx", RDQ("rbp", MATCH_CTX_CELL_OFF(_.op_frame_extra)))
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)
         + x86("note", HKN(1))
         + x86("mov", RDQ("rbp", -16), "rax")
         + x86("mov", "rax", RDQ("rsp", 0))
         + x86_anchor_leave()
         + x86_xfer_leave()
         + IF(cap_name_strict(),
               x86("comment",
                   "SN4-CAP-NAME-STRICT: rax != 0 = a deferred capture target resolved to a VALUE, not a NAME -- the terminus "
                       "fails instead of committing an indirect assignment (oracle: sbl retreats)")
             + x86("test", "rax", "rax")
             + x86("je", L(13))
             + x86_omega()
             + x86("def", L(13)))
         + (x86("note", "cas_mark")
             + x86("mov", "r12", RDQ("rbp", -8))
             + x86_abs_disp32_store64(0x70000000L, "r12")
             + x86("note", HKN(1))
             + x86("mov", "r13", RDQ("rbp", -16))
             + x86("note", HKN(2))
             + x86("mov", "r14", RDQ("rbp", -24))
             + x86("note", HKN(3))
             + x86("mov", "r15", RDQ("rbp", -32)))
         + IF(_.op_dval != 0.0,
               x86("note", "repl_start")
             + x86("mov", "eax", RDD("rbp", -36))
             + x86("mov", RDD("r12", 0), "eax")
             + x86("note", "repl_end")
             + x86("mov", "rax", RDQ("rbp", -56))
             + x86("mov", RDQ("r12", 8), "rax")
             + x86("add", "r12", (long)16))
         + x86("note", "frame_whack")
         + x86("mov", "rsp", "rbp")
         + x86("pop", "rbp")
         + x86_gc_site(X86_SITE_MATCH_LEAVE)
         + IF(_.op_dval == 0.0 && _.flat_deep_arrival, x86("note", HKN(0)) + std::string(""))
         + x86_gamma();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_end() {
    x86_begin();
    if (getenv("SCRIP_MEND_ADDR_DIAG")) fprintf(stderr, "[MEND] op_off=%d op_fc_disp=%d op_dval=%g rfc=%d op_tail=%d\n",
                                                 _.op_off, _.op_fc_disp, _.op_dval, rfc() ? 1 : 0, _.op_tail);
    return _.op_off < 0
         ? x86_alpha() + x86_bomb("IR_MATCH_END: head slot not resolved (operand[0] missing or unowned)")
         : _.op_tail && rfc()
         ? x86_alpha() + x86("mov", "rcx", "[rip@got + __]", (uint64_t)(uintptr_t)(const void *)&rtccb[0], "rtccb")
         + x86("mov", "rcx", RDQ("rcx", 200))
         + x86("test", "rcx", "rcx") + x86_omega("jne") + mend_bank_cursors()
         + release_pump()
         : x86("comment", "IR_MATCH_END")
         + x86_alpha() + x86("mov", "rcx", "[rip@got + __]", (uint64_t)(uintptr_t)(const void *)&rtccb[0], "rtccb")
         + x86("mov", "rcx", RDQ("rcx", 200))
         + x86("test", "rcx", "rcx") + x86_omega("jne") + mend_bank_cursors()
         + x86_align_leave()
         + release_pump();
}
