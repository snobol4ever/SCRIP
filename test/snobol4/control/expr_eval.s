                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
FN__PAT$0:
PAT$0_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 72
                        mov              rcx, qword ptr [rbp + 8]
                        mov              qword ptr [rbp + -8], rcx
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        mov              qword ptr [rbp + -32], r12
#-----------------------------------------------------------------------------------------------------------------------
n0_match_span_α:        sub              rsp, 16
                        lea              rdi, [rip + .C0]
                        movsxd           rcx, r14d
.Lx2_0:                 cmp              ecx, r15d;                           jge   .Lx2_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0;               je    .Lx2_1
                        add              ecx, 1;                              jmp   .Lx2_0
.Lx2_1:                 cmp              ecx, r14d;                           jg    .Lx2_240
                        add              rsp, 16;                             jmp   PAT$0_ω
.Lx2_240:               mov              dword ptr [rbp + -60], r14d
                        mov              r14d, ecx;                           jmp   PAT$0_γ
n0_match_span_β:        mov              r14d, dword ptr [rbp + -60]
                        add              rsp, 16;                             jmp   PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
PAT$0_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$0_β:
                                                                              jmp   n0_match_span_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$0_γ:
                        mov              rcx, qword ptr [rbp + -16]
                        push             rbp
                        push             rcx
                        mov              rcx, qword ptr [rbp + -8]
                        push             rcx
                        lea              rax, [rip + PAT$0_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
PAT$0_ω:
                        mov              r12, qword ptr [rbp + -32]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 8
                        ret
#-----------------------------------------------------------------------------------------------------------------------
FN__PAT$1:
PAT$1_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 72
                        mov              rcx, qword ptr [rbp + 8]
                        mov              qword ptr [rbp + -8], rcx
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        mov              qword ptr [rbp + -32], r12
#-----------------------------------------------------------------------------------------------------------------------
n3_match_any_α:         mov              eax, r14d
                        cmp              eax, r15d;                           jge   PAT$1_ω
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        lea              rdi, [rip + .C1]
                        cmp              byte ptr [rdi+rsi], 0;               je    PAT$1_ω
                        add              r14d, 1;                             jmp   n4_match_alternate_α
n3_match_any_β:         sub              r14d, 1;                             jmp   PAT$1_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_match_alternate_α:   mov              dword ptr [rbp + -72], r14d
                        lea              rax, [rip + .Lx11_21]
                        mov              qword ptr [rbp + -56], rax;          jmp   n7_match_any_α
.Lx11_21:               lea              rax, [rip + .Lx11_19]
                        mov              qword ptr [rbp + -56], rax;          jmp   n6_match_defer_α
n4_match_alternate_s0:  lea              rax, [rip + .Lx11_40]
                        mov              qword ptr [rbp + -64], rax;          jmp   n4_match_alternate_as
n4_match_alternate_s1:  lea              rax, [rip + .Lx11_41]
                        mov              qword ptr [rbp + -64], rax;          jmp   n4_match_alternate_as
.Lx11_40:                                                                     jmp   n7_match_any_β
.Lx11_41:                                                                     jmp   n6_match_defer_β
n4_match_alternate_as:                                                        jmp   n5_match_defer_α
n4_match_alternate_β:   mov              rax, qword ptr [rbp + -64];          jmp   rax
n4_match_alternate_af:  mov              r14d, dword ptr [rbp + -72]
                        mov              rax, qword ptr [rbp + -56];          jmp   rax
.Lx11_19:                                                                     jmp   n3_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n5_match_defer_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 1
                        lea              rdx, [rip + .S0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx12_16
                        mov              rax, qword ptr [rdx + 0]
.Lx12_16:               test             rax, rax;                            jz    .Lx12_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx12_5]
                        push             rcx
                        lea              rcx, [rip + .Lx12_4]
                        push             rcx;                                 jmp   rax
.Lx12_4:                                                                      jmp   PAT$1_γ
.Lx12_5:                add              rsp, 16;                             jmp   n4_match_alternate_β
.Lx12_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 1
                        lea              rdx, [rip + .S0]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            jns   .Lx12_240
                        add              rsp, 16;                             jmp   n4_match_alternate_β
.Lx12_240:              mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx12_6]
                        push             rcx
                        push             rax;                                 jmp   PAT$1_γ
.Lx12_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax
                        add              rsp, 16;                             jmp   n4_match_alternate_β
n5_match_defer_β:       cmp              qword ptr [rsp + 0], 0;              jne   .Lx12_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx12_12
                                                                              jmp   rax
.Lx12_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n6_match_defer_α:       mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S1]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx13_16
                        mov              rax, qword ptr [rdx + 0]
.Lx13_16:               test             rax, rax;                            jz    .Lx13_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx13_5]
                        push             rcx
                        lea              rcx, [rip + .Lx13_4]
                        push             rcx;                                 jmp   rax
.Lx13_4:                                                                      jmp   n4_match_alternate_s1
.Lx13_5:                                                                      jmp   n4_match_alternate_af
.Lx13_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S1]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n4_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx13_6]
                        push             rcx
                        push             rax;                                 jmp   n4_match_alternate_s1
.Lx13_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n4_match_alternate_af
n6_match_defer_β:       cmp              qword ptr [rsp + 0], 0;              jne   .Lx13_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx13_12
                                                                              jmp   rax
.Lx13_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n7_match_any_α:         mov              eax, r14d
                        cmp              eax, r15d;                           jge   n4_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 43;                             je    .Lx15_0
                        cmp              esi, 45;                             je    .Lx15_0
                                                                              jmp   n4_match_alternate_af
.Lx15_0:                add              r14d, 1;                             jmp   n4_match_alternate_s0
n7_match_any_β:         sub              r14d, 1;                             jmp   n4_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
PAT$1_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$1_β:
                                                                              jmp   n5_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$1_γ:
                        mov              rcx, qword ptr [rbp + -16]
                        push             rbp
                        push             rcx
                        mov              rcx, qword ptr [rbp + -8]
                        push             rcx
                        lea              rax, [rip + PAT$1_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
PAT$1_ω:
                        mov              r12, qword ptr [rbp + -32]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 8
                        ret
#-----------------------------------------------------------------------------------------------------------------------
FN__PAT$2:
PAT$2_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 136
                        mov              rcx, qword ptr [rbp + 8]
                        mov              qword ptr [rbp + -8], rcx
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        mov              qword ptr [rbp + -32], r12
#-----------------------------------------------------------------------------------------------------------------------
n16_match_alternate_α:  sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx30_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n21_match_defer_α
.Lx30_21:               lea              rax, [rip + .Lx30_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n18_match_defer_α
n16_match_alternate_s0: lea              rax, [rip + .Lx30_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n16_match_alternate_as
n16_match_alternate_s1: lea              rax, [rip + .Lx30_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n16_match_alternate_as
.Lx30_40:                                                                     jmp   n20_goto_β
.Lx30_41:                                                                     jmp   n17_goto_β
n16_match_alternate_as:                                                       jmp   PAT$2_γ
n16_match_alternate_β:  mov              rax, qword ptr [rsp + 8];            jmp   rax
n16_match_alternate_af: mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx30_19:               add              rsp, 32;                             jmp   PAT$2_ω
#-----------------------------------------------------------------------------------------------------------------------
n17_goto_α:                                                                   jmp   n16_match_alternate_af
n17_goto_β:                                                                   jmp   n16_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n18_match_defer_α:      mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 5
                        lea              rdx, [rip + .S2]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx32_16
                        mov              rax, qword ptr [rdx + 0]
.Lx32_16:               test             rax, rax;                            jz    .Lx32_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx32_5]
                        push             rcx
                        lea              rcx, [rip + .Lx32_4]
                        push             rcx;                                 jmp   rax
.Lx32_4:                                                                      jmp   n19_match_defer_α
.Lx32_5:                                                                      jmp   n16_match_alternate_af
.Lx32_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 5
                        lea              rdx, [rip + .S2]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n16_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx32_6]
                        push             rcx
                        push             rax;                                 jmp   n19_match_defer_α
.Lx32_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n16_match_alternate_af
n18_match_defer_β:      cmp              qword ptr [rsp + 0], 0;              jne   .Lx32_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx32_12
                                                                              jmp   rax
.Lx32_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n19_match_defer_α:      mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 6
                        lea              rdx, [rip + .S3]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx33_16
                        mov              rax, qword ptr [rdx + 0]
.Lx33_16:               test             rax, rax;                            jz    .Lx33_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx33_5]
                        push             rcx
                        lea              rcx, [rip + .Lx33_4]
                        push             rcx;                                 jmp   rax
.Lx33_4:                                                                      jmp   n16_match_alternate_s1
.Lx33_5:                                                                      jmp   n18_match_defer_β
.Lx33_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 6
                        lea              rdx, [rip + .S3]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n18_match_defer_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx33_6]
                        push             rcx
                        push             rax;                                 jmp   n16_match_alternate_s1
.Lx33_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n18_match_defer_β
n19_match_defer_β:      cmp              qword ptr [rsp + 0], 0;              jne   .Lx33_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx33_12
                                                                              jmp   rax
.Lx33_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n20_goto_α:                                                                   jmp   n16_match_alternate_af
n20_goto_β:                                                                   jmp   n16_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n21_match_defer_α:      mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S4]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx35_16
                        mov              rax, qword ptr [rdx + 0]
.Lx35_16:               test             rax, rax;                            jz    .Lx35_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx35_5]
                        push             rcx
                        lea              rcx, [rip + .Lx35_4]
                        push             rcx;                                 jmp   rax
.Lx35_4:                                                                      jmp   n22_match_lit_α
.Lx35_5:                                                                      jmp   n16_match_alternate_af
.Lx35_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S4]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n16_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx35_6]
                        push             rcx
                        push             rax;                                 jmp   n22_match_lit_α
.Lx35_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n16_match_alternate_af
n21_match_defer_β:      cmp              qword ptr [rsp + 0], 0;              jne   .Lx35_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx35_12
                                                                              jmp   rax
.Lx35_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n22_match_lit_α:        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n21_match_defer_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 46;                             jne   n21_match_defer_β
                        add              r14d, 1;                             jmp   n23_match_alternate_α
n22_match_lit_β:        sub              r14d, 1;                             jmp   n21_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n23_match_alternate_α:  sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx39_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n28_match_defer_α
.Lx39_21:               lea              rax, [rip + .Lx39_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n27_match_defer_α
n23_match_alternate_s0: lea              rax, [rip + .Lx39_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n23_match_alternate_as
n23_match_alternate_s1: lea              rax, [rip + .Lx39_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n23_match_alternate_as
.Lx39_40:                                                                     jmp   n28_match_defer_β
.Lx39_41:                                                                     jmp   n27_match_defer_β
n23_match_alternate_as:                                                       jmp   n24_match_alternate_α
n23_match_alternate_β:  mov              rax, qword ptr [rsp + 8];            jmp   rax
n23_match_alternate_af: mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx39_19:               add              rsp, 32;                             jmp   n22_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n24_match_alternate_α:  sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx41_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n26_match_defer_α
.Lx41_21:               lea              rax, [rip + .Lx41_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n25_match_defer_α
n24_match_alternate_s0: lea              rax, [rip + .Lx41_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n24_match_alternate_as
n24_match_alternate_s1: lea              rax, [rip + .Lx41_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n24_match_alternate_as
.Lx41_40:                                                                     jmp   n26_match_defer_β
.Lx41_41:                                                                     jmp   n25_match_defer_β
n24_match_alternate_as:                                                       jmp   n16_match_alternate_s0
n24_match_alternate_β:  mov              rax, qword ptr [rsp + 8];            jmp   rax
n24_match_alternate_af: mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx41_19:               add              rsp, 32;                             jmp   n23_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n25_match_defer_α:      mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 4
                        lea              rdx, [rip + .S5]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx42_16
                        mov              rax, qword ptr [rdx + 0]
.Lx42_16:               test             rax, rax;                            jz    .Lx42_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx42_5]
                        push             rcx
                        lea              rcx, [rip + .Lx42_4]
                        push             rcx;                                 jmp   rax
.Lx42_4:                                                                      jmp   n24_match_alternate_s1
.Lx42_5:                                                                      jmp   n24_match_alternate_af
.Lx42_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 4
                        lea              rdx, [rip + .S5]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n24_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx42_6]
                        push             rcx
                        push             rax;                                 jmp   n24_match_alternate_s1
.Lx42_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n24_match_alternate_af
n25_match_defer_β:      cmp              qword ptr [rsp + 0], 0;              jne   .Lx42_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx42_12
                                                                              jmp   rax
.Lx42_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n26_match_defer_α:      mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 3
                        lea              rdx, [rip + .S6]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx43_16
                        mov              rax, qword ptr [rdx + 0]
.Lx43_16:               test             rax, rax;                            jz    .Lx43_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx43_5]
                        push             rcx
                        lea              rcx, [rip + .Lx43_4]
                        push             rcx;                                 jmp   rax
.Lx43_4:                                                                      jmp   n24_match_alternate_s0
.Lx43_5:                                                                      jmp   n24_match_alternate_af
.Lx43_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 3
                        lea              rdx, [rip + .S6]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n24_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx43_6]
                        push             rcx
                        push             rax;                                 jmp   n24_match_alternate_s0
.Lx43_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n24_match_alternate_af
n26_match_defer_β:      cmp              qword ptr [rsp + 0], 0;              jne   .Lx43_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx43_12
                                                                              jmp   rax
.Lx43_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n27_match_defer_α:      mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 2
                        lea              rdx, [rip + .S7]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx44_16
                        mov              rax, qword ptr [rdx + 0]
.Lx44_16:               test             rax, rax;                            jz    .Lx44_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx44_5]
                        push             rcx
                        lea              rcx, [rip + .Lx44_4]
                        push             rcx;                                 jmp   rax
.Lx44_4:                                                                      jmp   n23_match_alternate_s1
.Lx44_5:                                                                      jmp   n23_match_alternate_af
.Lx44_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 2
                        lea              rdx, [rip + .S7]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n23_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx44_6]
                        push             rcx
                        push             rax;                                 jmp   n23_match_alternate_s1
.Lx44_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n23_match_alternate_af
n27_match_defer_β:      cmp              qword ptr [rsp + 0], 0;              jne   .Lx44_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx44_12
                                                                              jmp   rax
.Lx44_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n28_match_defer_α:      mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 1
                        lea              rdx, [rip + .S8]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx45_16
                        mov              rax, qword ptr [rdx + 0]
.Lx45_16:               test             rax, rax;                            jz    .Lx45_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx45_5]
                        push             rcx
                        lea              rcx, [rip + .Lx45_4]
                        push             rcx;                                 jmp   rax
.Lx45_4:                                                                      jmp   n23_match_alternate_s0
.Lx45_5:                                                                      jmp   n23_match_alternate_af
.Lx45_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 1
                        lea              rdx, [rip + .S8]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n23_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx45_6]
                        push             rcx
                        push             rax;                                 jmp   n23_match_alternate_s0
.Lx45_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n23_match_alternate_af
n28_match_defer_β:      cmp              qword ptr [rsp + 0], 0;              jne   .Lx45_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx45_12
                                                                              jmp   rax
.Lx45_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
PAT$2_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$2_β:
                                                                              jmp   PAT$2_ω
#-----------------------------------------------------------------------------------------------------------------------
PAT$2_γ:
                        mov              rcx, qword ptr [rbp + -16]
                        push             rbp
                        push             rcx
                        mov              rcx, qword ptr [rbp + -8]
                        push             rcx
                        lea              rax, [rip + PAT$2_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
PAT$2_ω:
                        mov              r12, qword ptr [rbp + -32]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 8
                        ret
#-----------------------------------------------------------------------------------------------------------------------
FN__PAT$3:
PAT$3_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 40
                        mov              rcx, qword ptr [rbp + 8]
                        mov              qword ptr [rbp + -8], rcx
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        mov              qword ptr [rbp + -32], r12
#-----------------------------------------------------------------------------------------------------------------------
n46_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n47_match_any_α
n46_match_assign_save_β:
                        add              rsp, 16;                             jmp   PAT$3_ω
#-----------------------------------------------------------------------------------------------------------------------
n47_match_any_α:        mov              eax, r14d
                        cmp              eax, r15d;                           jl    .Lx52_240
                        add              rsp, 16;                             jmp   PAT$3_ω
.Lx52_240:              movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 43;                             je    .Lx52_0
                        cmp              esi, 45;                             je    .Lx52_0
                        add              rsp, 16;                             jmp   PAT$3_ω
.Lx52_0:                add              r14d, 1;                             jmp   n48_match_assign_cond_α
n47_match_any_β:        sub              r14d, 1
                        add              rsp, 16;                             jmp   PAT$3_ω
#-----------------------------------------------------------------------------------------------------------------------
n48_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S9]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   PAT$3_γ
n48_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n47_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$3_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$3_β:
                                                                              jmp   n48_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$3_γ:
                        mov              rcx, qword ptr [rbp + -16]
                        push             rbp
                        push             rcx
                        mov              rcx, qword ptr [rbp + -8]
                        push             rcx
                        lea              rax, [rip + PAT$3_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
PAT$3_ω:
                        mov              r12, qword ptr [rbp + -32]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 8
                        ret
#-----------------------------------------------------------------------------------------------------------------------
FN__PAT$4:
PAT$4_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 40
                        mov              rcx, qword ptr [rbp + 8]
                        mov              qword ptr [rbp + -8], rcx
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        mov              qword ptr [rbp + -32], r12
#-----------------------------------------------------------------------------------------------------------------------
n55_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n56_match_any_α
n55_match_assign_save_β:
                        add              rsp, 16;                             jmp   PAT$4_ω
#-----------------------------------------------------------------------------------------------------------------------
n56_match_any_α:        mov              eax, r14d
                        cmp              eax, r15d;                           jl    .Lx61_240
                        add              rsp, 16;                             jmp   PAT$4_ω
.Lx61_240:              movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 42;                             je    .Lx61_0
                        cmp              esi, 47;                             je    .Lx61_0
                        add              rsp, 16;                             jmp   PAT$4_ω
.Lx61_0:                add              r14d, 1;                             jmp   n57_match_assign_cond_α
n56_match_any_β:        sub              r14d, 1
                        add              rsp, 16;                             jmp   PAT$4_ω
#-----------------------------------------------------------------------------------------------------------------------
n57_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S9]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   PAT$4_γ
n57_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n56_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$4_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$4_β:
                                                                              jmp   n57_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$4_γ:
                        mov              rcx, qword ptr [rbp + -16]
                        push             rbp
                        push             rcx
                        mov              rcx, qword ptr [rbp + -8]
                        push             rcx
                        lea              rax, [rip + PAT$4_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
PAT$4_ω:
                        mov              r12, qword ptr [rbp + -32]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 8
                        ret
#-----------------------------------------------------------------------------------------------------------------------
FN__PAT$5:
PAT$5_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 88
                        mov              rcx, qword ptr [rbp + 8]
                        mov              qword ptr [rbp + -8], rcx
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        mov              qword ptr [rbp + -32], r12
#-----------------------------------------------------------------------------------------------------------------------
n64_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rbp + -48], r14d;         jmp   n65_match_alternate_α
n64_match_assign_save_β:
                        add              rsp, 16;                             jmp   PAT$5_ω
#-----------------------------------------------------------------------------------------------------------------------
n65_match_alternate_α:  mov              dword ptr [rbp + -88], r14d
                        lea              rax, [rip + .Lx72_21]
                        mov              qword ptr [rbp + -72], rax;          jmp   n68_match_defer_α
.Lx72_21:               lea              rax, [rip + .Lx72_19]
                        mov              qword ptr [rbp + -72], rax;          jmp   n67_match_defer_α
n65_match_alternate_s0: lea              rax, [rip + .Lx72_40]
                        mov              qword ptr [rbp + -80], rax;          jmp   n65_match_alternate_as
n65_match_alternate_s1: lea              rax, [rip + .Lx72_41]
                        mov              qword ptr [rbp + -80], rax;          jmp   n65_match_alternate_as
.Lx72_40:                                                                     jmp   n68_match_defer_β
.Lx72_41:                                                                     jmp   n67_match_defer_β
n65_match_alternate_as:                                                       jmp   n66_match_assign_cond_α
n65_match_alternate_β:  mov              rax, qword ptr [rbp + -80];          jmp   rax
n65_match_alternate_af: mov              r14d, dword ptr [rbp + -88]
                        mov              rax, qword ptr [rbp + -72];          jmp   rax
.Lx72_19:                                                                     jmp   n64_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n66_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + -48]
                        lea              rcx, [rip + .S9]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   PAT$5_γ
n66_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n65_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n67_match_defer_α:      mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 1
                        lea              rdx, [rip + .S10]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx75_16
                        mov              rax, qword ptr [rdx + 0]
.Lx75_16:               test             rax, rax;                            jz    .Lx75_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx75_5]
                        push             rcx
                        lea              rcx, [rip + .Lx75_4]
                        push             rcx;                                 jmp   rax
.Lx75_4:                                                                      jmp   n65_match_alternate_s1
.Lx75_5:                                                                      jmp   n65_match_alternate_af
.Lx75_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 1
                        lea              rdx, [rip + .S10]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n65_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx75_6]
                        push             rcx
                        push             rax;                                 jmp   n65_match_alternate_s1
.Lx75_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n65_match_alternate_af
n67_match_defer_β:      cmp              qword ptr [rsp + 0], 0;              jne   .Lx75_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx75_12
                                                                              jmp   rax
.Lx75_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n68_match_defer_α:      mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S11]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx76_16
                        mov              rax, qword ptr [rdx + 0]
.Lx76_16:               test             rax, rax;                            jz    .Lx76_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx76_5]
                        push             rcx
                        lea              rcx, [rip + .Lx76_4]
                        push             rcx;                                 jmp   rax
.Lx76_4:                                                                      jmp   n65_match_alternate_s0
.Lx76_5:                                                                      jmp   n65_match_alternate_af
.Lx76_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S11]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n65_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx76_6]
                        push             rcx
                        push             rax;                                 jmp   n65_match_alternate_s0
.Lx76_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n65_match_alternate_af
n68_match_defer_β:      cmp              qword ptr [rsp + 0], 0;              jne   .Lx76_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx76_12
                                                                              jmp   rax
.Lx76_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
PAT$5_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$5_β:
                                                                              jmp   n66_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$5_γ:
                        mov              rcx, qword ptr [rbp + -16]
                        push             rbp
                        push             rcx
                        mov              rcx, qword ptr [rbp + -8]
                        push             rcx
                        lea              rax, [rip + PAT$5_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
PAT$5_ω:
                        mov              r12, qword ptr [rbp + -32]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 8
                        ret
#-----------------------------------------------------------------------------------------------------------------------
FN__PAT$6:
PAT$6_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 88
                        mov              rcx, qword ptr [rbp + 8]
                        mov              qword ptr [rbp + -8], rcx
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        mov              qword ptr [rbp + -32], r12
#-----------------------------------------------------------------------------------------------------------------------
n77_match_alternate_α:  mov              dword ptr [rbp + -88], r14d
                        lea              rax, [rip + .Lx84_21]
                        mov              qword ptr [rbp + -72], rax;          jmp   n82_match_defer_α
.Lx84_21:               lea              rax, [rip + .Lx84_19]
                        mov              qword ptr [rbp + -72], rax;          jmp   n79_match_lit_α
n77_match_alternate_s0: lea              rax, [rip + .Lx84_40]
                        mov              qword ptr [rbp + -80], rax;          jmp   n77_match_alternate_as
n77_match_alternate_s1: lea              rax, [rip + .Lx84_41]
                        mov              qword ptr [rbp + -80], rax;          jmp   n77_match_alternate_as
.Lx84_40:                                                                     jmp   n82_match_defer_β
.Lx84_41:                                                                     jmp   n78_goto_β
n77_match_alternate_as:                                                       jmp   PAT$6_γ
n77_match_alternate_β:  mov              rax, qword ptr [rbp + -80];          jmp   rax
n77_match_alternate_af: mov              r14d, dword ptr [rbp + -88]
                        mov              rax, qword ptr [rbp + -72];          jmp   rax
.Lx84_19:                                                                     jmp   PAT$6_ω
#-----------------------------------------------------------------------------------------------------------------------
n78_goto_α:                                                                   jmp   n77_match_alternate_af
n78_goto_β:                                                                   jmp   n77_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n79_match_lit_α:        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n77_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 40;                             jne   n77_match_alternate_af
                        add              r14d, 1;                             jmp   n80_match_defer_α
n79_match_lit_β:        sub              r14d, 1;                             jmp   n77_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n80_match_defer_α:      mov              rax, qword ptr [r9 + 336]            # expr
                        mov              rdx, qword ptr [r9 + 344]
                        cmp              al, 8;                               jne   .Lx88_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx88_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, qword ptr [r9 + 344];           jmp   .Lx88_10
.Lx88_9:                cmp              al, 88;                              jne   .Lx88_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx88_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx88_10
.Lx88_21:               xor              eax, eax
.Lx88_10:               test             rax, rax;                            jz    .Lx88_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx88_5]
                        push             rcx
                        lea              rcx, [rip + .Lx88_4]
                        push             rcx;                                 jmp   rax
.Lx88_4:                                                                      jmp   n81_match_lit_α
.Lx88_5:                                                                      jmp   n79_match_lit_β
.Lx88_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S12]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n79_match_lit_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx88_6]
                        push             rcx
                        push             rax;                                 jmp   n81_match_lit_α
.Lx88_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n79_match_lit_β
n80_match_defer_β:      cmp              qword ptr [rsp + 0], 0;              jne   .Lx88_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx88_12
                                                                              jmp   rax
.Lx88_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n81_match_lit_α:        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n80_match_defer_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 41;                             jne   n80_match_defer_β
                        add              r14d, 1;                             jmp   n77_match_alternate_s1
n81_match_lit_β:        sub              r14d, 1;                             jmp   n80_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n82_match_defer_α:      mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S13]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx91_16
                        mov              rax, qword ptr [rdx + 0]
.Lx91_16:               test             rax, rax;                            jz    .Lx91_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx91_5]
                        push             rcx
                        lea              rcx, [rip + .Lx91_4]
                        push             rcx;                                 jmp   rax
.Lx91_4:                                                                      jmp   n77_match_alternate_s0
.Lx91_5:                                                                      jmp   n77_match_alternate_af
.Lx91_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S13]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n77_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx91_6]
                        push             rcx
                        push             rax;                                 jmp   n77_match_alternate_s0
.Lx91_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n77_match_alternate_af
n82_match_defer_β:      cmp              qword ptr [rsp + 0], 0;              jne   .Lx91_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx91_12
                                                                              jmp   rax
.Lx91_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
PAT$6_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$6_β:
                                                                              jmp   n77_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$6_γ:
                        mov              rcx, qword ptr [rbp + -16]
                        push             rbp
                        push             rcx
                        mov              rcx, qword ptr [rbp + -8]
                        push             rcx
                        lea              rax, [rip + PAT$6_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
PAT$6_ω:
                        mov              r12, qword ptr [rbp + -32]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 8
                        ret
#-----------------------------------------------------------------------------------------------------------------------
FN__PAT$7:
PAT$7_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 136
                        mov              rcx, qword ptr [rbp + 8]
                        mov              qword ptr [rbp + -8], rcx
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        mov              qword ptr [rbp + -32], r12
#-----------------------------------------------------------------------------------------------------------------------
n92_match_alternate_α:  mov              dword ptr [rbp + -136], r14d
                        lea              rax, [rip + .Lx100_21]
                        mov              qword ptr [rbp + -120], rax;         jmp   n95_match_defer_α
.Lx100_21:              lea              rax, [rip + .Lx100_19]
                        mov              qword ptr [rbp + -120], rax;         jmp   n93_match_defer_α
n92_match_alternate_s0: lea              rax, [rip + .Lx100_40]
                        mov              qword ptr [rbp + -128], rax;         jmp   n92_match_alternate_as
n92_match_alternate_s1: lea              rax, [rip + .Lx100_41]
                        mov              qword ptr [rbp + -128], rax;         jmp   n92_match_alternate_as
.Lx100_40:                                                                    jmp   n94_goto_β
.Lx100_41:                                                                    jmp   n93_match_defer_β
n92_match_alternate_as:                                                       jmp   PAT$7_γ
n92_match_alternate_β:  mov              rax, qword ptr [rbp + -128];         jmp   rax
n92_match_alternate_af: mov              r14d, dword ptr [rbp + -136]
                        mov              rax, qword ptr [rbp + -120];         jmp   rax
.Lx100_19:                                                                    jmp   PAT$7_ω
#-----------------------------------------------------------------------------------------------------------------------
n93_match_defer_α:      mov              rax, qword ptr [r9 + 288]            # primary
                        mov              rdx, qword ptr [r9 + 296]
                        cmp              al, 8;                               jne   .Lx101_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx101_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, qword ptr [r9 + 296];           jmp   .Lx101_10
.Lx101_9:               cmp              al, 88;                              jne   .Lx101_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx101_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx101_10
.Lx101_21:              xor              eax, eax
.Lx101_10:              test             rax, rax;                            jz    .Lx101_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx101_5]
                        push             rcx
                        lea              rcx, [rip + .Lx101_4]
                        push             rcx;                                 jmp   rax
.Lx101_4:                                                                     jmp   n92_match_alternate_s1
.Lx101_5:                                                                     jmp   n92_match_alternate_af
.Lx101_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S14]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n92_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx101_6]
                        push             rcx
                        push             rax;                                 jmp   n92_match_alternate_s1
.Lx101_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n92_match_alternate_af
n93_match_defer_β:      cmp              qword ptr [rsp + 0], 0;              jne   .Lx101_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx101_12
                                                                              jmp   rax
.Lx101_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n94_goto_α:                                                                   jmp   n92_match_alternate_af
n94_goto_β:                                                                   jmp   n92_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n95_match_defer_α:      mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S15]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx103_16
                        mov              rax, qword ptr [rdx + 0]
.Lx103_16:              test             rax, rax;                            jz    .Lx103_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx103_5]
                        push             rcx
                        lea              rcx, [rip + .Lx103_4]
                        push             rcx;                                 jmp   rax
.Lx103_4:                                                                     jmp   n96_match_assign_save_α
.Lx103_5:                                                                     jmp   n92_match_alternate_af
.Lx103_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S15]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n92_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx103_6]
                        push             rcx
                        push             rax;                                 jmp   n96_match_assign_save_α
.Lx103_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n92_match_alternate_af
n95_match_defer_β:      cmp              qword ptr [rsp + 0], 0;              jne   .Lx103_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx103_12
                                                                              jmp   rax
.Lx103_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n96_match_assign_save_α:
                        mov              dword ptr [rbp + -64], r14d;         jmp   n97_match_defer_α
n96_match_assign_save_β:
                                                                              jmp   n95_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n97_match_defer_α:      mov              rax, qword ptr [r9 + 304]            # factor
                        mov              rdx, qword ptr [r9 + 312]
                        cmp              al, 8;                               jne   .Lx106_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx106_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, qword ptr [r9 + 312];           jmp   .Lx106_10
.Lx106_9:               cmp              al, 88;                              jne   .Lx106_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx106_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx106_10
.Lx106_21:              xor              eax, eax
.Lx106_10:              test             rax, rax;                            jz    .Lx106_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx106_5]
                        push             rcx
                        lea              rcx, [rip + .Lx106_4]
                        push             rcx;                                 jmp   rax
.Lx106_4:                                                                     jmp   n98_match_assign_cond_α
.Lx106_5:                                                                     jmp   n96_match_assign_save_β
.Lx106_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S16]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n96_match_assign_save_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx106_6]
                        push             rcx
                        push             rax;                                 jmp   n98_match_assign_cond_α
.Lx106_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n96_match_assign_save_β
n97_match_defer_β:      cmp              qword ptr [rsp + 0], 0;              jne   .Lx106_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx106_12
                                                                              jmp   rax
.Lx106_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n98_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + -64]
                        lea              rcx, [rip + .S17]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n92_match_alternate_s0
n98_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n97_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$7_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$7_β:
                                                                              jmp   n92_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$7_γ:
                        mov              rcx, qword ptr [rbp + -16]
                        push             rbp
                        push             rcx
                        mov              rcx, qword ptr [rbp + -8]
                        push             rcx
                        lea              rax, [rip + PAT$7_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
PAT$7_ω:
                        mov              r12, qword ptr [rbp + -32]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 8
                        ret
#-----------------------------------------------------------------------------------------------------------------------
FN__PAT$8:
PAT$8_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 136
                        mov              rcx, qword ptr [rbp + 8]
                        mov              qword ptr [rbp + -8], rcx
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        mov              qword ptr [rbp + -32], r12
#-----------------------------------------------------------------------------------------------------------------------
n109_match_alternate_α: mov              dword ptr [rbp + -136], r14d
                        lea              rax, [rip + .Lx118_21]
                        mov              qword ptr [rbp + -120], rax;         jmp   n112_match_defer_α
.Lx118_21:              lea              rax, [rip + .Lx118_19]
                        mov              qword ptr [rbp + -120], rax;         jmp   n110_match_defer_α
n109_match_alternate_s0:
                        lea              rax, [rip + .Lx118_40]
                        mov              qword ptr [rbp + -128], rax;         jmp   n109_match_alternate_as
n109_match_alternate_s1:
                        lea              rax, [rip + .Lx118_41]
                        mov              qword ptr [rbp + -128], rax;         jmp   n109_match_alternate_as
.Lx118_40:                                                                    jmp   n111_goto_β
.Lx118_41:                                                                    jmp   n110_match_defer_β
n109_match_alternate_as:
                                                                              jmp   PAT$8_γ
n109_match_alternate_β: mov              rax, qword ptr [rbp + -128];         jmp   rax
n109_match_alternate_af:
                        mov              r14d, dword ptr [rbp + -136]
                        mov              rax, qword ptr [rbp + -120];         jmp   rax
.Lx118_19:                                                                    jmp   PAT$8_ω
#-----------------------------------------------------------------------------------------------------------------------
n110_match_defer_α:     mov              rax, qword ptr [r9 + 304]            # factor
                        mov              rdx, qword ptr [r9 + 312]
                        cmp              al, 8;                               jne   .Lx119_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx119_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, qword ptr [r9 + 312];           jmp   .Lx119_10
.Lx119_9:               cmp              al, 88;                              jne   .Lx119_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx119_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx119_10
.Lx119_21:              xor              eax, eax
.Lx119_10:              test             rax, rax;                            jz    .Lx119_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx119_5]
                        push             rcx
                        lea              rcx, [rip + .Lx119_4]
                        push             rcx;                                 jmp   rax
.Lx119_4:                                                                     jmp   n109_match_alternate_s1
.Lx119_5:                                                                     jmp   n109_match_alternate_af
.Lx119_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S16]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n109_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx119_6]
                        push             rcx
                        push             rax;                                 jmp   n109_match_alternate_s1
.Lx119_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n109_match_alternate_af
n110_match_defer_β:     cmp              qword ptr [rsp + 0], 0;              jne   .Lx119_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx119_12
                                                                              jmp   rax
.Lx119_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n111_goto_α:                                                                  jmp   n109_match_alternate_af
n111_goto_β:                                                                  jmp   n109_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n112_match_defer_α:     mov              rax, qword ptr [r9 + 304]            # factor
                        mov              rdx, qword ptr [r9 + 312]
                        cmp              al, 8;                               jne   .Lx121_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx121_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, qword ptr [r9 + 312];           jmp   .Lx121_10
.Lx121_9:               cmp              al, 88;                              jne   .Lx121_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx121_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx121_10
.Lx121_21:              xor              eax, eax
.Lx121_10:              test             rax, rax;                            jz    .Lx121_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx121_5]
                        push             rcx
                        lea              rcx, [rip + .Lx121_4]
                        push             rcx;                                 jmp   rax
.Lx121_4:                                                                     jmp   n113_match_defer_α
.Lx121_5:                                                                     jmp   n109_match_alternate_af
.Lx121_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S16]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n109_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx121_6]
                        push             rcx
                        push             rax;                                 jmp   n113_match_defer_α
.Lx121_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n109_match_alternate_af
n112_match_defer_β:     cmp              qword ptr [rsp + 0], 0;              jne   .Lx121_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx121_12
                                                                              jmp   rax
.Lx121_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n113_match_defer_α:     mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S18]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx122_16
                        mov              rax, qword ptr [rdx + 0]
.Lx122_16:              test             rax, rax;                            jz    .Lx122_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx122_5]
                        push             rcx
                        lea              rcx, [rip + .Lx122_4]
                        push             rcx;                                 jmp   rax
.Lx122_4:                                                                     jmp   n114_match_assign_save_α
.Lx122_5:                                                                     jmp   n112_match_defer_β
.Lx122_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S18]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n112_match_defer_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx122_6]
                        push             rcx
                        push             rax;                                 jmp   n114_match_assign_save_α
.Lx122_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n112_match_defer_β
n113_match_defer_β:     cmp              qword ptr [rsp + 0], 0;              jne   .Lx122_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx122_12
                                                                              jmp   rax
.Lx122_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n114_match_assign_save_α:
                        mov              dword ptr [rbp + -64], r14d;         jmp   n115_match_defer_α
n114_match_assign_save_β:
                                                                              jmp   n113_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n115_match_defer_α:     mov              rax, qword ptr [r9 + 320]            # term
                        mov              rdx, qword ptr [r9 + 328]
                        cmp              al, 8;                               jne   .Lx125_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx125_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, qword ptr [r9 + 328];           jmp   .Lx125_10
.Lx125_9:               cmp              al, 88;                              jne   .Lx125_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx125_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx125_10
.Lx125_21:              xor              eax, eax
.Lx125_10:              test             rax, rax;                            jz    .Lx125_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx125_5]
                        push             rcx
                        lea              rcx, [rip + .Lx125_4]
                        push             rcx;                                 jmp   rax
.Lx125_4:                                                                     jmp   n116_match_assign_cond_α
.Lx125_5:                                                                     jmp   n114_match_assign_save_β
.Lx125_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S19]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n114_match_assign_save_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx125_6]
                        push             rcx
                        push             rax;                                 jmp   n116_match_assign_cond_α
.Lx125_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n114_match_assign_save_β
n115_match_defer_β:     cmp              qword ptr [rsp + 0], 0;              jne   .Lx125_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx125_12
                                                                              jmp   rax
.Lx125_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n116_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + -64]
                        lea              rcx, [rip + .S20]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n109_match_alternate_s0
n116_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n115_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$8_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$8_β:
                                                                              jmp   n109_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$8_γ:
                        mov              rcx, qword ptr [rbp + -16]
                        push             rbp
                        push             rcx
                        mov              rcx, qword ptr [rbp + -8]
                        push             rcx
                        lea              rax, [rip + PAT$8_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
PAT$8_ω:
                        mov              r12, qword ptr [rbp + -32]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 8
                        ret
#-----------------------------------------------------------------------------------------------------------------------
FN__PAT$9:
PAT$9_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 136
                        mov              rcx, qword ptr [rbp + 8]
                        mov              qword ptr [rbp + -8], rcx
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        mov              qword ptr [rbp + -32], r12
#-----------------------------------------------------------------------------------------------------------------------
n128_match_alternate_α: mov              dword ptr [rbp + -136], r14d
                        lea              rax, [rip + .Lx137_21]
                        mov              qword ptr [rbp + -120], rax;         jmp   n131_match_defer_α
.Lx137_21:              lea              rax, [rip + .Lx137_19]
                        mov              qword ptr [rbp + -120], rax;         jmp   n129_match_defer_α
n128_match_alternate_s0:
                        lea              rax, [rip + .Lx137_40]
                        mov              qword ptr [rbp + -128], rax;         jmp   n128_match_alternate_as
n128_match_alternate_s1:
                        lea              rax, [rip + .Lx137_41]
                        mov              qword ptr [rbp + -128], rax;         jmp   n128_match_alternate_as
.Lx137_40:                                                                    jmp   n130_goto_β
.Lx137_41:                                                                    jmp   n129_match_defer_β
n128_match_alternate_as:
                                                                              jmp   PAT$9_γ
n128_match_alternate_β: mov              rax, qword ptr [rbp + -128];         jmp   rax
n128_match_alternate_af:
                        mov              r14d, dword ptr [rbp + -136]
                        mov              rax, qword ptr [rbp + -120];         jmp   rax
.Lx137_19:                                                                    jmp   PAT$9_ω
#-----------------------------------------------------------------------------------------------------------------------
n129_match_defer_α:     mov              rax, qword ptr [r9 + 320]            # term
                        mov              rdx, qword ptr [r9 + 328]
                        cmp              al, 8;                               jne   .Lx138_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx138_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, qword ptr [r9 + 328];           jmp   .Lx138_10
.Lx138_9:               cmp              al, 88;                              jne   .Lx138_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx138_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx138_10
.Lx138_21:              xor              eax, eax
.Lx138_10:              test             rax, rax;                            jz    .Lx138_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx138_5]
                        push             rcx
                        lea              rcx, [rip + .Lx138_4]
                        push             rcx;                                 jmp   rax
.Lx138_4:                                                                     jmp   n128_match_alternate_s1
.Lx138_5:                                                                     jmp   n128_match_alternate_af
.Lx138_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S19]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n128_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx138_6]
                        push             rcx
                        push             rax;                                 jmp   n128_match_alternate_s1
.Lx138_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n128_match_alternate_af
n129_match_defer_β:     cmp              qword ptr [rsp + 0], 0;              jne   .Lx138_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx138_12
                                                                              jmp   rax
.Lx138_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n130_goto_α:                                                                  jmp   n128_match_alternate_af
n130_goto_β:                                                                  jmp   n128_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n131_match_defer_α:     mov              rax, qword ptr [r9 + 320]            # term
                        mov              rdx, qword ptr [r9 + 328]
                        cmp              al, 8;                               jne   .Lx140_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx140_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, qword ptr [r9 + 328];           jmp   .Lx140_10
.Lx140_9:               cmp              al, 88;                              jne   .Lx140_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx140_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx140_10
.Lx140_21:              xor              eax, eax
.Lx140_10:              test             rax, rax;                            jz    .Lx140_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx140_5]
                        push             rcx
                        lea              rcx, [rip + .Lx140_4]
                        push             rcx;                                 jmp   rax
.Lx140_4:                                                                     jmp   n132_match_defer_α
.Lx140_5:                                                                     jmp   n128_match_alternate_af
.Lx140_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S19]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n128_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx140_6]
                        push             rcx
                        push             rax;                                 jmp   n132_match_defer_α
.Lx140_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n128_match_alternate_af
n131_match_defer_β:     cmp              qword ptr [rsp + 0], 0;              jne   .Lx140_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx140_12
                                                                              jmp   rax
.Lx140_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n132_match_defer_α:     mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S21]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx141_16
                        mov              rax, qword ptr [rdx + 0]
.Lx141_16:              test             rax, rax;                            jz    .Lx141_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx141_5]
                        push             rcx
                        lea              rcx, [rip + .Lx141_4]
                        push             rcx;                                 jmp   rax
.Lx141_4:                                                                     jmp   n133_match_assign_save_α
.Lx141_5:                                                                     jmp   n131_match_defer_β
.Lx141_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S21]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n131_match_defer_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx141_6]
                        push             rcx
                        push             rax;                                 jmp   n133_match_assign_save_α
.Lx141_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n131_match_defer_β
n132_match_defer_β:     cmp              qword ptr [rsp + 0], 0;              jne   .Lx141_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx141_12
                                                                              jmp   rax
.Lx141_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n133_match_assign_save_α:
                        mov              dword ptr [rbp + -64], r14d;         jmp   n134_match_defer_α
n133_match_assign_save_β:
                                                                              jmp   n132_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n134_match_defer_α:     mov              rax, qword ptr [r9 + 336]            # expr
                        mov              rdx, qword ptr [r9 + 344]
                        cmp              al, 8;                               jne   .Lx144_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx144_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, qword ptr [r9 + 344];           jmp   .Lx144_10
.Lx144_9:               cmp              al, 88;                              jne   .Lx144_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx144_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx144_10
.Lx144_21:              xor              eax, eax
.Lx144_10:              test             rax, rax;                            jz    .Lx144_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx144_5]
                        push             rcx
                        lea              rcx, [rip + .Lx144_4]
                        push             rcx;                                 jmp   rax
.Lx144_4:                                                                     jmp   n135_match_assign_cond_α
.Lx144_5:                                                                     jmp   n133_match_assign_save_β
.Lx144_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S12]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n133_match_assign_save_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx144_6]
                        push             rcx
                        push             rax;                                 jmp   n135_match_assign_cond_α
.Lx144_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n133_match_assign_save_β
n134_match_defer_β:     cmp              qword ptr [rsp + 0], 0;              jne   .Lx144_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx144_12
                                                                              jmp   rax
.Lx144_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n135_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + -64]
                        lea              rcx, [rip + .S20]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n128_match_alternate_s0
n135_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n134_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$9_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$9_β:
                                                                              jmp   n128_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$9_γ:
                        mov              rcx, qword ptr [rbp + -16]
                        push             rbp
                        push             rcx
                        mov              rcx, qword ptr [rbp + -8]
                        push             rcx
                        lea              rax, [rip + PAT$9_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
PAT$9_ω:
                        mov              r12, qword ptr [rbp + -32]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 8
                        ret
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             module_init
                        mov              edi, 38
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 38
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
                        .section         .rodata
.Lgvan0:                .string          "Push"
.Lgvan1:                .string          "x"
.Lgvan2:                .string          "Pop"
.Lgvan3:                .string          "Unary"
.Lgvan4:                .string          "arg"
.Lgvan5:                .string          "op"
.Lgvan6:                .string          "Binary"
.Lgvan7:                .string          "left"
.Lgvan8:                .string          "right"
.Lgvan9:                .string          "stk"
.Lgvan10:               .string          "dummy"
.Lgvan11:               .string          "integer"
.Lgvan12:               .string          "exponent"
.Lgvan13:               .string          "epsilon"
.Lgvan14:               .string          "real"
.Lgvan15:               .string          "addop"
.Lgvan16:               .string          "mulop"
.Lgvan17:               .string          "constant"
.Lgvan18:               .string          "primary"
.Lgvan19:               .string          "factor"
.Lgvan20:               .string          "term"
.Lgvan21:               .string          "expr"
.Lgvan22:               .string          "line"
.Lgvan23:               .string          "PAT$1$V0"
.Lgvan24:               .string          "PAT$1$V1"
.Lgvan25:               .string          "PAT$2$V0"
.Lgvan26:               .string          "PAT$2$V1"
.Lgvan27:               .string          "PAT$2$V2"
.Lgvan28:               .string          "PAT$2$V3"
.Lgvan29:               .string          "PAT$2$V4"
.Lgvan30:               .string          "PAT$2$V5"
.Lgvan31:               .string          "PAT$2$V6"
.Lgvan32:               .string          "PAT$5$V0"
.Lgvan33:               .string          "PAT$5$V1"
.Lgvan34:               .string          "PAT$6$V0"
.Lgvan35:               .string          "PAT$7$V0"
.Lgvan36:               .string          "PAT$8$V0"
.Lgvan37:               .string          "PAT$9$V0"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .quad            .Lgvan4
                        .quad            .Lgvan5
                        .quad            .Lgvan6
                        .quad            .Lgvan7
                        .quad            .Lgvan8
                        .quad            .Lgvan9
                        .quad            .Lgvan10
                        .quad            .Lgvan11
                        .quad            .Lgvan12
                        .quad            .Lgvan13
                        .quad            .Lgvan14
                        .quad            .Lgvan15
                        .quad            .Lgvan16
                        .quad            .Lgvan17
                        .quad            .Lgvan18
                        .quad            .Lgvan19
                        .quad            .Lgvan20
                        .quad            .Lgvan21
                        .quad            .Lgvan22
                        .quad            .Lgvan23
                        .quad            .Lgvan24
                        .quad            .Lgvan25
                        .quad            .Lgvan26
                        .quad            .Lgvan27
                        .quad            .Lgvan28
                        .quad            .Lgvan29
                        .quad            .Lgvan30
                        .quad            .Lgvan31
                        .quad            .Lgvan32
                        .quad            .Lgvan33
                        .quad            .Lgvan34
                        .quad            .Lgvan35
                        .quad            .Lgvan36
                        .quad            .Lgvan37
                        .section         .text
                        .intel_syntax    noprefix
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        sub              rsp, 0
#-----------------------------------------------------------------------------------------------------------------------
n147_statement_begin_α:                                                       jmp   n148_statement_end_α
n147_statement_begin_β:                                                       jmp   n149_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n148_statement_end_α:                                                         jmp   n149_statement_begin_α
#=======================================================================================================================
#          DEFINE('Push(x)')
#-----------------------------------------------------------------------------------------------------------------------
n149_statement_begin_α:                                                       jmp   n150_define_α
n149_statement_begin_β:                                                       jmp   n152_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n150_define_α:          mov              rdi, qword ptr [rip + .Lx413_0]
                        mov              rsi, qword ptr [rip + .Lx413_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n156_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx413_0]
                        lea              rsi, [rip + Push_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n151_statement_end_α
n150_define_β:                                                                jmp   n149_statement_begin_β
.Lx413_0:               .quad            .Lx413_0_s
.Lx413_0_s:             .string          "Push"
.Lx413_1:               .quad            .Lx413_1_s
.Lx413_1_s:             .string          "x"
                                                                              jmp   .Lx414_245
#-----------------------------------------------------------------------------------------------------------------------
Push_α:                 sub              rsp, 64
                        mov              rax, qword ptr [r9 + 0]              # Push
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx414_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # x
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx414_41
.Lx414_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx414_41:              lea              rcx, [rip + Push_γ]
                        lea              r11, [rip + Push_ω]
                        push             r11
                        push             rcx
                        lea              rax, [rip + n156_statement_begin_α]; jmp   rax
Push_γ:                 mov              rdi, qword ptr [r9 + 0]              # Push
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rdx, 0;                              jbe   .Lx414_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # x
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx414_110
.Lx414_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx414_110:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 64
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
Push_ω:                 mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax              # Push
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rdx, 0;                              jbe   .Lx414_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # x
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx414_180
.Lx414_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx414_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx414_245:
#-----------------------------------------------------------------------------------------------------------------------
n151_statement_end_α:                                                         jmp   n152_statement_begin_α
#=======================================================================================================================
#          stk      =  TABLE()                       :(PushEnd)
#-----------------------------------------------------------------------------------------------------------------------
n152_statement_begin_α:                                                       jmp   n153_call_α
n152_statement_begin_β:                                                       jmp   n183_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n153_call_α:            sub              rsp, 16
                        .section         .rodata
.Lrkfnzd420:            .string          "TABLE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd420]
                        xor              esi, esi
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx419_240
                        add              rsp, 16;                             jmp   n152_statement_begin_β
.Lx419_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n154_assign_α
n153_call_β:            add              rsp, 16;                             jmp   n152_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n154_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # stk
                        mov              qword ptr [r9 + 152], rdx;           jmp   n155_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n155_statement_end_α:   add              rsp, 16;                             jmp   n183_statement_begin_α
#=======================================================================================================================
# Push     stk[0]   =  stk[0] + 1
#-----------------------------------------------------------------------------------------------------------------------
n156_statement_begin_α:                                                       jmp   n157_var_α
n156_statement_begin_β:                                                       jmp   n168_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n157_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n158_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n158_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx427_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n159_subscript_α
n158_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n156_statement_begin_β
.Lx427_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n159_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx428_240
                        add              rsp, 16;                             jmp   n158_lit_integer_β
.Lx428_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n160_var_α
n159_subscript_β:       add              rsp, 16;                             jmp   n158_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n160_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n161_lit_integer_α
n160_var_β:             add              rsp, 16;                             jmp   n159_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n161_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx430_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n162_subscript_α
n161_lit_integer_β:     add              rsp, 16;                             jmp   n160_var_β
.Lx430_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n162_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx431_240
                        add              rsp, 16;                             jmp   n161_lit_integer_β
.Lx431_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n163_deref_α
n162_subscript_β:       add              rsp, 16;                             jmp   n161_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n163_deref_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx432_240
                        add              rsp, 16;                             jmp   n162_subscript_β
.Lx432_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n164_lit_integer_α
n163_deref_β:           add              rsp, 16;                             jmp   n162_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n164_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx433_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n165_binop_α
n164_lit_integer_β:     add              rsp, 16;                             jmp   n163_deref_β
.Lx433_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n165_binop_α:           sub              rsp, 16
                        mov              ecx, dword ptr [rsp + 32]            # deref
                        mov              rax, qword ptr [rsp + 40]
                        cmp              cl, 3;                               jne   .Lx434_2
                        add              rax, 1
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx434_7
.Lx434_2:               mov              eax, ecx
                        mov              edx, ecx
                        and              edx, 1;                              jz    .Lx434_0
                        mov              rsi, qword ptr [rsp + 40]            # deref
                        mov              rdi, 1
                        cmp              al, 5;                               je    .Lx434_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx434_4
.Lx434_3:               movq             xmm0, rsi
.Lx434_4:               cvtsi2sd         xmm1, rdi
                        addsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx434_7:                                                                     jmp   n166_assign_var_α
.Lx434_0:               mov              rdi, qword ptr [rsp + 32]            # deref
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_add@PLT
                        cmp              al, 104;                             jne   .Lx434_240
                        add              rsp, 16;                             jmp   n164_lit_integer_β
.Lx434_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n166_assign_var_α
n165_binop_β:           add              rsp, 16;                             jmp   n164_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n166_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]           # subscript
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 16]            # binop
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx435_240
                        add              rsp, 16;                             jmp   n165_binop_β
.Lx435_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n167_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n167_statement_end_α:   add              rsp, 160;                            jmp   n168_statement_begin_α
#=======================================================================================================================
#          Push     =  .stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n168_statement_begin_α:                                                       jmp   n169_var_α
n168_statement_begin_β:                                                       jmp   n177_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n169_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n170_var_α
#-----------------------------------------------------------------------------------------------------------------------
n170_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n171_lit_integer_α
n170_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n168_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n171_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx442_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n172_subscript_α
n171_lit_integer_β:     add              rsp, 16;                             jmp   n170_var_β
.Lx442_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n172_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx443_240
                        add              rsp, 16;                             jmp   n171_lit_integer_β
.Lx443_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n173_deref_α
n172_subscript_β:       add              rsp, 16;                             jmp   n171_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n173_deref_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx444_240
                        add              rsp, 16;                             jmp   n172_subscript_β
.Lx444_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n174_subscript_α
n173_deref_β:           add              rsp, 16;                             jmp   n172_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n174_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]            # var
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]            # deref
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx445_240
                        add              rsp, 16;                             jmp   n173_deref_β
.Lx445_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n175_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n175_assign_α:          mov              rax, qword ptr [rsp + 0]             # subscript
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # Push
                        mov              qword ptr [r9 + 8], rdx;             jmp   n176_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n176_statement_end_α:   add              rsp, 96;                             jmp   n177_statement_begin_α
#=======================================================================================================================
#          $Push    =  x                             :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n177_statement_begin_α:                                                       jmp   n178_var_α
n177_statement_begin_β:                                                       jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n178_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]              # Push
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n179_call_α
#-----------------------------------------------------------------------------------------------------------------------
n179_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd453:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd453]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx452_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n177_statement_begin_β
.Lx452_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n180_var_α
n179_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n177_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n180_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n181_assign_var_α
n180_var_β:             add              rsp, 16
                        add              rsp, 32;                             jmp   n177_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n181_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx455_240
                        add              rsp, 16;                             jmp   n180_var_β
.Lx455_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n182_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n182_statement_end_α:   add              rsp, 64;                             jmp   NRETURN
#=======================================================================================================================
# PushEnd  <stmt 7, line 14: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n183_statement_begin_α:                                                       jmp   n184_statement_end_α
n183_statement_begin_β:                                                       jmp   n185_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n184_statement_end_α:                                                         jmp   n185_statement_begin_α
#=======================================================================================================================
#          DEFINE('Pop()')                           :(PopEnd)
#-----------------------------------------------------------------------------------------------------------------------
n185_statement_begin_α:                                                       jmp   n186_define_α
n185_statement_begin_β:                                                       jmp   n210_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n186_define_α:          mov              rdi, qword ptr [rip + .Lx465_0]
                        mov              rsi, qword ptr [rip + .Lx465_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n188_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx465_0]
                        lea              rsi, [rip + Pop_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n187_statement_end_α
n186_define_β:                                                                jmp   n185_statement_begin_β
.Lx465_0:               .quad            .Lx465_0_s
.Lx465_0_s:             .string          "Pop"
.Lx465_1:               .quad            .Lx465_1_s
.Lx465_1_s:             .string          ""
                                                                              jmp   .Lx466_245
#-----------------------------------------------------------------------------------------------------------------------
Pop_α:                  sub              rsp, 48
                        mov              rax, qword ptr [r9 + 32]             # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        lea              rcx, [rip + Pop_γ]
                        lea              r11, [rip + Pop_ω]
                        push             r11
                        push             rcx
                        lea              rax, [rip + n188_statement_begin_α]; jmp   rax
Pop_γ:                  mov              rdi, qword ptr [r9 + 32]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
Pop_ω:                  mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx466_245:
#-----------------------------------------------------------------------------------------------------------------------
n187_statement_end_α:                                                         jmp   n210_statement_begin_α
#=======================================================================================================================
# Pop      Pop      =  stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n188_statement_begin_α:                                                       jmp   n189_var_α
n188_statement_begin_β:                                                       jmp   n198_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n189_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n190_var_α
#-----------------------------------------------------------------------------------------------------------------------
n190_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n191_lit_integer_α
n190_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n188_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n191_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx473_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n192_subscript_α
n191_lit_integer_β:     add              rsp, 16;                             jmp   n190_var_β
.Lx473_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n192_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx474_240
                        add              rsp, 16;                             jmp   n191_lit_integer_β
.Lx474_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n193_deref_α
n192_subscript_β:       add              rsp, 16;                             jmp   n191_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n193_deref_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx475_240
                        add              rsp, 16;                             jmp   n192_subscript_β
.Lx475_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n194_subscript_α
n193_deref_β:           add              rsp, 16;                             jmp   n192_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n194_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]            # var
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]            # deref
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx476_240
                        add              rsp, 16;                             jmp   n193_deref_β
.Lx476_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n195_deref_α
n194_subscript_β:       add              rsp, 16;                             jmp   n193_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n195_deref_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx477_240
                        add              rsp, 16;                             jmp   n194_subscript_β
.Lx477_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n196_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n196_assign_α:          mov              rax, qword ptr [rsp + 0]             # deref
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax             # Pop
                        mov              qword ptr [r9 + 40], rdx;            jmp   n197_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n197_statement_end_α:   add              rsp, 112;                            jmp   n198_statement_begin_α
#=======================================================================================================================
#          stk[0]   =  stk[0] - 1                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n198_statement_begin_α:                                                       jmp   n199_var_α
n198_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n199_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n200_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n200_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx484_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n201_subscript_α
n200_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n198_statement_begin_β
.Lx484_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n201_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx485_240
                        add              rsp, 16;                             jmp   n200_lit_integer_β
.Lx485_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n202_var_α
n201_subscript_β:       add              rsp, 16;                             jmp   n200_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n202_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n203_lit_integer_α
n202_var_β:             add              rsp, 16;                             jmp   n201_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n203_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx487_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n204_subscript_α
n203_lit_integer_β:     add              rsp, 16;                             jmp   n202_var_β
.Lx487_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n204_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx488_240
                        add              rsp, 16;                             jmp   n203_lit_integer_β
.Lx488_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n205_deref_α
n204_subscript_β:       add              rsp, 16;                             jmp   n203_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n205_deref_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx489_240
                        add              rsp, 16;                             jmp   n204_subscript_β
.Lx489_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n206_lit_integer_α
n205_deref_β:           add              rsp, 16;                             jmp   n204_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n206_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx490_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n207_binop_α
n206_lit_integer_β:     add              rsp, 16;                             jmp   n205_deref_β
.Lx490_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n207_binop_α:           sub              rsp, 16
                        mov              ecx, dword ptr [rsp + 32]            # deref
                        mov              rax, qword ptr [rsp + 40]
                        cmp              cl, 3;                               jne   .Lx491_2
                        sub              rax, 1
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx491_7
.Lx491_2:               mov              eax, ecx
                        mov              edx, ecx
                        and              edx, 1;                              jz    .Lx491_0
                        mov              rsi, qword ptr [rsp + 40]            # deref
                        mov              rdi, 1
                        cmp              al, 5;                               je    .Lx491_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx491_4
.Lx491_3:               movq             xmm0, rsi
.Lx491_4:               cvtsi2sd         xmm1, rdi
                        subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx491_7:                                                                     jmp   n208_assign_var_α
.Lx491_0:               mov              rdi, qword ptr [rsp + 32]            # deref
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_sub@PLT
                        cmp              al, 104;                             jne   .Lx491_240
                        add              rsp, 16;                             jmp   n206_lit_integer_β
.Lx491_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n208_assign_var_α
n207_binop_β:           add              rsp, 16;                             jmp   n206_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n208_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]           # subscript
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 16]            # binop
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx492_240
                        add              rsp, 16;                             jmp   n207_binop_β
.Lx492_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n209_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n209_statement_end_α:   add              rsp, 160;                            jmp   RETURN
#=======================================================================================================================
# PopEnd  <stmt 11, line 18: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n210_statement_begin_α:                                                       jmp   n211_statement_end_α
n210_statement_begin_β:                                                       jmp   n212_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n211_statement_end_α:                                                         jmp   n212_statement_begin_α
#=======================================================================================================================
#          DEFINE('Unary()arg,op')                   :(UnaryEnd)
#-----------------------------------------------------------------------------------------------------------------------
n212_statement_begin_α:                                                       jmp   n213_define_α
n212_statement_begin_β:                                                       jmp   n238_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n213_define_α:          mov              rdi, qword ptr [rip + .Lx502_0]
                        mov              rsi, qword ptr [rip + .Lx502_1]
                        mov              edx, 2
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n215_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx502_0]
                        lea              rsi, [rip + Unary_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n214_statement_end_α
n213_define_β:                                                                jmp   n212_statement_begin_β
.Lx502_0:               .quad            .Lx502_0_s
.Lx502_0_s:             .string          "Unary"
.Lx502_1:               .quad            .Lx502_1_s
.Lx502_1_s:             .string          "arg,op"
                                                                              jmp   .Lx503_245
#-----------------------------------------------------------------------------------------------------------------------
Unary_α:                sub              rsp, 80
                        mov              rax, qword ptr [r9 + 48]             # Unary
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        cmp              rdx, 0;                              jbe   .Lx503_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 64]             # arg
                        mov              qword ptr [r9 + 64], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 72]
                        mov              qword ptr [r9 + 72], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx503_41
.Lx503_10:              mov              rax, qword ptr [r9 + 64]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
.Lx503_41:              cmp              rdx, 1;                              jbe   .Lx503_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 80]             # op
                        mov              qword ptr [r9 + 80], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 88]
                        mov              qword ptr [r9 + 88], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx503_42
.Lx503_11:              mov              rax, qword ptr [r9 + 80]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
.Lx503_42:              lea              rcx, [rip + Unary_γ]
                        lea              r11, [rip + Unary_ω]
                        push             r11
                        push             rcx
                        lea              rax, [rip + n215_statement_begin_α]; jmp   rax
Unary_γ:                mov              rdi, qword ptr [r9 + 48]             # Unary
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 56], rax
                        cmp              rdx, 0;                              jbe   .Lx503_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 64], rax             # arg
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx503_110
.Lx503_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 72], rax
.Lx503_110:             cmp              rdx, 1;                              jbe   .Lx503_81
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 88], rax;            jmp   .Lx503_111
.Lx503_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 88], rax
.Lx503_111:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 80
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
Unary_ω:                mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 48], rax             # Unary
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 56], rax
                        cmp              rdx, 0;                              jbe   .Lx503_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 64], rax             # arg
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx503_180
.Lx503_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 72], rax
.Lx503_180:             cmp              rdx, 1;                              jbe   .Lx503_151
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 88], rax;            jmp   .Lx503_181
.Lx503_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 88], rax
.Lx503_181:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 80
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx503_245:
#-----------------------------------------------------------------------------------------------------------------------
n214_statement_end_α:                                                         jmp   n238_statement_begin_α
#=======================================================================================================================
# Unary    arg      =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n215_statement_begin_α:                                                       jmp   n216_call_α
n215_statement_begin_β:                                                       jmp   n219_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n216_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig509z]
                        lea              rax, [rip + Pop_α];                  jmp   rax
.Lsig509z:              .quad            0
                        .quad            .Lx509_2
                        .quad            .Lx509_2
.Lx509_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx509_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx509_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx509_240
                        add              rsp, 16;                             jmp   n215_statement_begin_β
.Lx509_240:                                                                   jmp   n217_assign_α
n216_call_β:                                                                  jmp   n215_statement_begin_β
.Lx509_0:               .quad            .Lx509_0_s
.Lx509_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n217_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax             # arg
                        mov              qword ptr [r9 + 72], rdx;            jmp   n218_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n218_statement_end_α:   add              rsp, 16;                             jmp   n219_statement_begin_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n219_statement_begin_α:                                                       jmp   n220_call_α
n219_statement_begin_β:                                                       jmp   n223_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n220_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig516z]
                        lea              rax, [rip + Pop_α];                  jmp   rax
.Lsig516z:              .quad            0
                        .quad            .Lx516_2
                        .quad            .Lx516_2
.Lx516_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx516_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx516_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx516_240
                        add              rsp, 16;                             jmp   n219_statement_begin_β
.Lx516_240:                                                                   jmp   n221_assign_α
n220_call_β:                                                                  jmp   n219_statement_begin_β
.Lx516_0:               .quad            .Lx516_0_s
.Lx516_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n221_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              qword ptr [r9 + 88], rdx;            jmp   n222_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n222_statement_end_α:   add              rsp, 16;                             jmp   n223_statement_begin_α
#=======================================================================================================================
#          Push()   =  EVAL(op arg)
#-----------------------------------------------------------------------------------------------------------------------
n223_statement_begin_α:                                                       jmp   n224_lit_string_α
n223_statement_begin_β:                                                       jmp   n233_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n224_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx522_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n225_call_α
.Lx522_0:               .quad            .Lx522_0_s
.Lx522_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n225_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd524:            .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd524]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx523_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n223_statement_begin_β
.Lx523_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n226_call_α
n225_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n223_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n226_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig526z]
                        lea              rax, [rip + Push_α];                 jmp   rax
.Lsig526z:              .quad            0
                        .quad            .Lx526_2
                        .quad            .Lx526_2
.Lx526_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx526_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx526_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx526_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n223_statement_begin_β
.Lx526_240:                                                                   jmp   n227_var_α
n226_call_β:                                                                  jmp   n223_statement_begin_β
.Lx526_0:               .quad            .Lx526_0_s
.Lx526_0_s:             .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n227_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 80]             # op
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n228_var_α
n227_var_β:             add              rsp, 16
                        add              rsp, 48;                             jmp   n223_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n228_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # arg
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n229_binop_α
n228_var_β:             add              rsp, 16;                             jmp   n227_var_β
#-----------------------------------------------------------------------------------------------------------------------
n229_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n230_call_α
n229_binop_β:           add              rsp, 16;                             jmp   n228_var_β
#-----------------------------------------------------------------------------------------------------------------------
n230_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd531:            .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd531]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx530_240
                        add              rsp, 16;                             jmp   n229_binop_β
.Lx530_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n231_assign_var_α
n230_call_β:            add              rsp, 16;                             jmp   n229_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n231_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]            # call
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx532_240
                        add              rsp, 32;                             jmp   n229_binop_β
.Lx532_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n232_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n232_statement_end_α:   add              rsp, 128;                            jmp   n233_statement_begin_α
#=======================================================================================================================
#          Unary    =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n233_statement_begin_α:                                                       jmp   n234_lit_string_α
n233_statement_begin_β:                                                       jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n234_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx537_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n235_call_α
.Lx537_0:               .quad            .Lx537_0_s
.Lx537_0_s:             .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n235_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd539:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd539]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx538_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n233_statement_begin_β
.Lx538_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n236_assign_α
n235_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n233_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n236_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # Unary
                        mov              qword ptr [r9 + 56], rdx;            jmp   n237_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n237_statement_end_α:   add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# UnaryEnd  <stmt 17, line 24: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n238_statement_begin_α:                                                       jmp   n239_statement_end_α
n238_statement_begin_β:                                                       jmp   n240_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n239_statement_end_α:                                                         jmp   n240_statement_begin_α
#=======================================================================================================================
#          DEFINE('Binary()op,left,right')           :(BinaryEnd)
#-----------------------------------------------------------------------------------------------------------------------
n240_statement_begin_α:                                                       jmp   n241_define_α
n240_statement_begin_β:                                                       jmp   n276_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n241_define_α:          mov              rdi, qword ptr [rip + .Lx550_0]
                        mov              rsi, qword ptr [rip + .Lx550_1]
                        mov              edx, 3
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n243_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx550_0]
                        lea              rsi, [rip + Binary_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n242_statement_end_α
n241_define_β:                                                                jmp   n240_statement_begin_β
.Lx550_0:               .quad            .Lx550_0_s
.Lx550_0_s:             .string          "Binary"
.Lx550_1:               .quad            .Lx550_1_s
.Lx550_1_s:             .string          "op,left,right"
                                                                              jmp   .Lx551_245
#-----------------------------------------------------------------------------------------------------------------------
Binary_α:               sub              rsp, 96
                        mov              rax, qword ptr [r9 + 96]             # Binary
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        cmp              rdx, 0;                              jbe   .Lx551_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 80]             # op
                        mov              qword ptr [r9 + 80], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 88]
                        mov              qword ptr [r9 + 88], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx551_41
.Lx551_10:              mov              rax, qword ptr [r9 + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
.Lx551_41:              cmp              rdx, 1;                              jbe   .Lx551_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 112]            # left
                        mov              qword ptr [r9 + 112], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 120]
                        mov              qword ptr [r9 + 120], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx551_42
.Lx551_11:              mov              rax, qword ptr [r9 + 112]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 112], 0
                        mov              qword ptr [r9 + 120], 0
.Lx551_42:              cmp              rdx, 2;                              jbe   .Lx551_12
                        mov              rdi, qword ptr [rcx + 40]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 128]            # right
                        mov              qword ptr [r9 + 128], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 136]
                        mov              qword ptr [r9 + 136], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx551_43
.Lx551_12:              mov              rax, qword ptr [r9 + 128]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
.Lx551_43:              lea              rcx, [rip + Binary_γ]
                        lea              r11, [rip + Binary_ω]
                        push             r11
                        push             rcx
                        lea              rax, [rip + n243_statement_begin_α]; jmp   rax
Binary_γ:               mov              rdi, qword ptr [r9 + 96]             # Binary
                        mov              rsi, qword ptr [r9 + 104]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rdx, 0;                              jbe   .Lx551_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 88], rax;            jmp   .Lx551_110
.Lx551_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 88], rax
.Lx551_110:             cmp              rdx, 1;                              jbe   .Lx551_81
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 112], rax            # left
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 120], rax;           jmp   .Lx551_111
.Lx551_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 120], rax
.Lx551_111:             cmp              rdx, 2;                              jbe   .Lx551_82
                        mov              r11, qword ptr [rcx + 40]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 128], rax            # right
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx551_112
.Lx551_82:              mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 136], rax
.Lx551_112:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 96
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
Binary_ω:               mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax             # Binary
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rdx, 0;                              jbe   .Lx551_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 88], rax;            jmp   .Lx551_180
.Lx551_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 88], rax
.Lx551_180:             cmp              rdx, 1;                              jbe   .Lx551_151
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 112], rax            # left
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 120], rax;           jmp   .Lx551_181
.Lx551_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 120], rax
.Lx551_181:             cmp              rdx, 2;                              jbe   .Lx551_152
                        mov              r11, qword ptr [rcx + 40]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 128], rax            # right
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx551_182
.Lx551_152:             mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 136], rax
.Lx551_182:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 96
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx551_245:
#-----------------------------------------------------------------------------------------------------------------------
n242_statement_end_α:                                                         jmp   n276_statement_begin_α
#=======================================================================================================================
# Binary   right    =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n243_statement_begin_α:                                                       jmp   n244_call_α
n243_statement_begin_β:                                                       jmp   n247_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n244_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig557z]
                        lea              rax, [rip + Pop_α];                  jmp   rax
.Lsig557z:              .quad            0
                        .quad            .Lx557_2
                        .quad            .Lx557_2
.Lx557_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx557_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx557_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx557_240
                        add              rsp, 16;                             jmp   n243_statement_begin_β
.Lx557_240:                                                                   jmp   n245_assign_α
n244_call_β:                                                                  jmp   n243_statement_begin_β
.Lx557_0:               .quad            .Lx557_0_s
.Lx557_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n245_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax            # right
                        mov              qword ptr [r9 + 136], rdx;           jmp   n246_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n246_statement_end_α:   add              rsp, 16;                             jmp   n247_statement_begin_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n247_statement_begin_α:                                                       jmp   n248_call_α
n247_statement_begin_β:                                                       jmp   n251_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n248_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig564z]
                        lea              rax, [rip + Pop_α];                  jmp   rax
.Lsig564z:              .quad            0
                        .quad            .Lx564_2
                        .quad            .Lx564_2
.Lx564_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx564_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx564_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx564_240
                        add              rsp, 16;                             jmp   n247_statement_begin_β
.Lx564_240:                                                                   jmp   n249_assign_α
n248_call_β:                                                                  jmp   n247_statement_begin_β
.Lx564_0:               .quad            .Lx564_0_s
.Lx564_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n249_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              qword ptr [r9 + 88], rdx;            jmp   n250_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n250_statement_end_α:   add              rsp, 16;                             jmp   n251_statement_begin_α
#=======================================================================================================================
#          left     =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n251_statement_begin_α:                                                       jmp   n252_call_α
n251_statement_begin_β:                                                       jmp   n255_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n252_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig571z]
                        lea              rax, [rip + Pop_α];                  jmp   rax
.Lsig571z:              .quad            0
                        .quad            .Lx571_2
                        .quad            .Lx571_2
.Lx571_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx571_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx571_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx571_240
                        add              rsp, 16;                             jmp   n251_statement_begin_β
.Lx571_240:                                                                   jmp   n253_assign_α
n252_call_β:                                                                  jmp   n251_statement_begin_β
.Lx571_0:               .quad            .Lx571_0_s
.Lx571_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n253_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # left
                        mov              qword ptr [r9 + 120], rdx;           jmp   n254_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n254_statement_end_α:   add              rsp, 16;                             jmp   n255_statement_begin_α
#=======================================================================================================================
#          Push()   =  EVAL(left ' ' op ' ' right)
#-----------------------------------------------------------------------------------------------------------------------
n255_statement_begin_α:                                                       jmp   n256_lit_string_α
n255_statement_begin_β:                                                       jmp   n271_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n256_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx577_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n257_call_α
.Lx577_0:               .quad            .Lx577_0_s
.Lx577_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n257_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd579:            .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd579]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx578_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n255_statement_begin_β
.Lx578_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n258_call_α
n257_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n255_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n258_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig581z]
                        lea              rax, [rip + Push_α];                 jmp   rax
.Lsig581z:              .quad            0
                        .quad            .Lx581_2
                        .quad            .Lx581_2
.Lx581_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx581_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx581_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx581_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n255_statement_begin_β
.Lx581_240:                                                                   jmp   n259_var_α
n258_call_β:                                                                  jmp   n255_statement_begin_β
.Lx581_0:               .quad            .Lx581_0_s
.Lx581_0_s:             .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n259_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 112]            # left
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n260_lit_string_α
n259_var_β:             add              rsp, 16
                        add              rsp, 48;                             jmp   n255_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n260_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx583_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n261_binop_α
n260_lit_string_β:      add              rsp, 16;                             jmp   n259_var_β
.Lx583_0:               .quad            .Lx583_0_s
.Lx583_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n261_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n262_var_α
n261_binop_β:           add              rsp, 16;                             jmp   n260_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n262_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 80]             # op
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n263_binop_α
n262_var_β:             add              rsp, 16;                             jmp   n261_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n263_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # binop
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n264_lit_string_α
n263_binop_β:           add              rsp, 16;                             jmp   n262_var_β
#-----------------------------------------------------------------------------------------------------------------------
n264_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx587_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n265_binop_α
n264_lit_string_β:      add              rsp, 16;                             jmp   n263_binop_β
.Lx587_0:               .quad            .Lx587_0_s
.Lx587_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n265_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # binop
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n266_var_α
n265_binop_β:           add              rsp, 16;                             jmp   n264_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n266_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 128]            # right
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n267_binop_α
n266_var_β:             add              rsp, 16;                             jmp   n265_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n267_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # binop
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n268_call_α
n267_binop_β:           add              rsp, 16;                             jmp   n266_var_β
#-----------------------------------------------------------------------------------------------------------------------
n268_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd592:            .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd592]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx591_240
                        add              rsp, 16;                             jmp   n267_binop_β
.Lx591_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n269_assign_var_α
n268_call_β:            add              rsp, 16;                             jmp   n267_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n269_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 176]           # call
                        mov              rsi, qword ptr [rsp + 184]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx593_240
                        add              rsp, 32;                             jmp   n267_binop_β
.Lx593_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n270_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n270_statement_end_α:   add              rsp, 224;                            jmp   n271_statement_begin_α
#=======================================================================================================================
#          Binary   =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n271_statement_begin_α:                                                       jmp   n272_lit_string_α
n271_statement_begin_β:                                                       jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n272_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx598_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n273_call_α
.Lx598_0:               .quad            .Lx598_0_s
.Lx598_0_s:             .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n273_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd600:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd600]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx599_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n271_statement_begin_β
.Lx599_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n274_assign_α
n273_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n271_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n274_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax             # Binary
                        mov              qword ptr [r9 + 104], rdx;           jmp   n275_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n275_statement_end_α:   add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# BinaryEnd  <stmt 24, line 31: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n276_statement_begin_α:                                                       jmp   n277_statement_end_α
n276_statement_begin_β:                                                       jmp   n278_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n277_statement_end_α:                                                         jmp   n278_statement_begin_α
#=======================================================================================================================
#          integer  =  SPAN('0123456789')
#-----------------------------------------------------------------------------------------------------------------------
n278_statement_begin_α:                                                       jmp   n279_lit_string_α
n278_statement_begin_β:                                                       jmp   n283_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n279_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx610_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n280_call_α
.Lx610_0:               .quad            .Lx610_0_s
.Lx610_0_s:             .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n280_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd612:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd612]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx611_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n278_statement_begin_β
.Lx611_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n281_assign_α
n280_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n278_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n281_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 176], rax            # integer
                        mov              qword ptr [r9 + 184], rdx;           jmp   n282_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n282_statement_end_α:   add              rsp, 32;                             jmp   n283_statement_begin_α
#=======================================================================================================================
#          exponent =  ANY('eEdD') (ANY('+-') | epsilon) integer
#-----------------------------------------------------------------------------------------------------------------------
n283_statement_begin_α:                                                       jmp   n284_var_α
n283_statement_begin_β:                                                       jmp   n293_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n284_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 208]            # epsilon
                        mov              rdx, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n285_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n285_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 368], rax            # PAT$1$V0
                        mov              qword ptr [r9 + 376], rdx;           jmp   n286_var_α
#-----------------------------------------------------------------------------------------------------------------------
n286_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 176]            # integer
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n287_assign_α
n286_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n283_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n287_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 384], rax            # PAT$1$V1
                        mov              qword ptr [r9 + 392], rdx;           jmp   n288_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n288_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx622_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n289_lit_string_α
n288_lit_string_β:      add              rsp, 16;                             jmp   n286_var_β
.Lx622_0:               .quad            .Lx622_0_s
.Lx622_0_s:             .string          "PAT$1"
#-----------------------------------------------------------------------------------------------------------------------
n289_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx623_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n290_call_α
n289_lit_string_β:      add              rsp, 16;                             jmp   n288_lit_string_β
.Lx623_0:               .quad            .Lx623_0_s
.Lx623_0_s:             .string          "2"
#-----------------------------------------------------------------------------------------------------------------------
n290_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lrkfnzd625:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd625]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx624_240
                        add              rsp, 16;                             jmp   n289_lit_string_β
.Lx624_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n291_assign_α
n290_call_β:            add              rsp, 16;                             jmp   n289_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n291_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 192], rax            # exponent
                        mov              qword ptr [r9 + 200], rdx;           jmp   n292_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n292_statement_end_α:   add              rsp, 80;                             jmp   n293_statement_begin_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#-----------------------------------------------------------------------------------------------------------------------
n293_statement_begin_α:                                                       jmp   n294_var_α
n293_statement_begin_β:                                                       jmp   n313_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n294_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 176]            # integer
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n295_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n295_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 400], rax            # PAT$2$V0
                        mov              qword ptr [r9 + 408], rdx;           jmp   n296_var_α
#-----------------------------------------------------------------------------------------------------------------------
n296_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 176]            # integer
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n297_assign_α
n296_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n293_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n297_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 416], rax            # PAT$2$V1
                        mov              qword ptr [r9 + 424], rdx;           jmp   n298_var_α
#-----------------------------------------------------------------------------------------------------------------------
n298_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 208]            # epsilon
                        mov              rdx, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n299_assign_α
n298_var_β:             add              rsp, 16;                             jmp   n296_var_β
#-----------------------------------------------------------------------------------------------------------------------
n299_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 432], rax            # PAT$2$V2
                        mov              qword ptr [r9 + 440], rdx;           jmp   n300_var_α
#-----------------------------------------------------------------------------------------------------------------------
n300_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 192]            # exponent
                        mov              rdx, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n301_assign_α
n300_var_β:             add              rsp, 16;                             jmp   n298_var_β
#-----------------------------------------------------------------------------------------------------------------------
n301_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 448], rax            # PAT$2$V3
                        mov              qword ptr [r9 + 456], rdx;           jmp   n302_var_α
#-----------------------------------------------------------------------------------------------------------------------
n302_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 208]            # epsilon
                        mov              rdx, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n303_assign_α
n302_var_β:             add              rsp, 16;                             jmp   n300_var_β
#-----------------------------------------------------------------------------------------------------------------------
n303_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 464], rax            # PAT$2$V4
                        mov              qword ptr [r9 + 472], rdx;           jmp   n304_var_α
#-----------------------------------------------------------------------------------------------------------------------
n304_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 176]            # integer
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n305_assign_α
n304_var_β:             add              rsp, 16;                             jmp   n302_var_β
#-----------------------------------------------------------------------------------------------------------------------
n305_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 480], rax            # PAT$2$V5
                        mov              qword ptr [r9 + 488], rdx;           jmp   n306_var_α
#-----------------------------------------------------------------------------------------------------------------------
n306_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 192]            # exponent
                        mov              rdx, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n307_assign_α
n306_var_β:             add              rsp, 16;                             jmp   n304_var_β
#-----------------------------------------------------------------------------------------------------------------------
n307_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 496], rax            # PAT$2$V6
                        mov              qword ptr [r9 + 504], rdx;           jmp   n308_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n308_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx645_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n309_lit_string_α
n308_lit_string_β:      add              rsp, 16;                             jmp   n306_var_β
.Lx645_0:               .quad            .Lx645_0_s
.Lx645_0_s:             .string          "PAT$2"
#-----------------------------------------------------------------------------------------------------------------------
n309_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx646_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n310_call_α
n309_lit_string_β:      add              rsp, 16;                             jmp   n308_lit_string_β
.Lx646_0:               .quad            .Lx646_0_s
.Lx646_0_s:             .string          "7"
#-----------------------------------------------------------------------------------------------------------------------
n310_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lrkfnzd648:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd648]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx647_240
                        add              rsp, 16;                             jmp   n309_lit_string_β
.Lx647_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n311_assign_α
n310_call_β:            add              rsp, 16;                             jmp   n309_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n311_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 224], rax            # real
                        mov              qword ptr [r9 + 232], rdx;           jmp   n312_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n312_statement_end_α:                                                         jmp   n313_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n313_statement_begin_α:                                                       jmp   n314_statement_end_α
n313_statement_begin_β: add              rsp, 160;                            jmp   n315_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n314_statement_end_α:   add              rsp, 160;                            jmp   n315_statement_begin_α
#=======================================================================================================================
#          addop    =  ANY('+-') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n315_statement_begin_α:                                                       jmp   n316_lit_string_α
n315_statement_begin_β:                                                       jmp   n320_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n316_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx658_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n317_call_α
.Lx658_0:               .quad            .Lx658_0_s
.Lx658_0_s:             .string          "PAT$3"
#-----------------------------------------------------------------------------------------------------------------------
n317_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd660:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd660]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx659_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n315_statement_begin_β
.Lx659_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n318_assign_α
n317_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n315_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n318_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 240], rax            # addop
                        mov              qword ptr [r9 + 248], rdx;           jmp   n319_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n319_statement_end_α:   add              rsp, 32;                             jmp   n320_statement_begin_α
#=======================================================================================================================
#          mulop    =  ANY('*/') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n320_statement_begin_α:                                                       jmp   n321_lit_string_α
n320_statement_begin_β:                                                       jmp   n325_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n321_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx666_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n322_call_α
.Lx666_0:               .quad            .Lx666_0_s
.Lx666_0_s:             .string          "PAT$4"
#-----------------------------------------------------------------------------------------------------------------------
n322_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd668:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd668]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx667_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n320_statement_begin_β
.Lx667_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n323_assign_α
n322_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n320_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n323_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 256], rax            # mulop
                        mov              qword ptr [r9 + 264], rdx;           jmp   n324_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n324_statement_end_α:   add              rsp, 32;                             jmp   n325_statement_begin_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n325_statement_begin_α:                                                       jmp   n326_var_α
n325_statement_begin_β:                                                       jmp   n335_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n326_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]            # real
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n327_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n327_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 512], rax            # PAT$5$V0
                        mov              qword ptr [r9 + 520], rdx;           jmp   n328_var_α
#-----------------------------------------------------------------------------------------------------------------------
n328_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 176]            # integer
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n329_assign_α
n328_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n325_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n329_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 528], rax            # PAT$5$V1
                        mov              qword ptr [r9 + 536], rdx;           jmp   n330_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n330_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx678_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n331_lit_string_α
n330_lit_string_β:      add              rsp, 16;                             jmp   n328_var_β
.Lx678_0:               .quad            .Lx678_0_s
.Lx678_0_s:             .string          "PAT$5"
#-----------------------------------------------------------------------------------------------------------------------
n331_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx679_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n332_call_α
n331_lit_string_β:      add              rsp, 16;                             jmp   n330_lit_string_β
.Lx679_0:               .quad            .Lx679_0_s
.Lx679_0_s:             .string          "2"
#-----------------------------------------------------------------------------------------------------------------------
n332_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lrkfnzd681:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd681]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx680_240
                        add              rsp, 16;                             jmp   n331_lit_string_β
.Lx680_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n333_assign_α
n332_call_β:            add              rsp, 16;                             jmp   n331_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n333_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 272], rax            # constant
                        mov              qword ptr [r9 + 280], rdx;           jmp   n334_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n334_statement_end_α:                                                         jmp   n335_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n335_statement_begin_α:                                                       jmp   n336_statement_end_α
n335_statement_begin_β: add              rsp, 80;                             jmp   n337_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n336_statement_end_α:   add              rsp, 80;                             jmp   n337_statement_begin_α
#=======================================================================================================================
#          primary  =  constant | '(' *expr ')'
#-----------------------------------------------------------------------------------------------------------------------
n337_statement_begin_α:                                                       jmp   n338_var_α
n337_statement_begin_β:                                                       jmp   n345_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n338_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 272]            # constant
                        mov              rdx, qword ptr [r9 + 280]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n339_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n339_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 544], rax            # PAT$6$V0
                        mov              qword ptr [r9 + 552], rdx;           jmp   n340_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n340_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx693_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n341_lit_string_α
n340_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n337_statement_begin_β
.Lx693_0:               .quad            .Lx693_0_s
.Lx693_0_s:             .string          "PAT$6"
#-----------------------------------------------------------------------------------------------------------------------
n341_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx694_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n342_call_α
n341_lit_string_β:      add              rsp, 16;                             jmp   n340_lit_string_β
.Lx694_0:               .quad            .Lx694_0_s
.Lx694_0_s:             .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
n342_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lrkfnzd696:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd696]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx695_240
                        add              rsp, 16;                             jmp   n341_lit_string_β
.Lx695_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n343_assign_α
n342_call_β:            add              rsp, 16;                             jmp   n341_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n343_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 288], rax            # primary
                        mov              qword ptr [r9 + 296], rdx;           jmp   n344_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n344_statement_end_α:                                                         jmp   n345_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n345_statement_begin_α:                                                       jmp   n346_statement_end_α
n345_statement_begin_β: add              rsp, 64;                             jmp   n347_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n346_statement_end_α:   add              rsp, 64;                             jmp   n347_statement_begin_α
#=======================================================================================================================
#          factor   =  addop *factor . *Unary()
#-----------------------------------------------------------------------------------------------------------------------
n347_statement_begin_α:                                                       jmp   n348_var_α
n347_statement_begin_β:                                                       jmp   n355_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n348_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 240]            # addop
                        mov              rdx, qword ptr [r9 + 248]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n349_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n349_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 560], rax            # PAT$7$V0
                        mov              qword ptr [r9 + 568], rdx;           jmp   n350_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n350_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx708_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n351_lit_string_α
n350_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n347_statement_begin_β
.Lx708_0:               .quad            .Lx708_0_s
.Lx708_0_s:             .string          "PAT$7"
#-----------------------------------------------------------------------------------------------------------------------
n351_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx709_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n352_call_α
n351_lit_string_β:      add              rsp, 16;                             jmp   n350_lit_string_β
.Lx709_0:               .quad            .Lx709_0_s
.Lx709_0_s:             .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
n352_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lrkfnzd711:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd711]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx710_240
                        add              rsp, 16;                             jmp   n351_lit_string_β
.Lx710_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n353_assign_α
n352_call_β:            add              rsp, 16;                             jmp   n351_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n353_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 304], rax            # factor
                        mov              qword ptr [r9 + 312], rdx;           jmp   n354_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n354_statement_end_α:                                                         jmp   n355_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n355_statement_begin_α:                                                       jmp   n356_statement_end_α
n355_statement_begin_β: add              rsp, 64;                             jmp   n357_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n356_statement_end_α:   add              rsp, 64;                             jmp   n357_statement_begin_α
#=======================================================================================================================
#          term     =  *factor mulop *term . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n357_statement_begin_α:                                                       jmp   n358_var_α
n357_statement_begin_β:                                                       jmp   n365_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n358_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 256]            # mulop
                        mov              rdx, qword ptr [r9 + 264]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n359_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n359_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 576], rax            # PAT$8$V0
                        mov              qword ptr [r9 + 584], rdx;           jmp   n360_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n360_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx723_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n361_lit_string_α
n360_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n357_statement_begin_β
.Lx723_0:               .quad            .Lx723_0_s
.Lx723_0_s:             .string          "PAT$8"
#-----------------------------------------------------------------------------------------------------------------------
n361_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx724_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n362_call_α
n361_lit_string_β:      add              rsp, 16;                             jmp   n360_lit_string_β
.Lx724_0:               .quad            .Lx724_0_s
.Lx724_0_s:             .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
n362_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lrkfnzd726:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd726]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx725_240
                        add              rsp, 16;                             jmp   n361_lit_string_β
.Lx725_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n363_assign_α
n362_call_β:            add              rsp, 16;                             jmp   n361_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n363_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 320], rax            # term
                        mov              qword ptr [r9 + 328], rdx;           jmp   n364_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n364_statement_end_α:                                                         jmp   n365_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n365_statement_begin_α:                                                       jmp   n366_statement_end_α
n365_statement_begin_β: add              rsp, 64;                             jmp   n367_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n366_statement_end_α:   add              rsp, 64;                             jmp   n367_statement_begin_α
#=======================================================================================================================
#          expr     =  *term addop *expr . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n367_statement_begin_α:                                                       jmp   n368_var_α
n367_statement_begin_β:                                                       jmp   n375_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n368_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 240]            # addop
                        mov              rdx, qword ptr [r9 + 248]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n369_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n369_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 592], rax            # PAT$9$V0
                        mov              qword ptr [r9 + 600], rdx;           jmp   n370_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n370_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx738_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n371_lit_string_α
n370_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n367_statement_begin_β
.Lx738_0:               .quad            .Lx738_0_s
.Lx738_0_s:             .string          "PAT$9"
#-----------------------------------------------------------------------------------------------------------------------
n371_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx739_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n372_call_α
n371_lit_string_β:      add              rsp, 16;                             jmp   n370_lit_string_β
.Lx739_0:               .quad            .Lx739_0_s
.Lx739_0_s:             .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
n372_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lrkfnzd741:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd741]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx740_240
                        add              rsp, 16;                             jmp   n371_lit_string_β
.Lx740_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n373_assign_α
n372_call_β:            add              rsp, 16;                             jmp   n371_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n373_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 336], rax            # expr
                        mov              qword ptr [r9 + 344], rdx;           jmp   n374_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n374_statement_end_α:                                                         jmp   n375_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n375_statement_begin_α:                                                       jmp   n376_statement_end_α
n375_statement_begin_β: add              rsp, 64;                             jmp   n377_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n376_statement_end_α:   add              rsp, 64;                             jmp   n377_statement_begin_α
#=======================================================================================================================
#          &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n377_statement_begin_α:                                                       jmp   n378_lit_integer_α
n377_statement_begin_β:                                                       jmp   n381_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n378_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx751_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n379_keyword_assign_snobol4_α
.Lx751_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n379_keyword_assign_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx752_0]
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_kw_write_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx752_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n377_statement_begin_β
.Lx752_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n380_statement_end_α
.Lx752_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n380_statement_end_α:   add              rsp, 32;                             jmp   n381_statement_begin_α
#=======================================================================================================================
# loop     line     =  INPUT                         :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n381_statement_begin_α:                                                       jmp   n382_var_α
n381_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n382_var_α:             sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx757_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_GET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx757_240
                        add              rsp, 16;                             jmp   n381_statement_begin_β
.Lx757_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n383_assign_α
.Lx757_0:               .quad            .Lx757_0_s
.Lx757_0_s:             .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n383_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 352], rax            # line
                        mov              qword ptr [r9 + 360], rdx;           jmp   n384_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n384_statement_end_α:   add              rsp, 16;                             jmp   n385_statement_begin_α
#=======================================================================================================================
#          line     POS(0) expr RPOS(0)              :F(error)
#-----------------------------------------------------------------------------------------------------------------------
n385_statement_begin_α:                                                       jmp   n386_var_α
n385_statement_begin_β:                                                       jmp   n399_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n386_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 352]            # line
                        mov              rdx, qword ptr [r9 + 360]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n387_var_α
#-----------------------------------------------------------------------------------------------------------------------
n387_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 336]            # expr
                        mov              rdx, qword ptr [r9 + 344]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n388_assign_α
n387_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n399_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n388_assign_α:          mov              rsi, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx765_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n389_match_begin_α
n388_assign_β:                                                                jmp   n387_var_β
.Lx765_0:               .quad            .Lx765_0_s
.Lx765_0_s:             .string          "PATV$0"
#-----------------------------------------------------------------------------------------------------------------------
n389_match_begin_α:     mov              rdi, qword ptr [rsp + 16]            # var
                        mov              rsi, qword ptr [rsp + 24]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 24
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx767_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx767_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n390_match_pos_α
n389_match_begin_β:
.Lx767_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx767_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx767_1
                                                                              jmp   .Lx767_0
.Lx767_1:
n389_match_begin_af:    mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13                             # sig
                        mov              rsi, r15                             # len
                        call             rt_match_ctx_restore@PLT
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   n388_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n390_match_pos_α:       mov              rax, 0
                        cmp              r14d, eax;                           jne   n389_match_begin_β
                                                                              jmp   n391_match_defer_α
n390_match_pos_β:                                                             jmp   n389_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n391_match_defer_α:     lea              rdi, [rip + .S22]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx769_14
                        mov              rax, qword ptr [rdx + 0]
.Lx769_14:              test             rax, rax;                            jz    .Lx769_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx769_5]
                        push             rcx
                        lea              rcx, [rip + .Lx769_4]
                        push             rcx;                                 jmp   rax
.Lx769_4:                                                                     jmp   n392_match_rpos_α
.Lx769_5:                                                                     jmp   n389_match_begin_β
.Lx769_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S22]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n389_match_begin_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx769_6]
                        push             rcx
                        push             rax;                                 jmp   n392_match_rpos_α
.Lx769_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n389_match_begin_β
n391_match_defer_β:     cmp              qword ptr [rsp + 0], 0;              jne   .Lx769_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx769_12
                                                                              jmp   rax
.Lx769_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n392_match_rpos_α:      mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx;                           jne   n391_match_defer_β
                                                                              jmp   n393_match_end_α
#-----------------------------------------------------------------------------------------------------------------------
n393_match_end_α:       mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -8]            # cas_mark
                        mov              rax, qword ptr [rbp + -32]           # outer_Δ
                        mov              rcx, qword ptr [rbp + -16]           # outer_Σ
                        push             rax
                        push             rcx
                        lea              rcx, [rsp + 0]
                        mov              rsi, r12
                        mov              rdx, r13
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx772_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n389_match_begin_af
.Lx772_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n394_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n394_statement_end_α:   add              rsp, 32;                             jmp   n395_statement_begin_α
#=======================================================================================================================
#          OUTPUT   =  Pop()                         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n395_statement_begin_α:                                                       jmp   n396_call_α
n395_statement_begin_β:                                                       jmp   n381_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n396_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig778z]
                        lea              rax, [rip + Pop_α];                  jmp   rax
.Lsig778z:              .quad            0
                        .quad            .Lx778_2
                        .quad            .Lx778_2
.Lx778_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx778_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx778_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx778_240
                        add              rsp, 16;                             jmp   n395_statement_begin_β
.Lx778_240:                                                                   jmp   n397_assign_α
n396_call_β:                                                                  jmp   n395_statement_begin_β
.Lx778_0:               .quad            .Lx778_0_s
.Lx778_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n397_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx779_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n398_statement_end_α
.Lx779_0:               .quad            .Lx779_0_s
.Lx779_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n398_statement_end_α:   add              rsp, 16;                             jmp   n381_statement_begin_α
#=======================================================================================================================
# error    OUTPUT   = 'Bad input, try again'         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n399_statement_begin_α:                                                       jmp   n400_lit_string_α
n399_statement_begin_β:                                                       jmp   n381_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n400_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 20
                        mov              rax, qword ptr [rip + .Lx784_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n401_assign_α
.Lx784_0:               .quad            .Lx784_0_s
.Lx784_0_s:             .string          "Bad input, try again"
#-----------------------------------------------------------------------------------------------------------------------
n401_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx785_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n402_statement_end_α
.Lx785_0:               .quad            .Lx785_0_s
.Lx785_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n402_statement_end_α:   add              rsp, 16;                             jmp   n381_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 pop              rcx
                        add              rsp, 8;                              jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                add              rsp, 8
                        pop              rcx;                                 jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
NRETURN:                mov              rax, qword ptr [rip + rt_g_ret_by_name@GOTPCREL]
                        mov              dword ptr [rax + 0], 1;              jmp   RETURN
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
module_init:
                        sub              rsp, 8
                        .section         .rodata
.Lseala4:               .string          "Push"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            Push_α
                        lea              rdi, [rip + .Lseala4]
                        mov              rsi, qword ptr [rip + Push_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala5:               .string          "Pop"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            Pop_α
                        lea              rdi, [rip + .Lseala5]
                        mov              rsi, qword ptr [rip + Pop_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala6:               .string          "Unary"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            Unary_α
                        lea              rdi, [rip + .Lseala6]
                        mov              rsi, qword ptr [rip + Unary_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala7:               .string          "Binary"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            Binary_α
                        lea              rdi, [rip + .Lseala7]
                        mov              rsi, qword ptr [rip + Binary_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lstartup_pname8:       .string          "PAT$0"
                        .align           8
.Lstartup_prec8:
                        .quad            .Lstartup_pname8
                        .quad            FN__PAT$0
                        .quad            0
                        .quad            0
                        .quad            0
                        .long            0
                        .long            0
                        .long            32
                        .long            18
                        .long            0
                        .long            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_prec8]
                        call             rt_proc_register_rec@PLT
                        .section         .rodata
.Lstartup_pname9:       .string          "PAT$1"
                        .align           8
.Lstartup_prec9:
                        .quad            .Lstartup_pname9
                        .quad            FN__PAT$1
                        .quad            0
                        .quad            0
                        .quad            0
                        .long            0
                        .long            0
                        .long            112
                        .long            16
                        .long            0
                        .long            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_prec9]
                        call             rt_proc_register_rec@PLT
                        .section         .rodata
.Lstartup_pname10:      .string          "PAT$2"
                        .align           8
.Lstartup_prec10:
                        .quad            .Lstartup_pname10
                        .quad            FN__PAT$2
                        .quad            0
                        .quad            0
                        .quad            0
                        .long            0
                        .long            0
                        .long            256
                        .long            16
                        .long            0
                        .long            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_prec10]
                        call             rt_proc_register_rec@PLT
                        .section         .rodata
.Lstartup_pname11:      .string          "PAT$3"
                        .align           8
.Lstartup_prec11:
                        .quad            .Lstartup_pname11
                        .quad            FN__PAT$3
                        .quad            0
                        .quad            0
                        .quad            0
                        .long            0
                        .long            0
                        .long            96
                        .long            18
                        .long            0
                        .long            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_prec11]
                        call             rt_proc_register_rec@PLT
                        .section         .rodata
.Lstartup_pname12:      .string          "PAT$4"
                        .align           8
.Lstartup_prec12:
                        .quad            .Lstartup_pname12
                        .quad            FN__PAT$4
                        .quad            0
                        .quad            0
                        .quad            0
                        .long            0
                        .long            0
                        .long            96
                        .long            18
                        .long            0
                        .long            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_prec12]
                        call             rt_proc_register_rec@PLT
                        .section         .rodata
.Lstartup_pname13:      .string          "PAT$5"
                        .align           8
.Lstartup_prec13:
                        .quad            .Lstartup_pname13
                        .quad            FN__PAT$5
                        .quad            0
                        .quad            0
                        .quad            0
                        .long            0
                        .long            0
                        .long            144
                        .long            16
                        .long            0
                        .long            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_prec13]
                        call             rt_proc_register_rec@PLT
                        .section         .rodata
.Lstartup_pname14:      .string          "PAT$6"
                        .align           8
.Lstartup_prec14:
                        .quad            .Lstartup_pname14
                        .quad            FN__PAT$6
                        .quad            0
                        .quad            0
                        .quad            0
                        .long            0
                        .long            0
                        .long            112
                        .long            16
                        .long            0
                        .long            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_prec14]
                        call             rt_proc_register_rec@PLT
                        .section         .rodata
.Lstartup_pname15:      .string          "PAT$7"
                        .align           8
.Lstartup_prec15:
                        .quad            .Lstartup_pname15
                        .quad            FN__PAT$7
                        .quad            0
                        .quad            0
                        .quad            0
                        .long            0
                        .long            0
                        .long            160
                        .long            16
                        .long            0
                        .long            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_prec15]
                        call             rt_proc_register_rec@PLT
                        .section         .rodata
.Lstartup_pname16:      .string          "PAT$8"
                        .align           8
.Lstartup_prec16:
                        .quad            .Lstartup_pname16
                        .quad            FN__PAT$8
                        .quad            0
                        .quad            0
                        .quad            0
                        .long            0
                        .long            0
                        .long            176
                        .long            16
                        .long            0
                        .long            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_prec16]
                        call             rt_proc_register_rec@PLT
                        .section         .rodata
.Lstartup_pname17:      .string          "PAT$9"
                        .align           8
.Lstartup_prec17:
                        .quad            .Lstartup_pname17
                        .quad            FN__PAT$9
                        .quad            0
                        .quad            0
                        .quad            0
                        .long            0
                        .long            0
                        .long            176
                        .long            16
                        .long            0
                        .long            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_prec17]
                        call             rt_proc_register_rec@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.S0:                    .string          "PAT$1$V1"
.S1:                    .string          "PAT$1$V0"
.S2:                    .string          "PAT$2$V5"
.S3:                    .string          "PAT$2$V6"
.S4:                    .string          "PAT$2$V0"
.S5:                    .string          "PAT$2$V4"
.S6:                    .string          "PAT$2$V3"
.S7:                    .string          "PAT$2$V2"
.S8:                    .string          "PAT$2$V1"
.S9:                    .string          "*Push"
.S10:                   .string          "PAT$5$V1"
.S11:                   .string          "PAT$5$V0"
.S12:                   .string          "expr"
.S13:                   .string          "PAT$6$V0"
.S14:                   .string          "primary"
.S15:                   .string          "PAT$7$V0"
.S16:                   .string          "factor"
.S17:                   .string          "*Unary"
.S18:                   .string          "PAT$8$V0"
.S19:                   .string          "term"
.S20:                   .string          "*Binary"
.S21:                   .string          "PAT$9$V0"
.S22:                   .string          "PATV$0"
                        .text
                        .section         .rodata
.C0:                    .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
.C1:                    .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .text
                        .section         .note.GNU-stack,"",@progbits
