#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include "templates/x86/x86_rtcc.h"
#include "templates/x86/x86_rodata.h"
#include "emitter/emit_medium.h"
#include "emitter/bb_label.h"
#include "emitter/emit_state_.h"
#include "ir/descr_tags.h"
#include "emitter/emit_rtx.h"
#include "emitter/emit_str.h"
#include "runtime/rtx/rtcc.h"
#include "templates/x86/x86_call.h"
#include "templates/x86/x86_frame.h"
#include "templates/x86/x86_gc.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_ops.h"
extern "C" { extern uint64_t g_rtccb[32]; }
extern "C" int gva_count(void);
static int x86_rtx_plant(const char * name, int * cache);
static int x86_rtx_plant_veneer(void);
static int x86_rtx_plant_clobber(void);
static std::string x86_rtx_after_bare(const char * sym);
static std::string x86_call_text_adj(const char * sym, const char * sfx, int adj);
static std::string x86_call_ro_adj(const char * sym, uint64_t ptr, int adj);
static unsigned x86_rtcc_veneer_mask(void);
static int x86_rtcc_veneer_on(void);
static unsigned x86_rtcc_clob(const char * sym);
static std::string x86_ro_seal_q(int n, uint64_t val);
static std::string x86_ro_seal_addr(int n, const char * sym, const void * addr);
static std::string x86_ro_seal_str(int n, const char * lit);
static std::string x86_rtx_verdict_cmp(int kind);
std::string x86_cset_probe() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b4(0x80, 0x3C, 0x37, 0x00)) : x86_rec("cmp") + "byte ptr [rdi+rsi], 0\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_cset_bt() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b3(0x0F, 0xA3, 0x37)) : x86_rec("bt") + "dword ptr [rdi], esi\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x86_rtx_plant(const char * name, int * cache) { if (*cache < 0) { const char * e = getenv(name); *cache = (e && *e == '1') ? 1 : 0; } return *cache; }
static int x86_rtx_plant_veneer(void) { static int v = -1; return x86_rtx_plant("SCRIP_RTX_PLANT_VENEER", &v); }
static int x86_rtx_plant_clobber(void) { static int v = -1; return x86_rtx_plant("SCRIP_RTX_PLANT_CLOBBER", &v); }
static std::string x86_rtx_after_bare(const char * sym) {
    if (!x86_rtx_plant_clobber() || !emit_rtx_entry_is(sym)) return std::string();
    return x86_movabs_r64("r10", ~(uint64_t)0) + x86_movabs_r64("r11", ~(uint64_t)0);
}
static std::string x86_call_text_adj(const char * sym, const char * sfx, int adj) {
    if (emit_rtx_entry_is(sym)) return x86_rec("call") + "qword ptr [rip + " + sym + "@GOTPCREL]\n" + x86_gc_site_adj(X86_SITE_CALL, adj);
    return x86_rec("call") + sym + sfx + "\n" + x86_gc_site_adj(X86_SITE_CALL, adj);
}
std::string x86_call_text(const char * sym, const char * sfx) { return x86_call_text_adj(sym, sfx, 0); }
static std::string x86_call_ro_adj(const char * sym, uint64_t ptr, int adj) {
    if (MEDIUM_BINARY) { std::string code; code += (char)0x48; code += (char)0xB8; code += u64le(ptr); code += (char)0xFF; code += (char)0xD0;
        return x86_align_assert() + x86_Lrec(code) + x86_gc_site_adj(X86_SITE_CALL, adj) + x86_rtx_after_bare(sym) + x86_rtx_verdict_adj(sym, adj); }
    return x86_align_assert() + x86_call_text_adj(sym, "@PLT", adj) + x86_rtx_after_bare(sym) + x86_rtx_verdict_adj(sym, adj);
}
std::string x86_call_ro(const char * sym, uint64_t ptr) { return x86_call_ro_adj(sym, ptr, 0); }
static unsigned x86_rtcc_veneer_mask(void) {
    static int v = -1;
    if (v < 0) {
        const char * e = getenv("SCRIP_RTCC_VENEER");
        v = (e && *e) ? (int)strtoul(e, 0, 0) : x86_rtx_plant_veneer() ? (int)RTCC_C_ALL : (int)(RTCC_C_R9 | (x86_diag_regs_on() ? RTCC_C_R11 : 0u));
    }
    return (unsigned)v;
}
static int x86_rtcc_veneer_on(void) { return x86_rtcc_veneer_mask() != 0; }
unsigned x86_rtcc_live_mask(void) { unsigned m = x86_rtcc_veneer_mask(); return (RTCC_GLOBAL_R9_GVA && gva_count() == 0) ? (m & ~RTCC_C_R9) : m; }
static unsigned x86_rtcc_clob(const char * sym) { return x86_rtcc_clob_raw(sym) & x86_rtcc_live_mask(); }
unsigned x86_rtcc_clob_raw(const char * sym) {
    if (!sym) return RTCC_C_ALL;
    return (emit_rtx_entry_is(sym) && !x86_rtx_plant_veneer()) ? emit_rtx_clob_mask(sym) : RTCC_C_ALL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rtcc_wb_bin(uint64_t block, unsigned m) {
    std::string wb;
    if (!(m & (RTCC_C_R8 | RTCC_C_R10 | RTCC_C_R11)) && !((m & RTCC_C_R9) && !RTCC_GLOBAL_R9_GVA)) return wb;
    if (m & RTCC_C_R11) {
        wb += (char)0x41; wb += (char)0x53;
        wb += (char)0x49; wb += (char)0xBB; wb += u64le(block);
        if (m & RTCC_C_R8) { wb += (char)0x4D; wb += (char)0x89; wb += (char)0x43; wb += (char)40; }
        if ((m & RTCC_C_R9) && !RTCC_GLOBAL_R9_GVA) { wb += (char)0x4D; wb += (char)0x89; wb += (char)0x4B; wb += (char)48; }
        if (m & RTCC_C_R10) { wb += (char)0x4D; wb += (char)0x89; wb += (char)0x53; wb += (char)56; }
        wb += (char)0x41; wb += (char)0x8F; wb += (char)0x43; wb += (char)64;
        return wb;
    }
    wb += (char)0x48; wb += (char)0xB8; wb += u64le(block);
    if (m & RTCC_C_R8) { wb += (char)0x4C; wb += (char)0x89; wb += (char)0x40; wb += (char)40; }
    if ((m & RTCC_C_R9) && !RTCC_GLOBAL_R9_GVA) { wb += (char)0x4C; wb += (char)0x89; wb += (char)0x48; wb += (char)48; }
    if (m & RTCC_C_R10) { wb += (char)0x4C; wb += (char)0x89; wb += (char)0x50; wb += (char)56; }
    return wb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rtcc_rl_bin(uint64_t block, unsigned m) {
    std::string rl;
    if (m & RTCC_C_R11) {
        rl += (char)0x49; rl += (char)0xBB; rl += u64le(block);
        if (m & RTCC_C_R8) { rl += (char)0x4D; rl += (char)0x8B; rl += (char)0x43; rl += (char)40; }
        if (m & RTCC_C_R9) { rl += (char)0x4D; rl += (char)0x8B; rl += (char)0x4B; rl += (char)48; }
        if (m & RTCC_C_R10) { rl += (char)0x4D; rl += (char)0x8B; rl += (char)0x53; rl += (char)56; }
        rl += (char)0x4D; rl += (char)0x8B; rl += (char)0x5B; rl += (char)64;
        return rl;
    }
    if (m == RTCC_C_R9) {
        rl += (char)0x49; rl += (char)0xB9; rl += u64le(block);
        rl += (char)0x4D; rl += (char)0x8B; rl += (char)0x49; rl += (char)48;
        return rl;
    }
    if (!(m & (RTCC_C_R8 | RTCC_C_R9 | RTCC_C_R10))) return rl;
    rl += (char)0x48; rl += (char)0xB9; rl += u64le(block);
    if (m & RTCC_C_R8) { rl += (char)0x4C; rl += (char)0x8B; rl += (char)0x41; rl += (char)40; }
    if (m & RTCC_C_R9) { rl += (char)0x4C; rl += (char)0x8B; rl += (char)0x49; rl += (char)48; }
    if (m & RTCC_C_R10) { rl += (char)0x4C; rl += (char)0x8B; rl += (char)0x51; rl += (char)56; }
    return rl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rtcc_call_b(uint64_t ptr) {
    std::string c;
    c += (char)0x49; c += (char)0xBB; c += u64le(ptr); c += (char)0x41; c += (char)0xFF; c += (char)0xD3;
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rtcc_wb_text(unsigned m) {
    std::string wb;
    if (m & RTCC_C_R8) wb += x86_rec("mov") + "qword ptr [rip + g_rtccb+40], r8\n";
    if ((m & RTCC_C_R9) && !RTCC_GLOBAL_R9_GVA) wb += x86_rec("mov") + "qword ptr [rip + g_rtccb+48], r9\n";
    if (m & RTCC_C_R10) wb += x86_rec("mov") + "qword ptr [rip + g_rtccb+56], r10\n";
    if (m & RTCC_C_R11) wb += x86_rec("mov") + "qword ptr [rip + g_rtccb+64], r11\n";
    return wb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rtcc_rl_text(unsigned m) {
    std::string rl;
    if (g_emit_module_pic) {
        if (m & RTCC_C_R8) rl += x86_rec("mov") + "r8,  qword ptr [rip + g_rtccb@GOTPCREL]\n" + x86_rec("mov") + "r8,  qword ptr [r8 + 40]\n";
        if (m & RTCC_C_R9) rl += x86_rec("mov") + "r9,  qword ptr [rip + g_rtccb@GOTPCREL]\n" + x86_rec("mov") + "r9,  qword ptr [r9 + 48]\n";
        if (m & RTCC_C_R10) rl += x86_rec("mov") + "r10, qword ptr [rip + g_rtccb@GOTPCREL]\n" + x86_rec("mov") + "r10, qword ptr [r10 + 56]\n";
        if (m & RTCC_C_R11) rl += x86_rec("mov") + "r11, qword ptr [rip + g_rtccb@GOTPCREL]\n" + x86_rec("mov") + "r11, qword ptr [r11 + 64]\n";
        return rl;
    }
    if (m & RTCC_C_R8) rl += x86_rec("mov") + "r8,  qword ptr [rip + g_rtccb+40]\n";
    if (m & RTCC_C_R9) rl += x86_rec("mov") + "r9,  qword ptr [rip + g_rtccb+48]\n";
    if (m & RTCC_C_R10) rl += x86_rec("mov") + "r10, qword ptr [rip + g_rtccb+56]\n";
    if (m & RTCC_C_R11) rl += x86_rec("mov") + "r11, qword ptr [rip + g_rtccb+64]\n";
    return rl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rtx_reestablish(unsigned m) {
    std::string s;
    if (m & RTCC_C_R9) s += MEDIUM_BINARY ? x86_Lrec(x86_rtcc_rl_bin((uint64_t)(uintptr_t)g_rtccb, RTCC_C_R9)) : x86_rtcc_rl_text(RTCC_C_R9);
    if ((m & RTCC_C_R11) && x86_diag_regs_on()) s += x86_movabs_r64("r11", (uint64_t)_.nid);
    return s;
}
std::string x86_rtcc_call_adj(const char * sym, uint64_t ptr, int adj) {
    unsigned m = x86_rtcc_clob(sym);
    if (emit_rtx_entry_is(sym) && !x86_rtx_plant_veneer()) return x86_call_ro_adj(sym, ptr, adj) + x86_rtx_reestablish(m);
    uint64_t block = (uint64_t)(uintptr_t)g_rtccb;
    if (MEDIUM_BINARY) {
        std::string call_b = x86_rtcc_call_b(ptr);
        return x86_align_assert() + x86_Lrec(x86_rtcc_wb_bin(block, m)) + x86_Lrec(call_b) + x86_gc_site_adj(X86_SITE_CALL, adj) + x86_Lrec(x86_rtcc_rl_bin(block, m));
    }
    return x86_align_assert() + x86_rtcc_wb_text(m) + x86_call_text_adj(sym, "@PLT", adj) + x86_rtcc_rl_text(m);
}
std::string x86_rtcc_call(const char * sym, uint64_t ptr) { return x86_rtcc_call_adj(sym, ptr, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_call_dc(const char * dcname, uint64_t slot) {
    if (MEDIUM_BINARY) { std::string r = x86_movabs_r64("rax", slot); std::string c; c += (char)0xFF; c += (char)0x10; r += x86_Lrec(c); return r + x86_gc_site(X86_SITE_CALL); }
    return x86_rec("call") + dcname + "\n" + x86_gc_site(X86_SITE_CALL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_ro_seal_q(int n, uint64_t val) {
    if (MEDIUM_BINARY) return x86_Drec(X86_INTERNAL_BASE + n) + x86_Lrec(u64le(val));
    return x86_reclbl(x86_internal_name(n)) + "\n" + x86_rec(".quad") + std::to_string((unsigned long long)val) + "\n";
}
static std::string x86_ro_seal_addr(int n, const char * sym, const void * addr) {
    if (MEDIUM_BINARY) return x86_Drec(X86_INTERNAL_BASE + n) + x86_Lrec(u64le((uint64_t)(uintptr_t)addr));
    return x86_ro_cell(x86_internal_name(n), std::string(sym ? sym : "0"));
}
static std::string x86_ro_seal_str(int n, const char * lit) {
    const char * s = lit ? lit : "";
    if (MEDIUM_BINARY) return x86_Drec(X86_INTERNAL_BASE + n) + x86_Lrec(u64le((uint64_t)(uintptr_t)s));
    std::string slot = x86_internal_name(n);
    std::string sbuf = slot + "_s";
    return x86_ro_cell(slot, sbuf) + sbuf + ":\n .string \"" + x86_asm_str_escape(s) + "\"\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rtcc_call_descr(const char * sym, uint64_t ptr, int slot) {
    unsigned m = x86_rtcc_clob(sym);
    uint64_t block = (uint64_t)(uintptr_t)g_rtccb;
    std::string cap = x86("mov", FRQ(slot), "rax") + x86("mov", FRQ(slot + 8), "rdx");
    if (emit_rtx_entry_is(sym) && !x86_rtx_plant_veneer()) return x86_call_ro(sym, ptr) + cap + x86_rtx_reestablish(m);
    if (MEDIUM_BINARY) {
        std::string call_b = x86_rtcc_call_b(ptr);
        return x86_align_assert() + x86_Lrec(x86_rtcc_wb_bin(block, m)) + x86_Lrec(call_b) + x86_gc_site(X86_SITE_CALL) + cap + x86_Lrec(x86_rtcc_rl_bin(block, m));
    }
    return x86_align_assert() + x86_rtcc_wb_text(m) + x86_call_text(sym, "@PLT") + cap + x86_rtcc_rl_text(m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rtcc_call_descr_ops(const char * sym, uint64_t ptr, const std::string & r0, const std::string & r8) {
    std::string cap = x86("mov", r0.c_str(), "rax") + x86("mov", r8.c_str(), "rdx");
    unsigned m = x86_rtcc_clob(sym);
    if (emit_rtx_entry_is(sym) && !x86_rtx_plant_veneer()) return x86_call_ro(sym, ptr) + cap + x86_rtx_reestablish(m);
    uint64_t block = (uint64_t)(uintptr_t)g_rtccb;
    if (MEDIUM_BINARY) {
        std::string call_b = x86_rtcc_call_b(ptr);
        return x86_align_assert() + x86_Lrec(x86_rtcc_wb_bin(block, m)) + x86_Lrec(call_b) + x86_gc_site(X86_SITE_CALL) + cap + x86_Lrec(x86_rtcc_rl_bin(block, m));
    }
    return x86_align_assert() + x86_rtcc_wb_text(m) + x86_call_text(sym, "@PLT") + cap + x86_rtcc_rl_text(m);
}
static std::string x86_rtx_verdict_cmp(int kind) {
    if (kind == RTX_VERDICT_INT) return x86("cmp", "eax", (long)(int)RTX_NOT_HANDLED_INT);
    if (kind == RTX_VERDICT_PTR) return x86("cmp", "rax", (long)-1);
    return x86("cmp", "al", (long)RTX_NOT_HANDLED);
}
std::string x86_rtx_verdict_adj(const char * sym, int adj) {
    const char * cname = 0;
    int kind = 0;
    void * caddr = emit_rtx_entry_is(sym) ? emit_rtx_twin(sym, &cname, &kind) : 0;
    if (!caddr) return std::string();
    const struct bb_label_t * done = emit_label_alloc(".L%s_vd_%d_%d", x86_boxkind(), _.x86_uid, _.x86_vd_seq++);
    return x86_rtx_verdict_cmp(kind) + x86("jne", "extlbl", (uint64_t)(uintptr_t)done) + x86_rtcc_call_adj(cname, (uint64_t)(uintptr_t)caddr, adj) + x86_def_ext(done);
}
