/* emit_str.cpp — CPP return-String spike helpers.  See emit_str.h. */
#include "emit_str.h"
#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <deque>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" {
#include "emit_core.h"
#include "emit_io.h"
#include "emit.h"
#include "emit_globals.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string emit_fmt(const char * f, ...) {
    va_list ap; va_start(ap, f);
    va_list ap2; va_copy(ap2, ap);
    int n = vsnprintf(nullptr, 0, f, ap); va_end(ap);
    if (n <= 0) { va_end(ap2); return std::string(); }
    std::vector<char> buf((size_t)n + 1);
    vsnprintf(buf.data(), buf.size(), f, ap2); va_end(ap2);
    return std::string(buf.data(), (size_t)n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string u8(unsigned v) { char c = (char)(uint8_t)v; return std::string(&c, 1); }
std::string u32le(uint32_t v) { char b[4] = { (char)(uint8_t)v, (char)(uint8_t)(v>>8), (char)(uint8_t)(v>>16), (char)(uint8_t)(v>>24) }; return std::string(b, 4); }
std::string u64le(uint64_t v) { return u32le((uint32_t)v) + u32le((uint32_t)(v>>32)); }
std::string bytes(size_t n, const char * lit) { return std::string(lit, n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void rt_bomb(const char * msg);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* M3-NATIVE-0 loud bomb.  bomb_intern owns message strings for the lifetime of the emitter process so */
/* the movabs in bomb_bytes points at a pointer that never invalidates (deque never reallocates nodes).*/
static const char * bomb_intern(const char * msg) {
    static std::deque<std::string> pool;
    pool.emplace_back(msg ? msg : "(no message)");
    return pool.back().c_str();
}
std::string bomb_text(const char * msg) {
    const char * m = msg ? msg : "(unimplemented arm)";
    static int seq = 0;
    int id = seq++;
    return s_comment(std::string("# BOMB: ") + m)
         + s_directive(".intel_syntax noprefix")
         + s_directive(".section .rodata")
         + s_directive(emit_fmt(".Lbomb_msg_%d:", id))
         + s_directive(std::string(".asciz ") + "\"" + gas_escape_str(m) + "\"")
         + s_directive(".section .text")
         + s_2asm("lea", emit_fmt("rdi, [rip + .Lbomb_msg_%d]", id))
         + s_1asm("call rt_bomb@PLT")
         + s_1asm("ud2");
}
std::string bomb_bytes(const char * msg) {
    const char * m   = bomb_intern(msg ? msg : "(unimplemented arm)");
    return bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)m)
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)rt_bomb)
         + bytes(2, "\xFF\xD0")
         + bytes(2, "\x0F\x0B");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* NO-BUFFERS binary path.  A MEDIUM_BINARY arm returns bb_bin_t { sites, labels, is_def, bytes }.
 * bb_emit_asm_result walks the bytes string, emitting raw bytes and patching rel32/label-def sites. */
void bb_emit_asm_result(const std::string & out, const bb_bin_t & bin) {
    if (!MEDIUM_BINARY) {
        if (!out.empty()) emit_text_n(out.data(), out.size());
        return;
    }
    int n = (int)bin.sites.size();
    int pos = 0;
    for (int i = 0; i < n; i++) {
        for (; pos < bin.sites[i]; pos++) bb_emit_byte((uint8_t)(unsigned char)out[pos]);
        if (bin.is_def[i]) {
            bb_label_define(bin.labels[i]);
        } else {
            bb_emit_patch_rel32(bin.labels[i]);
            pos += 4;
        }
    }
    for (; pos < (int)out.size(); pos++) bb_emit_byte((uint8_t)(unsigned char)out[pos]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Variant of bb_emit_asm_result for combinator templates whose BINARY arm walks
 * g_emit.xa_bb_emit_pair_*[] and emits zero-or-5 bytes per pair.  Metadata is reconstructed
 * here from the same arrays — templates stay pure (no bin.sites/labels/is_def access).
 * FACT RULE: byte production stays in the template; this helper only reconstructs patch metadata.
 * Each pair: xa_bb_emit_pair_define[i] non-NULL → 0 bytes, bb_label_define at current offset.
 *            xa_bb_emit_pair_jmp[i]    non-NULL → 1 opcode byte (\\xE9) already in out, then rel32. */
void bb_emit_asm_result_pairs(const std::string & out) {
    if (!MEDIUM_BINARY) {
        if (!out.empty()) emit_text_n(out.data(), out.size());
        return;
    }
    int pos = 0;
    int n = g_emit.xa_bb_emit_pair_n;
    for (int i = 0; i < n; i++) {
        if (g_emit.xa_bb_emit_pair_define[i]) {
            bb_label_define(g_emit.xa_bb_emit_pair_define[i]);
        }
        if (g_emit.xa_bb_emit_pair_jmp[i]) {
            bb_emit_byte((uint8_t)(unsigned char)out[pos]); pos++;
            bb_emit_patch_rel32(g_emit.xa_bb_emit_pair_jmp[i]);
            pos += 4;
        }
    }
    for (; pos < (int)out.size(); pos++) bb_emit_byte((uint8_t)(unsigned char)out[pos]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string s_1asm (const std::string & a)                                                            { return std::string(" ") + a + "\n"; }
std::string s_2asm (const std::string & a, const std::string & b)                                     { return std::string(" ") + a + " " + b + "\n"; }
std::string s_3asm (const std::string & a, const std::string & b, const std::string & c)              { return std::string(" ") + a + " " + b + " " + c + "\n"; }
std::string s_L1asm(const std::string & l, const std::string & a)                                     { return l + " " + a + "\n"; }
std::string s_L2asm(const std::string & l, const std::string & a, const std::string & b)              { return l + " " + a + " " + b + "\n"; }
std::string s_directive(const std::string & line)                                                     { return line + "\n"; }
std::string s_comment  (const std::string & line)                                                     { return line + "\n"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string jvm_push_int2_str(long v) {
    if (v == -1) return "    iconst_m1\n";
    if (v >= 0 && v <= 5) return emit_fmt("    iconst_%ld\n", v);
    if (v >= -128 && v <= 127) return emit_fmt("    bipush %ld\n", v);
    if (v >= -32768 && v <= 32767) return emit_fmt("    sipush %ld\n", v);
    return emit_fmt("    ldc %ld\n", v);
}
std::string jvm_emit_ldc_string_str(const char * s) {
    std::string result = "    ldc \"";
    if (s) {
        for (const char * p = s; *p; p++) {
            if      (*p == '"')  result += "\\\""; else if (*p == '\\') result += "\\\\";
            else if (*p == '\n') result += "\\n";
            else if (*p == '\r') result += "\\r";
            else if (*p == '\t') result += "\\t";
            else                 result += *p;
        }
    }
    result += "\"\n";
    return result;
}
std::string jvm_class_hdr_str(const char * name) {
    return emit_fmt(".class public bb/bb_%s\n"
        ".super bb/bb_box\n"
        ".inner class public static final spec inner bb/bb_box$Spec outer bb/bb_box\n"
        ".inner class public static final matchstate inner bb/bb_box$MatchState outer bb/bb_box\n", name);
}
std::string jvm_init_ms_str_str(const char * name, const char * field) {
    return emit_fmt(".method public <init>(Lbb/bb_box$MatchState;Ljava/lang/String;)V\n"
        "    .limit stack 3\n"
        "    .limit locals 3\n"
        "    aload_0\n"
        "    aload_1\n"
        "    invokespecial bb/bb_box/<init>(Lbb/bb_box$MatchState;)V\n"
        "    aload_0\n"
        "    aload_2\n"
        "    putfield bb/bb_%s/%s Ljava/lang/String;\n"
        "    return\n"
        ".end method\n", name, field);
}
std::string jvm_init_ms_only_str(const char * name) {
    (void)name;
    return ".method public <init>(Lbb/bb_box$MatchState;)V\n"
        "    .limit stack 2\n"
        "    .limit locals 2\n"
        "    aload_0\n"
        "    aload_1\n"
        "    invokespecial bb/bb_box/<init>(Lbb/bb_box$MatchState;)V\n"
        "    return\n"
        ".end method\n";
}
std::string jvm_init_ms_int_str(const char * name, const char * field) {
    return emit_fmt(".method public <init>(Lbb/bb_box$MatchState;I)V\n"
        "    .limit stack 3\n"
        "    .limit locals 3\n"
        "    aload_0\n"
        "    aload_1\n"
        "    invokespecial bb/bb_box/<init>(Lbb/bb_box$MatchState;)V\n"
        "    aload_0\n"
        "    iload_2\n"
        "    putfield bb/bb_%s/%s I\n"
        "    aload_0\n"
        "    aconst_null\n"
        "    putfield bb/bb_%s/dyn Ljava/util/function/IntSupplier;\n"
        "    return\n"
        ".end method\n", name, field, name);
}
std::string jvm_val_helper_str(const char * name) {
    return emit_fmt(".method private val()I\n"
        "    .limit stack 2\n"
        "    .limit locals 1\n"
        "    aload_0\n"
        "    getfield bb/bb_%s/dyn Ljava/util/function/IntSupplier;\n"
        "    ifnull %s_val_static\n"
        "    aload_0\n"
        "    getfield bb/bb_%s/dyn Ljava/util/function/IntSupplier;\n"
        "    invokeinterface java/util/function/IntSupplier/getAsInt()I 1\n"
        "    ireturn\n"
        "%s_val_static:\n"
        "    aload_0\n"
        "    getfield bb/bb_%s/n I\n"
        "    ireturn\n"
        ".end method\n", name, name, name, name, name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string gas_escape_str(const char * s) {
    std::string result = "\"";
    for (const char * cp = s ? s : ""; *cp; cp++) {
        unsigned char c = (unsigned char)*cp;
        if (c == '"' || c == '\\') { result += '\\'; result += (char)c; }
        else if (c >= 32 && c < 127) result += (char)c;
        else result += emit_fmt("\\%03o", c);
    }
    result += '"';
    return result;
}
std::string js_escape_string_str(const char * s) {
    std::string result = "\"";
    if (s) {
        for (; *s; s++) {
            unsigned char c = (unsigned char)*s;
            if      (c == '"')  result += "\\\""; else if (c == '\\') result += "\\\\";
            else if (c == '\n') result += "\\n";
            else if (c == '\r') result += "\\r";
            else if (c == '\t') result += "\\t";
            else if (c < 0x20 || c > 0x7e) result += emit_fmt("\\x%02x", c);
            else                 result += c;
        }
    }
    result += "\"";
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string net_escape_ldstr_str(const char * s) {
    std::string result = "    ldstr      \"";
    if (s) {
        for (const unsigned char * p = (const unsigned char *)s; *p; p++) {
            if (*p == '"')       result += "\\\""; else if (*p == '\\') result += "\\\\";
            else if (*p < 0x20 || *p == 0x7f) result += emit_fmt("\\u%04X", (unsigned)*p);
            else                 result += *p;
        }
    }
    result += "\"\n";
    return result;
}
std::string net_class_hdr_str(int sid, int nid) {
    return emit_fmt(".class nested public auto ansi beforefieldinit pat_%d_%d\n       extends [mscorlib]System.Object\n       implements [boxes]Snobol4.Runtime.Boxes.IByrdBox\n{\n", sid, nid);
}
std::string net_α_hdr_str() {
    return "  .method public virtual instance valuetype [boxes]Snobol4.Runtime.Boxes.Spec\n          Alpha(class [boxes]Snobol4.Runtime.Boxes.MatchState ms) cil managed\n  {\n";
}
std::string net_β_hdr_str() {
    return "  .method public virtual instance valuetype [boxes]Snobol4.Runtime.Boxes.Spec\n          Beta(class [boxes]Snobol4.Runtime.Boxes.MatchState ms) cil managed\n  {\n";
}
std::string net_fail_ret_str() {
    return "    ldsfld     valuetype [boxes]Snobol4.Runtime.Boxes.Spec [boxes]Snobol4.Runtime.Boxes.Spec::Fail\n    ret\n";
}
std::string net_cursor_load_str() {
    return "    ldarg.1\n    ldfld      int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor\n";
}
std::string net_ms_length_str() {
    return "    callvirt   instance int32 [boxes]Snobol4.Runtime.Boxes.MatchState::get_Length()\n";
}
std::string net_spec_of_str() {
    return "    call       valuetype [boxes]Snobol4.Runtime.Boxes.Spec [boxes]Snobol4.Runtime.Boxes.Spec::Of(int32, int32)\n";
}
std::string net_charset_class_str(int sid, int nid, const char * tag) {
    return net_class_hdr_str(sid, nid) + 
           "  .field private string _chars\n" +
           "  .method public specialname rtspecialname instance void .ctor(string chars) cil managed\n  {\n" +
           "    .maxstack 3\n    ldarg.0\n    call       instance void [mscorlib]System.Object::.ctor()\n" +
           "    ldarg.0\n    ldarg.1\n    dup\n" +
           emit_fmt("    brtrue     %s_%d_%d_NN\n", tag, sid, nid) +
           "    pop\n    ldstr      \"\"\n" +
           emit_fmt("  %s_%d_%d_NN:\n", tag, sid, nid) +
           emit_fmt("    stfld      string pat_%d_%d::_chars\n    ret\n  }\n", sid, nid);
}
std::string net_push_i4_str(int v) {
    if (v >= 0 && v <= 8)          return emit_fmt("    ldc.i4.%d\n", v);
    else if (v == -1)               return "    ldc.i4.m1\n";
    else if (v >= -128 && v <= 127) return emit_fmt("    ldc.i4.s   %d\n", v);
    else                            return emit_fmt("    ldc.i4     %d\n", v);
}
std::string net_ctor_none_str(int sid, int nid) {
    (void)sid; (void)nid;
    return "  .method public specialname rtspecialname instance void .ctor() cil managed\n"
        "  {\n"
        "    .maxstack 1\n"
        "    ldarg.0\n"
        "    call       instance void [mscorlib]System.Object::.ctor()\n"
        "    ret\n"
        "  }\n";
}
std::string net_spec_zw_str() {
    return "    call       valuetype [boxes]Snobol4.Runtime.Boxes.Spec [boxes]Snobol4.Runtime.Boxes.Spec::ZeroWidth(int32)\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string s_descr_push(const char * stackreg, int v_imm, int slen_imm,
                         const char * payreg) {
    /* Emit, for the ACTIVE layout, the store of a built descriptor at      */
    /* [stackreg] followed by stackreg += DESCR_STRIDE.  In DESCR_LAYOUT_16  */
    /* this reproduces the historical quartet exactly (offsets 0/4/8,        */
    /* stride 16).  In DESCR_LAYOUT_8 it stores the packed form and, for a   */
    /* pointer payload, bases off RBP (payload is RBP-relative offset).      */
    const char * sr = stackreg ? stackreg : "r12";
    const char * pr = payreg   ? payreg   : "rax";
    int off_v    = DESCR_OFF_V;
    int off_slen = DESCR_OFF_SLEN;
    int off_pay  = DESCR_OFF_PAYLOAD;
    int stride   = DESCR_STRIDE;
    std::string s;
    std::string m_v    = (off_v    ? emit_fmt("dword ptr [%s+%d], %d", sr, off_v,    v_imm)
                                   : emit_fmt("dword ptr [%s], %d",     sr,          v_imm));
    std::string m_slen = (off_slen ? emit_fmt("dword ptr [%s+%d], %d", sr, off_slen, slen_imm)
                                   : emit_fmt("dword ptr [%s], %d",     sr,          slen_imm));
    s += s_2asm("mov", m_v);
    s += s_2asm("mov", m_slen);
    if (g_descr_layout == DESCR_LAYOUT_8) {
        s += s_2asm("mov", (off_pay ? emit_fmt("dword ptr [%s+%d], %s", sr, off_pay, pr)
                                    : emit_fmt("dword ptr [%s], %s",     sr,          pr)));
    } else {
        s += s_2asm("mov", (off_pay ? emit_fmt("[%s+%d], %s", sr, off_pay, pr)
                                    : emit_fmt("[%s], %s",     sr,          pr)));
    }
    s += s_2asm("add", emit_fmt("%s, %d", sr, stride));
    return s;
}
