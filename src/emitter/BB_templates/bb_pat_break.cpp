/* bb_pat_break.cpp — BB template for BREAK and BREAKX. x86() self-encoding (template-revamp, 2026-06-02, Opus 4.8).
   BREAK(S) matches the run of subject chars from the cursor up to but NOT including the first char in set S
   (SPITBOL Manual ch.3: "BREAK(S) matches up to but not including any character in S"); it FAILS if no char in
   S is found before the subject end. BREAKX(S) is the extended form (ch.3 word4.spt): on backtrack it "looks
   past" the stop point — steps past the break char and rescans to the NEXT char in S — so a subsequent element
   can drive it further (BREAK never returns a string containing a char of S; BREAKX may, if a subsequent
   pattern requires it). Mode-2 oracle (bb_exec.c IR_PAT_BREAK): α scans i=0.. to first cset char, fails if
   δ+i>=Σlen, else counter=i, δ+=i, → γ. β plain BREAK: δ-=counter, fail. β BREAKX (ival==1): origin=δ-counter,
   i=counter+1, rescan from origin+i to next cset char, fail (δ=origin) if none or i<=counter, else counter=i,
   δ=origin+i, → γ.
   REG-2 registers: Σ=R13/δ=R14d/Δ=R15d (ratified, established by BB_MATCH α per REG-0). The legacy subject
   model (movabs &Σ, [r10], movabs &Σlen) is GONE; r11 scratch + push/pop r11 removed; r10 is SysV caller-saved
   so push/pop around strchr (r13/r14/r15 are callee-saved and survive). The match-state scalars are ζ-frame
   slots (PER-BOX LOCAL STORAGE / NO-VALUE-STACK FACT RULES): z = matched length @ [r12+off], z_orig = BREAKX
   rescan origin @ [r12+off+4] — claimed by bb_slot_claim(8), register-relative so BINARY==TEXT (the old
   process-global rt_cs_t z/z_orig + movabs &zeta is GONE). strchr(cs,ch)!=NULL ⇒ char in set. Internal labels:
   plain BREAK loop=L(0)/done=L(1); BREAKX adds rescan loop2=L(2)/done2=L(3). x86 arm: ONE return, pure x86()
   concat, NO bb_bin_t, medium invisible. */
#include <string>
#include <cstring>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
int bb_slot_claim(int bytes);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const char * cset_chars() { return _.op_sval ? _.op_sval : ""; }
static inline const char * cset_label() { return emit_intern_str(cset_chars()); }
static inline uint64_t     cset_addr()  { return (uint64_t)(uintptr_t)(const void *)cset_chars(); }
static inline uint64_t     strchr_ptr() { const char *(*fp)(const char *, int) = strchr; return (uint64_t)(uintptr_t)(void *)fp; }
static inline int          is_breakx()  { return _.op_ival == 1; }
static inline int          zoff()       { return _.x86_scratch_off; }       /* z  @ [r12+off]   */
static inline int          zooff()      { return _.x86_scratch_off + 4; }   /* z_orig @ [r12+off+4] (BREAKX) */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_break_str() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment(is_breakx() ? "# BOX BREAKX()  [REG-2 Σ=r13 δ=r14 Δ=r15, ζ-frame z/z_orig, x86() self-encoding]"
                                         : "# BOX BREAK()  [REG-2 Σ=r13 δ=r14 Δ=r15, ζ-frame z, x86() self-encoding]"))
             /* α: z = 0; scan forward to the first cset char */
             + x86("mov",    FR(zoff()), (long)0)                       /* z = 0                 */
             + x86("def",    L(0))                                      /* loop:                 */
             + x86("mov",    "eax", "r14d")                             /* eax = δ               */
             + x86("add",    "eax", FR(zoff()))                         /* eax = δ + z           */
             + x86("cmp",    "eax", "r15d")                             /* (δ+z) vs Δ            */
             + x86("jge",    PORT_OMEGA)                                /* >= Δ → not found → fail*/
             + x86("movsxd", "rcx", "eax")
             + x86("movzx",  "esi", "[r13+rcx]")                        /* Σ[δ+z]                */
             + x86("lea",    "rdi", "[rip + __]", cset_addr(), cset_label())
             + x86("push",   "r10")
             + x86("call",   "strchr", strchr_ptr())
             + x86("pop",    "r10")
             + x86("test",   "rax", "rax")
             + x86("jnz",    L(1))                                      /* found break char → done*/
             + x86("add",    FR(zoff()), (long)1)                       /* z += 1                */
             + x86("jmp",    L(0))                                      /* loop                  */
             + x86("def",    L(1))                                      /* done:                 */
             + x86("mov",    "eax", "r14d")                             /* eax = δ               */
             + x86("add",    "eax", FR(zoff()))                         /* eax = δ + z           */
             + x86("mov",    "r14d", "eax")                             /* δ += z (break char not consumed)*/
             + x86("jmp",    PORT_GAMMA)
             + x86("def",    PORT_BETA)                                 /* β:                    */
             + (is_breakx()
                  /* BREAKX β: origin = δ − z (saved); z++; rescan from origin+z to NEXT cset char */
                ? (  x86("mov",    "eax", "r14d")                       /* eax = δ               */
                   + x86("sub",    "eax", FR(zoff()))                   /* eax = δ − z = origin  */
                   + x86("mov",    FR(zooff()), "eax")                  /* z_orig = origin       */
                   + x86("add",    FR(zoff()), (long)1)                 /* z++                   */
                   + x86("def",    L(2))                                /* loop2:                */
                   + x86("mov",    "eax", FR(zooff()))                  /* eax = z_orig          */
                   + x86("add",    "eax", FR(zoff()))                   /* eax = z_orig + z      */
                   + x86("cmp",    "eax", "r15d")                       /* (z_orig+z) vs Δ       */
                   + x86("jge",    PORT_OMEGA)                          /* >= Δ → none → fail    */
                   + x86("movsxd", "rcx", "eax")
                   + x86("movzx",  "esi", "[r13+rcx]")                  /* Σ[z_orig+z]           */
                   + x86("lea",    "rdi", "[rip + __]", cset_addr(), cset_label())
                   + x86("push",   "r10")
                   + x86("call",   "strchr", strchr_ptr())
                   + x86("pop",    "r10")
                   + x86("test",   "rax", "rax")
                   + x86("jnz",    L(3))                                /* found → done2         */
                   + x86("add",    FR(zoff()), (long)1)                 /* z++                   */
                   + x86("jmp",    L(2))                                /* loop2                 */
                   + x86("def",    L(3))                                /* done2:                */
                   + x86("mov",    "eax", FR(zooff()))                  /* eax = z_orig          */
                   + x86("add",    "eax", FR(zoff()))                   /* eax = z_orig + z      */
                   + x86("mov",    "r14d", "eax")                       /* δ = z_orig + z        */
                   + x86("jmp",    PORT_GAMMA))
                  /* plain BREAK β: undo (δ −= z) and fail */
                : (  x86("mov",    "eax", "r14d")                       /* eax = δ               */
                   + x86("sub",    "eax", FR(zoff()))                   /* eax = δ − z           */
                   + x86("mov",    "r14d", "eax")                       /* δ −= z                */
                   + x86("jmp",    PORT_OMEGA)));
    }
    if (PLATFORM_JVM) {
        return jvm_class_hdr_str("brk")
             + s_directive(".field private final chars Ljava/lang/String;")
             + s_directive(".field private matched_len I")
             + jvm_init_ms_str_str("brk", "chars")
             + s_directive(".method public α()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 6") + s_directive(".limit locals 3")
             + s_1asm("aload_0") + s_1asm("iconst_0")
             + s_2asm("putfield", "bb/bb_brk/matched_len I")
             + s_L1asm(emit_fmt("brk_%d_%d", 0, _.nid) + "_loop:", "aload_0")
             + s_2asm("getfield", "bb/bb_brk/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/matched_len I")
             + s_1asm("iadd") + s_1asm("istore_1") + s_1asm("iload_1")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/ω I")
             + s_1asm("if_icmpge " + emit_fmt("brk_%d_%d", 0, _.nid) + "_ω")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/chars Ljava/lang/String;")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/sigma Ljava/lang/String;")
             + s_1asm("iload_1")
             + s_2asm("invokevirtual", "java/lang/String/charAt(I)C")
             + s_2asm("invokevirtual", "java/lang/String/indexOf(I)I")
             + s_1asm("ifge " + emit_fmt("brk_%d_%d", 0, _.nid) + "_found")
             + s_1asm("aload_0") + s_1asm("dup")
             + s_2asm("getfield", "bb/bb_brk/matched_len I")
             + s_1asm("iconst_1") + s_1asm("iadd")
             + s_2asm("putfield", "bb/bb_brk/matched_len I")
             + s_1asm("goto " + emit_fmt("brk_%d_%d", 0, _.nid) + "_loop")
             + s_L1asm(emit_fmt("brk_%d_%d", 0, _.nid) + "_found:", "aload_0")
             + s_2asm("getfield", "bb/bb_brk/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I") + s_1asm("istore_2")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup") + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/matched_len I")
             + s_1asm("iadd") + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup") + s_1asm("iload_2")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/matched_len I")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
             + s_L1asm(emit_fmt("brk_%d_%d", 0, _.nid) + "_ω:", "aconst_null") + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method public β()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 4") + s_directive(".limit locals 1")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup") + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_brk/matched_len I")
             + s_1asm("isub") + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aconst_null") + s_1asm("areturn")
             + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        return emit_fmt("function make_pat_%d_%d(ms) { const chars = ", (int)_.op_ival, _.nid)
             + js_escape_string_str(_.op_sval)
             + "; let delta = 0; let self = { succ: null, fail: null,\n"
               "α() { delta = 0; while (ms.delta + delta < ms.omega && chars.indexOf(ms.sigma[ms.delta + delta]) < 0) delta++; const r = ms.sigma.slice(ms.delta"
                   ", ms.delta + delta); ms.delta += delta; self.succ.α(); return r; },\n"
               "β() { ms.delta -= delta; self.fail.α(); }\n"
               "}; return self; }\n";
    }
    if (PLATFORM_NET) {
        return net_class_hdr_str(0, _.nid)
             + s_directive(".field private string _chars")
             + s_directive(".field private int32  _count")
             + s_directive(".method public specialname rtspecialname instance void .ctor(string chars) cil managed")
             + s_1asm("{") + s_2asm(".maxstack", "3")
             + s_1asm("ldarg.0") + s_2asm("call", "instance void [mscorlib]System.Object::.ctor()")
             + s_1asm("ldarg.0") + s_1asm("ldarg.1") + s_1asm("dup")
             + s_1asm(emit_fmt("brtrue     BRK_%d_%d_NN", 0, _.nid))
             + s_1asm("pop") + s_2asm("ldstr", "\"\"")
             + s_directive(emit_fmt("  BRK_%d_%d_NN:", 0, _.nid))
             + s_1asm(emit_fmt("stfld      string pat_%d_%d::_chars", 0, _.nid))
             + s_1asm("ret") + s_1asm("}")
             + net_α_hdr_str() + s_2asm(".maxstack", "4")
             + s_2asm(".locals", "init (valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_r)")
             + s_1asm("ldarg.0") + s_1asm("ldc.i4.0")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_count", 0, _.nid))
             + s_directive(emit_fmt("  BRK_%d_%d_LOOP:", 0, _.nid))
             + net_cursor_load_str() + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, _.nid))
             + s_1asm("add") + s_1asm("ldarg.1") + net_ms_length_str()
             + s_1asm(emit_fmt("bge        BRK_%d_%d_EOS", 0, _.nid))
             + s_1asm("ldarg.1") + net_cursor_load_str() + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, _.nid))
             + s_1asm("add") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      string pat_%d_%d::_chars", 0, _.nid))
             + s_2asm("callvirt", "instance bool [boxes]Snobol4.Runtime.Boxes.MatchState::CharInSet(int32, string)")
             + s_1asm(emit_fmt("brtrue     BRK_%d_%d_FOUND", 0, _.nid))
             + s_1asm("ldarg.0") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, _.nid))
             + s_1asm("ldc.i4.1") + s_1asm("add")
             + s_1asm(emit_fmt("stfld      int32 pat_%d_%d::_count", 0, _.nid))
             + s_1asm(emit_fmt("br         BRK_%d_%d_LOOP", 0, _.nid))
             + s_directive(emit_fmt("  BRK_%d_%d_EOS:", 0, _.nid)) + net_fail_ret_str()
             + s_directive(emit_fmt("  BRK_%d_%d_FOUND:", 0, _.nid))
             + net_cursor_load_str() + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, _.nid))
             + net_spec_of_str() + s_1asm("stloc.0")
             + s_1asm("ldarg.1") + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, _.nid))
             + s_1asm("add")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm("ldloc.0") + s_1asm("ret") + s_1asm("}")
             + net_β_hdr_str() + s_2asm(".maxstack", "3")
             + s_1asm("ldarg.1") + s_1asm("ldarg.1") + net_cursor_load_str()
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("ldfld      int32 pat_%d_%d::_count", 0, _.nid))
             + s_1asm("sub")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + net_fail_ret_str() + s_1asm("}") + s_1asm("}")
             + net_escape_ldstr_str(_.op_sval ? _.op_sval : "")
             + s_1asm(emit_fmt("newobj     instance void pat_%d_%d::.ctor(string)", 0, _.nid));
    }
    if (PLATFORM_WASM) { return "          (call $bb_break_new)\n"; }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_break(void) {
    x86_begin();                                    /* TEXT: per-box uid for .Lx<uid>_<n> internal labels */
    _.x86_scratch_off = bb_slot_claim(8);           /* z @ [r12+off], z_orig @ [r12+off+4] (BREAKX) */
    bb_emit_x86(bb_pat_break_str());
}
