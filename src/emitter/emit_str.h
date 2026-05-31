#ifndef EMIT_STR_H
#define EMIT_STR_H
#ifdef __cplusplus
#include <string>
#include <vector>
#include <cstdio>
#include <cstdint>
extern "C" {
#include "emit_core.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string emit_fmt(const char * f, ...) __attribute__((format(printf, 1, 2)));
std::string u8   (unsigned v);
std::string u32le(uint32_t v);
std::string u64le(uint64_t v);
std::string bytes(size_t n, const char * lit);
std::string bomb_text (const char * msg);
std::string bomb_bytes(const char * msg);
std::string s_1asm  (const std::string & a);
std::string s_2asm  (const std::string & a, const std::string & b);
std::string s_3asm  (const std::string & a, const std::string & b, const std::string & c);
std::string s_L1asm (const std::string & l, const std::string & a);
std::string s_L2asm (const std::string & l, const std::string & a, const std::string & b);
std::string s_directive (const std::string & line);
std::string s_comment   (const std::string & line);
inline std::string s_1asm_c  (const char * a)                                              { return s_1asm(std::string(a ? a : "")); }
inline std::string s_2asm_c  (const char * a, const char * b)                              { return s_2asm(std::string(a ? a : ""), std::string(b ? b : "")); }
inline std::string s_3asm_c  (const char * a, const char * b, const char * c)              { return s_3asm(std::string(a ? a : ""), std::string(b ? b : ""), std::string(c ? c : "")); }
inline std::string s_L1asm_c (const char * l, const char * a)                              { return s_L1asm(std::string(l ? l : ""), std::string(a ? a : "")); }
inline std::string s_L2asm_c (const char * l, const char * a, const char * b)              { return s_L2asm(std::string(l ? l : ""), std::string(a ? a : ""), std::string(b ? b : "")); }
inline std::string s_directive_c(const char * line)                                        { return s_directive(std::string(line ? line : "")); }
inline std::string s_comment_c  (const char * line)                                        { return s_comment(std::string(line ? line : "")); }
struct bb_bin_t {
    std::vector<int>          sites;
    std::vector<bb_label_t *> labels;
    std::vector<bool>         is_def;
    std::string               bytes;
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_emit_asm_result(const std::string & out, const bb_bin_t & bin);
void bb_emit_asm_result_pairs(const std::string & out);
std::string jvm_push_int2_str(long v);
std::string jvm_emit_ldc_string_str(const char * s);
std::string js_escape_string_str(const char * s);
std::string gas_escape_str(const char * s);
std::string wasm_emit_data_segments_str(void);
std::string jvm_class_hdr_str(const char * name);
std::string jvm_init_ms_str_str(const char * name, const char * field);
std::string jvm_init_ms_only_str(const char * name);
std::string jvm_init_ms_int_str(const char * name, const char * field);
std::string jvm_val_helper_str(const char * name);
std::string net_escape_ldstr_str(const char * s);
std::string net_class_hdr_str(int sid, int nid);
std::string net_α_hdr_str();
std::string net_β_hdr_str();
std::string net_fail_ret_str();
std::string net_cursor_load_str();
std::string net_ms_length_str();
std::string net_spec_of_str();
std::string net_charset_class_str(int sid, int nid, const char * tag);
std::string net_push_i4_str(int v);
std::string net_ctor_none_str(int sid, int nid);
std::string net_spec_zw_str();
#define IF(c, ...) ((c) ? (__VA_ARGS__) : std::string())
template<typename F>
inline std::string FOR(int lo, int hi, F f) {
    std::string r;
    for (int i = lo; i < hi; i++) r += f(i);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
template<typename F>
inline std::string emit_for(int lo, int hi, F f) { return FOR(lo, hi, f); }
#endif
#endif
