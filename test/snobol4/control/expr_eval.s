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
                        mov              r11, 1
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
n0_match_span_β:        mov              r11, 1
                        mov              r14d, dword ptr [rbp + -60]
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
n3_match_any_α:         mov              r11, 2
                        mov              eax, r14d
                        cmp              eax, r15d;                           jge   PAT$1_ω
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        lea              rdi, [rip + .C1]
                        cmp              byte ptr [rdi+rsi], 0;               je    PAT$1_ω
                        add              r14d, 1;                             jmp   n4_match_alternate_α
n3_match_any_β:         mov              r11, 2
                        sub              r14d, 1;                             jmp   PAT$1_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_match_alternate_α:   mov              r11, 3
                        mov              dword ptr [rbp + -72], r14d
                        lea              rax, [rip + .Lx11_21]
                        mov              qword ptr [rbp + -56], rax;          jmp   n7_match_any_α
.Lx11_21:               lea              rax, [rip + .Lx11_19]
                        mov              qword ptr [rbp + -56], rax;          jmp   n6_match_defer_α
n4_match_alternate_s0:  mov              r11, 3
                        lea              rax, [rip + .Lx11_40]
                        mov              qword ptr [rbp + -64], rax;          jmp   n4_match_alternate_as
n4_match_alternate_s1:  mov              r11, 3
                        lea              rax, [rip + .Lx11_41]
                        mov              qword ptr [rbp + -64], rax;          jmp   n4_match_alternate_as
.Lx11_40:                                                                     jmp   n7_match_any_β
.Lx11_41:                                                                     jmp   n6_match_defer_β
n4_match_alternate_as:  mov              r11, 3;                              jmp   n5_match_defer_α
n4_match_alternate_β:   mov              r11, 3
                        mov              rax, qword ptr [rbp + -64];          jmp   rax
n4_match_alternate_af:  mov              r11, 3
                        mov              r14d, dword ptr [rbp + -72]
                        mov              rax, qword ptr [rbp + -56];          jmp   rax
.Lx11_19:                                                                     jmp   n3_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n5_match_defer_α:       sub              rsp, 16
                        mov              r11, 4
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 1
                        lea              rdx, [rip + .S0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
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
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
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
n5_match_defer_β:       mov              r11, 4
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx12_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx12_12
                                                                              jmp   rax
.Lx12_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n6_match_defer_α:       mov              r11, 5
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S1]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
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
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
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
n6_match_defer_β:       mov              r11, 5
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx13_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx13_12
                                                                              jmp   rax
.Lx13_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n7_match_any_α:         mov              r11, 6
                        mov              eax, r14d
                        cmp              eax, r15d;                           jge   n4_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 43;                             je    .Lx15_0
                        cmp              esi, 45;                             je    .Lx15_0
                                                                              jmp   n4_match_alternate_af
.Lx15_0:                add              r14d, 1;                             jmp   n4_match_alternate_s0
n7_match_any_β:         mov              r11, 6
                        sub              r14d, 1;                             jmp   n4_match_alternate_af
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
                        sub              rsp, 120
                        mov              rcx, qword ptr [rbp + 8]
                        mov              qword ptr [rbp + -8], rcx
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        mov              qword ptr [rbp + -32], r12
#-----------------------------------------------------------------------------------------------------------------------
n16_match_alternate_α:  mov              r11, 7
                        sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx28_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n19_match_defer_α
.Lx28_21:               lea              rax, [rip + .Lx28_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n17_match_defer_α
n16_match_alternate_s0: mov              r11, 7
                        lea              rax, [rip + .Lx28_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n16_match_alternate_as
n16_match_alternate_s1: mov              r11, 7
                        lea              rax, [rip + .Lx28_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n16_match_alternate_as
.Lx28_40:                                                                     jmp   n22_match_alternate_β
.Lx28_41:                                                                     jmp   n18_match_defer_β
n16_match_alternate_as: mov              r11, 7;                              jmp   PAT$2_γ
n16_match_alternate_β:  mov              r11, 7
                        mov              rax, qword ptr [rsp + 8];            jmp   rax
n16_match_alternate_af: mov              r11, 7
                        mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx28_19:               add              rsp, 32;                             jmp   PAT$2_ω
#-----------------------------------------------------------------------------------------------------------------------
n17_match_defer_α:      mov              r11, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 5
                        lea              rdx, [rip + .S2]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx29_16
                        mov              rax, qword ptr [rdx + 0]
.Lx29_16:               test             rax, rax;                            jz    .Lx29_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx29_5]
                        push             rcx
                        lea              rcx, [rip + .Lx29_4]
                        push             rcx;                                 jmp   rax
.Lx29_4:                                                                      jmp   n18_match_defer_α
.Lx29_5:                                                                      jmp   n16_match_alternate_af
.Lx29_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 5
                        lea              rdx, [rip + .S2]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n16_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx29_6]
                        push             rcx
                        push             rax;                                 jmp   n18_match_defer_α
.Lx29_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n16_match_alternate_af
n17_match_defer_β:      mov              r11, 8
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx29_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx29_12
                                                                              jmp   rax
.Lx29_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n18_match_defer_α:      mov              r11, 9
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 6
                        lea              rdx, [rip + .S3]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx30_16
                        mov              rax, qword ptr [rdx + 0]
.Lx30_16:               test             rax, rax;                            jz    .Lx30_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx30_5]
                        push             rcx
                        lea              rcx, [rip + .Lx30_4]
                        push             rcx;                                 jmp   rax
.Lx30_4:                                                                      jmp   n16_match_alternate_s1
.Lx30_5:                                                                      jmp   n17_match_defer_β
.Lx30_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 6
                        lea              rdx, [rip + .S3]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n17_match_defer_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx30_6]
                        push             rcx
                        push             rax;                                 jmp   n16_match_alternate_s1
.Lx30_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n17_match_defer_β
n18_match_defer_β:      mov              r11, 9
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx30_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx30_12
                                                                              jmp   rax
.Lx30_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n19_match_defer_α:      mov              r11, 10
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S4]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx31_16
                        mov              rax, qword ptr [rdx + 0]
.Lx31_16:               test             rax, rax;                            jz    .Lx31_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx31_5]
                        push             rcx
                        lea              rcx, [rip + .Lx31_4]
                        push             rcx;                                 jmp   rax
.Lx31_4:                                                                      jmp   n20_match_lit_α
.Lx31_5:                                                                      jmp   n16_match_alternate_af
.Lx31_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S4]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n16_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx31_6]
                        push             rcx
                        push             rax;                                 jmp   n20_match_lit_α
.Lx31_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n16_match_alternate_af
n19_match_defer_β:      mov              r11, 10
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx31_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx31_12
                                                                              jmp   rax
.Lx31_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n20_match_lit_α:        mov              r11, 11
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n19_match_defer_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 46;                             jne   n19_match_defer_β
                        add              r14d, 1;                             jmp   n21_match_alternate_α
n20_match_lit_β:        mov              r11, 11
                        sub              r14d, 1;                             jmp   n19_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n21_match_alternate_α:  mov              r11, 12
                        sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx35_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n26_match_defer_α
.Lx35_21:               lea              rax, [rip + .Lx35_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n25_match_defer_α
n21_match_alternate_s0: mov              r11, 12
                        lea              rax, [rip + .Lx35_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n21_match_alternate_as
n21_match_alternate_s1: mov              r11, 12
                        lea              rax, [rip + .Lx35_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n21_match_alternate_as
.Lx35_40:                                                                     jmp   n26_match_defer_β
.Lx35_41:                                                                     jmp   n25_match_defer_β
n21_match_alternate_as: mov              r11, 12;                             jmp   n22_match_alternate_α
n21_match_alternate_β:  mov              r11, 12
                        mov              rax, qword ptr [rsp + 8];            jmp   rax
n21_match_alternate_af: mov              r11, 12
                        mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx35_19:               add              rsp, 32;                             jmp   n20_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n22_match_alternate_α:  mov              r11, 13
                        sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx37_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n24_match_defer_α
.Lx37_21:               lea              rax, [rip + .Lx37_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n23_match_defer_α
n22_match_alternate_s0: mov              r11, 13
                        lea              rax, [rip + .Lx37_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n22_match_alternate_as
n22_match_alternate_s1: mov              r11, 13
                        lea              rax, [rip + .Lx37_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n22_match_alternate_as
.Lx37_40:                                                                     jmp   n24_match_defer_β
.Lx37_41:                                                                     jmp   n23_match_defer_β
n22_match_alternate_as: mov              r11, 13;                             jmp   n16_match_alternate_s0
n22_match_alternate_β:  mov              r11, 13
                        mov              rax, qword ptr [rsp + 8];            jmp   rax
n22_match_alternate_af: mov              r11, 13
                        mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx37_19:               add              rsp, 32;                             jmp   n21_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n23_match_defer_α:      mov              r11, 14
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 4
                        lea              rdx, [rip + .S5]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx38_16
                        mov              rax, qword ptr [rdx + 0]
.Lx38_16:               test             rax, rax;                            jz    .Lx38_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx38_5]
                        push             rcx
                        lea              rcx, [rip + .Lx38_4]
                        push             rcx;                                 jmp   rax
.Lx38_4:                                                                      jmp   n22_match_alternate_s1
.Lx38_5:                                                                      jmp   n22_match_alternate_af
.Lx38_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 4
                        lea              rdx, [rip + .S5]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n22_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx38_6]
                        push             rcx
                        push             rax;                                 jmp   n22_match_alternate_s1
.Lx38_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n22_match_alternate_af
n23_match_defer_β:      mov              r11, 14
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx38_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx38_12
                                                                              jmp   rax
.Lx38_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n24_match_defer_α:      mov              r11, 15
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 3
                        lea              rdx, [rip + .S6]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx39_16
                        mov              rax, qword ptr [rdx + 0]
.Lx39_16:               test             rax, rax;                            jz    .Lx39_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx39_5]
                        push             rcx
                        lea              rcx, [rip + .Lx39_4]
                        push             rcx;                                 jmp   rax
.Lx39_4:                                                                      jmp   n22_match_alternate_s0
.Lx39_5:                                                                      jmp   n22_match_alternate_af
.Lx39_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 3
                        lea              rdx, [rip + .S6]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n22_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx39_6]
                        push             rcx
                        push             rax;                                 jmp   n22_match_alternate_s0
.Lx39_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n22_match_alternate_af
n24_match_defer_β:      mov              r11, 15
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx39_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx39_12
                                                                              jmp   rax
.Lx39_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n25_match_defer_α:      mov              r11, 16
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 2
                        lea              rdx, [rip + .S7]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx40_16
                        mov              rax, qword ptr [rdx + 0]
.Lx40_16:               test             rax, rax;                            jz    .Lx40_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx40_5]
                        push             rcx
                        lea              rcx, [rip + .Lx40_4]
                        push             rcx;                                 jmp   rax
.Lx40_4:                                                                      jmp   n21_match_alternate_s1
.Lx40_5:                                                                      jmp   n21_match_alternate_af
.Lx40_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 2
                        lea              rdx, [rip + .S7]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n21_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx40_6]
                        push             rcx
                        push             rax;                                 jmp   n21_match_alternate_s1
.Lx40_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n21_match_alternate_af
n25_match_defer_β:      mov              r11, 16
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx40_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx40_12
                                                                              jmp   rax
.Lx40_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n26_match_defer_α:      mov              r11, 17
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 1
                        lea              rdx, [rip + .S8]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx41_16
                        mov              rax, qword ptr [rdx + 0]
.Lx41_16:               test             rax, rax;                            jz    .Lx41_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx41_5]
                        push             rcx
                        lea              rcx, [rip + .Lx41_4]
                        push             rcx;                                 jmp   rax
.Lx41_4:                                                                      jmp   n21_match_alternate_s0
.Lx41_5:                                                                      jmp   n21_match_alternate_af
.Lx41_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 1
                        lea              rdx, [rip + .S8]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n21_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx41_6]
                        push             rcx
                        push             rax;                                 jmp   n21_match_alternate_s0
.Lx41_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n21_match_alternate_af
n26_match_defer_β:      mov              r11, 17
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx41_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx41_12
                                                                              jmp   rax
.Lx41_12:                                                                     jmp   qword ptr [rsp]
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
n42_match_assign_save_α:
                        sub              rsp, 16
                        mov              r11, 18
                        mov              dword ptr [rsp + 0], r14d;           jmp   n43_match_any_α
n42_match_assign_save_β:
                        mov              r11, 18
                        add              rsp, 16;                             jmp   PAT$3_ω
#-----------------------------------------------------------------------------------------------------------------------
n43_match_any_α:        mov              r11, 19
                        mov              eax, r14d
                        cmp              eax, r15d;                           jl    .Lx48_240
                        add              rsp, 16;                             jmp   PAT$3_ω
.Lx48_240:              movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 43;                             je    .Lx48_0
                        cmp              esi, 45;                             je    .Lx48_0
                        add              rsp, 16;                             jmp   PAT$3_ω
.Lx48_0:                add              r14d, 1;                             jmp   n44_match_assign_cond_α
n43_match_any_β:        mov              r11, 19
                        sub              r14d, 1
                        add              rsp, 16;                             jmp   PAT$3_ω
#-----------------------------------------------------------------------------------------------------------------------
n44_match_assign_cond_α:
                        mov              r11, 20
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S9]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   PAT$3_γ
n44_match_assign_cond_β:
                        mov              r11, 20
                        sub              r12, 24;                             jmp   n43_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$3_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$3_β:
                                                                              jmp   n44_match_assign_cond_β
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
n51_match_assign_save_α:
                        sub              rsp, 16
                        mov              r11, 21
                        mov              dword ptr [rsp + 0], r14d;           jmp   n52_match_any_α
n51_match_assign_save_β:
                        mov              r11, 21
                        add              rsp, 16;                             jmp   PAT$4_ω
#-----------------------------------------------------------------------------------------------------------------------
n52_match_any_α:        mov              r11, 22
                        mov              eax, r14d
                        cmp              eax, r15d;                           jl    .Lx57_240
                        add              rsp, 16;                             jmp   PAT$4_ω
.Lx57_240:              movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 42;                             je    .Lx57_0
                        cmp              esi, 47;                             je    .Lx57_0
                        add              rsp, 16;                             jmp   PAT$4_ω
.Lx57_0:                add              r14d, 1;                             jmp   n53_match_assign_cond_α
n52_match_any_β:        mov              r11, 22
                        sub              r14d, 1
                        add              rsp, 16;                             jmp   PAT$4_ω
#-----------------------------------------------------------------------------------------------------------------------
n53_match_assign_cond_α:
                        mov              r11, 23
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S9]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   PAT$4_γ
n53_match_assign_cond_β:
                        mov              r11, 23
                        sub              r12, 24;                             jmp   n52_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$4_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$4_β:
                                                                              jmp   n53_match_assign_cond_β
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
n60_match_assign_save_α:
                        sub              rsp, 16
                        mov              r11, 24
                        mov              dword ptr [rbp + -48], r14d;         jmp   n61_match_alternate_α
n60_match_assign_save_β:
                        mov              r11, 24
                        add              rsp, 16;                             jmp   PAT$5_ω
#-----------------------------------------------------------------------------------------------------------------------
n61_match_alternate_α:  mov              r11, 25
                        mov              dword ptr [rbp + -88], r14d
                        lea              rax, [rip + .Lx68_21]
                        mov              qword ptr [rbp + -72], rax;          jmp   n64_match_defer_α
.Lx68_21:               lea              rax, [rip + .Lx68_19]
                        mov              qword ptr [rbp + -72], rax;          jmp   n63_match_defer_α
n61_match_alternate_s0: mov              r11, 25
                        lea              rax, [rip + .Lx68_40]
                        mov              qword ptr [rbp + -80], rax;          jmp   n61_match_alternate_as
n61_match_alternate_s1: mov              r11, 25
                        lea              rax, [rip + .Lx68_41]
                        mov              qword ptr [rbp + -80], rax;          jmp   n61_match_alternate_as
.Lx68_40:                                                                     jmp   n64_match_defer_β
.Lx68_41:                                                                     jmp   n63_match_defer_β
n61_match_alternate_as: mov              r11, 25;                             jmp   n62_match_assign_cond_α
n61_match_alternate_β:  mov              r11, 25
                        mov              rax, qword ptr [rbp + -80];          jmp   rax
n61_match_alternate_af: mov              r11, 25
                        mov              r14d, dword ptr [rbp + -88]
                        mov              rax, qword ptr [rbp + -72];          jmp   rax
.Lx68_19:                                                                     jmp   n60_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n62_match_assign_cond_α:
                        mov              r11, 26
                        mov              eax, dword ptr [rbp + -48]
                        lea              rcx, [rip + .S9]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   PAT$5_γ
n62_match_assign_cond_β:
                        mov              r11, 26
                        sub              r12, 24;                             jmp   n61_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n63_match_defer_α:      mov              r11, 27
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 1
                        lea              rdx, [rip + .S10]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx71_16
                        mov              rax, qword ptr [rdx + 0]
.Lx71_16:               test             rax, rax;                            jz    .Lx71_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx71_5]
                        push             rcx
                        lea              rcx, [rip + .Lx71_4]
                        push             rcx;                                 jmp   rax
.Lx71_4:                                                                      jmp   n61_match_alternate_s1
.Lx71_5:                                                                      jmp   n61_match_alternate_af
.Lx71_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 1
                        lea              rdx, [rip + .S10]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n61_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx71_6]
                        push             rcx
                        push             rax;                                 jmp   n61_match_alternate_s1
.Lx71_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n61_match_alternate_af
n63_match_defer_β:      mov              r11, 27
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx71_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx71_12
                                                                              jmp   rax
.Lx71_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n64_match_defer_α:      mov              r11, 28
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S11]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx72_16
                        mov              rax, qword ptr [rdx + 0]
.Lx72_16:               test             rax, rax;                            jz    .Lx72_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx72_5]
                        push             rcx
                        lea              rcx, [rip + .Lx72_4]
                        push             rcx;                                 jmp   rax
.Lx72_4:                                                                      jmp   n61_match_alternate_s0
.Lx72_5:                                                                      jmp   n61_match_alternate_af
.Lx72_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S11]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n61_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx72_6]
                        push             rcx
                        push             rax;                                 jmp   n61_match_alternate_s0
.Lx72_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n61_match_alternate_af
n64_match_defer_β:      mov              r11, 28
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx72_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx72_12
                                                                              jmp   rax
.Lx72_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
PAT$5_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$5_β:
                                                                              jmp   n62_match_assign_cond_β
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
                        sub              rsp, 72
                        mov              rcx, qword ptr [rbp + 8]
                        mov              qword ptr [rbp + -8], rcx
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        mov              qword ptr [rbp + -32], r12
#-----------------------------------------------------------------------------------------------------------------------
n73_match_alternate_α:  mov              r11, 29
                        mov              dword ptr [rbp + -72], r14d
                        lea              rax, [rip + .Lx79_21]
                        mov              qword ptr [rbp + -56], rax;          jmp   n77_match_defer_α
.Lx79_21:               lea              rax, [rip + .Lx79_19]
                        mov              qword ptr [rbp + -56], rax;          jmp   n74_match_lit_α
n73_match_alternate_s0: mov              r11, 29
                        lea              rax, [rip + .Lx79_40]
                        mov              qword ptr [rbp + -64], rax;          jmp   n73_match_alternate_as
n73_match_alternate_s1: mov              r11, 29
                        lea              rax, [rip + .Lx79_41]
                        mov              qword ptr [rbp + -64], rax;          jmp   n73_match_alternate_as
.Lx79_40:                                                                     jmp   n77_match_defer_β
.Lx79_41:                                                                     jmp   n76_match_lit_β
n73_match_alternate_as: mov              r11, 29;                             jmp   PAT$6_γ
n73_match_alternate_β:  mov              r11, 29
                        mov              rax, qword ptr [rbp + -64];          jmp   rax
n73_match_alternate_af: mov              r11, 29
                        mov              r14d, dword ptr [rbp + -72]
                        mov              rax, qword ptr [rbp + -56];          jmp   rax
.Lx79_19:                                                                     jmp   PAT$6_ω
#-----------------------------------------------------------------------------------------------------------------------
n74_match_lit_α:        mov              r11, 30
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n73_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 40;                             jne   n73_match_alternate_af
                        add              r14d, 1;                             jmp   n75_match_defer_α
n74_match_lit_β:        mov              r11, 30
                        sub              r14d, 1;                             jmp   n73_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n75_match_defer_α:      mov              r11, 31
                        mov              rax, qword ptr [r9 + 336]            # expr
                        mov              rdx, qword ptr [r9 + 344]
                        cmp              al, 8;                               jne   .Lx82_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx82_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, qword ptr [r9 + 344];           jmp   .Lx82_10
.Lx82_9:                cmp              al, 88;                              jne   .Lx82_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx82_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx82_10
.Lx82_21:               xor              eax, eax
.Lx82_10:               test             rax, rax;                            jz    .Lx82_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx82_5]
                        push             rcx
                        lea              rcx, [rip + .Lx82_4]
                        push             rcx;                                 jmp   rax
.Lx82_4:                                                                      jmp   n76_match_lit_α
.Lx82_5:                                                                      jmp   n74_match_lit_β
.Lx82_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S12]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n74_match_lit_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx82_6]
                        push             rcx
                        push             rax;                                 jmp   n76_match_lit_α
.Lx82_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n74_match_lit_β
n75_match_defer_β:      mov              r11, 31
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx82_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx82_12
                                                                              jmp   rax
.Lx82_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n76_match_lit_α:        mov              r11, 32
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n75_match_defer_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 41;                             jne   n75_match_defer_β
                        add              r14d, 1;                             jmp   n73_match_alternate_s1
n76_match_lit_β:        mov              r11, 32
                        sub              r14d, 1;                             jmp   n75_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n77_match_defer_α:      mov              r11, 33
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S13]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx85_16
                        mov              rax, qword ptr [rdx + 0]
.Lx85_16:               test             rax, rax;                            jz    .Lx85_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx85_5]
                        push             rcx
                        lea              rcx, [rip + .Lx85_4]
                        push             rcx;                                 jmp   rax
.Lx85_4:                                                                      jmp   n73_match_alternate_s0
.Lx85_5:                                                                      jmp   n73_match_alternate_af
.Lx85_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S13]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n73_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx85_6]
                        push             rcx
                        push             rax;                                 jmp   n73_match_alternate_s0
.Lx85_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n73_match_alternate_af
n77_match_defer_β:      mov              r11, 33
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx85_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx85_12
                                                                              jmp   rax
.Lx85_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
PAT$6_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$6_β:
                                                                              jmp   n73_match_alternate_β
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
                        sub              rsp, 120
                        mov              rcx, qword ptr [rbp + 8]
                        mov              qword ptr [rbp + -8], rcx
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        mov              qword ptr [rbp + -32], r12
#-----------------------------------------------------------------------------------------------------------------------
n86_match_alternate_α:  mov              r11, 34
                        mov              dword ptr [rbp + -120], r14d
                        lea              rax, [rip + .Lx93_21]
                        mov              qword ptr [rbp + -104], rax;         jmp   n88_match_defer_α
.Lx93_21:               lea              rax, [rip + .Lx93_19]
                        mov              qword ptr [rbp + -104], rax;         jmp   n87_match_defer_α
n86_match_alternate_s0: mov              r11, 34
                        lea              rax, [rip + .Lx93_40]
                        mov              qword ptr [rbp + -112], rax;         jmp   n86_match_alternate_as
n86_match_alternate_s1: mov              r11, 34
                        lea              rax, [rip + .Lx93_41]
                        mov              qword ptr [rbp + -112], rax;         jmp   n86_match_alternate_as
.Lx93_40:                                                                     jmp   n91_match_assign_cond_β
.Lx93_41:                                                                     jmp   n87_match_defer_β
n86_match_alternate_as: mov              r11, 34;                             jmp   PAT$7_γ
n86_match_alternate_β:  mov              r11, 34
                        mov              rax, qword ptr [rbp + -112];         jmp   rax
n86_match_alternate_af: mov              r11, 34
                        mov              r14d, dword ptr [rbp + -120]
                        mov              rax, qword ptr [rbp + -104];         jmp   rax
.Lx93_19:                                                                     jmp   PAT$7_ω
#-----------------------------------------------------------------------------------------------------------------------
n87_match_defer_α:      mov              r11, 35
                        mov              rax, qword ptr [r9 + 288]            # primary
                        mov              rdx, qword ptr [r9 + 296]
                        cmp              al, 8;                               jne   .Lx94_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx94_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, qword ptr [r9 + 296];           jmp   .Lx94_10
.Lx94_9:                cmp              al, 88;                              jne   .Lx94_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx94_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx94_10
.Lx94_21:               xor              eax, eax
.Lx94_10:               test             rax, rax;                            jz    .Lx94_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx94_5]
                        push             rcx
                        lea              rcx, [rip + .Lx94_4]
                        push             rcx;                                 jmp   rax
.Lx94_4:                                                                      jmp   n86_match_alternate_s1
.Lx94_5:                                                                      jmp   n86_match_alternate_af
.Lx94_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S14]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n86_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx94_6]
                        push             rcx
                        push             rax;                                 jmp   n86_match_alternate_s1
.Lx94_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n86_match_alternate_af
n87_match_defer_β:      mov              r11, 35
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx94_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx94_12
                                                                              jmp   rax
.Lx94_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n88_match_defer_α:      mov              r11, 36
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S15]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx95_16
                        mov              rax, qword ptr [rdx + 0]
.Lx95_16:               test             rax, rax;                            jz    .Lx95_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx95_5]
                        push             rcx
                        lea              rcx, [rip + .Lx95_4]
                        push             rcx;                                 jmp   rax
.Lx95_4:                                                                      jmp   n89_match_assign_save_α
.Lx95_5:                                                                      jmp   n86_match_alternate_af
.Lx95_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S15]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n86_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx95_6]
                        push             rcx
                        push             rax;                                 jmp   n89_match_assign_save_α
.Lx95_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n86_match_alternate_af
n88_match_defer_β:      mov              r11, 36
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx95_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx95_12
                                                                              jmp   rax
.Lx95_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n89_match_assign_save_α:
                        mov              r11, 37
                        mov              dword ptr [rbp + -64], r14d;         jmp   n90_match_defer_α
n89_match_assign_save_β:
                        mov              r11, 37;                             jmp   n88_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n90_match_defer_α:      mov              r11, 38
                        mov              rax, qword ptr [r9 + 304]            # factor
                        mov              rdx, qword ptr [r9 + 312]
                        cmp              al, 8;                               jne   .Lx98_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx98_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, qword ptr [r9 + 312];           jmp   .Lx98_10
.Lx98_9:                cmp              al, 88;                              jne   .Lx98_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx98_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx98_10
.Lx98_21:               xor              eax, eax
.Lx98_10:               test             rax, rax;                            jz    .Lx98_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx98_5]
                        push             rcx
                        lea              rcx, [rip + .Lx98_4]
                        push             rcx;                                 jmp   rax
.Lx98_4:                                                                      jmp   n91_match_assign_cond_α
.Lx98_5:                                                                      jmp   n89_match_assign_save_β
.Lx98_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S16]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n89_match_assign_save_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx98_6]
                        push             rcx
                        push             rax;                                 jmp   n91_match_assign_cond_α
.Lx98_6:                add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n89_match_assign_save_β
n90_match_defer_β:      mov              r11, 38
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx98_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx98_12
                                                                              jmp   rax
.Lx98_12:                                                                     jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n91_match_assign_cond_α:
                        mov              r11, 39
                        mov              eax, dword ptr [rbp + -64]
                        lea              rcx, [rip + .S17]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n86_match_alternate_s0
n91_match_assign_cond_β:
                        mov              r11, 39
                        sub              r12, 24;                             jmp   n90_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$7_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$7_β:
                                                                              jmp   n86_match_alternate_β
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
                        sub              rsp, 120
                        mov              rcx, qword ptr [rbp + 8]
                        mov              qword ptr [rbp + -8], rcx
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        mov              qword ptr [rbp + -32], r12
#-----------------------------------------------------------------------------------------------------------------------
n101_match_alternate_α: mov              r11, 40
                        mov              dword ptr [rbp + -120], r14d
                        lea              rax, [rip + .Lx109_21]
                        mov              qword ptr [rbp + -104], rax;         jmp   n103_match_defer_α
.Lx109_21:              lea              rax, [rip + .Lx109_19]
                        mov              qword ptr [rbp + -104], rax;         jmp   n102_match_defer_α
n101_match_alternate_s0:
                        mov              r11, 40
                        lea              rax, [rip + .Lx109_40]
                        mov              qword ptr [rbp + -112], rax;         jmp   n101_match_alternate_as
n101_match_alternate_s1:
                        mov              r11, 40
                        lea              rax, [rip + .Lx109_41]
                        mov              qword ptr [rbp + -112], rax;         jmp   n101_match_alternate_as
.Lx109_40:                                                                    jmp   n107_match_assign_cond_β
.Lx109_41:                                                                    jmp   n102_match_defer_β
n101_match_alternate_as:
                        mov              r11, 40;                             jmp   PAT$8_γ
n101_match_alternate_β: mov              r11, 40
                        mov              rax, qword ptr [rbp + -112];         jmp   rax
n101_match_alternate_af:
                        mov              r11, 40
                        mov              r14d, dword ptr [rbp + -120]
                        mov              rax, qword ptr [rbp + -104];         jmp   rax
.Lx109_19:                                                                    jmp   PAT$8_ω
#-----------------------------------------------------------------------------------------------------------------------
n102_match_defer_α:     mov              r11, 41
                        mov              rax, qword ptr [r9 + 304]            # factor
                        mov              rdx, qword ptr [r9 + 312]
                        cmp              al, 8;                               jne   .Lx110_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx110_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, qword ptr [r9 + 312];           jmp   .Lx110_10
.Lx110_9:               cmp              al, 88;                              jne   .Lx110_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx110_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx110_10
.Lx110_21:              xor              eax, eax
.Lx110_10:              test             rax, rax;                            jz    .Lx110_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx110_5]
                        push             rcx
                        lea              rcx, [rip + .Lx110_4]
                        push             rcx;                                 jmp   rax
.Lx110_4:                                                                     jmp   n101_match_alternate_s1
.Lx110_5:                                                                     jmp   n101_match_alternate_af
.Lx110_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S16]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n101_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx110_6]
                        push             rcx
                        push             rax;                                 jmp   n101_match_alternate_s1
.Lx110_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n101_match_alternate_af
n102_match_defer_β:     mov              r11, 41
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx110_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx110_12
                                                                              jmp   rax
.Lx110_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n103_match_defer_α:     mov              r11, 42
                        mov              rax, qword ptr [r9 + 304]            # factor
                        mov              rdx, qword ptr [r9 + 312]
                        cmp              al, 8;                               jne   .Lx111_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx111_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, qword ptr [r9 + 312];           jmp   .Lx111_10
.Lx111_9:               cmp              al, 88;                              jne   .Lx111_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx111_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx111_10
.Lx111_21:              xor              eax, eax
.Lx111_10:              test             rax, rax;                            jz    .Lx111_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx111_5]
                        push             rcx
                        lea              rcx, [rip + .Lx111_4]
                        push             rcx;                                 jmp   rax
.Lx111_4:                                                                     jmp   n104_match_defer_α
.Lx111_5:                                                                     jmp   n101_match_alternate_af
.Lx111_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S16]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n101_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx111_6]
                        push             rcx
                        push             rax;                                 jmp   n104_match_defer_α
.Lx111_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n101_match_alternate_af
n103_match_defer_β:     mov              r11, 42
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx111_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx111_12
                                                                              jmp   rax
.Lx111_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n104_match_defer_α:     mov              r11, 43
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S18]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx112_16
                        mov              rax, qword ptr [rdx + 0]
.Lx112_16:              test             rax, rax;                            jz    .Lx112_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx112_5]
                        push             rcx
                        lea              rcx, [rip + .Lx112_4]
                        push             rcx;                                 jmp   rax
.Lx112_4:                                                                     jmp   n105_match_assign_save_α
.Lx112_5:                                                                     jmp   n103_match_defer_β
.Lx112_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S18]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n103_match_defer_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx112_6]
                        push             rcx
                        push             rax;                                 jmp   n105_match_assign_save_α
.Lx112_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n103_match_defer_β
n104_match_defer_β:     mov              r11, 43
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx112_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx112_12
                                                                              jmp   rax
.Lx112_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n105_match_assign_save_α:
                        mov              r11, 44
                        mov              dword ptr [rbp + -64], r14d;         jmp   n106_match_defer_α
n105_match_assign_save_β:
                        mov              r11, 44;                             jmp   n104_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n106_match_defer_α:     mov              r11, 45
                        mov              rax, qword ptr [r9 + 320]            # term
                        mov              rdx, qword ptr [r9 + 328]
                        cmp              al, 8;                               jne   .Lx115_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx115_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, qword ptr [r9 + 328];           jmp   .Lx115_10
.Lx115_9:               cmp              al, 88;                              jne   .Lx115_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx115_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx115_10
.Lx115_21:              xor              eax, eax
.Lx115_10:              test             rax, rax;                            jz    .Lx115_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx115_5]
                        push             rcx
                        lea              rcx, [rip + .Lx115_4]
                        push             rcx;                                 jmp   rax
.Lx115_4:                                                                     jmp   n107_match_assign_cond_α
.Lx115_5:                                                                     jmp   n105_match_assign_save_β
.Lx115_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S19]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n105_match_assign_save_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx115_6]
                        push             rcx
                        push             rax;                                 jmp   n107_match_assign_cond_α
.Lx115_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n105_match_assign_save_β
n106_match_defer_β:     mov              r11, 45
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx115_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx115_12
                                                                              jmp   rax
.Lx115_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n107_match_assign_cond_α:
                        mov              r11, 46
                        mov              eax, dword ptr [rbp + -64]
                        lea              rcx, [rip + .S20]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n101_match_alternate_s0
n107_match_assign_cond_β:
                        mov              r11, 46
                        sub              r12, 24;                             jmp   n106_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$8_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$8_β:
                                                                              jmp   n101_match_alternate_β
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
                        sub              rsp, 120
                        mov              rcx, qword ptr [rbp + 8]
                        mov              qword ptr [rbp + -8], rcx
                        mov              rcx, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        mov              qword ptr [rbp + -32], r12
#-----------------------------------------------------------------------------------------------------------------------
n118_match_alternate_α: mov              r11, 47
                        mov              dword ptr [rbp + -120], r14d
                        lea              rax, [rip + .Lx126_21]
                        mov              qword ptr [rbp + -104], rax;         jmp   n120_match_defer_α
.Lx126_21:              lea              rax, [rip + .Lx126_19]
                        mov              qword ptr [rbp + -104], rax;         jmp   n119_match_defer_α
n118_match_alternate_s0:
                        mov              r11, 47
                        lea              rax, [rip + .Lx126_40]
                        mov              qword ptr [rbp + -112], rax;         jmp   n118_match_alternate_as
n118_match_alternate_s1:
                        mov              r11, 47
                        lea              rax, [rip + .Lx126_41]
                        mov              qword ptr [rbp + -112], rax;         jmp   n118_match_alternate_as
.Lx126_40:                                                                    jmp   n124_match_assign_cond_β
.Lx126_41:                                                                    jmp   n119_match_defer_β
n118_match_alternate_as:
                        mov              r11, 47;                             jmp   PAT$9_γ
n118_match_alternate_β: mov              r11, 47
                        mov              rax, qword ptr [rbp + -112];         jmp   rax
n118_match_alternate_af:
                        mov              r11, 47
                        mov              r14d, dword ptr [rbp + -120]
                        mov              rax, qword ptr [rbp + -104];         jmp   rax
.Lx126_19:                                                                    jmp   PAT$9_ω
#-----------------------------------------------------------------------------------------------------------------------
n119_match_defer_α:     mov              r11, 48
                        mov              rax, qword ptr [r9 + 320]            # term
                        mov              rdx, qword ptr [r9 + 328]
                        cmp              al, 8;                               jne   .Lx127_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx127_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, qword ptr [r9 + 328];           jmp   .Lx127_10
.Lx127_9:               cmp              al, 88;                              jne   .Lx127_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx127_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx127_10
.Lx127_21:              xor              eax, eax
.Lx127_10:              test             rax, rax;                            jz    .Lx127_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx127_5]
                        push             rcx
                        lea              rcx, [rip + .Lx127_4]
                        push             rcx;                                 jmp   rax
.Lx127_4:                                                                     jmp   n118_match_alternate_s1
.Lx127_5:                                                                     jmp   n118_match_alternate_af
.Lx127_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S19]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n118_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx127_6]
                        push             rcx
                        push             rax;                                 jmp   n118_match_alternate_s1
.Lx127_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n118_match_alternate_af
n119_match_defer_β:     mov              r11, 48
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx127_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx127_12
                                                                              jmp   rax
.Lx127_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n120_match_defer_α:     mov              r11, 49
                        mov              rax, qword ptr [r9 + 320]            # term
                        mov              rdx, qword ptr [r9 + 328]
                        cmp              al, 8;                               jne   .Lx128_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx128_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, qword ptr [r9 + 328];           jmp   .Lx128_10
.Lx128_9:               cmp              al, 88;                              jne   .Lx128_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx128_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx128_10
.Lx128_21:              xor              eax, eax
.Lx128_10:              test             rax, rax;                            jz    .Lx128_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx128_5]
                        push             rcx
                        lea              rcx, [rip + .Lx128_4]
                        push             rcx;                                 jmp   rax
.Lx128_4:                                                                     jmp   n121_match_defer_α
.Lx128_5:                                                                     jmp   n118_match_alternate_af
.Lx128_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S19]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n118_match_alternate_af
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx128_6]
                        push             rcx
                        push             rax;                                 jmp   n121_match_defer_α
.Lx128_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n118_match_alternate_af
n120_match_defer_β:     mov              r11, 49
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx128_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx128_12
                                                                              jmp   rax
.Lx128_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n121_match_defer_α:     mov              r11, 50
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S21]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx129_16
                        mov              rax, qword ptr [rdx + 0]
.Lx129_16:              test             rax, rax;                            jz    .Lx129_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx129_5]
                        push             rcx
                        lea              rcx, [rip + .Lx129_4]
                        push             rcx;                                 jmp   rax
.Lx129_4:                                                                     jmp   n122_match_assign_save_α
.Lx129_5:                                                                     jmp   n120_match_defer_β
.Lx129_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -24]
                        mov              esi, 0
                        lea              rdx, [rip + .S21]
                        mov              ecx, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_patv_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n120_match_defer_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx129_6]
                        push             rcx
                        push             rax;                                 jmp   n122_match_assign_save_α
.Lx129_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n120_match_defer_β
n121_match_defer_β:     mov              r11, 50
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx129_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx129_12
                                                                              jmp   rax
.Lx129_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n122_match_assign_save_α:
                        mov              r11, 51
                        mov              dword ptr [rbp + -64], r14d;         jmp   n123_match_defer_α
n122_match_assign_save_β:
                        mov              r11, 51;                             jmp   n121_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n123_match_defer_α:     mov              r11, 52
                        mov              rax, qword ptr [r9 + 336]            # expr
                        mov              rdx, qword ptr [r9 + 344]
                        cmp              al, 8;                               jne   .Lx132_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx132_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, qword ptr [r9 + 344];           jmp   .Lx132_10
.Lx132_9:               cmp              al, 88;                              jne   .Lx132_21
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_xpat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx132_21
                        mov              rax, qword ptr [rdx + 0];            jmp   .Lx132_10
.Lx132_21:              xor              eax, eax
.Lx132_10:              test             rax, rax;                            jz    .Lx132_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx132_5]
                        push             rcx
                        lea              rcx, [rip + .Lx132_4]
                        push             rcx;                                 jmp   rax
.Lx132_4:                                                                     jmp   n124_match_assign_cond_α
.Lx132_5:                                                                     jmp   n122_match_assign_save_β
.Lx132_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S12]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n122_match_assign_save_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx132_6]
                        push             rcx
                        push             rax;                                 jmp   n124_match_assign_cond_α
.Lx132_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n122_match_assign_save_β
n123_match_defer_β:     mov              r11, 52
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx132_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx132_12
                                                                              jmp   rax
.Lx132_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n124_match_assign_cond_α:
                        mov              r11, 53
                        mov              eax, dword ptr [rbp + -64]
                        lea              rcx, [rip + .S20]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n118_match_alternate_s0
n124_match_assign_cond_β:
                        mov              r11, 53
                        sub              r12, 24;                             jmp   n123_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
PAT$9_res:
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
PAT$9_β:
                                                                              jmp   n118_match_alternate_β
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
n135_statement_begin_α: mov              r11, 54
                        mov              r10, 0;                              jmp   n136_statement_end_α
n135_statement_begin_β: mov              r11, 54;                             jmp   n137_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n136_statement_end_α:   mov              r11, 55
                        mov              r10, 1;                              jmp   n137_statement_begin_α
#=======================================================================================================================
#          DEFINE('Push(x)')
#-----------------------------------------------------------------------------------------------------------------------
n137_statement_begin_α: mov              r11, 56
                        mov              r10, 2;                              jmp   n138_define_α
n137_statement_begin_β: mov              r11, 56;                             jmp   n140_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n138_define_α:          mov              r11, 57
                        mov              rdi, qword ptr [rip + .Lx401_0]
                        mov              rsi, qword ptr [rip + .Lx401_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n144_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx401_0]
                        lea              rsi, [rip + Push_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n139_statement_end_α
n138_define_β:          mov              r11, 57;                             jmp   n137_statement_begin_β
.Lx401_0:               .quad            .Lx401_0_s
.Lx401_0_s:             .string          "Push"
.Lx401_1:               .quad            .Lx401_1_s
.Lx401_1_s:             .string          "x"
                                                                              jmp   .Lx402_245
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
                        cmp              rdx, 0;                              jbe   .Lx402_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # x
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx402_41
.Lx402_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx402_41:              lea              rcx, [rip + Push_γ]
                        lea              rax, [rip + Push_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n144_statement_begin_α]; jmp   rax
Push_γ:                 mov              rdi, qword ptr [r9 + 0]              # Push
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rdx, 0;                              jbe   .Lx402_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # x
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx402_110
.Lx402_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx402_110:             mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx402_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # x
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx402_180
.Lx402_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx402_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx402_245:
#-----------------------------------------------------------------------------------------------------------------------
n139_statement_end_α:   mov              r11, 58
                        mov              r10, 2;                              jmp   n140_statement_begin_α
#=======================================================================================================================
#          stk      =  TABLE()                       :(PushEnd)
#-----------------------------------------------------------------------------------------------------------------------
n140_statement_begin_α: mov              r11, 59
                        mov              r10, 3;                              jmp   n141_call_α
n140_statement_begin_β: mov              r11, 59;                             jmp   n171_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n141_call_α:            sub              rsp, 16
                        mov              r11, 60
                        .section         .rodata
.Lrkfnzd408:            .string          "TABLE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd408]
                        xor              esi, esi
                        mov              edx, 0
                        mov              ecx, 327748
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx407_240
                        add              rsp, 16;                             jmp   n140_statement_begin_β
.Lx407_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n142_assign_α
n141_call_β:            mov              r11, 60
                        add              rsp, 16;                             jmp   n140_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n142_assign_α:          mov              r11, 61
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # stk
                        mov              qword ptr [r9 + 152], rdx;           jmp   n143_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n143_statement_end_α:   mov              r11, 62
                        mov              r10, 3
                        add              rsp, 16;                             jmp   n171_statement_begin_α
#=======================================================================================================================
# Push     stk[0]   =  stk[0] + 1
#-----------------------------------------------------------------------------------------------------------------------
n144_statement_begin_α: mov              r11, 63
                        mov              r10, 4;                              jmp   n145_var_α
n144_statement_begin_β: mov              r11, 63;                             jmp   n156_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n145_var_α:             sub              rsp, 16
                        mov              r11, 64
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n146_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n146_lit_integer_α:     sub              rsp, 16
                        mov              r11, 65
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx415_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n147_subscript_α
n146_lit_integer_β:     mov              r11, 65
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n144_statement_begin_β
.Lx415_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n147_subscript_α:       sub              rsp, 16
                        mov              r11, 66
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_subscript_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx416_240
                        add              rsp, 16;                             jmp   n146_lit_integer_β
.Lx416_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n148_var_α
n147_subscript_β:       mov              r11, 66
                        add              rsp, 16;                             jmp   n146_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n148_var_α:             sub              rsp, 16
                        mov              r11, 67
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n149_lit_integer_α
n148_var_β:             mov              r11, 67
                        add              rsp, 16;                             jmp   n147_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n149_lit_integer_α:     sub              rsp, 16
                        mov              r11, 68
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx418_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n150_subscript_α
n149_lit_integer_β:     mov              r11, 68
                        add              rsp, 16;                             jmp   n148_var_β
.Lx418_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n150_subscript_α:       sub              rsp, 16
                        mov              r11, 69
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx419_240
                        add              rsp, 16;                             jmp   n149_lit_integer_β
.Lx419_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n151_deref_α
n150_subscript_β:       mov              r11, 69
                        add              rsp, 16;                             jmp   n149_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n151_deref_α:           sub              rsp, 16
                        mov              r11, 70
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx420_240
                        add              rsp, 16;                             jmp   n150_subscript_β
.Lx420_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n152_lit_integer_α
n151_deref_β:           mov              r11, 70
                        add              rsp, 16;                             jmp   n150_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n152_lit_integer_α:     sub              rsp, 16
                        mov              r11, 71
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx421_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n153_binop_α
n152_lit_integer_β:     mov              r11, 71
                        add              rsp, 16;                             jmp   n151_deref_β
.Lx421_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n153_binop_α:           sub              rsp, 16
                        mov              r11, 72
                        mov              ecx, dword ptr [rsp + 32]            # deref
                        mov              rax, qword ptr [rsp + 40]
                        cmp              cl, 3;                               jne   .Lx422_2
                        add              rax, 1
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx422_7
.Lx422_2:               mov              eax, ecx
                        mov              edx, ecx
                        and              edx, 1;                              jz    .Lx422_0
                        mov              rsi, qword ptr [rsp + 40]            # deref
                        mov              rdi, 1
                        cmp              al, 5;                               je    .Lx422_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx422_4
.Lx422_3:               movq             xmm0, rsi
.Lx422_4:               cvtsi2sd         xmm1, rdi
                        addsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx422_7:                                                                     jmp   n154_assign_var_α
.Lx422_0:               mov              rdi, qword ptr [rsp + 32]            # deref
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              al, 104;                             jne   .Lx422_240
                        add              rsp, 16;                             jmp   n152_lit_integer_β
.Lx422_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n154_assign_var_α
n153_binop_β:           mov              r11, 72
                        add              rsp, 16;                             jmp   n152_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n154_assign_var_α:      sub              rsp, 16
                        mov              r11, 73
                        mov              rdi, qword ptr [rsp + 112]           # subscript
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 16]            # binop
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx423_240
                        add              rsp, 16;                             jmp   n153_binop_β
.Lx423_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n155_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n155_statement_end_α:   mov              r11, 74
                        mov              r10, 4
                        add              rsp, 160;                            jmp   n156_statement_begin_α
#=======================================================================================================================
#          Push     =  .stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n156_statement_begin_α: mov              r11, 75
                        mov              r10, 5;                              jmp   n157_var_α
n156_statement_begin_β: mov              r11, 75;                             jmp   n165_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n157_var_α:             sub              rsp, 16
                        mov              r11, 76
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n158_var_α
#-----------------------------------------------------------------------------------------------------------------------
n158_var_α:             sub              rsp, 16
                        mov              r11, 77
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n159_lit_integer_α
n158_var_β:             mov              r11, 77
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n156_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n159_lit_integer_α:     sub              rsp, 16
                        mov              r11, 78
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx430_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n160_subscript_α
n159_lit_integer_β:     mov              r11, 78
                        add              rsp, 16;                             jmp   n158_var_β
.Lx430_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n160_subscript_α:       sub              rsp, 16
                        mov              r11, 79
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx431_240
                        add              rsp, 16;                             jmp   n159_lit_integer_β
.Lx431_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n161_deref_α
n160_subscript_β:       mov              r11, 79
                        add              rsp, 16;                             jmp   n159_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n161_deref_α:           sub              rsp, 16
                        mov              r11, 80
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx432_240
                        add              rsp, 16;                             jmp   n160_subscript_β
.Lx432_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n162_subscript_α
n161_deref_β:           mov              r11, 80
                        add              rsp, 16;                             jmp   n160_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n162_subscript_α:       sub              rsp, 16
                        mov              r11, 81
                        mov              rdi, qword ptr [rsp + 80]            # var
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]            # deref
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_subscript_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx433_240
                        add              rsp, 16;                             jmp   n161_deref_β
.Lx433_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n163_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n163_assign_α:          mov              r11, 82
                        mov              rax, qword ptr [rsp + 0]             # subscript
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # Push
                        mov              qword ptr [r9 + 8], rdx;             jmp   n164_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n164_statement_end_α:   mov              r11, 83
                        mov              r10, 5
                        add              rsp, 96;                             jmp   n165_statement_begin_α
#=======================================================================================================================
#          $Push    =  x                             :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n165_statement_begin_α: mov              r11, 84
                        mov              r10, 6;                              jmp   n166_var_α
n165_statement_begin_β: mov              r11, 84;                             jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n166_var_α:             sub              rsp, 16
                        mov              r11, 85
                        mov              rax, qword ptr [r9 + 0]              # Push
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n167_call_α
#-----------------------------------------------------------------------------------------------------------------------
n167_call_α:            sub              rsp, 16
                        mov              r11, 86
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd441:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd441]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 524341
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx440_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n165_statement_begin_β
.Lx440_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n168_var_α
n167_call_β:            mov              r11, 86
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n165_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n168_var_α:             sub              rsp, 16
                        mov              r11, 87
                        mov              rax, qword ptr [r9 + 16]             # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n169_assign_var_α
n168_var_β:             mov              r11, 87
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n165_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n169_assign_var_α:      sub              rsp, 16
                        mov              r11, 88
                        mov              rdi, qword ptr [rsp + 32]            # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx443_240
                        add              rsp, 16;                             jmp   n168_var_β
.Lx443_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n170_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n170_statement_end_α:   mov              r11, 89
                        mov              r10, 6
                        add              rsp, 64;                             jmp   NRETURN
#=======================================================================================================================
# PushEnd  <stmt 7, line 14: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n171_statement_begin_α: mov              r11, 90
                        mov              r10, 7;                              jmp   n172_statement_end_α
n171_statement_begin_β: mov              r11, 90;                             jmp   n173_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n172_statement_end_α:   mov              r11, 91
                        mov              r10, 7;                              jmp   n173_statement_begin_α
#=======================================================================================================================
#          DEFINE('Pop()')                           :(PopEnd)
#-----------------------------------------------------------------------------------------------------------------------
n173_statement_begin_α: mov              r11, 92
                        mov              r10, 8;                              jmp   n174_define_α
n173_statement_begin_β: mov              r11, 92;                             jmp   n198_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n174_define_α:          mov              r11, 93
                        mov              rdi, qword ptr [rip + .Lx453_0]
                        mov              rsi, qword ptr [rip + .Lx453_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n176_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx453_0]
                        lea              rsi, [rip + Pop_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n175_statement_end_α
n174_define_β:          mov              r11, 93;                             jmp   n173_statement_begin_β
.Lx453_0:               .quad            .Lx453_0_s
.Lx453_0_s:             .string          "Pop"
.Lx453_1:               .quad            .Lx453_1_s
.Lx453_1_s:             .string          ""
                                                                              jmp   .Lx454_245
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
                        lea              rax, [rip + Pop_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n176_statement_begin_α]; jmp   rax
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
.Lx454_245:
#-----------------------------------------------------------------------------------------------------------------------
n175_statement_end_α:   mov              r11, 94
                        mov              r10, 8;                              jmp   n198_statement_begin_α
#=======================================================================================================================
# Pop      Pop      =  stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n176_statement_begin_α: mov              r11, 95
                        mov              r10, 9;                              jmp   n177_var_α
n176_statement_begin_β: mov              r11, 95;                             jmp   n186_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n177_var_α:             sub              rsp, 16
                        mov              r11, 96
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n178_var_α
#-----------------------------------------------------------------------------------------------------------------------
n178_var_α:             sub              rsp, 16
                        mov              r11, 97
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n179_lit_integer_α
n178_var_β:             mov              r11, 97
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n176_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n179_lit_integer_α:     sub              rsp, 16
                        mov              r11, 98
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx461_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n180_subscript_α
n179_lit_integer_β:     mov              r11, 98
                        add              rsp, 16;                             jmp   n178_var_β
.Lx461_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n180_subscript_α:       sub              rsp, 16
                        mov              r11, 99
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx462_240
                        add              rsp, 16;                             jmp   n179_lit_integer_β
.Lx462_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n181_deref_α
n180_subscript_β:       mov              r11, 99
                        add              rsp, 16;                             jmp   n179_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n181_deref_α:           sub              rsp, 16
                        mov              r11, 100
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx463_240
                        add              rsp, 16;                             jmp   n180_subscript_β
.Lx463_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n182_subscript_α
n181_deref_β:           mov              r11, 100
                        add              rsp, 16;                             jmp   n180_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n182_subscript_α:       sub              rsp, 16
                        mov              r11, 101
                        mov              rdi, qword ptr [rsp + 80]            # var
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]            # deref
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx464_240
                        add              rsp, 16;                             jmp   n181_deref_β
.Lx464_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n183_deref_α
n182_subscript_β:       mov              r11, 101
                        add              rsp, 16;                             jmp   n181_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n183_deref_α:           sub              rsp, 16
                        mov              r11, 102
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx465_240
                        add              rsp, 16;                             jmp   n182_subscript_β
.Lx465_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n184_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n184_assign_α:          mov              r11, 103
                        mov              rax, qword ptr [rsp + 0]             # deref
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax             # Pop
                        mov              qword ptr [r9 + 40], rdx;            jmp   n185_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n185_statement_end_α:   mov              r11, 104
                        mov              r10, 9
                        add              rsp, 112;                            jmp   n186_statement_begin_α
#=======================================================================================================================
#          stk[0]   =  stk[0] - 1                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n186_statement_begin_α: mov              r11, 105
                        mov              r10, 10;                             jmp   n187_var_α
n186_statement_begin_β: mov              r11, 105;                            jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n187_var_α:             sub              rsp, 16
                        mov              r11, 106
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n188_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n188_lit_integer_α:     sub              rsp, 16
                        mov              r11, 107
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx472_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n189_subscript_α
n188_lit_integer_β:     mov              r11, 107
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n186_statement_begin_β
.Lx472_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n189_subscript_α:       sub              rsp, 16
                        mov              r11, 108
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_subscript_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx473_240
                        add              rsp, 16;                             jmp   n188_lit_integer_β
.Lx473_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n190_var_α
n189_subscript_β:       mov              r11, 108
                        add              rsp, 16;                             jmp   n188_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n190_var_α:             sub              rsp, 16
                        mov              r11, 109
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n191_lit_integer_α
n190_var_β:             mov              r11, 109
                        add              rsp, 16;                             jmp   n189_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n191_lit_integer_α:     sub              rsp, 16
                        mov              r11, 110
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx475_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n192_subscript_α
n191_lit_integer_β:     mov              r11, 110
                        add              rsp, 16;                             jmp   n190_var_β
.Lx475_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n192_subscript_α:       sub              rsp, 16
                        mov              r11, 111
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx476_240
                        add              rsp, 16;                             jmp   n191_lit_integer_β
.Lx476_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n193_deref_α
n192_subscript_β:       mov              r11, 111
                        add              rsp, 16;                             jmp   n191_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n193_deref_α:           sub              rsp, 16
                        mov              r11, 112
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx477_240
                        add              rsp, 16;                             jmp   n192_subscript_β
.Lx477_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n194_lit_integer_α
n193_deref_β:           mov              r11, 112
                        add              rsp, 16;                             jmp   n192_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n194_lit_integer_α:     sub              rsp, 16
                        mov              r11, 113
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx478_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n195_binop_α
n194_lit_integer_β:     mov              r11, 113
                        add              rsp, 16;                             jmp   n193_deref_β
.Lx478_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n195_binop_α:           sub              rsp, 16
                        mov              r11, 114
                        mov              ecx, dword ptr [rsp + 32]            # deref
                        mov              rax, qword ptr [rsp + 40]
                        cmp              cl, 3;                               jne   .Lx479_2
                        sub              rax, 1
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx479_7
.Lx479_2:               mov              eax, ecx
                        mov              edx, ecx
                        and              edx, 1;                              jz    .Lx479_0
                        mov              rsi, qword ptr [rsp + 40]            # deref
                        mov              rdi, 1
                        cmp              al, 5;                               je    .Lx479_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx479_4
.Lx479_3:               movq             xmm0, rsi
.Lx479_4:               cvtsi2sd         xmm1, rdi
                        subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx479_7:                                                                     jmp   n196_assign_var_α
.Lx479_0:               mov              rdi, qword ptr [rsp + 32]            # deref
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_sub@PLT
                        cmp              al, 104;                             jne   .Lx479_240
                        add              rsp, 16;                             jmp   n194_lit_integer_β
.Lx479_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n196_assign_var_α
n195_binop_β:           mov              r11, 114
                        add              rsp, 16;                             jmp   n194_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n196_assign_var_α:      sub              rsp, 16
                        mov              r11, 115
                        mov              rdi, qword ptr [rsp + 112]           # subscript
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 16]            # binop
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx480_240
                        add              rsp, 16;                             jmp   n195_binop_β
.Lx480_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n197_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n197_statement_end_α:   mov              r11, 116
                        mov              r10, 10
                        add              rsp, 160;                            jmp   RETURN
#=======================================================================================================================
# PopEnd  <stmt 11, line 18: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n198_statement_begin_α: mov              r11, 117
                        mov              r10, 11;                             jmp   n199_statement_end_α
n198_statement_begin_β: mov              r11, 117;                            jmp   n200_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n199_statement_end_α:   mov              r11, 118
                        mov              r10, 11;                             jmp   n200_statement_begin_α
#=======================================================================================================================
#          DEFINE('Unary()arg,op')                   :(UnaryEnd)
#-----------------------------------------------------------------------------------------------------------------------
n200_statement_begin_α: mov              r11, 119
                        mov              r10, 12;                             jmp   n201_define_α
n200_statement_begin_β: mov              r11, 119;                            jmp   n226_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n201_define_α:          mov              r11, 120
                        mov              rdi, qword ptr [rip + .Lx490_0]
                        mov              rsi, qword ptr [rip + .Lx490_1]
                        mov              edx, 2
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n203_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx490_0]
                        lea              rsi, [rip + Unary_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n202_statement_end_α
n201_define_β:          mov              r11, 120;                            jmp   n200_statement_begin_β
.Lx490_0:               .quad            .Lx490_0_s
.Lx490_0_s:             .string          "Unary"
.Lx490_1:               .quad            .Lx490_1_s
.Lx490_1_s:             .string          "arg,op"
                                                                              jmp   .Lx491_245
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
                        cmp              rdx, 0;                              jbe   .Lx491_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 64]             # arg
                        mov              qword ptr [r9 + 64], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 72]
                        mov              qword ptr [r9 + 72], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx491_41
.Lx491_10:              mov              rax, qword ptr [r9 + 64]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
.Lx491_41:              cmp              rdx, 1;                              jbe   .Lx491_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 80]             # op
                        mov              qword ptr [r9 + 80], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 88]
                        mov              qword ptr [r9 + 88], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx491_42
.Lx491_11:              mov              rax, qword ptr [r9 + 80]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
.Lx491_42:              lea              rcx, [rip + Unary_γ]
                        lea              rax, [rip + Unary_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n203_statement_begin_α]; jmp   rax
Unary_γ:                mov              rdi, qword ptr [r9 + 48]             # Unary
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 56], rax
                        cmp              rdx, 0;                              jbe   .Lx491_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 64], rax             # arg
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx491_110
.Lx491_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 72], rax
.Lx491_110:             cmp              rdx, 1;                              jbe   .Lx491_81
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 88], rax;            jmp   .Lx491_111
.Lx491_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 88], rax
.Lx491_111:             mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx491_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 64], rax             # arg
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx491_180
.Lx491_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 72], rax
.Lx491_180:             cmp              rdx, 1;                              jbe   .Lx491_151
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 88], rax;            jmp   .Lx491_181
.Lx491_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 88], rax
.Lx491_181:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 80
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx491_245:
#-----------------------------------------------------------------------------------------------------------------------
n202_statement_end_α:   mov              r11, 121
                        mov              r10, 12;                             jmp   n226_statement_begin_α
#=======================================================================================================================
# Unary    arg      =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n203_statement_begin_α: mov              r11, 122
                        mov              r10, 13;                             jmp   n204_call_α
n203_statement_begin_β: mov              r11, 122;                            jmp   n207_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n204_call_α:            sub              rsp, 16
                        mov              r11, 123
                        lea              rcx, [rip + .Lsig497z]
                        lea              rax, [rip + Pop_α];                  jmp   rax
.Lsig497z:              .quad            0
                        .quad            .Lx497_2
                        .quad            .Lx497_2
.Lx497_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx497_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx497_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx497_240
                        add              rsp, 16;                             jmp   n203_statement_begin_β
.Lx497_240:                                                                   jmp   n205_assign_α
n204_call_β:            mov              r11, 123;                            jmp   n203_statement_begin_β
.Lx497_0:               .quad            .Lx497_0_s
.Lx497_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n205_assign_α:          mov              r11, 124
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax             # arg
                        mov              qword ptr [r9 + 72], rdx;            jmp   n206_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n206_statement_end_α:   mov              r11, 125
                        mov              r10, 13
                        add              rsp, 16;                             jmp   n207_statement_begin_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n207_statement_begin_α: mov              r11, 126
                        mov              r10, 14;                             jmp   n208_call_α
n207_statement_begin_β: mov              r11, 126;                            jmp   n211_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n208_call_α:            sub              rsp, 16
                        mov              r11, 127
                        lea              rcx, [rip + .Lsig504z]
                        lea              rax, [rip + Pop_α];                  jmp   rax
.Lsig504z:              .quad            0
                        .quad            .Lx504_2
                        .quad            .Lx504_2
.Lx504_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx504_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx504_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx504_240
                        add              rsp, 16;                             jmp   n207_statement_begin_β
.Lx504_240:                                                                   jmp   n209_assign_α
n208_call_β:            mov              r11, 127;                            jmp   n207_statement_begin_β
.Lx504_0:               .quad            .Lx504_0_s
.Lx504_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n209_assign_α:          mov              r11, 128
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              qword ptr [r9 + 88], rdx;            jmp   n210_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n210_statement_end_α:   mov              r11, 129
                        mov              r10, 14
                        add              rsp, 16;                             jmp   n211_statement_begin_α
#=======================================================================================================================
#          Push()   =  EVAL(op arg)
#-----------------------------------------------------------------------------------------------------------------------
n211_statement_begin_α: mov              r11, 130
                        mov              r10, 15;                             jmp   n212_lit_string_α
n211_statement_begin_β: mov              r11, 130;                            jmp   n221_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n212_lit_string_α:      sub              rsp, 16
                        mov              r11, 131
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx510_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n213_call_α
.Lx510_0:               .quad            .Lx510_0_s
.Lx510_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n213_call_α:            sub              rsp, 16
                        mov              r11, 132
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd512:            .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd512]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 655425
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx511_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n211_statement_begin_β
.Lx511_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n214_call_α
n213_call_β:            mov              r11, 132
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n211_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n214_call_α:            sub              rsp, 16
                        mov              r11, 133
                        lea              rcx, [rip + .Lsig514z]
                        lea              rax, [rip + Push_α];                 jmp   rax
.Lsig514z:              .quad            0
                        .quad            .Lx514_2
                        .quad            .Lx514_2
.Lx514_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx514_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx514_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx514_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n211_statement_begin_β
.Lx514_240:                                                                   jmp   n215_var_α
n214_call_β:            mov              r11, 133;                            jmp   n211_statement_begin_β
.Lx514_0:               .quad            .Lx514_0_s
.Lx514_0_s:             .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n215_var_α:             sub              rsp, 16
                        mov              r11, 134
                        mov              rax, qword ptr [r9 + 80]             # op
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n216_var_α
n215_var_β:             mov              r11, 134
                        add              rsp, 16
                        add              rsp, 48;                             jmp   n211_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n216_var_α:             sub              rsp, 16
                        mov              r11, 135
                        mov              rax, qword ptr [r9 + 64]             # arg
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n217_binop_α
n216_var_β:             mov              r11, 135
                        add              rsp, 16;                             jmp   n215_var_β
#-----------------------------------------------------------------------------------------------------------------------
n217_binop_α:           sub              rsp, 16
                        mov              r11, 136
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n218_call_α
n217_binop_β:           mov              r11, 136
                        add              rsp, 16;                             jmp   n216_var_β
#-----------------------------------------------------------------------------------------------------------------------
n218_call_α:            sub              rsp, 16
                        mov              r11, 137
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd519:            .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd519]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 262169
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx518_240
                        add              rsp, 16;                             jmp   n217_binop_β
.Lx518_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n219_assign_var_α
n218_call_β:            mov              r11, 137
                        add              rsp, 16;                             jmp   n217_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n219_assign_var_α:      sub              rsp, 16
                        mov              r11, 138
                        mov              rdi, qword ptr [rsp + 80]            # call
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx520_240
                        add              rsp, 32;                             jmp   n217_binop_β
.Lx520_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n220_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n220_statement_end_α:   mov              r11, 139
                        mov              r10, 15
                        add              rsp, 128;                            jmp   n221_statement_begin_α
#=======================================================================================================================
#          Unary    =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n221_statement_begin_α: mov              r11, 140
                        mov              r10, 16;                             jmp   n222_lit_string_α
n221_statement_begin_β: mov              r11, 140;                            jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n222_lit_string_α:      sub              rsp, 16
                        mov              r11, 141
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx525_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n223_call_α
.Lx525_0:               .quad            .Lx525_0_s
.Lx525_0_s:             .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n223_call_α:            sub              rsp, 16
                        mov              r11, 142
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd527:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd527]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 524341
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx526_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n221_statement_begin_β
.Lx526_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n224_assign_α
n223_call_β:            mov              r11, 142
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n221_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n224_assign_α:          mov              r11, 143
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # Unary
                        mov              qword ptr [r9 + 56], rdx;            jmp   n225_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n225_statement_end_α:   mov              r11, 144
                        mov              r10, 16
                        add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# UnaryEnd  <stmt 17, line 24: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n226_statement_begin_α: mov              r11, 145
                        mov              r10, 17;                             jmp   n227_statement_end_α
n226_statement_begin_β: mov              r11, 145;                            jmp   n228_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n227_statement_end_α:   mov              r11, 146
                        mov              r10, 17;                             jmp   n228_statement_begin_α
#=======================================================================================================================
#          DEFINE('Binary()op,left,right')           :(BinaryEnd)
#-----------------------------------------------------------------------------------------------------------------------
n228_statement_begin_α: mov              r11, 147
                        mov              r10, 18;                             jmp   n229_define_α
n228_statement_begin_β: mov              r11, 147;                            jmp   n264_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n229_define_α:          mov              r11, 148
                        mov              rdi, qword ptr [rip + .Lx538_0]
                        mov              rsi, qword ptr [rip + .Lx538_1]
                        mov              edx, 3
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n231_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx538_0]
                        lea              rsi, [rip + Binary_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n230_statement_end_α
n229_define_β:          mov              r11, 148;                            jmp   n228_statement_begin_β
.Lx538_0:               .quad            .Lx538_0_s
.Lx538_0_s:             .string          "Binary"
.Lx538_1:               .quad            .Lx538_1_s
.Lx538_1_s:             .string          "op,left,right"
                                                                              jmp   .Lx539_245
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
                        cmp              rdx, 0;                              jbe   .Lx539_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 80]             # op
                        mov              qword ptr [r9 + 80], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 88]
                        mov              qword ptr [r9 + 88], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx539_41
.Lx539_10:              mov              rax, qword ptr [r9 + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
.Lx539_41:              cmp              rdx, 1;                              jbe   .Lx539_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 112]            # left
                        mov              qword ptr [r9 + 112], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 120]
                        mov              qword ptr [r9 + 120], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx539_42
.Lx539_11:              mov              rax, qword ptr [r9 + 112]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 112], 0
                        mov              qword ptr [r9 + 120], 0
.Lx539_42:              cmp              rdx, 2;                              jbe   .Lx539_12
                        mov              rdi, qword ptr [rcx + 40]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 128]            # right
                        mov              qword ptr [r9 + 128], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 136]
                        mov              qword ptr [r9 + 136], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx539_43
.Lx539_12:              mov              rax, qword ptr [r9 + 128]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
.Lx539_43:              lea              rcx, [rip + Binary_γ]
                        lea              rax, [rip + Binary_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n231_statement_begin_α]; jmp   rax
Binary_γ:               mov              rdi, qword ptr [r9 + 96]             # Binary
                        mov              rsi, qword ptr [r9 + 104]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rdx, 0;                              jbe   .Lx539_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 88], rax;            jmp   .Lx539_110
.Lx539_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 88], rax
.Lx539_110:             cmp              rdx, 1;                              jbe   .Lx539_81
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 112], rax            # left
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 120], rax;           jmp   .Lx539_111
.Lx539_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 120], rax
.Lx539_111:             cmp              rdx, 2;                              jbe   .Lx539_82
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 128], rax            # right
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx539_112
.Lx539_82:              mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 136], rax
.Lx539_112:             mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx539_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 88], rax;            jmp   .Lx539_180
.Lx539_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 88], rax
.Lx539_180:             cmp              rdx, 1;                              jbe   .Lx539_151
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 112], rax            # left
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 120], rax;           jmp   .Lx539_181
.Lx539_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 120], rax
.Lx539_181:             cmp              rdx, 2;                              jbe   .Lx539_152
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 128], rax            # right
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx539_182
.Lx539_152:             mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 136], rax
.Lx539_182:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 96
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx539_245:
#-----------------------------------------------------------------------------------------------------------------------
n230_statement_end_α:   mov              r11, 149
                        mov              r10, 18;                             jmp   n264_statement_begin_α
#=======================================================================================================================
# Binary   right    =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n231_statement_begin_α: mov              r11, 150
                        mov              r10, 19;                             jmp   n232_call_α
n231_statement_begin_β: mov              r11, 150;                            jmp   n235_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n232_call_α:            sub              rsp, 16
                        mov              r11, 151
                        lea              rcx, [rip + .Lsig545z]
                        lea              rax, [rip + Pop_α];                  jmp   rax
.Lsig545z:              .quad            0
                        .quad            .Lx545_2
                        .quad            .Lx545_2
.Lx545_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx545_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx545_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx545_240
                        add              rsp, 16;                             jmp   n231_statement_begin_β
.Lx545_240:                                                                   jmp   n233_assign_α
n232_call_β:            mov              r11, 151;                            jmp   n231_statement_begin_β
.Lx545_0:               .quad            .Lx545_0_s
.Lx545_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n233_assign_α:          mov              r11, 152
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax            # right
                        mov              qword ptr [r9 + 136], rdx;           jmp   n234_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n234_statement_end_α:   mov              r11, 153
                        mov              r10, 19
                        add              rsp, 16;                             jmp   n235_statement_begin_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n235_statement_begin_α: mov              r11, 154
                        mov              r10, 20;                             jmp   n236_call_α
n235_statement_begin_β: mov              r11, 154;                            jmp   n239_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n236_call_α:            sub              rsp, 16
                        mov              r11, 155
                        lea              rcx, [rip + .Lsig552z]
                        lea              rax, [rip + Pop_α];                  jmp   rax
.Lsig552z:              .quad            0
                        .quad            .Lx552_2
                        .quad            .Lx552_2
.Lx552_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx552_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx552_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx552_240
                        add              rsp, 16;                             jmp   n235_statement_begin_β
.Lx552_240:                                                                   jmp   n237_assign_α
n236_call_β:            mov              r11, 155;                            jmp   n235_statement_begin_β
.Lx552_0:               .quad            .Lx552_0_s
.Lx552_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n237_assign_α:          mov              r11, 156
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              qword ptr [r9 + 88], rdx;            jmp   n238_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n238_statement_end_α:   mov              r11, 157
                        mov              r10, 20
                        add              rsp, 16;                             jmp   n239_statement_begin_α
#=======================================================================================================================
#          left     =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n239_statement_begin_α: mov              r11, 158
                        mov              r10, 21;                             jmp   n240_call_α
n239_statement_begin_β: mov              r11, 158;                            jmp   n243_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n240_call_α:            sub              rsp, 16
                        mov              r11, 159
                        lea              rcx, [rip + .Lsig559z]
                        lea              rax, [rip + Pop_α];                  jmp   rax
.Lsig559z:              .quad            0
                        .quad            .Lx559_2
                        .quad            .Lx559_2
.Lx559_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx559_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx559_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx559_240
                        add              rsp, 16;                             jmp   n239_statement_begin_β
.Lx559_240:                                                                   jmp   n241_assign_α
n240_call_β:            mov              r11, 159;                            jmp   n239_statement_begin_β
.Lx559_0:               .quad            .Lx559_0_s
.Lx559_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n241_assign_α:          mov              r11, 160
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # left
                        mov              qword ptr [r9 + 120], rdx;           jmp   n242_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n242_statement_end_α:   mov              r11, 161
                        mov              r10, 21
                        add              rsp, 16;                             jmp   n243_statement_begin_α
#=======================================================================================================================
#          Push()   =  EVAL(left ' ' op ' ' right)
#-----------------------------------------------------------------------------------------------------------------------
n243_statement_begin_α: mov              r11, 162
                        mov              r10, 22;                             jmp   n244_lit_string_α
n243_statement_begin_β: mov              r11, 162;                            jmp   n259_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n244_lit_string_α:      sub              rsp, 16
                        mov              r11, 163
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx565_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n245_call_α
.Lx565_0:               .quad            .Lx565_0_s
.Lx565_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n245_call_α:            sub              rsp, 16
                        mov              r11, 164
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd567:            .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd567]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 655425
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx566_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n243_statement_begin_β
.Lx566_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n246_call_α
n245_call_β:            mov              r11, 164
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n243_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n246_call_α:            sub              rsp, 16
                        mov              r11, 165
                        lea              rcx, [rip + .Lsig569z]
                        lea              rax, [rip + Push_α];                 jmp   rax
.Lsig569z:              .quad            0
                        .quad            .Lx569_2
                        .quad            .Lx569_2
.Lx569_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx569_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx569_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx569_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n243_statement_begin_β
.Lx569_240:                                                                   jmp   n247_var_α
n246_call_β:            mov              r11, 165;                            jmp   n243_statement_begin_β
.Lx569_0:               .quad            .Lx569_0_s
.Lx569_0_s:             .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n247_var_α:             sub              rsp, 16
                        mov              r11, 166
                        mov              rax, qword ptr [r9 + 112]            # left
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n248_lit_string_α
n247_var_β:             mov              r11, 166
                        add              rsp, 16
                        add              rsp, 48;                             jmp   n243_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n248_lit_string_α:      sub              rsp, 16
                        mov              r11, 167
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx571_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n249_binop_α
n248_lit_string_β:      mov              r11, 167
                        add              rsp, 16;                             jmp   n247_var_β
.Lx571_0:               .quad            .Lx571_0_s
.Lx571_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n249_binop_α:           sub              rsp, 16
                        mov              r11, 168
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n250_var_α
n249_binop_β:           mov              r11, 168
                        add              rsp, 16;                             jmp   n248_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n250_var_α:             sub              rsp, 16
                        mov              r11, 169
                        mov              rax, qword ptr [r9 + 80]             # op
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n251_binop_α
n250_var_β:             mov              r11, 169
                        add              rsp, 16;                             jmp   n249_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n251_binop_α:           sub              rsp, 16
                        mov              r11, 170
                        mov              rdi, qword ptr [rsp + 32]            # binop
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n252_lit_string_α
n251_binop_β:           mov              r11, 170
                        add              rsp, 16;                             jmp   n250_var_β
#-----------------------------------------------------------------------------------------------------------------------
n252_lit_string_α:      sub              rsp, 16
                        mov              r11, 171
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx575_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n253_binop_α
n252_lit_string_β:      mov              r11, 171
                        add              rsp, 16;                             jmp   n251_binop_β
.Lx575_0:               .quad            .Lx575_0_s
.Lx575_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n253_binop_α:           sub              rsp, 16
                        mov              r11, 172
                        mov              rdi, qword ptr [rsp + 32]            # binop
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n254_var_α
n253_binop_β:           mov              r11, 172
                        add              rsp, 16;                             jmp   n252_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n254_var_α:             sub              rsp, 16
                        mov              r11, 173
                        mov              rax, qword ptr [r9 + 128]            # right
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n255_binop_α
n254_var_β:             mov              r11, 173
                        add              rsp, 16;                             jmp   n253_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n255_binop_α:           sub              rsp, 16
                        mov              r11, 174
                        mov              rdi, qword ptr [rsp + 32]            # binop
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n256_call_α
n255_binop_β:           mov              r11, 174
                        add              rsp, 16;                             jmp   n254_var_β
#-----------------------------------------------------------------------------------------------------------------------
n256_call_α:            sub              rsp, 16
                        mov              r11, 175
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd580:            .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd580]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 262169
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx579_240
                        add              rsp, 16;                             jmp   n255_binop_β
.Lx579_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n257_assign_var_α
n256_call_β:            mov              r11, 175
                        add              rsp, 16;                             jmp   n255_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n257_assign_var_α:      sub              rsp, 16
                        mov              r11, 176
                        mov              rdi, qword ptr [rsp + 176]           # call
                        mov              rsi, qword ptr [rsp + 184]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx581_240
                        add              rsp, 32;                             jmp   n255_binop_β
.Lx581_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n258_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n258_statement_end_α:   mov              r11, 177
                        mov              r10, 22
                        add              rsp, 224;                            jmp   n259_statement_begin_α
#=======================================================================================================================
#          Binary   =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n259_statement_begin_α: mov              r11, 178
                        mov              r10, 23;                             jmp   n260_lit_string_α
n259_statement_begin_β: mov              r11, 178;                            jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n260_lit_string_α:      sub              rsp, 16
                        mov              r11, 179
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx586_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n261_call_α
.Lx586_0:               .quad            .Lx586_0_s
.Lx586_0_s:             .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n261_call_α:            sub              rsp, 16
                        mov              r11, 180
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd588:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd588]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 524341
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx587_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n259_statement_begin_β
.Lx587_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n262_assign_α
n261_call_β:            mov              r11, 180
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n259_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n262_assign_α:          mov              r11, 181
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax             # Binary
                        mov              qword ptr [r9 + 104], rdx;           jmp   n263_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n263_statement_end_α:   mov              r11, 182
                        mov              r10, 23
                        add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# BinaryEnd  <stmt 24, line 31: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n264_statement_begin_α: mov              r11, 183
                        mov              r10, 24;                             jmp   n265_statement_end_α
n264_statement_begin_β: mov              r11, 183;                            jmp   n266_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n265_statement_end_α:   mov              r11, 184
                        mov              r10, 24;                             jmp   n266_statement_begin_α
#=======================================================================================================================
#          integer  =  SPAN('0123456789')
#-----------------------------------------------------------------------------------------------------------------------
n266_statement_begin_α: mov              r11, 185
                        mov              r10, 25;                             jmp   n267_lit_string_α
n266_statement_begin_β: mov              r11, 185;                            jmp   n271_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n267_lit_string_α:      sub              rsp, 16
                        mov              r11, 186
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx598_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n268_call_α
.Lx598_0:               .quad            .Lx598_0_s
.Lx598_0_s:             .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n268_call_α:            sub              rsp, 16
                        mov              r11, 187
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd600:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd600]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 589876
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx599_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n266_statement_begin_β
.Lx599_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n269_assign_α
n268_call_β:            mov              r11, 187
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n266_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n269_assign_α:          mov              r11, 188
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 176], rax            # integer
                        mov              qword ptr [r9 + 184], rdx;           jmp   n270_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n270_statement_end_α:   mov              r11, 189
                        mov              r10, 25
                        add              rsp, 32;                             jmp   n271_statement_begin_α
#=======================================================================================================================
#          exponent =  ANY('eEdD') (ANY('+-') | epsilon) integer
#-----------------------------------------------------------------------------------------------------------------------
n271_statement_begin_α: mov              r11, 190
                        mov              r10, 26;                             jmp   n272_var_α
n271_statement_begin_β: mov              r11, 190;                            jmp   n281_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n272_var_α:             sub              rsp, 16
                        mov              r11, 191
                        mov              rax, qword ptr [r9 + 208]            # epsilon
                        mov              rdx, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n273_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n273_assign_α:          mov              r11, 192
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 368], rax            # PAT$1$V0
                        mov              qword ptr [r9 + 376], rdx;           jmp   n274_var_α
#-----------------------------------------------------------------------------------------------------------------------
n274_var_α:             sub              rsp, 16
                        mov              r11, 193
                        mov              rax, qword ptr [r9 + 176]            # integer
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n275_assign_α
n274_var_β:             mov              r11, 193
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n271_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n275_assign_α:          mov              r11, 194
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 384], rax            # PAT$1$V1
                        mov              qword ptr [r9 + 392], rdx;           jmp   n276_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n276_lit_string_α:      sub              rsp, 16
                        mov              r11, 195
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx610_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n277_lit_string_α
n276_lit_string_β:      mov              r11, 195
                        add              rsp, 16;                             jmp   n274_var_β
.Lx610_0:               .quad            .Lx610_0_s
.Lx610_0_s:             .string          "PAT$1"
#-----------------------------------------------------------------------------------------------------------------------
n277_lit_string_α:      sub              rsp, 16
                        mov              r11, 196
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx611_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n278_call_α
n277_lit_string_β:      mov              r11, 196
                        add              rsp, 16;                             jmp   n276_lit_string_β
.Lx611_0:               .quad            .Lx611_0_s
.Lx611_0_s:             .string          "2"
#-----------------------------------------------------------------------------------------------------------------------
n278_call_α:            sub              rsp, 16
                        mov              r11, 197
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
.Lrkfnzd613:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd613]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              ecx, 589876
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx612_240
                        add              rsp, 16;                             jmp   n277_lit_string_β
.Lx612_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n279_assign_α
n278_call_β:            mov              r11, 197
                        add              rsp, 16;                             jmp   n277_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n279_assign_α:          mov              r11, 198
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 192], rax            # exponent
                        mov              qword ptr [r9 + 200], rdx;           jmp   n280_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n280_statement_end_α:   mov              r11, 199
                        mov              r10, 26
                        add              rsp, 80;                             jmp   n281_statement_begin_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#-----------------------------------------------------------------------------------------------------------------------
n281_statement_begin_α: mov              r11, 200
                        mov              r10, 27;                             jmp   n282_var_α
n281_statement_begin_β: mov              r11, 200;                            jmp   n301_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n282_var_α:             sub              rsp, 16
                        mov              r11, 201
                        mov              rax, qword ptr [r9 + 176]            # integer
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n283_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n283_assign_α:          mov              r11, 202
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 400], rax            # PAT$2$V0
                        mov              qword ptr [r9 + 408], rdx;           jmp   n284_var_α
#-----------------------------------------------------------------------------------------------------------------------
n284_var_α:             sub              rsp, 16
                        mov              r11, 203
                        mov              rax, qword ptr [r9 + 176]            # integer
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n285_assign_α
n284_var_β:             mov              r11, 203
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n281_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n285_assign_α:          mov              r11, 204
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 416], rax            # PAT$2$V1
                        mov              qword ptr [r9 + 424], rdx;           jmp   n286_var_α
#-----------------------------------------------------------------------------------------------------------------------
n286_var_α:             sub              rsp, 16
                        mov              r11, 205
                        mov              rax, qword ptr [r9 + 208]            # epsilon
                        mov              rdx, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n287_assign_α
n286_var_β:             mov              r11, 205
                        add              rsp, 16;                             jmp   n284_var_β
#-----------------------------------------------------------------------------------------------------------------------
n287_assign_α:          mov              r11, 206
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 432], rax            # PAT$2$V2
                        mov              qword ptr [r9 + 440], rdx;           jmp   n288_var_α
#-----------------------------------------------------------------------------------------------------------------------
n288_var_α:             sub              rsp, 16
                        mov              r11, 207
                        mov              rax, qword ptr [r9 + 192]            # exponent
                        mov              rdx, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n289_assign_α
n288_var_β:             mov              r11, 207
                        add              rsp, 16;                             jmp   n286_var_β
#-----------------------------------------------------------------------------------------------------------------------
n289_assign_α:          mov              r11, 208
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 448], rax            # PAT$2$V3
                        mov              qword ptr [r9 + 456], rdx;           jmp   n290_var_α
#-----------------------------------------------------------------------------------------------------------------------
n290_var_α:             sub              rsp, 16
                        mov              r11, 209
                        mov              rax, qword ptr [r9 + 208]            # epsilon
                        mov              rdx, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n291_assign_α
n290_var_β:             mov              r11, 209
                        add              rsp, 16;                             jmp   n288_var_β
#-----------------------------------------------------------------------------------------------------------------------
n291_assign_α:          mov              r11, 210
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 464], rax            # PAT$2$V4
                        mov              qword ptr [r9 + 472], rdx;           jmp   n292_var_α
#-----------------------------------------------------------------------------------------------------------------------
n292_var_α:             sub              rsp, 16
                        mov              r11, 211
                        mov              rax, qword ptr [r9 + 176]            # integer
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n293_assign_α
n292_var_β:             mov              r11, 211
                        add              rsp, 16;                             jmp   n290_var_β
#-----------------------------------------------------------------------------------------------------------------------
n293_assign_α:          mov              r11, 212
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 480], rax            # PAT$2$V5
                        mov              qword ptr [r9 + 488], rdx;           jmp   n294_var_α
#-----------------------------------------------------------------------------------------------------------------------
n294_var_α:             sub              rsp, 16
                        mov              r11, 213
                        mov              rax, qword ptr [r9 + 192]            # exponent
                        mov              rdx, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n295_assign_α
n294_var_β:             mov              r11, 213
                        add              rsp, 16;                             jmp   n292_var_β
#-----------------------------------------------------------------------------------------------------------------------
n295_assign_α:          mov              r11, 214
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 496], rax            # PAT$2$V6
                        mov              qword ptr [r9 + 504], rdx;           jmp   n296_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n296_lit_string_α:      sub              rsp, 16
                        mov              r11, 215
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx633_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n297_lit_string_α
n296_lit_string_β:      mov              r11, 215
                        add              rsp, 16;                             jmp   n294_var_β
.Lx633_0:               .quad            .Lx633_0_s
.Lx633_0_s:             .string          "PAT$2"
#-----------------------------------------------------------------------------------------------------------------------
n297_lit_string_α:      sub              rsp, 16
                        mov              r11, 216
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx634_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n298_call_α
n297_lit_string_β:      mov              r11, 216
                        add              rsp, 16;                             jmp   n296_lit_string_β
.Lx634_0:               .quad            .Lx634_0_s
.Lx634_0_s:             .string          "7"
#-----------------------------------------------------------------------------------------------------------------------
n298_call_α:            sub              rsp, 16
                        mov              r11, 217
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
.Lrkfnzd636:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd636]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              ecx, 589876
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx635_240
                        add              rsp, 16;                             jmp   n297_lit_string_β
.Lx635_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n299_assign_α
n298_call_β:            mov              r11, 217
                        add              rsp, 16;                             jmp   n297_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n299_assign_α:          mov              r11, 218
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 224], rax            # real
                        mov              qword ptr [r9 + 232], rdx;           jmp   n300_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n300_statement_end_α:   mov              r11, 219
                        mov              r10, 27;                             jmp   n301_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n301_statement_begin_α: mov              r11, 220
                        mov              r10, 0;                              jmp   n302_statement_end_α
n301_statement_begin_β: mov              r11, 220
                        add              rsp, 160;                            jmp   n303_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n302_statement_end_α:   mov              r11, 221
                        mov              r10, 28
                        add              rsp, 160;                            jmp   n303_statement_begin_α
#=======================================================================================================================
#          addop    =  ANY('+-') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n303_statement_begin_α: mov              r11, 222
                        mov              r10, 29;                             jmp   n304_lit_string_α
n303_statement_begin_β: mov              r11, 222;                            jmp   n308_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n304_lit_string_α:      sub              rsp, 16
                        mov              r11, 223
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx646_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n305_call_α
.Lx646_0:               .quad            .Lx646_0_s
.Lx646_0_s:             .string          "PAT$3"
#-----------------------------------------------------------------------------------------------------------------------
n305_call_α:            sub              rsp, 16
                        mov              r11, 224
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd648:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd648]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 589876
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx647_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n303_statement_begin_β
.Lx647_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n306_assign_α
n305_call_β:            mov              r11, 224
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n303_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n306_assign_α:          mov              r11, 225
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 240], rax            # addop
                        mov              qword ptr [r9 + 248], rdx;           jmp   n307_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n307_statement_end_α:   mov              r11, 226
                        mov              r10, 29
                        add              rsp, 32;                             jmp   n308_statement_begin_α
#=======================================================================================================================
#          mulop    =  ANY('*/') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n308_statement_begin_α: mov              r11, 227
                        mov              r10, 30;                             jmp   n309_lit_string_α
n308_statement_begin_β: mov              r11, 227;                            jmp   n313_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n309_lit_string_α:      sub              rsp, 16
                        mov              r11, 228
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx654_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n310_call_α
.Lx654_0:               .quad            .Lx654_0_s
.Lx654_0_s:             .string          "PAT$4"
#-----------------------------------------------------------------------------------------------------------------------
n310_call_α:            sub              rsp, 16
                        mov              r11, 229
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd656:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd656]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 589876
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx655_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n308_statement_begin_β
.Lx655_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n311_assign_α
n310_call_β:            mov              r11, 229
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n308_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n311_assign_α:          mov              r11, 230
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 256], rax            # mulop
                        mov              qword ptr [r9 + 264], rdx;           jmp   n312_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n312_statement_end_α:   mov              r11, 231
                        mov              r10, 30
                        add              rsp, 32;                             jmp   n313_statement_begin_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n313_statement_begin_α: mov              r11, 232
                        mov              r10, 31;                             jmp   n314_var_α
n313_statement_begin_β: mov              r11, 232;                            jmp   n323_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n314_var_α:             sub              rsp, 16
                        mov              r11, 233
                        mov              rax, qword ptr [r9 + 224]            # real
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n315_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n315_assign_α:          mov              r11, 234
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 512], rax            # PAT$5$V0
                        mov              qword ptr [r9 + 520], rdx;           jmp   n316_var_α
#-----------------------------------------------------------------------------------------------------------------------
n316_var_α:             sub              rsp, 16
                        mov              r11, 235
                        mov              rax, qword ptr [r9 + 176]            # integer
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n317_assign_α
n316_var_β:             mov              r11, 235
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n313_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n317_assign_α:          mov              r11, 236
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 528], rax            # PAT$5$V1
                        mov              qword ptr [r9 + 536], rdx;           jmp   n318_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n318_lit_string_α:      sub              rsp, 16
                        mov              r11, 237
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx666_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n319_lit_string_α
n318_lit_string_β:      mov              r11, 237
                        add              rsp, 16;                             jmp   n316_var_β
.Lx666_0:               .quad            .Lx666_0_s
.Lx666_0_s:             .string          "PAT$5"
#-----------------------------------------------------------------------------------------------------------------------
n319_lit_string_α:      sub              rsp, 16
                        mov              r11, 238
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx667_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n320_call_α
n319_lit_string_β:      mov              r11, 238
                        add              rsp, 16;                             jmp   n318_lit_string_β
.Lx667_0:               .quad            .Lx667_0_s
.Lx667_0_s:             .string          "2"
#-----------------------------------------------------------------------------------------------------------------------
n320_call_α:            sub              rsp, 16
                        mov              r11, 239
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
.Lrkfnzd669:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd669]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              ecx, 589876
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx668_240
                        add              rsp, 16;                             jmp   n319_lit_string_β
.Lx668_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n321_assign_α
n320_call_β:            mov              r11, 239
                        add              rsp, 16;                             jmp   n319_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n321_assign_α:          mov              r11, 240
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 272], rax            # constant
                        mov              qword ptr [r9 + 280], rdx;           jmp   n322_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n322_statement_end_α:   mov              r11, 241
                        mov              r10, 31;                             jmp   n323_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n323_statement_begin_α: mov              r11, 242
                        mov              r10, 0;                              jmp   n324_statement_end_α
n323_statement_begin_β: mov              r11, 242
                        add              rsp, 80;                             jmp   n325_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n324_statement_end_α:   mov              r11, 243
                        mov              r10, 32
                        add              rsp, 80;                             jmp   n325_statement_begin_α
#=======================================================================================================================
#          primary  =  constant | '(' *expr ')'
#-----------------------------------------------------------------------------------------------------------------------
n325_statement_begin_α: mov              r11, 244
                        mov              r10, 33;                             jmp   n326_var_α
n325_statement_begin_β: mov              r11, 244;                            jmp   n333_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n326_var_α:             sub              rsp, 16
                        mov              r11, 245
                        mov              rax, qword ptr [r9 + 272]            # constant
                        mov              rdx, qword ptr [r9 + 280]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n327_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n327_assign_α:          mov              r11, 246
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 544], rax            # PAT$6$V0
                        mov              qword ptr [r9 + 552], rdx;           jmp   n328_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n328_lit_string_α:      sub              rsp, 16
                        mov              r11, 247
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx681_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n329_lit_string_α
n328_lit_string_β:      mov              r11, 247
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n325_statement_begin_β
.Lx681_0:               .quad            .Lx681_0_s
.Lx681_0_s:             .string          "PAT$6"
#-----------------------------------------------------------------------------------------------------------------------
n329_lit_string_α:      sub              rsp, 16
                        mov              r11, 248
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx682_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n330_call_α
n329_lit_string_β:      mov              r11, 248
                        add              rsp, 16;                             jmp   n328_lit_string_β
.Lx682_0:               .quad            .Lx682_0_s
.Lx682_0_s:             .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
n330_call_α:            sub              rsp, 16
                        mov              r11, 249
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
.Lrkfnzd684:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd684]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              ecx, 589876
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx683_240
                        add              rsp, 16;                             jmp   n329_lit_string_β
.Lx683_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n331_assign_α
n330_call_β:            mov              r11, 249
                        add              rsp, 16;                             jmp   n329_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n331_assign_α:          mov              r11, 250
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 288], rax            # primary
                        mov              qword ptr [r9 + 296], rdx;           jmp   n332_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n332_statement_end_α:   mov              r11, 251
                        mov              r10, 33;                             jmp   n333_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n333_statement_begin_α: mov              r11, 252
                        mov              r10, 0;                              jmp   n334_statement_end_α
n333_statement_begin_β: mov              r11, 252
                        add              rsp, 64;                             jmp   n335_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n334_statement_end_α:   mov              r11, 253
                        mov              r10, 34
                        add              rsp, 64;                             jmp   n335_statement_begin_α
#=======================================================================================================================
#          factor   =  addop *factor . *Unary()
#-----------------------------------------------------------------------------------------------------------------------
n335_statement_begin_α: mov              r11, 254
                        mov              r10, 35;                             jmp   n336_var_α
n335_statement_begin_β: mov              r11, 254;                            jmp   n343_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n336_var_α:             sub              rsp, 16
                        mov              r11, 255
                        mov              rax, qword ptr [r9 + 240]            # addop
                        mov              rdx, qword ptr [r9 + 248]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n337_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n337_assign_α:          mov              r11, 256
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 560], rax            # PAT$7$V0
                        mov              qword ptr [r9 + 568], rdx;           jmp   n338_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n338_lit_string_α:      sub              rsp, 16
                        mov              r11, 257
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx696_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n339_lit_string_α
n338_lit_string_β:      mov              r11, 257
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n335_statement_begin_β
.Lx696_0:               .quad            .Lx696_0_s
.Lx696_0_s:             .string          "PAT$7"
#-----------------------------------------------------------------------------------------------------------------------
n339_lit_string_α:      sub              rsp, 16
                        mov              r11, 258
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx697_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n340_call_α
n339_lit_string_β:      mov              r11, 258
                        add              rsp, 16;                             jmp   n338_lit_string_β
.Lx697_0:               .quad            .Lx697_0_s
.Lx697_0_s:             .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
n340_call_α:            sub              rsp, 16
                        mov              r11, 259
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
.Lrkfnzd699:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd699]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              ecx, 589876
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx698_240
                        add              rsp, 16;                             jmp   n339_lit_string_β
.Lx698_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n341_assign_α
n340_call_β:            mov              r11, 259
                        add              rsp, 16;                             jmp   n339_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n341_assign_α:          mov              r11, 260
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 304], rax            # factor
                        mov              qword ptr [r9 + 312], rdx;           jmp   n342_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n342_statement_end_α:   mov              r11, 261
                        mov              r10, 35;                             jmp   n343_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n343_statement_begin_α: mov              r11, 262
                        mov              r10, 0;                              jmp   n344_statement_end_α
n343_statement_begin_β: mov              r11, 262
                        add              rsp, 64;                             jmp   n345_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n344_statement_end_α:   mov              r11, 263
                        mov              r10, 36
                        add              rsp, 64;                             jmp   n345_statement_begin_α
#=======================================================================================================================
#          term     =  *factor mulop *term . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n345_statement_begin_α: mov              r11, 264
                        mov              r10, 37;                             jmp   n346_var_α
n345_statement_begin_β: mov              r11, 264;                            jmp   n353_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n346_var_α:             sub              rsp, 16
                        mov              r11, 265
                        mov              rax, qword ptr [r9 + 256]            # mulop
                        mov              rdx, qword ptr [r9 + 264]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n347_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n347_assign_α:          mov              r11, 266
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 576], rax            # PAT$8$V0
                        mov              qword ptr [r9 + 584], rdx;           jmp   n348_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n348_lit_string_α:      sub              rsp, 16
                        mov              r11, 267
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx711_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n349_lit_string_α
n348_lit_string_β:      mov              r11, 267
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n345_statement_begin_β
.Lx711_0:               .quad            .Lx711_0_s
.Lx711_0_s:             .string          "PAT$8"
#-----------------------------------------------------------------------------------------------------------------------
n349_lit_string_α:      sub              rsp, 16
                        mov              r11, 268
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx712_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n350_call_α
n349_lit_string_β:      mov              r11, 268
                        add              rsp, 16;                             jmp   n348_lit_string_β
.Lx712_0:               .quad            .Lx712_0_s
.Lx712_0_s:             .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
n350_call_α:            sub              rsp, 16
                        mov              r11, 269
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
.Lrkfnzd714:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd714]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              ecx, 589876
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx713_240
                        add              rsp, 16;                             jmp   n349_lit_string_β
.Lx713_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n351_assign_α
n350_call_β:            mov              r11, 269
                        add              rsp, 16;                             jmp   n349_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n351_assign_α:          mov              r11, 270
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 320], rax            # term
                        mov              qword ptr [r9 + 328], rdx;           jmp   n352_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n352_statement_end_α:   mov              r11, 271
                        mov              r10, 37;                             jmp   n353_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n353_statement_begin_α: mov              r11, 272
                        mov              r10, 0;                              jmp   n354_statement_end_α
n353_statement_begin_β: mov              r11, 272
                        add              rsp, 64;                             jmp   n355_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n354_statement_end_α:   mov              r11, 273
                        mov              r10, 38
                        add              rsp, 64;                             jmp   n355_statement_begin_α
#=======================================================================================================================
#          expr     =  *term addop *expr . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n355_statement_begin_α: mov              r11, 274
                        mov              r10, 39;                             jmp   n356_var_α
n355_statement_begin_β: mov              r11, 274;                            jmp   n363_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n356_var_α:             sub              rsp, 16
                        mov              r11, 275
                        mov              rax, qword ptr [r9 + 240]            # addop
                        mov              rdx, qword ptr [r9 + 248]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n357_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n357_assign_α:          mov              r11, 276
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 592], rax            # PAT$9$V0
                        mov              qword ptr [r9 + 600], rdx;           jmp   n358_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n358_lit_string_α:      sub              rsp, 16
                        mov              r11, 277
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx726_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n359_lit_string_α
n358_lit_string_β:      mov              r11, 277
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n355_statement_begin_β
.Lx726_0:               .quad            .Lx726_0_s
.Lx726_0_s:             .string          "PAT$9"
#-----------------------------------------------------------------------------------------------------------------------
n359_lit_string_α:      sub              rsp, 16
                        mov              r11, 278
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx727_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n360_call_α
n359_lit_string_β:      mov              r11, 278
                        add              rsp, 16;                             jmp   n358_lit_string_β
.Lx727_0:               .quad            .Lx727_0_s
.Lx727_0_s:             .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
n360_call_α:            sub              rsp, 16
                        mov              r11, 279
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
.Lrkfnzd729:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd729]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              ecx, 589876
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx728_240
                        add              rsp, 16;                             jmp   n359_lit_string_β
.Lx728_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n361_assign_α
n360_call_β:            mov              r11, 279
                        add              rsp, 16;                             jmp   n359_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n361_assign_α:          mov              r11, 280
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 336], rax            # expr
                        mov              qword ptr [r9 + 344], rdx;           jmp   n362_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n362_statement_end_α:   mov              r11, 281
                        mov              r10, 39;                             jmp   n363_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n363_statement_begin_α: mov              r11, 282
                        mov              r10, 0;                              jmp   n364_statement_end_α
n363_statement_begin_β: mov              r11, 282
                        add              rsp, 64;                             jmp   n365_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n364_statement_end_α:   mov              r11, 283
                        mov              r10, 40
                        add              rsp, 64;                             jmp   n365_statement_begin_α
#=======================================================================================================================
#          &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n365_statement_begin_α: mov              r11, 284
                        mov              r10, 41;                             jmp   n366_lit_integer_α
n365_statement_begin_β: mov              r11, 284;                            jmp   n369_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n366_lit_integer_α:     sub              rsp, 16
                        mov              r11, 285
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx739_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n367_keyword_assign_snobol4_α
.Lx739_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n367_keyword_assign_snobol4_α:
                        sub              rsp, 16
                        mov              r11, 286
                        mov              rdi, qword ptr [rip + .Lx740_0]
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_kw_write_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx740_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n365_statement_begin_β
.Lx740_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n368_statement_end_α
.Lx740_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n368_statement_end_α:   mov              r11, 287
                        mov              r10, 41
                        add              rsp, 32;                             jmp   n369_statement_begin_α
#=======================================================================================================================
# loop     line     =  INPUT                         :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n369_statement_begin_α: mov              r11, 288
                        mov              r10, 42;                             jmp   n370_var_α
n369_statement_begin_β: mov              r11, 288;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n370_var_α:             sub              rsp, 16
                        mov              r11, 289
                        mov              rdi, qword ptr [rip + .Lx745_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_GET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx745_240
                        add              rsp, 16;                             jmp   n369_statement_begin_β
.Lx745_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n371_assign_α
.Lx745_0:               .quad            .Lx745_0_s
.Lx745_0_s:             .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n371_assign_α:          mov              r11, 290
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 352], rax            # line
                        mov              qword ptr [r9 + 360], rdx;           jmp   n372_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n372_statement_end_α:   mov              r11, 291
                        mov              r10, 42
                        add              rsp, 16;                             jmp   n373_statement_begin_α
#=======================================================================================================================
#          line     POS(0) expr RPOS(0)              :F(error)
#-----------------------------------------------------------------------------------------------------------------------
n373_statement_begin_α: mov              r11, 292
                        mov              r10, 43;                             jmp   n374_var_α
n373_statement_begin_β: mov              r11, 292;                            jmp   n387_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n374_var_α:             sub              rsp, 16
                        mov              r11, 293
                        mov              rax, qword ptr [r9 + 352]            # line
                        mov              rdx, qword ptr [r9 + 360]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n375_var_α
#-----------------------------------------------------------------------------------------------------------------------
n375_var_α:             sub              rsp, 16
                        mov              r11, 294
                        mov              rax, qword ptr [r9 + 336]            # expr
                        mov              rdx, qword ptr [r9 + 344]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n376_assign_α
n375_var_β:             mov              r11, 294
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n387_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n376_assign_α:          mov              r11, 295
                        mov              rsi, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx753_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n377_match_begin_α
n376_assign_β:          mov              r11, 295;                            jmp   n375_var_β
.Lx753_0:               .quad            .Lx753_0_s
.Lx753_0_s:             .string          "PATV$0"
#-----------------------------------------------------------------------------------------------------------------------
n377_match_begin_α:     mov              r11, 296
                        mov              rdi, qword ptr [rsp + 16]            # var
                        mov              rsi, qword ptr [rsp + 24]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 24
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx755_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx755_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n378_match_pos_α
n377_match_begin_β:     mov              r11, 296
.Lx755_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx755_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx755_1
                                                                              jmp   .Lx755_0
.Lx755_1:
n377_match_begin_af:    mov              r11, 296
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   n376_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n378_match_pos_α:       mov              r11, 297
                        mov              rax, 0
                        cmp              r14d, eax;                           jne   n377_match_begin_β
                                                                              jmp   n379_match_defer_α
n378_match_pos_β:       mov              r11, 297;                            jmp   n377_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n379_match_defer_α:     mov              r11, 298
                        lea              rcx, [rip + g_sno_defer_cells+16384]
                        mov              rax, qword ptr [rcx + 0]
                        lea              rdx, [rip + .S22]
                        cmp              rax, rdx;                            jne   .Lx757_30
                        mov              rax, qword ptr [rcx + 8]
                        mov              edx, dword ptr [rax + 0]
                        and              edx, 255
                        cmp              edx, 2;                              jne   .Lx757_30
                        mov              edx, dword ptr [rax + 4]
                        cmp              edx, 1;                              jne   .Lx757_30
                        movsxd           rcx, r14d
                        cmp              ecx, r15d;                           jge   .Lx757_31
                        movzx            esi, byte ptr [r13+rcx]
                        mov              rdi, qword ptr [rax + 8]
                        xor              edx, edx
                        movzx            edi, byte ptr [rdi + rdx]
                        cmp              esi, edi;                            jne   .Lx757_31
                        mov              edx, r14d
                        add              edx, 1;                              jmp   .Lx757_0
.Lx757_31:              mov              edx, -1;                             jmp   .Lx757_0
.Lx757_30:              push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S22]
                        mov              esi, r14d
                        mov              rdx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_probe_run@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             rax, rax;                            jz    .Lx757_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx757_5]
                        push             rcx
                        lea              rcx, [rip + .Lx757_4]
                        push             rcx;                                 jmp   rax
.Lx757_4:                                                                     jmp   n380_match_rpos_α
.Lx757_5:                                                                     jmp   n377_match_begin_β
.Lx757_0:               mov              eax, edx
                        test             eax, eax;                            js    n377_match_begin_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx757_6]
                        push             rcx
                        push             rax;                                 jmp   n380_match_rpos_α
.Lx757_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n377_match_begin_β
n379_match_defer_β:     mov              r11, 298
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx757_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx757_12
                                                                              jmp   rax
.Lx757_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n380_match_rpos_α:      mov              r11, 299
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx;                           jne   n379_match_defer_β
                                                                              jmp   n381_match_end_α
#-----------------------------------------------------------------------------------------------------------------------
n381_match_end_α:       mov              r11, 300
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
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
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            je    .Lx760_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n377_match_begin_af
.Lx760_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n382_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n382_statement_end_α:   mov              r11, 301
                        mov              r10, 43
                        add              rsp, 32;                             jmp   n383_statement_begin_α
#=======================================================================================================================
#          OUTPUT   =  Pop()                         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n383_statement_begin_α: mov              r11, 302
                        mov              r10, 44;                             jmp   n384_call_α
n383_statement_begin_β: mov              r11, 302;                            jmp   n369_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n384_call_α:            sub              rsp, 16
                        mov              r11, 303
                        lea              rcx, [rip + .Lsig766z]
                        lea              rax, [rip + Pop_α];                  jmp   rax
.Lsig766z:              .quad            0
                        .quad            .Lx766_2
                        .quad            .Lx766_2
.Lx766_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx766_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx766_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx766_240
                        add              rsp, 16;                             jmp   n383_statement_begin_β
.Lx766_240:                                                                   jmp   n385_assign_α
n384_call_β:            mov              r11, 303;                            jmp   n383_statement_begin_β
.Lx766_0:               .quad            .Lx766_0_s
.Lx766_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n385_assign_α:          mov              r11, 304
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx767_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n386_statement_end_α
.Lx767_0:               .quad            .Lx767_0_s
.Lx767_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n386_statement_end_α:   mov              r11, 305
                        mov              r10, 44
                        add              rsp, 16;                             jmp   n369_statement_begin_α
#=======================================================================================================================
# error    OUTPUT   = 'Bad input, try again'         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n387_statement_begin_α: mov              r11, 306
                        mov              r10, 45;                             jmp   n388_lit_string_α
n387_statement_begin_β: mov              r11, 306;                            jmp   n369_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n388_lit_string_α:      sub              rsp, 16
                        mov              r11, 307
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 20
                        mov              rax, qword ptr [rip + .Lx772_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n389_assign_α
.Lx772_0:               .quad            .Lx772_0_s
.Lx772_0_s:             .string          "Bad input, try again"
#-----------------------------------------------------------------------------------------------------------------------
n389_assign_α:          mov              r11, 308
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx773_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n390_statement_end_α
.Lx773_0:               .quad            .Lx773_0_s
.Lx773_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n390_statement_end_α:   mov              r11, 309
                        mov              r10, 45
                        add              rsp, 16;                             jmp   n369_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 mov              r11, 310
                        pop              rcx
                        add              rsp, 8;                              jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                mov              r11, 311
                        add              rsp, 8
                        pop              rcx;                                 jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
NRETURN:                mov              r11, 312
                        mov              rax, qword ptr [rip + rt_g_ret_by_name@GOTPCREL]
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
