#include <string>
#include <stdint.h>
#include "emitter/emit_str.h"
#include "emitter/emit_gc_site.h"
#include "ir/descr_tags_dt_platom.h"
extern "C" {
#include "templates/xa/xa_templates.h"
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_gc.h"
#include "templates/x86/x86_rsp_tracker.h"
#include "templates/x86/x86_zeta.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string xa_call_tail_land(long bytes) {
    return x86_rsp_land(bytes)
         + x86_gc_site(X86_SITE_LANDING);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string xa_call_tail_fail() {
    return x86("mov32", "eax", (long)DT_FAIL)
         + x86("xor", "edx", "edx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string xa_call_tail_result_frame(int off) {
    return x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string xa_call_tail_result_zres() {
    return x86("note", ZRESN())
         + x86("mov", ZRES(0), "rax")
         + x86("note", ZRESN())
         + x86("mov", ZRES(8), "rdx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string xa_call_tail_fail_test() {
    return x86("cmp", "al", (long)DT_FAIL)
         + x86_omega("je");
}
