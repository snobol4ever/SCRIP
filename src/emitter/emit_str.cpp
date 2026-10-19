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
