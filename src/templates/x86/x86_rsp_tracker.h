#ifndef X86_RSP_TRACKER_H
#define X86_RSP_TRACKER_H
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include "ir/gc_frame_map.h"
extern "C++" {
typedef struct { uint64_t key; int kind, delta; } x86_rsp_snap_t;
struct x86_rsp_st_t { int delta = 0, before = 0, aligned = 0, unknown = 0, dead = 0, mark = 0, mark_unknown = 0, quiet = 0; int mark9 = 0, mark9_unknown = 0, mark9_set = 0;
    std::vector<x86_rsp_snap_t> j, p; size_t jp_done = 0; };
x86_rsp_st_t & x86_rsp_S(void);
int & x86_rsp_state(int k);
void x86_rsp_reset_all(void);
void x86_rsp_jump(int kind, uint64_t key);
void x86_rsp_define(int kind, uint64_t key);
struct x86_rsp_pre_t { int delta, before, aligned, unknown, dead, mark, mark_unknown, quiet, mark9, mark9_unknown, mark9_set; size_t jn; };
static_assert(offsetof(x86_rsp_st_t, j) == 48 && sizeof(x86_rsp_st_t) == 48 + 2 * sizeof(std::vector<x86_rsp_snap_t>) + sizeof(size_t),
    "x86_rsp_pre_t saves every scalar of x86_rsp_st_t: a field added to the state must join the saved record and its restore; jp_done is the commit's progress, never rolled back");
x86_rsp_pre_t x86_rsp_prepass_open(void);
void x86_rsp_prepass_commit(const x86_rsp_pre_t & b);
std::string x86_rsp_mark_save(void);
void x86_rsp_mark9_push(void);
void x86_rsp_mark9_pop(void);
std::string x86_rsp_mark_push(void);
std::string x86_rsp_mark_pop(void);
std::string x86_rsp_land(long k);
std::string x86_rsp_goto(void);
#define g_x86_rsp_delta x86_rsp_state(0)
#define g_x86_rsp_before x86_rsp_state(1)
#define g_x86_rsp_aligned x86_rsp_state(2)
#define g_x86_rsp_unknown x86_rsp_state(3)
std::string x86_rsp_rec(long d);
std::string x86_rsp_mark(char k);
std::string x86_rsp_unk(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rsp_mark_save(const char * slot);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
uint64_t x86_rsp_hash(const std::string & nm);
}
#endif
