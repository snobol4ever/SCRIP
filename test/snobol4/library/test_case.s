                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_lwr_α
proc_lwr_α:
#-----------------------------------------------------------------------------------------------------------------------
n0_save_restore_α:                                                            jmp   n1_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_deferred_α:     lea              rax, [rip + lwr_body];               jmp   rax
                                                                              jmp   n2_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n2_save_restore_α:
lwr_alpha:              sub              rsp, 48
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx8_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 0]              # lwr
                        mov              qword ptr [r9 + 0], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx8_41
.Lx8_10:                mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
.Lx8_41:                lea              r10, [rip + lwr_gamma]
                        lea              r11, [rip + lwr_omega]
                        push             r11
                        push             r10
                        lea              rax, [rip + lwr_body];               jmp   rax
lwr_gamma:              mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx8_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 8], rax;             jmp   .Lx8_110
.Lx8_80:                mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 8], rax
.Lx8_110:               mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
lwr_omega:              mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx8_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 8], rax;             jmp   .Lx8_180
.Lx8_150:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 8], rax
.Lx8_180:               mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_upr_α
proc_upr_α:
#-----------------------------------------------------------------------------------------------------------------------
n9_save_restore_α:                                                            jmp   n10_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n10_goto_deferred_α:    lea              rax, [rip + upr_body];               jmp   rax
                                                                              jmp   n11_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n11_save_restore_α:
upr_alpha:              sub              rsp, 48
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx17_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # upr
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx17_41
.Lx17_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx17_41:               lea              r10, [rip + upr_gamma]
                        lea              r11, [rip + upr_omega]
                        push             r11
                        push             r10
                        lea              rax, [rip + upr_body];               jmp   rax
upr_gamma:              mov              rdi, qword ptr [r9 + 16]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx17_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx17_110
.Lx17_80:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 24], rax
.Lx17_110:              mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
upr_omega:              mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx17_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx17_180
.Lx17_150:              mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 24], rax
.Lx17_180:              mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_cap_α
proc_cap_α:
#-----------------------------------------------------------------------------------------------------------------------
n18_save_restore_α:                                                           jmp   n19_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n19_goto_deferred_α:    lea              rax, [rip + cap_body];               jmp   rax
                                                                              jmp   n20_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n20_save_restore_α:
cap_alpha:              sub              rsp, 48
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx26_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 32]             # cap
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx26_41
.Lx26_10:               mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx26_41:               lea              r10, [rip + cap_gamma]
                        lea              r11, [rip + cap_omega]
                        push             r11
                        push             r10
                        lea              rax, [rip + cap_body];               jmp   rax
cap_gamma:              mov              rdi, qword ptr [r9 + 32]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx26_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx26_110
.Lx26_80:               mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 40], rax
.Lx26_110:              mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
cap_omega:              mov              rcx, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        cmp              rdx, 0;                              jbe   .Lx26_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx26_180
.Lx26_150:              mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 40], rax
.Lx26_180:              mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_icase_α
proc_icase_α:
#-----------------------------------------------------------------------------------------------------------------------
n27_save_restore_α:                                                           jmp   n28_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n28_goto_deferred_α:    lea              rax, [rip + icase_body];             jmp   rax
                                                                              jmp   n29_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n29_save_restore_α:
icase_alpha:            sub              rsp, 96
                        mov              rax, qword ptr [r9 + 80]             # letter
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
                        mov              rax, qword ptr [r9 + 96]             # ch
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              rax, qword ptr [r9 + 48]             # icase
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
                        mov              qword ptr [rsp + 48], r10
                        mov              qword ptr [rsp + 56], r11
                        mov              qword ptr [rsp + 64], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        cmp              rdx, 0;                              jbe   .Lx35_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 64]             # str
                        mov              qword ptr [r9 + 64], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 72]
                        mov              qword ptr [r9 + 72], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx35_41
.Lx35_10:               mov              rax, qword ptr [r9 + 64]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
.Lx35_41:               lea              r10, [rip + icase_gamma]
                        lea              r11, [rip + icase_omega]
                        push             r11
                        push             r10
                        lea              rax, [rip + icase_body];             jmp   rax
icase_gamma:            mov              rdi, qword ptr [r9 + 48]             # icase
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rcx, qword ptr [rsp + 64]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 96], rax             # ch
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax             # letter
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
                        cmp              rdx, 0;                              jbe   .Lx35_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 64], rax             # str
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx35_110
.Lx35_80:               mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 72], rax
.Lx35_110:              mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 96
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
icase_omega:            mov              rcx, qword ptr [rsp + 64]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 48], rax             # icase
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 96], rax             # ch
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax             # letter
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
                        cmp              rdx, 0;                              jbe   .Lx35_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 64], rax             # str
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx35_180
.Lx35_150:              mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 72], rax
.Lx35_180:              mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 96
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$0_α
proc_PAT$0_α:
proc_PAT$0_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 24
                        mov              qword ptr [rbp + -8], r10
                        mov              qword ptr [rbp + -16], r11
#-----------------------------------------------------------------------------------------------------------------------
n36_match_defer_α:      sub              rsp, 16
                        mov              rax, qword ptr [r9 + 112]            # PAT$0$V0
                        mov              rdx, qword ptr [r9 + 120]
                        cmp              eax, 8;                              jne   .Lx44_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx44_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx44_10
.Lx44_9:                xor              eax, eax
.Lx44_10:               test             rax, rax;                            jz    .Lx44_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx44_4]
                        lea              r11, [rip + .Lx44_5];                jmp   rax
.Lx44_4:                                                                      jmp   n37_match_alternate_α
.Lx44_5:                add              rsp, 16;                             jmp   proc_PAT$0_ω
.Lx44_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S0]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx44_2:                test             rax, rax;                            je    .Lx44_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx44_7]
                        lea              rdx, [rip + .Lx44_8];                jmp   rax
.Lx44_7:                mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_step@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx44_2
.Lx44_8:                mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_step@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx44_2
.Lx44_3:                add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_close@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            jns   .Lx44_240
                        add              rsp, 16;                             jmp   proc_PAT$0_ω
.Lx44_240:              mov              r14d, eax
                        lea              rax, [rip + .Lx44_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n37_match_alternate_α
.Lx44_6:                add              rsp, 16
                        add              rsp, 16;                             jmp   proc_PAT$0_ω
n36_match_defer_β:                                                            jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n37_match_alternate_α:  sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx46_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n41_var_α
.Lx46_21:               lea              rax, [rip + .Lx46_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n38_var_α
n37_match_alternate_s0: lea              rax, [rip + .Lx46_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n37_match_alternate_as
n37_match_alternate_s1: lea              rax, [rip + .Lx46_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n37_match_alternate_as
.Lx46_40:                                                                     jmp   n43_match_value_β
.Lx46_41:                                                                     jmp   n40_match_value_β
n37_match_alternate_as:                                                       jmp   proc_PAT$0_γ
n37_match_alternate_β:  mov              rax, qword ptr [rsp + 8];            jmp   rax
n37_match_alternate_af: mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx46_19:               add              rsp, 32;                             jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
n38_var_α:              mov              rax, qword ptr [r9 + 80]             # letter
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 176], rax           # result
                        mov              qword ptr [rsp + 184], rdx;          jmp   n39_call_α
n38_var_β:                                                                    jmp   n37_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n39_call_α:             lea              rcx, [rip + .Lsig49]
                        lea              rax, [rip + lwr_alpha];              jmp   rax
.Lsig49:                .quad            1
                        .quad            .Lx49_2
                        .quad            .Lx49_2
                        .quad            176
                        lea              r8, [rip + g_gc_pending]
                        mov              eax, dword ptr [r8 + 0]
                        test             eax, eax;                            jne   .Lx49_20
                        mov              rax, qword ptr [rsp + 176]
                        mov              rdx, qword ptr [rsp + 184]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx;             jmp   .Lx49_21
.Lx49_20:               mov              edi, 0
                        mov              rsi, qword ptr [rsp + 176]
                        mov              rdx, qword ptr [rsp + 184]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_arg_stage@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx49_21:               mov              rdi, qword ptr [rip + .Lx49_0]
                        mov              esi, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            je    .Lx49_1
                        mov              rdi, qword ptr [rip + .Lx49_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx49_3]
                        lea              rdx, [rip + .Lx49_4];                jmp   rax
.Lx49_3:                mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx49_2
.Lx49_4:                mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx49_2
.Lx49_1:                call             rt_faildescr@PLT
.Lx49_2:                mov              qword ptr [rsp + 128], rax
                        mov              qword ptr [rsp + 136], rdx
                        cmp              eax, 104;                            je    n37_match_alternate_af
                                                                              jmp   n40_match_value_α
n39_call_β:                                                                   jmp   n37_match_alternate_af
.Lx49_0:                .quad            .Lx49_0_s
.Lx49_0_s:              .string          "lwr"
#-----------------------------------------------------------------------------------------------------------------------
n40_match_value_α:      lea              rdi, [rsp + 128]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_value_get_pat_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            jz    .Lx50_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx50_4]
                        lea              rdx, [rip + .Lx50_5];                jmp   rax
.Lx50_4:                                                                      jmp   n37_match_alternate_s1
.Lx50_5:                                                                      jmp   n37_match_alternate_af
.Lx50_0:                lea              rdi, [rsp + 128]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_value_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              edi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_close@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            js    n37_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx50_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n37_match_alternate_s1
.Lx50_6:                add              rsp, 16;                             jmp   n37_match_alternate_af
n40_match_value_β:                                                            jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n41_var_α:              mov              rax, qword ptr [r9 + 80]             # letter
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 112], rax           # result
                        mov              qword ptr [rsp + 120], rdx;          jmp   n42_call_α
n41_var_β:                                                                    jmp   n37_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n42_call_α:             lea              rcx, [rip + .Lsig53]
                        lea              rax, [rip + upr_alpha];              jmp   rax
.Lsig53:                .quad            1
                        .quad            .Lx53_2
                        .quad            .Lx53_2
                        .quad            112
                        lea              r8, [rip + g_gc_pending]
                        mov              eax, dword ptr [r8 + 0]
                        test             eax, eax;                            jne   .Lx53_20
                        mov              rax, qword ptr [rsp + 112]
                        mov              rdx, qword ptr [rsp + 120]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx;             jmp   .Lx53_21
.Lx53_20:               mov              edi, 0
                        mov              rsi, qword ptr [rsp + 112]
                        mov              rdx, qword ptr [rsp + 120]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_arg_stage@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx53_21:               mov              rdi, qword ptr [rip + .Lx53_0]
                        mov              esi, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            je    .Lx53_1
                        mov              rdi, qword ptr [rip + .Lx53_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx53_3]
                        lea              rdx, [rip + .Lx53_4];                jmp   rax
.Lx53_3:                mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx53_2
.Lx53_4:                mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx53_2
.Lx53_1:                call             rt_faildescr@PLT
.Lx53_2:                mov              qword ptr [rsp + 64], rax
                        mov              qword ptr [rsp + 72], rdx
                        cmp              eax, 104;                            je    n37_match_alternate_af
                                                                              jmp   n43_match_value_α
n42_call_β:                                                                   jmp   n37_match_alternate_af
.Lx53_0:                .quad            .Lx53_0_s
.Lx53_0_s:              .string          "upr"
#-----------------------------------------------------------------------------------------------------------------------
n43_match_value_α:      lea              rdi, [rsp + 64]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_value_get_pat_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            jz    .Lx54_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx54_4]
                        lea              rdx, [rip + .Lx54_5];                jmp   rax
.Lx54_4:                                                                      jmp   n37_match_alternate_s0
.Lx54_5:                                                                      jmp   n37_match_alternate_af
.Lx54_0:                lea              rdi, [rsp + 64]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_value_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              edi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_close@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            js    n37_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx54_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n37_match_alternate_s0
.Lx54_6:                add              rsp, 16;                             jmp   n37_match_alternate_af
n43_match_value_β:                                                            jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_res:
                        mov              r10, qword ptr [rsp + 8]
                        mov              r11, qword ptr [rsp + 16]
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_β:
                                                                              jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_γ:
                        mov              r10, qword ptr [rbp + -8]
                        mov              r11, qword ptr [rbp + -16]
                        push             rbp
                        push             r11
                        push             r10
                        lea              rax, [rip + proc_PAT$0_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_ω:
                        mov              r10, qword ptr [rbp + -8]
                        mov              r11, qword ptr [rbp + -16]
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__lwr"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + lwr_body]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "LBL__upr"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + upr_body]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname2:       .string          "LBL__cap"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + cap_body]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname2]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname3:       .string          "LBL__icase"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + icase_body]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname3]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname8:       .string          "PAT$0"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname8]
                        lea              rsi, [rip + proc_PAT$0_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 208
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "lwr"
.Lgvan1:                .string          "upr"
.Lgvan2:                .string          "cap"
.Lgvan3:                .string          "icase"
.Lgvan4:                .string          "str"
.Lgvan5:                .string          "letter"
.Lgvan6:                .string          "ch"
.Lgvan7:                .string          "PAT$0$V0"
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
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 8
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 8
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
#         <stmt 1, line 1: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_begin_α:                                                        jmp   n56_statement_end_α
n55_statement_begin_β:                                                        jmp   n57_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_end_α:                                                          jmp   n57_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'no match ok'          :(END)
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_begin_α:                                                        jmp   n58_func_activate_α
n57_statement_begin_β:                                                        jmp   n67_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n58_func_activate_α:    mov              rdi, qword ptr [rip + .Lx240_0]
                        mov              rsi, qword ptr [rip + .Lx240_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_lwr_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n59_statement_end_α
n58_func_activate_β:                                                          jmp   n57_statement_begin_β
.Lx240_0:               .quad            .Lx240_0_s
.Lx240_0_s:             .string          "lwr"
.Lx240_1:               .quad            .Lx240_1_s
.Lx240_1_s:             .string          "lwr"
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_end_α:                                                          jmp   n67_statement_begin_α
#=======================================================================================================================
# lwr  <stmt 3, line 33: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
lwr_body:                                                                     jmp   n61_var_α
n60_statement_begin_β:                                                        jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n61_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]              # lwr
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n62_keyword_snobol4_α
#-----------------------------------------------------------------------------------------------------------------------
n62_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx246_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n63_keyword_snobol4_α
.Lx246_0:               .quad            .Lx246_0_s
.Lx246_0_s:             .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n63_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx247_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n64_call_α
.Lx247_0:               .quad            .Lx247_0_s
.Lx247_0_s:             .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n64_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              r8, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 24], r8
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], r8
                        .section         .rodata
.Lrkfnzd249:            .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd249]
                        lea              rsi, [rsp + 0]
                        mov              edx, 3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 48
                        cmp              eax, 104;                            jne   .Lx248_240
                        add              rsp, 16
                        add              rsp, 48;                             jmp   n60_statement_begin_β
.Lx248_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n65_assign_α
n64_call_β:             add              rsp, 16
                        add              rsp, 48;                             jmp   n60_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n65_assign_α:           mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # lwr
                        mov              qword ptr [r9 + 8], rdx;             jmp   n66_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_end_α:    add              rsp, 64;                             jmp   RETURN
#=======================================================================================================================
# lwr_end  <stmt 4, line 35: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_begin_α:                                                        jmp   n68_statement_end_α
n67_statement_begin_β:                                                        jmp   n69_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_end_α:                                                          jmp   n69_statement_begin_α
#=======================================================================================================================
#         <stmt 5, line 36: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_begin_α:                                                        jmp   n70_func_activate_α
n69_statement_begin_β:                                                        jmp   n79_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n70_func_activate_α:    mov              rdi, qword ptr [rip + .Lx260_0]
                        mov              rsi, qword ptr [rip + .Lx260_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_upr_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n71_statement_end_α
n70_func_activate_β:                                                          jmp   n69_statement_begin_β
.Lx260_0:               .quad            .Lx260_0_s
.Lx260_0_s:             .string          "upr"
.Lx260_1:               .quad            .Lx260_1_s
.Lx260_1_s:             .string          "upr"
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_end_α:                                                          jmp   n79_statement_begin_α
#=======================================================================================================================
# upr  <stmt 6, line 37: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
upr_body:                                                                     jmp   n73_var_α
n72_statement_begin_β:                                                        jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n73_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # upr
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n74_keyword_snobol4_α
#-----------------------------------------------------------------------------------------------------------------------
n74_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx266_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n75_keyword_snobol4_α
.Lx266_0:               .quad            .Lx266_0_s
.Lx266_0_s:             .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n75_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx267_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n76_call_α
.Lx267_0:               .quad            .Lx267_0_s
.Lx267_0_s:             .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n76_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              r8, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 24], r8
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], r8
                        .section         .rodata
.Lrkfnzd269:            .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd269]
                        lea              rsi, [rsp + 0]
                        mov              edx, 3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 48
                        cmp              eax, 104;                            jne   .Lx268_240
                        add              rsp, 16
                        add              rsp, 48;                             jmp   n72_statement_begin_β
.Lx268_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n77_assign_α
n76_call_β:             add              rsp, 16
                        add              rsp, 48;                             jmp   n72_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n77_assign_α:           mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax             # upr
                        mov              qword ptr [r9 + 24], rdx;            jmp   n78_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_end_α:    add              rsp, 64;                             jmp   RETURN
#=======================================================================================================================
# upr_end  <stmt 7, line 39: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_begin_α:                                                        jmp   n80_statement_end_α
n79_statement_begin_β:                                                        jmp   n81_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n80_statement_end_α:                                                          jmp   n81_statement_begin_α
#=======================================================================================================================
#         <stmt 8, line 40: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_begin_α:                                                        jmp   n82_func_activate_α
n81_statement_begin_β:                                                        jmp   n101_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n82_func_activate_α:    mov              rdi, qword ptr [rip + .Lx280_0]
                        mov              rsi, qword ptr [rip + .Lx280_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_cap_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n83_statement_end_α
n82_func_activate_β:                                                          jmp   n81_statement_begin_β
.Lx280_0:               .quad            .Lx280_0_s
.Lx280_0_s:             .string          "cap"
.Lx280_1:               .quad            .Lx280_1_s
.Lx280_1_s:             .string          "cap"
#-----------------------------------------------------------------------------------------------------------------------
n83_statement_end_α:                                                          jmp   n101_statement_begin_α
#=======================================================================================================================
# cap  <stmt 9, line 41: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
cap_body:                                                                     jmp   n85_var_α
n84_statement_begin_β:                                                        jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n85_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]             # cap
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n86_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx286_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n87_lit_integer_α
n86_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n84_statement_begin_β
.Lx286_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n87_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx287_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n88_call_α
n87_lit_integer_β:      add              rsp, 16;                             jmp   n86_lit_integer_β
.Lx287_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n88_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              r8, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 24], r8
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], r8
                        .section         .rodata
.Lrkfnzd289:            .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd289]
                        lea              rsi, [rsp + 0]
                        mov              edx, 3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 48
                        cmp              eax, 104;                            jne   .Lx288_240
                        add              rsp, 16;                             jmp   n87_lit_integer_β
.Lx288_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n89_keyword_snobol4_α
n88_call_β:             add              rsp, 16;                             jmp   n87_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n89_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx290_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n90_keyword_snobol4_α
.Lx290_0:               .quad            .Lx290_0_s
.Lx290_0_s:             .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n90_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx291_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n91_call_α
.Lx291_0:               .quad            .Lx291_0_s
.Lx291_0_s:             .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n91_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              r8, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 24], r8
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], r8
                        .section         .rodata
.Lrkfnzd293:            .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd293]
                        lea              rsi, [rsp + 0]
                        mov              edx, 3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 48
                        cmp              eax, 104;                            jne   .Lx292_240
                        add              rsp, 64;                             jmp   n87_lit_integer_β
.Lx292_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n92_var_α
n91_call_β:             add              rsp, 64;                             jmp   n87_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n92_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]             # cap
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n93_lit_integer_α
n92_var_β:              add              rsp, 80;                             jmp   n87_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n93_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx295_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n94_call_α
n93_lit_integer_β:      add              rsp, 16;                             jmp   n92_var_β
.Lx295_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n94_call_α:             sub              rsp, 16
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
.Lrkfnzd297:            .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd297]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 32
                        cmp              eax, 104;                            jne   .Lx296_240
                        add              rsp, 16;                             jmp   n93_lit_integer_β
.Lx296_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n95_keyword_snobol4_α
n94_call_β:             add              rsp, 16;                             jmp   n93_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n95_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx298_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n96_keyword_snobol4_α
.Lx298_0:               .quad            .Lx298_0_s
.Lx298_0_s:             .string          "UCASE"
#-----------------------------------------------------------------------------------------------------------------------
n96_keyword_snobol4_α:  sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx299_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_keyword_read_snobol4@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n97_call_α
.Lx299_0:               .quad            .Lx299_0_s
.Lx299_0_s:             .string          "LCASE"
#-----------------------------------------------------------------------------------------------------------------------
n97_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              r8, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 24], r8
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], r8
                        .section         .rodata
.Lrkfnzd301:            .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd301]
                        lea              rsi, [rsp + 0]
                        mov              edx, 3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 48
                        cmp              eax, 104;                            jne   .Lx300_240
                        add              rsp, 64;                             jmp   n93_lit_integer_β
.Lx300_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n98_binop_α
n97_call_β:             add              rsp, 64;                             jmp   n93_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n98_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]           # call
                        mov              rsi, qword ptr [rsp + 120]
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n99_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n99_assign_α:           mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax             # cap
                        mov              qword ptr [r9 + 40], rdx;            jmp   n100_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n100_statement_end_α:   add              rsp, 224;                            jmp   RETURN
#=======================================================================================================================
# cap_end  <stmt 10, line 44: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n101_statement_begin_α:                                                       jmp   n102_statement_end_α
n101_statement_begin_β:                                                       jmp   n103_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:                                                         jmp   n103_statement_begin_α
#=======================================================================================================================
#         <stmt 11, line 45: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_begin_α:                                                       jmp   n104_func_activate_α
n103_statement_begin_β:                                                       jmp   n145_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n104_func_activate_α:   mov              rdi, qword ptr [rip + .Lx313_0]
                        mov              rsi, qword ptr [rip + .Lx313_1]
                        mov              edx, 3
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_icase_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n105_statement_end_α
n104_func_activate_β:                                                         jmp   n103_statement_begin_β
.Lx313_0:               .quad            .Lx313_0_s
.Lx313_0_s:             .string          "icase"
.Lx313_1:               .quad            .Lx313_1_s
.Lx313_1_s:             .string          "str,letter,ch"
#-----------------------------------------------------------------------------------------------------------------------
n105_statement_end_α:                                                         jmp   n145_statement_begin_α
#=======================================================================================================================
# icase  <stmt 12, line 46: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
icase_body:                                                                   jmp   n107_var_α
n106_statement_begin_β:                                                       jmp   n110_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n107_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # str
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n108_call_α
#-----------------------------------------------------------------------------------------------------------------------
n108_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd320:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd320]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx319_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n106_statement_begin_β
.Lx319_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n109_statement_end_α
n108_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n106_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n109_statement_end_α:   add              rsp, 32;                             jmp   RETURN
#=======================================================================================================================
#         <stmt 13, line 47: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n110_statement_begin_α:                                                       jmp   n111_var_α
n110_statement_begin_β:                                                       jmp   n128_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n111_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # str
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n112_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n112_match_begin_α:     mov              rdi, qword ptr [rsp + 0]             # var
                        mov              rsi, qword ptr [rsp + 8]
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
.Lx327_0:               mov              r14d, dword ptr [rbp + -40];         jmp   n113_match_pos_α
n112_match_begin_β:     lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx327_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx327_1
                                                                              jmp   .Lx327_0
.Lx327_1:
n112_match_begin_af:    mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 16;                             jmp   n128_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n113_match_pos_α:       mov              rax, 0
                        cmp              r14d, eax;                           jne   n112_match_begin_β
                                                                              jmp   n114_match_assign_save_α
n113_match_pos_β:                                                             jmp   n112_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n114_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n115_match_any_α
n114_match_assign_save_β:
                        add              rsp, 16;                             jmp   n112_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n115_match_any_α:       mov              eax, r14d
                        cmp              eax, r15d;                           jl    .Lx332_239
                        add              rsp, 16;                             jmp   n112_match_begin_β
.Lx332_239:             movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        lea              rdi, [rip + .C0]
                        cmp              byte ptr [rdi+rsi], 0;               jne   .Lx332_240
                        add              rsp, 16;                             jmp   n112_match_begin_β
.Lx332_240:             add              r14d, 1;                             jmp   n116_match_assign_cond_α
n115_match_any_β:       sub              r14d, 1
                        add              rsp, 16;                             jmp   n112_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n116_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n117_match_end_α
n116_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n115_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n117_match_end_α:       mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 960], eax
                        mov              qword ptr [rsp + 984], r14
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              rdi, qword ptr [rbp + -8]            # cas_mark
                        mov              rdx, r13
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx336_1:               test             rax, rax;                            je    .Lx336_2
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx336_3]
                        lea              rdx, [rip + .Lx336_4];               jmp   rax
.Lx336_3:               mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_step@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx336_1
.Lx336_4:               mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_step@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx336_1
.Lx336_2:               call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n118_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n118_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx337_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n119_match_replace_α
.Lx337_0:               .quad            .Lx337_0_s
.Lx337_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n119_match_replace_α:   mov              rdi, qword ptr [rip + .Lx339_0]
                        mov              rsi, qword ptr [rsp + 1088]
                        mov              rdx, qword ptr [rsp + 1096]
                        mov              ecx, dword ptr [rsp + 928]
                        mov              r8, qword ptr [rsp + 952]
                        lea              r9, [rsp + 0]                        # lit_string
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_replace@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx339_1
.Lx339_0:               .quad            .Lx339_0_s
.Lx339_0_s:             .string          "str"
.Lx339_1:                                                                     jmp   n120_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n120_statement_end_α:   add              rsp, 16;                             jmp   n121_statement_begin_α
#=======================================================================================================================
#         <stmt 14, line 48: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n121_statement_begin_α:                                                       jmp   n122_var_α
n121_statement_begin_β:                                                       jmp   icase_body
#-----------------------------------------------------------------------------------------------------------------------
n122_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # icase
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n123_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n123_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # PAT$0$V0
                        mov              qword ptr [r9 + 120], rdx;           jmp   n124_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n124_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx346_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n125_call_α
n124_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n121_statement_begin_β
.Lx346_0:               .quad            .Lx346_0_s
.Lx346_0_s:             .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n125_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd348:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd348]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx347_240
                        add              rsp, 16;                             jmp   n124_lit_string_β
.Lx347_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n126_assign_α
n125_call_β:            add              rsp, 16;                             jmp   n124_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n126_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # icase
                        mov              qword ptr [r9 + 56], rdx;            jmp   n127_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n127_statement_end_α:   add              rsp, 48;                             jmp   icase_body
#=======================================================================================================================
# icase1  <stmt 15, line 49: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n128_statement_begin_α:                                                       jmp   n129_var_α
n128_statement_begin_β:                                                       jmp   n139_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n129_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # str
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n130_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n130_match_begin_α:     mov              rdi, qword ptr [rsp + 0]             # var
                        mov              rsi, qword ptr [rsp + 8]
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
.Lx356_0:               mov              r14d, dword ptr [rbp + -40];         jmp   n131_match_pos_α
n130_match_begin_β:     lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx356_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx356_1
                                                                              jmp   .Lx356_0
.Lx356_1:
n130_match_begin_af:    mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   n138_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n131_match_pos_α:       mov              rax, 0
                        cmp              r14d, eax;                           jne   n130_match_begin_β
                                                                              jmp   n132_match_assign_save_α
n131_match_pos_β:                                                             jmp   n130_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n132_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n133_match_len_α
n132_match_assign_save_β:
                        add              rsp, 16;                             jmp   n130_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n133_match_len_α:       mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jle   .Lx360_240
                        add              rsp, 16;                             jmp   n130_match_begin_β
.Lx360_240:             add              r14d, 1;                             jmp   n134_match_assign_cond_α
n133_match_len_β:       sub              r14d, 1
                        add              rsp, 16;                             jmp   n130_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n134_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S2]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n135_match_end_α
n134_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n133_match_len_β
#-----------------------------------------------------------------------------------------------------------------------
n135_match_end_α:       mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 1232], eax
                        mov              qword ptr [rsp + 1256], r14
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              rdi, qword ptr [rbp + -8]            # cas_mark
                        mov              rdx, r13
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx364_1:               test             rax, rax;                            je    .Lx364_2
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx364_3]
                        lea              rdx, [rip + .Lx364_4];               jmp   rax
.Lx364_3:               mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_step@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx364_1
.Lx364_4:               mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_step@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx364_1
.Lx364_2:               call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n136_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n136_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx365_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n137_match_replace_α
.Lx365_0:               .quad            .Lx365_0_s
.Lx365_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n137_match_replace_α:   mov              rdi, qword ptr [rip + .Lx367_0]
                        mov              rsi, qword ptr [rsp + 1360]
                        mov              rdx, qword ptr [rsp + 1368]
                        mov              ecx, dword ptr [rsp + 1200]
                        mov              r8, qword ptr [rsp + 1224]
                        lea              r9, [rsp + 0]                        # lit_string
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_replace@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx367_1
.Lx367_0:               .quad            .Lx367_0_s
.Lx367_0_s:             .string          "str"
.Lx367_1:                                                                     jmp   n138_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n138_statement_end_α:   add              rsp, 16;                             jmp   n139_statement_begin_α
#=======================================================================================================================
#         <stmt 16, line 50: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n139_statement_begin_α:                                                       jmp   n140_var_α
n139_statement_begin_β:                                                       jmp   icase_body
#-----------------------------------------------------------------------------------------------------------------------
n140_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # icase
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n141_var_α
#-----------------------------------------------------------------------------------------------------------------------
n141_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]             # ch
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n142_binop_α
n141_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n139_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n142_binop_α:           sub              rsp, 16
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n143_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n143_assign_α:          mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # icase
                        mov              qword ptr [r9 + 56], rdx;            jmp   n144_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n144_statement_end_α:   add              rsp, 48;                             jmp   icase_body
#=======================================================================================================================
# icase_end  <stmt 17, line 52: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n145_statement_begin_α:                                                       jmp   n146_statement_end_α
n145_statement_begin_β:                                                       jmp   n147_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n146_statement_end_α:                                                         jmp   n147_statement_begin_α
#=======================================================================================================================
#         <stmt 18, line 50: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n147_statement_begin_α:                                                       jmp   n148_statement_end_α
n147_statement_begin_β:                                                       jmp   n149_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n148_statement_end_α:                                                         jmp   n149_statement_begin_α
#=======================================================================================================================
#         <stmt 19, line 53: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n149_statement_begin_α:                                                       jmp   n150_lit_string_α
n149_statement_begin_β:                                                       jmp   n154_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n150_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx388_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n151_lit_integer_α
.Lx388_0:               .quad            .Lx388_0_s
.Lx388_0_s:             .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n151_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx389_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n152_call_α
n151_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n149_statement_begin_β
.Lx389_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n152_call_α:            sub              rsp, 16
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
.Lrkfnzd391:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd391]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 32
                        cmp              eax, 104;                            jne   .Lx390_240
                        add              rsp, 16;                             jmp   n151_lit_integer_β
.Lx390_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n153_statement_end_α
n152_call_β:            add              rsp, 16;                             jmp   n151_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n153_statement_end_α:   add              rsp, 48;                             jmp   n154_statement_begin_α
#=======================================================================================================================
#         <stmt 20, line 53: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n154_statement_begin_α:                                                       jmp   n155_statement_end_α
n154_statement_begin_β:                                                       jmp   n156_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n155_statement_end_α:                                                         jmp   n156_statement_begin_α
#=======================================================================================================================
#         <stmt 21, line 55: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n156_statement_begin_α:                                                       jmp   n157_lit_string_α
n156_statement_begin_β:                                                       jmp   n161_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n157_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx400_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n158_call_α
.Lx400_0:               .quad            .Lx400_0_s
.Lx400_0_s:             .string          "HELLO WORLD"
#-----------------------------------------------------------------------------------------------------------------------
n158_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig402z]
                        lea              rax, [rip + lwr_alpha];              jmp   rax
.Lsig402z:              .quad            1
                        .quad            .Lx402_2
                        .quad            .Lx402_2
                        .quad            16
.Lx402_2:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx402_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n156_statement_begin_β
.Lx402_240:                                                                   jmp   n159_assign_α
n158_call_β:                                                                  jmp   n156_statement_begin_β
.Lx402_0:               .quad            .Lx402_0_s
.Lx402_0_s:             .string          "lwr"
#-----------------------------------------------------------------------------------------------------------------------
n159_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx403_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n160_statement_end_α
.Lx403_0:               .quad            .Lx403_0_s
.Lx403_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n160_statement_end_α:   add              rsp, 32;                             jmp   n161_statement_begin_α
#=======================================================================================================================
#         <stmt 22, line 56: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n161_statement_begin_α:                                                       jmp   n162_lit_string_α
n161_statement_begin_β:                                                       jmp   n166_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n162_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx408_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n163_call_α
.Lx408_0:               .quad            .Lx408_0_s
.Lx408_0_s:             .string          "hello world"
#-----------------------------------------------------------------------------------------------------------------------
n163_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig410z]
                        lea              rax, [rip + upr_alpha];              jmp   rax
.Lsig410z:              .quad            1
                        .quad            .Lx410_2
                        .quad            .Lx410_2
                        .quad            16
.Lx410_2:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx410_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n161_statement_begin_β
.Lx410_240:                                                                   jmp   n164_assign_α
n163_call_β:                                                                  jmp   n161_statement_begin_β
.Lx410_0:               .quad            .Lx410_0_s
.Lx410_0_s:             .string          "upr"
#-----------------------------------------------------------------------------------------------------------------------
n164_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx411_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n165_statement_end_α
.Lx411_0:               .quad            .Lx411_0_s
.Lx411_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n165_statement_end_α:   add              rsp, 32;                             jmp   n166_statement_begin_α
#=======================================================================================================================
#         <stmt 23, line 57: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n166_statement_begin_α:                                                       jmp   n167_lit_string_α
n166_statement_begin_β:                                                       jmp   n171_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n167_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx416_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n168_call_α
.Lx416_0:               .quad            .Lx416_0_s
.Lx416_0_s:             .string          "hELLO wORLD"
#-----------------------------------------------------------------------------------------------------------------------
n168_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig418z]
                        lea              rax, [rip + cap_alpha];              jmp   rax
.Lsig418z:              .quad            1
                        .quad            .Lx418_2
                        .quad            .Lx418_2
                        .quad            16
.Lx418_2:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx418_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n166_statement_begin_β
.Lx418_240:                                                                   jmp   n169_assign_α
n168_call_β:                                                                  jmp   n166_statement_begin_β
.Lx418_0:               .quad            .Lx418_0_s
.Lx418_0_s:             .string          "cap"
#-----------------------------------------------------------------------------------------------------------------------
n169_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx419_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n170_statement_end_α
.Lx419_0:               .quad            .Lx419_0_s
.Lx419_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n170_statement_end_α:   add              rsp, 32;                             jmp   n171_statement_begin_α
#=======================================================================================================================
#         <stmt 24, line 57: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n171_statement_begin_α:                                                       jmp   n172_statement_end_α
n171_statement_begin_β:                                                       jmp   n173_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n172_statement_end_α:                                                         jmp   n173_statement_begin_α
#=======================================================================================================================
#         <stmt 25, line 60: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n173_statement_begin_α:                                                       jmp   n174_lit_string_α
n173_statement_begin_β:                                                       jmp   n185_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n174_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx428_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n175_match_begin_α
.Lx428_0:               .quad            .Lx428_0_s
.Lx428_0_s:             .string          "Hello"
#-----------------------------------------------------------------------------------------------------------------------
n175_match_begin_α:     mov              rdi, qword ptr [rsp + 0]             # lit_string
                        mov              rsi, qword ptr [rsp + 8]
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
.Lx430_0:               mov              r14d, dword ptr [rbp + -40];         jmp   n176_lit_string_α
n175_match_begin_β:     lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx430_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx430_1
                                                                              jmp   .Lx430_0
.Lx430_1:
n175_match_begin_af:    mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 16;                             jmp   n185_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n176_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx431_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n177_call_α
.Lx431_0:               .quad            .Lx431_0_s
.Lx431_0_s:             .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n177_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig433z]
                        lea              rax, [rip + icase_alpha];            jmp   rax
.Lsig433z:              .quad            1
                        .quad            .Lx433_2
                        .quad            .Lx433_2
                        .quad            16
.Lx433_2:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx433_240
                        add              rsp, 16;                             jmp   n175_match_begin_α
.Lx433_240:                                                                   jmp   n178_match_value_α
n177_call_β:                                                                  jmp   n175_match_begin_α
.Lx433_0:               .quad            .Lx433_0_s
.Lx433_0_s:             .string          "icase"
#-----------------------------------------------------------------------------------------------------------------------
n178_match_value_α:     lea              rdi, [rsp + 0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_value_get_pat_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            jz    .Lx434_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx434_4]
                        lea              rdx, [rip + .Lx434_5];               jmp   rax
.Lx434_4:                                                                     jmp   n179_match_end_α
.Lx434_5:                                                                     jmp   n175_match_begin_β
.Lx434_0:               lea              rdi, [rsp + 0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_value_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              edi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_close@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            js    n175_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx434_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n179_match_end_α
.Lx434_6:               add              rsp, 16;                             jmp   n175_match_begin_β
n178_match_value_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n179_match_end_α:       push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              rdi, qword ptr [rbp + -8]            # cas_mark
                        mov              rdx, r13
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx436_1:               test             rax, rax;                            je    .Lx436_2
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx436_3]
                        lea              rdx, [rip + .Lx436_4];               jmp   rax
.Lx436_3:               mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_step@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx436_1
.Lx436_4:               mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_step@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx436_1
.Lx436_2:               call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n180_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n180_statement_end_α:   add              rsp, 16;                             jmp   n181_statement_begin_α
#=======================================================================================================================
#         <stmt 26, line 61: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n181_statement_begin_α:                                                       jmp   n182_lit_string_α
n181_statement_begin_β:                                                       jmp   n185_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n182_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 15
                        mov              rax, qword ptr [rip + .Lx441_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n183_assign_α
.Lx441_0:               .quad            .Lx441_0_s
.Lx441_0_s:             .string          "ok: icase hello"
#-----------------------------------------------------------------------------------------------------------------------
n183_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx442_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n184_statement_end_α
.Lx442_0:               .quad            .Lx442_0_s
.Lx442_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n184_statement_end_α:   add              rsp, 16;                             jmp   n185_statement_begin_α
#=======================================================================================================================
# bad_ic1  <stmt 27, line 63: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n185_statement_begin_α:                                                       jmp   n186_statement_end_α
n185_statement_begin_β:                                                       jmp   n187_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n186_statement_end_α:                                                         jmp   n187_statement_begin_α
#=======================================================================================================================
#         <stmt 28, line 63: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n187_statement_begin_α:                                                       jmp   n188_lit_string_α
n187_statement_begin_β:                                                       jmp   n199_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n188_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx451_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n189_match_begin_α
.Lx451_0:               .quad            .Lx451_0_s
.Lx451_0_s:             .string          "HELLO"
#-----------------------------------------------------------------------------------------------------------------------
n189_match_begin_α:     mov              rdi, qword ptr [rsp + 0]             # lit_string
                        mov              rsi, qword ptr [rsp + 8]
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
.Lx453_0:               mov              r14d, dword ptr [rbp + -40];         jmp   n190_lit_string_α
n189_match_begin_β:     lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx453_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx453_1
                                                                              jmp   .Lx453_0
.Lx453_1:
n189_match_begin_af:    mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 16;                             jmp   n199_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n190_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx454_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n191_call_α
.Lx454_0:               .quad            .Lx454_0_s
.Lx454_0_s:             .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n191_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig456z]
                        lea              rax, [rip + icase_alpha];            jmp   rax
.Lsig456z:              .quad            1
                        .quad            .Lx456_2
                        .quad            .Lx456_2
                        .quad            16
.Lx456_2:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx456_240
                        add              rsp, 16;                             jmp   n189_match_begin_α
.Lx456_240:                                                                   jmp   n192_match_value_α
n191_call_β:                                                                  jmp   n189_match_begin_α
.Lx456_0:               .quad            .Lx456_0_s
.Lx456_0_s:             .string          "icase"
#-----------------------------------------------------------------------------------------------------------------------
n192_match_value_α:     lea              rdi, [rsp + 0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_value_get_pat_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            jz    .Lx457_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx457_4]
                        lea              rdx, [rip + .Lx457_5];               jmp   rax
.Lx457_4:                                                                     jmp   n193_match_end_α
.Lx457_5:                                                                     jmp   n189_match_begin_β
.Lx457_0:               lea              rdi, [rsp + 0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_value_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              edi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_close@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            js    n189_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx457_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n193_match_end_α
.Lx457_6:               add              rsp, 16;                             jmp   n189_match_begin_β
n192_match_value_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n193_match_end_α:       push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              rdi, qword ptr [rbp + -8]            # cas_mark
                        mov              rdx, r13
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx459_1:               test             rax, rax;                            je    .Lx459_2
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx459_3]
                        lea              rdx, [rip + .Lx459_4];               jmp   rax
.Lx459_3:               mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_step@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx459_1
.Lx459_4:               mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_step@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx459_1
.Lx459_2:               call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n194_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n194_statement_end_α:   add              rsp, 16;                             jmp   n195_statement_begin_α
#=======================================================================================================================
#         <stmt 29, line 64: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n195_statement_begin_α:                                                       jmp   n196_lit_string_α
n195_statement_begin_β:                                                       jmp   n199_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n196_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 15
                        mov              rax, qword ptr [rip + .Lx464_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n197_assign_α
.Lx464_0:               .quad            .Lx464_0_s
.Lx464_0_s:             .string          "ok: icase HELLO"
#-----------------------------------------------------------------------------------------------------------------------
n197_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx465_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n198_statement_end_α
.Lx465_0:               .quad            .Lx465_0_s
.Lx465_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n198_statement_end_α:   add              rsp, 16;                             jmp   n199_statement_begin_α
#=======================================================================================================================
# bad_ic2  <stmt 30, line 66: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n199_statement_begin_α:                                                       jmp   n200_statement_end_α
n199_statement_begin_β:                                                       jmp   n201_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n200_statement_end_α:                                                         jmp   n201_statement_begin_α
#=======================================================================================================================
#         <stmt 31, line 66: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n201_statement_begin_α:                                                       jmp   n202_lit_string_α
n201_statement_begin_β:                                                       jmp   n213_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n202_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx474_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n203_match_begin_α
.Lx474_0:               .quad            .Lx474_0_s
.Lx474_0_s:             .string          "HeLLo"
#-----------------------------------------------------------------------------------------------------------------------
n203_match_begin_α:     mov              rdi, qword ptr [rsp + 0]             # lit_string
                        mov              rsi, qword ptr [rsp + 8]
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
.Lx476_0:               mov              r14d, dword ptr [rbp + -40];         jmp   n204_lit_string_α
n203_match_begin_β:     lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx476_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx476_1
                                                                              jmp   .Lx476_0
.Lx476_1:
n203_match_begin_af:    mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 16;                             jmp   n213_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n204_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx477_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n205_call_α
.Lx477_0:               .quad            .Lx477_0_s
.Lx477_0_s:             .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n205_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig479z]
                        lea              rax, [rip + icase_alpha];            jmp   rax
.Lsig479z:              .quad            1
                        .quad            .Lx479_2
                        .quad            .Lx479_2
                        .quad            16
.Lx479_2:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx479_240
                        add              rsp, 16;                             jmp   n203_match_begin_α
.Lx479_240:                                                                   jmp   n206_match_value_α
n205_call_β:                                                                  jmp   n203_match_begin_α
.Lx479_0:               .quad            .Lx479_0_s
.Lx479_0_s:             .string          "icase"
#-----------------------------------------------------------------------------------------------------------------------
n206_match_value_α:     lea              rdi, [rsp + 0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_value_get_pat_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            jz    .Lx480_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx480_4]
                        lea              rdx, [rip + .Lx480_5];               jmp   rax
.Lx480_4:                                                                     jmp   n207_match_end_α
.Lx480_5:                                                                     jmp   n203_match_begin_β
.Lx480_0:               lea              rdi, [rsp + 0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_value_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              edi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_close@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            js    n203_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx480_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n207_match_end_α
.Lx480_6:               add              rsp, 16;                             jmp   n203_match_begin_β
n206_match_value_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n207_match_end_α:       push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              rdi, qword ptr [rbp + -8]            # cas_mark
                        mov              rdx, r13
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx482_1:               test             rax, rax;                            je    .Lx482_2
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx482_3]
                        lea              rdx, [rip + .Lx482_4];               jmp   rax
.Lx482_3:               mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_step@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx482_1
.Lx482_4:               mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_step@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx482_1
.Lx482_2:               call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n208_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n208_statement_end_α:   add              rsp, 16;                             jmp   n209_statement_begin_α
#=======================================================================================================================
#         <stmt 32, line 67: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n209_statement_begin_α:                                                       jmp   n210_lit_string_α
n209_statement_begin_β:                                                       jmp   n213_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n210_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 15
                        mov              rax, qword ptr [rip + .Lx487_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n211_assign_α
.Lx487_0:               .quad            .Lx487_0_s
.Lx487_0_s:             .string          "ok: icase HeLLo"
#-----------------------------------------------------------------------------------------------------------------------
n211_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx488_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n212_statement_end_α
.Lx488_0:               .quad            .Lx488_0_s
.Lx488_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n212_statement_end_α:   add              rsp, 16;                             jmp   n213_statement_begin_α
#=======================================================================================================================
# bad_ic3  <stmt 33, line 69: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n213_statement_begin_α:                                                       jmp   n214_statement_end_α
n213_statement_begin_β:                                                       jmp   n215_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n214_statement_end_α:                                                         jmp   n215_statement_begin_α
#=======================================================================================================================
#         <stmt 34, line 69: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n215_statement_begin_α:                                                       jmp   n216_lit_string_α
n215_statement_begin_β:                                                       jmp   n223_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n216_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx497_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n217_match_begin_α
.Lx497_0:               .quad            .Lx497_0_s
.Lx497_0_s:             .string          "world"
#-----------------------------------------------------------------------------------------------------------------------
n217_match_begin_α:     mov              rdi, qword ptr [rsp + 0]             # lit_string
                        mov              rsi, qword ptr [rsp + 8]
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
.Lx499_0:               mov              r14d, dword ptr [rbp + -40];         jmp   n218_lit_string_α
n217_match_begin_β:     lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx499_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx499_1
                                                                              jmp   .Lx499_0
.Lx499_1:
n217_match_begin_af:    mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 16;                             jmp   n223_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n218_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx500_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n219_call_α
.Lx500_0:               .quad            .Lx500_0_s
.Lx500_0_s:             .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n219_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig502z]
                        lea              rax, [rip + icase_alpha];            jmp   rax
.Lsig502z:              .quad            1
                        .quad            .Lx502_2
                        .quad            .Lx502_2
                        .quad            16
.Lx502_2:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx502_240
                        add              rsp, 16;                             jmp   n217_match_begin_α
.Lx502_240:                                                                   jmp   n220_match_value_α
n219_call_β:                                                                  jmp   n217_match_begin_α
.Lx502_0:               .quad            .Lx502_0_s
.Lx502_0_s:             .string          "icase"
#-----------------------------------------------------------------------------------------------------------------------
n220_match_value_α:     lea              rdi, [rsp + 0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_value_get_pat_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            jz    .Lx503_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx503_4]
                        lea              rdx, [rip + .Lx503_5];               jmp   rax
.Lx503_4:                                                                     jmp   n221_match_end_α
.Lx503_5:                                                                     jmp   n217_match_begin_β
.Lx503_0:               lea              rdi, [rsp + 0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_value_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              edi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_close@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            js    n217_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx503_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n221_match_end_α
.Lx503_6:               add              rsp, 16;                             jmp   n217_match_begin_β
n220_match_value_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n221_match_end_α:       push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              rdi, qword ptr [rbp + -8]            # cas_mark
                        mov              rdx, r13
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_end_ok_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx505_1:               test             rax, rax;                            je    .Lx505_2
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx505_3]
                        lea              rdx, [rip + .Lx505_4];               jmp   rax
.Lx505_3:               mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_step@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx505_1
.Lx505_4:               mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_dcap_step@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx505_1
.Lx505_2:               call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n222_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n222_statement_end_α:   add              rsp, 16;                             jmp   n227_statement_begin_α
#=======================================================================================================================
#         <stmt 35, line 70: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n223_statement_begin_α:                                                       jmp   n224_lit_string_α
n223_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n224_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx510_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n225_assign_α
.Lx510_0:               .quad            .Lx510_0_s
.Lx510_0_s:             .string          "no match ok"
#-----------------------------------------------------------------------------------------------------------------------
n225_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx511_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n226_statement_end_α
.Lx511_0:               .quad            .Lx511_0_s
.Lx511_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n226_statement_end_α:   add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# bad_ic4  <stmt 36, line 71: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n227_statement_begin_α:                                                       jmp   n228_lit_string_α
n227_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n228_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 32
                        mov              rax, qword ptr [rip + .Lx516_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n229_assign_α
.Lx516_0:               .quad            .Lx516_0_s
.Lx516_0_s:             .string          "FAIL: icase matched wrong string"
#-----------------------------------------------------------------------------------------------------------------------
n229_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx517_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n230_statement_end_α
.Lx517_0:               .quad            .Lx517_0_s
.Lx517_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n230_statement_end_α:   add              rsp, 16;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 pop              rcx
                        add              rsp, 8;                              jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                add              rsp, 8
                        pop              rcx;                                 jmp   rcx
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
.S0:                    .string          "PAT$0$V0"
.S1:                    .string          "letter"
.S2:                    .string          "ch"
                        .text
                        .section         .rodata
.C0:                    .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                        .byte            0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
                        .byte            1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0
                        .byte            0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
                        .byte            1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0
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
