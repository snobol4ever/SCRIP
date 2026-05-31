/* bb_capture.cpp — BB template for capture (pat . var).
   One file per Byrd Box per RULES.md (BB_templates folder rule). */
#include <string>
#include <deque>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_box.h"
#include "emit_bb.h"
#include "emit.h"
#include <gc.h>
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* M3-NATIVE-4 / SBL-CAP-2: BINARY arm runtime helper.
   Called from the BINARY blob with: rdi=varname, esi=saved_delta, edx=cur_delta, ecx=is_imm.
   Extracts the matched substring Σ[saved_delta..cur_delta) and assigns it to the variable. */
extern "C" void rt_cap_assign_cursor(const char *varname, int saved_delta, int cur_delta, int is_imm);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Per-template-call saved_Δ scratch slot.  Lives in a process-lifetime std::deque<int> so the
   pointer baked as imm64 into the brokered blob bytes remains valid forever — the GC never sees
   it (the bb_pool is mmap'd, not GC-scanned), but the deque on the C++ heap keeps the int alive
   without GC intervention.  std::deque guarantees pointers/refs to existing elements stay valid
   across push_back (only iterators may invalidate), matching the bomb_intern pattern. */
static int * cap_alloc_saved_delta_slot(void) {
    static std::deque<int> pool;
    pool.emplace_back(0);
    return &pool.back();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_capture_str(IR_t * pBB, int imm, bb_bin_t & bin) {
    bin = {};
    int nid = bb_node_id(pBB); int sid = 0;
    if (PLATFORM_X86) {
        const char *child_lbl = g_emit.bb_child_lbl;
        void       *child_fn  = g_emit.bb_child_fn;
        const char *varname   = g_emit.op_name1;
        if (MEDIUM_BINARY) {
            if (!child_fn || !varname || !varname[0]) {
                return std::string(); /* no child_fn or varname — emit nothing, honest skip */
            }
            /* SBL-CAP-2 MEDIUM_BINARY arm (M3-NATIVE-4).
               ABI on entry: r10 = &Δ (the global cursor int).  child_fn is a brokered bb_box_fn;
               XA_FLAT_PROLOGUE inside child re-establishes r10 = &Δ, so r10 survives the call
               value-wise — but per SysV ABI r10 is caller-saved, so push/pop r10 around each call
               to match the bb_pat_any pattern and stay defensive.
               child_fn signature: DESCR_t (*)(void *zeta, int entry).  zeta unused by brokered
               blobs (they use absolute addresses); entry=0 for α, entry=1 for β.
               saved_Δ slot: process-lifetime int via cap_alloc_saved_delta_slot().
               Layout (128 bytes total):
                [0]   41 8b 02              mov eax, [r10]            ; eax = current Δ
                [3]   48 b9 <8>             movabs rcx, &saved_Δ
                [13]  89 01                 mov [rcx], eax            ; *saved_Δ = Δ
                [15]  31 ff                 xor edi, edi              ; zeta=0
                [17]  31 f6                 xor esi, esi              ; entry=0
                [19]  48 b8 <8>             movabs rax, child_fn
                [29]  41 52                 push r10                  ; preserve r10
                [31]  ff d0                 call rax
                [33]  41 5a                 pop r10                   ; restore r10
                [35]  83 f8 63              cmp eax, 99               ; eax = DESCR_t.v
                [38]  0f 84 [r32]           je → ω                    ; site[0] rel32 @40
                [44]  e9 [r32]              jmp assign (internal)     ; pre-patched, rel32 @45
                lbl_β at [49]:
                [49]  31 ff                 xor edi, edi              ; zeta=0
                [51]  be 01 00 00 00        mov esi, 1                ; entry=1 (retry)
                [56]  48 b8 <8>             movabs rax, child_fn
                [66]  41 52                 push r10
                [68]  ff d0                 call rax
                [70]  41 5a                 pop r10
                [72]  83 f8 63              cmp eax, 99
                [75]  0f 84 [r32]           je → ω                    ; site[1] rel32 @77
                assign at [81]:
                [81]  48 bf <8>             movabs rdi, varname
                [91]  48 b9 <8>             movabs rcx, &saved_Δ
                [101] 8b 31                 mov esi, [rcx]            ; esi = saved_Δ
                [103] 41 8b 12              mov edx, [r10]            ; edx = current Δ
                [106] b9 <imm32>            mov ecx, imm
                [111] 48 b8 <8>             movabs rax, &rt_cap_assign_cursor
                [121] 41 52                 push r10                  ; preserve &Δ (caller-saved)
                [123] 41 52                 push r10                  ; keep rsp 16-aligned for the call
                [125] ff d0                 call rax                  ; NV_SET on OUTPUT etc. clobbers r10
                [127] 41 5a                 pop r10
                [129] 41 5a                 pop r10                   ; restore &Δ
                [131] e9 [r32]              jmp → γ                   ; site[3] rel32 @132
                end at [136].
               Sites (rel32 patches + label-defines):
                 site[0] = 40   ω   rel32
                 site[1] = 49   β   label-define (define lbl_β here)
                 site[2] = 77   ω   rel32
                 site[3] = 132  γ   rel32
               Internal jmp→assign: rel32 at @45, after-addr=49, target=81 → rel32 = 81-49 = 32.
               SBL-CAP-OUTPUT-R10 (2026-05-29): the rt_cap_assign_cursor call is the ONE call in this
               blob whose callee can run arbitrary side-effecting code — NV_SET_fn on the OUTPUT (or
               any associated) variable enters the print path (printf/fwrite), which clobbers the
               caller-saved r10 (= &Δ). The post-assign consumer of [r10] (broker final-cursor read /
               a following CAT element) then SIGSEGVs. The two pushes preserve r10 AND keep rsp
               16-aligned at the call so the print path's aligned SSE does not fault. */
            int *saved_delta = cap_alloc_saved_delta_slot();
            uint64_t sa  = (uint64_t)(uintptr_t)saved_delta;
            uint64_t cfa = (uint64_t)(uintptr_t)child_fn;
            uint64_t vna = (uint64_t)(uintptr_t)varname;
            uint64_t rca = (uint64_t)(uintptr_t)rt_cap_assign_cursor;
            std::string blob;
            blob += std::string("\x41\x8b\x02", 3);            /* [0]   mov eax, [r10] */
            blob += std::string("\x48\xb9", 2); blob += u64le(sa);   /* [3]   movabs rcx, &saved_Δ */
            blob += std::string("\x89\x01", 2);                /* [13]  mov [rcx], eax */
            blob += std::string("\x31\xff", 2);                /* [15]  xor edi, edi */
            blob += std::string("\x31\xf6", 2);                /* [17]  xor esi, esi */
            blob += std::string("\x48\xb8", 2); blob += u64le(cfa);  /* [19]  movabs rax, child_fn */
            blob += std::string("\x41\x52", 2);                /* [29]  push r10 */
            blob += std::string("\xff\xd0", 2);                /* [31]  call rax */
            blob += std::string("\x41\x5a", 2);                /* [33]  pop r10 */
            blob += std::string("\x83\xf8\x63", 3);            /* [35]  cmp eax, 99 */
            blob += std::string("\x0f\x84", 2); blob += u32le(0);    /* [38]  je rel32 ω    site rel32 @40 */
            blob += std::string("\xe9", 1);     blob += u32le(0);    /* [44]  jmp assign     rel32 @45 (pre-patched 32) */
            /* lbl_β at [49] */
            blob += std::string("\x31\xff", 2);                /* [49]  xor edi, edi */
            blob += std::string("\xbe\x01\x00\x00\x00", 5);    /* [51]  mov esi, 1 */
            blob += std::string("\x48\xb8", 2); blob += u64le(cfa);  /* [56]  movabs rax, child_fn */
            blob += std::string("\x41\x52", 2);                /* [66]  push r10 */
            blob += std::string("\xff\xd0", 2);                /* [68]  call rax */
            blob += std::string("\x41\x5a", 2);                /* [70]  pop r10 */
            blob += std::string("\x83\xf8\x63", 3);            /* [72]  cmp eax, 99 */
            blob += std::string("\x0f\x84", 2); blob += u32le(0);    /* [75]  je rel32 ω    site rel32 @77 */
            /* assign at [81] */
            blob += std::string("\x48\xbf", 2); blob += u64le(vna);  /* [81]  movabs rdi, varname */
            blob += std::string("\x48\xb9", 2); blob += u64le(sa);   /* [91]  movabs rcx, &saved_Δ */
            blob += std::string("\x8b\x31", 2);                /* [101] mov esi, [rcx] */
            blob += std::string("\x41\x8b\x12", 3);            /* [103] mov edx, [r10] */
            blob += std::string("\xb9", 1);     blob += u32le((uint32_t)imm); /* [106] mov ecx, imm */
            blob += std::string("\x48\xb8", 2); blob += u64le(rca);  /* [111] movabs rax, &rt_cap_assign_cursor */
            blob += std::string("\x41\x52", 2);                /* [121] push r10      preserve &Δ (caller-saved) */
            blob += std::string("\x41\x52", 2);                /* [123] push r10      keep rsp 16-aligned for the call */
            blob += std::string("\xff\xd0", 2);                /* [125] call rax */
            blob += std::string("\x41\x5a", 2);                /* [127] pop r10 */
            blob += std::string("\x41\x5a", 2);                /* [129] pop r10       restore &Δ (OUTPUT print clobbers it) */
            blob += std::string("\xe9", 1);     blob += u32le(0);    /* [131] jmp γ          rel32 @132 */
            /* Pre-patch internal jmp→assign at @45: rel32 = target(81) - after(49) = 32. */
            { int32_t r = 32; memcpy(const_cast<char*>(blob.data()) + 45, &r, 4); }
            bin = { {40, 49, 77, 132},
                    {_.lbl_ω_p, _.lbl_β_p, _.lbl_ω_p, _.lbl_γ_p},
                    {false, true, false, false} };
            (void)nid; (void)sid;
            return blob;
        }
        const char *vn_label = emit_intern_str(varname ? varname : "");
        if (!child_lbl || !child_lbl[0] || !varname || !varname[0]) {
            return std::string();
        }
        std::string z = emit_fmt(".Lcap%d_start", nid);
        std::string assign_lbl   = emit_fmt(".Lcap%d_assign", nid);
        bin = { {}, {}, {} };
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — CAPTURE"))
             + IF(MEDIUM_TEXT,
                   s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX %s(%s)", imm ? "CAP_IMM" : "CAP_COND", varname))
                 + s_directive(".section .data")
                 + s_directive(z + ": .long 0")
                 + s_directive(".section .text")
                 + s_directive(".intel_syntax noprefix")
                 + s_2asm("mov", "eax, [r10]")
                 + s_2asm("mov", "dword ptr [rip + " + z + "], eax")
                 + s_2asm("xor", "esi, esi")
                 + s_2asm("push", "r10")
                 + s_2asm("call", child_lbl)
                 + s_2asm("pop", "r10")
                 + s_2asm("cmp", "eax, 99")
                 + s_2asm("je", _.lbl_ω)
                 + s_2asm("jmp", assign_lbl.c_str())
                 + s_1asm(emit_fmt("%s:", _.lbl_β))
                 + s_2asm("mov", "esi, 1")
                 + s_2asm("push", "r10")
                 + s_2asm("call", child_lbl)
                 + s_2asm("pop", "r10")
                 + s_2asm("cmp", "eax, 99")
                 + s_2asm("je", _.lbl_ω)
                 + s_1asm(emit_fmt("%s:", assign_lbl.c_str()))
                 + s_2asm("lea", "rax, [rip + \xCE\xA3]")
                 + s_2asm("mov", "rax, [rax]")
                 + s_2asm("movsxd", "rcx, dword ptr [rip + " + z + "]")
                 + s_2asm("lea", "rsi, [rax+rcx]")
                 + s_2asm("mov", "edx, [r10]")
                 + s_2asm("sub", "edx, dword ptr [rip + " + z + "]")
                 + s_2asm("lea", emit_fmt("rdi, [rip + %s]", vn_label ? vn_label : "??"))
                 + s_2asm("push", "r10")
                 + s_2asm("call", "rt_cap_assign@PLT")
                 + s_2asm("pop", "r10")
                 + s_2asm("jmp", _.lbl_γ));
    }
    if (PLATFORM_JVM) {
        (void)imm; (void)sid; (void)nid;
        return jvm_class_hdr_str("capture")
             + s_directive(".inner interface public static abstract var_setter inner bb/bb_capture$VarSetter outer bb/bb_capture")
             + s_directive(".field private final child Lbb/bb_box;")
             + s_directive(".field private final varname Ljava/lang/String;")
             + s_directive(".field private final immediate Z")
             + s_directive(".field private final setter Lbb/bb_capture$VarSetter;")
             + s_directive(".field private pending_start I")
             + s_directive(".field private pending_len I")
             + s_directive(".field private has_pending Z")
             + s_directive(".method public <init>(Lbb/bb_box$MatchState;Lbb/bb_box;Ljava/lang/String;ZLbb/bb_capture$VarSetter;)V")
             + s_directive(".limit stack 3")
             + s_directive(".limit locals 6")
             + s_1asm("aload_0") + s_1asm("aload_1")
             + s_2asm("invokespecial", "bb/bb_box/<init>(Lbb/bb_box$MatchState;)V")
             + s_1asm("aload_0") + s_1asm("aload_2")
             + s_2asm("putfield", "bb/bb_capture/child Lbb/bb_box;")
             + s_1asm("aload_0") + s_1asm("aload_3")
             + s_2asm("putfield", "bb/bb_capture/varname Ljava/lang/String;")
             + s_1asm("aload_0") + s_2asm("iload", "4")
             + s_2asm("putfield", "bb/bb_capture/immediate Z")
             + s_1asm("aload_0") + s_2asm("aload", "5")
             + s_2asm("putfield", "bb/bb_capture/setter Lbb/bb_capture$VarSetter;")
             + s_1asm("return") + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        return emit_fmt("function make_pat_%d_%d(ms) { const varname = ", pBB->ival, nid)
             + js_escape_string_str(pBB->sval)
             + "; let self = { succ: null, fail: null,\n"
             + emit_fmt("α() { const cr = self.child.α(); if (cr === null) { self.fail.α(); return; } ms._do_capture(cr, varname, %d); self.succ.α(); return cr; },\n", imm)
             + emit_fmt("β() { const cr = self.child.β(); if (cr === null) { self.fail.α(); return; } ms._do_capture(cr, varname, %d); return cr; }\n", imm)
             + "}; return self; }\n";
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_capture(IR_t * pBB, int imm) { bb_bin_t bin; bb_emit_asm_result(bb_capture_str(pBB, imm, bin), bin); }
