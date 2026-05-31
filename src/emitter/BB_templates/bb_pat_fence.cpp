/* bb_pat_fence.cpp — BB template for FENCE.  CPP return-String (GOAL-HEADQUARTERS CPP rung).
   One file per opcode. Invariant #10: no grouping with other opcodes (distinct emit shape).
   PP-A3: the with-children traversal (walk_bb_flat + label minting) lives in the driver
   flat_drive_fence (emit_bb.c); this template owns only the pure macro/zero-child emission. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
extern "C" int g_flat_node_id;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_fence_str(IR_t * pBB) {
    int nid = bb_node_id(pBB); (void)nid;
    if (PLATFORM_X86) {
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — FENCE"))
             + IF(MEDIUM_BINARY, [&]() {
                   std::string b;
                   /* lbl_α define: zero bytes — handled by bb_emit_asm_result_pairs via pair arrays. */
                   /* Driver always populates xa_bb_emit_pair_* (including the 0-children case).     */
                   for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
                       if (g_emit.xa_bb_emit_pair_jmp[i]) { b += bytes(1, "\xE9"); b += u32le(0); }
                   }
                   return b;
               }())
             + IF(MEDIUM_TEXT, g_emit.xa_bb_emit_pair_n > 0
                    ? s_1asm(emit_fmt("%s:", _.lbl_α)) + FOR(0, g_emit.xa_bb_emit_pair_n, [](int i) {
                          return (g_emit.xa_bb_emit_pair_define[i] ? emit_fmt("%s:\n", g_emit.xa_bb_emit_pair_define[i]->name) : std::string())
                               + (g_emit.xa_bb_emit_pair_jmp[i]    ? s_1asm(emit_fmt("jmp %s", g_emit.xa_bb_emit_pair_jmp[i]->name)) : std::string());
                      })
                    : s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment("# BOX FENCE()")
                     + s_1asm(emit_fmt("jmp %s", _.lbl_γ))
                     + s_1asm(emit_fmt("%s: jmp %s", _.lbl_β, _.lbl_ω)));
    }
    if (PLATFORM_JVM) {
        return jvm_class_hdr_str("fence")
             + jvm_init_ms_only_str("fence")
             + s_directive(".method public \316\261()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 5")
             + s_directive(".limit locals 1")
             + s_2asm("new", "bb/bb_box$Spec")
             + s_1asm("dup")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_fence/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("iconst_0")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V")
             + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method public \316\262()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 1")
             + s_directive(".limit locals 1")
             + s_1asm("aconst_null")
             + s_1asm("areturn")
             + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        return emit_fmt("function make_pat_%d_%d(ms) { let self = { succ: null, fail: null,\n", pBB->ival, nid)
             + "\316\261() { self.succ.\316\261(); return ''; },\n\316\262() { self.fail.\316\261(); return null; }\n}; return self; }\n";
    }
    if (PLATFORM_NET) {
        int sid = 0;
        return net_class_hdr_str(sid, nid)
             + net_ctor_none_str(sid, nid)
             + net_α_hdr_str()
             + s_2asm(".maxstack", "1")
             + net_cursor_load_str()
             + net_spec_zw_str()
             + s_1asm("ret")
             + s_1asm("}")
             + net_β_hdr_str()
             + s_2asm(".maxstack", "1")
             + net_fail_ret_str()
             + s_1asm("}")
             + s_1asm("}")
             + s_1asm(emit_fmt("    newobj     instance void pat_%d_%d::.ctor()", sid, nid));
    }
    if (PLATFORM_WASM) { return std::string("          (call $bb_fence_new)\n"); }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_fence(IR_t * pBB) {
    std::string s = bb_pat_fence_str(pBB);
    if (MEDIUM_BINARY) bb_emit_asm_result_pairs(s);
    else if (!s.empty()) emit_text_n(s.data(), s.size());
}
