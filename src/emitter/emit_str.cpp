#include "emit.h"
#include <cstdarg>
#include <cstdint>
#include <cstring>
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
std::string jvm_push_int2_str(long v) {
    if (v == -1) return "    iconst_m1\n";
    if (v >= 0 && v <= 5) return emit_fmt("    iconst_%ld\n", v);
    if (v >= -128 && v <= 127) return emit_fmt("    bipush %ld\n", v);
    if (v >= -32768 && v <= 32767) return emit_fmt("    sipush %ld\n", v);
    return emit_fmt("    ldc %ld\n", v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string jvm_class_hdr_str(const char * name) {
    return emit_fmt(".class public bb/bb_%s\n"
        ".super bb/bb_box\n"
        ".inner class public static final spec inner bb/bb_box$Spec outer bb/bb_box\n"
        ".inner class public static final matchstate inner bb/bb_box$MatchState outer bb/bb_box\n", name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string net_class_hdr_str(int sid, int nid) {
    return emit_fmt(".class nested public auto ansi beforefieldinit pat_%d_%d\n       extends [mscorlib]System.Object\n       implements [boxes]Snobol4.Runtime.Boxes.IByrdBox\n{\n", sid, nid);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string net_α_hdr_str() {
    return "  .method public virtual instance valuetype [boxes]Snobol4.Runtime.Boxes.Spec\n          Alpha(class [boxes]Snobol4.Runtime.Boxes.MatchState ms) cil managed\n  {\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string net_β_hdr_str() {
    return "  .method public virtual instance valuetype [boxes]Snobol4.Runtime.Boxes.Spec\n          Beta(class [boxes]Snobol4.Runtime.Boxes.MatchState ms) cil managed\n  {\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string net_fail_ret_str() {
    return "    ldsfld     valuetype [boxes]Snobol4.Runtime.Boxes.Spec [boxes]Snobol4.Runtime.Boxes.Spec::Fail\n    ret\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string net_cursor_load_str() {
    return "    ldarg.1\n    ldfld      int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string net_ms_length_str() {
    return "    callvirt   instance int32 [boxes]Snobol4.Runtime.Boxes.MatchState::get_Length()\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string net_spec_of_str() {
    return "    call       valuetype [boxes]Snobol4.Runtime.Boxes.Spec [boxes]Snobol4.Runtime.Boxes.Spec::Of(int32, int32)\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string net_push_i4_str(int v) {
    if (v >= 0 && v <= 8)          return emit_fmt("    ldc.i4.%d\n", v);
    else if (v == -1)               return "    ldc.i4.m1\n";
    else if (v >= -128 && v <= 127) return emit_fmt("    ldc.i4.s   %d\n", v);
    else                            return emit_fmt("    ldc.i4     %d\n", v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string net_spec_zw_str() {
    return "    call       valuetype [boxes]Snobol4.Runtime.Boxes.Spec [boxes]Snobol4.Runtime.Boxes.Spec::ZeroWidth(int32)\n";
}
