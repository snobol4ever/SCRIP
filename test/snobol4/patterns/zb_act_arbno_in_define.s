                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__MATCHIT_α:
proc_LBL__MATCHIT_α_body:
#=======================================================================================================================
# MATCHIT S POS(0) ARBNO('a') . V RPOS(0)                            :S(MI_YES)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_var_α
n0_statement_begin_β:                                               jmp   n15_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # S
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n2_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n2_match_begin_α:       mov              rdi, qword ptr [rsp + 0]                       # var
                        mov              rsi, qword ptr [rsp + 8]
                        mov              qword ptr [rsp + 88], rsp                      # old____
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx23_0:                mov              r14d, dword ptr [rsp + 0]; jmp   n3_match_pos_α
n2_match_begin_β:       mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d;                 jg    .Lx23_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                    jne   .Lx23_1
                                                                    jmp   .Lx23_0
.Lx23_1:
n2_match_begin_af:      sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [r12 + 8]                       # cas_rsp_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 72]                      # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16;                   jmp   n15_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_match_pos_α:         mov              rax, 0
                        cmp              r14d, eax;                 jne   n2_match_begin_β
                                                                    jmp   n4_match_assign_save_α
n3_match_pos_β:                                                     jmp   n2_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n4_match_assign_save_α: sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d; jmp   n5_match_arbno_α
n4_match_assign_save_β: add              rsp, 16;                   jmp   n2_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_match_arbno_α:       sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              dword ptr [rsp + 4], r14d; jmp   n6_match_assign_cond_α
n5_match_arbno_β:                                                   jmp   n14_match_lit_α
n5_match_arbno_as:      mov              eax, dword ptr [rsp + 4]
                        cmp              r14d, eax;                 je    n14_match_lit_β
                        mov              dword ptr [rsp + 4], r14d; jmp   n6_match_assign_cond_α
n5_match_arbno_af:      mov              eax, dword ptr [rsp + 0]
                        cmp              r14d, eax;                 jne   n14_match_lit_β
                        add              rsp, 16;                   jmp   n4_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n6_match_assign_cond_α: mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                   jmp   n7_match_rpos_α
n6_match_assign_cond_β: sub              r12, 24;                   jmp   n5_match_arbno_β
#-----------------------------------------------------------------------------------------------------------------------
n7_match_rpos_α:        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx;                 jne   n6_match_assign_cond_β
                                                                    jmp   n8_match_end_α
#-----------------------------------------------------------------------------------------------------------------------
n8_match_end_α:         mov              r8, r12
.Lx33_8:                sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx33_8
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx33_5:                sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                  jne   .Lx33_5
                        lea              rdi, [r8 + 24]
                        mov              rdx, r13
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx33_1:                test             rax, rax;                  je    .Lx33_2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx33_3]
                        lea              rdx, [rip + .Lx33_4];      jmp   rax
.Lx33_3:                mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx33_1
.Lx33_4:                mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx33_1
.Lx33_2:                mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_dcap_end_ok_close@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx33_10:               sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                  jne   .Lx33_10
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n9_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_end_α:     add              rsp, 16;                   jmp   n10_statement_begin_α
#=======================================================================================================================
# MI_YES  MATCHIT = V                                                 :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_begin_α:                                              jmp   n11_var_α
n10_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # V
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n12_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n12_assign_α:           mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # MATCHIT
                        mov              qword ptr [r9 + 8], rdx;   jmp   n13_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_end_α:    add              rsp, 16;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n14_match_lit_α:        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                 jg    n5_match_arbno_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 97;                   jne   n5_match_arbno_af
                        add              r14d, 1;                   jmp   n5_match_arbno_as
n14_match_lit_β:        sub              r14d, 1;                   jmp   n5_match_arbno_af
#=======================================================================================================================
#         MATCHIT = 'fail'                                            :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_begin_α:                                              jmp   n16_lit_string_α
n15_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx46_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n17_assign_α
.Lx46_0:                .quad            .Lx46_0_s
.Lx46_0_s:              .string          "fail"
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:           mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # MATCHIT
                        mov              qword ptr [r9 + 8], rdx;   jmp   n18_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_end_α:    add              rsp, 16;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__MATCHIT_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__MATCHIT_β:
                                                                    jmp   proc_LBL__MATCHIT_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__MATCHIT_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__MATCHIT_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_MATCHIT_α
proc_MATCHIT_α:
proc_MATCHIT_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n50_save_restore_α:                                                 jmp   n51_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n51_goto_deferred_α:    lea              rax, [rip + proc_LBL__MATCHIT_α]; jmp   rax
                                                                    jmp   n52_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n52_save_restore_α:
MATCHIT_alpha:          mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx58_2
                        mov              eax, 16
.Lx58_2:                sub              rsp, 64
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 0]                        # MATCHIT
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0;                    jbe   .Lx58_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 16]                       # S
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx58_41
.Lx58_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx58_41:               lea              r10, [rip + MATCHIT_gamma]
                        lea              r11, [rip + MATCHIT_omega]
                        lea              rax, [rip + proc_LBL__MATCHIT_α]; jmp   rax
MATCHIT_gamma:          mov              rdi, qword ptr [r9 + 0]                        # MATCHIT
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx58_75
                        mov              eax, 16
.Lx58_75:               lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rcx, 0;                    jbe   .Lx58_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # S
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx58_110
.Lx58_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx58_110:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                    jbe   .Lx58_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx58_3:                mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
MATCHIT_omega:          mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx58_145
                        mov              eax, 16
.Lx58_145:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax                        # MATCHIT
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rcx, 0;                    jbe   .Lx58_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # S
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx58_180
.Lx58_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx58_180:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                    jbe   .Lx58_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx58_4:                mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_MATCHIT_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_MATCHIT_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_MATCHIT_β:
                                                                    jmp   proc_MATCHIT_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_MATCHIT_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_MATCHIT_ω:
                                                                    jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__MATCHIT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__MATCHIT_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 384
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "MATCHIT"
.Lgvan1:                .string          "S"
.Lgvan2:                .string          "V"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 3
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 3
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                    jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        sub              rsp, 0
#=======================================================================================================================
#         DEFINE('MATCHIT(S)') :(MATCHIT_END)
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_begin_α:                                              jmp   n60_func_activate_α
n59_statement_begin_β:                                              jmp   n62_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n60_func_activate_α:    mov              rdi, qword ptr [rip + .Lx79_0]
                        mov              rsi, qword ptr [rip + .Lx79_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_MATCHIT_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n61_statement_end_α
n60_func_activate_β:                                                jmp   n59_statement_begin_β
.Lx79_0:                .quad            .Lx79_0_s
.Lx79_0_s:              .string          "MATCHIT"
.Lx79_1:                .quad            .Lx79_1_s
.Lx79_1_s:              .string          "S"
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_end_α:                                                jmp   n62_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n62_statement_begin_α:                                              jmp   n63_statement_end_α
n62_statement_begin_β:                                              jmp   n64_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n63_statement_end_α:                                                jmp   n64_statement_begin_α
#=======================================================================================================================
#         OUTPUT = MATCHIT('aaa')
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_begin_α:                                              jmp   n65_lit_string_α
n64_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n65_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx88_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n66_call_α
.Lx88_0:                .quad            .Lx88_0_s
.Lx88_0_s:              .string          "aaa"
#-----------------------------------------------------------------------------------------------------------------------
n66_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx90_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + MATCHIT_alpha]; jmp   rax
.Lx90_2:                mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx90_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n64_statement_begin_β
.Lx90_240:                                                          jmp   n67_assign_α
n66_call_β:                                                         jmp   n64_statement_begin_β
.Lx90_0:                .quad            .Lx90_0_s
.Lx90_0_s:              .string          "MATCHIT"
#-----------------------------------------------------------------------------------------------------------------------
n67_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx91_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n68_statement_end_α
.Lx91_0:                .quad            .Lx91_0_s
.Lx91_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_end_α:    add              rsp, 32;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n69_goto_α:                                                         jmp   n70_statement_begin_α
n69_goto_β:                                                         jmp   main_ω
#=======================================================================================================================
# MI_YES  MATCHIT = V                                                 :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_begin_α:                                              jmp   n71_var_α
n70_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n71_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # V
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n72_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n72_assign_α:           mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # MATCHIT
                        mov              qword ptr [r9 + 8], rdx;   jmp   n73_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_end_α:                                                jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n74_goto_α:                                                         jmp   n62_statement_begin_α
n74_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 lea              rdi, [rip + .S1]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                    jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        add              rsp, 0
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        add              rsp, 0
                        mov              edi, 1
                        call             exit@PLT
                        .section         .rodata
.S0:                    .string          "V"
.S1:                    .string          "BOMB-RETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
                        .text
                        .section         .note.GNU-stack,"",@progbits
