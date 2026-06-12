#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
extern "C" int  rt_trail_mark(void);
extern "C" void rt_trail_unwind(int mark);
extern "C" void rt_pl_cells_init(void ** cells, int n);
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_query_frame() {
    if (PLATFORM_X86) {
        if (_.op_sa == 0) {
            return IF(MEDIUM_TEXT,
                       x86("label", _.lbl_α)
                     + x86("comment", "BOX QUERY_FRAME α  [PL-GZ seed ABI: ζ=r12 activation, trail-mark in frame row [ζ+0], logic vars = frame cells [ζ+8+8i], x86() self-encoding]"))
                 + x86("push", "r12")
                 + x86("mov", "r12", "rdi")
                 + x86("call", "rt_trail_mark", (uint64_t)(uintptr_t)(void *)rt_trail_mark)
                 + x86("mov", FR(0), "eax")
                 + (_.op_ival > 0
                     ? x86("lea", "rdi", FR(GZ_CELL_OFF(0)))
                     + x86("mov32", "esi", (long)_.op_ival)
                     + x86("call", "rt_pl_cells_init", (uint64_t)(uintptr_t)(void *)rt_pl_cells_init)
                     : std::string())
                 + x86("jmp", "γ");
        }
        if (_.op_sb == 2) {
            return IF(MEDIUM_TEXT,
                       x86("comment", "# BOX QUERY_FRAME soft-ω landing  [PL-GZ-6b two-segment query: the (arm0 ; true) PREFIX exhausted -> unwind(query mark) + jmp B0 α (δ); the soft-fall CONTINUES into the query tail instead of ret]"))
                 + x86("def", "ω")
                 + x86("mov", "edi", FR(0))
                 + x86("call", "rt_trail_unwind", (uint64_t)(uintptr_t)(void *)rt_trail_unwind)
                 + x86("jmp", "δ");
        }
        return IF(MEDIUM_TEXT,
                   x86("comment", _.op_sb
                     ? "# BOX QUERY_FRAME γ/ω landings  [verdict-in-rax: γ=1; query tail is (G ; true) so ω = trail-unwind + PROMOTED 1 (entering the true arm with bindings undone); ret to"
                       " the query driver]"
                     : "# BOX QUERY_FRAME γ/ω landings  [verdict-in-rax: γ=1, ω=trail-unwind+0; ret to the query driver]"))
             + x86("def", "γ")
             + x86("mov32", "eax", 1L)
             + x86("pop", "r12")
             + x86("ret")
             + x86("def", "ω")
             + x86("mov", "edi", FR(0))
             + x86("call", "rt_trail_unwind", (uint64_t)(uintptr_t)(void *)rt_trail_unwind)
             + x86("mov32", "eax", _.op_sb ? 1L : 0L)
             + x86("pop", "r12")
             + x86("ret");
    }
    return std::string();
}