#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "templates/x86/x86_gc.h"
#include "emitter/emit_medium.h"
#include "emitter/emit_state_em_03_sh_api.h"
#include "emitter/emit_state_.h"
#include "emitter/emit_str.h"
#include "emitter/emit_gc_site.h"
#include "templates/x86/x86_frame.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_rsp_tracker.h"
#include "templates/x86/x86_rtcc.h"
#include "ir/descr.h"
#include "ir/descr_tags_dt_platom.h"
#include "ir/ir_graph.h"
extern "C" { extern uint64_t g_rtccb[32]; }
extern "C" void rt_gc_poll_asm(void);
extern "C" int g_gc_pending;
extern "C" void rt_pl_cap_raise(void);
extern "C" void rt_pl_alarm_raise(void);
extern "C" void rt_gc_point_arr_c(DESCR_t * arr, int n, const char ** r0, char * floor);
extern "C" void rt_gc_point_arr_probe_c(DESCR_t * arr, int n, const char ** saved_subject_reg, char * floor);
static std::string x86_gc_gate(const std::string & body);
static int x86_scan_regs_graph();
static std::string x86_rt_gc_poll_scan_regs();
static int x86_poll_in_region(long n, int reg);
static std::string x86_poll_open(long n, int reg);
static std::string x86_poll_close(long n, int reg);
static std::string x86_rt_gc_poll_res_scan_regs(const std::string & raise);
static std::string x86_rt_gc_poll_rec1(const char * preg, const char * lenreg32, int keep_rax, int keep_rdx = 0, int reg = 0);
static std::string x86_rt_gc_poll_rec2(const char * preg0, const char * lenreg32_0, const char * preg1, const char * lenreg32_1);
static std::string x86_rt_gc_poll_rec2_res(const char * preg0, const char * lenreg32_0);
static std::string x86_rt_gc_poll_rec_sigma_res();
std::string x86_gc_site_adj(int kind, int adj) {
    int idx = emit_gc_site_new(kind, adj);
    if (idx < 0) return std::string();
    if (MEDIUM_BINARY) { std::string r; r += (char)'S'; r += (char)(idx & 255); r += (char)((idx >> 8) & 255); r += (char)((idx >> 16) & 255); r += (char)((idx >> 24) & 255); return r; }
    return x86_reclbl(emit_gc_site_label(idx)) + "\n";
}
std::string x86_gc_site(int kind) { return x86_gc_site_adj(kind, 0); }
std::string x86_gc_site_raw(int kind, int rule, int depth) {
    int idx = emit_gc_site_raw(kind, rule, depth);
    if (idx < 0) return std::string();
    if (MEDIUM_BINARY) { std::string r; r += (char)'S'; r += (char)(idx & 255); r += (char)((idx >> 8) & 255); r += (char)((idx >> 16) & 255); r += (char)((idx >> 24) & 255); return r; }
    return x86_reclbl(emit_gc_site_label(idx)) + "\n";
}
void x86_text_sites_scan(const std::string & r, bool real) {
    size_t i = 0, n = r.size();
    while (i < n) { size_t e = r.find('\n', i); if (e == std::string::npos) e = n; std::string line = r.substr(i, e - i); i = e + 1;
        if (line.compare(0, 6, "#@rsp ") == 0) { if (!g_x86_rsp_aligned) g_x86_rsp_delta += atoi(line.c_str() + 6); continue; }
        if (line.compare(0, 6, "#@rspU") == 0) { if (!g_x86_rsp_aligned) g_x86_rsp_unknown = 1; continue; }
        if (line.compare(0, 6, "#@rspA") == 0) { g_x86_rsp_aligned = 1; g_x86_rsp_before = g_x86_rsp_delta; continue; }
        if (line.compare(0, 6, "#@rspZ") == 0) { g_x86_rsp_aligned = 0; g_x86_rsp_delta = g_x86_rsp_before; continue; }
        if (line.compare(0, 6, "#@rspG") == 0) { x86_rsp_S().dead = 1; continue; }
        if (line.compare(0, 6, "#@rspK") == 0) { x86_rsp_S().mark = x86_rsp_S().delta; x86_rsp_S().mark_unknown = x86_rsp_S().unknown; continue; }
        if (line.compare(0, 6, "#@rspM") == 0) { x86_rsp_mark9_push(); continue; }
        if (line.compare(0, 6, "#@rspN") == 0) { x86_rsp_mark9_pop(); continue; }
        if (line.compare(0, 7, "#@rspk ") == 0) { x86_rsp_S().delta = x86_rsp_S().mark + atoi(line.c_str() + 7); x86_rsp_S().dead = 0; x86_rsp_S().unknown = x86_rsp_S().mark_unknown; continue; }
        if (!line.empty() && line[0] == '#') continue;
        size_t p = 0; while (p < line.size()) { size_t q = line.find(';', p); if (q == std::string::npos) q = line.size(); std::string ins = line.substr(p, q - p); p = q + 1;
            size_t a = ins.find_first_not_of(" \t"); if (a == std::string::npos) continue; ins = ins.substr(a); while (!ins.empty() && (ins.back() == ' ' || ins.back() == '\t')) ins.pop_back();
            if (ins.compare(0, 9, ".Lgcsite_") == 0) { size_t c = ins.find_first_of(": "); if (c != std::string::npos && real) { size_t u = ins.rfind('_', c);
                if (u != std::string::npos) emit_gc_site_play(atoi(ins.c_str() + u + 1), (const void *)0, g_x86_rsp_aligned ? g_x86_rsp_before : g_x86_rsp_delta, g_x86_rsp_aligned, g_x86_rsp_unknown);
                } continue; }
            if (!ins.empty() && ins.back() == ':') { std::string nm = ins.substr(0, ins.size() - 1); size_t sp = nm.find_first_of(" \t"); if (sp != std::string::npos) nm = nm.substr(0, sp);
                size_t L = nm.size();
                int is_a = L >= 3 && nm.compare(L - 3, 3, "_\xce\xb1") == 0, is_b = L >= 3 && nm.compare(L - 3, 3, "_\xce\xb2") == 0;
                if (is_a) { x86_rsp_S().delta = 0; x86_rsp_S().dead = 0; x86_rsp_S().unknown = 0; } else if (is_b) { x86_rsp_S().delta = _.op_fc_bytes > 0 ? (int)_.op_fc_bytes : 0;
                    x86_rsp_S().dead = 0; x86_rsp_S().unknown = 0; } else x86_rsp_define(4, x86_rsp_hash(nm));
                continue; }
            if (ins.size() > 2 && ins[0] == 'j') { size_t sp = ins.find_first_of(" \t"); if (sp != std::string::npos) { std::string mn = ins.substr(0, sp);
                size_t t0 = ins.find_first_not_of(" \t", sp); std::string tg = t0 != std::string::npos ? ins.substr(t0) : std::string(); size_t te = tg.find_first_of(" \t#");
                if (te != std::string::npos) tg = tg.substr(0, te);
                    if (!tg.empty() && tg.find('[') == std::string::npos && tg != "rax" && tg != "rcx" && tg != "rdx" && tg != "r11" && tg != "rdi" && tg != "rsi") x86_rsp_jump(4, x86_rsp_hash(tg));
                    if (mn == "jmp") x86_rsp_S().dead = 1; } continue; }
            if (ins.compare(0, 3, "ret") == 0) { x86_rsp_S().dead = 1; continue; }
            if (!ins.empty() && ins[0] != '.') x86_rsp_S().dead = 0; }
    }
}
static std::string x86_gc_gate(const std::string & body) {
    std::string test =
        x86("push", "rax") + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(const void *)&g_gc_pending, "g_gc_pending") + x86("mov", "eax", RDD("rax", 0)) +
        x86("test", "eax", "eax") + x86("pop", "rax");
    if (!MEDIUM_BINARY) return test + x86("directive", "\tje 1f") + body + x86("directive", "1:");
    { long n = x86_rec_bytes(body);
      if (n <= 0) { static int said;
          if (!said++ && getenv("SCRIP_GATE_DIAG")) fprintf(stderr, "[GATE-DIAG] a poll body carries a record kind x86_rec_bytes does not size; the site is emitted ungated\n"); return body; }
      std::string je = (n <= 127) ? x86_b2(0x74, (uint8_t)n) : (x86_b2(0x0F, 0x84) + x86_b4((uint8_t)(n & 255), (uint8_t)((n >> 8) & 255), (uint8_t)((n >> 16) & 255), (uint8_t)((n >> 24) & 255)));
      return test + x86_Lrec(je) + body; }
}
std::string x86_cap_raise_call() {
    uint64_t block = (uint64_t)(uintptr_t)g_rtccb, fp = (uint64_t)(uintptr_t)(void *)rt_pl_cap_raise;
    if (MEDIUM_BINARY) return x86_align_assert() + x86_Lrec(x86_rtcc_wb_bin(block, RTCC_C_R11)) + x86_Lrec(x86_rtcc_call_b(fp)) + x86_gc_site(X86_SITE_CALL) +
        x86_Lrec(x86_rtcc_rl_bin(block, RTCC_C_R11));
    return x86("call", "rt_pl_cap_raise", fp);
}
std::string x86_alarm_raise_call() {
    uint64_t block = (uint64_t)(uintptr_t)g_rtccb, fp = (uint64_t)(uintptr_t)(void *)rt_pl_alarm_raise;
    if (MEDIUM_BINARY) return x86_align_assert() + x86_Lrec(x86_rtcc_wb_bin(block, RTCC_C_R11)) + x86_Lrec(x86_rtcc_call_b(fp)) + x86_gc_site(X86_SITE_CALL) +
        x86_Lrec(x86_rtcc_rl_bin(block, RTCC_C_R11));
    return x86("call", "rt_pl_alarm_raise", fp);
}
static int x86_scan_regs_graph() { return g_emit_cfg && g_emit_cfg->scan_regs; }
static std::string x86_rt_gc_poll_scan_regs() {
    return x86("comment",
        "ARCH-ICON-RTX 10 R6: a scan_regs graph holds the subject pair at every instruction, so the poll spills it as ONE described cell {DT_S, r15d, r13} the walker relocates and reloads r13; "
        "rax rcx rdx rsi rdi are kept as rt_gc_poll_slow keeps them")
         + x86("sub", "rsp", (long)64)
         + x86_rsp_store32_imm(0, (long)DT_S)
         + x86_rsp_store32(4, "r15d")
         + x86_rsp_store64(8, "r13")
         + x86_rsp_store64(16, "rax")
         + x86_rsp_store64(24, "rcx")
         + x86_rsp_store64(32, "rdx")
         + x86_rsp_store64(40, "rsi")
         + x86_rsp_store64(48, "rdi")
         + x86_reg_disp32_lea64("rdi", "rsp", 0)
         + x86("mov", "esi", (long)1)
         + x86("mov", "edx", (long)0)
         + x86_reg_disp32_lea64("rcx", "rsp", 64)
         + x86_rtcc_call_adj("rt_gc_point_arr_c", (uint64_t)(uintptr_t)(void *)rt_gc_point_arr_c, 0)
         + x86_rsp_load64("r13", 8)
         + x86_rsp_load64("rax", 16)
         + x86_rsp_load64("rcx", 24)
         + x86_rsp_load64("rdx", 32)
         + x86_rsp_load64("rsi", 40)
         + x86_rsp_load64("rdi", 48)
         + x86("add", "rsp", (long)64);
}
std::string x86_rt_gc_poll_at(const char * f, int l, const std::string & raise) { const char * b = strrchr(f, '/');
    return x86("note", std::string("gc_poll ") + (b ? b + 1 : f) + ":" + std::to_string(l))
         + x86_gc_gate((x86_scan_regs_graph() ? x86_rt_gc_poll_scan_regs() : x86("call", "rt_gc_poll_asm", (uint64_t)(uintptr_t)(void *)rt_gc_poll_asm)) + raise); }
static int x86_poll_in_region(long n, int reg) { return reg && _.op_zrel >= n; }
static std::string x86_poll_open(long n, int reg) { return x86_poll_in_region(n, reg) ? std::string() : x86("sub", "rsp", n); }
static std::string x86_poll_close(long n, int reg) { return x86_poll_in_region(n, reg) ? std::string() : x86("add", "rsp", n); }
static std::string x86_rt_gc_poll_res_scan_regs(const std::string & raise) {
return x86_gc_gate(x86("comment", "ARCH-ICON-RTX 10 R6: the subject pair as one described cell and the box result rax:rdx as a second, both swept and relocated across the poll")
         + x86("sub", "rsp", (long)32)
         + x86_rsp_store32_imm(0, (long)DT_S)
         + x86_rsp_store32(4, "r15d")
         + x86_rsp_store64(8, "r13")
         + x86_rsp_store64(16, "rax")
         + x86_rsp_store64(24, "rdx")
         + x86_reg_disp32_lea64("rdi", "rsp", 0)
         + x86("mov", "esi", (long)2)
         + x86("mov", "edx", (long)0)
         + x86_reg_disp32_lea64("rcx", "rsp", 32)
         + x86_rtcc_call_adj("rt_gc_point_arr_c", (uint64_t)(uintptr_t)(void *)rt_gc_point_arr_c, 0)
         + x86_rsp_load64("r13", 8)
         + x86_rsp_load64("rax", 16)
         + x86_rsp_load64("rdx", 24)
         + x86("add", "rsp", (long)32)
         + raise);
}
std::string x86_rt_gc_poll_res(const std::string & raise, int reg) {
if (x86_scan_regs_graph()) return x86_rt_gc_poll_res_scan_regs(raise);
return x86_gc_gate(x86("comment",
    "ARCH-GC 6.5b: the box result lives in rax:rdx, so it is spilled as a DESCR cell at the sweep floor across the poll and reloaded -- the walker sweeps [poll floor, stack top) and relocates it; r1"
    "3 is spilled as a raw word BELOW the floor and handed to the probe entry as the saved subject, which rewrites it only when it aliases Sigma or g_scan_subj")
         + x86_poll_open(32, reg)
         + x86_reg_disp32_store64("rsp", 0, "r13")
         + x86_reg_disp32_store64("rsp", 16, "rax")
         + x86_reg_disp32_store64("rsp", 24, "rdx")
         + x86("xor", "edi", "edi")
         + x86("xor", "esi", "esi")
         + x86("mov", "rdx", "rsp")
         + x86_reg_disp32_lea64("rcx", "rsp", 16)
         + x86_rtcc_call_adj("rt_gc_point_arr_probe_c", (uint64_t)(uintptr_t)(void *)rt_gc_point_arr_probe_c, 0)
         + x86_reg_disp32_load64("r13", "rsp", 0)
         + x86_reg_disp32_load64("rax", "rsp", 16)
         + x86_reg_disp32_load64("rdx", "rsp", 24)
         + x86_poll_close(32, reg)
         + raise);
}
static std::string x86_rt_gc_poll_rec1(const char * preg, const char * lenreg32, int keep_rax, int keep_rdx, int reg) {
return x86_gc_gate(x86_poll_open(32, reg)
         + x86_rsp_store32_imm(0, (long)DT_S)
         + IF(lenreg32 != NULL, x86_rsp_store32(4, lenreg32))
         + IF(lenreg32 == NULL, x86_rsp_store32_imm(4, (long)0))
         + x86_rsp_store64(8, preg)
         + IF(keep_rax, x86_rsp_store64(16, "rax"))
         + IF(keep_rdx, x86_rsp_store64(24, "rdx"))
         + x86_reg_disp32_lea64("rdi", "rsp", 0)
         + x86("mov", "esi", (long)1)
         + x86("mov", "edx", (long)0)
         + x86_reg_disp32_lea64("rcx", "rsp", 32)
         + x86_rtcc_call_adj("rt_gc_point_arr_c", (uint64_t)(uintptr_t)(void *)rt_gc_point_arr_c, 0)
         + x86_rsp_load64(preg, 8)
         + IF(keep_rax, x86_rsp_load64("rax", 16))
         + IF(keep_rdx, x86_rsp_load64("rdx", 24))
         + x86_poll_close(32, reg));
}
std::string x86_rt_gc_poll_rec_sigma(int keep_rax, int reg) { return x86_rt_gc_poll_rec1("r13", "r15d", keep_rax, 0, reg); }
std::string x86_rt_gc_poll_rec_name(const char * preg) {
return x86_gc_gate(x86("sub", "rsp", (long)16)
         + x86_rsp_store32_imm(0, (long)DT_N)
         + x86_rsp_store32_imm(4, (long)1)
         + x86_rsp_store64(8, preg)
         + x86_reg_disp32_lea64("rdi", "rsp", 0)
         + x86("mov", "esi", (long)1)
         + x86("mov", "edx", (long)0)
         + x86_reg_disp32_lea64("rcx", "rsp", 16)
         + x86_rtcc_call_adj("rt_gc_point_arr_c", (uint64_t)(uintptr_t)(void *)rt_gc_point_arr_c, 0)
         + x86_rsp_load64(preg, 8)
         + x86("add", "rsp", (long)16));
}
static std::string x86_rt_gc_poll_rec2(const char * preg0, const char * lenreg32_0, const char * preg1, const char * lenreg32_1) {
return x86_gc_gate(x86("comment",
    "ARCH-GC 2b RULE 1a: two live string pointers across one poll, each written as a well-formed tagged DESCR cell -- the range walk relocates only what gc_cell_visit can TYPE, so a raw spilled poin"
    "ter is reported and never rooted")
         + x86("sub", "rsp", (long)32)
         + x86_rsp_store32_imm(0, (long)DT_S)
         + x86_rsp_store32(4, lenreg32_0)
         + x86_rsp_store64(8, preg0)
         + x86_rsp_store32_imm(16, (long)DT_S)
         + x86_rsp_store32(20, lenreg32_1)
         + x86_rsp_store64(24, preg1)
         + x86_reg_disp32_lea64("rdi", "rsp", 0)
         + x86("mov", "esi", (long)2)
         + x86("mov", "edx", (long)0)
         + x86_reg_disp32_lea64("rcx", "rsp", 32)
         + x86_rtcc_call_adj("rt_gc_point_arr_c", (uint64_t)(uintptr_t)(void *)rt_gc_point_arr_c, 0)
         + x86_rsp_load64(preg0, 8)
         + x86_rsp_load32(lenreg32_0, 4)
         + x86_rsp_load64(preg1, 24)
         + x86_rsp_load32(lenreg32_1, 20)
         + x86("add", "rsp", (long)32));
}
std::string x86_rt_gc_poll_rec_sigma_needle() { return x86_rt_gc_poll_rec2("r13", "r15d", "rax", "edx"); }
static std::string x86_rt_gc_poll_rec2_res(const char * preg0, const char * lenreg32_0) {
return x86_gc_gate(x86("comment",
    "ARCH-GC 2b RULE 1a, cell 1 RAW: a DESCR_t return arrives as rax={v,src,slen} rdx={ptr}, so the register pair IS a well-formed cell and writing DT_S over it would forge the tag instead of carryi"
    "ng it")
         + x86("sub", "rsp", (long)32)
         + x86_rsp_store32_imm(0, (long)DT_S)
         + x86_rsp_store32(4, lenreg32_0)
         + x86_rsp_store64(8, preg0)
         + x86_rsp_store64(16, "rax")
         + x86_rsp_store64(24, "rdx")
         + x86_reg_disp32_lea64("rdi", "rsp", 0)
         + x86("mov", "esi", (long)2)
         + x86("mov", "edx", (long)0)
         + x86_reg_disp32_lea64("rcx", "rsp", 32)
         + x86_rtcc_call_adj("rt_gc_point_arr_c", (uint64_t)(uintptr_t)(void *)rt_gc_point_arr_c, 0)
         + x86_rsp_load64(preg0, 8)
         + x86_rsp_load64("rax", 16)
         + x86_rsp_load64("rdx", 24)
         + x86("add", "rsp", (long)32));
}
static std::string x86_rt_gc_poll_rec_sigma_res() { return x86_rt_gc_poll_rec2_res("r13", "r15d"); }
std::string x86_rt_gc_poll_rec_subject_new() { return x86_rt_gc_poll_rec1("rax", "edx", 0, 1); }
std::string x86_rt_gc_poll_rec_sigma_word(int keep_rax, int reg) { return x86_rt_gc_poll_rec1("r13", "r15d", keep_rax, 1, reg); }
std::string x86_rt_gc_poll_rec_sigma_pair(int ptr_in_rax, int lbl) {
return x86_gc_gate(x86("sub", "rsp", (long)48)
         + x86_rsp_store32_imm(0, (long)DT_S) + x86_rsp_store32(4, "r15d") + x86_rsp_store64(8, "r13")
         + x86_rsp_store32_imm(16, (long)DT_I) + x86_rsp_store32_imm(20, 0L) + x86_rsp_store64(24, "rax")
         + x86_rsp_store32_imm(32, (long)DT_I) + x86_rsp_store32_imm(36, 0L) + x86_rsp_store64(40, "rdx")
         + IF(ptr_in_rax, x86("cmp", "rdx", (long)4) + x86("jne", L(lbl)) + x86_rsp_store32_imm(16, (long)DT_P) + x86("def", L(lbl)))
         + IF(!ptr_in_rax, x86("test", "rax", "rax") + x86("je", L(lbl)) + x86_rsp_store32_imm(32, (long)DT_P) + x86("def", L(lbl)))
         + x86_reg_disp32_lea64("rdi", "rsp", 0)
         + x86("mov", "esi", (long)3)
         + x86("mov", "edx", (long)0)
         + x86_reg_disp32_lea64("rcx", "rsp", 48)
         + x86_rtcc_call_adj("rt_gc_point_arr_c", (uint64_t)(uintptr_t)(void *)rt_gc_point_arr_c, 0)
         + x86_rsp_load64("r13", 8)
         + x86_rsp_load64("rax", 24)
         + x86_rsp_load64("rdx", 40)
         + x86("add", "rsp", (long)48));
}
static std::string x86_rt_gc_poll_rec_res_scan_regs(int reg) {
return x86_gc_gate(x86_poll_open(32, reg)
         + x86_rsp_store32_imm(0, (long)DT_S)
         + x86_rsp_store32(4, "r15d")
         + x86_rsp_store64(8, "r13")
         + x86_rsp_store64(16, "rax")
         + x86_rsp_store64(24, "rdx")
         + x86_reg_disp32_lea64("rdi", "rsp", 0)
         + x86("mov", "esi", (long)2)
         + x86("mov", "edx", (long)0)
         + x86_reg_disp32_lea64("rcx", "rsp", 32)
         + x86_rtcc_call_adj("rt_gc_point_arr_c", (uint64_t)(uintptr_t)(void *)rt_gc_point_arr_c, 0)
         + x86_rsp_load64("r13", 8)
         + x86_rsp_load64("rax", 16)
         + x86_rsp_load64("rdx", 24)
         + x86_poll_close(32, reg));
}
std::string x86_rt_gc_poll_rec_res(int reg) {
if (x86_scan_regs_graph()) return x86_rt_gc_poll_rec_res_scan_regs(reg);
return x86_gc_gate(x86_poll_open(32, reg)
         + x86_rsp_store64(0, "rax")
         + x86_rsp_store64(8, "rdx")
         + x86_rsp_store64(16, "r13")
         + x86_reg_disp32_lea64("rdi", "rsp", 0)
         + x86("mov", "esi", (long)1)
         + x86("lea", "rdx", RDQ("rsp", 16))
         + x86_reg_disp32_lea64("rcx", "rsp", 32)
         + x86_rtcc_call_adj("rt_gc_point_arr_probe_c", (uint64_t)(uintptr_t)(void *)rt_gc_point_arr_probe_c, 0)
         + x86_rsp_load64("rax", 0)
         + x86_rsp_load64("rdx", 8)
         + x86_rsp_load64("r13", 16)
         + x86_poll_close(32, reg));
}
