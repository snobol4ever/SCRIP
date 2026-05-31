/* bb_pat_span.cpp — BB template for SPAN. Converted to return-string (ER wave). */
#include <string>
#include <deque>
#include <cstring>
#include "emit_str.h"
extern "C" {
void *rt_cs_new(const char *chars);
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_span_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        const char *cs = pBB->sval ? pBB->sval : "";
        const char *cs_label = emit_intern_str(cs);
        std::string z   = emit_fmt(".Lspan%d_z", g_emit.bb_cs_id);
        std::string lp  = emit_fmt(".Lspan%d_loop", g_emit.bb_cs_id);
        std::string dn  = emit_fmt(".Lspan%d_done", g_emit.bb_cs_id);
        bin = { {}, {}, {} };
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — SPAN"))
             + IF(MEDIUM_BINARY, [&]() -> std::string {
                   /* SPAN BINARY arm (SBL-SPAN-2). Two process-lifetime deque-int scratch slots:
                      z_slot (matched len @active), zo_slot (origin Δ for β undo).
                      cs_addr = charset string ptr (from pBB->sval via rt_cs_new caller).
                      strchr(cs,ch)!=NULL → char in set → keep spanning.
                      Internal pre-patches: jge@58→+65, je@105→+18, jmp@123→-98.
                      External sites: {143→ω, 168→γ, 172→β(def), 192→ω, 216→γ}. 220 bytes. */
                   static std::deque<int> _pool;
                   _pool.emplace_back(0); int *z_slot  = &_pool.back();
                   _pool.emplace_back(0); int *zo_slot = &_pool.back();
                   uint64_t za  = (uint64_t)(uintptr_t)z_slot;
                   uint64_t zoa = (uint64_t)(uintptr_t)zo_slot;
                   uint64_t csa = (uint64_t)(uintptr_t)(const void *)(pBB->sval ? pBB->sval : "");
                   uint64_t strchr_addr;
                   { const char *(*fp)(const char *, int) = strchr; strchr_addr = (uint64_t)(uintptr_t)(void *)fp; }
                   std::string b;
                   /* α: z=0; load Σ into r11 */
                   b += bytes(2,"\x48\xB9") + u64le(za);                  /* [0]   movabs rcx, z_slot */
                   b += bytes(6,"\xC7\x01\x00\x00\x00\x00");                /* [10]  mov dword[rcx], 0 */
                   b += bytes(2,"\x48\xB8") + u64le(TEMPLATE_ADDR_SIGMA);  /* [16]  movabs rax, &Σ */
                   b += bytes(3,"\x4C\x8B\x18");                            /* [26]  mov r11, [rax] */
                   /* loop at [29] */
                   b += bytes(3,"\x41\x8B\x02");                            /* [29]  mov eax, [r10] */
                   b += bytes(2,"\x48\xB9") + u64le(za);                  /* [32]  movabs rcx, z_slot */
                   b += bytes(2,"\x03\x01");                                /* [42]  add eax, [rcx] */
                   b += bytes(2,"\x48\xB9") + u64le(TEMPLATE_ADDR_SIGLEN);/* [44]  movabs rcx, &Σlen */
                   b += bytes(2,"\x3B\x01");                                /* [54]  cmp eax, [rcx] */
                   b += bytes(2,"\x0F\x8D") + u32le(65u);                 /* [56]  jge done (+65→127) */
                   b += bytes(3,"\x48\x63\xC8");                            /* [62]  movsxd rcx, eax */
                   b += bytes(5,"\x41\x0F\xB6\x34\x0B");                    /* [65]  movzx esi, byte[r11+rcx] */
                   b += bytes(2,"\x48\xBF") + u64le(csa);                 /* [70]  movabs rdi, cs_addr */
                   b += bytes(2,"\x41\x52");                                /* [80]  push r10 */
                   b += bytes(2,"\x41\x53");                                /* [82]  push r11 */
                   b += bytes(2,"\x48\xB8") + u64le(strchr_addr);         /* [84]  movabs rax, strchr */
                   b += bytes(2,"\xFF\xD0");                                /* [94]  call rax */
                   b += bytes(2,"\x41\x5B");                                /* [96]  pop r11 */
                   b += bytes(2,"\x41\x5A");                                /* [98]  pop r10 */
                   b += bytes(3,"\x48\x85\xC0");                            /* [100] test rax, rax */
                   b += bytes(2,"\x0F\x84") + u32le(18u);                 /* [103] je done (+18→127) */
                   b += bytes(2,"\x48\xB9") + u64le(za);                  /* [109] movabs rcx, z_slot */
                   b += bytes(3,"\x83\x01\x01");                            /* [119] add dword[rcx], 1 */
                   b += bytes(1,"\xE9") + u32le((uint32_t)(int32_t)-98);   /* [122] jmp loop (-98→29) */
                   /* done at [127] */
                   b += bytes(2,"\x48\xB9") + u64le(za);                  /* [127] movabs rcx, z_slot */
                   b += bytes(2,"\x8B\x01");                                /* [137] mov eax, [rcx] */
                   b += bytes(2,"\x85\xC0");                                /* [139] test eax, eax */
                   b += bytes(2,"\x0F\x8E") + u32le(0);                   /* [141] jle ω  site[0]@143 */
                   b += bytes(3,"\x41\x8B\x12");                            /* [147] mov edx, [r10] */
                   b += bytes(2,"\x48\xB9") + u64le(zoa);                 /* [150] movabs rcx, zo_slot */
                   b += bytes(2,"\x89\x11");                                /* [160] mov [rcx], edx */
                   b += bytes(2,"\x01\xC2");                                /* [162] add edx, eax */
                   b += bytes(3,"\x41\x89\x12");                            /* [164] mov [r10], edx */
                   b += bytes(1,"\xE9") + u32le(0);                        /* [167] jmp γ  site[1]@168 */
                   /* lbl_β at [172] */
                   b += bytes(2,"\x48\xB9") + u64le(za);                  /* [172] movabs rcx, z_slot */
                   b += bytes(2,"\x8B\x01");                                /* [182] mov eax, [rcx] */
                   b += bytes(3,"\x83\xE8\x01");                            /* [184] sub eax, 1 */
                   b += bytes(3,"\x83\xF8\x01");                            /* [187] cmp eax, 1 */
                   b += bytes(2,"\x0F\x8C") + u32le(0);                   /* [190] jl ω  site[2]@192 */
                   b += bytes(2,"\x89\x01");                                /* [196] mov [rcx], eax */
                   b += bytes(2,"\x48\xB9") + u64le(zoa);                 /* [198] movabs rcx, zo_slot */
                   b += bytes(2,"\x8B\x11");                                /* [208] mov edx, [rcx] */
                   b += bytes(2,"\x01\xC2");                                /* [210] add edx, eax */
                   b += bytes(3,"\x41\x89\x12");                            /* [212] mov [r10], edx */
                   b += bytes(1,"\xE9") + u32le(0);                        /* [215] jmp γ  site[3]@216 */
                   /* end [220] */
                   bin = { {143, 168, 172, 192, 216},
                           {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p, _.lbl_γ_p},
                           {false, false, true, false, false} };
                   return b;
               }())
             + IF(MEDIUM_TEXT,
                   s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment("# BOX SPAN()")
                 + s_directive(".section .data")
                 + s_directive(z + ": .long 0")
                 + s_directive(".long 0")
                 + s_directive(".section .text")
                 + s_directive(".intel_syntax noprefix")
                 + s_2asm("mov", "dword ptr [rip + " + z + " + 0], 0")
                 + s_2asm("lea", "r11, [rip + \xCE\xA3]")
                 + s_2asm("mov", "r11, [r11]")
                 + s_1asm(emit_fmt("%s:", lp.c_str()))
                 + s_2asm("mov", "eax, [r10]")
                 + s_2asm("add", "eax, dword ptr [rip + " + z + " + 0]")
                 + s_2asm("lea", "rcx, [rip + \xCE\xA3" "len]")
                 + s_2asm("cmp", "eax, [rcx]")
                 + s_2asm("jge", dn.c_str())
                 + s_2asm("movsxd", "rcx, eax")
                 + s_2asm("movzx", "esi, byte ptr [r11+rcx]")
                 + s_2asm("lea", emit_fmt("rdi, [rip + %s]", cs_label ? cs_label : "??"))
                 + s_2asm("push", "r10")
                 + s_2asm("push", "r11")
                 + s_2asm("call", "strchr@PLT")
                 + s_2asm("pop", "r11")
                 + s_2asm("pop", "r10")
                 + s_2asm("test", "rax, rax")
                 + s_2asm("jz", dn.c_str())
                 + s_2asm("add", "dword ptr [rip + " + z + " + 0], 1")
                 + s_2asm("jmp", lp.c_str())
                 + s_1asm(emit_fmt("%s:", dn.c_str()))
                 + s_2asm("mov", "ecx, dword ptr [rip + " + z + " + 0]")
                 + s_2asm("cmp", "ecx, 0")
                 + s_2asm("jle", _.lbl_ω)
                 + s_2asm("mov", "eax, [r10]")
                 + s_2asm("mov", "dword ptr [rip + " + z + " + 4], eax")
                 + s_2asm("add", "eax, ecx")
                 + s_2asm("mov", "[r10], eax")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_1asm(emit_fmt("%s:", _.lbl_β))
                 + s_2asm("mov", "eax, dword ptr [rip + " + z + " + 4]")
                 + s_2asm("mov", "ecx, dword ptr [rip + " + z + " + 0]")
                 + s_2asm("sub", "ecx, 1")
                 + s_2asm("cmp", "ecx, 1")
                 + s_2asm("jl", _.lbl_ω)
                 + s_2asm("mov", "dword ptr [rip + " + z + " + 0], ecx")
                 + s_2asm("add", "eax, ecx")
                 + s_2asm("mov", "[r10], eax")
                 + s_2asm("jmp", _.lbl_γ));
    }
    if (PLATFORM_JVM) {
        return jvm_class_hdr_str("span")
             + s_directive(".field private final chars Ljava/lang/String;")
             + s_directive(".field private matched_len I")
             + jvm_init_ms_str_str("span", "chars")
             + s_directive(".method public α()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 6")
             + s_directive(".limit locals 3")
             + s_1asm("aload_0") + s_1asm("iconst_0")
             + s_2asm("putfield", "bb/bb_span/matched_len I")
             + s_L1asm(emit_fmt("span_%d_%d", 0, bb_node_id(pBB)) + "_loop:", "aload_0")
             + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_1asm("iadd") + s_1asm("istore_1") + s_1asm("iload_1")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/ω I")
             + s_1asm("if_icmpge " + emit_fmt("span_%d_%d", 0, bb_node_id(pBB)) + "_done")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/chars Ljava/lang/String;")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/sigma Ljava/lang/String;")
             + s_1asm("iload_1")
             + s_2asm("invokevirtual", "java/lang/String/charAt(I)C")
             + s_2asm("invokevirtual", "java/lang/String/indexOf(I)I")
             + s_1asm("iflt " + emit_fmt("span_%d_%d", 0, bb_node_id(pBB)) + "_done")
             + s_1asm("aload_0") + s_1asm("dup")
             + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_1asm("iconst_1") + s_1asm("iadd")
             + s_2asm("putfield", "bb/bb_span/matched_len I")
             + s_1asm("goto " + emit_fmt("span_%d_%d", 0, bb_node_id(pBB)) + "_loop")
             + s_L1asm(emit_fmt("span_%d_%d", 0, bb_node_id(pBB)) + "_done:", "aload_0")
             + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_1asm("ifle " + emit_fmt("span_%d_%d", 0, bb_node_id(pBB)) + "_ω")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I") + s_1asm("istore_2")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup") + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_1asm("iadd") + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup") + s_1asm("iload_2")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
             + s_L1asm(emit_fmt("span_%d_%d", 0, bb_node_id(pBB)) + "_ω:", "aconst_null") + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method public β()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 4") + s_directive(".limit locals 1")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup") + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_span/matched_len I")
             + s_1asm("isub") + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aconst_null") + s_1asm("areturn")
             + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        return emit_fmt("function make_pat_%d_%d(ms) { const chars = ", pBB->ival, bb_node_id(pBB))
             + js_escape_string_str(pBB->sval)
             + "; let delta = 0; let self = { succ: null, fail: null,\n"
               "α() { delta = 0; while (ms.delta + delta < ms.omega && chars.indexOf(ms.sigma[ms.delta + delta]) >= 0) delta++; if (delta <= 0) { self.fail.α(); return"
                   "; } const r = ms.sigma.slice(ms.delta, ms.delta + delta); ms.delta += delta; self.succ.α(); return r; },\n"
               "β() { ms.delta -= delta; self.fail.α(); }\n"
               "}; return self; }\n";
    }
    if (PLATFORM_NET) {
        return net_class_hdr_str(0, bb_node_id(pBB))
             + s_directive(".field private string _chars")
             + s_directive(".field private int32  _count")
             + s_directive(".method public specialname rtspecialname instance void .ctor(string chars) cil managed")
             + s_1asm("{") + s_2asm(".maxstack", "3")
             + s_1asm("ldarg.0") + s_2asm("call", "instance void [mscorlib]System.Object::.ctor()")
             + s_1asm("ldarg.0") + s_1asm("ldarg.1") + s_1asm("dup")
             + s_1asm(emit_fmt("brtrue     SP_%d_%d_NN", 0, bb_node_id(pBB)))
             + s_1asm("pop") + s_2asm("ldstr", "\"\"")
             + s_directive(emit_fmt("  SP_%d_%d_NN:", 0, bb_node_id(pBB)))
             + s_1asm(emit_fmt("stfld      string pat_%d_%d::_chars", 0, bb_node_id(pBB)))
             + s_1asm("ret") + s_1asm("}")
             + net_α_hdr_str() + s_2asm(".maxstack", "3")
             + s_2asm(".locals", "init (valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_r)")
             + s_1asm("ldarg.0") + s_1asm("ldc.i4.0")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_directive(emit_fmt("  SP_%d_%d_LOOP:", 0, bb_node_id(pBB)))
             + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("add")
             + s_1asm(emit_fmt("ldfld      string pat_%d_%d::_chars", 0, bb_node_id(pBB)))
             + s_2asm("callvirt", "instance bool [boxes]Snobol4.Runtime.Boxes.MatchState::CharInSet(int32, string)")
             + s_1asm(emit_fmt("brfalse    SP_%d_%d_DONE", 0, bb_node_id(pBB)))
             + s_1asm("ldarg.0") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("ldc.i4.1") + s_1asm("add")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm(emit_fmt("br         SP_%d_%d_LOOP", 0, bb_node_id(pBB)))
             + s_directive(emit_fmt("  SP_%d_%d_DONE:", 0, bb_node_id(pBB)))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("ldc.i4.0")
             + s_1asm(emit_fmt("ble        SP_%d_%d_FAIL", 0, bb_node_id(pBB)))
             + net_cursor_load_str() + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + net_spec_of_str() + s_1asm("stloc.0")
             + s_1asm("ldarg.1") + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("add")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm("ldloc.0") + s_1asm("ret")
             + s_directive(emit_fmt("  SP_%d_%d_FAIL:", 0, bb_node_id(pBB)))
             + net_fail_ret_str() + s_1asm("}")
             + net_β_hdr_str() + s_2asm(".maxstack", "3")
             + s_1asm("ldarg.1") + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, bb_node_id(pBB)))
             + s_1asm("sub")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + net_fail_ret_str() + s_1asm("}") + s_1asm("}")
             + net_escape_ldstr_str(pBB->sval ? pBB->sval : "")
             + s_1asm(emit_fmt("newobj     instance void pat_%d_%d::.ctor(string)", 0, bb_node_id(pBB)));
    }
    if (PLATFORM_WASM) { return "          (call $bb_span_new)\n"; }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_span(IR_t * pBB) {
    if (MEDIUM_TEXT)   g_emit.bb_cs_id = g_flat_node_id++;
    if (MEDIUM_BINARY) g_emit.bb_cs_zeta = rt_cs_new(pBB->sval ? pBB->sval : "");
    bb_bin_t bin;
    std::string out = bb_pat_span_str(pBB, bin);
    if (!out.empty()) bb_emit_asm_result(out, bin);
}
