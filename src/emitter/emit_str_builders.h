#ifndef EMIT_STR_BUILDERS_H
#define EMIT_STR_BUILDERS_H
#include <string>
#include <cstdio>
#include <sstream>
namespace EmitStr {
inline std::string jvm_push_int2_str(long v) {
    char buf[256];
    if (v == -1) { snprintf(buf, sizeof buf, "    iconst_m1\n"); return buf; }
    if (v >= 0 && v <= 5) { snprintf(buf, sizeof buf, "    iconst_%ld\n", v); return buf; }
    if (v >= -128 && v <= 127) { snprintf(buf, sizeof buf, "    bipush %ld\n", v); return buf; }
    if (v >= -32768 && v <= 32767) { snprintf(buf, sizeof buf, "    sipush %ld\n", v); return buf; }
    snprintf(buf, sizeof buf, "    ldc %ld\n", v);
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string jvm_emit_ldc_string_str(const char * s) {
    std::string result = "    ldc \"";
    if (s) {
        for (const char * p = s; *p; p++) {
            if      (*p == '"')  result += "\\\"";
            else if (*p == '\\') result += "\\\\";
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
inline std::string js_escape_string_str(const char * s) {
    std::string result = "\"";
    if (s) {
        for (; *s; s++) {
            unsigned char c = (unsigned char)*s;
            if      (c == '"')  result += "\\\"";
            else if (c == '\\') result += "\\\\";
            else if (c == '\n') result += "\\n";
            else if (c == '\r') result += "\\r";
            else if (c == '\t') result += "\\t";
            else if (c < 0x20 || c > 0x7e) {
                char hex[16];
                snprintf(hex, sizeof hex, "\\x%02x", c);
                result += hex;
            } else {
                result += c;
            }
        }
    }
    result += "\"";
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
template<typename... Args>
inline std::string format_str(const char * fmt, Args... args) {
    char buf[4096];
    snprintf(buf, sizeof buf, fmt, args...);
    return std::string(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string emit_comment_str(const char * line) {
    return std::string(line) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string emit_directive_str(const char * line) {
    return std::string(line) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
using namespace EmitStr;
#endif
