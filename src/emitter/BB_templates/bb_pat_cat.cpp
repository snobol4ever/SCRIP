/* bb_pat_cat.cpp — BB template for CAT (pattern concatenation). Converted to return-string (ER wave). */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
extern int g_flat_node_id;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_cat_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — CAT"))
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
        std::string tag_s = emit_fmt("seq_%d_%d", sid, nid);
        return jvm_class_hdr_str("seq")
             + s_directive(".field private final left Lbb/bb_box;")
             + s_directive(".field private final right Lbb/bb_box;")
             + s_directive(".field private matched_start I")
             + s_directive(".field private matched_len I")
             + s_directive(".method public <init>(Lbb/bb_box$MatchState;Lbb/bb_box;Lbb/bb_box;)V")
             + s_directive(".limit stack 3") + s_directive(".limit locals 4")
             + s_1asm("aload_0") + s_1asm("aload_1")
             + s_2asm("invokespecial", "bb/bb_box/<init>(Lbb/bb_box$MatchState;)V")
             + s_1asm("aload_0") + s_1asm("aload_2") + s_2asm("putfield", "bb/bb_seq/left Lbb/bb_box;")
             + s_1asm("aload_0") + s_1asm("aload_3") + s_2asm("putfield", "bb/bb_seq/right Lbb/bb_box;")
             + s_1asm("return") + s_directive(".end method")
             + s_directive(".method public α()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 6") + s_directive(".limit locals 2")
             + s_1asm("aload_0") + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_seq/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("putfield", "bb/bb_seq/matched_start I")
             + s_1asm("aload_0") + s_1asm("iconst_0")
             + s_2asm("putfield", "bb/bb_seq/matched_len I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_seq/left Lbb/bb_box;")
             + s_2asm("invokevirtual", "bb/bb_box/α()Lbb/bb_box$Spec;") + s_1asm("astore_1")
             + s_1asm("aload_1") + s_1asm("ifnull " + tag_s + "_ω")
             + s_1asm("aload_0") + s_1asm("dup")
             + s_2asm("getfield", "bb/bb_seq/matched_len I")
             + s_1asm("aload_1") + s_2asm("getfield", "bb/bb_box$Spec/len I")
             + s_1asm("iadd") + s_2asm("putfield", "bb/bb_seq/matched_len I")
             + s_1asm("aload_0")
             + s_2asm("invokevirtual", "bb/bb_seq/rightAlpha()Lbb/bb_box$Spec;") + s_1asm("areturn")
             + s_L1asm(tag_s + "_ω:", "aconst_null") + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method public β()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 6") + s_directive(".limit locals 2")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_seq/right Lbb/bb_box;")
             + s_2asm("invokevirtual", "bb/bb_box/β()Lbb/bb_box$Spec;") + s_1asm("astore_1")
             + s_1asm("aload_1") + s_1asm("ifnull " + tag_s + "_β_right_ω")
             + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup") + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_seq/matched_start I") + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_seq/matched_len I") + s_1asm("aload_1")
             + s_2asm("getfield", "bb/bb_box$Spec/len I") + s_1asm("iadd")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
             + s_L1asm(tag_s + "_β_right_ω:", "aload_0")
             + s_2asm("invokevirtual", "bb/bb_seq/leftβ()Lbb/bb_box$Spec;") + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method private rightAlpha()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 6") + s_directive(".limit locals 2")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_seq/right Lbb/bb_box;")
             + s_2asm("invokevirtual", "bb/bb_box/α()Lbb/bb_box$Spec;") + s_1asm("astore_1")
             + s_1asm("aload_1") + s_1asm("ifnull " + tag_s + "_rA_ω")
             + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup") + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_seq/matched_start I") + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_seq/matched_len I") + s_1asm("aload_1")
             + s_2asm("getfield", "bb/bb_box$Spec/len I") + s_1asm("iadd")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
             + s_L1asm(tag_s + "_rA_ω:", "aload_0")
             + s_2asm("invokevirtual", "bb/bb_seq/leftβ()Lbb/bb_box$Spec;") + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method private leftβ()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 6") + s_directive(".limit locals 2")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_seq/left Lbb/bb_box;")
             + s_2asm("invokevirtual", "bb/bb_box/β()Lbb/bb_box$Spec;") + s_1asm("astore_1")
             + s_1asm("aload_1") + s_1asm("ifnull " + tag_s + "_lB_ω")
             + s_1asm("aload_0") + s_1asm("aload_1")
             + s_2asm("getfield", "bb/bb_box$Spec/len I")
             + s_2asm("putfield", "bb/bb_seq/matched_len I")
             + s_1asm("aload_0")
             + s_2asm("invokevirtual", "bb/bb_seq/rightAlpha()Lbb/bb_box$Spec;") + s_1asm("areturn")
             + s_L1asm(tag_s + "_lB_ω:", "aconst_null") + s_1asm("areturn")
             + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        return emit_fmt("function make_pat_%d_%d(ms) { let self = { succ: null, fail: null,\n", pBB->ival, nid)
             + "α() { const lr = self.left.α(); if (lr === null) { self.fail.α(); return; }\n"
               "let rr = self.right.α(); while (rr === null) { const lr2 = self.left.β(); if (lr2 === null) { self.fail.α(); return; } rr = self.right.α(); }\n"
               "self.succ.α(); return rr; },\n"
               "β() { let rr = self.right.β(); while (rr === null) { const lr = self.left.β(); if (lr === null) { self.fail.α(); return; } rr = self.right.α(); }\n"
               "return rr; }\n"
               "}; return self; }\n";
    }
    if (PLATFORM_NET) {
        return net_class_hdr_str(sid, nid)
             + s_directive(".field private class [boxes]Snobol4.Runtime.Boxes.IByrdBox _left")
             + s_directive(".field private class [boxes]Snobol4.Runtime.Boxes.IByrdBox _right")
             + s_directive(".field private int32 _mStart")
             + s_directive(".field private int32 _mLen")
             + s_directive(".method public specialname rtspecialname instance void .ctor(class [boxes]Snobol4.Runtime.Boxes.IByrdBox left, class [boxes]Snobol4.Runtime.Boxes.IByrdBox right) cil"
                 " managed")
             + s_1asm("{") + s_2asm(".maxstack", "2")
             + s_1asm("ldarg.0") + s_2asm("call", "instance void [mscorlib]System.Object::.ctor()")
             + s_1asm("ldarg.0") + s_1asm("ldarg.1")
             + s_1asm(emit_fmt("stfld      class [boxes]Snobol4.Runtime.Boxes.IByrdBox pat_%d_%d::_left", sid, nid))
             + s_1asm("ldarg.0") + s_1asm("ldarg.2")
             + s_1asm(emit_fmt("stfld      class [boxes]Snobol4.Runtime.Boxes.IByrdBox pat_%d_%d::_right", sid, nid))
             + s_1asm("ret") + s_1asm("}")
             + net_α_hdr_str() + s_2asm(".maxstack", "3")
             + s_2asm(".locals", "init (valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_lr, valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_rr)")
             + s_1asm("ldarg.0") + net_cursor_load_str()
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_mStart", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      class [boxes]Snobol4.Runtime.Boxes.IByrdBox pat_%d_%d::_left", sid, nid))
             + s_1asm("ldarg.1")
             + s_2asm("callvirt", "instance valuetype [boxes]Snobol4.Runtime.Boxes.Spec [boxes]Snobol4.Runtime.Boxes.IByrdBox::Alpha(class [boxes]Snobol4.Runtime.Boxes.MatchState)")
             + s_1asm("stloc.0") + s_2asm("ldloca.s", "V_lr")
             + s_2asm("call", "instance bool [boxes]Snobol4.Runtime.Boxes.Spec::get_IsFail()")
             + s_1asm(emit_fmt("brtrue     CAT_%d_%d_FAIL", sid, nid))
             + s_1asm("ldarg.0") + s_2asm("ldloca.s", "V_lr")
             + s_2asm("ldfld", "int32 [boxes]Snobol4.Runtime.Boxes.Spec::Length")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_mLen", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      class [boxes]Snobol4.Runtime.Boxes.IByrdBox pat_%d_%d::_right", sid, nid))
             + s_1asm("ldarg.1")
             + s_2asm("callvirt", "instance valuetype [boxes]Snobol4.Runtime.Boxes.Spec [boxes]Snobol4.Runtime.Boxes.IByrdBox::Alpha(class [boxes]Snobol4.Runtime.Boxes.MatchState)")
             + s_1asm("stloc.1") + s_2asm("ldloca.s", "V_rr")
             + s_2asm("call", "instance bool [boxes]Snobol4.Runtime.Boxes.Spec::get_IsFail()")
             + s_1asm(emit_fmt("brtrue     CAT_%d_%d_FAIL", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_mStart", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_mLen", sid, nid))
             + s_2asm("ldloca.s", "V_rr")
             + s_2asm("ldfld", "int32 [boxes]Snobol4.Runtime.Boxes.Spec::Length")
             + s_1asm("add") + net_spec_of_str() + s_1asm("ret")
             + s_directive(emit_fmt("  CAT_%d_%d_FAIL:", sid, nid)) + net_fail_ret_str() + s_1asm("}")
             + net_β_hdr_str() + s_2asm(".maxstack", "2")
             + s_2asm(".locals", "init (valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_rr)")
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      class [boxes]Snobol4.Runtime.Boxes.IByrdBox pat_%d_%d::_right", sid, nid))
             + s_1asm("ldarg.1")
             + s_2asm("callvirt", "instance valuetype [boxes]Snobol4.Runtime.Boxes.Spec [boxes]Snobol4.Runtime.Boxes.IByrdBox::Beta(class [boxes]Snobol4.Runtime.Boxes.MatchState)")
             + s_1asm("stloc.0") + s_2asm("ldloca.s", "V_rr")
             + s_2asm("call", "instance bool [boxes]Snobol4.Runtime.Boxes.Spec::get_IsFail()")
             + s_1asm(emit_fmt("brfalse    CAT_%d_%d_BNOK", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      class [boxes]Snobol4.Runtime.Boxes.IByrdBox pat_%d_%d::_left", sid, nid))
             + s_1asm("ldarg.1")
             + s_2asm("callvirt", "instance valuetype [boxes]Snobol4.Runtime.Boxes.Spec [boxes]Snobol4.Runtime.Boxes.IByrdBox::Beta(class [boxes]Snobol4.Runtime.Boxes.MatchState)")
             + s_1asm("ret")
             + s_directive(emit_fmt("  CAT_%d_%d_BNOK:", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_mStart", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_mLen", sid, nid))
             + s_2asm("ldloca.s", "V_rr")
             + s_2asm("ldfld", "int32 [boxes]Snobol4.Runtime.Boxes.Spec::Length")
             + s_1asm("add") + net_spec_of_str() + s_1asm("ret")
             + s_1asm("}") + s_1asm("}")
             + s_1asm(emit_fmt("newobj     instance void pat_%d_%d::.ctor(class [boxes]Snobol4.Runtime.Boxes.IByrdBox, class [boxes]Snobol4.Runtime.Boxes.IByrdBox)", sid, nid));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_cat(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_pat_cat_str(pBB, bin), bin);
}
