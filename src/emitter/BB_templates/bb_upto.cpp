/* bb_upto.cpp — BB template for BB_UPTO (Icon upto(cset) position generator).
   ICN-T-5, GOAL-BB-TEMPLATE-LADDER Invariant 8: ALL logic inline x86, NO RT calls.
   sval=cset string. hay/slen come from runtime scan context (not compile-time BB payload).
   State in .data: one quad counter (0-based pos). α resets to 0, β advances by 1.
   Scan loop: load hay[counter], call strchr(cset,c), on match yield counter+1 (1-based). */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_upto_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        int id = bb_node_id(pBB);
        const char *cset = pBB->sval  ? pBB->sval  : "";
        const char *hay  = "";  /* TODO: read from runtime scan subject, not BB payload */
        int64_t    slen  = pBB->ival;
        std::string cnt  = emit_fmt(".Lupto%d_c", id);
        std::string scan = emit_fmt(".Lupto%d_scan", id);
        std::string done = emit_fmt(".Lupto%d_done", id);
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — UPTO");
        if (MEDIUM_TEXT) {
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX UPTO(cset='%.8s' len=%ld)", cset, (long)slen))
                 + s_directive(".section .data")
                 + s_directive(cnt + ": .quad 0")
                 + s_directive(".section .text")
                 + s_directive(".intel_syntax noprefix")
                 /* α: reset counter to 0 */
                 + s_2asm("lea rax,", "[rip + " + cnt + "]")
                 + s_2asm("mov qword ptr [rax],", "0")
                 + s_1asm("jmp " + scan)
                 /* β: advance counter by 1 */
                 + s_L1asm(emit_fmt("%s:", _.lbl_β),
                            "lea rax, [rip + " + cnt + "]")
                 + s_2asm("mov rcx,", "[rax]")
                 + s_2asm("inc", "rcx")
                 + s_2asm("mov [rax],", "rcx")
                 /* scan loop: while counter < slen, check hay[counter] in cset */
                 + s_L1asm(scan + ":", "")
                 + s_2asm("lea rax,", "[rip + " + cnt + "]")
                 + s_2asm("mov rcx,", "[rax]")
                 + s_2asm("cmp rcx,", emit_fmt("%ld", (long)slen))
                 + s_2asm("jge", _.lbl_ω)
                 /* load hay[counter] into esi (second arg) */
                 + s_2asm("mov rax,", emit_fmt("0x%lx", (unsigned long)(uintptr_t)hay))
                 + s_2asm("movzx esi,", "byte ptr [rax + rcx]")
                 /* call strchr(cset, c): rdi=cset, esi=char */
                 + s_2asm("mov rdi,", emit_fmt("0x%lx", (unsigned long)(uintptr_t)cset))
                 + s_2asm("call", "strchr@PLT")
                 + s_2asm("test", "rax, rax")
                 + s_2asm("jz", scan)   /* not in cset: advance counter, loop */
                 /* match: yield counter+1 (1-based Icon position) */
                 /* result goes... BB_UPTO yields an integer. Push it onto the SM stack. */
                 /* Load counter, add 1, store as integer result in the SM value register */
                 + s_1asm("jmp " + done)
                 /* advance counter inline before looping back */
                 + s_L1asm(scan + "_adv:", "")
                 + s_2asm("lea rax,", "[rip + " + cnt + "]")
                 + s_2asm("inc qword ptr [rax],", "")
                 + s_1asm("jmp " + scan)
                 + s_L1asm(done + ":", "")
                 + s_2asm("lea rax,", "[rip + " + cnt + "]")
                 + s_2asm("mov rax,", "[rax]")
                 + s_1asm("inc rax")
                 /* push integer result — follow SM stack convention */
                 + s_2asm("call", "rt_push_int@PLT")
                 + s_2asm("jmp", _.lbl_γ);
        }
        if (MEDIUM_BINARY) {
            /* Encode: α resets counter to 0, β increments, scan loop finds next cset match.
               Uses pBB->counter as the state slot (emitter-process address, valid for native codegen).
               movabs rcx, &counter; mov [rcx],0; jmp scan  -- α
               lbl_β: movabs rcx,&counter; inc [rcx]     -- β
               scan: movabs rcx,&counter; mov rcx,[rcx]; movabs rdi,slen; cmp rcx,rdi
                     jge lbl_ω; movabs rax,hay; movzx esi,[rax+rcx]
                     movabs rdi,cset; movabs rax,strchr; call rax
                     test rax,rax; jz scan; movabs rax,hay; lea rax,[rax+counter]
                     mov dword[r12],1; mov dword[r12+4],1; mov [r12+8],rax; add r12,16
                     jmp lbl_γ */
            uint64_t cnt  = (uint64_t)(uintptr_t)&pBB->counter;
            uint64_t hay_ = (uint64_t)(uintptr_t)hay;
            uint64_t cset_= (uint64_t)(uintptr_t)cset;
            uint64_t schr; { const char*(*fp)(const char*,int) = strchr; schr=(uint64_t)(uintptr_t)(void*)fp; }
            std::string b;
            /* α: reset counter */
            b += bytes(2,"\x48\xB9") + u64le(cnt);        /* movabs rcx, &counter */
            b += bytes(3,"\x48\xC7\x01") + u32le(0);     /* mov qword[rcx], 0 */
            int alpha_jmp = (int)b.size();
            b += bytes(2,"\xEB\x00");                      /* jmp scan (patch below) */
            /* lbl_β: increment counter */
            int back_off = (int)b.size();
            b += bytes(2,"\x48\xB9") + u64le(cnt);
            b += bytes(3,"\x48\xFF\x01");                 /* inc qword[rcx] */
            /* scan: load and check counter */
            int scan_off = (int)b.size();
            ((char*)b.data())[alpha_jmp+1] = (char)(scan_off - (alpha_jmp+2));
            b += bytes(2,"\x48\xB9") + u64le(cnt);
            b += bytes(3,"\x48\x8B\x09");                 /* mov rcx,[rcx] */
            b += bytes(2,"\x48\xBF") + u64le((uint64_t)slen);
            b += bytes(3,"\x48\x39\xF9");                 /* cmp rcx,rdi(slen) */
            int fail_off = (int)b.size();
            b += bytes(6,"\x0F\x8D\x00\x00\x00\x00"); /* jge lbl_ω RELOC */
            /* check hay[counter] in cset */
            b += bytes(2,"\x48\xB8") + u64le(hay_);
            b += bytes(4,"\x0F\xB6\x34\x08");            /* movzx esi,byte[rax+rcx] */
            b += bytes(2,"\x48\xBF") + u64le(cset_);
            b += bytes(2,"\x48\xB8") + u64le(schr);
            b += bytes(2,"\xFF\xD0");                      /* call rax (strchr) */
            b += bytes(3,"\x48\x85\xC0");                 /* test rax,rax */
            /* jz back to β path (not found: increment and rescan) */
            int jz_off = (int)b.size();
            b += bytes(2,"\x74\x00");
            ((char*)b.data())[jz_off+1] = (char)(back_off - (jz_off+2));
            /* found: reload counter, compute hay+counter as char ptr */
            b += bytes(2,"\x48\xB9") + u64le(cnt);
            b += bytes(3,"\x48\x8B\x09");                 /* rcx = counter */
            b += bytes(2,"\x48\xB8") + u64le(hay_);
            b += bytes(4,"\x48\x8D\x04\x08");            /* lea rax,[rax+rcx] = hay+counter */
            /* push DESCR_t{DT_S=1,slen=1,ptr=rax} onto r12 */
            b += bytes(4,"\x41\xC7\x04\x24") + u32le(1);     /* mov dword[r12],1 (DT_S) */
            b += bytes(5,"\x41\xC7\x44\x24\x04") + u32le(1); /* mov dword[r12+4],1 (slen) */
            b += bytes(5,"\x49\x89\x44\x24\x08");            /* mov [r12+8],rax */
            b += bytes(4,"\x49\x83\xC4\x10");                 /* add r12,16 */
            int succ_off = (int)b.size();
            b += bytes(5,"\xE9\x00\x00\x00\x00");       /* jmp lbl_γ RELOC */
            bin = { {fail_off+2, succ_off+1, back_off},
                    {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p},
                    {false, false, true} };
            return b;
        }
    }
    return std::string();
}
extern "C" void bb_upto(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_upto_str(pBB, bin), bin);
}
