/* bb_pat_alt.cpp — BB template for ALT (pattern alternation). Converted to return-string (ER wave). */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
extern int g_flat_node_id;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_alt_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — ALT"))
             + IF(MEDIUM_BINARY, [&]() {
                   std::string b;
                   for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
                       if (g_emit.xa_bb_emit_pair_define[i]) {
                           bin.sites.push_back((int)b.size());
                           bin.labels.push_back(g_emit.xa_bb_emit_pair_define[i]);
                           bin.is_def.push_back(true);
                       }
                       if (g_emit.xa_bb_emit_pair_jmp[i]) {
                           b += bytes(1, "\xE9");
                           bin.sites.push_back((int)b.size());
                           bin.labels.push_back(g_emit.xa_bb_emit_pair_jmp[i]);
                           bin.is_def.push_back(false);
                           b += u32le(0);
                       }
                   }
                   return b;
               }())
             + IF(MEDIUM_TEXT, FOR(0, g_emit.xa_bb_emit_pair_n, [](int i) {
                   return (g_emit.xa_bb_emit_pair_define[i] ? emit_fmt("%s:\n", g_emit.xa_bb_emit_pair_define[i]->name) : std::string())
                        + (g_emit.xa_bb_emit_pair_jmp[i]    ? s_1asm(emit_fmt("jmp %s", g_emit.xa_bb_emit_pair_jmp[i]->name)) : std::string());
               }));
    }
    int nid = bb_node_id(pBB); int sid = 0;
    if (PLATFORM_JVM) {
        std::string tag_s = emit_fmt("alt_%d_%d", sid, nid);
        return jvm_class_hdr_str("alt")
             + s_directive(".field private final children [Lbb/bb_box;")
             + s_directive(".field private final n I")
             + s_directive(".field private current I")
             + s_directive(".field private position I")
             + s_directive(".method public <init>(Lbb/bb_box$MatchState;[Lbb/bb_box;)V")
             + s_directive(".limit stack 3") + s_directive(".limit locals 3")
             + s_1asm("aload_0") + s_1asm("aload_1")
             + s_2asm("invokespecial", "bb/bb_box/<init>(Lbb/bb_box$MatchState;)V")
             + s_1asm("aload_0") + s_1asm("aload_2")
             + s_2asm("putfield", "bb/bb_alt/children [Lbb/bb_box;")
             + s_1asm("aload_0") + s_1asm("aload_2") + s_1asm("arraylength")
             + s_2asm("putfield", "bb/bb_alt/n I") + s_1asm("return")
             + s_directive(".end method")
             + s_directive(".method public α()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 3") + s_directive(".limit locals 1")
             + s_1asm("aload_0") + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_alt/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("putfield", "bb/bb_alt/position I")
             + s_1asm("aload_0") + s_1asm("iconst_1")
             + s_2asm("putfield", "bb/bb_alt/current I")
             + s_1asm("aload_0")
             + s_2asm("invokevirtual", "bb/bb_alt/tryAlpha()Lbb/bb_box$Spec;")
             + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method public β()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 4") + s_directive(".limit locals 2")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_alt/children [Lbb/bb_box;")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_alt/current I")
             + s_1asm("iconst_1") + s_1asm("isub") + s_1asm("aaload")
             + s_2asm("invokevirtual", "bb/bb_box/β()Lbb/bb_box$Spec;") + s_1asm("astore_1")
             + s_1asm("aload_1")
             + s_1asm("ifnull " + tag_s + "_β_ω")
             + s_1asm("aload_1") + s_1asm("areturn")
             + s_L1asm(tag_s + "_β_ω:", "aconst_null") + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method private tryAlpha()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 4") + s_directive(".limit locals 2")
             + s_L1asm(tag_s + "_try_loop:", "aload_0")
             + s_2asm("getfield", "bb/bb_alt/current I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_alt/n I")
             + s_1asm("if_icmpgt " + tag_s + "_try_ω")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_alt/ms Lbb/bb_box$MatchState;")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_alt/position I")
             + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_alt/children [Lbb/bb_box;")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_alt/current I")
             + s_1asm("iconst_1") + s_1asm("isub") + s_1asm("aaload")
             + s_2asm("invokevirtual", "bb/bb_box/α()Lbb/bb_box$Spec;") + s_1asm("astore_1")
             + s_1asm("aload_1")
             + s_1asm("ifnull " + tag_s + "_try_next")
             + s_1asm("aload_1") + s_1asm("areturn")
             + s_L1asm(tag_s + "_try_next:", "aload_0") + s_1asm("dup")
             + s_2asm("getfield", "bb/bb_alt/current I") + s_1asm("iconst_1") + s_1asm("iadd")
             + s_2asm("putfield", "bb/bb_alt/current I")
             + s_1asm("goto " + tag_s + "_try_loop")
             + s_L1asm(tag_s + "_try_ω:", "aconst_null") + s_1asm("areturn")
             + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        return emit_fmt("function make_pat_%d_%d(ms) { const children = self.children || []; let idx = 0; let self = { succ: null, fail: null,\n", pBB->ival, nid)
             + "α() { idx = 0; while (idx < children.length) { const r = children[idx].α(); if (r !== null) { self.succ.α(); return r; } idx++; } self.fail.α(); return null; },\n"
               "β() { idx--; if (idx >= 0 && idx < children.length) { const r = children[idx].β(); if (r !== null) { return r; } return self.β(); } self.fail.α(); return null; }\n"
               "}; return self; }\n";
    }
    if (PLATFORM_NET) {
        return net_class_hdr_str(sid, nid)
             + s_directive(".field private class [boxes]Snobol4.Runtime.Boxes.IByrdBox[] _children")
             + s_directive(".field private int32 _idx")
             + s_directive(".field private int32 _savedPos")
             + s_directive(".method public specialname rtspecialname instance void .ctor(class [boxes]Snobol4.Runtime.Boxes.IByrdBox[] children) cil managed")
             + s_1asm("{") + s_2asm(".maxstack", "2")
             + s_1asm("ldarg.0") + s_2asm("call", "instance void [mscorlib]System.Object::.ctor()")
             + s_1asm("ldarg.0") + s_1asm("ldarg.1")
             + s_1asm(emit_fmt("stfld      class [boxes]Snobol4.Runtime.Boxes.IByrdBox[] pat_%d_%d::_children", sid, nid))
             + s_1asm("ret") + s_1asm("}")
             + net_α_hdr_str() + s_2asm(".maxstack", "4")
             + s_2asm(".locals", "init (valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_r)")
             + s_1asm("ldarg.0") + s_1asm("ldc.i4.0")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_idx", sid, nid))
             + s_1asm("ldarg.0") + s_1asm("ldarg.1")
             + s_2asm("ldfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_savedPos", sid, nid))
             + s_directive(emit_fmt("  ALT_%d_%d_LOOP:", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_idx", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      class [boxes]Snobol4.Runtime.Boxes.IByrdBox[] pat_%d_%d::_children", sid, nid))
             + s_1asm("ldlen") + s_1asm("conv.i4")
             + s_1asm(emit_fmt("bge        ALT_%d_%d_FAIL", sid, nid))
             + s_1asm("ldarg.1") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_savedPos", sid, nid))
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      class [boxes]Snobol4.Runtime.Boxes.IByrdBox[] pat_%d_%d::_children", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_idx", sid, nid))
             + s_1asm("ldelem.ref") + s_1asm("ldarg.1")
             + s_2asm("callvirt", "instance valuetype [boxes]Snobol4.Runtime.Boxes.Spec [boxes]Snobol4.Runtime.Boxes.IByrdBox::Alpha(class [boxes]Snobol4.Runtime.Boxes.MatchState)")
             + s_1asm("stloc.0") + s_1asm("ldarg.0") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_idx", sid, nid))
             + s_1asm("ldc.i4.1") + s_1asm("add")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_idx", sid, nid))
             + s_2asm("ldloca.s", "V_r")
             + s_2asm("call", "instance bool [boxes]Snobol4.Runtime.Boxes.Spec::get_IsFail()")
             + s_1asm(emit_fmt("brtrue     ALT_%d_%d_LOOP", sid, nid))
             + s_1asm("ldloc.0") + s_1asm("ret")
             + s_directive(emit_fmt("  ALT_%d_%d_FAIL:", sid, nid)) + net_fail_ret_str() + s_1asm("}")
             + net_β_hdr_str() + s_2asm(".maxstack", "4")
             + s_2asm(".locals", "init (valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_r)")
             + s_directive(emit_fmt("  ALT_%d_%d_BLOOP:", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_idx", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      class [boxes]Snobol4.Runtime.Boxes.IByrdBox[] pat_%d_%d::_children", sid, nid))
             + s_1asm("ldlen") + s_1asm("conv.i4")
             + s_1asm(emit_fmt("bge        ALT_%d_%d_BFAIL", sid, nid))
             + s_1asm("ldarg.1") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_savedPos", sid, nid))
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      class [boxes]Snobol4.Runtime.Boxes.IByrdBox[] pat_%d_%d::_children", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_idx", sid, nid))
             + s_1asm("ldelem.ref") + s_1asm("ldarg.1")
             + s_2asm("callvirt", "instance valuetype [boxes]Snobol4.Runtime.Boxes.Spec [boxes]Snobol4.Runtime.Boxes.IByrdBox::Alpha(class [boxes]Snobol4.Runtime.Boxes.MatchState)")
             + s_1asm("stloc.0") + s_1asm("ldarg.0") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_idx", sid, nid))
             + s_1asm("ldc.i4.1") + s_1asm("add")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_idx", sid, nid))
             + s_2asm("ldloca.s", "V_r")
             + s_2asm("call", "instance bool [boxes]Snobol4.Runtime.Boxes.Spec::get_IsFail()")
             + s_1asm(emit_fmt("brtrue     ALT_%d_%d_BLOOP", sid, nid))
             + s_1asm("ldloc.0") + s_1asm("ret")
             + s_directive(emit_fmt("  ALT_%d_%d_BFAIL:", sid, nid)) + net_fail_ret_str()
             + s_1asm("}") + s_1asm("}")
             + s_1asm(emit_fmt("newobj     instance void pat_%d_%d::.ctor(class [boxes]Snobol4.Runtime.Boxes.IByrdBox[])", sid, nid));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_alt(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_pat_alt_str(pBB, bin), bin);
}
