/* bb_arbno.cpp — BB template for ARBNO(pat) zero-or-more match. Converted to return-string (ER wave). */
#include <string>
#include <deque>
#include <array>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_box.h"
#include "emit_bb.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_arbno_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    int nid = bb_node_id(pBB); int sid = 0;
    if (PLATFORM_X86) {
        const char *child_lbl = g_emit.bb_child_lbl;
        int have_child = MEDIUM_BINARY ? (g_emit.bb_child_fn != NULL) : (child_lbl && child_lbl[0]);
        if (!have_child) {
            if (MEDIUM_BINARY) {
                bin = { {1, 6}, {_.lbl_γ_p, _.lbl_ω_p}, {false, false} };
                return bytes(1,"\xE9")+u32le(0)+bytes(1,"\xE9")+u32le(0);
            }
            return std::string();
        }
        std::string saved = emit_fmt(".Larbno%d_saved", nid);
        std::string depth = emit_fmt(".Larbno%d_depth", nid);
        std::string stack = emit_fmt(".Larbno%d_stack", nid);
        std::string lp    = emit_fmt(".Larbno%d_loop", nid);
        std::string done  = emit_fmt(".Larbno%d_done", nid);
        std::string omega = emit_fmt(".Larbno%d_omega", nid);
        const int MAX_DEPTH = 256;
        bin = { {}, {}, {} };
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — ARBNO"))
             + IF(MEDIUM_BINARY, [&]() -> std::string {
                   /* ARBNO BINARY arm (SBL-ARBNO-3). Scratch: depth_slot,saved_slot (deque<int>),
                      stack_arr (deque<array<int,128>>). child_fn brokered ABI: fn(void*,int)→eax(99=fail).
                      MAX_DEPTH=128 (cmp edx,127+jg = 3B+6B). Internal rel32s pre-patched.
                      External sites: {182→γ, 186→β(def), 203→ω, 255→γ}. 259 bytes. */
                   void *cfn = g_emit.bb_child_fn;
                   if (!cfn) { bin = {{1,6},{_.lbl_γ_p,_.lbl_ω_p},{false,false}}; return bytes(1,"\xE9")+u32le(0)+bytes(1,"\xE9")+u32le(0); }
                   static std::deque<int> _di;
                   static std::deque<std::array<int,128>> _da;
                   _di.emplace_back(0); int *depth_slot = &_di.back();
                   _di.emplace_back(0); int *saved_slot = &_di.back();
                   _da.emplace_back(); int *stack_arr = _da.back().data();
                   uint64_t dp = (uint64_t)(uintptr_t)depth_slot;
                   uint64_t sp = (uint64_t)(uintptr_t)saved_slot;
                   uint64_t sa = (uint64_t)(uintptr_t)stack_arr;
                   uint64_t cf = (uint64_t)(uintptr_t)cfn;
                   std::string b;
                   b += bytes(2,"\x48\xB9")+u64le(sp);                    /* [0]   movabs rcx,saved_ptr */
                   b += bytes(3,"\x41\x8B\x02");                           /* [10]  mov eax,[r10] */
                   b += bytes(2,"\x89\x01");                                /* [13]  mov [rcx],eax */
                   b += bytes(2,"\x48\xB9")+u64le(dp);                    /* [15]  movabs rcx,depth_ptr */
                   b += bytes(6,"\xC7\x01\x00\x00\x00\x00");            /* [25]  mov dword[rcx],0 */
                   /* loop_top=[31] */
                   b += bytes(2,"\x31\xFF");                                /* [31]  xor edi,edi */
                   b += bytes(2,"\x31\xF6");                                /* [33]  xor esi,esi */
                   b += bytes(2,"\x48\xB8")+u64le(cf);                    /* [35]  movabs rax,child_fn */
                   b += bytes(2,"\x41\x52");                                /* [45]  push r10 */
                   b += bytes(2,"\xFF\xD0");                                /* [47]  call rax */
                   b += bytes(2,"\x41\x5A");                                /* [49]  pop r10 */
                   b += bytes(3,"\x83\xF8\x63");                           /* [51]  cmp eax,99 */
                   b += bytes(2,"\x0F\x84")+u32le(0x79u);                  /* [54]  je done(+121→181) */
                   b += bytes(2,"\x48\xB9")+u64le(dp);                    /* [60]  movabs rcx,depth_ptr */
                   b += bytes(2,"\x8B\x11");                                /* [70]  mov edx,[rcx] */
                   b += bytes(2,"\x85\xD2");                                /* [72]  test edx,edx */
                   b += bytes(2,"\x0F\x84")+u32le(0x16u);                  /* [74]  je chk_saved(+22→102) */
                   b += bytes(2,"\x48\xB9")+u64le(sa);                    /* [80]  movabs rcx,stack_ptr */
                   b += bytes(4,"\x48\x8D\x52\xFF");                      /* [90]  lea rdx,[rdx-1] */
                   b += bytes(3,"\x8B\x04\x91");                           /* [94]  mov eax,[rcx+rdx*4] */
                   b += bytes(1,"\xE9")+u32le(0x0Cu);                       /* [97]  jmp chk_val(+12→114) */
                   /* chk_saved=[102] */
                   b += bytes(2,"\x48\xB9")+u64le(sp);                    /* [102] movabs rcx,saved_ptr */
                   b += bytes(2,"\x8B\x01");                                /* [112] mov eax,[rcx] */
                   /* chk_val=[114] */
                   b += bytes(3,"\x41\x39\x02");                           /* [114] cmp [r10],eax */
                   b += bytes(2,"\x0F\x84")+u32le(0x3Au);                  /* [117] je done(+58→181) */
                   b += bytes(2,"\x48\xB9")+u64le(dp);                    /* [123] movabs rcx,depth_ptr */
                   b += bytes(2,"\x8B\x11");                                /* [133] mov edx,[rcx] */
                   b += bytes(3,"\x83\xFA\x7F");                           /* [135] cmp edx,127 */
                   b += bytes(2,"\x0F\x8F")+u32le(0x25u);                  /* [138] jg done(+37→181) */
                   b += bytes(2,"\x48\xB9")+u64le(sa);                    /* [144] movabs rcx,stack_ptr */
                   b += bytes(3,"\x48\x63\xD2");                           /* [154] movsxd rdx,edx */
                   b += bytes(3,"\x41\x8B\x02");                           /* [157] mov eax,[r10] */
                   b += bytes(3,"\x89\x04\x91");                           /* [160] mov [rcx+rdx*4],eax */
                   b += bytes(2,"\x48\xB9")+u64le(dp);                    /* [163] movabs rcx,depth_ptr */
                   b += bytes(3,"\x83\x01\x01");                           /* [173] add dword[rcx],1 */
                   b += bytes(1,"\xE9")+u32le((uint32_t)(int32_t)-150);     /* [176] jmp loop_top(-150→31) */
                   /* done=[181] */
                   b += bytes(1,"\xE9")+u32le(0);                           /* [181] jmp γ site[0]@182 */
                   /* lbl_β=[186] */
                   b += bytes(2,"\x48\xB9")+u64le(dp);                    /* [186] movabs rcx,depth_ptr */
                   b += bytes(2,"\x8B\x01");                                /* [196] mov eax,[rcx] */
                   b += bytes(3,"\x83\xE8\x01");                           /* [198] sub eax,1 */
                   b += bytes(2,"\x0F\x88")+u32le(0);                      /* [201] js ω site[2]@203 */
                   b += bytes(2,"\x89\x01");                                /* [207] mov [rcx],eax */
                   b += bytes(2,"\x85\xC0");                                /* [209] test eax,eax */
                   b += bytes(2,"\x0F\x85")+u32le(0x11u);                  /* [211] jnz pop_stack(+17→234) */
                   b += bytes(2,"\x48\xB9")+u64le(sp);                    /* [217] movabs rcx,saved_ptr */
                   b += bytes(2,"\x8B\x01");                                /* [227] mov eax,[rcx] */
                   b += bytes(1,"\xE9")+u32le(0x11u);                       /* [229] jmp restore(+17→251) */
                   /* pop_stack=[234] */
                   b += bytes(2,"\x48\xB9")+u64le(sa);                    /* [234] movabs rcx,stack_ptr */
                   b += bytes(4,"\x48\x8D\x50\xFF");                      /* [244] lea rdx,[rax-1] */
                   b += bytes(3,"\x8B\x04\x91");                           /* [248] mov eax,[rcx+rdx*4] */
                   /* restore=[251] */
                   b += bytes(3,"\x41\x89\x02");                           /* [251] mov [r10],eax */
                   b += bytes(1,"\xE9")+u32le(0);                           /* [254] jmp γ site[3]@255 */
                   /* end=[259] */
                   bin = { {182, 186, 203, 255},
                           {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p, _.lbl_γ_p},
                           {false, true, false, false} };
                   return b;
               }())
             + IF(MEDIUM_TEXT,
                   s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment("# BOX ARBNO()")
                 + s_directive(".section .data")
                 + s_directive(saved + ": .long 0")
                 + s_directive(depth + ": .long 0")
                 + s_directive(stack + ": .skip " + emit_fmt("%d", MAX_DEPTH * 4))
                 + s_directive(".section .text")
                 + s_directive(".intel_syntax noprefix")
                 + s_2asm("mov", "eax, [r10]")
                 + s_2asm("mov", "dword ptr [rip + " + saved + "], eax")
                 + s_2asm("mov", "dword ptr [rip + " + depth + "], 0")
                 + s_1asm(emit_fmt("%s:", lp.c_str()))
                 + s_2asm("mov", "ecx, dword ptr [rip + " + depth + "]")
                 + s_2asm("cmp", emit_fmt("ecx, %d", MAX_DEPTH))
                 + s_2asm("jge", done.c_str())
                 + s_2asm("mov", "eax, [r10]")
                 + s_2asm("push", "rax")
                 + s_2asm("xor", "esi, esi")
                 + s_2asm("push", "r10")
                 + s_2asm("call", child_lbl)
                 + s_2asm("pop", "r10")
                 + s_2asm("pop", "rcx")
                 + s_2asm("cmp", "eax, 99")
                 + s_2asm("je", done.c_str())
                 + s_2asm("mov", "eax, [r10]")
                 + s_2asm("cmp", "eax, ecx")
                 + s_2asm("je", done.c_str())
                 + s_2asm("mov", "ecx, dword ptr [rip + " + depth + "]")
                 + s_2asm("lea", "rdx, [rip + " + stack + "]")
                 + s_2asm("mov", "[rdx + rcx*4], eax")
                 + s_2asm("add", "ecx, 1")
                 + s_2asm("mov", "dword ptr [rip + " + depth + "], ecx")
                 + s_2asm("jmp", lp.c_str())
                 + s_1asm(emit_fmt("%s:", done.c_str()))
                 + s_2asm("jmp", _.lbl_γ)
                 + s_1asm(emit_fmt("%s:", _.lbl_β))
                 + s_2asm("mov", "ecx, dword ptr [rip + " + depth + "]")
                 + s_2asm("sub", "ecx, 1")
                 + s_2asm("jl", omega.c_str())
                 + s_2asm("mov", "dword ptr [rip + " + depth + "], ecx")
                 + s_2asm("cmp", "ecx, 0")
                 + s_2asm("jg", emit_fmt(".Larbno%d_pop", nid).c_str())
                 + s_2asm("mov", "eax, dword ptr [rip + " + saved + "]")
                 + s_2asm("jmp", emit_fmt(".Larbno%d_restore", nid).c_str())
                 + s_1asm(emit_fmt(".Larbno%d_pop:", nid))
                 + s_2asm("lea", "rdx, [rip + " + stack + "]")
                 + s_2asm("sub", "ecx, 1")
                 + s_2asm("mov", "eax, [rdx + rcx*4]")
                 + s_1asm(emit_fmt(".Larbno%d_restore:", nid))
                 + s_2asm("mov", "[r10], eax")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_1asm(emit_fmt("%s:", omega.c_str()))
                 + s_2asm("jmp", _.lbl_ω));
    }
    if (PLATFORM_JVM) {
        std::string tag_s = emit_fmt("arbno_%d_%d", sid, nid);
        return jvm_class_hdr_str("arbno")
             + s_directive(".field private static final MAX_DEPTH I = 64")
             + s_directive(".field private final body Lbb/bb_box;")
             + s_directive(".field private final frame_start [I")
             + s_directive(".field private final frame_match_st [I")
             + s_directive(".field private final frame_match_ln [I")
             + s_directive(".field private depth I")
             + s_directive(".method public <init>(Lbb/bb_box$MatchState;Lbb/bb_box;)V")
             + s_directive(".limit stack 4") + s_directive(".limit locals 3")
             + s_1asm("aload_0") + s_1asm("aload_1")
             + s_2asm("invokespecial", "bb/bb_box/<init>(Lbb/bb_box$MatchState;)V")
             + s_1asm("aload_0") + s_1asm("aload_2")
             + s_2asm("putfield", "bb/bb_arbno/body Lbb/bb_box;")
             + s_1asm("aload_0") + s_2asm("bipush", "64") + s_2asm("newarray", "int")
             + s_2asm("putfield", "bb/bb_arbno/frame_start [I")
             + s_1asm("aload_0") + s_2asm("bipush", "64") + s_2asm("newarray", "int")
             + s_2asm("putfield", "bb/bb_arbno/frame_match_st [I")
             + s_1asm("aload_0") + s_2asm("bipush", "64") + s_2asm("newarray", "int")
             + s_2asm("putfield", "bb/bb_arbno/frame_match_ln [I")
             + s_1asm("return") + s_directive(".end method")
             + s_directive(".method public α()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 5") + s_directive(".limit locals 1")
             + s_1asm("aload_0") + s_1asm("iconst_0") + s_2asm("putfield", "bb/bb_arbno/depth I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/frame_match_st [I")
             + s_1asm("iconst_0") + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_arbno/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I") + s_1asm("iastore")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/frame_match_ln [I")
             + s_1asm("iconst_0") + s_1asm("iconst_0") + s_1asm("iastore")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/frame_start [I")
             + s_1asm("iconst_0") + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_arbno/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I") + s_1asm("iastore")
             + s_1asm("aload_0")
             + s_2asm("invokevirtual", "bb/bb_arbno/tryBody()Lbb/bb_box$Spec;")
             + s_1asm("areturn") + s_directive(".end method")
             + s_directive(".method public β()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 5") + s_directive(".limit locals 1")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/depth I")
             + s_1asm("ifle " + tag_s + "_β_ω")
             + s_1asm("aload_0") + s_1asm("dup")
             + s_2asm("getfield", "bb/bb_arbno/depth I") + s_1asm("iconst_1") + s_1asm("isub")
             + s_2asm("putfield", "bb/bb_arbno/depth I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/ms Lbb/bb_box$MatchState;")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/frame_start [I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/depth I") + s_1asm("iaload")
             + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/frame_match_st [I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/depth I") + s_1asm("iaload")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/frame_match_ln [I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/depth I") + s_1asm("iaload")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
             + s_L1asm(tag_s + "_β_ω:", "aconst_null") + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method private tryBody()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 6") + s_directive(".limit locals 4")
             + s_L1asm(tag_s + "_tryBody_loop:", "aload_0")
             + s_2asm("getfield", "bb/bb_arbno/body Lbb/bb_box;")
             + s_2asm("invokevirtual", "bb/bb_box/α()Lbb/bb_box$Spec;") + s_1asm("astore_1")
             + s_1asm("aload_1") + s_1asm("ifnull " + tag_s + "_tryBody_ω")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/frame_start [I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/depth I") + s_1asm("iaload")
             + s_1asm("if_icmpne " + tag_s + "_tryBody_advance")
             + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/frame_match_st [I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/depth I") + s_1asm("iaload")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/frame_match_ln [I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/depth I") + s_1asm("iaload")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
             + s_L1asm(tag_s + "_tryBody_advance:", "aload_0")
             + s_2asm("getfield", "bb/bb_arbno/frame_match_st [I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/depth I") + s_1asm("iaload")
             + s_1asm("istore_2")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/frame_match_ln [I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/depth I") + s_1asm("iaload")
             + s_1asm("aload_1") + s_2asm("getfield", "bb/bb_box$Spec/len I") + s_1asm("iadd")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/depth I")
             + s_1asm("iconst_1") + s_1asm("iadd") + s_2asm("bipush", "64")
             + s_1asm("if_icmpge " + tag_s + "_tryBody_full")
             + s_1asm("aload_0") + s_1asm("dup")
             + s_2asm("getfield", "bb/bb_arbno/depth I") + s_1asm("iconst_1") + s_1asm("iadd")
             + s_2asm("putfield", "bb/bb_arbno/depth I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/frame_match_st [I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/depth I")
             + s_1asm("iload_2") + s_1asm("iastore") + s_2asm("istore", "3")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/frame_match_ln [I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/depth I")
             + s_2asm("iload", "3") + s_1asm("iastore")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/frame_start [I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/depth I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I") + s_1asm("iastore")
             + s_1asm("goto " + tag_s + "_tryBody_loop")
             + s_L1asm(tag_s + "_tryBody_full:", "new bb/bb_box$Spec")
             + s_1asm("dup_x1") + s_1asm("swap") + s_1asm("iload_2") + s_1asm("swap")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
             + s_L1asm(tag_s + "_tryBody_ω:", "new bb/bb_box$Spec") + s_1asm("dup")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/frame_match_st [I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/depth I") + s_1asm("iaload")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/frame_match_ln [I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arbno/depth I") + s_1asm("iaload")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
             + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        return emit_fmt("function make_pat_%d_%d(ms) { const stack = []; let self = { succ: null, fail: null,\n", pBB->ival, nid)
             + "α() { stack.length = 0; stack.push({ start: ms.delta }); while (true) { const frame = stack[stack.length - 1]; const br = self.body.α();\n"
               "if (br === null) { return ms.sigma.slice(stack[0].start, ms.delta - stack[0].start); }\n"
               "if (ms.delta === frame.start) { return ms.sigma.slice(stack[0].start, ms.delta - stack[0].start); }\n"
               "stack.push({ start: ms.delta }); } },\n"
               "β() { if (stack.length <= 1) { self.fail.α(); return"
                   "; } stack.pop(); const frame = stack[stack.length - 1]; ms.delta = frame.start; return ms.sigma.slice(stack[0].start, ms.delta - stack[0].start); }\n"
               "}; return self; }\n";
    }
    if (PLATFORM_NET) {
        return net_class_hdr_str(sid, nid)
             + s_directive(".field private class [boxes]Snobol4.Runtime.Boxes.IByrdBox _body")
             + s_directive(".field private int32[] _matchStart")
             + s_directive(".field private int32[] _matchLen")
             + s_directive(".field private int32[] _startStack")
             + s_directive(".field private int32   _depth")
             + s_directive(".method public specialname rtspecialname instance void .ctor(class [boxes]Snobol4.Runtime.Boxes.IByrdBox body) cil managed")
             + s_1asm("{") + s_2asm(".maxstack", "2")
             + s_1asm("ldarg.0") + s_2asm("call", "instance void [mscorlib]System.Object::.ctor()")
             + s_1asm("ldarg.0") + s_1asm("ldarg.1")
             + s_1asm(emit_fmt("stfld      class [boxes]Snobol4.Runtime.Boxes.IByrdBox pat_%d_%d::_body", sid, nid))
             + s_1asm("ldarg.0") + s_2asm("ldc.i4", "64") + s_2asm("newarr", "[mscorlib]System.Int32")
             + s_1asm(emit_fmt("stfld      int32[] pat_%d_%d::_matchStart", sid, nid))
             + s_1asm("ldarg.0") + s_2asm("ldc.i4", "64") + s_2asm("newarr", "[mscorlib]System.Int32")
             + s_1asm(emit_fmt("stfld      int32[] pat_%d_%d::_matchLen", sid, nid))
             + s_1asm("ldarg.0") + s_2asm("ldc.i4", "64") + s_2asm("newarr", "[mscorlib]System.Int32")
             + s_1asm(emit_fmt("stfld      int32[] pat_%d_%d::_startStack", sid, nid))
             + s_1asm("ret") + s_1asm("}")
             + net_α_hdr_str() + s_2asm(".maxstack", "4")
             + s_2asm(".locals", "init (int32 V_startHere, valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_br)")
             + s_1asm("ldarg.0") + s_1asm("ldc.i4.0")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_depth", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32[] pat_%d_%d::_matchStart", sid, nid))
             + s_1asm("ldc.i4.0") + net_cursor_load_str() + s_1asm("stelem.i4")
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32[] pat_%d_%d::_matchLen", sid, nid))
             + s_1asm("ldc.i4.0") + s_1asm("ldc.i4.0") + s_1asm("stelem.i4")
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32[] pat_%d_%d::_startStack", sid, nid))
             + s_1asm("ldc.i4.0") + net_cursor_load_str() + s_1asm("stelem.i4")
             + s_directive(emit_fmt("  ARBNO_%d_%d_LOOP:", sid, nid))
             + net_cursor_load_str() + s_1asm("stloc.0")
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      class [boxes]Snobol4.Runtime.Boxes.IByrdBox pat_%d_%d::_body", sid, nid))
             + s_1asm("ldarg.1")
             + s_2asm("callvirt", "instance valuetype [boxes]Snobol4.Runtime.Boxes.Spec [boxes]Snobol4.Runtime.Boxes.IByrdBox::Alpha(class [boxes]Snobol4.Runtime.Boxes.MatchState)")
             + s_1asm("stloc.1") + s_2asm("ldloca.s", "V_br")
             + s_2asm("call", "instance bool [boxes]Snobol4.Runtime.Boxes.Spec::get_IsFail()")
             + s_1asm(emit_fmt("brtrue     ARBNO_%d_%d_STOP", sid, nid))
             + net_cursor_load_str() + s_1asm("ldloc.0")
             + s_1asm(emit_fmt("beq        ARBNO_%d_%d_STOP", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_depth", sid, nid))
             + s_2asm("ldc.i4", "63")
             + s_1asm(emit_fmt("bge        ARBNO_%d_%d_STOP", sid, nid))
             + s_1asm("ldarg.0") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_depth", sid, nid))
             + s_1asm("ldc.i4.1") + s_1asm("add")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_depth", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32[] pat_%d_%d::_matchStart", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_depth", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32[] pat_%d_%d::_matchStart", sid, nid))
             + s_1asm("ldc.i4.0") + s_1asm("ldelem.i4") + s_1asm("stelem.i4")
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32[] pat_%d_%d::_matchLen", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_depth", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32[] pat_%d_%d::_matchLen", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_depth", sid, nid))
             + s_1asm("ldc.i4.1") + s_1asm("sub") + s_1asm("ldelem.i4")
             + s_2asm("ldloca.s", "V_br")
             + s_2asm("ldfld", "int32 [boxes]Snobol4.Runtime.Boxes.Spec::Length")
             + s_1asm("add") + s_1asm("stelem.i4")
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32[] pat_%d_%d::_startStack", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_depth", sid, nid))
             + net_cursor_load_str() + s_1asm("stelem.i4")
             + s_1asm(emit_fmt("br         ARBNO_%d_%d_LOOP", sid, nid))
             + s_directive(emit_fmt("  ARBNO_%d_%d_STOP:", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32[] pat_%d_%d::_matchStart", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_depth", sid, nid))
             + s_1asm("ldelem.i4")
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32[] pat_%d_%d::_matchLen", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_depth", sid, nid))
             + s_1asm("ldelem.i4")
             + net_spec_of_str() + s_1asm("ret") + s_1asm("}")
             + net_β_hdr_str() + s_2asm(".maxstack", "3")
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_depth", sid, nid))
             + s_1asm("ldc.i4.0")
             + s_1asm(emit_fmt("ble        ARBNO_%d_%d_BFAIL", sid, nid))
             + s_1asm("ldarg.0") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_depth", sid, nid))
             + s_1asm("ldc.i4.1") + s_1asm("sub")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_depth", sid, nid))
             + s_1asm("ldarg.1") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32[] pat_%d_%d::_startStack", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_depth", sid, nid))
             + s_1asm("ldelem.i4")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32[] pat_%d_%d::_matchStart", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_depth", sid, nid))
             + s_1asm("ldelem.i4")
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32[] pat_%d_%d::_matchLen", sid, nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_depth", sid, nid))
             + s_1asm("ldelem.i4")
             + net_spec_of_str() + s_1asm("ret")
             + s_directive(emit_fmt("  ARBNO_%d_%d_BFAIL:", sid, nid))
             + net_fail_ret_str() + s_1asm("}") + s_1asm("}")
             + s_1asm(emit_fmt("newobj     instance void pat_%d_%d::.ctor(class [boxes]Snobol4.Runtime.Boxes.IByrdBox)", sid, nid));
    }
    if (PLATFORM_WASM) { return "          (call $bb_arbno_new)\n"; }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_arbno(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_arbno_str(pBB, bin), bin);
}
