#ifndef X86_GC_H
#define X86_GC_H
#include <string>
#include "templates/x86/x86_frame.h"
extern "C++" {
#define X86_SITE_CALL 1
#define X86_SITE_LANDING 2
#define X86_SITE_RES 3
#define X86_SITE_MATCH_ENTER 4
#define X86_SITE_MATCH_LEAVE 5
#define X86_SITE_FN_EXIT 6
#define X86_SITE_RET_ROAD 7
#define X86_ADJ_ALIGNED (-1)
std::string x86_gc_site_adj(int kind, int adj);
std::string x86_gc_site(int kind);
std::string x86_gc_site_raw(int kind, int rule, int depth);
void x86_text_sites_scan(const std::string & r, bool real = true);
std::string x86_cap_raise_call();
#define x86_cap_raise(fail) (x86_resumable_frame() ? x86_cap_raise_call() + (fail) : std::string())
std::string x86_rt_gc_poll_at(const char * f, int l, const std::string & raise = std::string());
#define x86_rt_gc_poll() x86_rt_gc_poll_at(__FILE__, __LINE__)
#define x86_rt_gc_poll_or(fail) x86_rt_gc_poll_at(__FILE__, __LINE__, x86_cap_raise(fail))
#define x86_rt_gc_poll_fails() x86_rt_gc_poll_at(__FILE__, __LINE__, x86_cap_raise(std::string()))
std::string x86_rt_gc_poll_res(const std::string & raise = std::string(), int reg = 0);
std::string x86_rt_gc_poll_rec_sigma(int keep_rax, int reg = 0);
std::string x86_rt_gc_poll_rec_name(const char * preg);
std::string x86_rt_gc_poll_rec_sigma_needle();
std::string x86_rt_gc_poll_rec_subject_new();
std::string x86_rt_gc_poll_rec_sigma_word(int keep_rax, int reg = 0);
std::string x86_rt_gc_poll_rec_sigma_pair(int ptr_in_rax, int lbl);
std::string x86_rt_gc_poll_rec_res(int reg = 0);
}
#endif
