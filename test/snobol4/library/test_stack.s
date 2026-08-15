                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_init_α
proc_stack_init_α:
#-----------------------------------------------------------------------------------------------------------------------
n0_save_restore_α:                                                            jmp   n1_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_deferred_α:     lea              rax, [rip + stack_init_body];        jmp   rax
                                                                              jmp   n2_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n2_save_restore_α:
stack_init_alpha:       sub              rsp, 48
                        mov              rax, qword ptr [r9 + 0]              # stack_init
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        lea              r10, [rip + stack_init_gamma]
                        lea              r11, [rip + stack_init_omega]
                        push             r11
                        push             r10
                        lea              rax, [rip + stack_init_body];        jmp   rax
stack_init_gamma:       mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
stack_init_omega:       mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_push_α
proc_stack_push_α:
#-----------------------------------------------------------------------------------------------------------------------
n9_save_restore_α:                                                            jmp   n10_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n10_goto_deferred_α:    lea              rax, [rip + stack_push_body];        jmp   rax
                                                                              jmp   n11_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n11_save_restore_α:
stack_push_alpha:       sub              rsp, 64
                        mov              rax, qword ptr [r9 + 16]             # stack_push
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx17_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 32]             # x
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx17_41
.Lx17_10:               mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx17_41:               lea              r10, [rip + stack_push_gamma]
                        lea              r11, [rip + stack_push_omega]
                        push             r11
                        push             r10
                        lea              rax, [rip + stack_push_body];        jmp   rax
stack_push_gamma:       mov              rdi, qword ptr [r9 + 16]             # stack_push
                        mov              rsi, qword ptr [r9 + 24]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        cmp              rdx, 0;                              jbe   .Lx17_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 32], rax             # x
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx17_110
.Lx17_80:               mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 40], rax
.Lx17_110:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 64
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
stack_push_omega:       mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax             # stack_push
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        cmp              rdx, 0;                              jbe   .Lx17_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 32], rax             # x
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx17_180
.Lx17_150:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 40], rax
.Lx17_180:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_pop_α
proc_stack_pop_α:
#-----------------------------------------------------------------------------------------------------------------------
n18_save_restore_α:                                                           jmp   n19_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n19_goto_deferred_α:    lea              rax, [rip + stack_pop_body];         jmp   rax
                                                                              jmp   n20_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n20_save_restore_α:
stack_pop_alpha:        sub              rsp, 64
                        mov              rax, qword ptr [r9 + 48]             # stack_pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx26_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 64]             # var
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
.Lx26_41:               lea              r10, [rip + stack_pop_gamma]
                        lea              r11, [rip + stack_pop_omega]
                        push             r11
                        push             r10
                        lea              rax, [rip + stack_pop_body];         jmp   rax
stack_pop_gamma:        mov              rdi, qword ptr [r9 + 48]             # stack_pop
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 56], rax
                        cmp              rdx, 0;                              jbe   .Lx26_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 64], rax             # var
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx26_110
.Lx26_80:               mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 72], rax
.Lx26_110:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 64
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
stack_pop_omega:        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 48], rax             # stack_pop
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 56], rax
                        cmp              rdx, 0;                              jbe   .Lx26_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 64], rax             # var
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx26_180
.Lx26_150:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 72], rax
.Lx26_180:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_peek_α
proc_stack_peek_α:
#-----------------------------------------------------------------------------------------------------------------------
n27_save_restore_α:                                                           jmp   n28_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n28_goto_deferred_α:    lea              rax, [rip + stack_peek_body];        jmp   rax
                                                                              jmp   n29_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n29_save_restore_α:
stack_peek_alpha:       sub              rsp, 48
                        mov              rax, qword ptr [r9 + 80]             # stack_peek
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        lea              r10, [rip + stack_peek_gamma]
                        lea              r11, [rip + stack_peek_omega]
                        push             r11
                        push             r10
                        lea              rax, [rip + stack_peek_body];        jmp   rax
stack_peek_gamma:       mov              rdi, qword ptr [r9 + 80]
                        mov              rsi, qword ptr [r9 + 88]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
stack_peek_omega:       mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_top_α
proc_stack_top_α:
#-----------------------------------------------------------------------------------------------------------------------
n36_save_restore_α:                                                           jmp   n37_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n37_goto_deferred_α:    lea              rax, [rip + stack_top_body];         jmp   rax
                                                                              jmp   n38_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n38_save_restore_α:
stack_top_alpha:        sub              rsp, 48
                        mov              rax, qword ptr [r9 + 96]             # stack_top
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        lea              r10, [rip + stack_top_gamma]
                        lea              r11, [rip + stack_top_omega]
                        push             r11
                        push             r10
                        lea              rax, [rip + stack_top_body];         jmp   rax
stack_top_gamma:        mov              rdi, qword ptr [r9 + 96]
                        mov              rsi, qword ptr [r9 + 104]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
stack_top_omega:        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_stack_depth_α
proc_stack_depth_α:
#-----------------------------------------------------------------------------------------------------------------------
n45_save_restore_α:                                                           jmp   n46_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n46_goto_deferred_α:    lea              rax, [rip + stack_depth_body];       jmp   rax
                                                                              jmp   n47_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n47_save_restore_α:
stack_depth_alpha:      sub              rsp, 64
                        mov              rax, qword ptr [r9 + 112]            # stack_depth
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 112], 0
                        mov              qword ptr [r9 + 120], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx53_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 128]            # sd
                        mov              qword ptr [r9 + 128], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 136]
                        mov              qword ptr [r9 + 136], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx53_41
.Lx53_10:               mov              rax, qword ptr [r9 + 128]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
.Lx53_41:               lea              r10, [rip + stack_depth_gamma]
                        lea              r11, [rip + stack_depth_omega]
                        push             r11
                        push             r10
                        lea              rax, [rip + stack_depth_body];       jmp   rax
stack_depth_gamma:      mov              rdi, qword ptr [r9 + 112]            # stack_depth
                        mov              rsi, qword ptr [r9 + 120]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rdx, 0;                              jbe   .Lx53_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 128], rax            # sd
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx53_110
.Lx53_80:               mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
.Lx53_110:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 64
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
stack_depth_omega:      mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax            # stack_depth
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rdx, 0;                              jbe   .Lx53_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 128], rax            # sd
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx53_180
.Lx53_150:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
.Lx53_180:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
                                                                              jmp   main_ω
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lclassspec0:           .string          "slink(snext,sval)"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lclassspec0]
                        call             record_register@PLT
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__stack_init"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + stack_init_body]
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
.Lstartup_pname1:       .string          "LBL__stack_push"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + stack_push_body]
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
.Lstartup_pname2:       .string          "LBL__stack_pop"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname2]
                        lea              rsi, [rip + stack_pop_body]
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
.Lstartup_pname3:       .string          "LBL__stack_peek"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname3]
                        lea              rsi, [rip + stack_peek_body]
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
.Lstartup_pname4:       .string          "LBL__stack_top"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname4]
                        lea              rsi, [rip + stack_top_body]
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
.Lstartup_pname5:       .string          "LBL__stack_depth"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname5]
                        lea              rsi, [rip + stack_depth_body]
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
.Lgvan0:                .string          "stack_init"
.Lgvan1:                .string          "stack_push"
.Lgvan2:                .string          "x"
.Lgvan3:                .string          "stack_pop"
.Lgvan4:                .string          "var"
.Lgvan5:                .string          "stack_peek"
.Lgvan6:                .string          "stack_top"
.Lgvan7:                .string          "stack_depth"
.Lgvan8:                .string          "sd"
.Lgvan9:                .string          "stk"
.Lgvan10:               .string          "dummy"
.Lgvan11:               .string          "myvar"
.Lgvan12:               .string          "subject"
.Lgvan13:               .string          "w1"
.Lgvan14:               .string          "w2"
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
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 15
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 15
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         <stmt 1, line 1: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_begin_α:                                                        jmp   n55_statement_end_α
n54_statement_begin_β:                                                        jmp   n56_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_end_α:                                                          jmp   n56_statement_begin_α
#=======================================================================================================================
#         stack_push(99)
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_begin_α:                                                        jmp   n57_lit_string_α
n56_statement_begin_β:                                                        jmp   n60_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx340_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n58_call_α
.Lx340_0:               .quad            .Lx340_0_s
.Lx340_0_s:             .string          "slink(snext,sval)"
#-----------------------------------------------------------------------------------------------------------------------
n58_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd342:            .string          "DATA"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd342]
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
                        cmp              eax, 104;                            jne   .Lx341_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n56_statement_begin_β
.Lx341_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n59_statement_end_α
n58_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n56_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_end_α:    add              rsp, 32;                             jmp   n60_statement_begin_α
#=======================================================================================================================
#         stack_push(99)
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_begin_α:                                                        jmp   n61_statement_end_α
n60_statement_begin_β:                                                        jmp   n62_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_end_α:                                                          jmp   n62_statement_begin_α
#=======================================================================================================================
#         OUTPUT = myvar                  ;* 99
#-----------------------------------------------------------------------------------------------------------------------
n62_statement_begin_α:                                                        jmp   n63_func_activate_α
n62_statement_begin_β:                                                        jmp   n69_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n63_func_activate_α:    mov              rdi, qword ptr [rip + .Lx352_0]
                        mov              rsi, qword ptr [rip + .Lx352_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_init_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n64_statement_end_α
n63_func_activate_β:                                                          jmp   n62_statement_begin_β
.Lx352_0:               .quad            .Lx352_0_s
.Lx352_0_s:             .string          "stack_init"
.Lx352_1:               .quad            .Lx352_1_s
.Lx352_1_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_end_α:                                                          jmp   n69_statement_begin_α
#=======================================================================================================================
# stack_init  <stmt 5, line 54: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
stack_init_body:                                                              jmp   n66_lit_string_α
n65_statement_begin_β:                                                        jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n66_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx357_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n67_assign_α
.Lx357_0:               .quad            .Lx357_0_s
.Lx357_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n67_assign_α:           mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # stk
                        mov              qword ptr [r9 + 152], rdx;           jmp   n68_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_end_α:    add              rsp, 16;                             jmp   RETURN
#=======================================================================================================================
# stack_init_end  <stmt 6, line 56: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_begin_α:                                                        jmp   n70_statement_end_α
n69_statement_begin_β:                                                        jmp   n71_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_end_α:                                                          jmp   n71_statement_begin_α
#=======================================================================================================================
#         subject = 'hello world'
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_begin_α:                                                        jmp   n72_func_activate_α
n71_statement_begin_β:                                                        jmp   n85_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n72_func_activate_α:    mov              rdi, qword ptr [rip + .Lx368_0]
                        mov              rsi, qword ptr [rip + .Lx368_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_push_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n73_statement_end_α
n72_func_activate_β:                                                          jmp   n71_statement_begin_β
.Lx368_0:               .quad            .Lx368_0_s
.Lx368_0_s:             .string          "stack_push"
.Lx368_1:               .quad            .Lx368_1_s
.Lx368_1_s:             .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_end_α:                                                          jmp   n85_statement_begin_α
#=======================================================================================================================
# stack_push  <stmt 8, line 58: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
stack_push_body:                                                              jmp   n75_var_α
n74_statement_begin_β:                                                        jmp   n80_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n75_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n76_var_α
#-----------------------------------------------------------------------------------------------------------------------
n76_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]             # x
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n77_call_α
n76_var_β:              add              rsp, 16
                        add              rsp, 16;                             jmp   n74_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n77_call_α:             sub              rsp, 16
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
.Lrkfnzd376:            .string          "slink"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd376]
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
                        cmp              eax, 104;                            jne   .Lx375_240
                        add              rsp, 16;                             jmp   n76_var_β
.Lx375_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n78_assign_α
n77_call_β:             add              rsp, 16;                             jmp   n76_var_β
#-----------------------------------------------------------------------------------------------------------------------
n78_assign_α:           mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # stk
                        mov              qword ptr [r9 + 152], rdx;           jmp   n79_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_end_α:    add              rsp, 48;                             jmp   n80_statement_begin_α
#=======================================================================================================================
#         stack_push(w1)
#-----------------------------------------------------------------------------------------------------------------------
n80_statement_begin_α:                                                        jmp   n81_var_α
n80_statement_begin_β:                                                        jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n81_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n82_field_var_α
#-----------------------------------------------------------------------------------------------------------------------
n82_field_var_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx383_0]
                        mov              rsi, qword ptr [rsp + 16]            # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_field_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              eax, 104;                            jne   .Lx383_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n80_statement_begin_β
.Lx383_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n83_assign_α
.Lx383_0:               .quad            .Lx383_0_s
.Lx383_0_s:             .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n83_assign_α:           mov              rax, qword ptr [rsp + 0]             # field_var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax             # stack_push
                        mov              qword ptr [r9 + 24], rdx;            jmp   n84_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n84_statement_end_α:    add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# stack_push_end  <stmt 10, line 61: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n85_statement_begin_α:                                                        jmp   n86_statement_end_α
n85_statement_begin_β:                                                        jmp   n87_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n86_statement_end_α:                                                          jmp   n87_statement_begin_α
#=======================================================================================================================
#         OUTPUT = stack_pop()            ;* hello
#-----------------------------------------------------------------------------------------------------------------------
n87_statement_begin_α:                                                        jmp   n88_func_activate_α
n87_statement_begin_β:                                                        jmp   n125_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n88_func_activate_α:    mov              rdi, qword ptr [rip + .Lx394_0]
                        mov              rsi, qword ptr [rip + .Lx394_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_pop_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n89_statement_end_α
n88_func_activate_β:                                                          jmp   n87_statement_begin_β
.Lx394_0:               .quad            .Lx394_0_s
.Lx394_0_s:             .string          "stack_pop"
.Lx394_1:               .quad            .Lx394_1_s
.Lx394_1_s:             .string          "var"
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_end_α:                                                          jmp   n125_statement_begin_α
#=======================================================================================================================
# stack_pop  <stmt 12, line 63: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
stack_pop_body:                                                               jmp   n91_var_α
n90_statement_begin_β:                                                        jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n91_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n92_call_α
#-----------------------------------------------------------------------------------------------------------------------
n92_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd401:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd401]
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
                        cmp              eax, 104;                            jne   .Lx400_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n90_statement_begin_β
.Lx400_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n93_statement_end_α
n92_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n90_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n93_statement_end_α:    add              rsp, 32;                             jmp   n94_statement_begin_α
#=======================================================================================================================
#         <stmt 13, line 64: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n94_statement_begin_α:                                                        jmp   n95_var_α
n94_statement_begin_β:                                                        jmp   n108_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n95_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # var
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n96_call_α
#-----------------------------------------------------------------------------------------------------------------------
n96_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd408:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd408]
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
                        cmp              eax, 104;                            jne   .Lx407_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n94_statement_begin_β
.Lx407_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n97_statement_end_α
n96_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n94_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n97_statement_end_α:    add              rsp, 32;                             jmp   n98_statement_begin_α
#=======================================================================================================================
#         <stmt 14, line 65: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n98_statement_begin_α:                                                        jmp   n99_var_α
n98_statement_begin_β:                                                        jmp   n103_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n99_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n100_call_α
#-----------------------------------------------------------------------------------------------------------------------
n100_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd415:            .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd415]
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
                        cmp              eax, 104;                            jne   .Lx414_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n98_statement_begin_β
.Lx414_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n101_assign_α
n100_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n98_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n101_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # stack_pop
                        mov              qword ptr [r9 + 56], rdx;            jmp   n102_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:   add              rsp, 32;                             jmp   n103_statement_begin_α
#=======================================================================================================================
#         <stmt 15, line 66: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_begin_α:                                                       jmp   n104_var_α
n103_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n104_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n105_call_α
#-----------------------------------------------------------------------------------------------------------------------
n105_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd423:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd423]
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
                        cmp              eax, 104;                            jne   .Lx422_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n103_statement_begin_β
.Lx422_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n106_assign_α
n105_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n103_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n106_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # stk
                        mov              qword ptr [r9 + 152], rdx;           jmp   n107_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n107_statement_end_α:   add              rsp, 32;                             jmp   RETURN
#=======================================================================================================================
# stack_pop1  <stmt 16, line 67: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n108_statement_begin_α:                                                       jmp   n109_var_α
n108_statement_begin_β:                                                       jmp   n115_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n109_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # var
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n110_call_α
#-----------------------------------------------------------------------------------------------------------------------
n110_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd431:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd431]
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
                        cmp              eax, 104;                            jne   .Lx430_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n108_statement_begin_β
.Lx430_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n111_var_α
n110_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n108_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n111_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n112_call_α
n111_var_β:             add              rsp, 16
                        add              rsp, 32;                             jmp   n108_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n112_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd434:            .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd434]
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
                        cmp              eax, 104;                            jne   .Lx433_240
                        add              rsp, 16;                             jmp   n111_var_β
.Lx433_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n113_assign_var_α
n112_call_β:            add              rsp, 16;                             jmp   n111_var_β
#-----------------------------------------------------------------------------------------------------------------------
n113_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]            # call
                        mov              rsi, qword ptr [rsp + 56]
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
                        cmp              eax, 104;                            jne   .Lx435_240
                        add              rsp, 32;                             jmp   n111_var_β
.Lx435_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n114_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n114_statement_end_α:   add              rsp, 80;                             jmp   n115_statement_begin_α
#=======================================================================================================================
#         <stmt 17, line 68: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n115_statement_begin_α:                                                       jmp   n116_var_α
n115_statement_begin_β:                                                       jmp   n120_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n116_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n117_call_α
#-----------------------------------------------------------------------------------------------------------------------
n117_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd442:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd442]
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
                        cmp              eax, 104;                            jne   .Lx441_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n115_statement_begin_β
.Lx441_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n118_assign_α
n117_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n115_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n118_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # stk
                        mov              qword ptr [r9 + 152], rdx;           jmp   n119_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n119_statement_end_α:   add              rsp, 32;                             jmp   n120_statement_begin_α
#=======================================================================================================================
#         <stmt 18, line 69: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n120_statement_begin_α:                                                       jmp   n121_lit_string_α
n120_statement_begin_β:                                                       jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n121_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx448_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n122_call_α
.Lx448_0:               .quad            .Lx448_0_s
.Lx448_0_s:             .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n122_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd450:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd450]
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
                        cmp              eax, 104;                            jne   .Lx449_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n120_statement_begin_β
.Lx449_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n123_assign_α
n122_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n120_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n123_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # stack_pop
                        mov              qword ptr [r9 + 56], rdx;            jmp   n124_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n124_statement_end_α:   add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# stack_pop_end  <stmt 19, line 71: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n125_statement_begin_α:                                                       jmp   n126_statement_end_α
n125_statement_begin_β:                                                       jmp   n127_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n126_statement_end_α:                                                         jmp   n127_statement_begin_α
#=======================================================================================================================
#         <stmt 20, line 72: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n127_statement_begin_α:                                                       jmp   n128_func_activate_α
n127_statement_begin_β:                                                       jmp   n139_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n128_func_activate_α:   mov              rdi, qword ptr [rip + .Lx461_0]
                        mov              rsi, qword ptr [rip + .Lx461_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_peek_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n129_statement_end_α
n128_func_activate_β:                                                         jmp   n127_statement_begin_β
.Lx461_0:               .quad            .Lx461_0_s
.Lx461_0_s:             .string          "stack_peek"
.Lx461_1:               .quad            .Lx461_1_s
.Lx461_1_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n129_statement_end_α:                                                         jmp   n139_statement_begin_α
#=======================================================================================================================
# stack_peek  <stmt 21, line 73: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
stack_peek_body:                                                              jmp   n131_var_α
n130_statement_begin_β:                                                       jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n131_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n132_call_α
#-----------------------------------------------------------------------------------------------------------------------
n132_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd468:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd468]
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
                        cmp              eax, 104;                            jne   .Lx467_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n130_statement_begin_β
.Lx467_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n133_statement_end_α
n132_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n130_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n133_statement_end_α:   add              rsp, 32;                             jmp   n134_statement_begin_α
#=======================================================================================================================
#         <stmt 22, line 74: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n134_statement_begin_α:                                                       jmp   n135_var_α
n134_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n135_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n136_call_α
#-----------------------------------------------------------------------------------------------------------------------
n136_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd475:            .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd475]
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
                        cmp              eax, 104;                            jne   .Lx474_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n134_statement_begin_β
.Lx474_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n137_assign_α
n136_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n134_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n137_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax             # stack_peek
                        mov              qword ptr [r9 + 88], rdx;            jmp   n138_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n138_statement_end_α:   add              rsp, 32;                             jmp   RETURN
#=======================================================================================================================
# stack_peek_end  <stmt 23, line 76: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n139_statement_begin_α:                                                       jmp   n140_statement_end_α
n139_statement_begin_β:                                                       jmp   n141_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n140_statement_end_α:                                                         jmp   n141_statement_begin_α
#=======================================================================================================================
#         <stmt 24, line 77: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n141_statement_begin_α:                                                       jmp   n142_func_activate_α
n141_statement_begin_β:                                                       jmp   n153_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n142_func_activate_α:   mov              rdi, qword ptr [rip + .Lx486_0]
                        mov              rsi, qword ptr [rip + .Lx486_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_top_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n143_statement_end_α
n142_func_activate_β:                                                         jmp   n141_statement_begin_β
.Lx486_0:               .quad            .Lx486_0_s
.Lx486_0_s:             .string          "stack_top"
.Lx486_1:               .quad            .Lx486_1_s
.Lx486_1_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n143_statement_end_α:                                                         jmp   n153_statement_begin_α
#=======================================================================================================================
# stack_top  <stmt 25, line 78: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
stack_top_body:                                                               jmp   n145_var_α
n144_statement_begin_β:                                                       jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n145_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n146_call_α
#-----------------------------------------------------------------------------------------------------------------------
n146_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd493:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd493]
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
                        cmp              eax, 104;                            jne   .Lx492_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n144_statement_begin_β
.Lx492_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n147_statement_end_α
n146_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n144_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n147_statement_end_α:   add              rsp, 32;                             jmp   n148_statement_begin_α
#=======================================================================================================================
#         <stmt 26, line 79: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n148_statement_begin_α:                                                       jmp   n149_var_α
n148_statement_begin_β:                                                       jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n149_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n150_field_var_α
#-----------------------------------------------------------------------------------------------------------------------
n150_field_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx499_0]
                        mov              rsi, qword ptr [rsp + 16]            # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_field_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              eax, 104;                            jne   .Lx499_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n148_statement_begin_β
.Lx499_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n151_assign_α
.Lx499_0:               .quad            .Lx499_0_s
.Lx499_0_s:             .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n151_assign_α:          mov              rax, qword ptr [rsp + 0]             # field_var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax             # stack_top
                        mov              qword ptr [r9 + 104], rdx;           jmp   n152_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n152_statement_end_α:   add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# stack_top_end  <stmt 27, line 81: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n153_statement_begin_α:                                                       jmp   n154_statement_end_α
n153_statement_begin_β:                                                       jmp   n155_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n154_statement_end_α:                                                         jmp   n155_statement_begin_α
#=======================================================================================================================
#         <stmt 28, line 82: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n155_statement_begin_α:                                                       jmp   n156_func_activate_α
n155_statement_begin_β:                                                       jmp   n181_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n156_func_activate_α:   mov              rdi, qword ptr [rip + .Lx510_0]
                        mov              rsi, qword ptr [rip + .Lx510_1]
                        mov              edx, 1
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_stack_depth_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n157_statement_end_α
n156_func_activate_β:                                                         jmp   n155_statement_begin_β
.Lx510_0:               .quad            .Lx510_0_s
.Lx510_0_s:             .string          "stack_depth"
.Lx510_1:               .quad            .Lx510_1_s
.Lx510_1_s:             .string          "sd"
#-----------------------------------------------------------------------------------------------------------------------
n157_statement_end_α:                                                         jmp   n181_statement_begin_α
#=======================================================================================================================
# stack_depth  <stmt 29, line 83: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
stack_depth_body:                                                             jmp   n159_lit_integer_α
n158_statement_begin_β:                                                       jmp   n162_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n159_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx515_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n160_assign_α
.Lx515_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n160_assign_α:          mov              rax, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # stack_depth
                        mov              qword ptr [r9 + 120], rdx;           jmp   n161_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n161_statement_end_α:   add              rsp, 16;                             jmp   n162_statement_begin_α
#=======================================================================================================================
#         <stmt 30, line 84: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n162_statement_begin_α:                                                       jmp   n163_var_α
n162_statement_begin_β:                                                       jmp   n166_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n163_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n164_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n164_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax            # sd
                        mov              qword ptr [r9 + 136], rdx;           jmp   n165_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n165_statement_end_α:   add              rsp, 16;                             jmp   n166_statement_begin_α
#=======================================================================================================================
# stk_dep0  <stmt 31, line 85: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n166_statement_begin_α:                                                       jmp   n167_var_α
n166_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n167_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 128]            # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n168_call_α
#-----------------------------------------------------------------------------------------------------------------------
n168_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd529:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd529]
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
                        cmp              eax, 104;                            jne   .Lx528_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n166_statement_begin_β
.Lx528_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n169_statement_end_α
n168_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n166_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n169_statement_end_α:   add              rsp, 32;                             jmp   n170_statement_begin_α
#=======================================================================================================================
#         <stmt 32, line 86: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n170_statement_begin_α:                                                       jmp   n171_var_α
n170_statement_begin_β:                                                       jmp   n176_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n171_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 112]            # stack_depth
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n172_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n172_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx535_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n173_binop_α
n172_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n170_statement_begin_β
.Lx535_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n173_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx536_240
                        add              rsp, 16;                             jmp   n172_lit_integer_β
.Lx536_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n174_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n174_assign_α:          mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # stack_depth
                        mov              qword ptr [r9 + 120], rdx;           jmp   n175_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n175_statement_end_α:   add              rsp, 48;                             jmp   n176_statement_begin_α
#=======================================================================================================================
#         <stmt 33, line 87: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n176_statement_begin_α:                                                       jmp   n177_var_α
n176_statement_begin_β:                                                       jmp   n166_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n177_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 128]            # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n178_call_α
#-----------------------------------------------------------------------------------------------------------------------
n178_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd544:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd544]
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
                        cmp              eax, 104;                            jne   .Lx543_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n176_statement_begin_β
.Lx543_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n179_assign_α
n178_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n176_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n179_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax            # sd
                        mov              qword ptr [r9 + 136], rdx;           jmp   n180_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n180_statement_end_α:   add              rsp, 32;                             jmp   n166_statement_begin_α
#=======================================================================================================================
# stack_depth_end  <stmt 34, line 89: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n181_statement_begin_α:                                                       jmp   n182_statement_end_α
n181_statement_begin_β:                                                       jmp   n183_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n182_statement_end_α:                                                         jmp   n183_statement_begin_α
#=======================================================================================================================
#         <stmt 35, line 87: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n183_statement_begin_α:                                                       jmp   n184_statement_end_α
n183_statement_begin_β:                                                       jmp   n185_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n184_statement_end_α:                                                         jmp   n185_statement_begin_α
#=======================================================================================================================
#         <stmt 36, line 90: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n185_statement_begin_α:                                                       jmp   n186_lit_string_α
n185_statement_begin_β:                                                       jmp   n190_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n186_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx558_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n187_lit_integer_α
.Lx558_0:               .quad            .Lx558_0_s
.Lx558_0_s:             .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n187_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx559_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n188_call_α
n187_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n185_statement_begin_β
.Lx559_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n188_call_α:            sub              rsp, 16
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
.Lrkfnzd561:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd561]
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
                        cmp              eax, 104;                            jne   .Lx560_240
                        add              rsp, 16;                             jmp   n187_lit_integer_β
.Lx560_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n189_statement_end_α
n188_call_β:            add              rsp, 16;                             jmp   n187_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n189_statement_end_α:   add              rsp, 48;                             jmp   n190_statement_begin_α
#=======================================================================================================================
#         <stmt 37, line 90: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n190_statement_begin_α:                                                       jmp   n191_statement_end_α
n190_statement_begin_β:                                                       jmp   n192_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n191_statement_end_α:                                                         jmp   n192_statement_begin_α
#=======================================================================================================================
#         <stmt 38, line 93: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n192_statement_begin_α:                                                       jmp   n193_call_α
n192_statement_begin_β:                                                       jmp   n195_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n193_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig571z]
                        lea              rax, [rip + stack_init_alpha];       jmp   rax
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
.Lx571_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx571_240
                        add              rsp, 16;                             jmp   n192_statement_begin_β
.Lx571_240:                                                                   jmp   n194_statement_end_α
n193_call_β:                                                                  jmp   n192_statement_begin_β
.Lx571_0:               .quad            .Lx571_0_s
.Lx571_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n194_statement_end_α:   add              rsp, 16;                             jmp   n195_statement_begin_α
#=======================================================================================================================
#         <stmt 39, line 94: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n195_statement_begin_α:                                                       jmp   n196_lit_string_α
n195_statement_begin_β:                                                       jmp   n199_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n196_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx576_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n197_call_α
.Lx576_0:               .quad            .Lx576_0_s
.Lx576_0_s:             .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
n197_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig578z]
                        lea              rax, [rip + stack_push_alpha];       jmp   rax
.Lsig578z:              .quad            1
                        .quad            .Lx578_2
                        .quad            .Lx578_2
                        .quad            16
.Lx578_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx578_29
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
.Lx578_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx578_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n195_statement_begin_β
.Lx578_240:                                                                   jmp   n198_statement_end_α
n197_call_β:                                                                  jmp   n195_statement_begin_β
.Lx578_0:               .quad            .Lx578_0_s
.Lx578_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n198_statement_end_α:   add              rsp, 32;                             jmp   n199_statement_begin_α
#=======================================================================================================================
#         <stmt 40, line 95: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n199_statement_begin_α:                                                       jmp   n200_lit_string_α
n199_statement_begin_β:                                                       jmp   n203_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n200_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx583_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n201_call_α
.Lx583_0:               .quad            .Lx583_0_s
.Lx583_0_s:             .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n201_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig585z]
                        lea              rax, [rip + stack_push_alpha];       jmp   rax
.Lsig585z:              .quad            1
                        .quad            .Lx585_2
                        .quad            .Lx585_2
                        .quad            16
.Lx585_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx585_29
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
.Lx585_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx585_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n199_statement_begin_β
.Lx585_240:                                                                   jmp   n202_statement_end_α
n201_call_β:                                                                  jmp   n199_statement_begin_β
.Lx585_0:               .quad            .Lx585_0_s
.Lx585_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n202_statement_end_α:   add              rsp, 32;                             jmp   n203_statement_begin_α
#=======================================================================================================================
#         <stmt 41, line 96: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n203_statement_begin_α:                                                       jmp   n204_lit_string_α
n203_statement_begin_β:                                                       jmp   n207_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n204_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx590_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n205_call_α
.Lx590_0:               .quad            .Lx590_0_s
.Lx590_0_s:             .string          "c"
#-----------------------------------------------------------------------------------------------------------------------
n205_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig592z]
                        lea              rax, [rip + stack_push_alpha];       jmp   rax
.Lsig592z:              .quad            1
                        .quad            .Lx592_2
                        .quad            .Lx592_2
                        .quad            16
.Lx592_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx592_29
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
.Lx592_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx592_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n203_statement_begin_β
.Lx592_240:                                                                   jmp   n206_statement_end_α
n205_call_β:                                                                  jmp   n203_statement_begin_β
.Lx592_0:               .quad            .Lx592_0_s
.Lx592_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n206_statement_end_α:   add              rsp, 32;                             jmp   n207_statement_begin_α
#=======================================================================================================================
#         <stmt 42, line 97: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n207_statement_begin_α:                                                       jmp   n208_call_α
n207_statement_begin_β:                                                       jmp   n211_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n208_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig598z]
                        lea              rax, [rip + stack_depth_alpha];      jmp   rax
.Lsig598z:              .quad            0
                        .quad            .Lx598_2
                        .quad            .Lx598_2
.Lx598_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx598_29
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
.Lx598_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx598_240
                        add              rsp, 16;                             jmp   n207_statement_begin_β
.Lx598_240:                                                                   jmp   n209_assign_α
n208_call_β:                                                                  jmp   n207_statement_begin_β
.Lx598_0:               .quad            .Lx598_0_s
.Lx598_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n209_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx599_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n210_statement_end_α
.Lx599_0:               .quad            .Lx599_0_s
.Lx599_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n210_statement_end_α:   add              rsp, 16;                             jmp   n211_statement_begin_α
#=======================================================================================================================
#         <stmt 43, line 98: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n211_statement_begin_α:                                                       jmp   n212_call_α
n211_statement_begin_β:                                                       jmp   n215_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n212_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig605z]
                        lea              rax, [rip + stack_pop_alpha];        jmp   rax
.Lsig605z:              .quad            0
                        .quad            .Lx605_2
                        .quad            .Lx605_2
.Lx605_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx605_29
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
.Lx605_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx605_240
                        add              rsp, 16;                             jmp   n211_statement_begin_β
.Lx605_240:                                                                   jmp   n213_assign_α
n212_call_β:                                                                  jmp   n211_statement_begin_β
.Lx605_0:               .quad            .Lx605_0_s
.Lx605_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n213_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx606_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n214_statement_end_α
.Lx606_0:               .quad            .Lx606_0_s
.Lx606_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n214_statement_end_α:   add              rsp, 16;                             jmp   n215_statement_begin_α
#=======================================================================================================================
#         <stmt 44, line 99: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n215_statement_begin_α:                                                       jmp   n216_call_α
n215_statement_begin_β:                                                       jmp   n219_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n216_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig612z]
                        lea              rax, [rip + stack_pop_alpha];        jmp   rax
.Lsig612z:              .quad            0
                        .quad            .Lx612_2
                        .quad            .Lx612_2
.Lx612_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx612_29
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
.Lx612_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx612_240
                        add              rsp, 16;                             jmp   n215_statement_begin_β
.Lx612_240:                                                                   jmp   n217_assign_α
n216_call_β:                                                                  jmp   n215_statement_begin_β
.Lx612_0:               .quad            .Lx612_0_s
.Lx612_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n217_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx613_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n218_statement_end_α
.Lx613_0:               .quad            .Lx613_0_s
.Lx613_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n218_statement_end_α:   add              rsp, 16;                             jmp   n219_statement_begin_α
#=======================================================================================================================
#         <stmt 45, line 100: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n219_statement_begin_α:                                                       jmp   n220_call_α
n219_statement_begin_β:                                                       jmp   n223_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n220_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig619z]
                        lea              rax, [rip + stack_depth_alpha];      jmp   rax
.Lsig619z:              .quad            0
                        .quad            .Lx619_2
                        .quad            .Lx619_2
.Lx619_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx619_29
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
.Lx619_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx619_240
                        add              rsp, 16;                             jmp   n219_statement_begin_β
.Lx619_240:                                                                   jmp   n221_assign_α
n220_call_β:                                                                  jmp   n219_statement_begin_β
.Lx619_0:               .quad            .Lx619_0_s
.Lx619_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n221_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx620_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n222_statement_end_α
.Lx620_0:               .quad            .Lx620_0_s
.Lx620_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n222_statement_end_α:   add              rsp, 16;                             jmp   n223_statement_begin_α
#=======================================================================================================================
#         <stmt 46, line 101: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n223_statement_begin_α:                                                       jmp   n224_call_α
n223_statement_begin_β:                                                       jmp   n227_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n224_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig626z]
                        lea              rax, [rip + stack_pop_alpha];        jmp   rax
.Lsig626z:              .quad            0
                        .quad            .Lx626_2
                        .quad            .Lx626_2
.Lx626_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx626_29
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
.Lx626_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx626_240
                        add              rsp, 16;                             jmp   n223_statement_begin_β
.Lx626_240:                                                                   jmp   n225_assign_α
n224_call_β:                                                                  jmp   n223_statement_begin_β
.Lx626_0:               .quad            .Lx626_0_s
.Lx626_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n225_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx627_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n226_statement_end_α
.Lx627_0:               .quad            .Lx627_0_s
.Lx627_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n226_statement_end_α:   add              rsp, 16;                             jmp   n227_statement_begin_α
#=======================================================================================================================
#         <stmt 47, line 102: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n227_statement_begin_α:                                                       jmp   n228_call_α
n227_statement_begin_β:                                                       jmp   n231_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n228_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig633z]
                        lea              rax, [rip + stack_depth_alpha];      jmp   rax
.Lsig633z:              .quad            0
                        .quad            .Lx633_2
                        .quad            .Lx633_2
.Lx633_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx633_29
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
.Lx633_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx633_240
                        add              rsp, 16;                             jmp   n227_statement_begin_β
.Lx633_240:                                                                   jmp   n229_assign_α
n228_call_β:                                                                  jmp   n227_statement_begin_β
.Lx633_0:               .quad            .Lx633_0_s
.Lx633_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n229_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx634_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n230_statement_end_α
.Lx634_0:               .quad            .Lx634_0_s
.Lx634_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n230_statement_end_α:   add              rsp, 16;                             jmp   n231_statement_begin_α
#=======================================================================================================================
#         <stmt 48, line 102: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n231_statement_begin_α:                                                       jmp   n232_statement_end_α
n231_statement_begin_β:                                                       jmp   n233_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n232_statement_end_α:                                                         jmp   n233_statement_begin_α
#=======================================================================================================================
#         <stmt 49, line 105: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n233_statement_begin_α:                                                       jmp   n234_call_α
n233_statement_begin_β:                                                       jmp   n236_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n234_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig644z]
                        lea              rax, [rip + stack_pop_alpha];        jmp   rax
.Lsig644z:              .quad            0
                        .quad            .Lx644_2
                        .quad            .Lx644_2
.Lx644_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx644_29
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
.Lx644_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx644_240
                        add              rsp, 16;                             jmp   n233_statement_begin_β
.Lx644_240:                                                                   jmp   n235_statement_end_α
n234_call_β:                                                                  jmp   n233_statement_begin_β
.Lx644_0:               .quad            .Lx644_0_s
.Lx644_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n235_statement_end_α:   add              rsp, 16;                             jmp   n240_statement_begin_α
#=======================================================================================================================
#         <stmt 50, line 106: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n236_statement_begin_α:                                                       jmp   n237_lit_string_α
n236_statement_begin_β:                                                       jmp   n244_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n237_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx649_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n238_assign_α
.Lx649_0:               .quad            .Lx649_0_s
.Lx649_0_s:             .string          "empty ok"
#-----------------------------------------------------------------------------------------------------------------------
n238_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx650_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n239_statement_end_α
.Lx650_0:               .quad            .Lx650_0_s
.Lx650_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n239_statement_end_α:   add              rsp, 16;                             jmp   n244_statement_begin_α
#=======================================================================================================================
# bad_s1  <stmt 51, line 107: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n240_statement_begin_α:                                                       jmp   n241_lit_string_α
n240_statement_begin_β:                                                       jmp   n244_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n241_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx655_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n242_assign_α
.Lx655_0:               .quad            .Lx655_0_s
.Lx655_0_s:             .string          "FAIL: empty pop should FRETURN"
#-----------------------------------------------------------------------------------------------------------------------
n242_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx656_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n243_statement_end_α
.Lx656_0:               .quad            .Lx656_0_s
.Lx656_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n243_statement_end_α:   add              rsp, 16;                             jmp   n244_statement_begin_α
#=======================================================================================================================
# s1  <stmt 52, line 109: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n244_statement_begin_α:                                                       jmp   n245_statement_end_α
n244_statement_begin_β:                                                       jmp   n246_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n245_statement_end_α:                                                         jmp   n246_statement_begin_α
#=======================================================================================================================
#         <stmt 53, line 111: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n246_statement_begin_α:                                                       jmp   n247_call_α
n246_statement_begin_β:                                                       jmp   n249_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n247_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig666z]
                        lea              rax, [rip + stack_init_alpha];       jmp   rax
.Lsig666z:              .quad            0
                        .quad            .Lx666_2
                        .quad            .Lx666_2
.Lx666_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx666_29
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
.Lx666_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx666_240
                        add              rsp, 16;                             jmp   n246_statement_begin_β
.Lx666_240:                                                                   jmp   n248_statement_end_α
n247_call_β:                                                                  jmp   n246_statement_begin_β
.Lx666_0:               .quad            .Lx666_0_s
.Lx666_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n248_statement_end_α:   add              rsp, 16;                             jmp   n249_statement_begin_α
#=======================================================================================================================
#         <stmt 54, line 112: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n249_statement_begin_α:                                                       jmp   n250_lit_string_α
n249_statement_begin_β:                                                       jmp   n253_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n250_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx671_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n251_call_α
.Lx671_0:               .quad            .Lx671_0_s
.Lx671_0_s:             .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n251_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig673z]
                        lea              rax, [rip + stack_push_alpha];       jmp   rax
.Lsig673z:              .quad            1
                        .quad            .Lx673_2
                        .quad            .Lx673_2
                        .quad            16
.Lx673_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx673_29
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
.Lx673_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx673_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n249_statement_begin_β
.Lx673_240:                                                                   jmp   n252_statement_end_α
n251_call_β:                                                                  jmp   n249_statement_begin_β
.Lx673_0:               .quad            .Lx673_0_s
.Lx673_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n252_statement_end_α:   add              rsp, 32;                             jmp   n253_statement_begin_α
#=======================================================================================================================
#         <stmt 55, line 113: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n253_statement_begin_α:                                                       jmp   n254_call_α
n253_statement_begin_β:                                                       jmp   n257_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n254_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig679z]
                        lea              rax, [rip + stack_peek_alpha];       jmp   rax
.Lsig679z:              .quad            0
                        .quad            .Lx679_2
                        .quad            .Lx679_2
.Lx679_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx679_29
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
.Lx679_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx679_240
                        add              rsp, 16;                             jmp   n253_statement_begin_β
.Lx679_240:                                                                   jmp   n255_assign_α
n254_call_β:                                                                  jmp   n253_statement_begin_β
.Lx679_0:               .quad            .Lx679_0_s
.Lx679_0_s:             .string          "stack_peek"
#-----------------------------------------------------------------------------------------------------------------------
n255_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx680_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n256_statement_end_α
.Lx680_0:               .quad            .Lx680_0_s
.Lx680_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n256_statement_end_α:   add              rsp, 16;                             jmp   n257_statement_begin_α
#=======================================================================================================================
#         <stmt 56, line 114: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n257_statement_begin_α:                                                       jmp   n258_call_α
n257_statement_begin_β:                                                       jmp   n261_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n258_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig686z]
                        lea              rax, [rip + stack_depth_alpha];      jmp   rax
.Lsig686z:              .quad            0
                        .quad            .Lx686_2
                        .quad            .Lx686_2
.Lx686_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx686_29
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
.Lx686_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx686_240
                        add              rsp, 16;                             jmp   n257_statement_begin_β
.Lx686_240:                                                                   jmp   n259_assign_α
n258_call_β:                                                                  jmp   n257_statement_begin_β
.Lx686_0:               .quad            .Lx686_0_s
.Lx686_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n259_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx687_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n260_statement_end_α
.Lx687_0:               .quad            .Lx687_0_s
.Lx687_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n260_statement_end_α:   add              rsp, 16;                             jmp   n261_statement_begin_α
#=======================================================================================================================
#         <stmt 57, line 115: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n261_statement_begin_α:                                                       jmp   n262_call_α
n261_statement_begin_β:                                                       jmp   n265_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n262_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig693z]
                        lea              rax, [rip + stack_pop_alpha];        jmp   rax
.Lsig693z:              .quad            0
                        .quad            .Lx693_2
                        .quad            .Lx693_2
.Lx693_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx693_29
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
.Lx693_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx693_240
                        add              rsp, 16;                             jmp   n261_statement_begin_β
.Lx693_240:                                                                   jmp   n263_assign_α
n262_call_β:                                                                  jmp   n261_statement_begin_β
.Lx693_0:               .quad            .Lx693_0_s
.Lx693_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n263_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx694_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n264_statement_end_α
.Lx694_0:               .quad            .Lx694_0_s
.Lx694_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n264_statement_end_α:   add              rsp, 16;                             jmp   n265_statement_begin_α
#=======================================================================================================================
#         <stmt 58, line 115: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n265_statement_begin_α:                                                       jmp   n266_statement_end_α
n265_statement_begin_β:                                                       jmp   n267_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n266_statement_end_α:                                                         jmp   n267_statement_begin_α
#=======================================================================================================================
#         <stmt 59, line 118: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n267_statement_begin_α:                                                       jmp   n268_call_α
n267_statement_begin_β:                                                       jmp   n270_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n268_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig704z]
                        lea              rax, [rip + stack_init_alpha];       jmp   rax
.Lsig704z:              .quad            0
                        .quad            .Lx704_2
                        .quad            .Lx704_2
.Lx704_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx704_29
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
.Lx704_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx704_240
                        add              rsp, 16;                             jmp   n267_statement_begin_β
.Lx704_240:                                                                   jmp   n269_statement_end_α
n268_call_β:                                                                  jmp   n267_statement_begin_β
.Lx704_0:               .quad            .Lx704_0_s
.Lx704_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n269_statement_end_α:   add              rsp, 16;                             jmp   n270_statement_begin_α
#=======================================================================================================================
#         <stmt 60, line 119: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n270_statement_begin_α:                                                       jmp   n271_lit_integer_α
n270_statement_begin_β:                                                       jmp   n274_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n271_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx709_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n272_call_α
.Lx709_0:               .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n272_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig711z]
                        lea              rax, [rip + stack_push_alpha];       jmp   rax
.Lsig711z:              .quad            1
                        .quad            .Lx711_2
                        .quad            .Lx711_2
                        .quad            16
.Lx711_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx711_29
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
.Lx711_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx711_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n270_statement_begin_β
.Lx711_240:                                                                   jmp   n273_statement_end_α
n272_call_β:                                                                  jmp   n270_statement_begin_β
.Lx711_0:               .quad            .Lx711_0_s
.Lx711_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n273_statement_end_α:   add              rsp, 32;                             jmp   n274_statement_begin_α
#=======================================================================================================================
#         <stmt 61, line 120: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n274_statement_begin_α:                                                       jmp   n275_lit_integer_α
n274_statement_begin_β:                                                       jmp   n278_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n275_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx716_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n276_call_α
.Lx716_0:               .quad            99
#-----------------------------------------------------------------------------------------------------------------------
n276_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig718z]
                        lea              rax, [rip + stack_push_alpha];       jmp   rax
.Lsig718z:              .quad            1
                        .quad            .Lx718_2
                        .quad            .Lx718_2
                        .quad            16
.Lx718_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx718_29
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
.Lx718_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx718_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n274_statement_begin_β
.Lx718_240:                                                                   jmp   n277_statement_end_α
n276_call_β:                                                                  jmp   n274_statement_begin_β
.Lx718_0:               .quad            .Lx718_0_s
.Lx718_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n277_statement_end_α:   add              rsp, 32;                             jmp   n278_statement_begin_α
#=======================================================================================================================
#         <stmt 62, line 121: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n278_statement_begin_α:                                                       jmp   n279_lit_string_α
n278_statement_begin_β:                                                       jmp   n282_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n279_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx723_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n280_call_α
.Lx723_0:               .quad            .Lx723_0_s
.Lx723_0_s:             .string          "myvar"
#-----------------------------------------------------------------------------------------------------------------------
n280_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig725z]
                        lea              rax, [rip + stack_pop_alpha];        jmp   rax
.Lsig725z:              .quad            1
                        .quad            .Lx725_2
                        .quad            .Lx725_2
                        .quad            16
.Lx725_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx725_29
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
.Lx725_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx725_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n278_statement_begin_β
.Lx725_240:                                                                   jmp   n281_statement_end_α
n280_call_β:                                                                  jmp   n278_statement_begin_β
.Lx725_0:               .quad            .Lx725_0_s
.Lx725_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n281_statement_end_α:   add              rsp, 32;                             jmp   n282_statement_begin_α
#=======================================================================================================================
#         <stmt 63, line 122: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n282_statement_begin_α:                                                       jmp   n283_var_α
n282_statement_begin_β:                                                       jmp   n286_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n283_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 176]            # myvar
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n284_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n284_assign_α:          mov              rsi, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx731_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n285_statement_end_α
.Lx731_0:               .quad            .Lx731_0_s
.Lx731_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n285_statement_end_α:   add              rsp, 16;                             jmp   n286_statement_begin_α
#=======================================================================================================================
#         <stmt 64, line 122: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n286_statement_begin_α:                                                       jmp   n287_statement_end_α
n286_statement_begin_β:                                                       jmp   n288_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n287_statement_end_α:                                                         jmp   n288_statement_begin_α
#=======================================================================================================================
#         <stmt 65, line 125: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n288_statement_begin_α:                                                       jmp   n289_call_α
n288_statement_begin_β:                                                       jmp   n291_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n289_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig741z]
                        lea              rax, [rip + stack_init_alpha];       jmp   rax
.Lsig741z:              .quad            0
                        .quad            .Lx741_2
                        .quad            .Lx741_2
.Lx741_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx741_29
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
.Lx741_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx741_240
                        add              rsp, 16;                             jmp   n288_statement_begin_β
.Lx741_240:                                                                   jmp   n290_statement_end_α
n289_call_β:                                                                  jmp   n288_statement_begin_β
.Lx741_0:               .quad            .Lx741_0_s
.Lx741_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n290_statement_end_α:   add              rsp, 16;                             jmp   n291_statement_begin_α
#=======================================================================================================================
#         <stmt 66, line 126: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n291_statement_begin_α:                                                       jmp   n292_lit_string_α
n291_statement_begin_β:                                                       jmp   n295_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n292_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx746_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n293_assign_α
.Lx746_0:               .quad            .Lx746_0_s
.Lx746_0_s:             .string          "hello world"
#-----------------------------------------------------------------------------------------------------------------------
n293_assign_α:          mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 192], rax            # subject
                        mov              qword ptr [r9 + 200], rdx;           jmp   n294_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n294_statement_end_α:   add              rsp, 16;                             jmp   n295_statement_begin_α
#=======================================================================================================================
#         <stmt 67, line 127: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n295_statement_begin_α:                                                       jmp   n296_var_α
n295_statement_begin_β:                                                       jmp   n327_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n296_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 192]            # subject
                        mov              rdx, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n297_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n297_match_begin_α:     mov              rdi, qword ptr [rsp + 0]             # var
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
.Lx754_0:               mov              r14d, dword ptr [rbp + -40];         jmp   n298_match_assign_save_α
n297_match_begin_β:     lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx754_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx754_1
                                                                              jmp   .Lx754_0
.Lx754_1:
n297_match_begin_af:    mov              r12, qword ptr [rbp + -8]            # cas_mark
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
                        add              rsp, 16;                             jmp   n327_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n298_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n299_match_break_α
n298_match_assign_save_β:
                        add              rsp, 16;                             jmp   n297_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n299_match_break_α:     sub              rsp, 16
                        movsxd           rcx, r14d
.Lx758_0:               cmp              ecx, r15d;                           jl    .Lx758_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n297_match_begin_β
.Lx758_240:             movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32;                             je    .Lx758_1
                        add              ecx, 1;                              jmp   .Lx758_0
.Lx758_1:               mov              dword ptr [rsp + 0], r14d
                        mov              r14d, ecx;                           jmp   n300_match_assign_cond_α
n299_match_break_β:     mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n297_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n300_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n301_match_lit_α
n300_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n299_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n301_match_lit_α:       mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n300_match_assign_cond_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 32;                             jne   n300_match_assign_cond_β
                        add              r14d, 1;                             jmp   n302_match_assign_save_α
n301_match_lit_β:       sub              r14d, 1;                             jmp   n300_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n302_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n303_match_rem_α
n302_match_assign_save_β:
                        add              rsp, 16;                             jmp   n301_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n303_match_rem_α:       sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d;                          jmp   n304_match_assign_cond_α
n303_match_rem_β:       mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n301_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n304_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n305_match_end_α
n304_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n303_match_rem_β
#-----------------------------------------------------------------------------------------------------------------------
n305_match_end_α:       mov              eax, dword ptr [rbp + -40]           # repl_start
                        mov              dword ptr [rbp + -48], eax
                        mov              qword ptr [rbp + -56], r14           # repl_end
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
.Lx769_1:               test             rax, rax;                            je    .Lx769_2
                        lea              rcx, [rip + .Lx769_3]
                        lea              rdx, [rip + .Lx769_4];               jmp   rax
.Lx769_3:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx769_1
.Lx769_4:               mov              qword ptr [rip + rtccb+40], r8
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx769_1
.Lx769_2:               call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              eax, dword ptr [rbp + -48]           # repl_start
                        mov              dword ptr [r12 + 0], eax
                        mov              rax, qword ptr [rbp + -56]           # repl_end
                        mov              qword ptr [r12 + 8], rax
                        add              r12, 16
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n306_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n306_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx770_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n307_match_replace_α
.Lx770_0:               .quad            .Lx770_0_s
.Lx770_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n307_match_replace_α:   mov              rdi, qword ptr [rip + .Lx772_0]
                        mov              rsi, qword ptr [rsp + 16]            # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              ecx, dword ptr [r12 + -16]           # repl_start
                        mov              r8, qword ptr [r12 + -8]             # repl_end
                        sub              r12, 16
                        lea              r9, [rsp + 0]                        # lit_string
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_replace@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16;                             jmp   .Lx772_1
.Lx772_0:               .quad            .Lx772_0_s
.Lx772_0_s:             .string          "subject"
.Lx772_1:                                                                     jmp   n308_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n308_statement_end_α:   add              rsp, 16;                             jmp   n309_statement_begin_α
#=======================================================================================================================
#         <stmt 68, line 128: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n309_statement_begin_α:                                                       jmp   n310_var_α
n309_statement_begin_β:                                                       jmp   n313_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n310_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 208]            # w1
                        mov              rdx, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n311_call_α
#-----------------------------------------------------------------------------------------------------------------------
n311_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig779z]
                        lea              rax, [rip + stack_push_alpha];       jmp   rax
.Lsig779z:              .quad            1
                        .quad            .Lx779_2
                        .quad            .Lx779_2
                        .quad            16
.Lx779_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx779_29
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
.Lx779_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx779_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n309_statement_begin_β
.Lx779_240:                                                                   jmp   n312_statement_end_α
n311_call_β:                                                                  jmp   n309_statement_begin_β
.Lx779_0:               .quad            .Lx779_0_s
.Lx779_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n312_statement_end_α:   add              rsp, 32;                             jmp   n313_statement_begin_α
#=======================================================================================================================
#         <stmt 69, line 129: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n313_statement_begin_α:                                                       jmp   n314_var_α
n313_statement_begin_β:                                                       jmp   n317_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n314_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]            # w2
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n315_call_α
#-----------------------------------------------------------------------------------------------------------------------
n315_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig786z]
                        lea              rax, [rip + stack_push_alpha];       jmp   rax
.Lsig786z:              .quad            1
                        .quad            .Lx786_2
                        .quad            .Lx786_2
                        .quad            16
.Lx786_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx786_29
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
.Lx786_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx786_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n313_statement_begin_β
.Lx786_240:                                                                   jmp   n316_statement_end_α
n315_call_β:                                                                  jmp   n313_statement_begin_β
.Lx786_0:               .quad            .Lx786_0_s
.Lx786_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n316_statement_end_α:   add              rsp, 32;                             jmp   n317_statement_begin_α
#=======================================================================================================================
#         <stmt 70, line 130: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n317_statement_begin_α:                                                       jmp   n318_call_α
n317_statement_begin_β:                                                       jmp   n321_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n318_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig792z]
                        lea              rax, [rip + stack_pop_alpha];        jmp   rax
.Lsig792z:              .quad            0
                        .quad            .Lx792_2
                        .quad            .Lx792_2
.Lx792_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx792_29
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
.Lx792_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx792_240
                        add              rsp, 16;                             jmp   n317_statement_begin_β
.Lx792_240:                                                                   jmp   n319_assign_α
n318_call_β:                                                                  jmp   n317_statement_begin_β
.Lx792_0:               .quad            .Lx792_0_s
.Lx792_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n319_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx793_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n320_statement_end_α
.Lx793_0:               .quad            .Lx793_0_s
.Lx793_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n320_statement_end_α:   add              rsp, 16;                             jmp   n321_statement_begin_α
#=======================================================================================================================
#         <stmt 71, line 131: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n321_statement_begin_α:                                                       jmp   n322_call_α
n321_statement_begin_β:                                                       jmp   n325_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n322_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig799z]
                        lea              rax, [rip + stack_pop_alpha];        jmp   rax
.Lsig799z:              .quad            0
                        .quad            .Lx799_2
                        .quad            .Lx799_2
.Lx799_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx799_29
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
.Lx799_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx799_240
                        add              rsp, 16;                             jmp   n321_statement_begin_β
.Lx799_240:                                                                   jmp   n323_assign_α
n322_call_β:                                                                  jmp   n321_statement_begin_β
.Lx799_0:               .quad            .Lx799_0_s
.Lx799_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n323_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx800_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n324_statement_end_α
.Lx800_0:               .quad            .Lx800_0_s
.Lx800_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n324_statement_end_α:   add              rsp, 16;                             jmp   n325_statement_begin_α
#=======================================================================================================================
#         <stmt 72, line 132: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n325_statement_begin_α:                                                       jmp   n326_statement_end_α
n325_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n326_statement_end_α:                                                         jmp   main_γ
#=======================================================================================================================
# bad_s2  <stmt 73, line 133: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n327_statement_begin_α:                                                       jmp   n328_lit_string_α
n327_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n328_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 26
                        mov              rax, qword ptr [rip + .Lx809_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n329_assign_α
.Lx809_0:               .quad            .Lx809_0_s
.Lx809_0_s:             .string          "FAIL: pattern match failed"
#-----------------------------------------------------------------------------------------------------------------------
n329_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx810_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n330_statement_end_α
.Lx810_0:               .quad            .Lx810_0_s
.Lx810_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n330_statement_end_α:   add              rsp, 16;                             jmp   main_γ
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
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              edi, 1
                        call             exit@PLT
                        .section         .rodata
.S0:                    .string          "w1"
.S1:                    .string          "w2"
                        .text
                        .section         .note.GNU-stack,"",@progbits
