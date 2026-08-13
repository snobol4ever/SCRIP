                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_pad_left_α
proc_pad_left_α:
#-----------------------------------------------------------------------------------------------------------------------
n0_save_restore_α:                                                            jmp   n1_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_deferred_α:     lea              rax, [rip + pad_left_body];          jmp   rax
                                                                              jmp   n2_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n2_save_restore_α:
pad_left_alpha:         mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 48
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                              jbe   .Lx8_2
                        mov              eax, 48
.Lx8_2:                 sub              rsp, 96
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 0]                        # pad_left
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0;                              jbe   .Lx8_10
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 88], rdx;            jmp   .Lx8_41
.Lx8_10:                mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx8_41:                cmp              rcx, 1;                              jbe   .Lx8_11
                        mov              rax, qword ptr [r8 + 96]
                        mov              rdx, qword ptr [r9 + 32]                       # n
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 96], rdx
                        mov              rax, qword ptr [r8 + 104]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 104], rdx;           jmp   .Lx8_42
.Lx8_11:                mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx8_42:                cmp              rcx, 2;                              jbe   .Lx8_12
                        mov              rax, qword ptr [r8 + 112]
                        mov              rdx, qword ptr [r9 + 48]                       # c
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [r8 + 112], rdx
                        mov              rax, qword ptr [r8 + 120]
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [r8 + 120], rdx;           jmp   .Lx8_43
.Lx8_12:                mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [r8 + 32], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [r8 + 40], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx8_43:                lea              r10, [rip + pad_left_gamma]
                        lea              r11, [rip + pad_left_omega]
                        lea              rax, [rip + pad_left_body];          jmp   rax
pad_left_gamma:         mov              rdi, qword ptr [r9 + 0]                        # pad_left
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                              jbe   .Lx8_75
                        mov              eax, 48
.Lx8_75:                lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rcx, 0;                              jbe   .Lx8_80
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx8_110
.Lx8_80:                mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx8_110:               cmp              rcx, 1;                              jbe   .Lx8_81
                        mov              rax, qword ptr [r8 + 96]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 104]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx8_111
.Lx8_81:                mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx8_111:               cmp              rcx, 2;                              jbe   .Lx8_82
                        mov              rax, qword ptr [r8 + 112]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 120]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx8_112
.Lx8_82:                mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx8_112:               mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 128
                        cmp              rcx, 3;                              jbe   .Lx8_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx8_3:                 mov              rcx, qword ptr [r8 + 64]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
pad_left_omega:         mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                              jbe   .Lx8_145
                        mov              eax, 48
.Lx8_145:               lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax                        # pad_left
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rcx, 0;                              jbe   .Lx8_150
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx8_180
.Lx8_150:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx8_180:               cmp              rcx, 1;                              jbe   .Lx8_151
                        mov              rax, qword ptr [r8 + 96]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 104]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx8_181
.Lx8_151:               mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx8_181:               cmp              rcx, 2;                              jbe   .Lx8_152
                        mov              rax, qword ptr [r8 + 112]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 120]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx8_182
.Lx8_152:               mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx8_182:               mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 128
                        cmp              rcx, 3;                              jbe   .Lx8_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx8_4:                 mov              rcx, qword ptr [r8 + 72]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_pad_right_α
proc_pad_right_α:
#-----------------------------------------------------------------------------------------------------------------------
n9_save_restore_α:                                                            jmp   n10_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n10_goto_deferred_α:    lea              rax, [rip + pad_right_body];         jmp   rax
                                                                              jmp   n11_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n11_save_restore_α:
pad_right_alpha:        mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 48
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                              jbe   .Lx17_2
                        mov              eax, 48
.Lx17_2:                sub              rsp, 96
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 64]                       # pad_right
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0;                              jbe   .Lx17_10
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 88], rdx;            jmp   .Lx17_41
.Lx17_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx17_41:               cmp              rcx, 1;                              jbe   .Lx17_11
                        mov              rax, qword ptr [r8 + 96]
                        mov              rdx, qword ptr [r9 + 32]                       # n
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 96], rdx
                        mov              rax, qword ptr [r8 + 104]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 104], rdx;           jmp   .Lx17_42
.Lx17_11:               mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx17_42:               cmp              rcx, 2;                              jbe   .Lx17_12
                        mov              rax, qword ptr [r8 + 112]
                        mov              rdx, qword ptr [r9 + 48]                       # c
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [r8 + 112], rdx
                        mov              rax, qword ptr [r8 + 120]
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [r8 + 120], rdx;           jmp   .Lx17_43
.Lx17_12:               mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [r8 + 32], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [r8 + 40], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx17_43:               lea              r10, [rip + pad_right_gamma]
                        lea              r11, [rip + pad_right_omega]
                        lea              rax, [rip + pad_right_body];         jmp   rax
pad_right_gamma:        mov              rdi, qword ptr [r9 + 64]                       # pad_right
                        mov              rsi, qword ptr [r9 + 72]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                              jbe   .Lx17_75
                        mov              eax, 48
.Lx17_75:               lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        cmp              rcx, 0;                              jbe   .Lx17_80
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx17_110
.Lx17_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx17_110:              cmp              rcx, 1;                              jbe   .Lx17_81
                        mov              rax, qword ptr [r8 + 96]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 104]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx17_111
.Lx17_81:               mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx17_111:              cmp              rcx, 2;                              jbe   .Lx17_82
                        mov              rax, qword ptr [r8 + 112]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 120]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx17_112
.Lx17_82:               mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx17_112:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 128
                        cmp              rcx, 3;                              jbe   .Lx17_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx17_3:                mov              rcx, qword ptr [r8 + 64]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
pad_right_omega:        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 3;                              jbe   .Lx17_145
                        mov              eax, 48
.Lx17_145:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax                       # pad_right
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        cmp              rcx, 0;                              jbe   .Lx17_150
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx17_180
.Lx17_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx17_180:              cmp              rcx, 1;                              jbe   .Lx17_151
                        mov              rax, qword ptr [r8 + 96]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 104]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx17_181
.Lx17_151:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx17_181:              cmp              rcx, 2;                              jbe   .Lx17_152
                        mov              rax, qword ptr [r8 + 112]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              rax, qword ptr [r8 + 120]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx17_182
.Lx17_152:              mov              rax, qword ptr [r8 + 32]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 40]
                        mov              qword ptr [r9 + 56], rax
.Lx17_182:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 128
                        cmp              rcx, 3;                              jbe   .Lx17_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx17_4:                mov              rcx, qword ptr [r8 + 72]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_ltrim_α
proc_ltrim_α:
#-----------------------------------------------------------------------------------------------------------------------
n18_save_restore_α:                                                           jmp   n19_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n19_goto_deferred_α:    lea              rax, [rip + ltrim_body];             jmp   rax
                                                                              jmp   n20_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n20_save_restore_α:
ltrim_alpha:            mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                              jbe   .Lx26_2
                        mov              eax, 16
.Lx26_2:                sub              rsp, 96
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 96]                       # ws
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              rax, qword ptr [r9 + 112]                      # r
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 112], 0
                        mov              qword ptr [r9 + 120], 0
                        mov              rax, qword ptr [r9 + 80]                       # ltrim
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
                        mov              qword ptr [rsp + 48], r10
                        mov              qword ptr [rsp + 56], r11
                        mov              qword ptr [rsp + 64], rcx
                        cmp              rcx, 0;                              jbe   .Lx26_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 56], rdx;            jmp   .Lx26_41
.Lx26_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx26_41:               lea              r10, [rip + ltrim_gamma]
                        lea              r11, [rip + ltrim_omega]
                        lea              rax, [rip + ltrim_body];             jmp   rax
ltrim_gamma:            mov              rdi, qword ptr [r9 + 80]                       # ltrim
                        mov              rsi, qword ptr [r9 + 88]
                        mov              rcx, qword ptr [rsp + 64]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                              jbe   .Lx26_75
                        mov              eax, 16
.Lx26_75:               lea              r8, [rsp + 80]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rcx, 0;                              jbe   .Lx26_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx26_110
.Lx26_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx26_110:              mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              eax, 128
                        cmp              rcx, 1;                              jbe   .Lx26_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 112
.Lx26_3:                mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
ltrim_omega:            mov              rcx, qword ptr [rsp + 64]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                              jbe   .Lx26_145
                        mov              eax, 16
.Lx26_145:              lea              r8, [rsp + 80]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 80], rax                       # ltrim
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rcx, 0;                              jbe   .Lx26_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx26_180
.Lx26_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx26_180:              mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              eax, 128
                        cmp              rcx, 1;                              jbe   .Lx26_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 112
.Lx26_4:                mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_rtrim_α
proc_rtrim_α:
#-----------------------------------------------------------------------------------------------------------------------
n27_save_restore_α:                                                           jmp   n28_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n28_goto_deferred_α:    lea              rax, [rip + rtrim_body];             jmp   rax
                                                                              jmp   n29_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n29_save_restore_α:
rtrim_alpha:            mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                              jbe   .Lx35_2
                        mov              eax, 16
.Lx35_2:                sub              rsp, 112
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 96]                       # ws
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              rax, qword ptr [r9 + 144]                      # i
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 144], 0
                        mov              qword ptr [r9 + 152], 0
                        mov              rax, qword ptr [r9 + 160]                      # ch
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 160], 0
                        mov              qword ptr [r9 + 168], 0
                        mov              rax, qword ptr [r9 + 128]                      # rtrim
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
                        mov              qword ptr [rsp + 64], r10
                        mov              qword ptr [rsp + 72], r11
                        mov              qword ptr [rsp + 80], rcx
                        cmp              rcx, 0;                              jbe   .Lx35_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 56], rdx;            jmp   .Lx35_41
.Lx35_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx35_41:               lea              r10, [rip + rtrim_gamma]
                        lea              r11, [rip + rtrim_omega]
                        lea              rax, [rip + rtrim_body];             jmp   rax
rtrim_gamma:            mov              rdi, qword ptr [r9 + 128]                      # rtrim
                        mov              rsi, qword ptr [r9 + 136]
                        mov              rcx, qword ptr [rsp + 80]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                              jbe   .Lx35_75
                        mov              eax, 16
.Lx35_75:               lea              r8, [rsp + 96]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 160], rax                      # ch
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 168], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 152], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rcx, 0;                              jbe   .Lx35_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx35_110
.Lx35_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx35_110:              mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              eax, 144
                        cmp              rcx, 1;                              jbe   .Lx35_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 128
.Lx35_3:                mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
rtrim_omega:            mov              rcx, qword ptr [rsp + 80]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                              jbe   .Lx35_145
                        mov              eax, 16
.Lx35_145:              lea              r8, [rsp + 96]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax                      # rtrim
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 160], rax                      # ch
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 168], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 152], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rcx, 0;                              jbe   .Lx35_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx35_180
.Lx35_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx35_180:              mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              eax, 144
                        cmp              rcx, 1;                              jbe   .Lx35_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 128
.Lx35_4:                mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_trimws_α
proc_trimws_α:
#-----------------------------------------------------------------------------------------------------------------------
n36_save_restore_α:                                                           jmp   n37_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n37_goto_deferred_α:    lea              rax, [rip + trimws_body];            jmp   rax
                                                                              jmp   n38_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n38_save_restore_α:
trimws_alpha:           mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                              jbe   .Lx44_2
                        mov              eax, 16
.Lx44_2:                sub              rsp, 64
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 176]                      # trimws
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 176], 0
                        mov              qword ptr [r9 + 184], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0;                              jbe   .Lx44_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 56], rdx;            jmp   .Lx44_41
.Lx44_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx44_41:               lea              r10, [rip + trimws_gamma]
                        lea              r11, [rip + trimws_omega]
                        lea              rax, [rip + trimws_body];            jmp   rax
trimws_gamma:           mov              rdi, qword ptr [r9 + 176]                      # trimws
                        mov              rsi, qword ptr [r9 + 184]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                              jbe   .Lx44_75
                        mov              eax, 16
.Lx44_75:               lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 176], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 184], rax
                        cmp              rcx, 0;                              jbe   .Lx44_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx44_110
.Lx44_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx44_110:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                              jbe   .Lx44_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx44_3:                mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
trimws_omega:           mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                              jbe   .Lx44_145
                        mov              eax, 16
.Lx44_145:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 176], rax                      # trimws
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 184], rax
                        cmp              rcx, 0;                              jbe   .Lx44_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx44_180
.Lx44_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx44_180:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                              jbe   .Lx44_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx44_4:                mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_repeat_α
proc_repeat_α:
#-----------------------------------------------------------------------------------------------------------------------
n45_save_restore_α:                                                           jmp   n46_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n46_goto_deferred_α:    lea              rax, [rip + repeat_body];            jmp   rax
                                                                              jmp   n47_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n47_save_restore_α:
repeat_alpha:           mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                              jbe   .Lx53_2
                        mov              eax, 32
.Lx53_2:                sub              rsp, 80
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 192]                      # repeat
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 192], 0
                        mov              qword ptr [r9 + 200], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0;                              jbe   .Lx53_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx;            jmp   .Lx53_41
.Lx53_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx53_41:               cmp              rcx, 1;                              jbe   .Lx53_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 32]                       # n
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 88], rdx;            jmp   .Lx53_42
.Lx53_11:               mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx53_42:               lea              r10, [rip + repeat_gamma]
                        lea              r11, [rip + repeat_omega]
                        lea              rax, [rip + repeat_body];            jmp   rax
repeat_gamma:           mov              rdi, qword ptr [r9 + 192]                      # repeat
                        mov              rsi, qword ptr [r9 + 200]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                              jbe   .Lx53_75
                        mov              eax, 32
.Lx53_75:               lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 192], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 200], rax
                        cmp              rcx, 0;                              jbe   .Lx53_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx53_110
.Lx53_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx53_110:              cmp              rcx, 1;                              jbe   .Lx53_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx53_111
.Lx53_81:               mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx53_111:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                              jbe   .Lx53_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx53_3:                mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
repeat_omega:           mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                              jbe   .Lx53_145
                        mov              eax, 32
.Lx53_145:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 192], rax                      # repeat
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 200], rax
                        cmp              rcx, 0;                              jbe   .Lx53_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx53_180
.Lx53_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx53_180:              cmp              rcx, 1;                              jbe   .Lx53_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 32], rax                       # n
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx53_181
.Lx53_151:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx53_181:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                              jbe   .Lx53_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx53_4:                mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_contains_α
proc_contains_α:
#-----------------------------------------------------------------------------------------------------------------------
n54_save_restore_α:                                                           jmp   n55_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n55_goto_deferred_α:    lea              rax, [rip + contains_body];          jmp   rax
                                                                              jmp   n56_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n56_save_restore_α:
contains_alpha:         mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                              jbe   .Lx62_2
                        mov              eax, 32
.Lx62_2:                sub              rsp, 80
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 208]                      # contains
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 208], 0
                        mov              qword ptr [r9 + 216], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0;                              jbe   .Lx62_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx;            jmp   .Lx62_41
.Lx62_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx62_41:               cmp              rcx, 1;                              jbe   .Lx62_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [r8 + 88], rdx;            jmp   .Lx62_42
.Lx62_11:               mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx62_42:               lea              r10, [rip + contains_gamma]
                        lea              r11, [rip + contains_omega]
                        lea              rax, [rip + contains_body];          jmp   rax
contains_gamma:         mov              rdi, qword ptr [r9 + 208]                      # contains
                        mov              rsi, qword ptr [r9 + 216]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                              jbe   .Lx62_75
                        mov              eax, 32
.Lx62_75:               lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 208], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 216], rax
                        cmp              rcx, 0;                              jbe   .Lx62_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx62_110
.Lx62_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx62_110:              cmp              rcx, 1;                              jbe   .Lx62_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx62_111
.Lx62_81:               mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx62_111:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                              jbe   .Lx62_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx62_3:                mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
contains_omega:         mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                              jbe   .Lx62_145
                        mov              eax, 32
.Lx62_145:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 208], rax                      # contains
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 216], rax
                        cmp              rcx, 0;                              jbe   .Lx62_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx62_180
.Lx62_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx62_180:              cmp              rcx, 1;                              jbe   .Lx62_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx62_181
.Lx62_151:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx62_181:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                              jbe   .Lx62_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx62_4:                mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_startswith_α
proc_startswith_α:
#-----------------------------------------------------------------------------------------------------------------------
n63_save_restore_α:                                                           jmp   n64_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n64_goto_deferred_α:    lea              rax, [rip + startswith_body];        jmp   rax
                                                                              jmp   n65_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n65_save_restore_α:
startswith_alpha:       mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                              jbe   .Lx71_2
                        mov              eax, 32
.Lx71_2:                sub              rsp, 80
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 240]                      # startswith
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 248]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 240], 0
                        mov              qword ptr [r9 + 248], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0;                              jbe   .Lx71_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx;            jmp   .Lx71_41
.Lx71_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx71_41:               cmp              rcx, 1;                              jbe   .Lx71_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [r8 + 88], rdx;            jmp   .Lx71_42
.Lx71_11:               mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx71_42:               lea              r10, [rip + startswith_gamma]
                        lea              r11, [rip + startswith_omega]
                        lea              rax, [rip + startswith_body];        jmp   rax
startswith_gamma:       mov              rdi, qword ptr [r9 + 240]                      # startswith
                        mov              rsi, qword ptr [r9 + 248]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                              jbe   .Lx71_75
                        mov              eax, 32
.Lx71_75:               lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 240], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 248], rax
                        cmp              rcx, 0;                              jbe   .Lx71_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx71_110
.Lx71_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx71_110:              cmp              rcx, 1;                              jbe   .Lx71_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx71_111
.Lx71_81:               mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx71_111:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                              jbe   .Lx71_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx71_3:                mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
startswith_omega:       mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                              jbe   .Lx71_145
                        mov              eax, 32
.Lx71_145:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 240], rax                      # startswith
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 248], rax
                        cmp              rcx, 0;                              jbe   .Lx71_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx71_180
.Lx71_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx71_180:              cmp              rcx, 1;                              jbe   .Lx71_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx71_181
.Lx71_151:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx71_181:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                              jbe   .Lx71_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx71_4:                mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_endswith_α
proc_endswith_α:
#-----------------------------------------------------------------------------------------------------------------------
n72_save_restore_α:                                                           jmp   n73_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n73_goto_deferred_α:    lea              rax, [rip + endswith_body];          jmp   rax
                                                                              jmp   n74_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n74_save_restore_α:
endswith_alpha:         mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                              jbe   .Lx80_2
                        mov              eax, 32
.Lx80_2:                sub              rsp, 80
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 256]                      # endswith
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 264]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 256], 0
                        mov              qword ptr [r9 + 264], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0;                              jbe   .Lx80_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx;            jmp   .Lx80_41
.Lx80_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx80_41:               cmp              rcx, 1;                              jbe   .Lx80_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [r8 + 88], rdx;            jmp   .Lx80_42
.Lx80_11:               mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx80_42:               lea              r10, [rip + endswith_gamma]
                        lea              r11, [rip + endswith_omega]
                        lea              rax, [rip + endswith_body];          jmp   rax
endswith_gamma:         mov              rdi, qword ptr [r9 + 256]                      # endswith
                        mov              rsi, qword ptr [r9 + 264]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                              jbe   .Lx80_75
                        mov              eax, 32
.Lx80_75:               lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 256], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 264], rax
                        cmp              rcx, 0;                              jbe   .Lx80_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx80_110
.Lx80_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx80_110:              cmp              rcx, 1;                              jbe   .Lx80_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx80_111
.Lx80_81:               mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx80_111:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                              jbe   .Lx80_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx80_3:                mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
endswith_omega:         mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                              jbe   .Lx80_145
                        mov              eax, 32
.Lx80_145:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 256], rax                      # endswith
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 264], rax
                        cmp              rcx, 0;                              jbe   .Lx80_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx80_180
.Lx80_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx80_180:              cmp              rcx, 1;                              jbe   .Lx80_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx80_181
.Lx80_151:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx80_181:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                              jbe   .Lx80_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx80_4:                mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_index_α
proc_index_α:
#-----------------------------------------------------------------------------------------------------------------------
n81_save_restore_α:                                                           jmp   n82_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n82_goto_deferred_α:    lea              rax, [rip + index_body];             jmp   rax
                                                                              jmp   n83_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n83_save_restore_α:
index_alpha:            mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                              jbe   .Lx89_2
                        mov              eax, 32
.Lx89_2:                sub              rsp, 96
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 288]                      # ix
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 288], 0
                        mov              qword ptr [r9 + 296], 0
                        mov              rax, qword ptr [r9 + 272]                      # index
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 280]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 272], 0
                        mov              qword ptr [r9 + 280], 0
                        mov              qword ptr [rsp + 32], r10
                        mov              qword ptr [rsp + 40], r11
                        mov              qword ptr [rsp + 48], rcx
                        cmp              rcx, 0;                              jbe   .Lx89_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx;            jmp   .Lx89_41
.Lx89_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx89_41:               cmp              rcx, 1;                              jbe   .Lx89_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 224]                      # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [r8 + 88], rdx;            jmp   .Lx89_42
.Lx89_11:               mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx89_42:               lea              r10, [rip + index_gamma]
                        lea              r11, [rip + index_omega]
                        lea              rax, [rip + index_body];             jmp   rax
index_gamma:            mov              rdi, qword ptr [r9 + 272]                      # index
                        mov              rsi, qword ptr [r9 + 280]
                        mov              rcx, qword ptr [rsp + 48]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                              jbe   .Lx89_75
                        mov              eax, 32
.Lx89_75:               lea              r8, [rsp + 64]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 272], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 280], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 288], rax                      # ix
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 296], rax
                        cmp              rcx, 0;                              jbe   .Lx89_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx89_110
.Lx89_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx89_110:              cmp              rcx, 1;                              jbe   .Lx89_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx89_111
.Lx89_81:               mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx89_111:              mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              eax, 128
                        cmp              rcx, 2;                              jbe   .Lx89_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 96
.Lx89_3:                mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
index_omega:            mov              rcx, qword ptr [rsp + 48]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                              jbe   .Lx89_145
                        mov              eax, 32
.Lx89_145:              lea              r8, [rsp + 64]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 272], rax                      # index
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 280], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 288], rax                      # ix
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 296], rax
                        cmp              rcx, 0;                              jbe   .Lx89_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # s
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx89_180
.Lx89_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx89_180:              cmp              rcx, 1;                              jbe   .Lx89_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 224], rax                      # t
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx89_181
.Lx89_151:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 232], rax
.Lx89_181:              mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              eax, 128
                        cmp              rcx, 2;                              jbe   .Lx89_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 96
.Lx89_4:                mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "pad_left"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + pad_left_body]
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
.Lstartup_pname1:       .string          "pad_right"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + pad_right_body]
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
.Lstartup_pname2:       .string          "ltrim"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + ltrim_body]
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
.Lstartup_pname3:       .string          "rtrim"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + rtrim_body]
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
.Lstartup_pname4:       .string          "trimws"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname4]
                        lea              rsi, [rip + trimws_body]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname4]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname5:       .string          "repeat"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + repeat_body]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname5]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname6:       .string          "contains"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname6]
                        lea              rsi, [rip + contains_body]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname6]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname7:       .string          "startswith"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname7]
                        lea              rsi, [rip + startswith_body]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname7]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname8:       .string          "endswith"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname8]
                        lea              rsi, [rip + endswith_body]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname8]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname9:       .string          "index"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname9]
                        lea              rsi, [rip + index_body]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname9]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "pad_left"
.Lgvan1:                .string          "s"
.Lgvan2:                .string          "n"
.Lgvan3:                .string          "c"
.Lgvan4:                .string          "pad_right"
.Lgvan5:                .string          "ltrim"
.Lgvan6:                .string          "ws"
.Lgvan7:                .string          "r"
.Lgvan8:                .string          "rtrim"
.Lgvan9:                .string          "i"
.Lgvan10:               .string          "ch"
.Lgvan11:               .string          "trimws"
.Lgvan12:               .string          "repeat"
.Lgvan13:               .string          "contains"
.Lgvan14:               .string          "t"
.Lgvan15:               .string          "startswith"
.Lgvan16:               .string          "endswith"
.Lgvan17:               .string          "index"
.Lgvan18:               .string          "ix"
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
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 19
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 19
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        sub              rsp, 0
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_begin_α:                                                        jmp   n91_statement_end_α
n90_statement_begin_β:                                                        jmp   n92_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_end_α:                                                          jmp   n92_statement_begin_α
n91_statement_end_β:                                                          jmp   n92_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'endswith ok'          :(ew1)
#-----------------------------------------------------------------------------------------------------------------------
n92_statement_begin_α:                                                        jmp   n93_func_activate_α
n92_statement_begin_β:                                                        jmp   n124_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n93_func_activate_α:    mov              rdi, qword ptr [rip + .Lx477_0]
                        mov              rsi, qword ptr [rip + .Lx477_1]
                        mov              edx, 3
                        mov              ecx, 3
                        mov              r8d, 0
                        lea              r9, [rip + proc_pad_left_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n94_statement_end_α
n93_func_activate_β:                                                          jmp   n92_statement_begin_β
.Lx477_0:               .quad            .Lx477_0_s
.Lx477_0_s:             .string          "pad_left"
.Lx477_1:               .quad            .Lx477_1_s
.Lx477_1_s:             .string          "s,n,c"
#-----------------------------------------------------------------------------------------------------------------------
n94_statement_end_α:                                                          jmp   n124_statement_begin_α
n94_statement_end_β:                                                          jmp   n124_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
pad_left_body:                                                                jmp   n96_var_α
n95_statement_begin_β:                                                        jmp   n102_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n96_var_α:              mov              rax, qword ptr [r9 + 48]                       # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 128], rax                     # result
                        mov              qword ptr [rsp + 136], rdx;          jmp   n97_call_α
n96_var_β:                                                                    jmp   n95_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n97_call_α:             mov              rax, qword ptr [rsp + 128]
                        mov              qword ptr [rsp + 96], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [rsp + 104], rax
                        .section         .rodata
.Lrkfn484:              .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn484]
                        lea              rsi, [rsp + 96]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 80], rax
                        mov              qword ptr [rsp + 88], rdx
                        cmp              eax, 104;                            je    n95_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n98_lit_string_α
n97_call_β:                                                                   jmp   n95_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n98_lit_string_α:       mov              qword ptr [rsp + 144], 2                       # result
                        mov              dword ptr [rsp + 148], 1
                        mov              rax, qword ptr [rip + .Lx485_0]
                        mov              qword ptr [rsp + 152], rax;          jmp   n99_binop_α
n98_lit_string_β:                                                             jmp   n95_statement_begin_β
.Lx485_0:               .quad            .Lx485_0_s
.Lx485_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n99_binop_α:            mov              rdi, qword ptr [rsp + 80]
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 144]
                        mov              rcx, qword ptr [rsp + 152]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 64], rax
                        mov              qword ptr [rsp + 72], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n100_assign_α
n99_binop_β:                                                                  jmp   n95_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n100_assign_α:          mov              rax, qword ptr [rsp + 64]
                        mov              rdx, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              qword ptr [r9 + 56], rdx;            jmp   n101_statement_end_α
n100_assign_β:                                                                jmp   n95_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n101_statement_end_α:                                                         jmp   n102_statement_begin_α
n101_statement_end_β:                                                         jmp   n102_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_begin_α:                                                       jmp   n103_var_α
n102_statement_begin_β:                                                       jmp   n113_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n103_var_α:             mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 288], rax                     # result
                        mov              qword ptr [rsp + 296], rdx;          jmp   n104_call_α
n103_var_β:                                                                   jmp   n102_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n104_call_α:            mov              rax, qword ptr [rsp + 288]
                        mov              qword ptr [rsp + 256], rax
                        mov              rax, qword ptr [rsp + 296]
                        mov              qword ptr [rsp + 264], rax
                        .section         .rodata
.Lrkfn494:              .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn494]
                        lea              rsi, [rsp + 256]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 240], rax
                        mov              qword ptr [rsp + 248], rdx
                        cmp              eax, 104;                            je    n102_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n105_var_α
n104_call_β:                                                                  jmp   n102_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n105_var_α:             mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 224], rax                     # result
                        mov              qword ptr [rsp + 232], rdx;          jmp   n106_coerce_numeric_α
n105_var_β:                                                                   jmp   n102_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n106_coerce_numeric_α:  mov              eax, dword ptr [rsp + 240]
                        cmp              eax, 5;                              je    .Lx497_1
                        cmp              eax, 3;                              jne   .Lx497_0
                        mov              eax, dword ptr [rsp + 224]
                        cmp              eax, 3;                              jne   .Lx497_0
.Lx497_1:               mov              rax, qword ptr [rsp + 240]
                        mov              qword ptr [rsp + 208], rax
                        mov              rax, qword ptr [rsp + 248]
                        mov              qword ptr [rsp + 216], rax;          jmp   n107_coerce_numeric_α
.Lx497_0:               lea              rdi, [rsp + 240]
                        lea              rsi, [rsp + 224]
                        lea              rdx, [rsp + 208]
                        mov              rcx, 109
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n107_coerce_numeric_α
n106_coerce_numeric_β:                                                        jmp   n102_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n107_coerce_numeric_α:  mov              eax, dword ptr [rsp + 224]
                        cmp              eax, 5;                              je    .Lx499_1
                        cmp              eax, 3;                              jne   .Lx499_0
                        mov              eax, dword ptr [rsp + 240]
                        cmp              eax, 3;                              jne   .Lx499_0
.Lx499_1:               mov              rax, qword ptr [rsp + 224]
                        mov              qword ptr [rsp + 192], rax
                        mov              rax, qword ptr [rsp + 232]
                        mov              qword ptr [rsp + 200], rax;          jmp   n108_cmp_test_α
.Lx499_0:               lea              rdi, [rsp + 224]
                        lea              rsi, [rsp + 240]
                        lea              rdx, [rsp + 192]
                        mov              rcx, 110
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n108_cmp_test_α
n107_coerce_numeric_β:                                                        jmp   n102_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n108_cmp_test_α:        lea              rdi, [rsp + 208]
                        lea              rsi, [rsp + 192]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_cmp_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             eax, eax;                            js    n102_statement_begin_β
                        mov              qword ptr [rsp + 176], 0
                        mov              qword ptr [rsp + 184], 0;            jmp   n109_var_α
n108_cmp_test_β:                                                              jmp   n102_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n109_var_α:             mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 304], rax                     # result
                        mov              qword ptr [rsp + 312], rdx;          jmp   n110_binop_α
n109_var_β:                                                                   jmp   n102_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n110_binop_α:           mov              rdi, qword ptr [rsp + 176]
                        mov              rsi, qword ptr [rsp + 184]
                        mov              rdx, qword ptr [rsp + 304]
                        mov              rcx, qword ptr [rsp + 312]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 160], rax
                        mov              qword ptr [rsp + 168], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n111_assign_α
n110_binop_β:                                                                 jmp   n102_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n111_assign_α:          mov              rax, qword ptr [rsp + 160]
                        mov              rdx, qword ptr [rsp + 168]
                        mov              qword ptr [r9 + 0], rax                        # pad_left
                        mov              qword ptr [r9 + 8], rdx;             jmp   n112_statement_end_α
n111_assign_β:                                                                jmp   n102_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n112_statement_end_α:                                                         jmp   RETURN
n112_statement_end_β:                                                         jmp   n113_statement_begin_α
#=======================================================================================================================
#         startswith('foobar', 'bar')     :S(bad_sw2)
#-----------------------------------------------------------------------------------------------------------------------
n113_statement_begin_α:                                                       jmp   n114_var_α
n113_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n114_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]                       # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n115_var_α
n114_var_β:             add              rsp, 16;                             jmp   n113_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n115_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n116_var_α
n115_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n113_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n116_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n117_call_α
n116_var_β:             add              rsp, 16;                             jmp   n115_var_β
#-----------------------------------------------------------------------------------------------------------------------
n117_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd513:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd513]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx512_240
                        add              rsp, 16;                             jmp   n116_var_β
.Lx512_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n118_binop_α
n117_call_β:            add              rsp, 16;                             jmp   n116_var_β
#-----------------------------------------------------------------------------------------------------------------------
n118_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]                      # var
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]                      # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104;                            jne   .Lx514_240
                        add              rsp, 32;                             jmp   n116_var_β
.Lx514_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n119_call_α
n118_binop_β:           add              rsp, 32;                             jmp   n116_var_β
#-----------------------------------------------------------------------------------------------------------------------
n119_call_α:            sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 112]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lrkfnzd516:            .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd516]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104;                            jne   .Lx515_240
                        add              rsp, 16;                             jmp   n118_binop_β
.Lx515_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n120_var_α
n119_call_β:            add              rsp, 16;                             jmp   n118_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n120_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n121_binop_α
n120_var_β:             add              rsp, 32;                             jmp   n118_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n121_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n122_assign_α
n121_binop_β:           add              rsp, 16;                             jmp   n120_var_β
#-----------------------------------------------------------------------------------------------------------------------
n122_assign_α:          mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # pad_left
                        mov              qword ptr [r9 + 8], rdx;             jmp   n123_statement_end_α
n122_assign_β:                                                                jmp   n113_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n123_statement_end_α:                                                         jmp   RETURN
n123_statement_end_β:                                                         jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n124_statement_begin_α:                                                       jmp   n125_statement_end_α
n124_statement_begin_β:                                                       jmp   n126_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n125_statement_end_α:                                                         jmp   n126_statement_begin_α
n125_statement_end_β:                                                         jmp   n126_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n126_statement_begin_α:                                                       jmp   n127_func_activate_α
n126_statement_begin_β:                                                       jmp   n158_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n127_func_activate_α:   mov              rdi, qword ptr [rip + .Lx529_0]
                        mov              rsi, qword ptr [rip + .Lx529_1]
                        mov              edx, 3
                        mov              ecx, 3
                        mov              r8d, 0
                        lea              r9, [rip + proc_pad_right_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n128_statement_end_α
n127_func_activate_β:                                                         jmp   n126_statement_begin_β
.Lx529_0:               .quad            .Lx529_0_s
.Lx529_0_s:             .string          "pad_right"
.Lx529_1:               .quad            .Lx529_1_s
.Lx529_1_s:             .string          "s,n,c"
#-----------------------------------------------------------------------------------------------------------------------
n128_statement_end_α:                                                         jmp   n158_statement_begin_α
n128_statement_end_β:                                                         jmp   n158_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
pad_right_body:                                                               jmp   n130_var_α
n129_statement_begin_β:                                                       jmp   n136_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n130_var_α:             mov              rax, qword ptr [r9 + 48]                       # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 608], rax                     # result
                        mov              qword ptr [rsp + 616], rdx;          jmp   n131_call_α
n130_var_β:                                                                   jmp   n129_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n131_call_α:            mov              rax, qword ptr [rsp + 608]
                        mov              qword ptr [rsp + 576], rax
                        mov              rax, qword ptr [rsp + 616]
                        mov              qword ptr [rsp + 584], rax
                        .section         .rodata
.Lrkfn536:              .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn536]
                        lea              rsi, [rsp + 576]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 560], rax
                        mov              qword ptr [rsp + 568], rdx
                        cmp              eax, 104;                            je    n129_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n132_lit_string_α
n131_call_β:                                                                  jmp   n129_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n132_lit_string_α:      mov              qword ptr [rsp + 624], 2                       # result
                        mov              dword ptr [rsp + 628], 1
                        mov              rax, qword ptr [rip + .Lx537_0]
                        mov              qword ptr [rsp + 632], rax;          jmp   n133_binop_α
n132_lit_string_β:                                                            jmp   n129_statement_begin_β
.Lx537_0:               .quad            .Lx537_0_s
.Lx537_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n133_binop_α:           mov              rdi, qword ptr [rsp + 560]
                        mov              rsi, qword ptr [rsp + 568]
                        mov              rdx, qword ptr [rsp + 624]
                        mov              rcx, qword ptr [rsp + 632]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 544], rax
                        mov              qword ptr [rsp + 552], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n134_assign_α
n133_binop_β:                                                                 jmp   n129_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n134_assign_α:          mov              rax, qword ptr [rsp + 544]
                        mov              rdx, qword ptr [rsp + 552]
                        mov              qword ptr [r9 + 48], rax                       # c
                        mov              qword ptr [r9 + 56], rdx;            jmp   n135_statement_end_α
n134_assign_β:                                                                jmp   n129_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n135_statement_end_α:                                                         jmp   n136_statement_begin_α
n135_statement_end_β:                                                         jmp   n136_statement_begin_α
#=======================================================================================================================
#         OUTPUT = index('foobar', 'xyz') ;* 0
#-----------------------------------------------------------------------------------------------------------------------
n136_statement_begin_α:                                                       jmp   n137_var_α
n136_statement_begin_β:                                                       jmp   n147_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n137_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n138_call_α
n137_var_β:             add              rsp, 16;                             jmp   n136_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n138_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd546:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd546]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx545_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n136_statement_begin_β
.Lx545_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n139_var_α
n138_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n136_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n139_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n140_coerce_numeric_α
n139_var_β:             add              rsp, 16
                        add              rsp, 32;                             jmp   n136_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n140_coerce_numeric_α:  sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # call
                        cmp              eax, 5;                              je    .Lx549_1
                        cmp              eax, 3;                              jne   .Lx549_0
                        mov              eax, dword ptr [rsp + 16]                      # var
                        cmp              eax, 3;                              jne   .Lx549_0
.Lx549_1:               mov              rax, qword ptr [rsp + 32]                      # call
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # call
                        mov              qword ptr [rsp + 8], rax;            jmp   n141_coerce_numeric_α
.Lx549_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 109
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n141_coerce_numeric_α
n140_coerce_numeric_β:  add              rsp, 16;                             jmp   n139_var_β
#-----------------------------------------------------------------------------------------------------------------------
n141_coerce_numeric_α:  sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5;                              je    .Lx551_1
                        cmp              eax, 3;                              jne   .Lx551_0
                        mov              eax, dword ptr [rsp + 48]                      # call
                        cmp              eax, 3;                              jne   .Lx551_0
.Lx551_1:               mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n142_cmp_test_α
.Lx551_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                                # call
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 110
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n142_cmp_test_α
n141_coerce_numeric_β:  add              rsp, 16;                             jmp   n140_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n142_cmp_test_α:        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_cmp_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             eax, eax;                            jns   .Lx553_240
                        add              rsp, 16;                             jmp   n141_coerce_numeric_β
.Lx553_240:             mov              qword ptr [rsp + 0], 0                         # result
                        mov              qword ptr [rsp + 8], 0;              jmp   n143_var_α
n142_cmp_test_β:        add              rsp, 16;                             jmp   n141_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n143_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n144_binop_α
n143_var_β:             add              rsp, 16;                             jmp   n142_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n144_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # cmp_test
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n145_assign_α
n144_binop_β:           add              rsp, 16;                             jmp   n143_var_β
#-----------------------------------------------------------------------------------------------------------------------
n145_assign_α:          mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax                       # pad_right
                        mov              qword ptr [r9 + 72], rdx;            jmp   n146_statement_end_α
n145_assign_β:                                                                jmp   n136_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n146_statement_end_α:   add              rsp, 128;                            jmp   RETURN
n146_statement_end_β:   add              rsp, 128;                            jmp   n147_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n147_statement_begin_α:                                                       jmp   n148_var_α
n147_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n148_var_α:             mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 816], rax                     # result
                        mov              qword ptr [rsp + 824], rdx;          jmp   n149_var_α
n148_var_β:                                                                   jmp   n147_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n149_var_α:             mov              rax, qword ptr [r9 + 48]                       # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 896], rax                     # result
                        mov              qword ptr [rsp + 904], rdx;          jmp   n150_var_α
n149_var_β:                                                                   jmp   n147_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n150_var_α:             mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 928], rax                     # result
                        mov              qword ptr [rsp + 936], rdx;          jmp   n151_var_α
n150_var_β:                                                                   jmp   n147_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n151_var_α:             mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 992], rax                     # result
                        mov              qword ptr [rsp + 1000], rdx;         jmp   n152_call_α
n151_var_β:                                                                   jmp   n147_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n152_call_α:            mov              rax, qword ptr [rsp + 992]
                        mov              qword ptr [rsp + 960], rax
                        mov              rax, qword ptr [rsp + 1000]
                        mov              qword ptr [rsp + 968], rax
                        .section         .rodata
.Lrkfn566:              .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn566]
                        lea              rsi, [rsp + 960]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 944], rax
                        mov              qword ptr [rsp + 952], rdx
                        cmp              eax, 104;                            je    n147_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n153_binop_α
n152_call_β:                                                                  jmp   n147_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n153_binop_α:           mov              eax, dword ptr [rsp + 928]
                        cmp              eax, 3;                              jne   .Lx567_0
                        mov              eax, dword ptr [rsp + 944]
                        cmp              eax, 3;                              jne   .Lx567_0
                        mov              rax, qword ptr [rsp + 936]
                        mov              rcx, qword ptr [rsp + 952]
                        sub              rax, rcx
                        mov              qword ptr [rsp + 912], 3
                        mov              qword ptr [rsp + 920], rax;          jmp   n154_call_α
.Lx567_0:               mov              rdi, qword ptr [rsp + 928]
                        mov              rsi, qword ptr [rsp + 936]
                        mov              rdx, qword ptr [rsp + 944]
                        mov              rcx, qword ptr [rsp + 952]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104;                            je    n147_statement_begin_β
                        mov              qword ptr [rsp + 912], rax
                        mov              qword ptr [rsp + 920], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n154_call_α
n153_binop_β:                                                                 jmp   n147_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n154_call_α:            mov              rax, qword ptr [rsp + 912]
                        mov              qword ptr [rsp + 864], rax
                        mov              rax, qword ptr [rsp + 920]
                        mov              qword ptr [rsp + 872], rax
                        mov              rax, qword ptr [rsp + 896]
                        mov              qword ptr [rsp + 848], rax
                        mov              rax, qword ptr [rsp + 904]
                        mov              qword ptr [rsp + 856], rax
                        .section         .rodata
.Lrkfn569:              .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn569]
                        lea              rsi, [rsp + 848]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 832], rax
                        mov              qword ptr [rsp + 840], rdx
                        cmp              eax, 104;                            je    n147_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n155_binop_α
n154_call_β:                                                                  jmp   n147_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n155_binop_α:           mov              rdi, qword ptr [rsp + 816]
                        mov              rsi, qword ptr [rsp + 824]
                        mov              rdx, qword ptr [rsp + 832]
                        mov              rcx, qword ptr [rsp + 840]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 800], rax
                        mov              qword ptr [rsp + 808], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n156_assign_α
n155_binop_β:                                                                 jmp   n147_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n156_assign_α:          mov              rax, qword ptr [rsp + 800]
                        mov              rdx, qword ptr [rsp + 808]
                        mov              qword ptr [r9 + 64], rax                       # pad_right
                        mov              qword ptr [r9 + 72], rdx;            jmp   n157_statement_end_α
n156_assign_β:                                                                jmp   n147_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n157_statement_end_α:                                                         jmp   RETURN
n157_statement_end_β:                                                         jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n158_statement_begin_α:                                                       jmp   n159_statement_end_α
n158_statement_begin_β:                                                       jmp   n160_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n159_statement_end_α:                                                         jmp   n160_statement_begin_α
n159_statement_end_β:                                                         jmp   n160_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n160_statement_begin_α:                                                       jmp   n161_func_activate_α
n160_statement_begin_β:                                                       jmp   n194_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n161_func_activate_α:   mov              rdi, qword ptr [rip + .Lx581_0]
                        mov              rsi, qword ptr [rip + .Lx581_1]
                        mov              edx, 3
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_ltrim_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n162_statement_end_α
n161_func_activate_β:                                                         jmp   n160_statement_begin_β
.Lx581_0:               .quad            .Lx581_0_s
.Lx581_0_s:             .string          "ltrim"
.Lx581_1:               .quad            .Lx581_1_s
.Lx581_1_s:             .string          "s,ws,r"
#-----------------------------------------------------------------------------------------------------------------------
n162_statement_end_α:                                                         jmp   n194_statement_begin_α
n162_statement_end_β:                                                         jmp   n194_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
ltrim_body:                                                                   jmp   n164_lit_string_α
n163_statement_begin_β:                                                       jmp   n176_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n164_lit_string_α:      mov              qword ptr [rsp + 1072], 2                      # result
                        mov              dword ptr [rsp + 1076], 1
                        mov              rax, qword ptr [rip + .Lx586_0]
                        mov              qword ptr [rsp + 1080], rax;         jmp   n165_lit_integer_α
n164_lit_string_β:                                                            jmp   n163_statement_begin_β
.Lx586_0:               .quad            .Lx586_0_s
.Lx586_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n165_lit_integer_α:     mov              qword ptr [rsp + 1136], 3                      # result
                        mov              rax, qword ptr [rip + .Lx587_0]
                        mov              qword ptr [rsp + 1144], rax;         jmp   n166_call_α
n165_lit_integer_β:                                                           jmp   n163_statement_begin_β
.Lx587_0:               .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n166_call_α:            mov              rax, qword ptr [rsp + 1136]
                        mov              qword ptr [rsp + 1104], rax
                        mov              rax, qword ptr [rsp + 1144]
                        mov              qword ptr [rsp + 1112], rax
                        .section         .rodata
.Lbynamefn107:          .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn107]
                        lea              rsi, [rsp + 1104]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1088], rax
                        mov              qword ptr [rsp + 1096], rdx
                        cmp              eax, 104;                            je    n163_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n167_binop_α
n166_call_β:                                                                  jmp   n163_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n167_binop_α:           mov              rdi, qword ptr [rsp + 1072]
                        mov              rsi, qword ptr [rsp + 1080]
                        mov              rdx, qword ptr [rsp + 1088]
                        mov              rcx, qword ptr [rsp + 1096]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1056], rax
                        mov              qword ptr [rsp + 1064], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n168_lit_integer_α
n167_binop_β:                                                                 jmp   n163_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n168_lit_integer_α:     mov              qword ptr [rsp + 1200], 3                      # result
                        mov              rax, qword ptr [rip + .Lx590_0]
                        mov              qword ptr [rsp + 1208], rax;         jmp   n169_call_α
n168_lit_integer_β:                                                           jmp   n163_statement_begin_β
.Lx590_0:               .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n169_call_α:            mov              rax, qword ptr [rsp + 1200]
                        mov              qword ptr [rsp + 1168], rax
                        mov              rax, qword ptr [rsp + 1208]
                        mov              qword ptr [rsp + 1176], rax
                        .section         .rodata
.Lbynamefn110:          .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn110]
                        lea              rsi, [rsp + 1168]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1152], rax
                        mov              qword ptr [rsp + 1160], rdx
                        cmp              eax, 104;                            je    n163_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n170_binop_α
n169_call_β:                                                                  jmp   n163_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n170_binop_α:           mov              rdi, qword ptr [rsp + 1056]
                        mov              rsi, qword ptr [rsp + 1064]
                        mov              rdx, qword ptr [rsp + 1152]
                        mov              rcx, qword ptr [rsp + 1160]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1040], rax
                        mov              qword ptr [rsp + 1048], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n171_lit_integer_α
n170_binop_β:                                                                 jmp   n163_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n171_lit_integer_α:     mov              qword ptr [rsp + 1264], 3                      # result
                        mov              rax, qword ptr [rip + .Lx593_0]
                        mov              qword ptr [rsp + 1272], rax;         jmp   n172_call_α
n171_lit_integer_β:                                                           jmp   n163_statement_begin_β
.Lx593_0:               .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n172_call_α:            mov              rax, qword ptr [rsp + 1264]
                        mov              qword ptr [rsp + 1232], rax
                        mov              rax, qword ptr [rsp + 1272]
                        mov              qword ptr [rsp + 1240], rax
                        .section         .rodata
.Lbynamefn113:          .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn113]
                        lea              rsi, [rsp + 1232]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1216], rax
                        mov              qword ptr [rsp + 1224], rdx
                        cmp              eax, 104;                            je    n163_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n173_binop_α
n172_call_β:                                                                  jmp   n163_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n173_binop_α:           mov              rdi, qword ptr [rsp + 1040]
                        mov              rsi, qword ptr [rsp + 1048]
                        mov              rdx, qword ptr [rsp + 1216]
                        mov              rcx, qword ptr [rsp + 1224]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1024], rax
                        mov              qword ptr [rsp + 1032], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n174_assign_α
n173_binop_β:                                                                 jmp   n163_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n174_assign_α:          mov              rax, qword ptr [rsp + 1024]
                        mov              rdx, qword ptr [rsp + 1032]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              qword ptr [r9 + 104], rdx;           jmp   n175_statement_end_α
n174_assign_β:                                                                jmp   n163_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n175_statement_end_α:                                                         jmp   n176_statement_begin_α
n175_statement_end_β:                                                         jmp   n176_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n176_statement_begin_α:                                                       jmp   n177_var_α
n176_statement_begin_β:                                                       jmp   n190_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n177_var_α:             mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 1504], rax                    # result
                        mov              qword ptr [rsp + 1512], rdx;         jmp   n178_match_begin_α
n177_var_β:                                                                   jmp   n190_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n178_match_begin_α:     mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              qword ptr [rsp + 40], rsp                      # old____
                        mov              rdi, qword ptr [rsp + 1504]
                        mov              rsi, qword ptr [rsp + 1512]
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
.Lx603_0:               mov              r14d, dword ptr [rsp + 0];           jmp   n179_match_pos_α
n178_match_begin_β:     mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d;                           jg    .Lx603_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx603_1
                                                                              jmp   .Lx603_0
.Lx603_1:
n178_match_begin_af:    sub              r12, 24                                        # cas_mark
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   n190_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n179_match_pos_α:       mov              rax, 0
                        cmp              r14d, eax;                           jne   n178_match_begin_β
                                                                              jmp   n180_match_alternate_α
n179_match_pos_β:                                                             jmp   n178_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n180_match_alternate_α: sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx606_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n189_match_span_α
.Lx606_21:              lea              rax, [rip + .Lx606_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n188_match_lit_α
n180_match_alternate_s0: lea              rax, [rip + .Lx606_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n180_match_alternate_as
n180_match_alternate_s1: lea              rax, [rip + .Lx606_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n180_match_alternate_as
.Lx606_40:                                                                    jmp   n189_match_span_β
.Lx606_41:                                                                    jmp   n188_match_lit_β
n180_match_alternate_as:                                                      jmp   n181_match_assign_save_α
n180_match_alternate_β: mov              rax, qword ptr [rsp + 8];            jmp   rax
n180_match_alternate_af: mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx606_19:              add              rsp, 32;                             jmp   n179_match_pos_β
#-----------------------------------------------------------------------------------------------------------------------
n181_match_assign_save_α: sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n182_match_rem_α
n181_match_assign_save_β: add              rsp, 16;                           jmp   n180_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n182_match_rem_α:       sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d;                          jmp   n183_match_assign_cond_α
n182_match_rem_β:       mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n180_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n183_match_assign_cond_α: mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n184_match_end_α
n183_match_assign_cond_β: sub              r12, 24;                           jmp   n182_match_rem_β
#-----------------------------------------------------------------------------------------------------------------------
n184_match_end_α:       mov              r8, r12
.Lx613_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                            jne   .Lx613_9
                        mov              eax, dword ptr [rsp + 32]
                        mov              dword ptr [rsp + 1360], eax
                        mov              qword ptr [rsp + 1384], r14
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx613_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                            jne   .Lx613_5
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
.Lx613_1:               test             rax, rax;                            je    .Lx613_2
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
                        lea              rcx, [rip + .Lx613_3]
                        lea              rdx, [rip + .Lx613_4];               jmp   rax
.Lx613_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx613_1
.Lx613_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx613_1
.Lx613_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx613_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                            jne   .Lx613_10
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   n185_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n185_lit_string_α:      mov              qword ptr [rsp + 1392], 2                      # result
                        mov              dword ptr [rsp + 1396], 0
                        mov              rax, qword ptr [rip + .Lx614_0]
                        mov              qword ptr [rsp + 1400], rax;         jmp   n186_match_replace_α
n185_lit_string_β:                                                            jmp   n190_statement_begin_α
.Lx614_0:               .quad            .Lx614_0_s
.Lx614_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n186_match_replace_α:   mov              rdi, qword ptr [rip + .Lx616_0]
                        mov              rsi, qword ptr [rsp + 1504]
                        mov              rdx, qword ptr [rsp + 1512]
                        mov              ecx, dword ptr [rsp + 1296]
                        mov              r8, qword ptr [rsp + 1320]
                        lea              r9, [rsp + 1392]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_replace@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx616_1
.Lx616_0:               .quad            .Lx616_0_s
.Lx616_0_s:             .string          "s"
.Lx616_1:                                                                     jmp   n187_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n187_statement_end_α:                                                         jmp   n190_statement_begin_α
n187_statement_end_β:                                                         jmp   n190_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n188_match_lit_α:                                                             jmp   n180_match_alternate_s1
n188_match_lit_β:                                                             jmp   n180_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n189_match_span_α:      lea              rdi, [rip + .S1]
                        lea              rsi, [rsp + 1440]
                        lea              rdx, [rsp + 1448]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_pat_prim_str@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax;                            js    n180_match_alternate_af
                        mov              r8, qword ptr [rsp + 1440]
                        mov              r9d, dword ptr [rsp + 1448]
                        mov              dword ptr [rsp + 1440], 0
.Lx622_0:               mov              eax, r14d
                        add              eax, dword ptr [rsp + 1440]
                        cmp              eax, r15d;                           jge   .Lx622_1
                        movsxd           rcx, eax
                        movzx            esi, byte ptr [r13+rcx]
                        mov              edx, 0
.Lx622_2:               cmp              edx, r9d;                            jge   .Lx622_1
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi;                            je    .Lx622_3
                        add              edx, 1;                              jmp   .Lx622_2
.Lx622_3:               add              dword ptr [rsp + 1440], 1;           jmp   .Lx622_0
.Lx622_1:               mov              eax, dword ptr [rsp + 1440]
                        test             eax, eax;                            jle   n180_match_alternate_af
                        mov              edx, r14d
                        mov              dword ptr [rsp + 1444], edx
                        add              edx, eax
                        mov              r14d, edx;                           jmp   n180_match_alternate_s0
n189_match_span_β:      mov              r14d, dword ptr [rsp + 1444];        jmp   n180_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n190_statement_begin_α:                                                       jmp   n191_var_α
n190_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n191_var_α:             mov              rax, qword ptr [r9 + 112]                      # r
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 1520], rax                    # result
                        mov              qword ptr [rsp + 1528], rdx;         jmp   n192_assign_α
n191_var_β:                                                                   jmp   n190_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n192_assign_α:          mov              rax, qword ptr [rsp + 1520]
                        mov              rdx, qword ptr [rsp + 1528]
                        mov              qword ptr [r9 + 80], rax                       # ltrim
                        mov              qword ptr [r9 + 88], rdx;            jmp   n193_statement_end_α
n192_assign_β:                                                                jmp   n190_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n193_statement_end_α:                                                         jmp   RETURN
n193_statement_end_β:                                                         jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n194_statement_begin_α:                                                       jmp   n195_statement_end_α
n194_statement_begin_β:                                                       jmp   n196_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n195_statement_end_α:                                                         jmp   n196_statement_begin_α
n195_statement_end_β:                                                         jmp   n196_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n196_statement_begin_α:                                                       jmp   n197_func_activate_α
n196_statement_begin_β:                                                       jmp   n252_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n197_func_activate_α:   mov              rdi, qword ptr [rip + .Lx636_0]
                        mov              rsi, qword ptr [rip + .Lx636_1]
                        mov              edx, 4
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_rtrim_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n198_statement_end_α
n197_func_activate_β:                                                         jmp   n196_statement_begin_β
.Lx636_0:               .quad            .Lx636_0_s
.Lx636_0_s:             .string          "rtrim"
.Lx636_1:               .quad            .Lx636_1_s
.Lx636_1_s:             .string          "s,ws,i,ch"
#-----------------------------------------------------------------------------------------------------------------------
n198_statement_end_α:                                                         jmp   n252_statement_begin_α
n198_statement_end_β:                                                         jmp   n252_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
rtrim_body:                                                                   jmp   n200_lit_string_α
n199_statement_begin_β:                                                       jmp   n212_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n200_lit_string_α:      mov              qword ptr [rsp + 1600], 2                      # result
                        mov              dword ptr [rsp + 1604], 1
                        mov              rax, qword ptr [rip + .Lx641_0]
                        mov              qword ptr [rsp + 1608], rax;         jmp   n201_lit_integer_α
n200_lit_string_β:                                                            jmp   n199_statement_begin_β
.Lx641_0:               .quad            .Lx641_0_s
.Lx641_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n201_lit_integer_α:     mov              qword ptr [rsp + 1664], 3                      # result
                        mov              rax, qword ptr [rip + .Lx642_0]
                        mov              qword ptr [rsp + 1672], rax;         jmp   n202_call_α
n201_lit_integer_β:                                                           jmp   n199_statement_begin_β
.Lx642_0:               .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n202_call_α:            mov              rax, qword ptr [rsp + 1664]
                        mov              qword ptr [rsp + 1632], rax
                        mov              rax, qword ptr [rsp + 1672]
                        mov              qword ptr [rsp + 1640], rax
                        .section         .rodata
.Lbynamefn143:          .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn143]
                        lea              rsi, [rsp + 1632]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1616], rax
                        mov              qword ptr [rsp + 1624], rdx
                        cmp              eax, 104;                            je    n199_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n203_binop_α
n202_call_β:                                                                  jmp   n199_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n203_binop_α:           mov              rdi, qword ptr [rsp + 1600]
                        mov              rsi, qword ptr [rsp + 1608]
                        mov              rdx, qword ptr [rsp + 1616]
                        mov              rcx, qword ptr [rsp + 1624]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1584], rax
                        mov              qword ptr [rsp + 1592], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n204_lit_integer_α
n203_binop_β:                                                                 jmp   n199_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n204_lit_integer_α:     mov              qword ptr [rsp + 1728], 3                      # result
                        mov              rax, qword ptr [rip + .Lx645_0]
                        mov              qword ptr [rsp + 1736], rax;         jmp   n205_call_α
n204_lit_integer_β:                                                           jmp   n199_statement_begin_β
.Lx645_0:               .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n205_call_α:            mov              rax, qword ptr [rsp + 1728]
                        mov              qword ptr [rsp + 1696], rax
                        mov              rax, qword ptr [rsp + 1736]
                        mov              qword ptr [rsp + 1704], rax
                        .section         .rodata
.Lbynamefn146:          .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn146]
                        lea              rsi, [rsp + 1696]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1680], rax
                        mov              qword ptr [rsp + 1688], rdx
                        cmp              eax, 104;                            je    n199_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n206_binop_α
n205_call_β:                                                                  jmp   n199_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n206_binop_α:           mov              rdi, qword ptr [rsp + 1584]
                        mov              rsi, qword ptr [rsp + 1592]
                        mov              rdx, qword ptr [rsp + 1680]
                        mov              rcx, qword ptr [rsp + 1688]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1568], rax
                        mov              qword ptr [rsp + 1576], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n207_lit_integer_α
n206_binop_β:                                                                 jmp   n199_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n207_lit_integer_α:     mov              qword ptr [rsp + 1792], 3                      # result
                        mov              rax, qword ptr [rip + .Lx648_0]
                        mov              qword ptr [rsp + 1800], rax;         jmp   n208_call_α
n207_lit_integer_β:                                                           jmp   n199_statement_begin_β
.Lx648_0:               .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n208_call_α:            mov              rax, qword ptr [rsp + 1792]
                        mov              qword ptr [rsp + 1760], rax
                        mov              rax, qword ptr [rsp + 1800]
                        mov              qword ptr [rsp + 1768], rax
                        .section         .rodata
.Lbynamefn149:          .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn149]
                        lea              rsi, [rsp + 1760]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1744], rax
                        mov              qword ptr [rsp + 1752], rdx
                        cmp              eax, 104;                            je    n199_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n209_binop_α
n208_call_β:                                                                  jmp   n199_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n209_binop_α:           mov              rdi, qword ptr [rsp + 1568]
                        mov              rsi, qword ptr [rsp + 1576]
                        mov              rdx, qword ptr [rsp + 1744]
                        mov              rcx, qword ptr [rsp + 1752]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1552], rax
                        mov              qword ptr [rsp + 1560], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n210_assign_α
n209_binop_β:                                                                 jmp   n199_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n210_assign_α:          mov              rax, qword ptr [rsp + 1552]
                        mov              rdx, qword ptr [rsp + 1560]
                        mov              qword ptr [r9 + 96], rax                       # ws
                        mov              qword ptr [r9 + 104], rdx;           jmp   n211_statement_end_α
n210_assign_β:                                                                jmp   n199_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n211_statement_end_α:                                                         jmp   n212_statement_begin_α
n211_statement_end_β:                                                         jmp   n212_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n212_statement_begin_α:                                                       jmp   n213_var_α
n212_statement_begin_β:                                                       jmp   n217_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n213_var_α:             mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 1856], rax                    # result
                        mov              qword ptr [rsp + 1864], rdx;         jmp   n214_call_α
n213_var_β:                                                                   jmp   n212_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n214_call_α:            mov              rax, qword ptr [rsp + 1856]
                        mov              qword ptr [rsp + 1824], rax
                        mov              rax, qword ptr [rsp + 1864]
                        mov              qword ptr [rsp + 1832], rax
                        .section         .rodata
.Lrkfn658:              .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn658]
                        lea              rsi, [rsp + 1824]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1808], rax
                        mov              qword ptr [rsp + 1816], rdx
                        cmp              eax, 104;                            je    n212_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n215_assign_α
n214_call_β:                                                                  jmp   n212_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n215_assign_α:          mov              rax, qword ptr [rsp + 1808]
                        mov              rdx, qword ptr [rsp + 1816]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              qword ptr [r9 + 152], rdx;           jmp   n216_statement_end_α
n215_assign_β:                                                                jmp   n212_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n216_statement_end_α:                                                         jmp   n217_statement_begin_α
n216_statement_end_β:                                                         jmp   n217_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n217_statement_begin_α:                                                       jmp   n218_var_α
n217_statement_begin_β:                                                       jmp   n224_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n218_var_α:             mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 1920], rax                    # result
                        mov              qword ptr [rsp + 1928], rdx;         jmp   n219_lit_integer_α
n218_var_β:                                                                   jmp   n217_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n219_lit_integer_α:     mov              qword ptr [rsp + 1904], 3                      # result
                        mov              rax, qword ptr [rip + .Lx665_0]
                        mov              qword ptr [rsp + 1912], rax;         jmp   n220_coerce_numeric_α
n219_lit_integer_β:                                                           jmp   n217_statement_begin_β
.Lx665_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n220_coerce_numeric_α:  mov              eax, dword ptr [rsp + 1920]
                        cmp              eax, 5;                              je    .Lx667_1
                        cmp              eax, 3;                              jne   .Lx667_0
                        mov              eax, dword ptr [rsp + 1904]
                        cmp              eax, 3;                              jne   .Lx667_0
.Lx667_1:               mov              rax, qword ptr [rsp + 1920]
                        mov              qword ptr [rsp + 1888], rax
                        mov              rax, qword ptr [rsp + 1928]
                        mov              qword ptr [rsp + 1896], rax;         jmp   n221_coerce_numeric_α
.Lx667_0:               lea              rdi, [rsp + 1920]
                        lea              rsi, [rsp + 1904]
                        lea              rdx, [rsp + 1888]
                        mov              rcx, 118
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n221_coerce_numeric_α
n220_coerce_numeric_β:                                                        jmp   n217_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n221_coerce_numeric_α:  mov              eax, dword ptr [rsp + 1904]
                        cmp              eax, 5;                              je    .Lx669_1
                        cmp              eax, 3;                              jne   .Lx669_0
                        mov              eax, dword ptr [rsp + 1920]
                        cmp              eax, 3;                              jne   .Lx669_0
.Lx669_1:               mov              rax, qword ptr [rsp + 1904]
                        mov              qword ptr [rsp + 1872], rax
                        mov              rax, qword ptr [rsp + 1912]
                        mov              qword ptr [rsp + 1880], rax;         jmp   n222_cmp_test_α
.Lx669_0:               lea              rdi, [rsp + 1904]
                        lea              rsi, [rsp + 1920]
                        lea              rdx, [rsp + 1872]
                        mov              rcx, 119
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n222_cmp_test_α
n221_coerce_numeric_β:                                                        jmp   n217_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n222_cmp_test_α:        lea              rdi, [rsp + 1888]
                        lea              rsi, [rsp + 1872]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_cmp_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             eax, eax;                            jg    n217_statement_begin_β
                                                                              jmp   n223_statement_end_α
n222_cmp_test_β:                                                              jmp   n217_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n223_statement_end_α:                                                         jmp   n245_statement_begin_α
n223_statement_end_β:                                                         jmp   n224_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n224_statement_begin_α:                                                       jmp   n225_var_α
n224_statement_begin_β:                                                       jmp   n231_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n225_var_α:             mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 2016], rax                    # result
                        mov              qword ptr [rsp + 2024], rdx;         jmp   n226_var_α
n225_var_β:                                                                   jmp   n224_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n226_var_α:             mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 2032], rax                    # result
                        mov              qword ptr [rsp + 2040], rdx;         jmp   n227_lit_integer_α
n226_var_β:                                                                   jmp   n224_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n227_lit_integer_α:     mov              qword ptr [rsp + 2048], 3                      # result
                        mov              rax, qword ptr [rip + .Lx678_0]
                        mov              qword ptr [rsp + 2056], rax;         jmp   n228_call_α
n227_lit_integer_β:                                                           jmp   n224_statement_begin_β
.Lx678_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n228_call_α:            mov              rax, qword ptr [rsp + 2048]
                        mov              qword ptr [rsp + 1984], rax
                        mov              rax, qword ptr [rsp + 2056]
                        mov              qword ptr [rsp + 1992], rax
                        mov              rax, qword ptr [rsp + 2032]
                        mov              qword ptr [rsp + 1968], rax
                        mov              rax, qword ptr [rsp + 2040]
                        mov              qword ptr [rsp + 1976], rax
                        mov              rax, qword ptr [rsp + 2016]
                        mov              qword ptr [rsp + 1952], rax
                        mov              rax, qword ptr [rsp + 2024]
                        mov              qword ptr [rsp + 1960], rax
                        .section         .rodata
.Lrkfn680:              .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn680]
                        lea              rsi, [rsp + 1952]
                        mov              edx, 3
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1936], rax
                        mov              qword ptr [rsp + 1944], rdx
                        cmp              eax, 104;                            je    n224_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n229_assign_α
n228_call_β:                                                                  jmp   n224_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n229_assign_α:          mov              rax, qword ptr [rsp + 1936]
                        mov              rdx, qword ptr [rsp + 1944]
                        mov              qword ptr [r9 + 160], rax                      # ch
                        mov              qword ptr [r9 + 168], rdx;           jmp   n230_statement_end_α
n229_assign_β:                                                                jmp   n224_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n230_statement_end_α:                                                         jmp   n231_statement_begin_α
n230_statement_end_β:                                                         jmp   n231_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n231_statement_begin_α:                                                       jmp   n232_var_α
n231_statement_begin_β:                                                       jmp   n245_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n232_var_α:             mov              rax, qword ptr [r9 + 160]                      # ch
                        mov              rdx, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 2192], rax                    # result
                        mov              qword ptr [rsp + 2200], rdx;         jmp   n233_var_α
n232_var_β:                                                                   jmp   n245_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n233_var_α:             mov              rax, qword ptr [r9 + 96]                       # ws
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 2176], rax                    # result
                        mov              qword ptr [rsp + 2184], rdx;         jmp   n234_coerce_string_α
n233_var_β:                                                                   jmp   n245_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n234_coerce_string_α:   lea              rdi, [rsp + 2176]
                        lea              rsi, [rsp + 2160]
                        mov              rdx, 3866683
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_str_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n235_match_begin_α
n234_coerce_string_β:                                                         jmp   n245_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n235_match_begin_α:     mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              qword ptr [rsp + 40], rsp                      # old____
                        mov              rdi, qword ptr [rsp + 2192]
                        mov              rsi, qword ptr [rsp + 2200]
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
.Lx691_0:               mov              r14d, dword ptr [rsp + 0];           jmp   n236_match_any_α
n235_match_begin_β:     mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d;                           jg    .Lx691_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx691_1
                                                                              jmp   .Lx691_0
.Lx691_1:
n235_match_begin_af:    sub              r12, 24                                        # cas_mark
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   n245_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n236_match_any_α:       mov              eax, r14d
                        cmp              eax, r15d;                           jge   n235_match_begin_β
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        mov              r8, qword ptr [rsp + 2168]
                        mov              r9d, dword ptr [rsp + 2164]
                        mov              edx, 0
.Lx693_5:               cmp              edx, r9d;                            jge   n235_match_begin_β
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi;                            je    .Lx693_6
                        add              edx, 1;                              jmp   .Lx693_5
.Lx693_6:               add              r14d, 1;                             jmp   n237_match_end_α
n236_match_any_β:       sub              r14d, 1;                             jmp   n235_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n237_match_end_α:       mov              r8, r12
.Lx695_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                            jne   .Lx695_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx695_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                            jne   .Lx695_5
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
.Lx695_1:               test             rax, rax;                            je    .Lx695_2
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
                        lea              rcx, [rip + .Lx695_3]
                        lea              rdx, [rip + .Lx695_4];               jmp   rax
.Lx695_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx695_1
.Lx695_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx695_1
.Lx695_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx695_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                            jne   .Lx695_10
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   n238_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n238_statement_end_α:                                                         jmp   n239_statement_begin_α
n238_statement_end_β:                                                         jmp   n245_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n239_statement_begin_α:                                                       jmp   n240_var_α
n239_statement_begin_β:                                                       jmp   n217_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n240_var_α:             mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 2224], rax                    # result
                        mov              qword ptr [rsp + 2232], rdx;         jmp   n241_lit_integer_α
n240_var_β:                                                                   jmp   n239_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n241_lit_integer_α:     mov              qword ptr [rsp + 2240], 3                      # result
                        mov              rax, qword ptr [rip + .Lx701_0]
                        mov              qword ptr [rsp + 2248], rax;         jmp   n242_binop_α
n241_lit_integer_β:                                                           jmp   n239_statement_begin_β
.Lx701_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n242_binop_α:           mov              eax, dword ptr [rsp + 2224]
                        cmp              eax, 3;                              jne   .Lx702_0
                        mov              rax, qword ptr [rsp + 2232]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rsp + 2208], 3
                        mov              qword ptr [rsp + 2216], rax;         jmp   n243_assign_α
.Lx702_0:               mov              rdi, qword ptr [rsp + 2224]
                        mov              rsi, qword ptr [rsp + 2232]
                        mov              rdx, qword ptr [rsp + 2240]
                        mov              rcx, qword ptr [rsp + 2248]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104;                            jne   .Lx702_240
                        add              rsp, 32;                             jmp   n239_statement_begin_β
.Lx702_240:             mov              qword ptr [rsp + 2208], rax
                        mov              qword ptr [rsp + 2216], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n243_assign_α
n242_binop_β:           add              rsp, 32;                             jmp   n239_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n243_assign_α:          mov              rax, qword ptr [rsp + 2208]
                        mov              rdx, qword ptr [rsp + 2216]
                        mov              qword ptr [r9 + 144], rax                      # i
                        mov              qword ptr [r9 + 152], rdx;           jmp   n244_statement_end_α
n243_assign_β:                                                                jmp   n239_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n244_statement_end_α:                                                         jmp   n217_statement_begin_α
n244_statement_end_β:                                                         jmp   n217_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n245_statement_begin_α:                                                       jmp   n246_var_α
n245_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n246_var_α:             mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 2336], rax                    # result
                        mov              qword ptr [rsp + 2344], rdx;         jmp   n247_lit_integer_α
n246_var_β:                                                                   jmp   n245_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n247_lit_integer_α:     mov              qword ptr [rsp + 2352], 3                      # result
                        mov              rax, qword ptr [rip + .Lx709_0]
                        mov              qword ptr [rsp + 2360], rax;         jmp   n248_var_α
n247_lit_integer_β:                                                           jmp   n245_statement_begin_β
.Lx709_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n248_var_α:             mov              rax, qword ptr [r9 + 144]                      # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 2368], rax                    # result
                        mov              qword ptr [rsp + 2376], rdx;         jmp   n249_call_α
n248_var_β:                                                                   jmp   n245_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n249_call_α:            mov              rax, qword ptr [rsp + 2368]
                        mov              qword ptr [rsp + 2304], rax
                        mov              rax, qword ptr [rsp + 2376]
                        mov              qword ptr [rsp + 2312], rax
                        mov              rax, qword ptr [rsp + 2352]
                        mov              qword ptr [rsp + 2288], rax
                        mov              rax, qword ptr [rsp + 2360]
                        mov              qword ptr [rsp + 2296], rax
                        mov              rax, qword ptr [rsp + 2336]
                        mov              qword ptr [rsp + 2272], rax
                        mov              rax, qword ptr [rsp + 2344]
                        mov              qword ptr [rsp + 2280], rax
                        .section         .rodata
.Lrkfn712:              .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn712]
                        lea              rsi, [rsp + 2272]
                        mov              edx, 3
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 2256], rax
                        mov              qword ptr [rsp + 2264], rdx
                        cmp              eax, 104;                            je    n245_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n250_assign_α
n249_call_β:                                                                  jmp   n245_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n250_assign_α:          mov              rax, qword ptr [rsp + 2256]
                        mov              rdx, qword ptr [rsp + 2264]
                        mov              qword ptr [r9 + 128], rax                      # rtrim
                        mov              qword ptr [r9 + 136], rdx;           jmp   n251_statement_end_α
n250_assign_β:                                                                jmp   n245_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n251_statement_end_α:                                                         jmp   RETURN
n251_statement_end_β:                                                         jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n252_statement_begin_α:                                                       jmp   n253_statement_end_α
n252_statement_begin_β:                                                       jmp   n254_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n253_statement_end_α:                                                         jmp   n254_statement_begin_α
n253_statement_end_β:                                                         jmp   n254_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n254_statement_begin_α:                                                       jmp   n255_func_activate_α
n254_statement_begin_β:                                                       jmp   n263_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n255_func_activate_α:   mov              rdi, qword ptr [rip + .Lx723_0]
                        mov              rsi, qword ptr [rip + .Lx723_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_trimws_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n256_statement_end_α
n255_func_activate_β:                                                         jmp   n254_statement_begin_β
.Lx723_0:               .quad            .Lx723_0_s
.Lx723_0_s:             .string          "trimws"
.Lx723_1:               .quad            .Lx723_1_s
.Lx723_1_s:             .string          "s"
#-----------------------------------------------------------------------------------------------------------------------
n256_statement_end_α:                                                         jmp   n263_statement_begin_α
n256_statement_end_β:                                                         jmp   n263_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
trimws_body:                                                                  jmp   n258_var_α
n257_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n258_var_α:             mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 2496], rax                    # result
                        mov              qword ptr [rsp + 2504], rdx;         jmp   n259_call_α
n258_var_β:                                                                   jmp   n257_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n259_call_α:            sub              rsp, 48
                        mov              rax, qword ptr [rsp + 2544]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 2552]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx730_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + rtrim_alpha];            jmp   rax
                        lea              r8, [rip + g_gc_pending]
                        mov              eax, dword ptr [r8 + 0]
                        test             eax, eax;                            jne   .Lx730_20
                        mov              rax, qword ptr [rsp + 2496]
                        mov              rdx, qword ptr [rsp + 2504]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx;             jmp   .Lx730_21
.Lx730_20:              mov              edi, 0
                        mov              rsi, qword ptr [rsp + 2496]
                        mov              rdx, qword ptr [rsp + 2504]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx730_21:              mov              rdi, qword ptr [rip + .Lx730_0]
                        mov              esi, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax;                            je    .Lx730_1
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
                        lea              rcx, [rip + .Lx730_3]
                        lea              rdx, [rip + .Lx730_4];               jmp   rax
.Lx730_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx730_2
.Lx730_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx730_2
.Lx730_1:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_faildescr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx730_2:               mov              qword ptr [rsp + 2448], rax
                        mov              qword ptr [rsp + 2456], rdx
                        cmp              eax, 104;                            je    n257_statement_begin_β
                                                                              jmp   n260_call_α
n259_call_β:                                                                  jmp   n257_statement_begin_β
.Lx730_0:               .quad            .Lx730_0_s
.Lx730_0_s:             .string          "rtrim"
#-----------------------------------------------------------------------------------------------------------------------
n260_call_α:            sub              rsp, 48
                        mov              rax, qword ptr [rsp + 2496]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 2504]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx732_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + ltrim_alpha];            jmp   rax
                        lea              r8, [rip + g_gc_pending]
                        mov              eax, dword ptr [r8 + 0]
                        test             eax, eax;                            jne   .Lx732_20
                        mov              rax, qword ptr [rsp + 2448]
                        mov              rdx, qword ptr [rsp + 2456]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx;             jmp   .Lx732_21
.Lx732_20:              mov              edi, 0
                        mov              rsi, qword ptr [rsp + 2448]
                        mov              rdx, qword ptr [rsp + 2456]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx732_21:              mov              rdi, qword ptr [rip + .Lx732_0]
                        mov              esi, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax;                            je    .Lx732_1
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
                        lea              rcx, [rip + .Lx732_3]
                        lea              rdx, [rip + .Lx732_4];               jmp   rax
.Lx732_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx732_2
.Lx732_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx732_2
.Lx732_1:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_faildescr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx732_2:               mov              qword ptr [rsp + 2400], rax
                        mov              qword ptr [rsp + 2408], rdx
                        cmp              eax, 104;                            je    n257_statement_begin_β
                                                                              jmp   n261_assign_α
n260_call_β:                                                                  jmp   n257_statement_begin_β
.Lx732_0:               .quad            .Lx732_0_s
.Lx732_0_s:             .string          "ltrim"
#-----------------------------------------------------------------------------------------------------------------------
n261_assign_α:          mov              rax, qword ptr [rsp + 2400]
                        mov              rdx, qword ptr [rsp + 2408]
                        mov              qword ptr [r9 + 176], rax                      # trimws
                        mov              qword ptr [r9 + 184], rdx;           jmp   n262_statement_end_α
n261_assign_β:                                                                jmp   n257_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n262_statement_end_α:                                                         jmp   RETURN
n262_statement_end_β:                                                         jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n263_statement_begin_α:                                                       jmp   n264_statement_end_α
n263_statement_begin_β:                                                       jmp   n265_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n264_statement_end_α:                                                         jmp   n265_statement_begin_α
n264_statement_end_β:                                                         jmp   n265_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n265_statement_begin_α:                                                       jmp   n266_func_activate_α
n265_statement_begin_β:                                                       jmp   n274_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n266_func_activate_α:   mov              rdi, qword ptr [rip + .Lx743_0]
                        mov              rsi, qword ptr [rip + .Lx743_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_repeat_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n267_statement_end_α
n266_func_activate_β:                                                         jmp   n265_statement_begin_β
.Lx743_0:               .quad            .Lx743_0_s
.Lx743_0_s:             .string          "repeat"
.Lx743_1:               .quad            .Lx743_1_s
.Lx743_1_s:             .string          "s,n"
#-----------------------------------------------------------------------------------------------------------------------
n267_statement_end_α:                                                         jmp   n274_statement_begin_α
n267_statement_end_β:                                                         jmp   n274_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
repeat_body:                                                                  jmp   n269_var_α
n268_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n269_var_α:             mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 2592], rax                    # result
                        mov              qword ptr [rsp + 2600], rdx;         jmp   n270_var_α
n269_var_β:                                                                   jmp   n268_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n270_var_α:             mov              rax, qword ptr [r9 + 32]                       # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 2608], rax                    # result
                        mov              qword ptr [rsp + 2616], rdx;         jmp   n271_call_α
n270_var_β:                                                                   jmp   n268_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n271_call_α:            mov              rax, qword ptr [rsp + 2608]
                        mov              qword ptr [rsp + 2560], rax
                        mov              rax, qword ptr [rsp + 2616]
                        mov              qword ptr [rsp + 2568], rax
                        mov              rax, qword ptr [rsp + 2592]
                        mov              qword ptr [rsp + 2544], rax
                        mov              rax, qword ptr [rsp + 2600]
                        mov              qword ptr [rsp + 2552], rax
                        .section         .rodata
.Lrkfn751:              .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn751]
                        lea              rsi, [rsp + 2544]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 2528], rax
                        mov              qword ptr [rsp + 2536], rdx
                        cmp              eax, 104;                            je    n268_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n272_assign_α
n271_call_β:                                                                  jmp   n268_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n272_assign_α:          mov              rax, qword ptr [rsp + 2528]
                        mov              rdx, qword ptr [rsp + 2536]
                        mov              qword ptr [r9 + 192], rax                      # repeat
                        mov              qword ptr [r9 + 200], rdx;           jmp   n273_statement_end_α
n272_assign_β:                                                                jmp   n268_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n273_statement_end_α:                                                         jmp   RETURN
n273_statement_end_β:                                                         jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n274_statement_begin_α:                                                       jmp   n275_statement_end_α
n274_statement_begin_β:                                                       jmp   n276_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n275_statement_end_α:                                                         jmp   n276_statement_begin_α
n275_statement_end_β:                                                         jmp   n276_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n276_statement_begin_α:                                                       jmp   n277_func_activate_α
n276_statement_begin_β:                                                       jmp   n290_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n277_func_activate_α:   mov              rdi, qword ptr [rip + .Lx762_0]
                        mov              rsi, qword ptr [rip + .Lx762_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_contains_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n278_statement_end_α
n277_func_activate_β:                                                         jmp   n276_statement_begin_β
.Lx762_0:               .quad            .Lx762_0_s
.Lx762_0_s:             .string          "contains"
.Lx762_1:               .quad            .Lx762_1_s
.Lx762_1_s:             .string          "s,t"
#-----------------------------------------------------------------------------------------------------------------------
n278_statement_end_α:                                                         jmp   n290_statement_begin_α
n278_statement_end_β:                                                         jmp   n290_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
contains_body:                                                                jmp   n280_var_α
n279_statement_begin_β:                                                       jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n280_var_α:             mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 2800], rax                    # result
                        mov              qword ptr [rsp + 2808], rdx;         jmp   n281_var_α
n280_var_β:                                                                   jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n281_var_α:             mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 2784], rax                    # result
                        mov              qword ptr [rsp + 2792], rdx;         jmp   n282_assign_α
n281_var_β:                                                                   jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n282_assign_α:          mov              rsi, qword ptr [rsp + 2784]
                        mov              rdx, qword ptr [rsp + 2792]
                        mov              rdi, qword ptr [rip + .Lx769_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n283_var_α
n282_assign_β:                                                                jmp   FRETURN
.Lx769_0:               .quad            .Lx769_0_s
.Lx769_0_s:             .string          "PATV$0"
#-----------------------------------------------------------------------------------------------------------------------
n283_var_α:             mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 2768], rax                    # result
                        mov              qword ptr [rsp + 2776], rdx;         jmp   n284_coerce_string_α
n283_var_β:                                                                   jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n284_coerce_string_α:   lea              rdi, [rsp + 2768]
                        lea              rsi, [rsp + 2752]
                        mov              rdx, 4522053
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_str_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n285_match_begin_α
n284_coerce_string_β:                                                         jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n285_match_begin_α:     mov              qword ptr [rsp + 2704], r13                    # outer_Σ
                        mov              qword ptr [rsp + 2712], r14                    # outer_δ
                        mov              qword ptr [rsp + 2720], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 2728], rax                    # cap_gen
                        mov              qword ptr [rsp + 2696], rsp                    # old____
                        mov              rdi, qword ptr [rsp + 2800]
                        mov              rsi, qword ptr [rsp + 2808]
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
                        mov              qword ptr [rsp + 2672], rsp                    # zls2_mark
                        mov              dword ptr [rsp + 2656], 0                      # start_δ
.Lx774_0:               mov              r14d, dword ptr [rsp + 2656];        jmp   n286_match_break_α
n285_match_begin_β:     add              dword ptr [rsp + 2656], 1
                        mov              eax, dword ptr [rsp + 2656]
                        cmp              eax, r15d;                           jg    .Lx774_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx774_1
                                                                              jmp   .Lx774_0
.Lx774_1:
n285_match_begin_af:    sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rsp + 2672]
                        mov              r13, qword ptr [rsp + 2704]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2712]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2720]                    # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 2728]                    # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n286_match_break_α:     sub              rsp, 16
                        mov              dword ptr [rsp + 0], 0
                        mov              r8, qword ptr [rsp + 2760]
                        mov              r9d, dword ptr [rsp + 2756]
.Lx776_0:               mov              eax, r14d
                        add              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d;                           jl    .Lx776_240
                        add              rsp, 16;                             jmp   n285_match_begin_β
.Lx776_240:             movsxd           rcx, eax
                        movzx            esi, byte ptr [r13+rcx]
                        mov              edx, 0
.Lx776_2:               cmp              edx, r9d;                            jge   .Lx776_3
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi;                            je    .Lx776_1
                        add              edx, 1;                              jmp   .Lx776_2
.Lx776_3:               add              dword ptr [rsp + 0], 1;              jmp   .Lx776_0
.Lx776_1:               mov              eax, r14d
                        add              eax, dword ptr [rsp + 0]
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, eax;                           jmp   n287_match_defer_α
n286_match_break_β:     mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16;                             jmp   n285_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n287_match_defer_α:     lea              rdi, [rip + .S2]
                        xor              esi, esi
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_get_pat_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax;                            jz    .Lx777_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx777_4]
                        lea              r11, [rip + .Lx777_5];               jmp   rax
.Lx777_4:                                                                     jmp   n288_match_end_α
.Lx777_5:                                                                     jmp   n286_match_break_β
.Lx777_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S2]
                        xor              esi, esi
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx777_2:               test             rax, rax;                            je    .Lx777_3
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
                        lea              rcx, [rip + .Lx777_7]
                        lea              rdx, [rip + .Lx777_8];               jmp   rax
.Lx777_7:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        call             rt_defer_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx777_2
.Lx777_8:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        call             rt_defer_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx777_2
.Lx777_3:               add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_close@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             eax, eax;                            js    n286_match_break_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx777_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n288_match_end_α
.Lx777_6:               add              rsp, 16;                             jmp   n286_match_break_β
n287_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n288_match_end_α:       mov              r8, r12
.Lx779_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                            jne   .Lx779_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx779_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                            jne   .Lx779_5
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
.Lx779_1:               test             rax, rax;                            je    .Lx779_2
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
                        lea              rcx, [rip + .Lx779_3]
                        lea              rdx, [rip + .Lx779_4];               jmp   rax
.Lx779_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx779_1
.Lx779_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx779_1
.Lx779_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx779_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                            jne   .Lx779_10
                        mov              r13, qword ptr [rsp + 2704]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2712]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2720]                    # outer_Δ
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   n289_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n289_statement_end_α:                                                         jmp   RETURN
n289_statement_end_β:                                                         jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n290_statement_begin_α:                                                       jmp   n291_statement_end_α
n290_statement_begin_β:                                                       jmp   n292_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n291_statement_end_α:                                                         jmp   n292_statement_begin_α
n291_statement_end_β:                                                         jmp   n292_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n292_statement_begin_α:                                                       jmp   n293_func_activate_α
n292_statement_begin_β:                                                       jmp   n304_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n293_func_activate_α:   mov              rdi, qword ptr [rip + .Lx789_0]
                        mov              rsi, qword ptr [rip + .Lx789_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_startswith_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n294_statement_end_α
n293_func_activate_β:                                                         jmp   n292_statement_begin_β
.Lx789_0:               .quad            .Lx789_0_s
.Lx789_0_s:             .string          "startswith"
.Lx789_1:               .quad            .Lx789_1_s
.Lx789_1_s:             .string          "s,t"
#-----------------------------------------------------------------------------------------------------------------------
n294_statement_end_α:                                                         jmp   n304_statement_begin_α
n294_statement_end_β:                                                         jmp   n304_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
startswith_body:                                                              jmp   n296_var_α
n295_statement_begin_β:                                                       jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n296_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n297_var_α
n296_var_β:             add              rsp, 16;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n297_var_α:             mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 2944], rax                    # result
                        mov              qword ptr [rsp + 2952], rdx;         jmp   n298_assign_α
n297_var_β:                                                                   jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n298_assign_α:          mov              rsi, qword ptr [rsp + 2944]
                        mov              rdx, qword ptr [rsp + 2952]
                        mov              rdi, qword ptr [rip + .Lx796_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n299_match_begin_α
n298_assign_β:                                                                jmp   FRETURN
.Lx796_0:               .quad            .Lx796_0_s
.Lx796_0_s:             .string          "PATV$1"
#-----------------------------------------------------------------------------------------------------------------------
n299_match_begin_α:     mov              rdi, qword ptr [rsp + 0]
                        mov              rsi, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [rsp + 2896], r13                    # outer_Σ
                        mov              qword ptr [rsp + 2904], r14                    # outer_δ
                        mov              qword ptr [rsp + 2912], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 2920], rax                    # cap_gen
                        mov              qword ptr [rsp + 2888], rsp                    # old____
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
                        mov              qword ptr [rsp + 2864], rsp                    # zls2_mark
                        mov              dword ptr [rsp + 2848], 0                      # start_δ
.Lx798_0:               mov              r14d, dword ptr [rsp + 2848];        jmp   n300_match_pos_α
n299_match_begin_β:     add              dword ptr [rsp + 2848], 1
                        mov              eax, dword ptr [rsp + 2848]
                        cmp              eax, r15d;                           jg    .Lx798_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx798_1
                                                                              jmp   .Lx798_0
.Lx798_1:
n299_match_begin_af:    sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rsp + 2864]
                        mov              r13, qword ptr [rsp + 2896]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2904]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2912]                    # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 2920]                    # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n300_match_pos_α:       mov              rax, 0
                        cmp              r14d, eax;                           jne   n299_match_begin_β
                                                                              jmp   n301_match_defer_α
n300_match_pos_β:                                                             jmp   n299_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n301_match_defer_α:     lea              rdi, [rip + .S3]
                        xor              esi, esi
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_get_pat_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax;                            jz    .Lx800_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx800_4]
                        lea              r11, [rip + .Lx800_5];               jmp   rax
.Lx800_4:                                                                     jmp   n302_match_end_α
.Lx800_5:                                                                     jmp   n299_match_begin_β
.Lx800_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S3]
                        xor              esi, esi
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx800_2:               test             rax, rax;                            je    .Lx800_3
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
                        lea              rcx, [rip + .Lx800_7]
                        lea              rdx, [rip + .Lx800_8];               jmp   rax
.Lx800_7:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        call             rt_defer_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx800_2
.Lx800_8:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        call             rt_defer_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx800_2
.Lx800_3:               add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_close@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             eax, eax;                            js    n299_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx800_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n302_match_end_α
.Lx800_6:               add              rsp, 16;                             jmp   n299_match_begin_β
n301_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n302_match_end_α:       mov              r8, r12
.Lx802_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                            jne   .Lx802_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx802_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                            jne   .Lx802_5
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
.Lx802_1:               test             rax, rax;                            je    .Lx802_2
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
                        lea              rcx, [rip + .Lx802_3]
                        lea              rdx, [rip + .Lx802_4];               jmp   rax
.Lx802_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx802_1
.Lx802_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx802_1
.Lx802_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx802_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                            jne   .Lx802_10
                        mov              r13, qword ptr [rsp + 2896]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 2904]                    # outer_δ
                        mov              r15, qword ptr [rsp + 2912]                    # outer_Δ
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   n303_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n303_statement_end_α:                                                         jmp   RETURN
n303_statement_end_β:                                                         jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n304_statement_begin_α:                                                       jmp   n305_statement_end_α
n304_statement_begin_β:                                                       jmp   n306_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n305_statement_end_α:                                                         jmp   n306_statement_begin_α
n305_statement_end_β:                                                         jmp   n306_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n306_statement_begin_α:                                                       jmp   n307_func_activate_α
n306_statement_begin_β:                                                       jmp   n318_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n307_func_activate_α:   mov              rdi, qword ptr [rip + .Lx812_0]
                        mov              rsi, qword ptr [rip + .Lx812_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_endswith_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n308_statement_end_α
n307_func_activate_β:                                                         jmp   n306_statement_begin_β
.Lx812_0:               .quad            .Lx812_0_s
.Lx812_0_s:             .string          "endswith"
.Lx812_1:               .quad            .Lx812_1_s
.Lx812_1_s:             .string          "s,t"
#-----------------------------------------------------------------------------------------------------------------------
n308_statement_end_α:                                                         jmp   n318_statement_begin_α
n308_statement_end_β:                                                         jmp   n318_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
endswith_body:                                                                jmp   n310_var_α
n309_statement_begin_β:                                                       jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n310_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n311_var_α
n310_var_β:             add              rsp, 16;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n311_var_α:             mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 3104], rax                    # result
                        mov              qword ptr [rsp + 3112], rdx;         jmp   n312_assign_α
n311_var_β:                                                                   jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n312_assign_α:          mov              rsi, qword ptr [rsp + 3104]
                        mov              rdx, qword ptr [rsp + 3112]
                        mov              rdi, qword ptr [rip + .Lx819_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n313_match_begin_α
n312_assign_β:                                                                jmp   FRETURN
.Lx819_0:               .quad            .Lx819_0_s
.Lx819_0_s:             .string          "PATV$2"
#-----------------------------------------------------------------------------------------------------------------------
n313_match_begin_α:     mov              rdi, qword ptr [rsp + 0]
                        mov              rsi, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [rsp + 3056], r13                    # outer_Σ
                        mov              qword ptr [rsp + 3064], r14                    # outer_δ
                        mov              qword ptr [rsp + 3072], r15                    # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 3080], rax                    # cap_gen
                        mov              qword ptr [rsp + 3048], rsp                    # old____
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
                        mov              qword ptr [rsp + 3024], rsp                    # zls2_mark
                        mov              dword ptr [rsp + 3008], 0                      # start_δ
.Lx821_0:               mov              r14d, dword ptr [rsp + 3008];        jmp   n314_match_defer_α
n313_match_begin_β:     add              dword ptr [rsp + 3008], 1
                        mov              eax, dword ptr [rsp + 3008]
                        cmp              eax, r15d;                           jg    .Lx821_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx821_1
                                                                              jmp   .Lx821_0
.Lx821_1:
n313_match_begin_af:    sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rsp + 3024]
                        mov              r13, qword ptr [rsp + 3056]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 3064]                    # outer_δ
                        mov              r15, qword ptr [rsp + 3072]                    # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rsp + 3080]                    # cap_gen
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_match_ctx_restore@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n314_match_defer_α:     lea              rdi, [rip + .S4]
                        xor              esi, esi
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_get_pat_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax;                            jz    .Lx822_0
                        mov              r8d, 0
                        lea              r10, [rip + .Lx822_4]
                        lea              r11, [rip + .Lx822_5];               jmp   rax
.Lx822_4:                                                                     jmp   n315_match_rpos_α
.Lx822_5:                                                                     jmp   n313_match_begin_β
.Lx822_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S4]
                        xor              esi, esi
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_open@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx822_2:               test             rax, rax;                            je    .Lx822_3
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
                        lea              rcx, [rip + .Lx822_7]
                        lea              rdx, [rip + .Lx822_8];               jmp   rax
.Lx822_7:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        call             rt_defer_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx822_2
.Lx822_8:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        call             rt_defer_step@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx822_2
.Lx822_3:               add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_defer_close@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             eax, eax;                            js    n313_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx822_6]
                        sub              rsp, 8
                        push             rax;                                 jmp   n315_match_rpos_α
.Lx822_6:               add              rsp, 16;                             jmp   n313_match_begin_β
n314_match_defer_β:                                                           jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n315_match_rpos_α:      mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx;                           jne   n313_match_begin_β
                                                                              jmp   n316_match_end_α
n315_match_rpos_β:                                                            jmp   n313_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n316_match_end_α:       mov              r8, r12
.Lx825_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                            jne   .Lx825_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx825_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                            jne   .Lx825_5
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
.Lx825_1:               test             rax, rax;                            je    .Lx825_2
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
                        lea              rcx, [rip + .Lx825_3]
                        lea              rdx, [rip + .Lx825_4];               jmp   rax
.Lx825_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx825_1
.Lx825_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx825_1
.Lx825_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx825_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                            jne   .Lx825_10
                        mov              r13, qword ptr [rsp + 3056]                    # outer_Σ
                        mov              r14, qword ptr [rsp + 3064]                    # outer_δ
                        mov              r15, qword ptr [rsp + 3072]                    # outer_Δ
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   n317_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n317_statement_end_α:                                                         jmp   RETURN
n317_statement_end_β:                                                         jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n318_statement_begin_α:                                                       jmp   n319_statement_end_α
n318_statement_begin_β:                                                       jmp   n320_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n319_statement_end_α:                                                         jmp   n320_statement_begin_α
n319_statement_end_β:                                                         jmp   n320_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n320_statement_begin_α:                                                       jmp   n321_func_activate_α
n320_statement_begin_β:                                                       jmp   n348_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n321_func_activate_α:   mov              rdi, qword ptr [rip + .Lx835_0]
                        mov              rsi, qword ptr [rip + .Lx835_1]
                        mov              edx, 3
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_index_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n322_statement_end_α
n321_func_activate_β:                                                         jmp   n320_statement_begin_β
.Lx835_0:               .quad            .Lx835_0_s
.Lx835_0_s:             .string          "index"
.Lx835_1:               .quad            .Lx835_1_s
.Lx835_1_s:             .string          "s,t,ix"
#-----------------------------------------------------------------------------------------------------------------------
n322_statement_end_α:                                                         jmp   n348_statement_begin_α
n322_statement_end_β:                                                         jmp   n348_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
index_body:                                                                   jmp   n324_lit_integer_α
n323_statement_begin_β:                                                       jmp   n327_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n324_lit_integer_α:     mov              qword ptr [rsp + 3152], 3                      # result
                        mov              rax, qword ptr [rip + .Lx840_0]
                        mov              qword ptr [rsp + 3160], rax;         jmp   n325_assign_α
n324_lit_integer_β:                                                           jmp   n323_statement_begin_β
.Lx840_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n325_assign_α:          mov              rax, qword ptr [rsp + 3152]
                        mov              rdx, qword ptr [rsp + 3160]
                        mov              qword ptr [r9 + 272], rax                      # index
                        mov              qword ptr [r9 + 280], rdx;           jmp   n326_statement_end_α
n325_assign_β:                                                                jmp   n323_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n326_statement_end_α:                                                         jmp   n327_statement_begin_α
n326_statement_end_β:                                                         jmp   n327_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n327_statement_begin_α:                                                       jmp   n328_var_α
n327_statement_begin_β:                                                       jmp   n331_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n328_var_α:             mov              rax, qword ptr [r9 + 16]                       # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 3168], rax                    # result
                        mov              qword ptr [rsp + 3176], rdx;         jmp   n329_assign_α
n328_var_β:                                                                   jmp   n327_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n329_assign_α:          mov              rax, qword ptr [rsp + 3168]
                        mov              rdx, qword ptr [rsp + 3176]
                        mov              qword ptr [r9 + 288], rax                      # ix
                        mov              qword ptr [r9 + 296], rdx;           jmp   n330_statement_end_α
n329_assign_β:                                                                jmp   n327_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n330_statement_end_α:                                                         jmp   n331_statement_begin_α
n330_statement_end_β:                                                         jmp   n331_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n331_statement_begin_α:                                                       jmp   n332_var_α
n331_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n332_var_α:             mov              rax, qword ptr [r9 + 288]                      # ix
                        mov              rdx, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 3360], rax                    # result
                        mov              qword ptr [rsp + 3368], rdx;         jmp   n333_var_α
n332_var_β:                                                                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n333_var_α:             mov              rax, qword ptr [r9 + 224]                      # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 3344], rax                    # result
                        mov              qword ptr [rsp + 3352], rdx;         jmp   n334_coerce_string_α
n333_var_β:                                                                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n334_coerce_string_α:   lea              rdi, [rsp + 3344]
                        lea              rsi, [rsp + 3328]
                        mov              rdx, 4522053
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_str_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n335_match_begin_α
n334_coerce_string_β:                                                         jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n335_match_begin_α:     mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              qword ptr [rsp + 40], rsp                      # old____
                        mov              rdi, qword ptr [rsp + 3360]
                        mov              rsi, qword ptr [rsp + 3368]
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
.Lx857_0:               mov              r14d, dword ptr [rsp + 0];           jmp   n336_match_assign_save_α
n335_match_begin_β:     mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d;                           jg    .Lx857_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx857_1
                                                                              jmp   .Lx857_0
.Lx857_1:
n335_match_begin_af:    sub              r12, 24                                        # cas_mark
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n336_match_assign_save_α: sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n337_match_break_α
n336_match_assign_save_β: add              rsp, 16;                           jmp   n335_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n337_match_break_α:     sub              rsp, 16
                        mov              dword ptr [rsp + 0], 0
                        mov              r8, qword ptr [rsp + 3336]
                        mov              r9d, dword ptr [rsp + 3332]
.Lx861_0:               mov              eax, r14d
                        add              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d;                           jl    .Lx861_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n335_match_begin_β
.Lx861_240:             movsxd           rcx, eax
                        movzx            esi, byte ptr [r13+rcx]
                        mov              edx, 0
.Lx861_2:               cmp              edx, r9d;                            jge   .Lx861_3
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi;                            je    .Lx861_1
                        add              edx, 1;                              jmp   .Lx861_2
.Lx861_3:               add              dword ptr [rsp + 0], 1;              jmp   .Lx861_0
.Lx861_1:               mov              eax, r14d
                        add              eax, dword ptr [rsp + 0]
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, eax;                           jmp   n338_match_assign_cond_α
n337_match_break_β:     mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n335_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n338_match_assign_cond_α: mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S5]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n339_match_end_α
n338_match_assign_cond_β: sub              r12, 24;                           jmp   n337_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n339_match_end_α:       mov              r8, r12
.Lx865_9:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                            jne   .Lx865_9
                        mov              rsp, qword ptr [r8 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r8, rsi
.Lx865_5:               sub              r8, 24
                        mov              rax, qword ptr [r8 + 0]
                        test             rax, rax;                            jne   .Lx865_5
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
.Lx865_1:               test             rax, rax;                            je    .Lx865_2
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
                        lea              rcx, [rip + .Lx865_3]
                        lea              rdx, [rip + .Lx865_4];               jmp   rax
.Lx865_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx865_1
.Lx865_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   .Lx865_1
.Lx865_2:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
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
.Lx865_10:              sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax;                            jne   .Lx865_10
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
                        mov              r11,  qword ptr [r11 + 64];          jmp   n340_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n340_statement_end_α:                                                         jmp   n341_statement_begin_α
n340_statement_end_β:                                                         jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n341_statement_begin_α:                                                       jmp   n342_var_α
n341_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n342_var_α:             mov              rax, qword ptr [r9 + 288]                      # ix
                        mov              rdx, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 3440], rax                    # result
                        mov              qword ptr [rsp + 3448], rdx;         jmp   n343_call_α
n342_var_β:                                                                   jmp   n341_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n343_call_α:            mov              rax, qword ptr [rsp + 3440]
                        mov              qword ptr [rsp + 3408], rax
                        mov              rax, qword ptr [rsp + 3448]
                        mov              qword ptr [rsp + 3416], rax
                        .section         .rodata
.Lrkfn872:              .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn872]
                        lea              rsi, [rsp + 3408]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 3392], rax
                        mov              qword ptr [rsp + 3400], rdx
                        cmp              eax, 104;                            je    n341_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n344_lit_integer_α
n343_call_β:                                                                  jmp   n341_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n344_lit_integer_α:     mov              qword ptr [rsp + 3456], 3                      # result
                        mov              rax, qword ptr [rip + .Lx873_0]
                        mov              qword ptr [rsp + 3464], rax;         jmp   n345_binop_α
n344_lit_integer_β:                                                           jmp   n341_statement_begin_β
.Lx873_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n345_binop_α:           mov              eax, dword ptr [rsp + 3392]
                        cmp              eax, 3;                              jne   .Lx874_0
                        mov              rax, qword ptr [rsp + 3400]
                        mov              rcx, 1
                        add              rax, rcx
                        mov              qword ptr [rsp + 3376], 3
                        mov              qword ptr [rsp + 3384], rax;         jmp   n346_assign_α
.Lx874_0:               mov              rdi, qword ptr [rsp + 3392]
                        mov              rsi, qword ptr [rsp + 3400]
                        mov              rdx, qword ptr [rsp + 3456]
                        mov              rcx, qword ptr [rsp + 3464]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            je    n341_statement_begin_β
                        mov              qword ptr [rsp + 3376], rax
                        mov              qword ptr [rsp + 3384], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n346_assign_α
n345_binop_β:                                                                 jmp   n341_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n346_assign_α:          mov              rax, qword ptr [rsp + 3376]
                        mov              rdx, qword ptr [rsp + 3384]
                        mov              qword ptr [r9 + 272], rax                      # index
                        mov              qword ptr [r9 + 280], rdx;           jmp   n347_statement_end_α
n346_assign_β:                                                                jmp   n341_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n347_statement_end_α:                                                         jmp   RETURN
n347_statement_end_β:                                                         jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n348_statement_begin_α:                                                       jmp   n349_statement_end_α
n348_statement_begin_β:                                                       jmp   n350_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n349_statement_end_α:                                                         jmp   n350_statement_begin_α
n349_statement_end_β:                                                         jmp   n350_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n350_statement_begin_α:                                                       jmp   n351_statement_end_α
n350_statement_begin_β:                                                       jmp   n352_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n351_statement_end_α:                                                         jmp   n352_statement_begin_α
n351_statement_end_β:                                                         jmp   n352_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n352_statement_begin_α:                                                       jmp   n353_lit_string_α
n352_statement_begin_β:                                                       jmp   n357_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n353_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx888_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n354_lit_integer_α
n353_lit_string_β:      add              rsp, 16;                             jmp   n352_statement_begin_β
.Lx888_0:               .quad            .Lx888_0_s
.Lx888_0_s:             .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n354_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx889_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n355_call_α
n354_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n352_statement_begin_β
.Lx889_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n355_call_α:            sub              rsp, 16
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
.Lrkfnzd891:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd891]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104;                            jne   .Lx890_240
                        add              rsp, 16;                             jmp   n354_lit_integer_β
.Lx890_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n356_statement_end_α
n355_call_β:            add              rsp, 16;                             jmp   n354_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n356_statement_end_α:                                                         jmp   n357_statement_begin_α
n356_statement_end_β:                                                         jmp   n357_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n357_statement_begin_α:                                                       jmp   n358_statement_end_α
n357_statement_begin_β:                                                       jmp   n359_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n358_statement_end_α:                                                         jmp   n359_statement_begin_α
n358_statement_end_β:                                                         jmp   n359_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n359_statement_begin_α:                                                       jmp   n360_lit_string_α
n359_statement_begin_β:                                                       jmp   n366_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n360_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx900_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n361_lit_integer_α
n360_lit_string_β:      add              rsp, 16
                        add              rsp, 48;                             jmp   n359_statement_begin_β
.Lx900_0:               .quad            .Lx900_0_s
.Lx900_0_s:             .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n361_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx901_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n362_lit_string_α
n361_lit_integer_β:     add              rsp, 16;                             jmp   n360_lit_string_β
.Lx901_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n362_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx902_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n363_call_α
n362_lit_string_β:      add              rsp, 16;                             jmp   n361_lit_integer_β
.Lx902_0:               .quad            .Lx902_0_s
.Lx902_0_s:             .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n363_call_α:            sub              rsp, 16
                        sub              rsp, 80
                        mov              rax, qword ptr [rsp + 128]                     # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 112]                     # lit_integer
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 72], rax
                        mov              eax, 3
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx904_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + pad_left_alpha];         jmp   rax
.Lx904_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx904_240
                        add              rsp, 16;                             jmp   n362_lit_string_β
.Lx904_240:                                                                   jmp   n364_assign_α
n363_call_β:                                                                  jmp   n362_lit_string_β
.Lx904_0:               .quad            .Lx904_0_s
.Lx904_0_s:             .string          "pad_left"
#-----------------------------------------------------------------------------------------------------------------------
n364_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx905_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n365_statement_end_α
n364_assign_β:                                                                jmp   n359_statement_begin_β
.Lx905_0:               .quad            .Lx905_0_s
.Lx905_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n365_statement_end_α:                                                         jmp   n366_statement_begin_α
n365_statement_end_β:                                                         jmp   n366_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n366_statement_begin_α:                                                       jmp   n367_lit_string_α
n366_statement_begin_β:                                                       jmp   n373_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n367_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx910_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n368_lit_integer_α
n367_lit_string_β:      add              rsp, 16
                        add              rsp, 112;                            jmp   n366_statement_begin_β
.Lx910_0:               .quad            .Lx910_0_s
.Lx910_0_s:             .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n368_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx911_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n369_lit_string_α
n368_lit_integer_β:     add              rsp, 16;                             jmp   n367_lit_string_β
.Lx911_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n369_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx912_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n370_call_α
n369_lit_string_β:      add              rsp, 16;                             jmp   n368_lit_integer_β
.Lx912_0:               .quad            .Lx912_0_s
.Lx912_0_s:             .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n370_call_α:            sub              rsp, 16
                        sub              rsp, 80
                        mov              rax, qword ptr [rsp + 128]                     # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 112]                     # lit_integer
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 72], rax
                        mov              eax, 3
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx914_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + pad_right_alpha];        jmp   rax
.Lx914_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx914_240
                        add              rsp, 16;                             jmp   n369_lit_string_β
.Lx914_240:                                                                   jmp   n371_assign_α
n370_call_β:                                                                  jmp   n369_lit_string_β
.Lx914_0:               .quad            .Lx914_0_s
.Lx914_0_s:             .string          "pad_right"
#-----------------------------------------------------------------------------------------------------------------------
n371_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx915_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n372_statement_end_α
n371_assign_β:                                                                jmp   n366_statement_begin_β
.Lx915_0:               .quad            .Lx915_0_s
.Lx915_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n372_statement_end_α:                                                         jmp   n373_statement_begin_α
n372_statement_end_β:                                                         jmp   n373_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n373_statement_begin_α:                                                       jmp   n374_lit_string_α
n373_statement_begin_β:                                                       jmp   n378_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n374_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx920_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n375_call_α
n374_lit_string_β:      add              rsp, 16
                        add              rsp, 176;                            jmp   n373_statement_begin_β
.Lx920_0:               .quad            .Lx920_0_s
.Lx920_0_s:             .string          "   hello"
#-----------------------------------------------------------------------------------------------------------------------
n375_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx922_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + ltrim_alpha];            jmp   rax
.Lx922_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx922_240
                        add              rsp, 16;                             jmp   n374_lit_string_β
.Lx922_240:                                                                   jmp   n376_assign_α
n375_call_β:                                                                  jmp   n374_lit_string_β
.Lx922_0:               .quad            .Lx922_0_s
.Lx922_0_s:             .string          "ltrim"
#-----------------------------------------------------------------------------------------------------------------------
n376_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx923_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n377_statement_end_α
n376_assign_β:                                                                jmp   n373_statement_begin_β
.Lx923_0:               .quad            .Lx923_0_s
.Lx923_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n377_statement_end_α:                                                         jmp   n378_statement_begin_α
n377_statement_end_β:                                                         jmp   n378_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n378_statement_begin_α:                                                       jmp   n379_lit_string_α
n378_statement_begin_β:                                                       jmp   n383_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n379_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx928_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n380_call_α
n379_lit_string_β:      add              rsp, 16
                        add              rsp, 208;                            jmp   n378_statement_begin_β
.Lx928_0:               .quad            .Lx928_0_s
.Lx928_0_s:             .string          "hello   "
#-----------------------------------------------------------------------------------------------------------------------
n380_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx930_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + rtrim_alpha];            jmp   rax
.Lx930_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx930_240
                        add              rsp, 16;                             jmp   n379_lit_string_β
.Lx930_240:                                                                   jmp   n381_assign_α
n380_call_β:                                                                  jmp   n379_lit_string_β
.Lx930_0:               .quad            .Lx930_0_s
.Lx930_0_s:             .string          "rtrim"
#-----------------------------------------------------------------------------------------------------------------------
n381_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx931_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n382_statement_end_α
n381_assign_β:                                                                jmp   n378_statement_begin_β
.Lx931_0:               .quad            .Lx931_0_s
.Lx931_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n382_statement_end_α:                                                         jmp   n383_statement_begin_α
n382_statement_end_β:                                                         jmp   n383_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n383_statement_begin_α:                                                       jmp   n384_lit_string_α
n383_statement_begin_β:                                                       jmp   n388_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n384_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 9
                        mov              rax, qword ptr [rip + .Lx936_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n385_call_α
n384_lit_string_β:      add              rsp, 16
                        add              rsp, 240;                            jmp   n383_statement_begin_β
.Lx936_0:               .quad            .Lx936_0_s
.Lx936_0_s:             .string          "  hello  "
#-----------------------------------------------------------------------------------------------------------------------
n385_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx938_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + trimws_alpha];           jmp   rax
.Lx938_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx938_240
                        add              rsp, 16;                             jmp   n384_lit_string_β
.Lx938_240:                                                                   jmp   n386_assign_α
n385_call_β:                                                                  jmp   n384_lit_string_β
.Lx938_0:               .quad            .Lx938_0_s
.Lx938_0_s:             .string          "trimws"
#-----------------------------------------------------------------------------------------------------------------------
n386_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx939_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n387_statement_end_α
n386_assign_β:                                                                jmp   n383_statement_begin_β
.Lx939_0:               .quad            .Lx939_0_s
.Lx939_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n387_statement_end_α:                                                         jmp   n388_statement_begin_α
n387_statement_end_β:                                                         jmp   n388_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n388_statement_begin_α:                                                       jmp   n389_lit_string_α
n388_statement_begin_β:                                                       jmp   n394_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n389_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx944_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n390_lit_integer_α
n389_lit_string_β:      add              rsp, 16
                        add              rsp, 272;                            jmp   n388_statement_begin_β
.Lx944_0:               .quad            .Lx944_0_s
.Lx944_0_s:             .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n390_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx945_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n391_call_α
n390_lit_integer_β:     add              rsp, 16;                             jmp   n389_lit_string_β
.Lx945_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n391_call_α:            sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]                      # lit_integer
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx947_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + repeat_alpha];           jmp   rax
.Lx947_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx947_240
                        add              rsp, 16;                             jmp   n390_lit_integer_β
.Lx947_240:                                                                   jmp   n392_assign_α
n391_call_β:                                                                  jmp   n390_lit_integer_β
.Lx947_0:               .quad            .Lx947_0_s
.Lx947_0_s:             .string          "repeat"
#-----------------------------------------------------------------------------------------------------------------------
n392_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx948_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n393_statement_end_α
n392_assign_β:                                                                jmp   n388_statement_begin_β
.Lx948_0:               .quad            .Lx948_0_s
.Lx948_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n393_statement_end_α:                                                         jmp   n394_statement_begin_α
n393_statement_end_β:                                                         jmp   n394_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n394_statement_begin_α:                                                       jmp   n395_statement_end_α
n394_statement_begin_β:                                                       jmp   n396_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n395_statement_end_α:                                                         jmp   n396_statement_begin_α
n395_statement_end_β:                                                         jmp   n396_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n396_statement_begin_α:                                                       jmp   n397_lit_string_α
n396_statement_begin_β: add              rsp, 320;                            jmp   n405_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n397_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx957_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n398_lit_string_α
n397_lit_string_β:      add              rsp, 16
                        add              rsp, 320;                            jmp   n396_statement_begin_β
.Lx957_0:               .quad            .Lx957_0_s
.Lx957_0_s:             .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n398_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx958_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n399_call_α
n398_lit_string_β:      add              rsp, 16;                             jmp   n397_lit_string_β
.Lx958_0:               .quad            .Lx958_0_s
.Lx958_0_s:             .string          "oba"
#-----------------------------------------------------------------------------------------------------------------------
n399_call_α:            sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx960_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + contains_alpha];         jmp   rax
.Lx960_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx960_240
                        add              rsp, 16;                             jmp   n398_lit_string_β
.Lx960_240:                                                                   jmp   n400_statement_end_α
n399_call_β:                                                                  jmp   n398_lit_string_β
.Lx960_0:               .quad            .Lx960_0_s
.Lx960_0_s:             .string          "contains"
#-----------------------------------------------------------------------------------------------------------------------
n400_statement_end_α:                                                         jmp   n401_statement_begin_α
n400_statement_end_β:   add              rsp, 368;                            jmp   n405_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n401_statement_begin_α:                                                       jmp   n402_lit_string_α
n401_statement_begin_β:                                                       jmp   n409_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n402_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx965_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n403_assign_α
n402_lit_string_β:      add              rsp, 16
                        add              rsp, 368;                            jmp   n401_statement_begin_β
.Lx965_0:               .quad            .Lx965_0_s
.Lx965_0_s:             .string          "contains ok"
#-----------------------------------------------------------------------------------------------------------------------
n403_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx966_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n404_statement_end_α
n403_assign_β:                                                                jmp   n401_statement_begin_β
.Lx966_0:               .quad            .Lx966_0_s
.Lx966_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n404_statement_end_α:                                                         jmp   n409_statement_begin_α
n404_statement_end_β:                                                         jmp   n409_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n405_statement_begin_α:                                                       jmp   n406_lit_string_α
n405_statement_begin_β:                                                       jmp   n409_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n406_lit_string_α:      mov              qword ptr [rsp + 4224], 2                      # result
                        mov              dword ptr [rsp + 4228], 14
                        mov              rax, qword ptr [rip + .Lx971_0]
                        mov              qword ptr [rsp + 4232], rax;         jmp   n407_assign_α
n406_lit_string_β:                                                            jmp   n405_statement_begin_β
.Lx971_0:               .quad            .Lx971_0_s
.Lx971_0_s:             .string          "FAIL: contains"
#-----------------------------------------------------------------------------------------------------------------------
n407_assign_α:          mov              rsi, qword ptr [rsp + 4224]
                        mov              rdx, qword ptr [rsp + 4232]
                        mov              rdi, qword ptr [rip + .Lx972_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n408_statement_end_α
n407_assign_β:                                                                jmp   n405_statement_begin_β
.Lx972_0:               .quad            .Lx972_0_s
.Lx972_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n408_statement_end_α:                                                         jmp   n409_statement_begin_α
n408_statement_end_β:                                                         jmp   n409_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n409_statement_begin_α:                                                       jmp   n410_statement_end_α
n409_statement_begin_β:                                                       jmp   n411_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n410_statement_end_α:                                                         jmp   n411_statement_begin_α
n410_statement_end_β:                                                         jmp   n411_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n411_statement_begin_α:                                                       jmp   n412_lit_string_α
n411_statement_begin_β: add              rsp, 384;                            jmp   n420_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n412_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx981_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n413_lit_string_α
n412_lit_string_β:      add              rsp, 16
                        add              rsp, 384;                            jmp   n411_statement_begin_β
.Lx981_0:               .quad            .Lx981_0_s
.Lx981_0_s:             .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n413_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx982_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n414_call_α
n413_lit_string_β:      add              rsp, 16;                             jmp   n412_lit_string_β
.Lx982_0:               .quad            .Lx982_0_s
.Lx982_0_s:             .string          "foo"
#-----------------------------------------------------------------------------------------------------------------------
n414_call_α:            sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx984_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + startswith_alpha];       jmp   rax
.Lx984_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx984_240
                        add              rsp, 16;                             jmp   n413_lit_string_β
.Lx984_240:                                                                   jmp   n415_statement_end_α
n414_call_β:                                                                  jmp   n413_lit_string_β
.Lx984_0:               .quad            .Lx984_0_s
.Lx984_0_s:             .string          "startswith"
#-----------------------------------------------------------------------------------------------------------------------
n415_statement_end_α:                                                         jmp   n416_statement_begin_α
n415_statement_end_β:   add              rsp, 432;                            jmp   n420_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n416_statement_begin_α:                                                       jmp   n417_lit_string_α
n416_statement_begin_β:                                                       jmp   n424_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n417_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 13
                        mov              rax, qword ptr [rip + .Lx989_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n418_assign_α
n417_lit_string_β:      add              rsp, 16
                        add              rsp, 432;                            jmp   n416_statement_begin_β
.Lx989_0:               .quad            .Lx989_0_s
.Lx989_0_s:             .string          "startswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n418_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx990_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n419_statement_end_α
n418_assign_β:                                                                jmp   n416_statement_begin_β
.Lx990_0:               .quad            .Lx990_0_s
.Lx990_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n419_statement_end_α:                                                         jmp   n424_statement_begin_α
n419_statement_end_β:                                                         jmp   n424_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n420_statement_begin_α:                                                       jmp   n421_lit_string_α
n420_statement_begin_β:                                                       jmp   n424_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n421_lit_string_α:      mov              qword ptr [rsp + 4352], 2                      # result
                        mov              dword ptr [rsp + 4356], 16
                        mov              rax, qword ptr [rip + .Lx995_0]
                        mov              qword ptr [rsp + 4360], rax;         jmp   n422_assign_α
n421_lit_string_β:                                                            jmp   n420_statement_begin_β
.Lx995_0:               .quad            .Lx995_0_s
.Lx995_0_s:             .string          "FAIL: startswith"
#-----------------------------------------------------------------------------------------------------------------------
n422_assign_α:          mov              rsi, qword ptr [rsp + 4352]
                        mov              rdx, qword ptr [rsp + 4360]
                        mov              rdi, qword ptr [rip + .Lx996_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n423_statement_end_α
n422_assign_β:                                                                jmp   n420_statement_begin_β
.Lx996_0:               .quad            .Lx996_0_s
.Lx996_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n423_statement_end_α:                                                         jmp   n424_statement_begin_α
n423_statement_end_β:                                                         jmp   n424_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n424_statement_begin_α:                                                       jmp   n425_statement_end_α
n424_statement_begin_β:                                                       jmp   n426_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n425_statement_end_α:                                                         jmp   n426_statement_begin_α
n425_statement_end_β:                                                         jmp   n426_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n426_statement_begin_α:                                                       jmp   n427_lit_string_α
n426_statement_begin_β: add              rsp, 448;                            jmp   n435_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n427_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1005_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n428_lit_string_α
n427_lit_string_β:      add              rsp, 16
                        add              rsp, 448;                            jmp   n426_statement_begin_β
.Lx1005_0:              .quad            .Lx1005_0_s
.Lx1005_0_s:            .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n428_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1006_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n429_call_α
n428_lit_string_β:      add              rsp, 16;                             jmp   n427_lit_string_β
.Lx1006_0:              .quad            .Lx1006_0_s
.Lx1006_0_s:            .string          "bar"
#-----------------------------------------------------------------------------------------------------------------------
n429_call_α:            sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx1008_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + endswith_alpha];         jmp   rax
.Lx1008_2:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx1008_240
                        add              rsp, 16;                             jmp   n428_lit_string_β
.Lx1008_240:                                                                  jmp   n430_statement_end_α
n429_call_β:                                                                  jmp   n428_lit_string_β
.Lx1008_0:              .quad            .Lx1008_0_s
.Lx1008_0_s:            .string          "endswith"
#-----------------------------------------------------------------------------------------------------------------------
n430_statement_end_α:                                                         jmp   n431_statement_begin_α
n430_statement_end_β:   add              rsp, 496;                            jmp   n435_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n431_statement_begin_α:                                                       jmp   n432_lit_string_α
n431_statement_begin_β:                                                       jmp   n439_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n432_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx1013_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n433_assign_α
n432_lit_string_β:      add              rsp, 16
                        add              rsp, 496;                            jmp   n431_statement_begin_β
.Lx1013_0:              .quad            .Lx1013_0_s
.Lx1013_0_s:            .string          "endswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n433_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1014_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n434_statement_end_α
n433_assign_β:                                                                jmp   n431_statement_begin_β
.Lx1014_0:              .quad            .Lx1014_0_s
.Lx1014_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n434_statement_end_α:                                                         jmp   n439_statement_begin_α
n434_statement_end_β:                                                         jmp   n439_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n435_statement_begin_α:                                                       jmp   n436_lit_string_α
n435_statement_begin_β:                                                       jmp   n439_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n436_lit_string_α:      mov              qword ptr [rsp + 4480], 2                      # result
                        mov              dword ptr [rsp + 4484], 14
                        mov              rax, qword ptr [rip + .Lx1019_0]
                        mov              qword ptr [rsp + 4488], rax;         jmp   n437_assign_α
n436_lit_string_β:                                                            jmp   n435_statement_begin_β
.Lx1019_0:              .quad            .Lx1019_0_s
.Lx1019_0_s:            .string          "FAIL: endswith"
#-----------------------------------------------------------------------------------------------------------------------
n437_assign_α:          mov              rsi, qword ptr [rsp + 4480]
                        mov              rdx, qword ptr [rsp + 4488]
                        mov              rdi, qword ptr [rip + .Lx1020_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n438_statement_end_α
n437_assign_β:                                                                jmp   n435_statement_begin_β
.Lx1020_0:              .quad            .Lx1020_0_s
.Lx1020_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n438_statement_end_α:                                                         jmp   n439_statement_begin_α
n438_statement_end_β:                                                         jmp   n439_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n439_statement_begin_α:                                                       jmp   n440_statement_end_α
n439_statement_begin_β:                                                       jmp   n441_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n440_statement_end_α:                                                         jmp   n441_statement_begin_α
n440_statement_end_β:                                                         jmp   n441_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n441_statement_begin_α:                                                       jmp   n442_lit_string_α
n441_statement_begin_β: add              rsp, 512;                            jmp   n446_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n442_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1029_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n443_lit_string_α
n442_lit_string_β:      add              rsp, 16
                        add              rsp, 512;                            jmp   n441_statement_begin_β
.Lx1029_0:              .quad            .Lx1029_0_s
.Lx1029_0_s:            .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n443_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1030_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n444_call_α
n443_lit_string_β:      add              rsp, 16;                             jmp   n442_lit_string_β
.Lx1030_0:              .quad            .Lx1030_0_s
.Lx1030_0_s:            .string          "bar"
#-----------------------------------------------------------------------------------------------------------------------
n444_call_α:            sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx1032_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + startswith_alpha];       jmp   rax
.Lx1032_2:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx1032_240
                        add              rsp, 16;                             jmp   n443_lit_string_β
.Lx1032_240:                                                                  jmp   n445_statement_end_α
n444_call_β:                                                                  jmp   n443_lit_string_β
.Lx1032_0:              .quad            .Lx1032_0_s
.Lx1032_0_s:            .string          "startswith"
#-----------------------------------------------------------------------------------------------------------------------
n445_statement_end_α:                                                         jmp   n450_statement_begin_α
n445_statement_end_β:   add              rsp, 560;                            jmp   n446_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n446_statement_begin_α:                                                       jmp   n447_lit_string_α
n446_statement_begin_β:                                                       jmp   n454_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n447_lit_string_α:      mov              qword ptr [rsp + 4592], 2                      # result
                        mov              dword ptr [rsp + 4596], 16
                        mov              rax, qword ptr [rip + .Lx1037_0]
                        mov              qword ptr [rsp + 4600], rax;         jmp   n448_assign_α
n447_lit_string_β:                                                            jmp   n446_statement_begin_β
.Lx1037_0:              .quad            .Lx1037_0_s
.Lx1037_0_s:            .string          "no startswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n448_assign_α:          mov              rsi, qword ptr [rsp + 4592]
                        mov              rdx, qword ptr [rsp + 4600]
                        mov              rdi, qword ptr [rip + .Lx1038_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n449_statement_end_α
n448_assign_β:                                                                jmp   n446_statement_begin_β
.Lx1038_0:              .quad            .Lx1038_0_s
.Lx1038_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n449_statement_end_α:                                                         jmp   n454_statement_begin_α
n449_statement_end_β:                                                         jmp   n454_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n450_statement_begin_α:                                                       jmp   n451_lit_string_α
n450_statement_begin_β:                                                       jmp   n454_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n451_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx1043_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n452_assign_α
n451_lit_string_β:      add              rsp, 16
                        add              rsp, 560;                            jmp   n450_statement_begin_β
.Lx1043_0:              .quad            .Lx1043_0_s
.Lx1043_0_s:            .string          "FAIL: startswith matched wrong"
#-----------------------------------------------------------------------------------------------------------------------
n452_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1044_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n453_statement_end_α
n452_assign_β:                                                                jmp   n450_statement_begin_β
.Lx1044_0:              .quad            .Lx1044_0_s
.Lx1044_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n453_statement_end_α:                                                         jmp   n454_statement_begin_α
n453_statement_end_β:                                                         jmp   n454_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n454_statement_begin_α:                                                       jmp   n455_statement_end_α
n454_statement_begin_β:                                                       jmp   n456_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n455_statement_end_α:                                                         jmp   n456_statement_begin_α
n455_statement_end_β:                                                         jmp   n456_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n456_statement_begin_α:                                                       jmp   n457_lit_string_α
n456_statement_begin_β:                                                       jmp   n462_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n457_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1053_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n458_lit_string_α
n457_lit_string_β:      add              rsp, 16
                        add              rsp, 576;                            jmp   n456_statement_begin_β
.Lx1053_0:              .quad            .Lx1053_0_s
.Lx1053_0_s:            .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n458_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1054_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n459_call_α
n458_lit_string_β:      add              rsp, 16;                             jmp   n457_lit_string_β
.Lx1054_0:              .quad            .Lx1054_0_s
.Lx1054_0_s:            .string          "oba"
#-----------------------------------------------------------------------------------------------------------------------
n459_call_α:            sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx1056_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + index_alpha];            jmp   rax
.Lx1056_2:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx1056_240
                        add              rsp, 16;                             jmp   n458_lit_string_β
.Lx1056_240:                                                                  jmp   n460_assign_α
n459_call_β:                                                                  jmp   n458_lit_string_β
.Lx1056_0:              .quad            .Lx1056_0_s
.Lx1056_0_s:            .string          "index"
#-----------------------------------------------------------------------------------------------------------------------
n460_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1057_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n461_statement_end_α
n460_assign_β:                                                                jmp   n456_statement_begin_β
.Lx1057_0:              .quad            .Lx1057_0_s
.Lx1057_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n461_statement_end_α:                                                         jmp   n462_statement_begin_α
n461_statement_end_β:                                                         jmp   n462_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n462_statement_begin_α:                                                       jmp   n463_lit_string_α
n462_statement_begin_β: add              rsp, 624;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n463_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx1062_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n464_lit_string_α
n463_lit_string_β:      add              rsp, 16
                        add              rsp, 624;                            jmp   n462_statement_begin_β
.Lx1062_0:              .quad            .Lx1062_0_s
.Lx1062_0_s:            .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n464_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx1063_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n465_call_α
n464_lit_string_β:      add              rsp, 16;                             jmp   n463_lit_string_β
.Lx1063_0:              .quad            .Lx1063_0_s
.Lx1063_0_s:            .string          "xyz"
#-----------------------------------------------------------------------------------------------------------------------
n465_call_α:            sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx1065_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + index_alpha];            jmp   rax
.Lx1065_2:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx1065_240
                        add              rsp, 16;                             jmp   n464_lit_string_β
.Lx1065_240:                                                                  jmp   n466_assign_α
n465_call_β:                                                                  jmp   n464_lit_string_β
.Lx1065_0:              .quad            .Lx1065_0_s
.Lx1065_0_s:            .string          "index"
#-----------------------------------------------------------------------------------------------------------------------
n466_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx1066_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64];          jmp   n467_statement_end_α
n466_assign_β:                                                                jmp   n462_statement_begin_β
.Lx1066_0:              .quad            .Lx1066_0_s
.Lx1066_0_s:            .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n467_statement_end_α:   add              rsp, 672;                            jmp   main_γ
n467_statement_end_β:   add              rsp, 672;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 lea              rdi, [rip + .S6]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                lea              rdi, [rip + .S7]
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
.S0:                    .string          "r"
.S1:                    .string          "ws"
.S2:                    .string          "PATV$0"
.S3:                    .string          "PATV$1"
.S4:                    .string          "PATV$2"
.S5:                    .string          "ix"
.S6:                    .string          "BOMB-RETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
.S7:                    .string          "BOMB-FRETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
                        .text
                        .section         .note.GNU-stack,"",@progbits
