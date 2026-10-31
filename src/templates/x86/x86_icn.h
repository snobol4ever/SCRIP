#ifndef X86_ICN_H
#define X86_ICN_H
#include <cstdint>
#include <string>
extern "C++" {
int icn_gen_regime();
std::string x86_scan_env_store();
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_scan_env_load_rr();
typedef struct { uint64_t ptr; uint64_t len; } IcnSubjRegs_t;
std::string x86_icn_pos_set(int a, int strict, int k);
std::string x86_icn_subject_set(int a, int mode, int k);
typedef struct { int64_t v; int64_t ok; } WindowPos;
std::string x86_scan_window(int wi, int wj, int bank, int k);
std::string x86_icn_subject_result(int off);
}
#endif
