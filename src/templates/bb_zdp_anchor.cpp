#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" void rt_zdp_anchor(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" std::string bb_zdp_anchor(long op, long node) {
    return x86("comment", "ZDP-ANCHOR")
         + x86("push", "rdi")
         + x86("push", "rsi")
         + x86("push", "rdx")
         + x86("mov",  "rdi", "rsp")
         + x86("add",  "rdi", 24L)
         + x86("mov",  "rsi", (long)op)
         + x86("mov",  "rdx", (long)node)
         + x86("call", "rt_zdp_anchor", (uint64_t)(uintptr_t)(void *)rt_zdp_anchor)
         + x86("pop",  "rdx")
         + x86("pop",  "rsi")
         + x86("pop",  "rdi");
}
extern "C" void rt_zdp_origin(void);
extern "C" void rt_zdp_probe(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" std::string bb_zdp_origin(long node) {
    return x86("comment", "ZDP-ORIGIN")
         + x86("push", "rdi")
         + x86("push", "rsi")
         + x86("mov",  "rdi", "rsp")
         + x86("add",  "rdi", 16L)
         + x86("mov",  "rsi", (long)node)
         + x86("call", "rt_zdp_origin", (uint64_t)(uintptr_t)(void *)rt_zdp_origin)
         + x86("pop",  "rsi")
         + x86("pop",  "rdi");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" std::string bb_zdp_probe(long op, long node, long port, long expect, long want_rbp) {
    return x86("comment", "ZDP-PROBE")
         + x86("push", "rdi")
         + x86("push", "rsi")
         + x86("push", "rdx")
         + x86("push", "rcx")
         + x86("push", "r8")
         + x86("mov",  "rdi", "rsp")
         + x86("add",  "rdi", 40L)
         + x86("mov",  "rsi", (long)op)
         + x86("mov",  "rdx", (long)node)
         + x86("mov",  "rcx", (long)expect)
         + x86("mov",  "r8",  (long)(port | (want_rbp == 1L ? 4L : 0L) | (want_rbp == 2L ? 8L : 0L)))
         + x86("call", "rt_zdp_probe", (uint64_t)(uintptr_t)(void *)rt_zdp_probe)
         + x86("pop",  "r8")
         + x86("pop",  "rcx")
         + x86("pop",  "rdx")
         + x86("pop",  "rsi")
         + x86("pop",  "rdi");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int x86_zdp_on_c(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZDP_TEARDOWN"); v = (e && *e == '1') ? 1 : 0; } return v; }
extern "C" int zzone_on_c(void) { return zzone_on(); }
