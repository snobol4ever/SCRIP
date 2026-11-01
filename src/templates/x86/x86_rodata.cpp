#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include "templates/x86/x86_rodata.h"
#include "emitter/emit_medium.h"
#include "emitter/bb_label.h"
#include "emitter/emit_data.h"
#include "emitter/emit_walk.h"
#include "emitter/emit_str.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_ops.h"
std::string x86_load_ro(const char * dst, const char * label, uint64_t ptr) {
    if (MEDIUM_BINARY) {
        int m = x86_rnum(dst); std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; code += (char)rex; code += (char)(0xB8 | (m & 7)); code += u64le(ptr); return x86_Lrec(code);
    }
    if (g_emit_module_pic && label && label[0] != '.') {
        std::string sym(label), off;
        size_t plus = sym.find('+');
        if (plus != std::string::npos) { off = sym.substr(plus + 1); sym = sym.substr(0, plus); }
        std::string r = x86_rec("mov") + dst + ", qword ptr [rip + " + sym + "@GOTPCREL]\n";
        if (!off.empty() && off != "0") r += x86_rec("lea") + dst + ", [" + dst + " + " + off + "]\n";
        return r;
    }
    return x86_rec("lea") + dst + ", [rip + " + (label ? label : "??") + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_load_got(const char * dst, const char * label, uint64_t ptr) {
    if (MEDIUM_BINARY) {
        int m = x86_rnum(dst); std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; code += (char)rex; code += (char)(0xB8 | (m & 7)); code += u64le(ptr); return x86_Lrec(code);
    }
    return x86_rec("mov") + dst + ", qword ptr [rip + " + (label ? label : "??") + "@GOTPCREL]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_ro_load_q(const char * reg, int n) {
    if (MEDIUM_BINARY) {
        int m = x86_rnum(reg); uint8_t rex = 0x48; if (m >= 8) rex |= 0x04;
        std::string code; code += (char)rex; code += (char)0x8B; code += (char)((0 << 6) | ((m & 7) << 3) | 5);
        return x86_Lrec(code) + x86_Jrec(X86_INTERNAL_BASE + n);
    }
    return x86_rec("mov") + reg + ", qword ptr [rip + " + x86_internal_name(n) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_asm_str_escape(const char * s) {
    std::string o;
    for (const char * p = s; p && *p; ++p) {
        char c = *p;
        if (c == '\\' || c == '"') { o += '\\'; o += c; }
        else if (c == '\n') { o += "\\n"; }
        else if (c == '\t') { o += "\\t"; }
        else o += c;
    }
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_asm_str_escape(const char * s, size_t len) {
    std::string o;
    for (size_t i = 0; i < len; ++i) {
        char c = s[i];
        if (c == '\\' || c == '"') { o += '\\'; o += c; }
        else if (c == '\n') { o += "\\n"; }
        else if (c == '\t') { o += "\\t"; }
        else if (c == '\0') { o += "\\000"; }
        else o += c;
    }
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_ro_open(void) { return x86_rec(".pushsection") + ".data.rel.ro,\"aw\",@progbits; .p2align 3; "; }
std::string x86_ro_cell(const std::string & name, const std::string & target) { return x86_ro_open() + name + ": .quad " + target + "; .popsection\n"; }
std::string x86_ro_rec_head(const struct bb_label_t * lbl, uint64_t v) {
    if (MEDIUM_BINARY) return x86_def_ext(lbl) + x86_Lrec(u64le(v));
    return x86_ro_open() + (lbl && lbl->name ? lbl->name : "?") + ": .quad " + std::to_string((unsigned long long)v) + "\n";
}
std::string x86_ro_rec_tail_ilbl(int n) {
    if (MEDIUM_BINARY) return x86_quad_ilbl(n);
    return std::string(" .quad ") + x86_internal_name(n) + "; .popsection\n";
}
std::string x86_ro_rec_tail_q(uint64_t v) {
    if (MEDIUM_BINARY) return x86_Lrec(u64le(v));
    return std::string(" .quad ") + std::to_string((unsigned long long)v) + "; .popsection\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_strtab_lbl(const char * s) {
    char lbl[24]; strtab_label(lbl, sizeof lbl, s);
    return std::string(lbl);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rodata_str_lea(const char * reg, const char * str, const char * stem) {
    std::string fl = std::string(".L") + x86_boxkind() + stem + std::to_string(g_flat_node_id++);
    return x86("directive", ".section .rodata")
         + x86("directive", (fl + ": .string \"" + std::string(str) + "\"").c_str())
         + x86("directive", ".section .text")
         + x86("directive", ".intel_syntax noprefix")
         + x86("lea", reg, "[rip + __]", (uint64_t)(uintptr_t)str, fl.c_str());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
uint64_t x86_cset_word(const char * s, long n, int k) {
    uint64_t w = 0;
    for (long i = 0; s && i < n; i++) if (((unsigned char)s[i] >> 6) == k) w |= 1ull << ((unsigned char)s[i] & 63);
    return w;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
uint64_t x86_csettab_ptr(const char * cset) {
    char lbl[24];
    return (uint64_t)(uintptr_t)csettab_label(lbl, sizeof lbl, cset);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_csettab_lbl(const char * cset) {
    char lbl[24]; csettab_label(lbl, sizeof lbl, cset);
    return std::string(lbl);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_load_ro_str(const char * dst, const char * s) {
    const char * str = s ? s : "";
    const char * lbl = emit_intern_str(str);
    char lblbuf[24];
    if (!MEDIUM_BINARY && (!lbl || !lbl[0])) { strtab_label(lblbuf, sizeof lblbuf, str); lbl = lblbuf; }
    return x86_load_ro(dst, lbl, (uint64_t)(uintptr_t)(const void *)str);
}
