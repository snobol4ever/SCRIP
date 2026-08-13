                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__fib_α:
proc_LBL__fib_α_body:
#=======================================================================================================================
# fib     LE(n, 1)                                                   :S(base)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_var_α
n0_statement_begin_β:                                               jmp   n11_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n2_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx26_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n3_coerce_numeric_α
n2_lit_integer_β:       add              rsp, 16
                        add              rsp, 16;                   jmp   n0_statement_begin_β
.Lx26_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n3_coerce_numeric_α:    sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5;                    je    .Lx28_1
                        cmp              eax, 3;                    jne   .Lx28_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3;                    jne   .Lx28_0
.Lx28_1:                mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax;  jmp   n4_coerce_numeric_α
.Lx28_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                                # lit_integer
                        lea              rdx, [rsp + 0]                                 # result
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n4_coerce_numeric_α
n3_coerce_numeric_β:    add              rsp, 16;                   jmp   n2_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n4_coerce_numeric_α:    sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5;                    je    .Lx30_1
                        cmp              eax, 3;                    jne   .Lx30_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3;                    jne   .Lx30_0
.Lx30_1:                mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax;  jmp   n5_cmp_test_α
.Lx30_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                                # var
                        lea              rdx, [rsp + 0]                                 # result
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n5_cmp_test_α
n4_coerce_numeric_β:    add              rsp, 16;                   jmp   n3_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n5_cmp_test_α:          sub              rsp, 16
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
                        test             eax, eax;                  jle   .Lx32_240
                        add              rsp, 16;                   jmp   n4_coerce_numeric_β
.Lx32_240:                                                          jmp   n6_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:     add              rsp, 80;                   jmp   n7_statement_begin_α
#=======================================================================================================================
# base    fib = n                                                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_begin_α:                                               jmp   n8_var_α
n7_statement_begin_β:                                               jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n9_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n9_assign_α:            mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # fib
                        mov              qword ptr [r9 + 8], rdx;   jmp   n10_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_end_α:    add              rsp, 16;                   jmp   RETURN
#=======================================================================================================================
#         fib = fib(n - 1) + fib(n - 2)                             :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_begin_α:                                              jmp   n12_var_α
n11_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n12_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n13_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx44_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n14_binop_α
n13_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n11_statement_begin_β
.Lx44_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n14_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104;                  jne   .Lx45_240
                        add              rsp, 16;                   jmp   n13_lit_integer_β
.Lx45_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n15_call_α
n14_binop_β:            add              rsp, 16;                   jmp   n13_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n15_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # binop
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx47_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + fib_alpha];    jmp   rax
.Lx47_2:                mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx47_240
                        add              rsp, 16;                   jmp   n14_binop_β
.Lx47_240:                                                          jmp   n16_var_α
n15_call_β:                                                         jmp   n14_binop_β
.Lx47_0:                .quad            .Lx47_0_s
.Lx47_0_s:              .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n16_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n17_lit_integer_α
n16_var_β:              add              rsp, 32;                   jmp   n14_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx49_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n18_binop_α
n17_lit_integer_β:      add              rsp, 16;                   jmp   n16_var_β
.Lx49_0:                .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n18_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104;                  jne   .Lx50_240
                        add              rsp, 16;                   jmp   n17_lit_integer_β
.Lx50_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n19_call_α
n18_binop_β:            add              rsp, 16;                   jmp   n17_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n19_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # binop
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx52_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + fib_alpha];    jmp   rax
.Lx52_2:                mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx52_240
                        add              rsp, 16;                   jmp   n18_binop_β
.Lx52_240:                                                          jmp   n20_binop_α
n19_call_β:                                                         jmp   n18_binop_β
.Lx52_0:                .quad            .Lx52_0_s
.Lx52_0_s:              .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n20_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # call
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                  jne   .Lx53_240
                        add              rsp, 32;                   jmp   n18_binop_β
.Lx53_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n21_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n21_assign_α:           mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # fib
                        mov              qword ptr [r9 + 8], rdx;   jmp   n22_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_end_α:    add              rsp, 144;                  jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__fib_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__fib_β:
                                                                    jmp   proc_LBL__fib_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__fib_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__fib_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_fib_α
proc_fib_α:
proc_fib_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n57_save_restore_α:                                                 jmp   n58_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n58_goto_deferred_α:    lea              rax, [rip + proc_LBL__fib_α]; jmp   rax
                                                                    jmp   n59_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n59_save_restore_α:
fib_alpha:              mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx65_2
                        mov              eax, 16
.Lx65_2:                sub              rsp, 64
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 0]                        # fib
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0;                    jbe   .Lx65_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 16]                       # n
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx65_41
.Lx65_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx65_41:               lea              r10, [rip + fib_gamma]
                        lea              r11, [rip + fib_omega]
                        lea              rax, [rip + proc_LBL__fib_α]; jmp   rax
fib_gamma:              mov              rdi, qword ptr [r9 + 0]                        # fib
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx65_75
                        mov              eax, 16
.Lx65_75:               lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rcx, 0;                    jbe   .Lx65_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # n
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx65_110
.Lx65_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx65_110:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                    jbe   .Lx65_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx65_3:                mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
fib_omega:              mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx65_145
                        mov              eax, 16
.Lx65_145:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax                        # fib
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rcx, 0;                    jbe   .Lx65_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # n
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx65_180
.Lx65_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx65_180:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                    jbe   .Lx65_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx65_4:                mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_fib_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_fib_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_fib_β:
                                                                    jmp   proc_fib_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_fib_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_fib_ω:
                                                                    jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__fib"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__fib_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 592
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "fib"
.Lgvan1:                .string          "n"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 2
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 2
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                    jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         DEFINE('fib(n)')                                            :(fib_end)
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_begin_α:                                              jmp   n67_func_activate_α
n66_statement_begin_β:                                              jmp   n69_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n67_func_activate_α:    mov              rdi, qword ptr [rip + .Lx101_0]
                        mov              rsi, qword ptr [rip + .Lx101_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_fib_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n68_statement_end_α
n67_func_activate_β:                                                jmp   n66_statement_begin_β
.Lx101_0:               .quad            .Lx101_0_s
.Lx101_0_s:             .string          "fib"
.Lx101_1:               .quad            .Lx101_1_s
.Lx101_1_s:             .string          "n"
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_end_α:                                                jmp   n69_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_begin_α:                                              jmp   n70_statement_end_α
n69_statement_begin_β:                                              jmp   n71_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_end_α:                                                jmp   n71_statement_begin_α
#=======================================================================================================================
#         OUTPUT = fib(0)
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_begin_α:                                              jmp   n72_lit_integer_α
n71_statement_begin_β:                                              jmp   n76_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n72_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx110_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n73_call_α
.Lx110_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n73_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx112_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + fib_alpha];    jmp   rax
.Lx112_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx112_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n71_statement_begin_β
.Lx112_240:                                                         jmp   n74_assign_α
n73_call_β:                                                         jmp   n71_statement_begin_β
.Lx112_0:               .quad            .Lx112_0_s
.Lx112_0_s:             .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n74_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx113_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n75_statement_end_α
.Lx113_0:               .quad            .Lx113_0_s
.Lx113_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n75_statement_end_α:    add              rsp, 32;                   jmp   n76_statement_begin_α
#=======================================================================================================================
#         OUTPUT = fib(1)
#-----------------------------------------------------------------------------------------------------------------------
n76_statement_begin_α:                                              jmp   n77_lit_integer_α
n76_statement_begin_β:                                              jmp   n81_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n77_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx118_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n78_call_α
.Lx118_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n78_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx120_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + fib_alpha];    jmp   rax
.Lx120_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx120_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n76_statement_begin_β
.Lx120_240:                                                         jmp   n79_assign_α
n78_call_β:                                                         jmp   n76_statement_begin_β
.Lx120_0:               .quad            .Lx120_0_s
.Lx120_0_s:             .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n79_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx121_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n80_statement_end_α
.Lx121_0:               .quad            .Lx121_0_s
.Lx121_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n80_statement_end_α:    add              rsp, 32;                   jmp   n81_statement_begin_α
#=======================================================================================================================
#         OUTPUT = fib(6)
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_begin_α:                                              jmp   n82_lit_integer_α
n81_statement_begin_β:                                              jmp   n86_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n82_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx126_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n83_call_α
.Lx126_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n83_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx128_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + fib_alpha];    jmp   rax
.Lx128_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx128_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n81_statement_begin_β
.Lx128_240:                                                         jmp   n84_assign_α
n83_call_β:                                                         jmp   n81_statement_begin_β
.Lx128_0:               .quad            .Lx128_0_s
.Lx128_0_s:             .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n84_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx129_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n85_statement_end_α
.Lx129_0:               .quad            .Lx129_0_s
.Lx129_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n85_statement_end_α:    add              rsp, 32;                   jmp   n86_statement_begin_α
#=======================================================================================================================
#         OUTPUT = fib(10)
#-----------------------------------------------------------------------------------------------------------------------
n86_statement_begin_α:                                              jmp   n87_lit_integer_α
n86_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n87_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx134_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n88_call_α
.Lx134_0:               .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n88_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx136_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + fib_alpha];    jmp   rax
.Lx136_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx136_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n86_statement_begin_β
.Lx136_240:                                                         jmp   n89_assign_α
n88_call_β:                                                         jmp   n86_statement_begin_β
.Lx136_0:               .quad            .Lx136_0_s
.Lx136_0_s:             .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n89_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx137_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n90_statement_end_α
.Lx137_0:               .quad            .Lx137_0_s
.Lx137_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_end_α:    add              rsp, 32;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n91_goto_α:                                                         jmp   n92_statement_begin_α
n91_goto_β:                                                         jmp   main_ω
#=======================================================================================================================
# base    fib = n                                                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n92_statement_begin_α:                                              jmp   n93_var_α
n92_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n93_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n94_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n94_assign_α:           mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # fib
                        mov              qword ptr [r9 + 8], rdx;   jmp   n95_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n95_statement_end_α:                                                jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n96_goto_α:                                                         jmp   n69_statement_begin_α
n96_goto_β:                                                         jmp   main_ω
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
