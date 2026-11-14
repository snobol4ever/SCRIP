#ifndef EMITTER_EMIT_STR_H
#define EMITTER_EMIT_STR_H
#ifndef __cplusplus
#error emit_str.h is C++ only
#endif
#include <string>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <initializer_list>
std::string emit_fmt(const char * f, ...) __attribute__((format(printf, 1, 2)));
std::string u8 (unsigned v);
std::string u32le(uint32_t v);
std::string u64le(uint64_t v);
std::string bytes(size_t n, const char * lit);
std::string gas_escape_str(const char * s);
#define IF(c, ...) ((c) ? (__VA_ARGS__) : std::string())
#define IF_M3(...) IF(MEDIUM_BINARY, __VA_ARGS__)
#define IF_M4(...) IF(MEDIUM_TEXT, __VA_ARGS__)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
template<typename F> inline std::string FOR(int lo, int hi, F f) { std::string r; for (int i = lo; i < hi; i++) r += f(i); return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
template<typename F> inline std::string emit_for(int lo, int hi, F f) { return FOR(lo, hi, f); }
std::string emit_seq(std::initializer_list<std::string> parts);
long emit_lit_d(const char * s, long k);
uint64_t emit_double_bits(double d);
#endif
