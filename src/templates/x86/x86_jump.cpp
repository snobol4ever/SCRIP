#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "templates/x86/x86_jump.h"
#include "emitter/emit_medium.h"
#include "emitter/bb_label.h"
#include "emitter/emit_state_xa_pair.h"
#include "emitter/emit_state_em_03_sh_api.h"
#include "emitter/emit_state_.h"
#include "emitter/emit_str.h"
#include "templates/x86/x86_call.h"
#include "templates/x86/x86_frame.h"
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_rsp_tracker.h"
#include "templates/x86/x86_zverify.h"
#include "ir/ct_arena.h"
#include "ir/ir_graph.h"
static const char * x86_tgt0();
static struct bb_label_t * x86_tgt0_p();
static const char * x86_tgt1();
static struct bb_label_t * x86_tgt1_p();
static uint8_t x86_jcc_op(const char * mnem);
static const char * x86_jcc_canon(uint8_t op);
static std::string x86_call_tgt(int t);
static int x86_internal_id(int n);
static std::string x86_ext_ptr_bytes(const void * p);
static std::string x86_pair_idx_bytes(int idx);
static std::string x86_lit_bytes(const std::string & b);
static const char * x86_tgt0() { return _.lbl_t0; }
static struct bb_label_t * x86_tgt0_p() { return _.lbl_t0_p; }
static const char * x86_tgt1() { return _.lbl_t1; }
static struct bb_label_t * x86_tgt1_p() { return _.lbl_t1_p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint8_t x86_jcc_op(const char * mnem) {
    if (!strcmp(mnem, "je") || !strcmp(mnem, "jz")) return 0x84;
    if (!strcmp(mnem, "jne") || !strcmp(mnem, "jnz")) return 0x85;
    if (!strcmp(mnem, "jb") || !strcmp(mnem, "jc") || !strcmp(mnem, "jnae")) return 0x82;
    if (!strcmp(mnem, "jae") || !strcmp(mnem, "jnc") || !strcmp(mnem, "jnb")) return 0x83;
    if (!strcmp(mnem, "jbe") || !strcmp(mnem, "jna")) return 0x86;
    if (!strcmp(mnem, "ja") || !strcmp(mnem, "jnbe")) return 0x87;
    if (!strcmp(mnem, "jo")) return 0x80;
    if (!strcmp(mnem, "jno")) return 0x81;
    if (!strcmp(mnem, "js")) return 0x88;
    if (!strcmp(mnem, "jns")) return 0x89;
    if (!strcmp(mnem, "jl") || !strcmp(mnem, "jnge")) return 0x8C;
    if (!strcmp(mnem, "jge") || !strcmp(mnem, "jnl")) return 0x8D;
    if (!strcmp(mnem, "jle") || !strcmp(mnem, "jng")) return 0x8E;
    if (!strcmp(mnem, "jg") || !strcmp(mnem, "jnle")) return 0x8F;
    fprintf(stderr, "[x86] FATAL x86_jcc_op: unknown condition code '%s' (no BINARY opcode; add it)\n", mnem); abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jcc(const char * mnem, int port) {
    if (port == X86P_OMEGA && (x86_fc_on() || _.op_wpop != 0 || (_.op_xrec & 8) || x86_pl_trace_on())) return x86_fc_jcc_omega(mnem);
    if (port == X86P_GAMMA && ((x86_fc_on() && _.op_fc_base < 0) || _.op_zgpop != 0 || _.op_zcp_dst >= 0 || (_.op_xrec & 6) || x86_pl_trace_on())) return x86_fc_jcc_gamma(mnem);
    return x86_port_hook(X86H_JCC, port)
         + (MEDIUM_BINARY ? (x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))) + x86_Jrec(port))
                          : x86_rec(mnem) + x86_portname(port) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * x86_jcc_canon(uint8_t op) {
    switch (op) {
        case 0x82: return "jb"; case 0x83: return "jae";
        case 0x80: return "jo"; case 0x81: return "jno";
        case 0x84: return "je"; case 0x85: return "jne";
        case 0x86: return "jbe"; case 0x87: return "ja";
        case 0x88: return "js"; case 0x89: return "jns";
        case 0x8C: return "jl"; case 0x8D: return "jge";
        case 0x8E: return "jle"; case 0x8F: return "jg";
    }
    fprintf(stderr, "[x86] FATAL x86_jcc_canon: unknown jcc opcode 0x%02X (x86_jcc_op grew an arm this switch does not spell)\n", (unsigned)op); abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * x86_jcc_invert(const char * m) { return x86_jcc_canon((uint8_t)(x86_jcc_op(m) ^ 1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jmp(int port) {
    return x86_port_hook(X86H_JMP, port)
         + x86_zdp_rbp_omega_at(port)
         + x86_zdp_rbp_gamma_at(port)
         + (MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE9)) + x86_Jrec(port))
                          : (x86_rec("jmp") + x86_portname(port) + "\n")) + x86_rsp_goto();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_deflabel(int port) {
    std::string s = MEDIUM_BINARY ? x86_Drec(port) : (std::string(1, '\x01') + (char)('0' + port) + x86_reclbl(x86_portname(port)) + "\n");
    return s + (port == X86P_ALPHA ? x86_zdp_rbp_pre_alpha() : std::string()) + x86_port_hook(X86H_DEF, port) + x86_zdp_probe_at(port) + x86_zdp_rbp_at(port);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jmp_tgt(int t) {
    const char * nm = (t == X86T_TGT0) ? _.lbl_t0 : _.lbl_t1;
    return (MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE9)) + x86_Jrec(t))
                         : (x86_rec("jmp") + nm + "\n")) + x86_rsp_goto();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jcc_tgt(const char * mnem, int t) {
    const char * nm = (t == X86T_TGT0) ? _.lbl_t0 : _.lbl_t1;
    return MEDIUM_BINARY ? (x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))) + x86_Jrec(t))
                         : x86_rec(mnem) + nm + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_call_tgt(int t) {
    const char * nm = (t == X86T_TGT0) ? _.lbl_t0 : _.lbl_t1;
    return MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE8)) + x86_Jrec(t))
                         : (x86_rec("call") + nm + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_lea_tgt(const char * dst, int t) {
    const char * nm = (t == X86T_TGT0) ? _.lbl_t0 : _.lbl_t1;
    if (MEDIUM_BINARY) {
        int m = x86_rnum(dst); std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x04; code += (char)rex; code += (char)0x8D; code += (char)(0x05 | ((m & 7) << 3));
        return x86_Lrec(code) + x86_Jrec(t);
    }
    return x86_rec("lea") + dst + ", [rip + " + nm + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x86_internal_id(int n) {
    if (n < 0 || n >= X86_INTERNAL_MAX)
        { fprintf(stderr, "FATAL x86_asm: internal label L(%d) out of range [0,%d) -- the two-byte record id holds X86_INTERNAL_MAX; split the box or raise it\n", n, X86_INTERNAL_MAX)
        ; abort(); }
    return X86_INTERNAL_BASE + n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * L(int n) { static char b[8][16]; static int i; i = (i + 1) & 7; snprintf(b[i], 16, "L%d", n); return b[i]; }
std::string x86_internal_name(int n) { return std::string(".Lx") + std::to_string(_.x86_uid) + "_" + std::to_string(n); }
std::string LS(int n) { return x86_internal_name(n) + "_s"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jmp_id(int n) {
    int id = x86_internal_id(n);
    return (MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE9)) + x86_Jrec(id))
                         : (x86_rec("jmp") + x86_internal_name(n) + "\n")) + x86_rsp_goto();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jcc_id(const char * mnem, int n) {
    int id = x86_internal_id(n);
    return MEDIUM_BINARY ? (x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))) + x86_Jrec(id))
                         : x86_rec(mnem) + x86_internal_name(n) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_deflabel_id(int n) {
    int id = x86_internal_id(n);
    return MEDIUM_BINARY ? x86_Drec(id) : x86_reclbl(x86_internal_name(n)) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_lea_id(const char * dst, int n) {
    int id = x86_internal_id(n);
    if (MEDIUM_BINARY) { int m = x86_rnum(dst); std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x04; code += (char)rex; code += (char)0x8D; code += (char)(0x05 | ((m & 7) << 3));
        return x86_Lrec(code) + x86_Jrec(id); }
    return x86_rec("lea") + dst + ", [rip + " + x86_internal_name(n) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jmp_reg(const char * r) {
    if (MEDIUM_BINARY) { int m = x86_rnum(r); std::string code; if (m >= 8) code += (char)0x41; code += (char)0xFF; code += (char)(0xE0 | (m & 7)); return x86_Lrec(code) + x86_rsp_goto(); }
    return x86_rec("jmp") + r + "\n" + x86_rsp_goto();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jmp_lblptr(bb_label_t * l, const char * txt) {
    if (MEDIUM_BINARY) { std::string r = x86_Lrec(x86_b1(0xE9)); r += (char)'X'; uint64_t v = (uint64_t)(uintptr_t)l; for (int j = 0; j < 8; j++) r += (char)(unsigned char)(v >> (8 * j)); return r; }
    return x86_rec("jmp") + txt + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jmpfn(const char * sym, uint64_t fp) {
    if (MEDIUM_BINARY) { std::string r = x86_movabs_r64("rax", fp); std::string c; c += (char)0xFF; c += (char)0xE0; r += x86_Lrec(c); return r; }
    return x86_rec("jmp") + sym + "@PLT\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jmp_mem(const char * base, int disp) {
    if (MEDIUM_BINARY) {
        int b = x86_rnum(base); std::string code; if (b >= 8) code += (char)0x41; code += (char)0xFF; int lo = b & 7; int sib = (lo == 4);
        if (disp == 0 && lo != 5) { code += (char)(0x20 | lo); if (sib) code += (char)0x24; }
        else if (disp >= -128 && disp <= 127) { code += (char)(0x60 | lo); if (sib) code += (char)0x24; code += (char)(disp & 0xFF); }
        else { code += (char)(0xA0 | lo); if (sib) code += (char)0x24; code += u32le((uint32_t)disp); }
        return x86_Lrec(code) + x86_rsp_goto();
    }
    char b2[disp ? fmt_len("qword ptr [%s + %d]\n", base, disp) : fmt_len("qword ptr [%s]\n", base)]; if (disp) snprintf(b2, sizeof b2, "qword ptr [%s + %d]\n", base, disp);
        else snprintf(b2, sizeof b2, "qword ptr [%s]\n", base);
    return x86_rec("jmp") + b2 + x86_rsp_goto();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_ext_ptr_bytes(const void * p) {
    uint64_t v = (uint64_t)(uintptr_t)p; std::string s; for (int i = 0; i < 8; i++) { s += (char)(unsigned char)(v & 0xFF); v >>= 8; } return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_def_ext(const struct bb_label_t * lbl) {
    if (MEDIUM_BINARY) { std::string r; r += (char)'Y'; r += x86_ext_ptr_bytes(lbl); return r; }
    return x86_reclbl(std::string(lbl && lbl->name ? lbl->name : "?")) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jmp_ext(const struct bb_label_t * lbl) {
    if (MEDIUM_BINARY) { std::string r; r += x86_Lrec(x86_b1(0xE9)); r += (char)'X'; r += x86_ext_ptr_bytes(lbl); return r + x86_rsp_goto(); }
    return x86_rec("jmp") + (lbl ? lbl->name : "?") + "\n" + x86_rsp_goto();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jcc_ext(const char * mnem, const struct bb_label_t * lbl) {
    if (MEDIUM_BINARY) { std::string r; r += x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))); r += (char)'X'; r += x86_ext_ptr_bytes(lbl); return r; }
    return x86_rec(mnem) + (lbl ? lbl->name : "?") + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_lea_ext(const char * dst, const struct bb_label_t * lbl) {
    if (MEDIUM_BINARY) { int g = x86_rnum(dst); std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x8D; c += (char)(0x05 | ((g & 7) << 3));
        std::string r = x86_Lrec(c); r += (char)'X'; r += x86_ext_ptr_bytes(lbl); return r; }
    return x86_rec("lea") + dst + ", [rip + " + (lbl && lbl->name ? lbl->name : "?") + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jmp_via_cell(const char * label, uint64_t cell) {
    if (MEDIUM_BINARY) { std::string c; c += (char)0x48; c += (char)0xB8; c += u64le(cell); return x86_Lrec(c) + x86_Lrec(std::string("\x48\x8B\x00", 3)) + x86_Lrec(std::string("\xFF\xE0", 2)); }
    return x86_rec("lea") + "rax, [rip + " + (label ? label : "??") + "]\n" + x86_rec("jmp") + "rax\n";
}
std::string x86_jmp_via_cellp(const char * label, uint64_t cell) {
    if (MEDIUM_BINARY) return x86_jmp_via_cell(label, cell);
    return x86_rec("mov") + "rax, [rip + " + (label ? label : "??") + "]\n" + x86_rec("mov") + "rax, [rax]\n" + x86_rec("jmp") + "rax\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_quad_ilbl(int n) {
    int id = x86_internal_id(n);
    if (MEDIUM_BINARY) { std::string r; r += (char)'Q'; r += (char)(unsigned char)(id & 255); r += (char)(unsigned char)((id >> 8) & 255); return r; }
    return std::string(" .quad ") + x86_internal_name(n) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_lea_rip_id(const char * reg, int n) {
    int g = x86_rnum(reg);
    int id = x86_internal_id(n);
    if (MEDIUM_BINARY) { std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x8D; c += (char)(0x05 | ((g & 7) << 3)); return x86_Lrec(c) + x86_Jrec(id); }
    return x86_rec("lea") + reg + ", [rip + " + x86_internal_name(n) + "]\n";
}
const char * PAIR(int idx) { static char b[8][16]; static int i; i = (i + 1) & 7; snprintf(b[i], 16, "P%d", idx); return b[i]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jmp_frame64(int off) {
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, 0); c += (char)0xFF; c += x86_frame_modrm(4, off); return x86_Lrec(c); }
    return x86_rec("jmp") + "qword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jmp_through_fn_cell(const char * label, uint64_t cell) {
    return (MEDIUM_BINARY ? x86("movabs", "rax", cell) : x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)0, label))
         + x86("mov", "rax", RDQ("rax", 0)) + x86("jmp", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_pair_idx_bytes(int idx) { std::string r; for (int j = 0; j < 4; j++) r += (char)(unsigned char)(((unsigned)idx) >> (8 * j)); return r; }
int x86_pair_idx_read(const std::string & s, size_t & i) { unsigned v = 0; for (int j = 0; j < 4; j++) v |= ((unsigned)(unsigned char)s[i++]) << (8 * j); return (int)v; }
bb_label_t * x86_pair_tgt(int idx) { return bb_label_fold(XA_PAIR(idx).jmp ? XA_PAIR(idx).jmp : XA_PAIR(idx).define); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_pair_jmp(int idx) {
    if (MEDIUM_BINARY) { std::string r; r += x86_Lrec(x86_b1(0xE9)); r += (char)'F'; r += x86_pair_idx_bytes(idx); return r + x86_rsp_goto(); }
    return x86_rec("jmp") + (x86_pair_tgt(idx) ? x86_pair_tgt(idx)->name : "??") + "\n" + x86_rsp_goto();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_pair_loop() {
    std::string r;
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
        if (MEDIUM_BINARY) {
            if (XA_PAIR(i).define) { r += (char)'E'; r += x86_pair_idx_bytes(i); r += x86_port_canary(); r += x86_port_hook(X86H_DEF, X86P_BETA, XA_PAIR(i).define->name); }
            if (XA_PAIR(i).jmp) { if (g_emit_cfg && g_emit_cfg->spine_cells) r += x86_port_hook(X86H_JMP, X86P_GAMMA); r += x86_Lrec(x86_b1(0xE9)); r += (char)'F'; r += x86_pair_idx_bytes(i); }
        } else {
            if (XA_PAIR(i).define) { r += emit_fmt("%s:\n", XA_PAIR(i).define->name); r += x86_port_canary(); r += x86_port_hook(X86H_DEF, X86P_BETA, XA_PAIR(i).define->name); }
            if (XA_PAIR(i).jmp) { if (g_emit_cfg && g_emit_cfg->spine_cells) r += x86_port_hook(X86H_JMP, X86P_GAMMA); r += x86_rec("jmp") + XA_PAIR(i).jmp->name + "\n"; }
        }
    }
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_deflabel_pair(int idx) {
    const char * pnm = XA_PAIR(idx).define ? XA_PAIR(idx).define->name : "??";
    if (MEDIUM_BINARY) { std::string r; r += (char)'E'; r += x86_pair_idx_bytes(idx); r += x86_port_canary(); r += x86_port_hook(X86H_DEF_PAIR, X86P_BETA, pnm); return r; }
    return emit_fmt("%s:\n", pnm) + x86_port_canary() + x86_port_hook(X86H_DEF_PAIR, X86P_BETA, pnm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jmp_pair(int idx) {
    if (MEDIUM_BINARY) { std::string r; r += x86_Lrec(x86_b1(0xE9)); r += (char)'F'; r += x86_pair_idx_bytes(idx); return r + x86_rsp_goto(); }
    return x86_rec("jmp") + (x86_pair_tgt(idx) ? x86_pair_tgt(idx)->name : "??") + "\n" + x86_rsp_goto();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_jcc_pair(const char * mnem, int idx) {
    if (MEDIUM_BINARY) { std::string r; r += x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))); r += (char)'F'; r += x86_pair_idx_bytes(idx); return r; }
    return x86_rec(mnem) + (x86_pair_tgt(idx) ? x86_pair_tgt(idx)->name : "??") + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_lit_bytes(const std::string & b) {
    if (!MEDIUM_BINARY) return b;
    std::string r; size_t i = 0;
    while (i < b.size()) { size_t k = b.size()-i; if (k > 255) k = 255; r += x86_Lrec(b.substr(i,k)); i += k; }
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
struct bb_label_t * x86_label_for(int id, std::vector<bb_label_t> & internal, std::vector<std::array<char, 16>> & inm) {
    if (id == X86T_TGT0) return _.lbl_t0_p;
    if (id == X86T_TGT1) return _.lbl_t1_p;
    if (id < X86_INTERNAL_BASE) return x86_portlbl(id);
    if ((size_t)(id - X86_INTERNAL_BASE) >= internal.size())
        { fprintf(stderr, "FATAL bb_emit_x86: record label id %d is past the prepass's %zu internal labels -- the prepass and the emit pass disagree\n", id, internal.size());
        abort(); }
    { int k = id - X86_INTERNAL_BASE; bb_label_t * l = &internal[(size_t)k]; if (l->name[0] == '\0') { snprintf(inm[(size_t)k].data(), inm[(size_t)k].size(), ".Lxi%d", k);
        l->name = inm[(size_t)k].data(); } return l; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_internal_resolve(const std::string & s) {
    static const char * const x86_greek[4] = { "\xce\xb1", "\xce\xb2", "\xce\xb3", "\xcf\x89" };
    const char * fam = _.x86_uid_kind ? _.x86_uid_kind : (_.flat_fam ? _.flat_fam : "anon");
    size_t famlen = strlen(fam);
    static const char * const adhoc_tags[] = { "bynamegenfn", "bynamefnzd", "bynamefn", "rkfnzd", "rkfn", "sig" };
    int portof[X86_INTERNAL_MAX]; for (int k = 0; k < X86_INTERNAL_MAX; k++) portof[k] = -1;
    int cur = X86P_ALPHA; size_t n = s.size();
    for (size_t i = 0; i < n; ) {
        if (X86_IS_PORTMARK(s, i, n)) { cur = s[i + 1] - '0'; i += 2; continue; }
        if (i + 3 < n && s[i] == '.' && s[i + 1] == 'L' && s[i + 2] == 'x' && x86_ir_digit(s[i + 3])) {
            size_t j = i + 3; x86_ir_num(s, j);
            if (j < n && s[j] == '_' && j + 1 < n && x86_ir_digit(s[j + 1])) {
                size_t k = j + 1; long ln = x86_ir_num(s, k); if (ln < X86_INTERNAL_MAX && portof[ln] < 0) portof[ln] = cur; i = k; continue;
            }
        }
        i++;
    }
    std::string out; out.reserve(n + 32); cur = X86P_ALPHA;
    for (size_t i = 0; i < n; ) {
        if (X86_IS_PORTMARK(s, i, n)) { cur = s[i + 1] - '0'; i += 2; continue; }
        if (i + 3 < n && s[i] == '.' && s[i + 1] == 'L' && s[i + 2] == 'x' && x86_ir_digit(s[i + 3])) {
            size_t j = i + 3; long uid = x86_ir_num(s, j);
            if (j < n && s[j] == '_' && j + 1 < n && x86_ir_digit(s[j + 1])) {
                size_t k = j + 1; long ln = x86_ir_num(s, k); int p = (ln < X86_INTERNAL_MAX && portof[ln] >= 0) ? portof[ln] : X86P_ALPHA;
                out += ".L"; out += fam; out += "_"; out += x86_greek[p]; out += "_"; out += std::to_string(uid); out += "_"; out += std::to_string(ln); i = k; continue;
            }
        }
        if (famlen && i + 2 + famlen < n && s[i] == '.' && s[i + 1] == 'L' && s.compare(i + 2, famlen, fam) == 0 && s[i + 2 + famlen] == '_') {
            size_t tp = i + 2 + famlen + 1; bool hit = false;
            for (size_t ti = 0; ti < sizeof(adhoc_tags) / sizeof(adhoc_tags[0]) && !hit; ti++) {
                size_t tl = strlen(adhoc_tags[ti]);
                if (tp + tl < n && s.compare(tp, tl, adhoc_tags[ti]) == 0 && x86_ir_digit(s[tp + tl])) {
                    size_t k = tp + tl; size_t dstart = k; x86_ir_num(s, k);
                    out += ".L"; out += fam; out += "_"; out += x86_greek[cur]; out += "_"; out += adhoc_tags[ti]; out.append(s, dstart, k - dstart);
                    if (k < n && s[k] == 'z' && !strcmp(adhoc_tags[ti], "sig")) { out += 'z'; k++; }
                    i = k; hit = true;
                }
            }
            if (hit) continue;
        }
        out += s[i++];
    }
    return out;
}
