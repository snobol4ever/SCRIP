/* bb_pat_atp.cpp — BB template for @var cursor capture (SBL-ATP).
   @var is a zero-width single-shot leaf: α writes the current 0-based cursor Δ to var as an
   integer (via rt_at_cursor), then succeeds to γ; β fails to ω (single attempt, like a leaf
   POS). Mirrors the mode-2 oracle bb_exec.c case BB_PAT_ATP and lower.c TT_CAPT_CURSOR n==1.
   One file per Byrd Box per RULES.md (BB_templates folder rule). X86 ONLY — other backends stub. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit_bb.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SBL-ATP BINARY/TEXT arm runtime helper. Called with rdi=varname, esi=cur_delta; writes Δ as an
   integer DESCR to the variable. Declared here so the BINARY arm can bake &rt_at_cursor as imm64. */
extern "C" void rt_at_cursor(const char *varname, int cur_delta);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_atp_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    int nid = bb_node_id(pBB); int sid = 0;
    const char *varname = (g_emit.op_name1 && g_emit.op_name1[0]) ? g_emit.op_name1
                        : (pBB->sval ? pBB->sval : "");
    if (PLATFORM_X86) {
        if (!varname || !varname[0]) {
            return std::string(); /* no varname — emit nothing, honest skip */
        }
        if (MEDIUM_BINARY) {
            /* SBL-ATP MEDIUM_BINARY arm. ABI on entry: r10 = &Δ (the global cursor int).
               rt_at_cursor(varname, Δ) writes Δ as an integer DESCR to the variable. The call is
               side-effecting (NV_SET on an associated/OUTPUT var can enter the print path which
               clobbers caller-saved r10), so push r10 TWICE around the call — preserve r10 AND keep
               rsp 16-aligned at the call, exactly as bb_capture's SBL-CAP-OUTPUT-R10 convention.
               Layout (44 bytes total):
                [0]   41 8b 32              mov esi, [r10]            ; esi = cur_delta = Δ
                [3]   48 bf <8>             movabs rdi, varname
                [13]  48 b8 <8>             movabs rax, &rt_at_cursor
                [23]  41 52                 push r10                  ; preserve &Δ (caller-saved)
                [25]  41 52                 push r10                  ; keep rsp 16-aligned for the call
                [27]  ff d0                 call rax
                [29]  41 5a                 pop r10
                [31]  41 5a                 pop r10                   ; restore &Δ
                [33]  e9 [r32]              jmp → γ                   ; site[0] rel32 @34
                lbl_β at [38]:
                [38]  e9 [r32]              jmp → ω                   ; site[2] rel32 @39
                end at [43].
               Sites: site[0]=34 γ rel32; site[1]=38 β label-define; site[2]=39 ω rel32. */
            uint64_t vna = (uint64_t)(uintptr_t)varname;
            uint64_t rca = (uint64_t)(uintptr_t)rt_at_cursor;
            std::string blob;
            blob += std::string("\x41\x8b\x32", 3);            /* [0]  mov esi, [r10] */
            blob += std::string("\x48\xbf", 2); blob += u64le(vna);  /* [3]  movabs rdi, varname */
            blob += std::string("\x48\xb8", 2); blob += u64le(rca);  /* [13] movabs rax, &rt_at_cursor */
            blob += std::string("\x41\x52", 2);                /* [23] push r10 */
            blob += std::string("\x41\x52", 2);                /* [25] push r10 */
            blob += std::string("\xff\xd0", 2);                /* [27] call rax */
            blob += std::string("\x41\x5a", 2);                /* [29] pop r10 */
            blob += std::string("\x41\x5a", 2);                /* [31] pop r10 */
            blob += std::string("\xe9", 1);     blob += u32le(0);    /* [33] jmp γ   rel32 @34 */
            /* lbl_β at [38] */
            blob += std::string("\xe9", 1);     blob += u32le(0);    /* [38] jmp ω   rel32 @39 */
            bin = { {34, 38, 39},
                    {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p},
                    {false, true, false} };
            (void)nid; (void)sid;
            return blob;
        }
        const char *vn_label = emit_intern_str(varname);
        bin = { {}, {}, {} };
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — ATP"))
             + IF(MEDIUM_TEXT,
                   s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX ATP(@%s)", varname))
                 + s_2asm("mov", "esi, [r10]")
                 + s_2asm("lea", emit_fmt("rdi, [rip + %s]", vn_label ? vn_label : "??"))
                 + s_2asm("push", "r10")
                 + s_2asm("push", "r10")
                 + s_2asm("call", "rt_at_cursor@PLT")
                 + s_2asm("pop", "r10")
                 + s_2asm("pop", "r10")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_atp(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_pat_atp_str(pBB, bin), bin);
}
