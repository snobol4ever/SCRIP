                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_max_α
proc_max_α:
#-----------------------------------------------------------------------------------------------------------------------
n0_save_restore_α:                                                  jmp   n1_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_deferred_α:     lea              rax, [rip + max_body];     jmp   rax
                                                                    jmp   n2_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n2_save_restore_α:
max_alpha:              mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx8_2
                        mov              eax, 32
.Lx8_2:                 sub              rsp, 64
                        add              rsp, rax
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        cmp              rcx, 0;                    jbe   .Lx8_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 0]                        # max
                        mov              qword ptr [r9 + 0], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              qword ptr [r8 + 72], rdx;  jmp   .Lx8_41
.Lx8_10:                mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
.Lx8_41:                cmp              rcx, 1;                    jbe   .Lx8_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 16]                       # x
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx8_42
.Lx8_11:                mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx8_42:                lea              r10, [rip + max_gamma]
                        lea              r11, [rip + max_omega]
                        lea              rax, [rip + max_body];     jmp   rax
max_gamma:              mov              rdi, qword ptr [r9 + 0]                        # max
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx8_75
                        mov              eax, 32
.Lx8_75:                lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx8_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 8], rax;   jmp   .Lx8_110
.Lx8_80:                mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
.Lx8_110:               cmp              rcx, 1;                    jbe   .Lx8_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # x
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx8_111
.Lx8_81:                mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 24], rax
.Lx8_111:               mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 96
                        cmp              rcx, 2;                    jbe   .Lx8_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx8_3:                 mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
max_omega:              mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx8_145
                        mov              eax, 32
.Lx8_145:               lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx8_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 0], rax                        # max
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 8], rax;   jmp   .Lx8_180
.Lx8_150:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
.Lx8_180:               cmp              rcx, 1;                    jbe   .Lx8_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # x
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx8_181
.Lx8_151:               mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 24], rax
.Lx8_181:               mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 96
                        cmp              rcx, 2;                    jbe   .Lx8_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx8_4:                 mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_min_α
proc_min_α:
#-----------------------------------------------------------------------------------------------------------------------
n9_save_restore_α:                                                  jmp   n10_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n10_goto_deferred_α:    lea              rax, [rip + min_body];     jmp   rax
                                                                    jmp   n11_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n11_save_restore_α:
min_alpha:              mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx17_2
                        mov              eax, 32
.Lx17_2:                sub              rsp, 64
                        add              rsp, rax
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        cmp              rcx, 0;                    jbe   .Lx17_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 32]                       # min
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 72], rdx;  jmp   .Lx17_41
.Lx17_10:               mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx17_41:               cmp              rcx, 1;                    jbe   .Lx17_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 16]                       # x
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx17_42
.Lx17_11:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx17_42:               lea              r10, [rip + min_gamma]
                        lea              r11, [rip + min_omega]
                        lea              rax, [rip + min_body];     jmp   rax
min_gamma:              mov              rdi, qword ptr [r9 + 32]                       # min
                        mov              rsi, qword ptr [r9 + 40]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx17_75
                        mov              eax, 32
.Lx17_75:               lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx17_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx17_110
.Lx17_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 40], rax
.Lx17_110:              cmp              rcx, 1;                    jbe   .Lx17_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # x
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx17_111
.Lx17_81:               mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 24], rax
.Lx17_111:              mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 96
                        cmp              rcx, 2;                    jbe   .Lx17_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx17_3:                mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
min_omega:              mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx17_145
                        mov              eax, 32
.Lx17_145:              lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx17_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 32], rax                       # min
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx17_180
.Lx17_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 40], rax
.Lx17_180:              cmp              rcx, 1;                    jbe   .Lx17_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 16], rax                       # x
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx17_181
.Lx17_151:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 24], rax
.Lx17_181:              mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 96
                        cmp              rcx, 2;                    jbe   .Lx17_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx17_4:                mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_abs_α
proc_abs_α:
#-----------------------------------------------------------------------------------------------------------------------
n18_save_restore_α:                                                 jmp   n19_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n19_goto_deferred_α:    lea              rax, [rip + abs_body];     jmp   rax
                                                                    jmp   n20_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n20_save_restore_α:
abs_alpha:              mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx26_2
                        mov              eax, 16
.Lx26_2:                sub              rsp, 48
                        add              rsp, rax
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        cmp              rcx, 0;                    jbe   .Lx26_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 48]                       # abs
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx26_41
.Lx26_10:               mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx26_41:               lea              r10, [rip + abs_gamma]
                        lea              r11, [rip + abs_omega]
                        lea              rax, [rip + abs_body];     jmp   rax
abs_gamma:              mov              rdi, qword ptr [r9 + 48]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx26_75
                        mov              eax, 16
.Lx26_75:               lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx26_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 56], rax;  jmp   .Lx26_110
.Lx26_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 56], rax
.Lx26_110:              mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1;                    jbe   .Lx26_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx26_3:                mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
abs_omega:              mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx26_145
                        mov              eax, 16
.Lx26_145:              lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx26_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 56], rax;  jmp   .Lx26_180
.Lx26_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 56], rax
.Lx26_180:              mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1;                    jbe   .Lx26_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx26_4:                mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_sign_α
proc_sign_α:
#-----------------------------------------------------------------------------------------------------------------------
n27_save_restore_α:                                                 jmp   n28_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n28_goto_deferred_α:    lea              rax, [rip + sign_body];    jmp   rax
                                                                    jmp   n29_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n29_save_restore_α:
sign_alpha:             mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx35_2
                        mov              eax, 16
.Lx35_2:                sub              rsp, 48
                        add              rsp, rax
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        cmp              rcx, 0;                    jbe   .Lx35_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 64]                       # sign
                        mov              qword ptr [r9 + 64], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [r9 + 72], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx35_41
.Lx35_10:               mov              rax, qword ptr [r9 + 64]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
.Lx35_41:               lea              r10, [rip + sign_gamma]
                        lea              r11, [rip + sign_omega]
                        lea              rax, [rip + sign_body];    jmp   rax
sign_gamma:             mov              rdi, qword ptr [r9 + 64]
                        mov              rsi, qword ptr [r9 + 72]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx35_75
                        mov              eax, 16
.Lx35_75:               lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx35_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 72], rax;  jmp   .Lx35_110
.Lx35_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 72], rax
.Lx35_110:              mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1;                    jbe   .Lx35_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx35_3:                mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
sign_omega:             mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx35_145
                        mov              eax, 16
.Lx35_145:              lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx35_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 72], rax;  jmp   .Lx35_180
.Lx35_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 72], rax
.Lx35_180:              mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1;                    jbe   .Lx35_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx35_4:                mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_gcd_α
proc_gcd_α:
#-----------------------------------------------------------------------------------------------------------------------
n36_save_restore_α:                                                 jmp   n37_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n37_goto_deferred_α:    lea              rax, [rip + gcd_body];     jmp   rax
                                                                    jmp   n38_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n38_save_restore_α:
gcd_alpha:              mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx44_2
                        mov              eax, 32
.Lx44_2:                sub              rsp, 80
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 112]                      # r
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 112], 0
                        mov              qword ptr [r9 + 120], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0;                    jbe   .Lx44_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 80]                       # gcd
                        mov              qword ptr [r9 + 80], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [r9 + 88], rax
                        mov              qword ptr [r8 + 72], rdx;  jmp   .Lx44_41
.Lx44_10:               mov              rax, qword ptr [r9 + 80]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
.Lx44_41:               cmp              rcx, 1;                    jbe   .Lx44_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 96]                       # b
                        mov              qword ptr [r9 + 96], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [r9 + 104], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx44_42
.Lx44_11:               mov              rax, qword ptr [r9 + 96]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
.Lx44_42:               lea              r10, [rip + gcd_gamma]
                        lea              r11, [rip + gcd_omega]
                        lea              rax, [rip + gcd_body];     jmp   rax
gcd_gamma:              mov              rdi, qword ptr [r9 + 80]                       # gcd
                        mov              rsi, qword ptr [r9 + 88]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx44_75
                        mov              eax, 32
.Lx44_75:               lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rcx, 0;                    jbe   .Lx44_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 80], rax                       # gcd
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 88], rax;  jmp   .Lx44_110
.Lx44_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 88], rax
.Lx44_110:              cmp              rcx, 1;                    jbe   .Lx44_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 104], rax; jmp   .Lx44_111
.Lx44_81:               mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 104], rax
.Lx44_111:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                    jbe   .Lx44_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx44_3:                mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
gcd_omega:              mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx44_145
                        mov              eax, 32
.Lx44_145:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rcx, 0;                    jbe   .Lx44_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 80], rax                       # gcd
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 88], rax;  jmp   .Lx44_180
.Lx44_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 88], rax
.Lx44_180:              cmp              rcx, 1;                    jbe   .Lx44_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 104], rax; jmp   .Lx44_181
.Lx44_151:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 104], rax
.Lx44_181:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                    jbe   .Lx44_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx44_4:                mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_lcm_α
proc_lcm_α:
#-----------------------------------------------------------------------------------------------------------------------
n45_save_restore_α:                                                 jmp   n46_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n46_goto_deferred_α:    lea              rax, [rip + lcm_body];     jmp   rax
                                                                    jmp   n47_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n47_save_restore_α:
lcm_alpha:              mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx53_2
                        mov              eax, 32
.Lx53_2:                sub              rsp, 96
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 160]                      # g
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 160], 0
                        mov              qword ptr [r9 + 168], 0
                        mov              rax, qword ptr [r9 + 128]                      # lcm
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
                        mov              qword ptr [rsp + 32], r10
                        mov              qword ptr [rsp + 40], r11
                        mov              qword ptr [rsp + 48], rcx
                        cmp              rcx, 0;                    jbe   .Lx53_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 144]                      # a
                        mov              qword ptr [r9 + 144], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [r9 + 152], rax
                        mov              qword ptr [r8 + 72], rdx;  jmp   .Lx53_41
.Lx53_10:               mov              rax, qword ptr [r9 + 144]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 152]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 144], 0
                        mov              qword ptr [r9 + 152], 0
.Lx53_41:               cmp              rcx, 1;                    jbe   .Lx53_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 96]                       # b
                        mov              qword ptr [r9 + 96], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [r9 + 104], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx53_42
.Lx53_11:               mov              rax, qword ptr [r9 + 96]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
.Lx53_42:               lea              r10, [rip + lcm_gamma]
                        lea              r11, [rip + lcm_omega]
                        lea              rax, [rip + lcm_body];     jmp   rax
lcm_gamma:              mov              rdi, qword ptr [r9 + 128]                      # lcm
                        mov              rsi, qword ptr [r9 + 136]
                        mov              rcx, qword ptr [rsp + 48]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx53_75
                        mov              eax, 32
.Lx53_75:               lea              r8, [rsp + 64]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 160], rax                      # g
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 168], rax
                        cmp              rcx, 0;                    jbe   .Lx53_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 144], rax                      # a
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 152], rax; jmp   .Lx53_110
.Lx53_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 144], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 152], rax
.Lx53_110:              cmp              rcx, 1;                    jbe   .Lx53_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 104], rax; jmp   .Lx53_111
.Lx53_81:               mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 104], rax
.Lx53_111:              mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              eax, 128
                        cmp              rcx, 2;                    jbe   .Lx53_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 96
.Lx53_3:                mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
lcm_omega:              mov              rcx, qword ptr [rsp + 48]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx53_145
                        mov              eax, 32
.Lx53_145:              lea              r8, [rsp + 64]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 128], rax                      # lcm
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 160], rax                      # g
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 168], rax
                        cmp              rcx, 0;                    jbe   .Lx53_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 144], rax                      # a
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 152], rax; jmp   .Lx53_180
.Lx53_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 144], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 152], rax
.Lx53_180:              cmp              rcx, 1;                    jbe   .Lx53_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 104], rax; jmp   .Lx53_181
.Lx53_151:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 104], rax
.Lx53_181:              mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              eax, 128
                        cmp              rcx, 2;                    jbe   .Lx53_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 96
.Lx53_4:                mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   main_ω
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__max"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + max_body]
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
.Lstartup_pname1:       .string          "LBL__min"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + min_body]
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
.Lstartup_pname2:       .string          "LBL__abs"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + abs_body]
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
.Lstartup_pname3:       .string          "LBL__sign"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + sign_body]
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
.Lstartup_pname4:       .string          "LBL__gcd"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname4]
                        lea              rsi, [rip + gcd_body]
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
.Lstartup_pname5:       .string          "LBL__lcm"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + lcm_body]
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
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "max"
.Lgvan1:                .string          "x"
.Lgvan2:                .string          "min"
.Lgvan3:                .string          "abs"
.Lgvan4:                .string          "sign"
.Lgvan5:                .string          "gcd"
.Lgvan6:                .string          "b"
.Lgvan7:                .string          "r"
.Lgvan8:                .string          "lcm"
.Lgvan9:                .string          "a"
.Lgvan10:               .string          "g"
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
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 11
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 11
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                    jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_begin_α:                                              jmp   n55_statement_end_α
n54_statement_begin_β:                                              jmp   n56_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_end_α:                                                jmp   n56_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_begin_α:                                              jmp   n57_func_activate_α
n56_statement_begin_β:                                              jmp   n69_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n57_func_activate_α:    mov              rdi, qword ptr [rip + .Lx252_0]
                        mov              rsi, qword ptr [rip + .Lx252_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_max_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n58_statement_end_α
n57_func_activate_β:                                                jmp   n56_statement_begin_β
.Lx252_0:               .quad            .Lx252_0_s
.Lx252_0_s:             .string          "max"
.Lx252_1:               .quad            .Lx252_1_s
.Lx252_1_s:             .string          "max,x"
#-----------------------------------------------------------------------------------------------------------------------
n58_statement_end_α:                                                jmp   n69_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
max_body:                                                           jmp   n60_var_α
n59_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n60_var_α:              mov              rax, qword ptr [r9 + 0]                        # max
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 128], rax                     # result
                        mov              qword ptr [rsp + 136], rdx; jmp   n61_var_α
#-----------------------------------------------------------------------------------------------------------------------
n61_var_α:              mov              rax, qword ptr [r9 + 16]                       # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 112], rax                     # result
                        mov              qword ptr [rsp + 120], rdx; jmp   n62_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n62_coerce_numeric_α:   mov              eax, dword ptr [rsp + 128]
                        cmp              eax, 5;                    je    .Lx260_1
                        cmp              eax, 3;                    jne   .Lx260_0
                        mov              eax, dword ptr [rsp + 112]
                        cmp              eax, 3;                    jne   .Lx260_0
.Lx260_1:               mov              rax, qword ptr [rsp + 128]
                        mov              qword ptr [rsp + 96], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [rsp + 104], rax; jmp   n63_coerce_numeric_α
.Lx260_0:               lea              rdi, [rsp + 128]
                        lea              rsi, [rsp + 112]
                        lea              rdx, [rsp + 96]
                        mov              rcx, 147
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n63_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n63_coerce_numeric_α:   mov              eax, dword ptr [rsp + 112]
                        cmp              eax, 5;                    je    .Lx262_1
                        cmp              eax, 3;                    jne   .Lx262_0
                        mov              eax, dword ptr [rsp + 128]
                        cmp              eax, 3;                    jne   .Lx262_0
.Lx262_1:               mov              rax, qword ptr [rsp + 112]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 88], rax; jmp   n64_cmp_test_α
.Lx262_0:               lea              rdi, [rsp + 112]
                        lea              rsi, [rsp + 128]
                        lea              rdx, [rsp + 80]
                        mov              rcx, 148
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n64_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n64_cmp_test_α:         lea              rdi, [rsp + 96]
                        lea              rsi, [rsp + 80]
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
                        test             eax, eax;                  jns   n59_statement_begin_β
                        mov              qword ptr [rsp + 64], 0
                        mov              qword ptr [rsp + 72], 0;   jmp   n65_var_α
#-----------------------------------------------------------------------------------------------------------------------
n65_var_α:              mov              rax, qword ptr [r9 + 16]                       # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 144], rax                     # result
                        mov              qword ptr [rsp + 152], rdx; jmp   n66_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n66_binop_α:            mov              rdi, qword ptr [rsp + 64]
                        mov              rsi, qword ptr [rsp + 72]
                        mov              rdx, qword ptr [rsp + 144]
                        mov              rcx, qword ptr [rsp + 152]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n67_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n67_assign_α:           mov              rax, qword ptr [rsp + 48]
                        mov              rdx, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 0], rax                        # max
                        mov              qword ptr [r9 + 8], rdx;   jmp   n68_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_end_α:                                                jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_begin_α:                                              jmp   n70_statement_end_α
n69_statement_begin_β:                                              jmp   n71_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_end_α:                                                jmp   n71_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_begin_α:                                              jmp   n72_func_activate_α
n71_statement_begin_β:                                              jmp   n84_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n72_func_activate_α:    mov              rdi, qword ptr [rip + .Lx277_0]
                        mov              rsi, qword ptr [rip + .Lx277_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_min_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n73_statement_end_α
n72_func_activate_β:                                                jmp   n71_statement_begin_β
.Lx277_0:               .quad            .Lx277_0_s
.Lx277_0_s:             .string          "min"
.Lx277_1:               .quad            .Lx277_1_s
.Lx277_1_s:             .string          "min,x"
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_end_α:                                                jmp   n84_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
min_body:                                                           jmp   n75_var_α
n74_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n75_var_α:              mov              rax, qword ptr [r9 + 32]                       # min
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 256], rax                     # result
                        mov              qword ptr [rsp + 264], rdx; jmp   n76_var_α
#-----------------------------------------------------------------------------------------------------------------------
n76_var_α:              mov              rax, qword ptr [r9 + 16]                       # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 240], rax                     # result
                        mov              qword ptr [rsp + 248], rdx; jmp   n77_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n77_coerce_numeric_α:   mov              eax, dword ptr [rsp + 256]
                        cmp              eax, 5;                    je    .Lx285_1
                        cmp              eax, 3;                    jne   .Lx285_0
                        mov              eax, dword ptr [rsp + 240]
                        cmp              eax, 3;                    jne   .Lx285_0
.Lx285_1:               mov              rax, qword ptr [rsp + 256]
                        mov              qword ptr [rsp + 224], rax
                        mov              rax, qword ptr [rsp + 264]
                        mov              qword ptr [rsp + 232], rax; jmp   n78_coerce_numeric_α
.Lx285_0:               lea              rdi, [rsp + 256]
                        lea              rsi, [rsp + 240]
                        lea              rdx, [rsp + 224]
                        mov              rcx, 111
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n78_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n78_coerce_numeric_α:   mov              eax, dword ptr [rsp + 240]
                        cmp              eax, 5;                    je    .Lx287_1
                        cmp              eax, 3;                    jne   .Lx287_0
                        mov              eax, dword ptr [rsp + 256]
                        cmp              eax, 3;                    jne   .Lx287_0
.Lx287_1:               mov              rax, qword ptr [rsp + 240]
                        mov              qword ptr [rsp + 208], rax
                        mov              rax, qword ptr [rsp + 248]
                        mov              qword ptr [rsp + 216], rax; jmp   n79_cmp_test_α
.Lx287_0:               lea              rdi, [rsp + 240]
                        lea              rsi, [rsp + 256]
                        lea              rdx, [rsp + 208]
                        mov              rcx, 112
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n79_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n79_cmp_test_α:         lea              rdi, [rsp + 224]
                        lea              rsi, [rsp + 208]
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
                        test             eax, eax;                  jle   n74_statement_begin_β
                        mov              qword ptr [rsp + 192], 0
                        mov              qword ptr [rsp + 200], 0;  jmp   n80_var_α
#-----------------------------------------------------------------------------------------------------------------------
n80_var_α:              mov              rax, qword ptr [r9 + 16]                       # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 272], rax                     # result
                        mov              qword ptr [rsp + 280], rdx; jmp   n81_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n81_binop_α:            mov              rdi, qword ptr [rsp + 192]
                        mov              rsi, qword ptr [rsp + 200]
                        mov              rdx, qword ptr [rsp + 272]
                        mov              rcx, qword ptr [rsp + 280]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 176], rax
                        mov              qword ptr [rsp + 184], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n82_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n82_assign_α:           mov              rax, qword ptr [rsp + 176]
                        mov              rdx, qword ptr [rsp + 184]
                        mov              qword ptr [r9 + 32], rax                       # min
                        mov              qword ptr [r9 + 40], rdx;  jmp   n83_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n83_statement_end_α:                                                jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n84_statement_begin_α:                                              jmp   n85_statement_end_α
n84_statement_begin_β:                                              jmp   n86_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n85_statement_end_α:                                                jmp   n86_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n86_statement_begin_α:                                              jmp   n87_func_activate_α
n86_statement_begin_β:                                              jmp   n100_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n87_func_activate_α:    mov              rdi, qword ptr [rip + .Lx302_0]
                        mov              rsi, qword ptr [rip + .Lx302_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_abs_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n88_statement_end_α
n87_func_activate_β:                                                jmp   n86_statement_begin_β
.Lx302_0:               .quad            .Lx302_0_s
.Lx302_0_s:             .string          "abs"
.Lx302_1:               .quad            .Lx302_1_s
.Lx302_1_s:             .string          "abs"
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_end_α:                                                jmp   n100_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
abs_body:                                                           jmp   n90_var_α
n89_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n90_var_α:              mov              rax, qword ptr [r9 + 48]                       # abs
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 384], rax                     # result
                        mov              qword ptr [rsp + 392], rdx; jmp   n91_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n91_lit_integer_α:      mov              qword ptr [rsp + 368], 3                       # result
                        mov              rax, qword ptr [rip + .Lx308_0]
                        mov              qword ptr [rsp + 376], rax; jmp   n92_coerce_numeric_α
.Lx308_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n92_coerce_numeric_α:   mov              eax, dword ptr [rsp + 384]
                        cmp              eax, 5;                    je    .Lx310_1
                        cmp              eax, 3;                    jne   .Lx310_0
                        mov              eax, dword ptr [rsp + 368]
                        cmp              eax, 3;                    jne   .Lx310_0
.Lx310_1:               mov              rax, qword ptr [rsp + 384]
                        mov              qword ptr [rsp + 352], rax
                        mov              rax, qword ptr [rsp + 392]
                        mov              qword ptr [rsp + 360], rax; jmp   n93_coerce_numeric_α
.Lx310_0:               lea              rdi, [rsp + 384]
                        lea              rsi, [rsp + 368]
                        lea              rdx, [rsp + 352]
                        mov              rcx, 147
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n93_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n93_coerce_numeric_α:   mov              eax, dword ptr [rsp + 368]
                        cmp              eax, 5;                    je    .Lx312_1
                        cmp              eax, 3;                    jne   .Lx312_0
                        mov              eax, dword ptr [rsp + 384]
                        cmp              eax, 3;                    jne   .Lx312_0
.Lx312_1:               mov              rax, qword ptr [rsp + 368]
                        mov              qword ptr [rsp + 336], rax
                        mov              rax, qword ptr [rsp + 376]
                        mov              qword ptr [rsp + 344], rax; jmp   n94_cmp_test_α
.Lx312_0:               lea              rdi, [rsp + 368]
                        lea              rsi, [rsp + 384]
                        lea              rdx, [rsp + 336]
                        mov              rcx, 148
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n94_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n94_cmp_test_α:         lea              rdi, [rsp + 352]
                        lea              rsi, [rsp + 336]
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
                        test             eax, eax;                  jns   n89_statement_begin_β
                        mov              qword ptr [rsp + 320], 0
                        mov              qword ptr [rsp + 328], 0;  jmp   n95_var_α
#-----------------------------------------------------------------------------------------------------------------------
n95_var_α:              mov              rax, qword ptr [r9 + 48]                       # abs
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 416], rax                     # result
                        mov              qword ptr [rsp + 424], rdx; jmp   n96_unop_α
#-----------------------------------------------------------------------------------------------------------------------
n96_unop_α:             mov              rdi, qword ptr [rsp + 416]
                        mov              rsi, qword ptr [rsp + 424]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_num_neg@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 400], rax
                        mov              qword ptr [rsp + 408], rdx; jmp   n97_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n97_binop_α:            mov              rdi, qword ptr [rsp + 320]
                        mov              rsi, qword ptr [rsp + 328]
                        mov              rdx, qword ptr [rsp + 400]
                        mov              rcx, qword ptr [rsp + 408]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n98_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n98_assign_α:           mov              rax, qword ptr [rsp + 304]
                        mov              rdx, qword ptr [rsp + 312]
                        mov              qword ptr [r9 + 48], rax                       # abs
                        mov              qword ptr [r9 + 56], rdx;  jmp   n99_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n99_statement_end_α:                                                jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n100_statement_begin_α:                                             jmp   n101_statement_end_α
n100_statement_begin_β:                                             jmp   n102_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n101_statement_end_α:                                               jmp   n102_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_begin_α:                                             jmp   n103_func_activate_α
n102_statement_begin_β:                                             jmp   n126_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n103_func_activate_α:   mov              rdi, qword ptr [rip + .Lx328_0]
                        mov              rsi, qword ptr [rip + .Lx328_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_sign_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n104_statement_end_α
n103_func_activate_β:                                               jmp   n102_statement_begin_β
.Lx328_0:               .quad            .Lx328_0_s
.Lx328_0_s:             .string          "sign"
.Lx328_1:               .quad            .Lx328_1_s
.Lx328_1_s:             .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_end_α:                                               jmp   n126_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
sign_body:                                                          jmp   n106_var_α
n105_statement_begin_β:                                             jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n106_var_α:             mov              rax, qword ptr [r9 + 64]                       # sign
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 528], rax                     # result
                        mov              qword ptr [rsp + 536], rdx; jmp   n107_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n107_lit_integer_α:     mov              qword ptr [rsp + 512], 3                       # result
                        mov              rax, qword ptr [rip + .Lx334_0]
                        mov              qword ptr [rsp + 520], rax; jmp   n108_coerce_numeric_α
.Lx334_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n108_coerce_numeric_α:  mov              eax, dword ptr [rsp + 528]
                        cmp              eax, 5;                    je    .Lx336_1
                        cmp              eax, 3;                    jne   .Lx336_0
                        mov              eax, dword ptr [rsp + 512]
                        cmp              eax, 3;                    jne   .Lx336_0
.Lx336_1:               mov              rax, qword ptr [rsp + 528]
                        mov              qword ptr [rsp + 496], rax
                        mov              rax, qword ptr [rsp + 536]
                        mov              qword ptr [rsp + 504], rax; jmp   n109_coerce_numeric_α
.Lx336_0:               lea              rdi, [rsp + 528]
                        lea              rsi, [rsp + 512]
                        lea              rdx, [rsp + 496]
                        mov              rcx, 147
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n109_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n109_coerce_numeric_α:  mov              eax, dword ptr [rsp + 512]
                        cmp              eax, 5;                    je    .Lx338_1
                        cmp              eax, 3;                    jne   .Lx338_0
                        mov              eax, dword ptr [rsp + 528]
                        cmp              eax, 3;                    jne   .Lx338_0
.Lx338_1:               mov              rax, qword ptr [rsp + 512]
                        mov              qword ptr [rsp + 480], rax
                        mov              rax, qword ptr [rsp + 520]
                        mov              qword ptr [rsp + 488], rax; jmp   n110_cmp_test_α
.Lx338_0:               lea              rdi, [rsp + 512]
                        lea              rsi, [rsp + 528]
                        lea              rdx, [rsp + 480]
                        mov              rcx, 148
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n110_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n110_cmp_test_α:        lea              rdi, [rsp + 496]
                        lea              rsi, [rsp + 480]
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
                        test             eax, eax;                  jns   n105_statement_begin_β
                        mov              qword ptr [rsp + 464], 0
                        mov              qword ptr [rsp + 472], 0;  jmp   n111_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n111_lit_integer_α:     mov              qword ptr [rsp + 560], 3                       # result
                        mov              rax, qword ptr [rip + .Lx341_0]
                        mov              qword ptr [rsp + 568], rax; jmp   n112_unop_α
.Lx341_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n112_unop_α:            mov              rdi, qword ptr [rsp + 560]
                        mov              rsi, qword ptr [rsp + 568]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_num_neg@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 544], rax
                        mov              qword ptr [rsp + 552], rdx; jmp   n113_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n113_binop_α:           mov              rdi, qword ptr [rsp + 464]
                        mov              rsi, qword ptr [rsp + 472]
                        mov              rdx, qword ptr [rsp + 544]
                        mov              rcx, qword ptr [rsp + 552]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 448], rax
                        mov              qword ptr [rsp + 456], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n114_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n114_assign_α:          mov              rax, qword ptr [rsp + 448]
                        mov              rdx, qword ptr [rsp + 456]
                        mov              qword ptr [r9 + 64], rax                       # sign
                        mov              qword ptr [r9 + 72], rdx;  jmp   n115_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n115_statement_end_α:                                               jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n116_statement_begin_α:                                             jmp   n117_var_α
n116_statement_begin_β:                                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n117_var_α:             mov              rax, qword ptr [r9 + 64]                       # sign
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 656], rax                     # result
                        mov              qword ptr [rsp + 664], rdx; jmp   n118_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n118_lit_integer_α:     mov              qword ptr [rsp + 640], 3                       # result
                        mov              rax, qword ptr [rip + .Lx350_0]
                        mov              qword ptr [rsp + 648], rax; jmp   n119_coerce_numeric_α
.Lx350_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n119_coerce_numeric_α:  mov              eax, dword ptr [rsp + 656]
                        cmp              eax, 5;                    je    .Lx352_1
                        cmp              eax, 3;                    jne   .Lx352_0
                        mov              eax, dword ptr [rsp + 640]
                        cmp              eax, 3;                    jne   .Lx352_0
.Lx352_1:               mov              rax, qword ptr [rsp + 656]
                        mov              qword ptr [rsp + 624], rax
                        mov              rax, qword ptr [rsp + 664]
                        mov              qword ptr [rsp + 632], rax; jmp   n120_coerce_numeric_α
.Lx352_0:               lea              rdi, [rsp + 656]
                        lea              rsi, [rsp + 640]
                        lea              rdx, [rsp + 624]
                        mov              rcx, 111
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n120_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n120_coerce_numeric_α:  mov              eax, dword ptr [rsp + 640]
                        cmp              eax, 5;                    je    .Lx354_1
                        cmp              eax, 3;                    jne   .Lx354_0
                        mov              eax, dword ptr [rsp + 656]
                        cmp              eax, 3;                    jne   .Lx354_0
.Lx354_1:               mov              rax, qword ptr [rsp + 640]
                        mov              qword ptr [rsp + 608], rax
                        mov              rax, qword ptr [rsp + 648]
                        mov              qword ptr [rsp + 616], rax; jmp   n121_cmp_test_α
.Lx354_0:               lea              rdi, [rsp + 640]
                        lea              rsi, [rsp + 656]
                        lea              rdx, [rsp + 608]
                        mov              rcx, 112
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n121_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n121_cmp_test_α:        lea              rdi, [rsp + 624]
                        lea              rsi, [rsp + 608]
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
                        test             eax, eax;                  jle   n116_statement_begin_β
                        mov              qword ptr [rsp + 592], 0
                        mov              qword ptr [rsp + 600], 0;  jmp   n122_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n122_lit_integer_α:     mov              qword ptr [rsp + 672], 3                       # result
                        mov              rax, qword ptr [rip + .Lx357_0]
                        mov              qword ptr [rsp + 680], rax; jmp   n123_binop_α
.Lx357_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n123_binop_α:           mov              rdi, qword ptr [rsp + 592]
                        mov              rsi, qword ptr [rsp + 600]
                        mov              rdx, qword ptr [rsp + 672]
                        mov              rcx, qword ptr [rsp + 680]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 576], rax
                        mov              qword ptr [rsp + 584], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n124_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n124_assign_α:          mov              rax, qword ptr [rsp + 576]
                        mov              rdx, qword ptr [rsp + 584]
                        mov              qword ptr [r9 + 64], rax                       # sign
                        mov              qword ptr [r9 + 72], rdx;  jmp   n125_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n125_statement_end_α:                                               jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n126_statement_begin_α:                                             jmp   n127_statement_end_α
n126_statement_begin_β:                                             jmp   n128_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n127_statement_end_α:                                               jmp   n128_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n128_statement_begin_α:                                             jmp   n129_func_activate_α
n128_statement_begin_β:                                             jmp   n150_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n129_func_activate_α:   mov              rdi, qword ptr [rip + .Lx369_0]
                        mov              rsi, qword ptr [rip + .Lx369_1]
                        mov              edx, 3
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_gcd_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n130_statement_end_α
n129_func_activate_β:                                               jmp   n128_statement_begin_β
.Lx369_0:               .quad            .Lx369_0_s
.Lx369_0_s:             .string          "gcd"
.Lx369_1:               .quad            .Lx369_1_s
.Lx369_1_s:             .string          "gcd,b,r"
#-----------------------------------------------------------------------------------------------------------------------
n130_statement_end_α:                                               jmp   n150_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
gcd_body:                                                           jmp   n132_var_α
n131_statement_begin_β:                                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n132_var_α:             mov              rax, qword ptr [r9 + 96]                       # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 768], rax                     # result
                        mov              qword ptr [rsp + 776], rdx; jmp   n133_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n133_lit_integer_α:     mov              qword ptr [rsp + 784], 3                       # result
                        mov              rax, qword ptr [rip + .Lx375_0]
                        mov              qword ptr [rsp + 792], rax; jmp   n134_call_α
.Lx375_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n134_call_α:            mov              rax, qword ptr [rsp + 784]
                        mov              qword ptr [rsp + 736], rax
                        mov              rax, qword ptr [rsp + 792]
                        mov              qword ptr [rsp + 744], rax
                        mov              rax, qword ptr [rsp + 768]
                        mov              qword ptr [rsp + 720], rax
                        mov              rax, qword ptr [rsp + 776]
                        mov              qword ptr [rsp + 728], rax
                        .section         .rodata
.Lrkfn377:              .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn377]
                        lea              rsi, [rsp + 720]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 704], rax
                        mov              qword ptr [rsp + 712], rdx
                        cmp              eax, 104;                  je    n131_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n135_statement_end_α
n134_call_β:                                                        jmp   n131_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n135_statement_end_α:                                               jmp   n136_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n136_statement_begin_α:                                             jmp   n137_var_α
n136_statement_begin_β:                                             jmp   n142_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n137_var_α:             mov              rax, qword ptr [r9 + 80]                       # gcd
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 864], rax                     # result
                        mov              qword ptr [rsp + 872], rdx; jmp   n138_var_α
#-----------------------------------------------------------------------------------------------------------------------
n138_var_α:             mov              rax, qword ptr [r9 + 96]                       # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 880], rax                     # result
                        mov              qword ptr [rsp + 888], rdx; jmp   n139_call_α
#-----------------------------------------------------------------------------------------------------------------------
n139_call_α:            mov              rax, qword ptr [rsp + 880]
                        mov              qword ptr [rsp + 832], rax
                        mov              rax, qword ptr [rsp + 888]
                        mov              qword ptr [rsp + 840], rax
                        mov              rax, qword ptr [rsp + 864]
                        mov              qword ptr [rsp + 816], rax
                        mov              rax, qword ptr [rsp + 872]
                        mov              qword ptr [rsp + 824], rax
                        .section         .rodata
.Lrkfn385:              .string          "REMDR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn385]
                        lea              rsi, [rsp + 816]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 800], rax
                        mov              qword ptr [rsp + 808], rdx
                        cmp              eax, 104;                  je    n136_statement_begin_β
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n140_assign_α
n139_call_β:                                                        jmp   n136_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n140_assign_α:          mov              rax, qword ptr [rsp + 800]
                        mov              rdx, qword ptr [rsp + 808]
                        mov              qword ptr [r9 + 112], rax                      # r
                        mov              qword ptr [r9 + 120], rdx; jmp   n141_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n141_statement_end_α:                                               jmp   n142_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n142_statement_begin_α:                                             jmp   n143_var_α
n142_statement_begin_β:                                             jmp   n146_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n143_var_α:             mov              rax, qword ptr [r9 + 96]                       # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 896], rax                     # result
                        mov              qword ptr [rsp + 904], rdx; jmp   n144_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n144_assign_α:          mov              rax, qword ptr [rsp + 896]
                        mov              rdx, qword ptr [rsp + 904]
                        mov              qword ptr [r9 + 80], rax                       # gcd
                        mov              qword ptr [r9 + 88], rdx;  jmp   n145_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n145_statement_end_α:                                               jmp   n146_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n146_statement_begin_α:                                             jmp   n147_var_α
n146_statement_begin_β:                                             jmp   gcd_body
#-----------------------------------------------------------------------------------------------------------------------
n147_var_α:             mov              rax, qword ptr [r9 + 112]                      # r
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 912], rax                     # result
                        mov              qword ptr [rsp + 920], rdx; jmp   n148_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n148_assign_α:          mov              rax, qword ptr [rsp + 912]
                        mov              rdx, qword ptr [rsp + 920]
                        mov              qword ptr [r9 + 96], rax                       # b
                        mov              qword ptr [r9 + 104], rdx; jmp   n149_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n149_statement_end_α:                                               jmp   gcd_body
#-----------------------------------------------------------------------------------------------------------------------
n150_statement_begin_α:                                             jmp   n151_statement_end_α
n150_statement_begin_β:                                             jmp   n152_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n151_statement_end_α:                                               jmp   n152_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n152_statement_begin_α:                                             jmp   n153_func_activate_α
n152_statement_begin_β:                                             jmp   n169_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n153_func_activate_α:   mov              rdi, qword ptr [rip + .Lx408_0]
                        mov              rsi, qword ptr [rip + .Lx408_1]
                        mov              edx, 3
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_lcm_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n154_statement_end_α
n153_func_activate_β:                                               jmp   n152_statement_begin_β
.Lx408_0:               .quad            .Lx408_0_s
.Lx408_0_s:             .string          "lcm"
.Lx408_1:               .quad            .Lx408_1_s
.Lx408_1_s:             .string          "a,b,g"
#-----------------------------------------------------------------------------------------------------------------------
n154_statement_end_α:                                               jmp   n169_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
lcm_body:                                                           jmp   n156_var_α
n155_statement_begin_β:                                             jmp   n161_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n156_var_α:             mov              rax, qword ptr [r9 + 144]                      # a
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 1008], rax                    # result
                        mov              qword ptr [rsp + 1016], rdx; jmp   n157_var_α
#-----------------------------------------------------------------------------------------------------------------------
n157_var_α:             mov              rax, qword ptr [r9 + 96]                       # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 1024], rax                    # result
                        mov              qword ptr [rsp + 1032], rdx; jmp   n158_call_α
#-----------------------------------------------------------------------------------------------------------------------
n158_call_α:            sub              rsp, 64
                        mov              rax, qword ptr [rsp + 1072]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 1080]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 1088]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 1096]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx416_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + gcd_alpha];    jmp   rax
                        lea              r8, [rip + g_gc_pending]
                        mov              eax, dword ptr [r8 + 0]
                        test             eax, eax;                  jne   .Lx416_20
                        mov              rax, qword ptr [rsp + 1008]
                        mov              rdx, qword ptr [rsp + 1016]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 0], rax
                        mov              qword ptr [r8 + 8], rdx;   jmp   .Lx416_21
.Lx416_20:              mov              edi, 0
                        mov              rsi, qword ptr [rsp + 1008]
                        mov              rdx, qword ptr [rsp + 1016]
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
.Lx416_21:              lea              r8, [rip + g_gc_pending]
                        mov              eax, dword ptr [r8 + 0]
                        test             eax, eax;                  jne   .Lx416_22
                        mov              rax, qword ptr [rsp + 1024]
                        mov              rdx, qword ptr [rsp + 1032]
                        lea              r8, [rip + g_call_args]
                        mov              qword ptr [r8 + 16], rax
                        mov              qword ptr [r8 + 24], rdx;  jmp   .Lx416_23
.Lx416_22:              mov              edi, 1
                        mov              rsi, qword ptr [rsp + 1024]
                        mov              rdx, qword ptr [rsp + 1032]
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
.Lx416_23:              mov              rdi, qword ptr [rip + .Lx416_0]
                        mov              esi, 2
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
                        test             rax, rax;                  je    .Lx416_1
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
                        lea              rcx, [rip + .Lx416_3]
                        lea              rdx, [rip + .Lx416_4];     jmp   rax
.Lx416_3:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx416_2
.Lx416_4:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   .Lx416_2
.Lx416_1:               mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_faildescr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
.Lx416_2:               mov              qword ptr [rsp + 944], rax
                        mov              qword ptr [rsp + 952], rdx
                        cmp              eax, 104;                  je    n155_statement_begin_β
                                                                    jmp   n159_assign_α
n158_call_β:                                                        jmp   n155_statement_begin_β
.Lx416_0:               .quad            .Lx416_0_s
.Lx416_0_s:             .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n159_assign_α:          mov              rax, qword ptr [rsp + 944]
                        mov              rdx, qword ptr [rsp + 952]
                        mov              qword ptr [r9 + 160], rax                      # g
                        mov              qword ptr [r9 + 168], rdx; jmp   n160_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n160_statement_end_α:                                               jmp   n161_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n161_statement_begin_α:                                             jmp   n162_var_α
n161_statement_begin_β:                                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n162_var_α:             mov              rax, qword ptr [r9 + 144]                      # a
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 1072], rax                    # result
                        mov              qword ptr [rsp + 1080], rdx; jmp   n163_var_α
#-----------------------------------------------------------------------------------------------------------------------
n163_var_α:             mov              rax, qword ptr [r9 + 160]                      # g
                        mov              rdx, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 1088], rax                    # result
                        mov              qword ptr [rsp + 1096], rdx; jmp   n164_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n164_binop_α:           mov              rdi, qword ptr [rsp + 1072]
                        mov              rsi, qword ptr [rsp + 1080]
                        mov              rdx, qword ptr [rsp + 1088]
                        mov              rcx, qword ptr [rsp + 1096]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_div@PLT
                        cmp              eax, 104;                  jne   .Lx424_240
                        add              rsp, 32;                   jmp   n161_statement_begin_β
.Lx424_240:             mov              qword ptr [rsp + 1056], rax
                        mov              qword ptr [rsp + 1064], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n165_var_α
#-----------------------------------------------------------------------------------------------------------------------
n165_var_α:             mov              rax, qword ptr [r9 + 96]                       # b
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 1104], rax                    # result
                        mov              qword ptr [rsp + 1112], rdx; jmp   n166_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n166_binop_α:           mov              eax, dword ptr [rsp + 1056]
                        cmp              eax, 3;                    jne   .Lx426_0
                        mov              eax, dword ptr [rsp + 1104]
                        cmp              eax, 3;                    jne   .Lx426_0
                        mov              rax, qword ptr [rsp + 1064]
                        mov              rcx, qword ptr [rsp + 1112]
                        imul             rax, rcx
                        mov              qword ptr [rsp + 1040], 3
                        mov              qword ptr [rsp + 1048], rax; jmp   n167_assign_α
.Lx426_0:               mov              rdi, qword ptr [rsp + 1056]
                        mov              rsi, qword ptr [rsp + 1064]
                        mov              rdx, qword ptr [rsp + 1104]
                        mov              rcx, qword ptr [rsp + 1112]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_mul@PLT
                        cmp              eax, 104;                  jne   .Lx426_240
                        add              rsp, 32;                   jmp   n161_statement_begin_β
.Lx426_240:             mov              qword ptr [rsp + 1040], rax
                        mov              qword ptr [rsp + 1048], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n167_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n167_assign_α:          mov              rax, qword ptr [rsp + 1040]
                        mov              rdx, qword ptr [rsp + 1048]
                        mov              qword ptr [r9 + 128], rax                      # lcm
                        mov              qword ptr [r9 + 136], rdx; jmp   n168_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n168_statement_end_α:                                               jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n169_statement_begin_α:                                             jmp   n170_statement_end_α
n169_statement_begin_β:                                             jmp   n171_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n170_statement_end_α:                                               jmp   n171_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n171_statement_begin_α:                                             jmp   n172_statement_end_α
n171_statement_begin_β:                                             jmp   n173_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n172_statement_end_α:                                               jmp   n173_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n173_statement_begin_α:                                             jmp   n174_lit_string_α
n173_statement_begin_β:                                             jmp   n178_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n174_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx440_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n175_lit_integer_α
.Lx440_0:               .quad            .Lx440_0_s
.Lx440_0_s:             .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n175_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx441_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n176_call_α
n175_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                   jmp   n173_statement_begin_β
.Lx441_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n176_call_α:            sub              rsp, 16
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
.Lrkfnzd443:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd443]
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
                        cmp              eax, 104;                  jne   .Lx442_240
                        add              rsp, 16;                   jmp   n175_lit_integer_β
.Lx442_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n177_statement_end_α
n176_call_β:            add              rsp, 16;                   jmp   n175_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n177_statement_end_α:                                               jmp   n178_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n178_statement_begin_α:                                             jmp   n179_statement_end_α
n178_statement_begin_β:                                             jmp   n180_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n179_statement_end_α:                                               jmp   n180_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n180_statement_begin_α:                                             jmp   n181_lit_integer_α
n180_statement_begin_β:                                             jmp   n186_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n181_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx452_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n182_lit_integer_α
n181_lit_integer_β:     add              rsp, 16
                        add              rsp, 48;                   jmp   n180_statement_begin_β
.Lx452_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n182_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx453_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n183_call_α
n182_lit_integer_β:     add              rsp, 16;                   jmp   n181_lit_integer_β
.Lx453_0:               .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n183_call_α:            sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx455_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + max_alpha];    jmp   rax
.Lx455_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx455_240
                        add              rsp, 16;                   jmp   n182_lit_integer_β
.Lx455_240:                                                         jmp   n184_assign_α
n183_call_β:                                                        jmp   n182_lit_integer_β
.Lx455_0:               .quad            .Lx455_0_s
.Lx455_0_s:             .string          "max"
#-----------------------------------------------------------------------------------------------------------------------
n184_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx456_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n185_statement_end_α
.Lx456_0:               .quad            .Lx456_0_s
.Lx456_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n185_statement_end_α:                                               jmp   n186_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n186_statement_begin_α:                                             jmp   n187_lit_integer_α
n186_statement_begin_β:                                             jmp   n192_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n187_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx461_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n188_lit_integer_α
n187_lit_integer_β:     add              rsp, 16
                        add              rsp, 96;                   jmp   n186_statement_begin_β
.Lx461_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n188_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx462_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n189_call_α
n188_lit_integer_β:     add              rsp, 16;                   jmp   n187_lit_integer_β
.Lx462_0:               .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n189_call_α:            sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx464_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + min_alpha];    jmp   rax
.Lx464_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx464_240
                        add              rsp, 16;                   jmp   n188_lit_integer_β
.Lx464_240:                                                         jmp   n190_assign_α
n189_call_β:                                                        jmp   n188_lit_integer_β
.Lx464_0:               .quad            .Lx464_0_s
.Lx464_0_s:             .string          "min"
#-----------------------------------------------------------------------------------------------------------------------
n190_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx465_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n191_statement_end_α
.Lx465_0:               .quad            .Lx465_0_s
.Lx465_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n191_statement_end_α:                                               jmp   n192_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n192_statement_begin_α:                                             jmp   n193_lit_real_α
n192_statement_begin_β:                                             jmp   n198_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n193_lit_real_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx470_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n194_lit_real_α
.Lx470_0:               .quad            4615063718147915776
#-----------------------------------------------------------------------------------------------------------------------
n194_lit_real_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx471_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n195_call_α
.Lx471_0:               .quad            4611911198408756429
#-----------------------------------------------------------------------------------------------------------------------
n195_call_α:            sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_real
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx473_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + max_alpha];    jmp   rax
.Lx473_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx473_240
                        add              rsp, 16
                        add              rsp, 176;                  jmp   n192_statement_begin_β
.Lx473_240:                                                         jmp   n196_assign_α
n195_call_β:                                                        jmp   n192_statement_begin_β
.Lx473_0:               .quad            .Lx473_0_s
.Lx473_0_s:             .string          "max"
#-----------------------------------------------------------------------------------------------------------------------
n196_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx474_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n197_statement_end_α
.Lx474_0:               .quad            .Lx474_0_s
.Lx474_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n197_statement_end_α:                                               jmp   n198_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n198_statement_begin_α:                                             jmp   n199_lit_real_α
n198_statement_begin_β:                                             jmp   n204_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n199_lit_real_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx479_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n200_lit_real_α
.Lx479_0:               .quad            4615063718147915776
#-----------------------------------------------------------------------------------------------------------------------
n200_lit_real_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 5                         # result
                        mov              rax, qword ptr [rip + .Lx480_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n201_call_α
.Lx480_0:               .quad            4611911198408756429
#-----------------------------------------------------------------------------------------------------------------------
n201_call_α:            sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_real
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx482_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + min_alpha];    jmp   rax
.Lx482_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx482_240
                        add              rsp, 16
                        add              rsp, 224;                  jmp   n198_statement_begin_β
.Lx482_240:                                                         jmp   n202_assign_α
n201_call_β:                                                        jmp   n198_statement_begin_β
.Lx482_0:               .quad            .Lx482_0_s
.Lx482_0_s:             .string          "min"
#-----------------------------------------------------------------------------------------------------------------------
n202_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx483_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n203_statement_end_α
.Lx483_0:               .quad            .Lx483_0_s
.Lx483_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n203_statement_end_α:                                               jmp   n204_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n204_statement_begin_α:                                             jmp   n205_lit_integer_α
n204_statement_begin_β:                                             jmp   n210_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n205_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx488_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n206_unop_α
n205_lit_integer_β:     add              rsp, 16
                        add              rsp, 240;                  jmp   n204_statement_begin_β
.Lx488_0:               .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n206_unop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_num_neg@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n207_call_α
n206_unop_β:            add              rsp, 16;                   jmp   n205_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n207_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # unop
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx491_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + abs_alpha];    jmp   rax
.Lx491_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx491_240
                        add              rsp, 16;                   jmp   n206_unop_β
.Lx491_240:                                                         jmp   n208_assign_α
n207_call_β:                                                        jmp   n206_unop_β
.Lx491_0:               .quad            .Lx491_0_s
.Lx491_0_s:             .string          "abs"
#-----------------------------------------------------------------------------------------------------------------------
n208_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx492_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n209_statement_end_α
.Lx492_0:               .quad            .Lx492_0_s
.Lx492_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n209_statement_end_α:                                               jmp   n210_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n210_statement_begin_α:                                             jmp   n211_lit_integer_α
n210_statement_begin_β:                                             jmp   n215_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n211_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx497_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n212_call_α
n211_lit_integer_β:     add              rsp, 16
                        add              rsp, 288;                  jmp   n210_statement_begin_β
.Lx497_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n212_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx499_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + sign_alpha];   jmp   rax
.Lx499_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx499_240
                        add              rsp, 16;                   jmp   n211_lit_integer_β
.Lx499_240:                                                         jmp   n213_assign_α
n212_call_β:                                                        jmp   n211_lit_integer_β
.Lx499_0:               .quad            .Lx499_0_s
.Lx499_0_s:             .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n213_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx500_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n214_statement_end_α
.Lx500_0:               .quad            .Lx500_0_s
.Lx500_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n214_statement_end_α:                                               jmp   n215_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n215_statement_begin_α:                                             jmp   n216_lit_integer_α
n215_statement_begin_β:                                             jmp   n220_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n216_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx505_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n217_call_α
n216_lit_integer_β:     add              rsp, 16
                        add              rsp, 320;                  jmp   n215_statement_begin_β
.Lx505_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n217_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx507_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + sign_alpha];   jmp   rax
.Lx507_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx507_240
                        add              rsp, 16;                   jmp   n216_lit_integer_β
.Lx507_240:                                                         jmp   n218_assign_α
n217_call_β:                                                        jmp   n216_lit_integer_β
.Lx507_0:               .quad            .Lx507_0_s
.Lx507_0_s:             .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n218_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx508_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n219_statement_end_α
.Lx508_0:               .quad            .Lx508_0_s
.Lx508_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n219_statement_end_α:                                               jmp   n220_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n220_statement_begin_α:                                             jmp   n221_lit_integer_α
n220_statement_begin_β:                                             jmp   n226_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n221_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx513_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n222_unop_α
n221_lit_integer_β:     add              rsp, 16
                        add              rsp, 352;                  jmp   n220_statement_begin_β
.Lx513_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n222_unop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_num_neg@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n223_call_α
n222_unop_β:            add              rsp, 16;                   jmp   n221_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n223_call_α:            sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # unop
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx516_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + sign_alpha];   jmp   rax
.Lx516_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx516_240
                        add              rsp, 16;                   jmp   n222_unop_β
.Lx516_240:                                                         jmp   n224_assign_α
n223_call_β:                                                        jmp   n222_unop_β
.Lx516_0:               .quad            .Lx516_0_s
.Lx516_0_s:             .string          "sign"
#-----------------------------------------------------------------------------------------------------------------------
n224_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx517_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n225_statement_end_α
.Lx517_0:               .quad            .Lx517_0_s
.Lx517_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n225_statement_end_α:                                               jmp   n226_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n226_statement_begin_α:                                             jmp   n227_lit_integer_α
n226_statement_begin_β:                                             jmp   n232_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n227_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx522_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n228_lit_integer_α
n227_lit_integer_β:     add              rsp, 16
                        add              rsp, 400;                  jmp   n226_statement_begin_β
.Lx522_0:               .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n228_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx523_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n229_call_α
n228_lit_integer_β:     add              rsp, 16;                   jmp   n227_lit_integer_β
.Lx523_0:               .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n229_call_α:            sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx525_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + gcd_alpha];    jmp   rax
.Lx525_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx525_240
                        add              rsp, 16;                   jmp   n228_lit_integer_β
.Lx525_240:                                                         jmp   n230_assign_α
n229_call_β:                                                        jmp   n228_lit_integer_β
.Lx525_0:               .quad            .Lx525_0_s
.Lx525_0_s:             .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n230_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx526_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n231_statement_end_α
.Lx526_0:               .quad            .Lx526_0_s
.Lx526_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n231_statement_end_α:                                               jmp   n232_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n232_statement_begin_α:                                             jmp   n233_lit_integer_α
n232_statement_begin_β:                                             jmp   n238_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n233_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx531_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n234_lit_integer_α
n233_lit_integer_β:     add              rsp, 16
                        add              rsp, 448;                  jmp   n232_statement_begin_β
.Lx531_0:               .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n234_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx532_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n235_call_α
n234_lit_integer_β:     add              rsp, 16;                   jmp   n233_lit_integer_β
.Lx532_0:               .quad            75
#-----------------------------------------------------------------------------------------------------------------------
n235_call_α:            sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx534_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + gcd_alpha];    jmp   rax
.Lx534_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx534_240
                        add              rsp, 16;                   jmp   n234_lit_integer_β
.Lx534_240:                                                         jmp   n236_assign_α
n235_call_β:                                                        jmp   n234_lit_integer_β
.Lx534_0:               .quad            .Lx534_0_s
.Lx534_0_s:             .string          "gcd"
#-----------------------------------------------------------------------------------------------------------------------
n236_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx535_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n237_statement_end_α
.Lx535_0:               .quad            .Lx535_0_s
.Lx535_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n237_statement_end_α:                                               jmp   n238_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n238_statement_begin_α:                                             jmp   n239_lit_integer_α
n238_statement_begin_β: add              rsp, 496;                  jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n239_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx540_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n240_lit_integer_α
n239_lit_integer_β:     add              rsp, 16
                        add              rsp, 496;                  jmp   n238_statement_begin_β
.Lx540_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n240_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx541_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n241_call_α
n240_lit_integer_β:     add              rsp, 16;                   jmp   n239_lit_integer_β
.Lx541_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n241_call_α:            sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx543_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + lcm_alpha];    jmp   rax
.Lx543_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx543_240
                        add              rsp, 16;                   jmp   n240_lit_integer_β
.Lx543_240:                                                         jmp   n242_assign_α
n241_call_β:                                                        jmp   n240_lit_integer_β
.Lx543_0:               .quad            .Lx543_0_s
.Lx543_0_s:             .string          "lcm"
#-----------------------------------------------------------------------------------------------------------------------
n242_assign_α:          mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx544_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n243_statement_end_α
.Lx544_0:               .quad            .Lx544_0_s
.Lx544_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n243_statement_end_α:   add              rsp, 544;                  jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                    jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              edi, 1
                        call             exit@PLT
                        .section         .rodata
.S0:                    .string          "BOMB-RETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
                        .text
                        .section         .note.GNU-stack,"",@progbits
