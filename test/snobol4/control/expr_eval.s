                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Push_α
proc_Push_α:
#-----------------------------------------------------------------------------------------------------------------------
n0_save_restore_α:                                                            jmp   n1_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_deferred_α:     lea              rax, [rip + Push_body];              jmp   rax
                                                                              jmp   n2_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n2_save_restore_α:
Push_alpha:             sub              rsp, 64
                        mov              rax, qword ptr [r9 + 0]              # Push
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx8_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # x
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx8_41
.Lx8_10:                mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx8_41:                lea              r10, [rip + Push_gamma]
                        lea              r11, [rip + Push_omega]
                        push             r11
                        push             r10
                        lea              rax, [rip + Push_body];              jmp   rax
Push_gamma:             mov              rdi, qword ptr [r9 + 0]              # Push
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rdx, 0;                              jbe   .Lx8_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # x
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx8_110
.Lx8_80:                mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx8_110:               mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 64
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
Push_omega:             mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax              # Push
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rdx, 0;                              jbe   .Lx8_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # x
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx8_180
.Lx8_150:               mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx8_180:               mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Pop_α
proc_Pop_α:
#-----------------------------------------------------------------------------------------------------------------------
n9_save_restore_α:                                                            jmp   n10_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n10_goto_deferred_α:    lea              rax, [rip + Pop_body];               jmp   rax
                                                                              jmp   n11_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n11_save_restore_α:
Pop_alpha:              sub              rsp, 48
                        mov              rax, qword ptr [r9 + 32]             # Pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        lea              r10, [rip + Pop_gamma]
                        lea              r11, [rip + Pop_omega]
                        push             r11
                        push             r10
                        lea              rax, [rip + Pop_body];               jmp   rax
Pop_gamma:              mov              rdi, qword ptr [r9 + 32]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
Pop_omega:              mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Unary_α
proc_Unary_α:
#-----------------------------------------------------------------------------------------------------------------------
n18_save_restore_α:                                                           jmp   n19_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n19_goto_deferred_α:    lea              rax, [rip + Unary_body];             jmp   rax
                                                                              jmp   n20_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n20_save_restore_α:
Unary_alpha:            sub              rsp, 80
                        mov              rax, qword ptr [r9 + 48]             # Unary
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        cmp              rdx, 0;                              jbe   .Lx26_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 64]             # arg
                        mov              qword ptr [r9 + 64], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 72]
                        mov              qword ptr [r9 + 72], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx26_41
.Lx26_10:               mov              rax, qword ptr [r9 + 64]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
.Lx26_41:               cmp              rdx, 1;                              jbe   .Lx26_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 80]             # op
                        mov              qword ptr [r9 + 80], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 88]
                        mov              qword ptr [r9 + 88], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx26_42
.Lx26_11:               mov              rax, qword ptr [r9 + 80]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
.Lx26_42:               lea              r10, [rip + Unary_gamma]
                        lea              r11, [rip + Unary_omega]
                        push             r11
                        push             r10
                        lea              rax, [rip + Unary_body];             jmp   rax
Unary_gamma:            mov              rdi, qword ptr [r9 + 48]             # Unary
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 56], rax
                        cmp              rdx, 0;                              jbe   .Lx26_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 64], rax             # arg
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx26_110
.Lx26_80:               mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 72], rax
.Lx26_110:              cmp              rdx, 1;                              jbe   .Lx26_81
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 88], rax;            jmp   .Lx26_111
.Lx26_81:               mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 88], rax
.Lx26_111:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 80
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
Unary_omega:            mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 48], rax             # Unary
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 56], rax
                        cmp              rdx, 0;                              jbe   .Lx26_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 64], rax             # arg
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx26_180
.Lx26_150:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 72], rax
.Lx26_180:              cmp              rdx, 1;                              jbe   .Lx26_151
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 88], rax;            jmp   .Lx26_181
.Lx26_151:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 88], rax
.Lx26_181:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 80
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_Binary_α
proc_Binary_α:
#-----------------------------------------------------------------------------------------------------------------------
n27_save_restore_α:                                                           jmp   n28_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n28_goto_deferred_α:    lea              rax, [rip + Binary_body];            jmp   rax
                                                                              jmp   n29_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n29_save_restore_α:
Binary_alpha:           sub              rsp, 96
                        mov              rax, qword ptr [r9 + 96]             # Binary
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        cmp              rdx, 0;                              jbe   .Lx35_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 80]             # op
                        mov              qword ptr [r9 + 80], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 88]
                        mov              qword ptr [r9 + 88], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx35_41
.Lx35_10:               mov              rax, qword ptr [r9 + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
.Lx35_41:               cmp              rdx, 1;                              jbe   .Lx35_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 112]            # left
                        mov              qword ptr [r9 + 112], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 120]
                        mov              qword ptr [r9 + 120], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx35_42
.Lx35_11:               mov              rax, qword ptr [r9 + 112]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 112], 0
                        mov              qword ptr [r9 + 120], 0
.Lx35_42:               cmp              rdx, 2;                              jbe   .Lx35_12
                        mov              rdi, qword ptr [rcx + 40]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 128]            # right
                        mov              qword ptr [r9 + 128], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 136]
                        mov              qword ptr [r9 + 136], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx35_43
.Lx35_12:               mov              rax, qword ptr [r9 + 128]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
.Lx35_43:               lea              r10, [rip + Binary_gamma]
                        lea              r11, [rip + Binary_omega]
                        push             r11
                        push             r10
                        lea              rax, [rip + Binary_body];            jmp   rax
Binary_gamma:           mov              rdi, qword ptr [r9 + 96]             # Binary
                        mov              rsi, qword ptr [r9 + 104]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rdx, 0;                              jbe   .Lx35_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 88], rax;            jmp   .Lx35_110
.Lx35_80:               mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 88], rax
.Lx35_110:              cmp              rdx, 1;                              jbe   .Lx35_81
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 112], rax            # left
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 120], rax;           jmp   .Lx35_111
.Lx35_81:               mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 120], rax
.Lx35_111:              cmp              rdx, 2;                              jbe   .Lx35_82
                        mov              r11, qword ptr [rcx + 40]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 128], rax            # right
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx35_112
.Lx35_82:               mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 136], rax
.Lx35_112:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 96
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
Binary_omega:           mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax             # Binary
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rdx, 0;                              jbe   .Lx35_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 88], rax;            jmp   .Lx35_180
.Lx35_150:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 88], rax
.Lx35_180:              cmp              rdx, 1;                              jbe   .Lx35_151
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 112], rax            # left
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 120], rax;           jmp   .Lx35_181
.Lx35_151:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 120], rax
.Lx35_181:              cmp              rdx, 2;                              jbe   .Lx35_152
                        mov              r11, qword ptr [rcx + 40]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 128], rax            # right
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx35_182
.Lx35_152:              mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 136], rax
.Lx35_182:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 96
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$0_α
proc_PAT$0_α:
proc_PAT$0_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n36_match_span_α:       sub              rsp, 16
                        lea              rdi, [rip + .C0]
                        movsxd           rcx, r14d
.Lx38_0:                cmp              ecx, r15d;                           jge   .Lx38_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              byte ptr [rdi+rsi], 0;               je    .Lx38_1
                        add              ecx, 1;                              jmp   .Lx38_0
.Lx38_1:                cmp              ecx, r14d;                           jg    .Lx38_240
                        add              rsp, 16;                             jmp   proc_PAT$0_ω
.Lx38_240:              mov              dword ptr [rsp + 4], r14d
                        mov              r14d, ecx;                           jmp   proc_PAT$0_γ
n36_match_span_β:       mov              r14d, dword ptr [rsp + 4]
                        add              rsp, 16;                             jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_res:
                        mov              r10, qword ptr [rsp + 8]
                        mov              r11, qword ptr [rsp + 16]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_β:
                                                                              jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_γ:
                        sub              rsp, 8
                        push             r11
                        push             r10
                        lea              rax, [rip + proc_PAT$0_res]
                        push             rax;                                 jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_ω:
                                                                              jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$1_α
proc_PAT$1_α:
proc_PAT$1_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 24
                        mov              qword ptr [rbp + -8], r10
                        mov              qword ptr [rbp + -16], r11
#-----------------------------------------------------------------------------------------------------------------------
n39_match_any_α:        mov              eax, r14d
                        cmp              eax, r15d;                           jge   proc_PAT$1_ω
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        lea              rdi, [rip + .C1]
                        cmp              byte ptr [rdi+rsi], 0;               je    proc_PAT$1_ω
                        add              r14d, 1;                             jmp   n40_match_alternate_α
n39_match_any_β:        sub              r14d, 1;                             jmp   proc_PAT$1_ω
#-----------------------------------------------------------------------------------------------------------------------
n40_match_alternate_α:  sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx47_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n43_match_any_α
.Lx47_21:               lea              rax, [rip + .Lx47_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n42_match_defer_α
n40_match_alternate_s0: lea              rax, [rip + .Lx47_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n40_match_alternate_as
n40_match_alternate_s1: lea              rax, [rip + .Lx47_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n40_match_alternate_as
.Lx47_40:                                                                     jmp   n43_match_any_β
.Lx47_41:                                                                     jmp   n42_match_defer_β
n40_match_alternate_as:                                                       jmp   n41_match_defer_α
n40_match_alternate_β:  mov              rax, qword ptr [rsp + 8];            jmp   rax
n40_match_alternate_af: mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx47_19:               add              rsp, 32;                             jmp   n39_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n41_match_defer_α:      sub              rsp, 16
                        mov              rax, qword ptr [r9 + 384]            # PAT$1$V1
                        mov              rdx, qword ptr [r9 + 392]
                        cmp              eax, 8;                              jne   .Lx48_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx48_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx48_10
.Lx48_9:                xor              eax, eax
.Lx48_10:               test             rax, rax;                            jz    .Lx48_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx48_4]
                        lea              r11, [rip + .Lx48_5];                jmp   rax
.Lx48_4:                                                                      jmp   proc_PAT$1_γ
.Lx48_5:                add              rsp, 16;                             jmp   n40_match_alternate_β
.Lx48_0:                push             r14
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
.Lx48_2:                test             rax, rax;                            je    .Lx48_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx48_7]
                        lea              rdx, [rip + .Lx48_8];                jmp   rax
.Lx48_7:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx48_2
.Lx48_8:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx48_2
.Lx48_3:                add              rsp, 8
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
                        test             eax, eax;                            jns   .Lx48_240
                        add              rsp, 16;                             jmp   n40_match_alternate_β
.Lx48_240:              mov              r14d, eax
                        lea              rax, [rip + .Lx48_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   proc_PAT$1_γ
.Lx48_6:                add              rsp, 16
                        add              rsp, 16;                             jmp   n40_match_alternate_β
n41_match_defer_β:                                                            jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n42_match_defer_α:      mov              rax, qword ptr [r9 + 368]            # PAT$1$V0
                        mov              rdx, qword ptr [r9 + 376]
                        cmp              eax, 8;                              jne   .Lx49_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx49_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx49_10
.Lx49_9:                xor              eax, eax
.Lx49_10:               test             rax, rax;                            jz    .Lx49_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx49_4]
                        lea              r11, [rip + .Lx49_5];                jmp   rax
.Lx49_4:                                                                      jmp   n40_match_alternate_s1
.Lx49_5:                                                                      jmp   n40_match_alternate_af
.Lx49_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx49_2:                test             rax, rax;                            je    .Lx49_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx49_7]
                        lea              rdx, [rip + .Lx49_8];                jmp   rax
.Lx49_7:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx49_2
.Lx49_8:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx49_2
.Lx49_3:                add              rsp, 8
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
                        test             eax, eax;                            js    n40_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx49_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n40_match_alternate_s1
.Lx49_6:                add              rsp, 16;                             jmp   n40_match_alternate_af
n42_match_defer_β:                                                            jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n43_match_any_α:        mov              eax, r14d
                        cmp              eax, r15d;                           jge   n40_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 43;                             je    .Lx51_0
                        cmp              esi, 45;                             je    .Lx51_0
                                                                              jmp   n40_match_alternate_af
.Lx51_0:                add              r14d, 1;                             jmp   n40_match_alternate_s0
n43_match_any_β:        sub              r14d, 1;                             jmp   n40_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_res:
                        mov              r10, qword ptr [rsp + 8]
                        mov              r11, qword ptr [rsp + 16]
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_β:
                                                                              jmp   proc_PAT$1_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_γ:
                        mov              r10, qword ptr [rbp + -8]
                        mov              r11, qword ptr [rbp + -16]
                        push             rbp
                        push             r11
                        push             r10
                        lea              rax, [rip + proc_PAT$1_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$1_ω:
                        mov              r10, qword ptr [rbp + -8]
                        mov              r11, qword ptr [rbp + -16]
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$2_α
proc_PAT$2_α:
proc_PAT$2_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 24
                        mov              qword ptr [rbp + -8], r10
                        mov              qword ptr [rbp + -16], r11
#-----------------------------------------------------------------------------------------------------------------------
n52_match_alternate_α:  sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx66_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n57_match_defer_α
.Lx66_21:               lea              rax, [rip + .Lx66_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n54_match_defer_α
n52_match_alternate_s0: lea              rax, [rip + .Lx66_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n52_match_alternate_as
n52_match_alternate_s1: lea              rax, [rip + .Lx66_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n52_match_alternate_as
.Lx66_40:                                                                     jmp   n56_goto_β
.Lx66_41:                                                                     jmp   n53_goto_β
n52_match_alternate_as:                                                       jmp   proc_PAT$2_γ
n52_match_alternate_β:  mov              rax, qword ptr [rsp + 8];            jmp   rax
n52_match_alternate_af: mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx66_19:               add              rsp, 32;                             jmp   proc_PAT$2_ω
#-----------------------------------------------------------------------------------------------------------------------
n53_goto_α:                                                                   jmp   n52_match_alternate_af
n53_goto_β:                                                                   jmp   n52_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n54_match_defer_α:      mov              rax, qword ptr [r9 + 480]            # PAT$2$V5
                        mov              rdx, qword ptr [r9 + 488]
                        cmp              eax, 8;                              jne   .Lx68_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx68_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx68_10
.Lx68_9:                xor              eax, eax
.Lx68_10:               test             rax, rax;                            jz    .Lx68_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx68_4]
                        lea              r11, [rip + .Lx68_5];                jmp   rax
.Lx68_4:                                                                      jmp   n55_match_defer_α
.Lx68_5:                                                                      jmp   n52_match_alternate_af
.Lx68_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S2]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx68_2:                test             rax, rax;                            je    .Lx68_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx68_7]
                        lea              rdx, [rip + .Lx68_8];                jmp   rax
.Lx68_7:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx68_2
.Lx68_8:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx68_2
.Lx68_3:                add              rsp, 8
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
                        test             eax, eax;                            js    n52_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx68_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n55_match_defer_α
.Lx68_6:                add              rsp, 16;                             jmp   n52_match_alternate_af
n54_match_defer_β:                                                            jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n55_match_defer_α:      mov              rax, qword ptr [r9 + 496]            # PAT$2$V6
                        mov              rdx, qword ptr [r9 + 504]
                        cmp              eax, 8;                              jne   .Lx69_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx69_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx69_10
.Lx69_9:                xor              eax, eax
.Lx69_10:               test             rax, rax;                            jz    .Lx69_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx69_4]
                        lea              r11, [rip + .Lx69_5];                jmp   rax
.Lx69_4:                                                                      jmp   n52_match_alternate_s1
.Lx69_5:                                                                      jmp   n52_match_alternate_af
.Lx69_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S3]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx69_2:                test             rax, rax;                            je    .Lx69_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx69_7]
                        lea              rdx, [rip + .Lx69_8];                jmp   rax
.Lx69_7:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx69_2
.Lx69_8:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx69_2
.Lx69_3:                add              rsp, 8
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
                        test             eax, eax;                            js    n52_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx69_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n52_match_alternate_s1
.Lx69_6:                add              rsp, 16;                             jmp   n52_match_alternate_af
n55_match_defer_β:                                                            jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n56_goto_α:                                                                   jmp   n52_match_alternate_af
n56_goto_β:                                                                   jmp   n52_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n57_match_defer_α:      mov              rax, qword ptr [r9 + 400]            # PAT$2$V0
                        mov              rdx, qword ptr [r9 + 408]
                        cmp              eax, 8;                              jne   .Lx71_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx71_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx71_10
.Lx71_9:                xor              eax, eax
.Lx71_10:               test             rax, rax;                            jz    .Lx71_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx71_4]
                        lea              r11, [rip + .Lx71_5];                jmp   rax
.Lx71_4:                                                                      jmp   n58_match_lit_α
.Lx71_5:                                                                      jmp   n52_match_alternate_af
.Lx71_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S4]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx71_2:                test             rax, rax;                            je    .Lx71_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx71_7]
                        lea              rdx, [rip + .Lx71_8];                jmp   rax
.Lx71_7:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx71_2
.Lx71_8:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx71_2
.Lx71_3:                add              rsp, 8
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
                        test             eax, eax;                            js    n52_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx71_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n58_match_lit_α
.Lx71_6:                add              rsp, 16;                             jmp   n52_match_alternate_af
n57_match_defer_β:                                                            jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n58_match_lit_α:        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n52_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 46;                             jne   n52_match_alternate_af
                        add              r14d, 1;                             jmp   n59_match_alternate_α
n58_match_lit_β:        sub              r14d, 1;                             jmp   n52_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n59_match_alternate_α:  sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx75_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n64_match_defer_α
.Lx75_21:               lea              rax, [rip + .Lx75_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n63_match_defer_α
n59_match_alternate_s0: lea              rax, [rip + .Lx75_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n59_match_alternate_as
n59_match_alternate_s1: lea              rax, [rip + .Lx75_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n59_match_alternate_as
.Lx75_40:                                                                     jmp   n64_match_defer_β
.Lx75_41:                                                                     jmp   n63_match_defer_β
n59_match_alternate_as:                                                       jmp   n60_match_alternate_α
n59_match_alternate_β:  mov              rax, qword ptr [rsp + 8];            jmp   rax
n59_match_alternate_af: mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx75_19:               add              rsp, 32;                             jmp   n58_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n60_match_alternate_α:  sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx77_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n62_match_defer_α
.Lx77_21:               lea              rax, [rip + .Lx77_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n61_match_defer_α
n60_match_alternate_s0: lea              rax, [rip + .Lx77_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n60_match_alternate_as
n60_match_alternate_s1: lea              rax, [rip + .Lx77_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n60_match_alternate_as
.Lx77_40:                                                                     jmp   n62_match_defer_β
.Lx77_41:                                                                     jmp   n61_match_defer_β
n60_match_alternate_as:                                                       jmp   n52_match_alternate_s0
n60_match_alternate_β:  mov              rax, qword ptr [rsp + 8];            jmp   rax
n60_match_alternate_af: mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx77_19:               add              rsp, 32;                             jmp   n59_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n61_match_defer_α:      mov              rax, qword ptr [r9 + 464]            # PAT$2$V4
                        mov              rdx, qword ptr [r9 + 472]
                        cmp              eax, 8;                              jne   .Lx78_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx78_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx78_10
.Lx78_9:                xor              eax, eax
.Lx78_10:               test             rax, rax;                            jz    .Lx78_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx78_4]
                        lea              r11, [rip + .Lx78_5];                jmp   rax
.Lx78_4:                                                                      jmp   n60_match_alternate_s1
.Lx78_5:                                                                      jmp   n60_match_alternate_af
.Lx78_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S5]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx78_2:                test             rax, rax;                            je    .Lx78_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx78_7]
                        lea              rdx, [rip + .Lx78_8];                jmp   rax
.Lx78_7:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx78_2
.Lx78_8:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx78_2
.Lx78_3:                add              rsp, 8
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
                        test             eax, eax;                            js    n60_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx78_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n60_match_alternate_s1
.Lx78_6:                add              rsp, 16;                             jmp   n60_match_alternate_af
n61_match_defer_β:                                                            jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n62_match_defer_α:      mov              rax, qword ptr [r9 + 448]            # PAT$2$V3
                        mov              rdx, qword ptr [r9 + 456]
                        cmp              eax, 8;                              jne   .Lx79_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx79_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx79_10
.Lx79_9:                xor              eax, eax
.Lx79_10:               test             rax, rax;                            jz    .Lx79_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx79_4]
                        lea              r11, [rip + .Lx79_5];                jmp   rax
.Lx79_4:                                                                      jmp   n60_match_alternate_s0
.Lx79_5:                                                                      jmp   n60_match_alternate_af
.Lx79_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S6]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx79_2:                test             rax, rax;                            je    .Lx79_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx79_7]
                        lea              rdx, [rip + .Lx79_8];                jmp   rax
.Lx79_7:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx79_2
.Lx79_8:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx79_2
.Lx79_3:                add              rsp, 8
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
                        test             eax, eax;                            js    n60_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx79_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n60_match_alternate_s0
.Lx79_6:                add              rsp, 16;                             jmp   n60_match_alternate_af
n62_match_defer_β:                                                            jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n63_match_defer_α:      mov              rax, qword ptr [r9 + 432]            # PAT$2$V2
                        mov              rdx, qword ptr [r9 + 440]
                        cmp              eax, 8;                              jne   .Lx80_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx80_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx80_10
.Lx80_9:                xor              eax, eax
.Lx80_10:               test             rax, rax;                            jz    .Lx80_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx80_4]
                        lea              r11, [rip + .Lx80_5];                jmp   rax
.Lx80_4:                                                                      jmp   n59_match_alternate_s1
.Lx80_5:                                                                      jmp   n59_match_alternate_af
.Lx80_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S7]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx80_2:                test             rax, rax;                            je    .Lx80_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx80_7]
                        lea              rdx, [rip + .Lx80_8];                jmp   rax
.Lx80_7:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx80_2
.Lx80_8:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx80_2
.Lx80_3:                add              rsp, 8
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
                        test             eax, eax;                            js    n59_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx80_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n59_match_alternate_s1
.Lx80_6:                add              rsp, 16;                             jmp   n59_match_alternate_af
n63_match_defer_β:                                                            jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n64_match_defer_α:      mov              rax, qword ptr [r9 + 416]            # PAT$2$V1
                        mov              rdx, qword ptr [r9 + 424]
                        cmp              eax, 8;                              jne   .Lx81_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx81_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx81_10
.Lx81_9:                xor              eax, eax
.Lx81_10:               test             rax, rax;                            jz    .Lx81_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx81_4]
                        lea              r11, [rip + .Lx81_5];                jmp   rax
.Lx81_4:                                                                      jmp   n59_match_alternate_s0
.Lx81_5:                                                                      jmp   n59_match_alternate_af
.Lx81_0:                push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S8]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx81_2:                test             rax, rax;                            je    .Lx81_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx81_7]
                        lea              rdx, [rip + .Lx81_8];                jmp   rax
.Lx81_7:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx81_2
.Lx81_8:                mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx81_2
.Lx81_3:                add              rsp, 8
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
                        test             eax, eax;                            js    n59_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx81_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n59_match_alternate_s0
.Lx81_6:                add              rsp, 16;                             jmp   n59_match_alternate_af
n64_match_defer_β:                                                            jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_res:
                        mov              r10, qword ptr [rsp + 8]
                        mov              r11, qword ptr [rsp + 16]
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_β:
                                                                              jmp   proc_PAT$2_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_γ:
                        mov              r10, qword ptr [rbp + -8]
                        mov              r11, qword ptr [rbp + -16]
                        push             rbp
                        push             r11
                        push             r10
                        lea              rax, [rip + proc_PAT$2_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$2_ω:
                        mov              r10, qword ptr [rbp + -8]
                        mov              r11, qword ptr [rbp + -16]
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$3_α
proc_PAT$3_α:
proc_PAT$3_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n82_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n83_match_any_α
n82_match_assign_save_β:
                        add              rsp, 16;                             jmp   proc_PAT$3_ω
#-----------------------------------------------------------------------------------------------------------------------
n83_match_any_α:        mov              eax, r14d
                        cmp              eax, r15d;                           jl    .Lx88_240
                        add              rsp, 16;                             jmp   proc_PAT$3_ω
.Lx88_240:              movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 43;                             je    .Lx88_0
                        cmp              esi, 45;                             je    .Lx88_0
                        add              rsp, 16;                             jmp   proc_PAT$3_ω
.Lx88_0:                add              r14d, 1;                             jmp   n84_match_assign_cond_α
n83_match_any_β:        sub              r14d, 1
                        add              rsp, 16;                             jmp   proc_PAT$3_ω
#-----------------------------------------------------------------------------------------------------------------------
n84_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S9]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   proc_PAT$3_γ
n84_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n83_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_res:
                        mov              r10, qword ptr [rsp + 8]
                        mov              r11, qword ptr [rsp + 16]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_β:
                                                                              jmp   proc_PAT$3_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_γ:
                        sub              rsp, 8
                        push             r11
                        push             r10
                        lea              rax, [rip + proc_PAT$3_res]
                        push             rax;                                 jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$3_ω:
                                                                              jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$4_α
proc_PAT$4_α:
proc_PAT$4_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n91_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n92_match_any_α
n91_match_assign_save_β:
                        add              rsp, 16;                             jmp   proc_PAT$4_ω
#-----------------------------------------------------------------------------------------------------------------------
n92_match_any_α:        mov              eax, r14d
                        cmp              eax, r15d;                           jl    .Lx97_240
                        add              rsp, 16;                             jmp   proc_PAT$4_ω
.Lx97_240:              movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 42;                             je    .Lx97_0
                        cmp              esi, 47;                             je    .Lx97_0
                        add              rsp, 16;                             jmp   proc_PAT$4_ω
.Lx97_0:                add              r14d, 1;                             jmp   n93_match_assign_cond_α
n92_match_any_β:        sub              r14d, 1
                        add              rsp, 16;                             jmp   proc_PAT$4_ω
#-----------------------------------------------------------------------------------------------------------------------
n93_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S9]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   proc_PAT$4_γ
n93_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n92_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_res:
                        mov              r10, qword ptr [rsp + 8]
                        mov              r11, qword ptr [rsp + 16]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_β:
                                                                              jmp   proc_PAT$4_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_γ:
                        sub              rsp, 8
                        push             r11
                        push             r10
                        lea              rax, [rip + proc_PAT$4_res]
                        push             rax;                                 jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$4_ω:
                                                                              jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$5_α
proc_PAT$5_α:
proc_PAT$5_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 40
                        mov              qword ptr [rbp + -8], r10
                        mov              qword ptr [rbp + -16], r11
#-----------------------------------------------------------------------------------------------------------------------
n100_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rbp + -32], r14d;         jmp   n101_match_alternate_α
n100_match_assign_save_β:
                        add              rsp, 16;                             jmp   proc_PAT$5_ω
#-----------------------------------------------------------------------------------------------------------------------
n101_match_alternate_α: sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx108_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n104_match_defer_α
.Lx108_21:              lea              rax, [rip + .Lx108_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n103_match_defer_α
n101_match_alternate_s0:
                        lea              rax, [rip + .Lx108_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n101_match_alternate_as
n101_match_alternate_s1:
                        lea              rax, [rip + .Lx108_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n101_match_alternate_as
.Lx108_40:                                                                    jmp   n104_match_defer_β
.Lx108_41:                                                                    jmp   n103_match_defer_β
n101_match_alternate_as:
                                                                              jmp   n102_match_assign_cond_α
n101_match_alternate_β: mov              rax, qword ptr [rsp + 8];            jmp   rax
n101_match_alternate_af:
                        mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx108_19:              add              rsp, 32;                             jmp   n100_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n102_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + -32]
                        lea              rcx, [rip + .S9]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   proc_PAT$5_γ
n102_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n101_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n103_match_defer_α:     mov              rax, qword ptr [r9 + 528]            # PAT$5$V1
                        mov              rdx, qword ptr [r9 + 536]
                        cmp              eax, 8;                              jne   .Lx111_9
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx111_10
.Lx111_9:               xor              eax, eax
.Lx111_10:              test             rax, rax;                            jz    .Lx111_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx111_4]
                        lea              r11, [rip + .Lx111_5];               jmp   rax
.Lx111_4:                                                                     jmp   n101_match_alternate_s1
.Lx111_5:                                                                     jmp   n101_match_alternate_af
.Lx111_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S10]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx111_2:               test             rax, rax;                            je    .Lx111_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx111_7]
                        lea              rdx, [rip + .Lx111_8];               jmp   rax
.Lx111_7:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx111_2
.Lx111_8:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx111_2
.Lx111_3:               add              rsp, 8
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
                        test             eax, eax;                            js    n101_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx111_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n101_match_alternate_s1
.Lx111_6:               add              rsp, 16;                             jmp   n101_match_alternate_af
n103_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n104_match_defer_α:     mov              rax, qword ptr [r9 + 512]            # PAT$5$V0
                        mov              rdx, qword ptr [r9 + 520]
                        cmp              eax, 8;                              jne   .Lx112_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx112_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx112_10
.Lx112_9:               xor              eax, eax
.Lx112_10:              test             rax, rax;                            jz    .Lx112_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx112_4]
                        lea              r11, [rip + .Lx112_5];               jmp   rax
.Lx112_4:                                                                     jmp   n101_match_alternate_s0
.Lx112_5:                                                                     jmp   n101_match_alternate_af
.Lx112_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S11]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx112_2:               test             rax, rax;                            je    .Lx112_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx112_7]
                        lea              rdx, [rip + .Lx112_8];               jmp   rax
.Lx112_7:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx112_2
.Lx112_8:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx112_2
.Lx112_3:               add              rsp, 8
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
                        test             eax, eax;                            js    n101_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx112_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n101_match_alternate_s0
.Lx112_6:               add              rsp, 16;                             jmp   n101_match_alternate_af
n104_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_res:
                        mov              r10, qword ptr [rsp + 8]
                        mov              r11, qword ptr [rsp + 16]
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_β:
                                                                              jmp   proc_PAT$5_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_γ:
                        mov              r10, qword ptr [rbp + -8]
                        mov              r11, qword ptr [rbp + -16]
                        push             rbp
                        push             r11
                        push             r10
                        lea              rax, [rip + proc_PAT$5_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$5_ω:
                        mov              r10, qword ptr [rbp + -8]
                        mov              r11, qword ptr [rbp + -16]
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$6_α
proc_PAT$6_α:
proc_PAT$6_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 24
                        mov              qword ptr [rbp + -8], r10
                        mov              qword ptr [rbp + -16], r11
#-----------------------------------------------------------------------------------------------------------------------
n113_match_alternate_α: sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx120_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n118_match_defer_α
.Lx120_21:              lea              rax, [rip + .Lx120_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n115_match_lit_α
n113_match_alternate_s0:
                        lea              rax, [rip + .Lx120_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n113_match_alternate_as
n113_match_alternate_s1:
                        lea              rax, [rip + .Lx120_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n113_match_alternate_as
.Lx120_40:                                                                    jmp   n118_match_defer_β
.Lx120_41:                                                                    jmp   n114_goto_β
n113_match_alternate_as:
                                                                              jmp   proc_PAT$6_γ
n113_match_alternate_β: mov              rax, qword ptr [rsp + 8];            jmp   rax
n113_match_alternate_af:
                        mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx120_19:              add              rsp, 32;                             jmp   proc_PAT$6_ω
#-----------------------------------------------------------------------------------------------------------------------
n114_goto_α:                                                                  jmp   n113_match_alternate_af
n114_goto_β:                                                                  jmp   n113_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n115_match_lit_α:       mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n113_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 40;                             jne   n113_match_alternate_af
                        add              r14d, 1;                             jmp   n116_match_defer_α
n115_match_lit_β:       sub              r14d, 1;                             jmp   n113_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n116_match_defer_α:     mov              rax, qword ptr [r9 + 336]            # expr
                        mov              rdx, qword ptr [r9 + 344]
                        cmp              eax, 8;                              jne   .Lx124_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx124_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx124_10
.Lx124_9:               xor              eax, eax
.Lx124_10:              test             rax, rax;                            jz    .Lx124_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx124_4]
                        lea              r11, [rip + .Lx124_5];               jmp   rax
.Lx124_4:                                                                     jmp   n117_match_lit_α
.Lx124_5:                                                                     jmp   n115_match_lit_β
.Lx124_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S12]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx124_2:               test             rax, rax;                            je    .Lx124_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx124_7]
                        lea              rdx, [rip + .Lx124_8];               jmp   rax
.Lx124_7:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx124_2
.Lx124_8:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx124_2
.Lx124_3:               add              rsp, 8
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
                        test             eax, eax;                            js    n115_match_lit_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx124_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n117_match_lit_α
.Lx124_6:               add              rsp, 16;                             jmp   n115_match_lit_β
n116_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n117_match_lit_α:       mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n113_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 41;                             jne   n113_match_alternate_af
                        add              r14d, 1;                             jmp   n113_match_alternate_s1
n117_match_lit_β:       sub              r14d, 1;                             jmp   n113_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n118_match_defer_α:     mov              rax, qword ptr [r9 + 544]            # PAT$6$V0
                        mov              rdx, qword ptr [r9 + 552]
                        cmp              eax, 8;                              jne   .Lx127_9
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx127_10
.Lx127_9:               xor              eax, eax
.Lx127_10:              test             rax, rax;                            jz    .Lx127_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx127_4]
                        lea              r11, [rip + .Lx127_5];               jmp   rax
.Lx127_4:                                                                     jmp   n113_match_alternate_s0
.Lx127_5:                                                                     jmp   n113_match_alternate_af
.Lx127_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S13]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx127_2:               test             rax, rax;                            je    .Lx127_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx127_7]
                        lea              rdx, [rip + .Lx127_8];               jmp   rax
.Lx127_7:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx127_2
.Lx127_8:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx127_2
.Lx127_3:               add              rsp, 8
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
                        test             eax, eax;                            js    n113_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx127_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n113_match_alternate_s0
.Lx127_6:               add              rsp, 16;                             jmp   n113_match_alternate_af
n118_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_res:
                        mov              r10, qword ptr [rsp + 8]
                        mov              r11, qword ptr [rsp + 16]
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_β:
                                                                              jmp   proc_PAT$6_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_γ:
                        mov              r10, qword ptr [rbp + -8]
                        mov              r11, qword ptr [rbp + -16]
                        push             rbp
                        push             r11
                        push             r10
                        lea              rax, [rip + proc_PAT$6_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$6_ω:
                        mov              r10, qword ptr [rbp + -8]
                        mov              r11, qword ptr [rbp + -16]
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$7_α
proc_PAT$7_α:
proc_PAT$7_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 40
                        mov              qword ptr [rbp + -8], r10
                        mov              qword ptr [rbp + -16], r11
#-----------------------------------------------------------------------------------------------------------------------
n128_match_alternate_α: sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx136_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n131_match_defer_α
.Lx136_21:              lea              rax, [rip + .Lx136_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n129_match_defer_α
n128_match_alternate_s0:
                        lea              rax, [rip + .Lx136_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n128_match_alternate_as
n128_match_alternate_s1:
                        lea              rax, [rip + .Lx136_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n128_match_alternate_as
.Lx136_40:                                                                    jmp   n130_goto_β
.Lx136_41:                                                                    jmp   n129_match_defer_β
n128_match_alternate_as:
                                                                              jmp   proc_PAT$7_γ
n128_match_alternate_β: mov              rax, qword ptr [rsp + 8];            jmp   rax
n128_match_alternate_af:
                        mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx136_19:              add              rsp, 32;                             jmp   proc_PAT$7_ω
#-----------------------------------------------------------------------------------------------------------------------
n129_match_defer_α:     mov              rax, qword ptr [r9 + 288]            # primary
                        mov              rdx, qword ptr [r9 + 296]
                        cmp              eax, 8;                              jne   .Lx137_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx137_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx137_10
.Lx137_9:               xor              eax, eax
.Lx137_10:              test             rax, rax;                            jz    .Lx137_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx137_4]
                        lea              r11, [rip + .Lx137_5];               jmp   rax
.Lx137_4:                                                                     jmp   n128_match_alternate_s1
.Lx137_5:                                                                     jmp   n128_match_alternate_af
.Lx137_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S14]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx137_2:               test             rax, rax;                            je    .Lx137_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx137_7]
                        lea              rdx, [rip + .Lx137_8];               jmp   rax
.Lx137_7:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx137_2
.Lx137_8:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx137_2
.Lx137_3:               add              rsp, 8
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
                        test             eax, eax;                            js    n128_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx137_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n128_match_alternate_s1
.Lx137_6:               add              rsp, 16;                             jmp   n128_match_alternate_af
n129_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n130_goto_α:                                                                  jmp   n128_match_alternate_af
n130_goto_β:                                                                  jmp   n128_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n131_match_defer_α:     mov              rax, qword ptr [r9 + 560]            # PAT$7$V0
                        mov              rdx, qword ptr [r9 + 568]
                        cmp              eax, 8;                              jne   .Lx139_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx139_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx139_10
.Lx139_9:               xor              eax, eax
.Lx139_10:              test             rax, rax;                            jz    .Lx139_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx139_4]
                        lea              r11, [rip + .Lx139_5];               jmp   rax
.Lx139_4:                                                                     jmp   n132_match_assign_save_α
.Lx139_5:                                                                     jmp   n128_match_alternate_af
.Lx139_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S15]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx139_2:               test             rax, rax;                            je    .Lx139_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx139_7]
                        lea              rdx, [rip + .Lx139_8];               jmp   rax
.Lx139_7:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx139_2
.Lx139_8:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx139_2
.Lx139_3:               add              rsp, 8
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
                        test             eax, eax;                            js    n128_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx139_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n132_match_assign_save_α
.Lx139_6:               add              rsp, 16;                             jmp   n128_match_alternate_af
n131_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n132_match_assign_save_α:
                        mov              dword ptr [rbp + -32], r14d;         jmp   n133_match_defer_α
n132_match_assign_save_β:
                                                                              jmp   n128_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n133_match_defer_α:     mov              rax, qword ptr [r9 + 304]            # factor
                        mov              rdx, qword ptr [r9 + 312]
                        cmp              eax, 8;                              jne   .Lx142_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx142_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx142_10
.Lx142_9:               xor              eax, eax
.Lx142_10:              test             rax, rax;                            jz    .Lx142_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx142_4]
                        lea              r11, [rip + .Lx142_5];               jmp   rax
.Lx142_4:                                                                     jmp   n134_match_assign_cond_α
.Lx142_5:                                                                     jmp   n132_match_assign_save_β
.Lx142_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S16]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx142_2:               test             rax, rax;                            je    .Lx142_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx142_7]
                        lea              rdx, [rip + .Lx142_8];               jmp   rax
.Lx142_7:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx142_2
.Lx142_8:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx142_2
.Lx142_3:               add              rsp, 8
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
                        test             eax, eax;                            js    n132_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx142_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n134_match_assign_cond_α
.Lx142_6:               add              rsp, 16;                             jmp   n132_match_assign_save_β
n133_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n134_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + -32]
                        lea              rcx, [rip + .S17]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n128_match_alternate_s0
n134_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n133_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_res:
                        mov              r10, qword ptr [rsp + 8]
                        mov              r11, qword ptr [rsp + 16]
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_β:
                                                                              jmp   proc_PAT$7_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_γ:
                        mov              r10, qword ptr [rbp + -8]
                        mov              r11, qword ptr [rbp + -16]
                        push             rbp
                        push             r11
                        push             r10
                        lea              rax, [rip + proc_PAT$7_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$7_ω:
                        mov              r10, qword ptr [rbp + -8]
                        mov              r11, qword ptr [rbp + -16]
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$8_α
proc_PAT$8_α:
proc_PAT$8_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 40
                        mov              qword ptr [rbp + -8], r10
                        mov              qword ptr [rbp + -16], r11
#-----------------------------------------------------------------------------------------------------------------------
n145_match_alternate_α: sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx154_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n148_match_defer_α
.Lx154_21:              lea              rax, [rip + .Lx154_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n146_match_defer_α
n145_match_alternate_s0:
                        lea              rax, [rip + .Lx154_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n145_match_alternate_as
n145_match_alternate_s1:
                        lea              rax, [rip + .Lx154_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n145_match_alternate_as
.Lx154_40:                                                                    jmp   n147_goto_β
.Lx154_41:                                                                    jmp   n146_match_defer_β
n145_match_alternate_as:
                                                                              jmp   proc_PAT$8_γ
n145_match_alternate_β: mov              rax, qword ptr [rsp + 8];            jmp   rax
n145_match_alternate_af:
                        mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx154_19:              add              rsp, 32;                             jmp   proc_PAT$8_ω
#-----------------------------------------------------------------------------------------------------------------------
n146_match_defer_α:     mov              rax, qword ptr [r9 + 304]            # factor
                        mov              rdx, qword ptr [r9 + 312]
                        cmp              eax, 8;                              jne   .Lx155_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx155_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx155_10
.Lx155_9:               xor              eax, eax
.Lx155_10:              test             rax, rax;                            jz    .Lx155_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx155_4]
                        lea              r11, [rip + .Lx155_5];               jmp   rax
.Lx155_4:                                                                     jmp   n145_match_alternate_s1
.Lx155_5:                                                                     jmp   n145_match_alternate_af
.Lx155_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S16]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx155_2:               test             rax, rax;                            je    .Lx155_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx155_7]
                        lea              rdx, [rip + .Lx155_8];               jmp   rax
.Lx155_7:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx155_2
.Lx155_8:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx155_2
.Lx155_3:               add              rsp, 8
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
                        test             eax, eax;                            js    n145_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx155_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n145_match_alternate_s1
.Lx155_6:               add              rsp, 16;                             jmp   n145_match_alternate_af
n146_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n147_goto_α:                                                                  jmp   n145_match_alternate_af
n147_goto_β:                                                                  jmp   n145_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n148_match_defer_α:     mov              rax, qword ptr [r9 + 304]            # factor
                        mov              rdx, qword ptr [r9 + 312]
                        cmp              eax, 8;                              jne   .Lx157_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx157_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx157_10
.Lx157_9:               xor              eax, eax
.Lx157_10:              test             rax, rax;                            jz    .Lx157_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx157_4]
                        lea              r11, [rip + .Lx157_5];               jmp   rax
.Lx157_4:                                                                     jmp   n149_match_defer_α
.Lx157_5:                                                                     jmp   n145_match_alternate_af
.Lx157_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S16]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx157_2:               test             rax, rax;                            je    .Lx157_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx157_7]
                        lea              rdx, [rip + .Lx157_8];               jmp   rax
.Lx157_7:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx157_2
.Lx157_8:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx157_2
.Lx157_3:               add              rsp, 8
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
                        test             eax, eax;                            js    n145_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx157_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n149_match_defer_α
.Lx157_6:               add              rsp, 16;                             jmp   n145_match_alternate_af
n148_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n149_match_defer_α:     mov              rax, qword ptr [r9 + 576]            # PAT$8$V0
                        mov              rdx, qword ptr [r9 + 584]
                        cmp              eax, 8;                              jne   .Lx158_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx158_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx158_10
.Lx158_9:               xor              eax, eax
.Lx158_10:              test             rax, rax;                            jz    .Lx158_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx158_4]
                        lea              r11, [rip + .Lx158_5];               jmp   rax
.Lx158_4:                                                                     jmp   n150_match_assign_save_α
.Lx158_5:                                                                     jmp   n145_match_alternate_af
.Lx158_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S18]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx158_2:               test             rax, rax;                            je    .Lx158_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx158_7]
                        lea              rdx, [rip + .Lx158_8];               jmp   rax
.Lx158_7:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx158_2
.Lx158_8:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx158_2
.Lx158_3:               add              rsp, 8
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
                        test             eax, eax;                            js    n145_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx158_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n150_match_assign_save_α
.Lx158_6:               add              rsp, 16;                             jmp   n145_match_alternate_af
n149_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n150_match_assign_save_α:
                        mov              dword ptr [rbp + -32], r14d;         jmp   n151_match_defer_α
n150_match_assign_save_β:
                                                                              jmp   n145_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n151_match_defer_α:     mov              rax, qword ptr [r9 + 320]            # term
                        mov              rdx, qword ptr [r9 + 328]
                        cmp              eax, 8;                              jne   .Lx161_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx161_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx161_10
.Lx161_9:               xor              eax, eax
.Lx161_10:              test             rax, rax;                            jz    .Lx161_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx161_4]
                        lea              r11, [rip + .Lx161_5];               jmp   rax
.Lx161_4:                                                                     jmp   n152_match_assign_cond_α
.Lx161_5:                                                                     jmp   n150_match_assign_save_β
.Lx161_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S19]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx161_2:               test             rax, rax;                            je    .Lx161_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx161_7]
                        lea              rdx, [rip + .Lx161_8];               jmp   rax
.Lx161_7:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx161_2
.Lx161_8:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx161_2
.Lx161_3:               add              rsp, 8
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
                        test             eax, eax;                            js    n150_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx161_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n152_match_assign_cond_α
.Lx161_6:               add              rsp, 16;                             jmp   n150_match_assign_save_β
n151_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n152_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + -32]
                        lea              rcx, [rip + .S20]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n145_match_alternate_s0
n152_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n151_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_res:
                        mov              r10, qword ptr [rsp + 8]
                        mov              r11, qword ptr [rsp + 16]
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_β:
                                                                              jmp   proc_PAT$8_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_γ:
                        mov              r10, qword ptr [rbp + -8]
                        mov              r11, qword ptr [rbp + -16]
                        push             rbp
                        push             r11
                        push             r10
                        lea              rax, [rip + proc_PAT$8_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$8_ω:
                        mov              r10, qword ptr [rbp + -8]
                        mov              r11, qword ptr [rbp + -16]
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$9_α
proc_PAT$9_α:
proc_PAT$9_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 40
                        mov              qword ptr [rbp + -8], r10
                        mov              qword ptr [rbp + -16], r11
#-----------------------------------------------------------------------------------------------------------------------
n164_match_alternate_α: sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx173_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n167_match_defer_α
.Lx173_21:              lea              rax, [rip + .Lx173_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n165_match_defer_α
n164_match_alternate_s0:
                        lea              rax, [rip + .Lx173_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n164_match_alternate_as
n164_match_alternate_s1:
                        lea              rax, [rip + .Lx173_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n164_match_alternate_as
.Lx173_40:                                                                    jmp   n166_goto_β
.Lx173_41:                                                                    jmp   n165_match_defer_β
n164_match_alternate_as:
                                                                              jmp   proc_PAT$9_γ
n164_match_alternate_β: mov              rax, qword ptr [rsp + 8];            jmp   rax
n164_match_alternate_af:
                        mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx173_19:              add              rsp, 32;                             jmp   proc_PAT$9_ω
#-----------------------------------------------------------------------------------------------------------------------
n165_match_defer_α:     mov              rax, qword ptr [r9 + 320]            # term
                        mov              rdx, qword ptr [r9 + 328]
                        cmp              eax, 8;                              jne   .Lx174_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx174_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx174_10
.Lx174_9:               xor              eax, eax
.Lx174_10:              test             rax, rax;                            jz    .Lx174_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx174_4]
                        lea              r11, [rip + .Lx174_5];               jmp   rax
.Lx174_4:                                                                     jmp   n164_match_alternate_s1
.Lx174_5:                                                                     jmp   n164_match_alternate_af
.Lx174_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S19]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx174_2:               test             rax, rax;                            je    .Lx174_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx174_7]
                        lea              rdx, [rip + .Lx174_8];               jmp   rax
.Lx174_7:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx174_2
.Lx174_8:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx174_2
.Lx174_3:               add              rsp, 8
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
                        test             eax, eax;                            js    n164_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx174_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n164_match_alternate_s1
.Lx174_6:               add              rsp, 16;                             jmp   n164_match_alternate_af
n165_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n166_goto_α:                                                                  jmp   n164_match_alternate_af
n166_goto_β:                                                                  jmp   n164_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n167_match_defer_α:     mov              rax, qword ptr [r9 + 320]            # term
                        mov              rdx, qword ptr [r9 + 328]
                        cmp              eax, 8;                              jne   .Lx176_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx176_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx176_10
.Lx176_9:               xor              eax, eax
.Lx176_10:              test             rax, rax;                            jz    .Lx176_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx176_4]
                        lea              r11, [rip + .Lx176_5];               jmp   rax
.Lx176_4:                                                                     jmp   n168_match_defer_α
.Lx176_5:                                                                     jmp   n164_match_alternate_af
.Lx176_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S19]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx176_2:               test             rax, rax;                            je    .Lx176_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx176_7]
                        lea              rdx, [rip + .Lx176_8];               jmp   rax
.Lx176_7:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx176_2
.Lx176_8:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx176_2
.Lx176_3:               add              rsp, 8
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
                        test             eax, eax;                            js    n164_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx176_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n168_match_defer_α
.Lx176_6:               add              rsp, 16;                             jmp   n164_match_alternate_af
n167_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n168_match_defer_α:     mov              rax, qword ptr [r9 + 592]            # PAT$9$V0
                        mov              rdx, qword ptr [r9 + 600]
                        cmp              eax, 8;                              jne   .Lx177_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx177_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx177_10
.Lx177_9:               xor              eax, eax
.Lx177_10:              test             rax, rax;                            jz    .Lx177_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx177_4]
                        lea              r11, [rip + .Lx177_5];               jmp   rax
.Lx177_4:                                                                     jmp   n169_match_assign_save_α
.Lx177_5:                                                                     jmp   n164_match_alternate_af
.Lx177_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S21]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx177_2:               test             rax, rax;                            je    .Lx177_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx177_7]
                        lea              rdx, [rip + .Lx177_8];               jmp   rax
.Lx177_7:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx177_2
.Lx177_8:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx177_2
.Lx177_3:               add              rsp, 8
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
                        test             eax, eax;                            js    n164_match_alternate_af
                        mov              r14d, eax
                        lea              rax, [rip + .Lx177_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n169_match_assign_save_α
.Lx177_6:               add              rsp, 16;                             jmp   n164_match_alternate_af
n168_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n169_match_assign_save_α:
                        mov              dword ptr [rbp + -32], r14d;         jmp   n170_match_defer_α
n169_match_assign_save_β:
                                                                              jmp   n164_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n170_match_defer_α:     mov              rax, qword ptr [r9 + 336]            # expr
                        mov              rdx, qword ptr [r9 + 344]
                        cmp              eax, 8;                              jne   .Lx180_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax;                            jne   .Lx180_10
                        mov              rdi, rdx
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             dtp_fn_of@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx180_10
.Lx180_9:               xor              eax, eax
.Lx180_10:              test             rax, rax;                            jz    .Lx180_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx180_4]
                        lea              r11, [rip + .Lx180_5];               jmp   rax
.Lx180_4:                                                                     jmp   n171_match_assign_cond_α
.Lx180_5:                                                                     jmp   n169_match_assign_save_β
.Lx180_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S12]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx180_2:               test             rax, rax;                            je    .Lx180_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx180_7]
                        lea              rdx, [rip + .Lx180_8];               jmp   rax
.Lx180_7:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx180_2
.Lx180_8:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx180_2
.Lx180_3:               add              rsp, 8
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
                        test             eax, eax;                            js    n169_match_assign_save_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx180_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n171_match_assign_cond_α
.Lx180_6:               add              rsp, 16;                             jmp   n169_match_assign_save_β
n170_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n171_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + -32]
                        lea              rcx, [rip + .S20]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n164_match_alternate_s0
n171_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n170_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_res:
                        mov              r10, qword ptr [rsp + 8]
                        mov              r11, qword ptr [rsp + 16]
                        mov              rbp, qword ptr [rsp + 24]
                        add              rsp, 32
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_β:
                                                                              jmp   proc_PAT$9_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_γ:
                        mov              r10, qword ptr [rbp + -8]
                        mov              r11, qword ptr [rbp + -16]
                        push             rbp
                        push             r11
                        push             r10
                        lea              rax, [rip + proc_PAT$9_res]
                        push             rax
                        mov              rbp, qword ptr [rbp + 0];            jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$9_ω:
                        mov              r10, qword ptr [rbp + -8]
                        mov              r11, qword ptr [rbp + -16]
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__Push"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + Push_body]
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
.Lstartup_pname1:       .string          "LBL__Pop"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + Pop_body]
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
.Lstartup_pname2:       .string          "LBL__Unary"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + Unary_body]
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
.Lstartup_pname3:       .string          "LBL__Binary"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + Binary_body]
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
                        mov              esi, 32
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_zstatic@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname9:       .string          "PAT$1"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname9]
                        lea              rsi, [rip + proc_PAT$1_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 112
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname10:      .string          "PAT$2"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname10]
                        lea              rsi, [rip + proc_PAT$2_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 256
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname10]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname11:      .string          "PAT$3"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname11]
                        lea              rsi, [rip + proc_PAT$3_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 96
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 1
                        call             rt_proc_set_zstatic@PLT
                        lea              rdi, [rip + .Lstartup_pname11]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname12:      .string          "PAT$4"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname12]
                        lea              rsi, [rip + proc_PAT$4_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 96
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 1
                        call             rt_proc_set_zstatic@PLT
                        lea              rdi, [rip + .Lstartup_pname12]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname13:      .string          "PAT$5"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname13]
                        lea              rsi, [rip + proc_PAT$5_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 144
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname13]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname14:      .string          "PAT$6"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname14]
                        lea              rsi, [rip + proc_PAT$6_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 112
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname14]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname15:      .string          "PAT$7"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname15]
                        lea              rsi, [rip + proc_PAT$7_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 160
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname15]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname16:      .string          "PAT$8"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname16]
                        lea              rsi, [rip + proc_PAT$8_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 176
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname16]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname17:      .string          "PAT$9"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname17]
                        lea              rsi, [rip + proc_PAT$9_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 176
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname17]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
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
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
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
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        sub              rsp, 0
#=======================================================================================================================
#         <stmt 1, line 1: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n183_statement_begin_α:                                                       jmp   n184_statement_end_α
n183_statement_begin_β:                                                       jmp   n185_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n184_statement_end_α:                                                         jmp   n185_statement_begin_α
#=======================================================================================================================
#          DEFINE('Push(x)')
#-----------------------------------------------------------------------------------------------------------------------
n185_statement_begin_α:                                                       jmp   n186_func_activate_α
n185_statement_begin_β:                                                       jmp   n188_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n186_func_activate_α:   mov              rdi, qword ptr [rip + .Lx444_0]
                        mov              rsi, qword ptr [rip + .Lx444_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_Push_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n187_statement_end_α
n186_func_activate_β:                                                         jmp   n185_statement_begin_β
.Lx444_0:               .quad            .Lx444_0_s
.Lx444_0_s:             .string          "Push"
.Lx444_1:               .quad            .Lx444_1_s
.Lx444_1_s:             .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n187_statement_end_α:                                                         jmp   n188_statement_begin_α
#=======================================================================================================================
#          stk      =  TABLE()                       :(PushEnd)
#-----------------------------------------------------------------------------------------------------------------------
n188_statement_begin_α:                                                       jmp   n189_call_α
n188_statement_begin_β:                                                       jmp   n219_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n189_call_α:            sub              rsp, 16
                        .section         .rodata
.Lrkfnzd450:            .string          "TABLE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd450]
                        xor              esi, esi
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              eax, 104;                            jne   .Lx449_240
                        add              rsp, 16;                             jmp   n188_statement_begin_β
.Lx449_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n190_assign_α
n189_call_β:            add              rsp, 16;                             jmp   n188_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n190_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # stk
                        mov              qword ptr [r9 + 152], rdx;           jmp   n191_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n191_statement_end_α:   add              rsp, 16;                             jmp   n219_statement_begin_α
#=======================================================================================================================
# Push     stk[0]   =  stk[0] + 1
#-----------------------------------------------------------------------------------------------------------------------
Push_body:                                                                    jmp   n193_var_α
n192_statement_begin_β:                                                       jmp   n204_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n193_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n194_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n194_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx457_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n195_subscript_α
n194_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n192_statement_begin_β
.Lx457_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n195_subscript_α:       sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx458_240
                        add              rsp, 16;                             jmp   n194_lit_integer_β
.Lx458_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n196_var_α
n195_subscript_β:       add              rsp, 16;                             jmp   n194_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n196_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n197_lit_integer_α
n196_var_β:             add              rsp, 16;                             jmp   n195_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n197_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx460_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n198_subscript_α
n197_lit_integer_β:     add              rsp, 16;                             jmp   n196_var_β
.Lx460_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n198_subscript_α:       sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx461_240
                        add              rsp, 16;                             jmp   n197_lit_integer_β
.Lx461_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n199_deref_α
n198_subscript_β:       add              rsp, 16;                             jmp   n197_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n199_deref_α:           sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx462_240
                        add              rsp, 16;                             jmp   n198_subscript_β
.Lx462_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n200_lit_integer_α
n199_deref_β:           add              rsp, 16;                             jmp   n198_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n200_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx463_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n201_binop_α
n200_lit_integer_β:     add              rsp, 16;                             jmp   n199_deref_β
.Lx463_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n201_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # deref
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx464_240
                        add              rsp, 16;                             jmp   n200_lit_integer_β
.Lx464_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n202_assign_var_α
n201_binop_β:           add              rsp, 16;                             jmp   n200_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n202_assign_var_α:      sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx465_240
                        add              rsp, 16;                             jmp   n201_binop_β
.Lx465_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n203_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n203_statement_end_α:   add              rsp, 160;                            jmp   n204_statement_begin_α
#=======================================================================================================================
#          Push     =  .stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
n204_statement_begin_α:                                                       jmp   n205_var_α
n204_statement_begin_β:                                                       jmp   n213_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n205_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n206_var_α
#-----------------------------------------------------------------------------------------------------------------------
n206_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n207_lit_integer_α
n206_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n204_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n207_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx472_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n208_subscript_α
n207_lit_integer_β:     add              rsp, 16;                             jmp   n206_var_β
.Lx472_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n208_subscript_α:       sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx473_240
                        add              rsp, 16;                             jmp   n207_lit_integer_β
.Lx473_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n209_deref_α
n208_subscript_β:       add              rsp, 16;                             jmp   n207_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n209_deref_α:           sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx474_240
                        add              rsp, 16;                             jmp   n208_subscript_β
.Lx474_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n210_subscript_α
n209_deref_β:           add              rsp, 16;                             jmp   n208_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n210_subscript_α:       sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx475_240
                        add              rsp, 16;                             jmp   n209_deref_β
.Lx475_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n211_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n211_assign_α:          mov              rax, qword ptr [rsp + 0]             # subscript
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # Push
                        mov              qword ptr [r9 + 8], rdx;             jmp   n212_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n212_statement_end_α:   add              rsp, 96;                             jmp   n213_statement_begin_α
#=======================================================================================================================
#          $Push    =  x                             :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n213_statement_begin_α:                                                       jmp   n214_var_α
n213_statement_begin_β:                                                       jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n214_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]              # Push
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n215_call_α
#-----------------------------------------------------------------------------------------------------------------------
n215_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd483:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd483]
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
                        cmp              eax, 104;                            jne   .Lx482_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n213_statement_begin_β
.Lx482_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n216_var_α
n215_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n213_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n216_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n217_assign_var_α
n216_var_β:             add              rsp, 16
                        add              rsp, 32;                             jmp   n213_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n217_assign_var_α:      sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx485_240
                        add              rsp, 16;                             jmp   n216_var_β
.Lx485_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n218_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n218_statement_end_α:   add              rsp, 64;                             jmp   NRETURN
#=======================================================================================================================
# PushEnd  <stmt 7, line 14: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n219_statement_begin_α:                                                       jmp   n220_statement_end_α
n219_statement_begin_β:                                                       jmp   n221_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n220_statement_end_α:                                                         jmp   n221_statement_begin_α
#=======================================================================================================================
#          DEFINE('Pop()')                           :(PopEnd)
#-----------------------------------------------------------------------------------------------------------------------
n221_statement_begin_α:                                                       jmp   n222_func_activate_α
n221_statement_begin_β:                                                       jmp   n246_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n222_func_activate_α:   mov              rdi, qword ptr [rip + .Lx495_0]
                        mov              rsi, qword ptr [rip + .Lx495_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_Pop_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n223_statement_end_α
n222_func_activate_β:                                                         jmp   n221_statement_begin_β
.Lx495_0:               .quad            .Lx495_0_s
.Lx495_0_s:             .string          "Pop"
.Lx495_1:               .quad            .Lx495_1_s
.Lx495_1_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n223_statement_end_α:                                                         jmp   n246_statement_begin_α
#=======================================================================================================================
# Pop      Pop      =  stk[stk[0]]
#-----------------------------------------------------------------------------------------------------------------------
Pop_body:                                                                     jmp   n225_var_α
n224_statement_begin_β:                                                       jmp   n234_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n225_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n226_var_α
#-----------------------------------------------------------------------------------------------------------------------
n226_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n227_lit_integer_α
n226_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n224_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n227_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx502_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n228_subscript_α
n227_lit_integer_β:     add              rsp, 16;                             jmp   n226_var_β
.Lx502_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n228_subscript_α:       sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx503_240
                        add              rsp, 16;                             jmp   n227_lit_integer_β
.Lx503_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n229_deref_α
n228_subscript_β:       add              rsp, 16;                             jmp   n227_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n229_deref_α:           sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx504_240
                        add              rsp, 16;                             jmp   n228_subscript_β
.Lx504_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n230_subscript_α
n229_deref_β:           add              rsp, 16;                             jmp   n228_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n230_subscript_α:       sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx505_240
                        add              rsp, 16;                             jmp   n229_deref_β
.Lx505_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n231_deref_α
n230_subscript_β:       add              rsp, 16;                             jmp   n229_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n231_deref_α:           sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx506_240
                        add              rsp, 16;                             jmp   n230_subscript_β
.Lx506_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n232_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n232_assign_α:          mov              rax, qword ptr [rsp + 0]             # deref
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax             # Pop
                        mov              qword ptr [r9 + 40], rdx;            jmp   n233_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n233_statement_end_α:   add              rsp, 112;                            jmp   n234_statement_begin_α
#=======================================================================================================================
#          stk[0]   =  stk[0] - 1                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n234_statement_begin_α:                                                       jmp   n235_var_α
n234_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n235_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n236_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n236_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx513_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n237_subscript_α
n236_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n234_statement_begin_β
.Lx513_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n237_subscript_α:       sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx514_240
                        add              rsp, 16;                             jmp   n236_lit_integer_β
.Lx514_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n238_var_α
n237_subscript_β:       add              rsp, 16;                             jmp   n236_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n238_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n239_lit_integer_α
n238_var_β:             add              rsp, 16;                             jmp   n237_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n239_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx516_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n240_subscript_α
n239_lit_integer_β:     add              rsp, 16;                             jmp   n238_var_β
.Lx516_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n240_subscript_α:       sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx517_240
                        add              rsp, 16;                             jmp   n239_lit_integer_β
.Lx517_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n241_deref_α
n240_subscript_β:       add              rsp, 16;                             jmp   n239_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n241_deref_α:           sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx518_240
                        add              rsp, 16;                             jmp   n240_subscript_β
.Lx518_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n242_lit_integer_α
n241_deref_β:           add              rsp, 16;                             jmp   n240_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n242_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx519_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n243_binop_α
n242_lit_integer_β:     add              rsp, 16;                             jmp   n241_deref_β
.Lx519_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n243_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # deref
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104;                            jne   .Lx520_240
                        add              rsp, 16;                             jmp   n242_lit_integer_β
.Lx520_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n244_assign_var_α
n243_binop_β:           add              rsp, 16;                             jmp   n242_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n244_assign_var_α:      sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx521_240
                        add              rsp, 16;                             jmp   n243_binop_β
.Lx521_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n245_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n245_statement_end_α:   add              rsp, 160;                            jmp   RETURN
#=======================================================================================================================
# PopEnd  <stmt 11, line 18: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n246_statement_begin_α:                                                       jmp   n247_statement_end_α
n246_statement_begin_β:                                                       jmp   n248_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n247_statement_end_α:                                                         jmp   n248_statement_begin_α
#=======================================================================================================================
#          DEFINE('Unary()arg,op')                   :(UnaryEnd)
#-----------------------------------------------------------------------------------------------------------------------
n248_statement_begin_α:                                                       jmp   n249_func_activate_α
n248_statement_begin_β:                                                       jmp   n274_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n249_func_activate_α:   mov              rdi, qword ptr [rip + .Lx531_0]
                        mov              rsi, qword ptr [rip + .Lx531_1]
                        mov              edx, 2
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_Unary_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n250_statement_end_α
n249_func_activate_β:                                                         jmp   n248_statement_begin_β
.Lx531_0:               .quad            .Lx531_0_s
.Lx531_0_s:             .string          "Unary"
.Lx531_1:               .quad            .Lx531_1_s
.Lx531_1_s:             .string          "arg,op"
#-----------------------------------------------------------------------------------------------------------------------
n250_statement_end_α:                                                         jmp   n274_statement_begin_α
#=======================================================================================================================
# Unary    arg      =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
Unary_body:                                                                   jmp   n252_call_α
n251_statement_begin_β:                                                       jmp   n255_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n252_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig537z]
                        lea              rax, [rip + Pop_alpha];              jmp   rax
.Lsig537z:              .quad            0
                        .quad            .Lx537_2
                        .quad            .Lx537_2
.Lx537_2:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx537_240
                        add              rsp, 16;                             jmp   n251_statement_begin_β
.Lx537_240:                                                                   jmp   n253_assign_α
n252_call_β:                                                                  jmp   n251_statement_begin_β
.Lx537_0:               .quad            .Lx537_0_s
.Lx537_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n253_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax             # arg
                        mov              qword ptr [r9 + 72], rdx;            jmp   n254_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n254_statement_end_α:   add              rsp, 16;                             jmp   n255_statement_begin_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n255_statement_begin_α:                                                       jmp   n256_call_α
n255_statement_begin_β:                                                       jmp   n259_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n256_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig544z]
                        lea              rax, [rip + Pop_alpha];              jmp   rax
.Lsig544z:              .quad            0
                        .quad            .Lx544_2
                        .quad            .Lx544_2
.Lx544_2:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx544_240
                        add              rsp, 16;                             jmp   n255_statement_begin_β
.Lx544_240:                                                                   jmp   n257_assign_α
n256_call_β:                                                                  jmp   n255_statement_begin_β
.Lx544_0:               .quad            .Lx544_0_s
.Lx544_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n257_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              qword ptr [r9 + 88], rdx;            jmp   n258_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n258_statement_end_α:   add              rsp, 16;                             jmp   n259_statement_begin_α
#=======================================================================================================================
#          Push()   =  EVAL(op arg)
#-----------------------------------------------------------------------------------------------------------------------
n259_statement_begin_α:                                                       jmp   n260_lit_string_α
n259_statement_begin_β:                                                       jmp   n269_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n260_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx550_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n261_call_α
.Lx550_0:               .quad            .Lx550_0_s
.Lx550_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n261_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd552:            .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd552]
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
                        cmp              eax, 104;                            jne   .Lx551_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n259_statement_begin_β
.Lx551_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n262_call_α
n261_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n259_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n262_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig554z]
                        lea              rax, [rip + Push_alpha];             jmp   rax
.Lsig554z:              .quad            0
                        .quad            .Lx554_2
                        .quad            .Lx554_2
.Lx554_2:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx554_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n259_statement_begin_β
.Lx554_240:                                                                   jmp   n263_var_α
n262_call_β:                                                                  jmp   n259_statement_begin_β
.Lx554_0:               .quad            .Lx554_0_s
.Lx554_0_s:             .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n263_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 80]             # op
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n264_var_α
n263_var_β:             add              rsp, 16
                        add              rsp, 48;                             jmp   n259_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n264_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # arg
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n265_binop_α
n264_var_β:             add              rsp, 16;                             jmp   n263_var_β
#-----------------------------------------------------------------------------------------------------------------------
n265_binop_α:           sub              rsp, 16
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n266_call_α
n265_binop_β:           add              rsp, 16;                             jmp   n264_var_β
#-----------------------------------------------------------------------------------------------------------------------
n266_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd559:            .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd559]
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
                        cmp              eax, 104;                            jne   .Lx558_240
                        add              rsp, 16;                             jmp   n265_binop_β
.Lx558_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n267_assign_var_α
n266_call_β:            add              rsp, 16;                             jmp   n265_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n267_assign_var_α:      sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx560_240
                        add              rsp, 32;                             jmp   n265_binop_β
.Lx560_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n268_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n268_statement_end_α:   add              rsp, 128;                            jmp   n269_statement_begin_α
#=======================================================================================================================
#          Unary    =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n269_statement_begin_α:                                                       jmp   n270_lit_string_α
n269_statement_begin_β:                                                       jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n270_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx565_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n271_call_α
.Lx565_0:               .quad            .Lx565_0_s
.Lx565_0_s:             .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n271_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd567:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd567]
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
                        cmp              eax, 104;                            jne   .Lx566_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n269_statement_begin_β
.Lx566_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n272_assign_α
n271_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n269_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n272_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # Unary
                        mov              qword ptr [r9 + 56], rdx;            jmp   n273_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n273_statement_end_α:   add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# UnaryEnd  <stmt 17, line 24: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n274_statement_begin_α:                                                       jmp   n275_statement_end_α
n274_statement_begin_β:                                                       jmp   n276_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n275_statement_end_α:                                                         jmp   n276_statement_begin_α
#=======================================================================================================================
#          DEFINE('Binary()op,left,right')           :(BinaryEnd)
#-----------------------------------------------------------------------------------------------------------------------
n276_statement_begin_α:                                                       jmp   n277_func_activate_α
n276_statement_begin_β:                                                       jmp   n312_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n277_func_activate_α:   mov              rdi, qword ptr [rip + .Lx578_0]
                        mov              rsi, qword ptr [rip + .Lx578_1]
                        mov              edx, 3
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_Binary_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n278_statement_end_α
n277_func_activate_β:                                                         jmp   n276_statement_begin_β
.Lx578_0:               .quad            .Lx578_0_s
.Lx578_0_s:             .string          "Binary"
.Lx578_1:               .quad            .Lx578_1_s
.Lx578_1_s:             .string          "op,left,right"
#-----------------------------------------------------------------------------------------------------------------------
n278_statement_end_α:                                                         jmp   n312_statement_begin_α
#=======================================================================================================================
# Binary   right    =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
Binary_body:                                                                  jmp   n280_call_α
n279_statement_begin_β:                                                       jmp   n283_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n280_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig584z]
                        lea              rax, [rip + Pop_alpha];              jmp   rax
.Lsig584z:              .quad            0
                        .quad            .Lx584_2
                        .quad            .Lx584_2
.Lx584_2:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx584_240
                        add              rsp, 16;                             jmp   n279_statement_begin_β
.Lx584_240:                                                                   jmp   n281_assign_α
n280_call_β:                                                                  jmp   n279_statement_begin_β
.Lx584_0:               .quad            .Lx584_0_s
.Lx584_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n281_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax            # right
                        mov              qword ptr [r9 + 136], rdx;           jmp   n282_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n282_statement_end_α:   add              rsp, 16;                             jmp   n283_statement_begin_α
#=======================================================================================================================
#          op       =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n283_statement_begin_α:                                                       jmp   n284_call_α
n283_statement_begin_β:                                                       jmp   n287_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n284_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig591z]
                        lea              rax, [rip + Pop_alpha];              jmp   rax
.Lsig591z:              .quad            0
                        .quad            .Lx591_2
                        .quad            .Lx591_2
.Lx591_2:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx591_240
                        add              rsp, 16;                             jmp   n283_statement_begin_β
.Lx591_240:                                                                   jmp   n285_assign_α
n284_call_β:                                                                  jmp   n283_statement_begin_β
.Lx591_0:               .quad            .Lx591_0_s
.Lx591_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n285_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax             # op
                        mov              qword ptr [r9 + 88], rdx;            jmp   n286_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n286_statement_end_α:   add              rsp, 16;                             jmp   n287_statement_begin_α
#=======================================================================================================================
#          left     =  Pop()
#-----------------------------------------------------------------------------------------------------------------------
n287_statement_begin_α:                                                       jmp   n288_call_α
n287_statement_begin_β:                                                       jmp   n291_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n288_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig598z]
                        lea              rax, [rip + Pop_alpha];              jmp   rax
.Lsig598z:              .quad            0
                        .quad            .Lx598_2
                        .quad            .Lx598_2
.Lx598_2:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx598_240
                        add              rsp, 16;                             jmp   n287_statement_begin_β
.Lx598_240:                                                                   jmp   n289_assign_α
n288_call_β:                                                                  jmp   n287_statement_begin_β
.Lx598_0:               .quad            .Lx598_0_s
.Lx598_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n289_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # left
                        mov              qword ptr [r9 + 120], rdx;           jmp   n290_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n290_statement_end_α:   add              rsp, 16;                             jmp   n291_statement_begin_α
#=======================================================================================================================
#          Push()   =  EVAL(left ' ' op ' ' right)
#-----------------------------------------------------------------------------------------------------------------------
n291_statement_begin_α:                                                       jmp   n292_lit_string_α
n291_statement_begin_β:                                                       jmp   n307_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n292_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx604_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n293_call_α
.Lx604_0:               .quad            .Lx604_0_s
.Lx604_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n293_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd606:            .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd606]
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
                        cmp              eax, 104;                            jne   .Lx605_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n291_statement_begin_β
.Lx605_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n294_call_α
n293_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n291_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n294_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig608z]
                        lea              rax, [rip + Push_alpha];             jmp   rax
.Lsig608z:              .quad            0
                        .quad            .Lx608_2
                        .quad            .Lx608_2
.Lx608_2:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx608_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n291_statement_begin_β
.Lx608_240:                                                                   jmp   n295_var_α
n294_call_β:                                                                  jmp   n291_statement_begin_β
.Lx608_0:               .quad            .Lx608_0_s
.Lx608_0_s:             .string          "Push"
#-----------------------------------------------------------------------------------------------------------------------
n295_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 112]            # left
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n296_lit_string_α
n295_var_β:             add              rsp, 16
                        add              rsp, 48;                             jmp   n291_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n296_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx610_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n297_binop_α
n296_lit_string_β:      add              rsp, 16;                             jmp   n295_var_β
.Lx610_0:               .quad            .Lx610_0_s
.Lx610_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n297_binop_α:           sub              rsp, 16
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n298_var_α
n297_binop_β:           add              rsp, 16;                             jmp   n296_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n298_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 80]             # op
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n299_binop_α
n298_var_β:             add              rsp, 16;                             jmp   n297_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n299_binop_α:           sub              rsp, 16
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n300_lit_string_α
n299_binop_β:           add              rsp, 16;                             jmp   n298_var_β
#-----------------------------------------------------------------------------------------------------------------------
n300_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx614_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n301_binop_α
n300_lit_string_β:      add              rsp, 16;                             jmp   n299_binop_β
.Lx614_0:               .quad            .Lx614_0_s
.Lx614_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n301_binop_α:           sub              rsp, 16
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n302_var_α
n301_binop_β:           add              rsp, 16;                             jmp   n300_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n302_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 128]            # right
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n303_binop_α
n302_var_β:             add              rsp, 16;                             jmp   n301_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n303_binop_α:           sub              rsp, 16
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n304_call_α
n303_binop_β:           add              rsp, 16;                             jmp   n302_var_β
#-----------------------------------------------------------------------------------------------------------------------
n304_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd619:            .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd619]
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
                        cmp              eax, 104;                            jne   .Lx618_240
                        add              rsp, 16;                             jmp   n303_binop_β
.Lx618_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n305_assign_var_α
n304_call_β:            add              rsp, 16;                             jmp   n303_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n305_assign_var_α:      sub              rsp, 16
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
                        cmp              eax, 104;                            jne   .Lx620_240
                        add              rsp, 32;                             jmp   n303_binop_β
.Lx620_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n306_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n306_statement_end_α:   add              rsp, 224;                            jmp   n307_statement_begin_α
#=======================================================================================================================
#          Binary   =  .dummy                        :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n307_statement_begin_α:                                                       jmp   n308_lit_string_α
n307_statement_begin_β:                                                       jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n308_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx625_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n309_call_α
.Lx625_0:               .quad            .Lx625_0_s
.Lx625_0_s:             .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n309_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd627:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd627]
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
                        cmp              eax, 104;                            jne   .Lx626_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n307_statement_begin_β
.Lx626_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n310_assign_α
n309_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n307_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n310_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax             # Binary
                        mov              qword ptr [r9 + 104], rdx;           jmp   n311_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n311_statement_end_α:   add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# BinaryEnd  <stmt 24, line 31: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n312_statement_begin_α:                                                       jmp   n313_statement_end_α
n312_statement_begin_β:                                                       jmp   n314_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n313_statement_end_α:                                                         jmp   n314_statement_begin_α
#=======================================================================================================================
#          integer  =  SPAN('0123456789')
#-----------------------------------------------------------------------------------------------------------------------
n314_statement_begin_α:                                                       jmp   n315_lit_string_α
n314_statement_begin_β:                                                       jmp   n319_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n315_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx637_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n316_call_α
.Lx637_0:               .quad            .Lx637_0_s
.Lx637_0_s:             .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n316_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd639:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd639]
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
                        cmp              eax, 104;                            jne   .Lx638_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n314_statement_begin_β
.Lx638_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n317_assign_α
n316_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n314_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n317_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 176], rax            # integer
                        mov              qword ptr [r9 + 184], rdx;           jmp   n318_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n318_statement_end_α:   add              rsp, 32;                             jmp   n319_statement_begin_α
#=======================================================================================================================
#          exponent =  ANY('eEdD') (ANY('+-') | epsilon) integer
#-----------------------------------------------------------------------------------------------------------------------
n319_statement_begin_α:                                                       jmp   n320_var_α
n319_statement_begin_β:                                                       jmp   n328_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n320_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 176]            # integer
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n321_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n321_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 384], rax            # PAT$1$V1
                        mov              qword ptr [r9 + 392], rdx;           jmp   n322_var_α
#-----------------------------------------------------------------------------------------------------------------------
n322_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 208]            # epsilon
                        mov              rdx, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n323_assign_α
n322_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n319_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n323_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 368], rax            # PAT$1$V0
                        mov              qword ptr [r9 + 376], rdx;           jmp   n324_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n324_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx649_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n325_call_α
n324_lit_string_β:      add              rsp, 16;                             jmp   n322_var_β
.Lx649_0:               .quad            .Lx649_0_s
.Lx649_0_s:             .string          "PAT$1"
#-----------------------------------------------------------------------------------------------------------------------
n325_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd651:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd651]
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
                        cmp              eax, 104;                            jne   .Lx650_240
                        add              rsp, 16;                             jmp   n324_lit_string_β
.Lx650_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n326_assign_α
n325_call_β:            add              rsp, 16;                             jmp   n324_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n326_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 192], rax            # exponent
                        mov              qword ptr [r9 + 200], rdx;           jmp   n327_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n327_statement_end_α:   add              rsp, 64;                             jmp   n328_statement_begin_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#-----------------------------------------------------------------------------------------------------------------------
n328_statement_begin_α:                                                       jmp   n329_var_α
n328_statement_begin_β:                                                       jmp   n347_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n329_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 192]            # exponent
                        mov              rdx, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n330_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n330_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 496], rax            # PAT$2$V6
                        mov              qword ptr [r9 + 504], rdx;           jmp   n331_var_α
#-----------------------------------------------------------------------------------------------------------------------
n331_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 176]            # integer
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n332_assign_α
n331_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n328_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n332_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 480], rax            # PAT$2$V5
                        mov              qword ptr [r9 + 488], rdx;           jmp   n333_var_α
#-----------------------------------------------------------------------------------------------------------------------
n333_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 208]            # epsilon
                        mov              rdx, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n334_assign_α
n333_var_β:             add              rsp, 16;                             jmp   n331_var_β
#-----------------------------------------------------------------------------------------------------------------------
n334_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 464], rax            # PAT$2$V4
                        mov              qword ptr [r9 + 472], rdx;           jmp   n335_var_α
#-----------------------------------------------------------------------------------------------------------------------
n335_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 192]            # exponent
                        mov              rdx, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n336_assign_α
n335_var_β:             add              rsp, 16;                             jmp   n333_var_β
#-----------------------------------------------------------------------------------------------------------------------
n336_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 448], rax            # PAT$2$V3
                        mov              qword ptr [r9 + 456], rdx;           jmp   n337_var_α
#-----------------------------------------------------------------------------------------------------------------------
n337_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 208]            # epsilon
                        mov              rdx, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n338_assign_α
n337_var_β:             add              rsp, 16;                             jmp   n335_var_β
#-----------------------------------------------------------------------------------------------------------------------
n338_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 432], rax            # PAT$2$V2
                        mov              qword ptr [r9 + 440], rdx;           jmp   n339_var_α
#-----------------------------------------------------------------------------------------------------------------------
n339_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 176]            # integer
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n340_assign_α
n339_var_β:             add              rsp, 16;                             jmp   n337_var_β
#-----------------------------------------------------------------------------------------------------------------------
n340_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 416], rax            # PAT$2$V1
                        mov              qword ptr [r9 + 424], rdx;           jmp   n341_var_α
#-----------------------------------------------------------------------------------------------------------------------
n341_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 176]            # integer
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n342_assign_α
n341_var_β:             add              rsp, 16;                             jmp   n339_var_β
#-----------------------------------------------------------------------------------------------------------------------
n342_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 400], rax            # PAT$2$V0
                        mov              qword ptr [r9 + 408], rdx;           jmp   n343_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n343_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx671_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n344_call_α
n343_lit_string_β:      add              rsp, 16;                             jmp   n341_var_β
.Lx671_0:               .quad            .Lx671_0_s
.Lx671_0_s:             .string          "PAT$2"
#-----------------------------------------------------------------------------------------------------------------------
n344_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd673:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd673]
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
                        cmp              eax, 104;                            jne   .Lx672_240
                        add              rsp, 16;                             jmp   n343_lit_string_β
.Lx672_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n345_assign_α
n344_call_β:            add              rsp, 16;                             jmp   n343_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n345_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 224], rax            # real
                        mov              qword ptr [r9 + 232], rdx;           jmp   n346_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n346_statement_end_α:   add              rsp, 144;                            jmp   n347_statement_begin_α
#=======================================================================================================================
#          real     =  integer '.' (integer | epsilon) (exponent | epsilon)
#-----------------------------------------------------------------------------------------------------------------------
n347_statement_begin_α:                                                       jmp   n348_statement_end_α
n347_statement_begin_β:                                                       jmp   n349_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n348_statement_end_α:                                                         jmp   n349_statement_begin_α
#=======================================================================================================================
#          addop    =  ANY('+-') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n349_statement_begin_α:                                                       jmp   n350_lit_string_α
n349_statement_begin_β:                                                       jmp   n354_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n350_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx683_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n351_call_α
.Lx683_0:               .quad            .Lx683_0_s
.Lx683_0_s:             .string          "PAT$3"
#-----------------------------------------------------------------------------------------------------------------------
n351_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd685:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd685]
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
                        cmp              eax, 104;                            jne   .Lx684_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n349_statement_begin_β
.Lx684_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n352_assign_α
n351_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n349_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n352_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 240], rax            # addop
                        mov              qword ptr [r9 + 248], rdx;           jmp   n353_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n353_statement_end_α:   add              rsp, 32;                             jmp   n354_statement_begin_α
#=======================================================================================================================
#          mulop    =  ANY('*/') . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n354_statement_begin_α:                                                       jmp   n355_lit_string_α
n354_statement_begin_β:                                                       jmp   n359_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n355_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx691_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n356_call_α
.Lx691_0:               .quad            .Lx691_0_s
.Lx691_0_s:             .string          "PAT$4"
#-----------------------------------------------------------------------------------------------------------------------
n356_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd693:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd693]
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
                        cmp              eax, 104;                            jne   .Lx692_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n354_statement_begin_β
.Lx692_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n357_assign_α
n356_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n354_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n357_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 256], rax            # mulop
                        mov              qword ptr [r9 + 264], rdx;           jmp   n358_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n358_statement_end_α:   add              rsp, 32;                             jmp   n359_statement_begin_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n359_statement_begin_α:                                                       jmp   n360_var_α
n359_statement_begin_β:                                                       jmp   n368_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n360_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 176]            # integer
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n361_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n361_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 528], rax            # PAT$5$V1
                        mov              qword ptr [r9 + 536], rdx;           jmp   n362_var_α
#-----------------------------------------------------------------------------------------------------------------------
n362_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]            # real
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n363_assign_α
n362_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n359_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n363_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 512], rax            # PAT$5$V0
                        mov              qword ptr [r9 + 520], rdx;           jmp   n364_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n364_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx703_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n365_call_α
n364_lit_string_β:      add              rsp, 16;                             jmp   n362_var_β
.Lx703_0:               .quad            .Lx703_0_s
.Lx703_0_s:             .string          "PAT$5"
#-----------------------------------------------------------------------------------------------------------------------
n365_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd705:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd705]
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
                        cmp              eax, 104;                            jne   .Lx704_240
                        add              rsp, 16;                             jmp   n364_lit_string_β
.Lx704_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n366_assign_α
n365_call_β:            add              rsp, 16;                             jmp   n364_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n366_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 272], rax            # constant
                        mov              qword ptr [r9 + 280], rdx;           jmp   n367_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n367_statement_end_α:   add              rsp, 64;                             jmp   n368_statement_begin_α
#=======================================================================================================================
#          constant =  (real | integer) . *Push()
#-----------------------------------------------------------------------------------------------------------------------
n368_statement_begin_α:                                                       jmp   n369_statement_end_α
n368_statement_begin_β:                                                       jmp   n370_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n369_statement_end_α:                                                         jmp   n370_statement_begin_α
#=======================================================================================================================
#          primary  =  constant | '(' *expr ')'
#-----------------------------------------------------------------------------------------------------------------------
n370_statement_begin_α:                                                       jmp   n371_var_α
n370_statement_begin_β:                                                       jmp   n377_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n371_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 272]            # constant
                        mov              rdx, qword ptr [r9 + 280]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n372_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n372_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 544], rax            # PAT$6$V0
                        mov              qword ptr [r9 + 552], rdx;           jmp   n373_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n373_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx717_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n374_call_α
n373_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n370_statement_begin_β
.Lx717_0:               .quad            .Lx717_0_s
.Lx717_0_s:             .string          "PAT$6"
#-----------------------------------------------------------------------------------------------------------------------
n374_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd719:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd719]
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
                        cmp              eax, 104;                            jne   .Lx718_240
                        add              rsp, 16;                             jmp   n373_lit_string_β
.Lx718_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n375_assign_α
n374_call_β:            add              rsp, 16;                             jmp   n373_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n375_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 288], rax            # primary
                        mov              qword ptr [r9 + 296], rdx;           jmp   n376_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n376_statement_end_α:   add              rsp, 48;                             jmp   n377_statement_begin_α
#=======================================================================================================================
#          primary  =  constant | '(' *expr ')'
#-----------------------------------------------------------------------------------------------------------------------
n377_statement_begin_α:                                                       jmp   n378_statement_end_α
n377_statement_begin_β:                                                       jmp   n379_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n378_statement_end_α:                                                         jmp   n379_statement_begin_α
#=======================================================================================================================
#          factor   =  addop *factor . *Unary()
#-----------------------------------------------------------------------------------------------------------------------
n379_statement_begin_α:                                                       jmp   n380_var_α
n379_statement_begin_β:                                                       jmp   n386_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n380_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 240]            # addop
                        mov              rdx, qword ptr [r9 + 248]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n381_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n381_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 560], rax            # PAT$7$V0
                        mov              qword ptr [r9 + 568], rdx;           jmp   n382_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n382_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx731_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n383_call_α
n382_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n379_statement_begin_β
.Lx731_0:               .quad            .Lx731_0_s
.Lx731_0_s:             .string          "PAT$7"
#-----------------------------------------------------------------------------------------------------------------------
n383_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd733:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd733]
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
                        cmp              eax, 104;                            jne   .Lx732_240
                        add              rsp, 16;                             jmp   n382_lit_string_β
.Lx732_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n384_assign_α
n383_call_β:            add              rsp, 16;                             jmp   n382_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n384_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 304], rax            # factor
                        mov              qword ptr [r9 + 312], rdx;           jmp   n385_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n385_statement_end_α:   add              rsp, 48;                             jmp   n386_statement_begin_α
#=======================================================================================================================
#          factor   =  addop *factor . *Unary()
#-----------------------------------------------------------------------------------------------------------------------
n386_statement_begin_α:                                                       jmp   n387_statement_end_α
n386_statement_begin_β:                                                       jmp   n388_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n387_statement_end_α:                                                         jmp   n388_statement_begin_α
#=======================================================================================================================
#          term     =  *factor mulop *term . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n388_statement_begin_α:                                                       jmp   n389_var_α
n388_statement_begin_β:                                                       jmp   n395_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n389_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 256]            # mulop
                        mov              rdx, qword ptr [r9 + 264]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n390_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n390_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 576], rax            # PAT$8$V0
                        mov              qword ptr [r9 + 584], rdx;           jmp   n391_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n391_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx745_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n392_call_α
n391_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n388_statement_begin_β
.Lx745_0:               .quad            .Lx745_0_s
.Lx745_0_s:             .string          "PAT$8"
#-----------------------------------------------------------------------------------------------------------------------
n392_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd747:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd747]
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
                        cmp              eax, 104;                            jne   .Lx746_240
                        add              rsp, 16;                             jmp   n391_lit_string_β
.Lx746_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n393_assign_α
n392_call_β:            add              rsp, 16;                             jmp   n391_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n393_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 320], rax            # term
                        mov              qword ptr [r9 + 328], rdx;           jmp   n394_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n394_statement_end_α:   add              rsp, 48;                             jmp   n395_statement_begin_α
#=======================================================================================================================
#          term     =  *factor mulop *term . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n395_statement_begin_α:                                                       jmp   n396_statement_end_α
n395_statement_begin_β:                                                       jmp   n397_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n396_statement_end_α:                                                         jmp   n397_statement_begin_α
#=======================================================================================================================
#          expr     =  *term addop *expr . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n397_statement_begin_α:                                                       jmp   n398_var_α
n397_statement_begin_β:                                                       jmp   n404_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n398_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 240]            # addop
                        mov              rdx, qword ptr [r9 + 248]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n399_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n399_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 592], rax            # PAT$9$V0
                        mov              qword ptr [r9 + 600], rdx;           jmp   n400_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n400_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx759_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n401_call_α
n400_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n397_statement_begin_β
.Lx759_0:               .quad            .Lx759_0_s
.Lx759_0_s:             .string          "PAT$9"
#-----------------------------------------------------------------------------------------------------------------------
n401_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd761:            .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd761]
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
                        cmp              eax, 104;                            jne   .Lx760_240
                        add              rsp, 16;                             jmp   n400_lit_string_β
.Lx760_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n402_assign_α
n401_call_β:            add              rsp, 16;                             jmp   n400_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n402_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 336], rax            # expr
                        mov              qword ptr [r9 + 344], rdx;           jmp   n403_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n403_statement_end_α:   add              rsp, 48;                             jmp   n404_statement_begin_α
#=======================================================================================================================
#          expr     =  *term addop *expr . *Binary()
#-----------------------------------------------------------------------------------------------------------------------
n404_statement_begin_α:                                                       jmp   n405_statement_end_α
n404_statement_begin_β:                                                       jmp   n406_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n405_statement_end_α:                                                         jmp   n406_statement_begin_α
#=======================================================================================================================
#          &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n406_statement_begin_α:                                                       jmp   n407_lit_string_α
n406_statement_begin_β:                                                       jmp   n411_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n407_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx771_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n408_lit_integer_α
.Lx771_0:               .quad            .Lx771_0_s
.Lx771_0_s:             .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n408_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx772_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n409_call_α
n408_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n406_statement_begin_β
.Lx772_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n409_call_α:            sub              rsp, 16
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
.Lrkfnzd774:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd774]
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
                        cmp              eax, 104;                            jne   .Lx773_240
                        add              rsp, 16;                             jmp   n408_lit_integer_β
.Lx773_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n410_statement_end_α
n409_call_β:            add              rsp, 16;                             jmp   n408_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n410_statement_end_α:   add              rsp, 48;                             jmp   n411_statement_begin_α
#=======================================================================================================================
# loop     line     =  INPUT                         :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n411_statement_begin_α:                                                       jmp   n412_var_α
n411_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n412_var_α:             sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx779_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_GET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              eax, 104;                            jne   .Lx779_240
                        add              rsp, 16;                             jmp   n411_statement_begin_β
.Lx779_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n413_assign_α
.Lx779_0:               .quad            .Lx779_0_s
.Lx779_0_s:             .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n413_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 352], rax            # line
                        mov              qword ptr [r9 + 360], rdx;           jmp   n414_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n414_statement_end_α:   add              rsp, 16;                             jmp   n415_statement_begin_α
#=======================================================================================================================
#          line     POS(0) expr RPOS(0)              :F(error)
#-----------------------------------------------------------------------------------------------------------------------
n415_statement_begin_α:                                                       jmp   n416_var_α
n415_statement_begin_β:                                                       jmp   n429_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n416_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 352]            # line
                        mov              rdx, qword ptr [r9 + 360]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n417_var_α
#-----------------------------------------------------------------------------------------------------------------------
n417_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 336]            # expr
                        mov              rdx, qword ptr [r9 + 344]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n418_assign_α
n417_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n429_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n418_assign_α:          mov              rsi, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx787_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n419_match_begin_α
n418_assign_β:                                                                jmp   n417_var_β
.Lx787_0:               .quad            .Lx787_0_s
.Lx787_0_s:             .string          "PATV$0"
#-----------------------------------------------------------------------------------------------------------------------
n419_match_begin_α:     mov              rdi, qword ptr [rsp + 16]            # var
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
.Lx789_0:               mov              r14d, dword ptr [rbp + -40];         jmp   n420_match_pos_α
n419_match_begin_β:     lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx789_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx789_1
                                                                              jmp   .Lx789_0
.Lx789_1:
n419_match_begin_af:    mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   n418_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n420_match_pos_α:       mov              rax, 0
                        cmp              r14d, eax;                           jne   n419_match_begin_β
                                                                              jmp   n421_match_defer_α
n420_match_pos_β:                                                             jmp   n419_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n421_match_defer_α:     lea              rdi, [rip + .S22]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_get_pat_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            jz    .Lx791_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx791_4]
                        lea              r11, [rip + .Lx791_5];               jmp   rax
.Lx791_4:                                                                     jmp   n422_match_rpos_α
.Lx791_5:                                                                     jmp   n419_match_begin_β
.Lx791_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S22]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
.Lx791_2:               test             rax, rax;                            je    .Lx791_3
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx791_7]
                        lea              rdx, [rip + .Lx791_8];               jmp   rax
.Lx791_7:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx791_2
.Lx791_8:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx791_2
.Lx791_3:               add              rsp, 8
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
                        test             eax, eax;                            js    n419_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx791_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n422_match_rpos_α
.Lx791_6:               add              rsp, 16;                             jmp   n419_match_begin_β
n421_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n422_match_rpos_α:      mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx;                           jne   n419_match_begin_β
                                                                              jmp   n423_match_end_α
#-----------------------------------------------------------------------------------------------------------------------
n423_match_end_α:       push             r14
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
.Lx794_1:               test             rax, rax;                            je    .Lx794_2
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx794_3]
                        lea              rdx, [rip + .Lx794_4];               jmp   rax
.Lx794_3:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx794_1
.Lx794_4:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx794_1
.Lx794_2:               call             rt_dcap_end_ok_close@PLT
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
                        pop              rbp;                                 jmp   n424_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n424_statement_end_α:   add              rsp, 32;                             jmp   n425_statement_begin_α
#=======================================================================================================================
#          OUTPUT   =  Pop()                         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n425_statement_begin_α:                                                       jmp   n426_call_α
n425_statement_begin_β:                                                       jmp   n411_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n426_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig800z]
                        lea              rax, [rip + Pop_alpha];              jmp   rax
.Lsig800z:              .quad            0
                        .quad            .Lx800_2
                        .quad            .Lx800_2
.Lx800_2:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx800_240
                        add              rsp, 16;                             jmp   n425_statement_begin_β
.Lx800_240:                                                                   jmp   n427_assign_α
n426_call_β:                                                                  jmp   n425_statement_begin_β
.Lx800_0:               .quad            .Lx800_0_s
.Lx800_0_s:             .string          "Pop"
#-----------------------------------------------------------------------------------------------------------------------
n427_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx801_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n428_statement_end_α
.Lx801_0:               .quad            .Lx801_0_s
.Lx801_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n428_statement_end_α:   add              rsp, 16;                             jmp   n411_statement_begin_α
#=======================================================================================================================
# error    OUTPUT   = 'Bad input, try again'         :(loop)
#-----------------------------------------------------------------------------------------------------------------------
n429_statement_begin_α:                                                       jmp   n430_lit_string_α
n429_statement_begin_β:                                                       jmp   n411_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n430_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 20
                        mov              rax, qword ptr [rip + .Lx806_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n431_assign_α
.Lx806_0:               .quad            .Lx806_0_s
.Lx806_0_s:             .string          "Bad input, try again"
#-----------------------------------------------------------------------------------------------------------------------
n431_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx807_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n432_statement_end_α
.Lx807_0:               .quad            .Lx807_0_s
.Lx807_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n432_statement_end_α:   add              rsp, 16;                             jmp   n411_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 pop              rcx
                        add              rsp, 8;                              jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                add              rsp, 8
                        pop              rcx;                                 jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
NRETURN:                mov              qword ptr [rsp + 32], 2              # result
                        mov              dword ptr [rsp + 36], 0
                        mov              rax, qword ptr [rip + .Lx814_0]
                        mov              qword ptr [rsp + 40], rax;           jmp   n436_call_α
.Lx814_0:               .quad            .Lx814_0_s
.Lx814_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n436_call_α:            mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn816:              .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn816]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 104;                            je    FRETURN
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   RETURN
n436_call_β:                                                                  jmp   FRETURN
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
