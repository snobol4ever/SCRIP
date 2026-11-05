#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "templates/x86/x86_call.h"
#include "emitter/emit_medium.h"
#include "emitter/bb_label.h"
#include "emitter/emit_state.h"
#include "emitter/emit_data.h"
#include "emitter/emit_regime.h"
#include "emitter/emit_str.h"
#include "lower/lower_prolog.h"
#include "runtime/rt/pl_trail_layout.h"
#include "runtime/rt/rt_pl_trail.h"
#include "templates/x86/x86_frame.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_rtcc.h"
#include "templates/x86/x86_rodata.h"
#include "templates/x86/x86_zverify.h"
#include "runtime/core/g_runtime.h"
extern "C" { void rt_pl_tr_refuse(const char *); }
std::string bb_glue_flat_enter();
std::string bb_glue_flat_leave();
extern "C" uint64_t * rt_port_counts_slot(int uid, int port, const char * label);
extern "C" void rt_pl_port_trace(const char * stem, const char * target, long ev, long uid, long ball);
static int x86_port_exit_audit_on();
static int x86_omega_trace_on();
static int x86_fc_miss(int bump);
static int x86_fc_synth_id();
static std::string x86_pl_ball_rec_probe();
static int x86_portcount_on(void);
static std::string x86_portcount(int port);
static std::string x86_pl_trace_ro(const char * dst, const char * s);
static std::string x86_pl_trace_ev(int site, int port, const char * lbl);
const char * x86_portname(int p) {
    switch (p) { case X86P_ALPHA: return _.lbl_α; case X86P_BETA: return _.lbl_β;
                 case X86P_GAMMA: return _.lbl_γ; default: return _.lbl_ω; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
struct bb_label_t * x86_portlbl(int p) {
    switch (p) { case X86P_ALPHA: return _.lbl_α_p; case X86P_BETA: return _.lbl_β_p;
                 case X86P_GAMMA: return _.lbl_γ_p; default: return _.lbl_ω_p; }
}
int x86_pl_trace_on() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_PL_TRACE"); v = (e && *e && *e != '0') ? 1 : 0; } return v; }
static int x86_port_exit_audit_on() { static int v = -1; if (v < 0) v = getenv("SCRIP_PORT_EXIT_AUDIT") ? 1 : 0; return v; }
static int x86_omega_trace_on() { static int v = -1; if (v < 0) v = getenv("SCRIP_ZETA_OMEGA_TRACE") ? 1 : 0; return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int x86_fc_on() { return _.op_fc_bytes > 0; }
static int x86_fc_miss(int bump) { static int n = 0; if (bump) n++; return n; }
int x86_fc_hit(int off) { int w = _.op_fc_bytes > 0 ? (int)_.op_fc_bytes : (int)_.op_fc_wbytes; int granted = w > 0 && _.op_fc_base >= 0;
    int hit = granted && off >= _.op_fc_base && off < _.op_fc_base + w; if (granted && !hit) { int own = _.op_own_ci > 0 && off < (int)_.op_own_ci; int fullcell = _.op_fc_bytes > 0;
    int defect = own && fullcell; if (defect) x86_fc_miss(1); static int on = -1; if (on < 0) { const char * e = getenv("SCRIP_FC_AUDIT"); on = (e && *e == '1') ? 1 : 0;
    } if (on)
    fprintf(stderr, "[FC-%s] granted box falls back to [off %d]: window=[%d,%d) w=%d ci=%ld\n", defect ? "MISS" : (own ? "FLAT-BYDESIGN" : "CROSS"), off, _.op_fc_base, _.op_fc_base + w, w,
    (long)_.op_own_ci); } return hit; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x86_fc_synth_id() {
    int id = _.x86_fc_synth--;
    if (id < X86_LITERAL_MAX)
        { fprintf(stderr, "FATAL x86_asm: a box ran its synthesized port labels down into the literal L(n) range [0,%d)\n", X86_LITERAL_MAX); abort(); }
    return id;
}
std::string x86_fc_jcc_omega(const char * mnem) {
    int id = x86_fc_synth_id();
    return x86_jcc_id(x86_jcc_invert(mnem), id)
         + x86_jmp(X86P_OMEGA)
         + x86_deflabel_id(id);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_fc_jcc_gamma(const char * mnem) {
    int id = x86_fc_synth_id();
    return x86_jcc_id(x86_jcc_invert(mnem), id)
         + x86_jmp(X86P_GAMMA)
         + x86_deflabel_id(id);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int x86_port_of(const char * s) {
    if ((unsigned char)s[0] == 0xCE) {
        switch ((unsigned char)s[1]) { case 0xB1: return X86P_ALPHA; case 0xB2: return X86P_BETA;
                                       case 0xB3: return X86P_GAMMA; }
    }
    if ((unsigned char)s[0] == 0xCF && (unsigned char)s[1] == 0x89) return X86P_OMEGA;
    return -1;
}
std::string x86_pl_tr_pop_head() { return x86("sub", "r12", X86_PL_TR_ENTRY_BYTES) + x86("mov", "rdi", RDQ("r12", 0)) + x86("test", "rdi", "rdi"); }
std::string x86_pl_tr_pop_restore() { return x86("mov", "rax", RDQ("r12", 16)) + x86("mov", "rdx", RDQ("r12", 24)) + x86("mov", RDQ("rdi", 0), "rax") + x86("mov", RDQ("rdi", 8), "rdx"); }
std::string x86_pl_tr_top_sync() { return x86("mov", "rax", "r12") + x86("and", "rax", X86_PL_TR_ARENA_MASK) + x86("mov", RDQ("rax", 0), "r12"); }
std::string x86_pl_choice_mark(const std::string & mark_mem, int l0) { return x86("note",
        "pl_choice_mark inline: sentinels at the trail top above the current B's mark are dead choices' and are dropped; then this choice's sentinel {0, HB = the heap frontier} goes under its mark")
    + x86("def", L(l0))
    + x86("mov", "rax", "r12")
    + x86("and", "rax", (long)(-X86_PL_TR_ARENA_MASK - 1))
    + x86("cmp", "rax", X86_PL_TR_HEADER_BYTES)
    + x86("jbe", L(l0 + 1))
    + x86("test", "r13", "r13")
    + x86("je", L(l0 + 2))
    + x86("cmp", "r12", RDQ("r13", 0))
    + x86("jbe", L(l0 + 1))
    + x86("def", L(l0 + 2))
    + x86("cmp", RDQ("r12", -X86_PL_TR_ENTRY_BYTES), 0L)
    + x86("jne", L(l0 + 1))
    + x86("sub", "r12", X86_PL_TR_ENTRY_BYTES)
    + x86("jmp", L(l0))
    + x86("def", L(l0 + 1))
    + x86("cmp", "rax", (long)(-X86_PL_TR_ARENA_MASK - X86_PL_TR_ENTRY_BYTES))
    + x86("jae", L(l0 + 3))
    + x86_load_got("rax", "g_runtime", (uint64_t)(uintptr_t)(void *)&g_runtime)
    + x86("mov", "rax", RDQ("rax", 0))
    + x86("mov", RDQ("r12", 0), 0L)
    + x86("mov", RDQ("r12", 8), "rax")
    + x86("mov", RDQ("r12", 16), 0L)
    + x86("mov", RDQ("r12", 24), 0L)
    + x86("add", "r12", X86_PL_TR_ENTRY_BYTES)
    + x86_pl_tr_top_sync()
    + x86("mov", mark_mem, "r12")
    + x86("jmp", L(l0 + 4))
    + x86("def", L(l0 + 3))
    + x86("mov", "rdi", "r12")
    + x86_call_ro("rt_pl_tr_refuse", (uint64_t)(uintptr_t)(void *)rt_pl_tr_refuse)
    + (MEDIUM_BINARY ? x86_Lrec(x86_b2(0x0F, 0x0B)) : x86_recn("ud2") + "\n")
    + x86("def", L(l0 + 4)); }
std::string x86_pl_tr_tidy(int l0) { return x86("note",
        "pl_tr_tidy inline (the trail rule): above the mark of r13, the choice this commit leaves, and above the activation's trail floor (the header word a resumed continuation sets: entries be"
            "low it are another owner's), every entry the trail test would not record under r13 is dropped, and an entry with a pad word "
            "(a global's restore, PL_TR_PAD_KEEP) is kept; "
            "never while a wake is pending or call_residue_vars is open, whose readers find their entries by trail offset; "
            "the choice's own tidy watermark, the trail top after the last walk under it, kept as a DT_I in the old-value words of the choice's sentinel entry (the entry below its mark: "
            "{0, HB, DT_I, watermark}, dead with the choice), starts the walk where the last one under this choice stopped when it still lies between the start and the top; a backtrack into "
            "the choice unwinds below it and the next walk starts at the mark again; so a run of commits under one choice judges each entry once, not once per commit (R-2122)")
    + x86("test", "r13", "r13")
    + x86("je", L(l0 + 9))
    + x86("mov", "rax", "r12")
    + x86("and", "rax", X86_PL_TR_ARENA_MASK)
    + x86("cmp", RDQ("rax", X86_PL_TR_WAKE_OFF), 0L)
    + x86("jne", L(l0 + 9))
    + x86("cmp", RDQ("rax", X86_PL_TR_RESIDUE_OFF), 0L)
    + x86("jne", L(l0 + 9))
    + x86("mov", "rsi", RDQ("r13", 0))
    + x86("mov", "rcx", RDQ("rax", X86_PL_TR_FLOOR_OFF))
    + x86("cmp", "rsi", "rcx")
    + x86("jae", L(l0 + 5))
    + x86("mov", "rsi", "rcx")
    + x86("def", L(l0 + 5))
    + x86("mov", "rcx", RDQ("r13", 0))
    + x86("mov", "rdx", "rcx")
    + x86("and", "rdx", (long)(-X86_PL_TR_ARENA_MASK - 1))
    + x86("cmp", "rdx", X86_PL_TR_HEADER_BYTES + X86_PL_TR_ENTRY_BYTES)
    + x86("jb", L(l0 + 6))
    + x86("cmp", RDQ("rcx", -X86_PL_TR_ENTRY_BYTES), 0L)
    + x86("jne", L(l0 + 6))
    + x86("mov", "rdx", RDQ("rcx", 24 - X86_PL_TR_ENTRY_BYTES))
    + x86("cmp", "rdx", "rsi")
    + x86("jbe", L(l0 + 6))
    + x86("cmp", "rdx", "r12")
    + x86("ja", L(l0 + 6))
    + x86("mov", "rsi", "rdx")
    + x86("def", L(l0 + 6))
    + x86("cmp", "rsi", "r12")
    + x86("jae", L(l0 + 7))
    + x86("mov", "rdi", "rsi")
    + x86("def", L(l0))
    + x86("mov", "rax", RDQ("rsi", 0))
    + x86("test", "rax", "rax")
    + x86("je", L(l0 + 3))
    + x86("cmp", RDQ("rsi", 8), 0L)
    + x86("jne", L(l0 + 2))
    + x86("cmp", "rax", "rsp")
    + x86("jbe", L(l0 + 1))
    + x86("mov", "rcx", RDQ("r13", 32))
    + x86("shr", "rcx", 8L)
    + x86("cmp", "rax", "rcx")
    + x86("jb", L(l0 + 3))
    + x86("jmp", L(l0 + 2))
    + x86("def", L(l0 + 1))
    + x86("mov", "rcx", RDQ("r13", 0))
    + x86("cmp", RDQ("rcx", -X86_PL_TR_ENTRY_BYTES), 0L)
    + x86("jne", L(l0 + 2))
    + x86("cmp", "rax", RDQ("rcx", 8 - X86_PL_TR_ENTRY_BYTES))
    + x86("jb", L(l0 + 2))
    + x86_load_got("rcx", "g_runtime", (uint64_t)(uintptr_t)(void *)&g_runtime)
    + x86("cmp", "rax", RDQ("rcx", 0))
    + x86("jb", L(l0 + 3))
    + x86("def", L(l0 + 2))
    + x86("cmp", "rsi", "rdi")
    + x86("je", L(l0 + 4))
    + x86("mov", "rdx", RDQ("rsi", 0))
    + x86("mov", RDQ("rdi", 0), "rdx")
    + x86("mov", "rdx", RDQ("rsi", 8))
    + x86("mov", RDQ("rdi", 8), "rdx")
    + x86("mov", "rdx", RDQ("rsi", 16))
    + x86("mov", RDQ("rdi", 16), "rdx")
    + x86("mov", "rdx", RDQ("rsi", 24))
    + x86("mov", RDQ("rdi", 24), "rdx")
    + x86("def", L(l0 + 4))
    + x86("add", "rdi", X86_PL_TR_ENTRY_BYTES)
    + x86("def", L(l0 + 3))
    + x86("add", "rsi", X86_PL_TR_ENTRY_BYTES)
    + x86("cmp", "rsi", "r12")
    + x86("jb", L(l0))
    + x86("mov", "r12", "rdi")
    + x86_pl_tr_top_sync()
    + x86("def", L(l0 + 7))
    + x86("mov", "rcx", RDQ("r13", 0))
    + x86("mov", "rdx", "rcx")
    + x86("and", "rdx", (long)(-X86_PL_TR_ARENA_MASK - 1))
    + x86("cmp", "rdx", X86_PL_TR_HEADER_BYTES + X86_PL_TR_ENTRY_BYTES)
    + x86("jb", L(l0 + 9))
    + x86("cmp", RDQ("rcx", -X86_PL_TR_ENTRY_BYTES), 0L)
    + x86("jne", L(l0 + 9))
    + x86("mov", RDQ("rcx", 16 - X86_PL_TR_ENTRY_BYTES), (long)DT_I)
    + x86("mov", RDQ("rcx", 24 - X86_PL_TR_ENTRY_BYTES), "r12")
    + x86("def", L(l0 + 9)); }
std::string x86_pl_header_open(int hoff, int prev_off) { return x86("mov", "rax", "rsp")
    + x86_raw_pack("rax")
    + x86("mov", FRQ(hoff + 32), "rax")
    + (prev_off >= 0 ? x86("mov", FRQ(prev_off), "r13") : std::string())
    + x86("note", "pl_header_open inline (D1): the construct's own choice header in its op slots, its mark at +0 and rsp at the open at +32; B is banked, then B := the header")
    + x86("lea", "r13", FRQ(hoff)); }
std::string x86_pl_catch_link(int rec) { return x86("mov", "rdx", "r12")
    + x86("and", "rdx", X86_PL_TR_ARENA_MASK)
    + x86("mov", "rcx", RDQ("rdx", X86_PL_TR_CATCH_OFF))
    + x86("mov", FRQ(rec), "rcx")
    + x86("note", "pl_catch_link (CTO-508): the catch record becomes the innermost, its prev the record the trail header's catch word held")
    + x86("lea", "rcx", FRQ(rec))
    + x86("mov", RDQ("rdx", X86_PL_TR_CATCH_OFF), "rcx"); }
std::string x86_pl_catch_open(int rec, struct bb_label_t * resume) { return x86("lea", "rax", "extlbl", (uint64_t)(uintptr_t)resume)
    + x86("mov", FRQ(rec + 8), "rax")
    + x86("note", "pl_catch_open (CTO-508): resume_pc is the goal arm's omega UNMARK alpha; rbp, rbx and r14 as the construct entered, no rsp word (R-C)")
    + x86("mov", FRQ(rec + 16), "rbp")
    + x86("mov", FRQ(rec + 24), "rbx")
    + x86("mov", FRQ(rec + 32), "r14")
    + x86_pl_catch_link(rec); }
std::string x86_pl_catch_unlink(int rec) { return x86("mov", "rdx", "r12")
    + x86("and", "rdx", X86_PL_TR_ARENA_MASK)
    + x86("note", "pl_catch_unlink (CTO-508): the goal arm exits, the trail header's catch word returns to this record's prev")
    + x86("mov", "rcx", FRQ(rec))
    + x86("mov", RDQ("rdx", X86_PL_TR_CATCH_OFF), "rcx"); }
std::string x86_pl_frame_choice_open(const char * fb, int kt, int l_take, int l_done, int l_mark) { return x86("note",
        "pl_frame_choice_open inline (ARCH-PROLOG-C-OUT-OF-THE-BOX 2.2): the floor word re-aimed at this frame's base; this frame becomes the youngest choice unless a younger one already is")
    + x86("mov", "rax", fb) + x86_raw_pack("rax") + x86("mov", RDQ(fb, kt - 32), "rax") + x86("lea", "rcx", RDQ(fb, kt - 64)) + x86("test", "r13", "r13") + x86("je", L(l_take)) +
    x86("cmp", "r13", "rcx") + x86("jb", L(l_done)) + x86("def", L(l_take)) + (l_mark >= 0 ? x86_pl_choice_mark(RDQ(fb, kt - 64), l_mark) : std::string()) + x86("mov", "r13", "rcx") +
    x86("def", L(l_done)); }
std::string x86_release_acts(bool below_rsp, int kind) {
    std::string s;
    const int * offs = (const int *)0;
    int n = g_emit_cfg ? emit_zrow_release(below_rsp ? 1 : 0, kind, &offs) : 0;
    if (n <= 0) return s;
    for (int i = 0; i < n; i++) {
        if (!below_rsp) {
            s += x86("mov", FRQ(offs[i]), 0L);
            continue;
        }
        s += x86("mov", "rax", FRQ(offs[i]));
        s += x86("mov", "rcx", "rax");
        s += x86("sub", "rcx", "rsp");
        s += x86("shr", "rcx", 63L);
        s += x86("sub", "rcx", 1L);
        s += x86("and", "rax", "rcx");
        s += x86("mov", FRQ(offs[i]), "rax");
    }
    return s;
}
std::string x86_pl_scc_release_check(const char * fb, int kt, int l_ok) { return x86("mov", "rax", "r12") + x86("and", "rax", X86_PL_TR_ARENA_MASK) + x86("mov", "rcx", RDQ("rax", 32)) +
    x86("test", "rcx", "rcx") + x86("je", L(l_ok)) + x86("lea", "rdx", RDQ(fb, kt)) + x86("cmp", "rcx", "rdx") + x86("jae", L(l_ok)) +
    x86_bomb("setup_call_cleanup: a frame released on its failure road still holds a linked cleanup record, or one younger") + x86("def", L(l_ok)); }
static std::string x86_pl_ball_rec_probe() { return x86("mov", "rax", "r12") + x86("and", "rax", X86_PL_TR_ARENA_MASK) + x86("mov", "rax", RDQ("rax", 32)) + x86("test", "rax", "rax"); }
std::string x86_pl_alarm_probe() { return x86("push", "rax") + x86("mov", "rax", "r12") + x86("and", "rax", X86_PL_TR_ARENA_MASK) + x86("mov", "eax", RDD("rax", 128)) + x86("test", "eax", "eax") +
    x86("pop", "rax"); }
std::string x86_pl_ball_rule(int l_go, int l_skip) {
    if (!emit_pl_scc_armed()) return x86("test", "r15", "r15") + x86("jne", L(l_skip));
    return x86("test", "r15", "r15") + x86("je", L(l_go)) + x86_pl_ball_rec_probe() + x86("je", L(l_skip)) + x86("lea", "rdx", x86_frame_top()) + x86("cmp", "rax", "rdx") +
        x86("jae", L(l_skip)) + x86("def", L(l_go));
}
std::string x86_pl_ball_rule_omega(int l_go) {
    if (!emit_pl_scc_armed()) return x86("test", "r15", "r15") + x86_omega("jne");
    return x86("test", "r15", "r15") + x86("je", L(l_go)) + x86_pl_ball_rec_probe() + x86_omega("je") + x86("lea", "rdx", x86_frame_top()) + x86("cmp", "rax", "rdx") +
        x86_omega("jae") + x86("def", L(l_go));
}
std::string x86_pl_tr_undo_prestep(int l_skip) { return x86("mov", "rax", "r12")
    + x86("and", "rax", X86_PL_TR_ARENA_MASK)
    + x86("note", "pl_tr_undo_prestep (CEO-2035 section 10.3): the first unwind since the last probe keeps UNDO_LIST as UNDO_PENDING; the unwind then restores UNDO_LIST to its tail at the mark")
    + x86("cmp", RDQ("rax", X86_PL_TR_UNDO_PENDING_OFF), 0L)
    + x86("jne", L(l_skip))
    + x86("mov", "rcx", RDQ("rax", X86_PL_TR_UNDO_LIST_OFF))
    + x86("mov", RDQ("rax", X86_PL_TR_UNDO_PENDING_OFF), "rcx")
    + x86("mov", "rcx", RDQ("rax", X86_PL_TR_UNDO_LIST_OFF + 8))
    + x86("mov", RDQ("rax", X86_PL_TR_UNDO_PENDING_OFF + 8), "rcx")
    + x86("def", L(l_skip)); }
std::string x86_pl_tr_unwind_at(const std::string & mark_mem, int l_loop, int l_done) { return x86("note", "pl_tr_unwind: pop the r12 trail to the mark, inline") + x86_pl_tr_undo_prestep(l_loop + 7) + x86("mov", "rsi", mark_mem) +
    x86("def", L(l_loop)) + x86("cmp", "rsi", "r12") + x86("jae", L(l_done)) + x86_pl_tr_pop_head() + x86("je", L(l_loop)) + x86_pl_tr_pop_restore() + x86("jmp", L(l_loop)) +
    x86("def", L(l_done)) + x86_pl_tr_top_sync(); }
int x86_trace_hooks_on() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_MON_VARS"); const char * s = getenv("SCRIP_SNO_STMTKW"); const char * m = getenv("MONITOR_BIN");
    v = (e && *e == (char)48) ? 0 : (((s && *s == (char)49) || (m && *m && *m != (char)48) || g_runtime.abi.monitor_bin) ? 1 : 0); } return v; }
int x86_diag_regs_on() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_DIAG_REGS"); v = (e && *e && *e != '0') ? 1 : 0; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x86_portcount_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_PORT_COUNTS"); v = (e && *e && *e != '0') ? 1 : 0; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_portcount(int port) {
    if (!x86_portcount_on() || (port != X86P_ALPHA && port != X86P_BETA) || !MEDIUM_BINARY) return std::string();
    uint64_t * cell = rt_port_counts_slot(_.x86_uid, port == X86P_ALPHA ? 0 : 1, x86_portname(port));
    if (!cell) return std::string();
    return x86("push", "rax") + x86("push", "rcx")
         + x86_movabs_r64("rax", (uint64_t)(uintptr_t)cell)
         + x86("mov", "rcx", RDQ("rax", 0))
         + x86("lea", "rcx", RDQ("rcx", 1))
         + x86("mov", RDQ("rax", 0), "rcx")
         + x86("pop", "rcx") + x86("pop", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_pl_trace_ro(const char * dst, const char * s) {
    if (!s) return x86("mov", dst, 0L);
    char lbl[24]; strtab_label(lbl, sizeof lbl, s);
    return x86_load_ro(dst, lbl, (uint64_t)(uintptr_t)(MEDIUM_BINARY ? ct_strdup(s) : s));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_pl_trace_ev(int site, int port, const char * lbl) {
    int def = site == X86H_DEF || site == X86H_DEF_PAIR;
    if (def ? (port != X86P_ALPHA && port != X86P_BETA) : (site != X86H_JMP || (port != X86P_GAMMA && port != X86P_OMEGA))) return std::string();
    const char * own = def ? (lbl ? lbl : x86_portname(port)) : _.lbl_α;
    size_t len = own ? strlen(own) : 0; char stem[len + 64];
    if (len > 3 && own[len - 3] == '_' && (unsigned char)own[len - 2] == 0xCE) len -= 3;
    if (len > sizeof stem - 64) len = sizeof stem - 64;
    memcpy(stem, own ? own : "", len); stem[len] = 0;
    if (_.op_sval && *_.op_sval) snprintf(stem + len, sizeof stem - len, " %.48s", _.op_sval);
    const char * tgt = def ? NULL : x86_portname(port);
    long ev = (long)port | ((long)site << 4) | ((long)_.op_node_kind << 8);
    return x86("push", "rax") + x86("push", "rax") + x86("push", "rdi") + x86("push", "rsi") + x86("push", "rdx") + x86("push", "rcx")
         + x86("push", "r8") + x86("push", "r9") + x86("push", "r10") + x86("push", "r11")
         + x86_pl_trace_ro("rdi", stem) + x86_pl_trace_ro("rsi", tgt) + x86("mov", "rdx", ev) + x86("mov", "rcx", (long)_.x86_uid) + x86("mov", "r8", "r15")
         + x86_call_ro("rt_pl_port_trace", (uint64_t)(uintptr_t)(void *)rt_pl_port_trace)
         + x86("pop", "r11") + x86("pop", "r10") + x86("pop", "r9") + x86("pop", "r8")
         + x86("pop", "rcx") + x86("pop", "rdx") + x86("pop", "rsi") + x86("pop", "rdi") + x86("pop", "rax") + x86("pop", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_port_hook(int site, int port, const char * lbl) {
    std::string s;
    if (x86_pl_trace_on()) s += x86_pl_trace_ev(site, port, lbl);
    if (site == X86H_DEF) s += x86_portcount(port);
    if (site == X86H_JMP) s += x86_port_canary();
    if ((site == X86H_JMP || site == X86H_JCC) && x86_portname(port) && x86_port_exit_audit_on() && emit_port_exit_label_promotes(x86_portname(port)))
        fprintf(stderr, "[PORT-EXIT-PROMOTION] target=%s port=%s site=%s uid=%d\n", x86_portname(port), port == X86P_GAMMA ? "γ" : port == X86P_OMEGA ? "ω" : "?", site == X86H_JMP ? "jmp" : "jcc",
            _.x86_uid);
    if (site == X86H_JMP && port == X86P_OMEGA && x86_omega_trace_on())
        fprintf(stderr, "[OMEGA-TRACE] x86_uid=%d op_omega_is_death=%s\n", _.x86_uid, _.op_omega_is_death ? "TRUE-DEATH" : "internal-alias");
    if (x86_fc_on()) {
        int zwco = _.op_fc_base < 0 && !_.op_zres;
        if (site == X86H_DEF && port == X86P_ALPHA) { s += bb_glue_flat_enter();
            for (int j = 0; j < _.op_zsticky / 16; j++) s += x86_serial_vars()
                ? x86("note", "a Prolog variable homed in this box is created here: it draws its serial from the root record's standing cell 255 and points at itself")
                + x86("mov", "rax", RDQ("r14", -PL_SERIAL_OFF)) + x86("add", RDQ("r14", -PL_SERIAL_OFF), (long)PL_SERIAL_STEP)
                + x86("mov", RDQ("rsp", _.op_zk_own + 16 * j), "rax") + x86("lea", "rax", RDQ("rsp", _.op_zk_own + 16 * j)) +
                    x86("mov", RDQ("rsp", _.op_zk_own + 16 * j + 8), "rax")
                : x86("mov", RDQ("rsp", _.op_zk_own + 16 * j), (long)DT_SNUL) + x86("mov", RDQ("rsp", _.op_zk_own + 16 * j + 8), 0L); }
        if (zwco && site == X86H_DEF && port == X86P_BETA) s += bb_glue_flat_enter();
        if (zwco && site == X86H_DEF_PAIR && port == X86P_BETA && !_.op_pair_rejoin) s += bb_glue_flat_enter();
        if (zwco && site == X86H_JMP && port == X86P_GAMMA) s += bb_glue_flat_leave();
        if (site == X86H_JMP && port == X86P_OMEGA && !_.op_wsteal) s += bb_glue_flat_leave();
    }
    if (site == X86H_DEF && port == X86P_BETA && _.op_zbgrow > 0) s += x86("sub", "rsp", (long)_.op_zbgrow);
    if (site == X86H_JMP && port == X86P_GAMMA && (_.op_zcp_dst >= 0 || (_.op_xrec & 2)))
        s += x86("mov", "rax", (_.op_xrec & 1) ? RDQ("rbp", _.op_zcp_sxf) : x86_zref(_.op_zcp_src, 1))
           + x86("mov", (_.op_xrec & 2) ? RDQ("rbp", _.op_zcp_dxf) : x86_zref(_.op_zcp_dst, 1), "rax")
           + x86("mov", "rax", (_.op_xrec & 1) ? RDQ("rbp", _.op_zcp_sxf + 8) : x86_zref(_.op_zcp_src + 8, 1))
           + x86("mov", (_.op_xrec & 2) ? RDQ("rbp", _.op_zcp_dxf + 8) : x86_zref(_.op_zcp_dst + 8, 1), "rax");
    if (site == X86H_JMP && port == X86P_GAMMA && (_.op_xrec & 4)) s += x86("mov", "rsp", RDQ("rbp", _.op_grec)) + IF(_.op_grec_pop != 0, x86_add("rsp", (long)_.op_grec_pop));
    else if (site == X86H_JMP && port == X86P_GAMMA && _.op_zgpop != 0) s += x86_add("rsp", (long)_.op_zgpop);
    if (site == X86H_JMP && port == X86P_OMEGA && (_.op_xrec & 8)) s += x86("mov", "rsp", RDQ("rbp", _.op_wrec)) + IF(_.op_wrec_pop != 0, x86_add("rsp", (long)_.op_wrec_pop));
    else if (site == X86H_JMP && port == X86P_OMEGA && _.op_wpop != 0) s += x86_add("rsp", (long)_.op_wpop);
    if (site == X86H_DEF && port == X86P_ALPHA) {
        static int on = -1;
        if (on < 0) { const char *e = getenv("SCRIP_RBX_FIELD_TRACE"); on = (e && *e == '1') ? 1 : 0; }
        if (on) fprintf(stderr, "[RBX-FIELD] fc_bytes=%ld fc_base=%ld\n", (long)_.op_fc_bytes, (long)_.op_fc_base);
    }
    if (x86_diag_regs_on() && (site == X86H_DEF || site == X86H_DEF_PAIR) && (port == X86P_ALPHA || port == X86P_BETA) && !emit_diag_regs_suppress())
        s += x86("mov", "r11", (long)_.nid);
    return s;
}
