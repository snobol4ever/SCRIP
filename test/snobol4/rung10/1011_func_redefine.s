                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__myfunc2_α:
proc_LBL__myfunc2_α_body:
#=======================================================================================================================
# myfunc2 myfunc = NE(myfunc, 1) myfunc * myfunc(myfunc - 1) :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_var_α
n0_statement_begin_β:                                               jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # myfunc
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n2_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx18_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n3_coerce_numeric_α
n2_lit_integer_β:       add              rsp, 16
                        add              rsp, 16;                   jmp   n0_statement_begin_β
.Lx18_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n3_coerce_numeric_α:    sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5;                    je    .Lx20_1
                        cmp              eax, 3;                    jne   .Lx20_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3;                    jne   .Lx20_0
.Lx20_1:                mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax;  jmp   n4_coerce_numeric_α
.Lx20_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                                # lit_integer
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 149
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
                        cmp              eax, 5;                    je    .Lx22_1
                        cmp              eax, 3;                    jne   .Lx22_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3;                    jne   .Lx22_0
.Lx22_1:                mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax;  jmp   n5_cmp_test_α
.Lx22_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 150
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
                        test             eax, eax;                  jne   .Lx24_240
                        add              rsp, 16;                   jmp   n4_coerce_numeric_β
.Lx24_240:              mov              qword ptr [rsp + 0], 0                         # result
                        mov              qword ptr [rsp + 8], 0;    jmp   n6_var_α
n5_cmp_test_β:          add              rsp, 16;                   jmp   n4_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # myfunc
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n7_var_α
n6_var_β:               add              rsp, 16;                   jmp   n5_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n7_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]                        # myfunc
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n8_lit_integer_α
n7_var_β:               add              rsp, 16;                   jmp   n6_var_β
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx27_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n9_binop_α
n8_lit_integer_β:       add              rsp, 16;                   jmp   n7_var_β
.Lx27_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n9_binop_α:             sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104;                  jne   .Lx28_240
                        add              rsp, 16;                   jmp   n8_lit_integer_β
.Lx28_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n10_call_α
n9_binop_β:             add              rsp, 16;                   jmp   n8_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n10_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # binop
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx30_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + myfunc_alpha]; jmp   rax
.Lx30_2:                mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx30_240
                        add              rsp, 16;                   jmp   n9_binop_β
.Lx30_240:                                                          jmp   n11_binop_α
n10_call_β:                                                         jmp   n9_binop_β
.Lx30_0:                .quad            .Lx30_0_s
.Lx30_0_s:              .string          "myfunc"
#-----------------------------------------------------------------------------------------------------------------------
n11_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]                      # var
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]                      # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_mul@PLT
                        cmp              eax, 104;                  jne   .Lx31_240
                        add              rsp, 32;                   jmp   n9_binop_β
.Lx31_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n12_binop_α
n11_binop_β:            add              rsp, 32;                   jmp   n9_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n12_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]                     # cmp_test
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 16]                      # binop
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n13_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n13_assign_α:           mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # myfunc
                        mov              qword ptr [r9 + 8], rdx;   jmp   n14_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_end_α:    add              rsp, 192;                  jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__myfunc2_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__myfunc2_β:
                                                                    jmp   proc_LBL__myfunc2_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__myfunc2_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__myfunc2_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_myfunc_α
proc_myfunc_α:
proc_myfunc_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n36_save_restore_α:                                                 jmp   n37_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n37_goto_deferred_α:    lea              rax, [rip + proc_LBL__myfunc2_α]; jmp   rax
                                                                    jmp   n38_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n38_save_restore_α:
myfunc_alpha:           mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx44_2
                        mov              eax, 16
.Lx44_2:                sub              rsp, 48
                        add              rsp, rax
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              qword ptr [rsp + 16], rcx
                        cmp              rcx, 0;                    jbe   .Lx44_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 0]                        # myfunc
                        mov              qword ptr [r9 + 0], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx44_41
.Lx44_10:               mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
.Lx44_41:               lea              r10, [rip + myfunc_gamma]
                        lea              r11, [rip + myfunc_omega]
                        lea              rax, [rip + proc_LBL__myfunc2_α]; jmp   rax
myfunc_gamma:           mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx44_75
                        mov              eax, 16
.Lx44_75:               lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx44_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 8], rax;   jmp   .Lx44_110
.Lx44_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
.Lx44_110:              mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1;                    jbe   .Lx44_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx44_3:                mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
myfunc_omega:           mov              rcx, qword ptr [rsp + 16]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx44_145
                        mov              eax, 16
.Lx44_145:              lea              r8, [rsp + 32]
                        sub              r8, rax
                        cmp              rcx, 0;                    jbe   .Lx44_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 8], rax;   jmp   .Lx44_180
.Lx44_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
.Lx44_180:              mov              r10, qword ptr [rsp + 0]
                        mov              r11, qword ptr [rsp + 8]
                        mov              eax, 80
                        cmp              rcx, 1;                    jbe   .Lx44_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 64
.Lx44_4:                mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_myfunc_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_myfunc_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_myfunc_β:
                                                                    jmp   proc_myfunc_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_myfunc_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_myfunc_ω:
                                                                    jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__myfunc2"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__myfunc2_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 672
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "myfunc"
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
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_begin_α:                                              jmp   n46_statement_end_α
n45_statement_begin_β:                                              jmp   n47_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_end_α:                                                jmp   n47_statement_begin_α
#=======================================================================================================================
#         DEFINE('myfunc(n)')                            :(myfunc_end)
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_begin_α:                                              jmp   n48_func_activate_α
n47_statement_begin_β:                                              jmp   n50_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n48_func_activate_α:    mov              rdi, qword ptr [rip + .Lx116_0]
                        mov              rsi, qword ptr [rip + .Lx116_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_myfunc_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n49_statement_end_α
n48_func_activate_β:                                                jmp   n47_statement_begin_β
.Lx116_0:               .quad            .Lx116_0_s
.Lx116_0_s:             .string          "myfunc"
.Lx116_1:               .quad            .Lx116_1_s
.Lx116_1_s:             .string          "myfunc"
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_end_α:                                                jmp   n50_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_begin_α:                                              jmp   n51_statement_end_α
n50_statement_begin_β:                                              jmp   n52_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_end_α:                                                jmp   n52_statement_begin_α
#=======================================================================================================================
#         NE(myfunc(3), 6)           :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_begin_α:                                              jmp   n53_lit_integer_α
n52_statement_begin_β:                                              jmp   n64_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n53_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx125_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n54_call_α
.Lx125_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n54_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx127_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + myfunc_alpha]; jmp   rax
.Lx127_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx127_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n52_statement_begin_β
.Lx127_240:                                                         jmp   n55_lit_integer_α
n54_call_β:                                                         jmp   n52_statement_begin_β
.Lx127_0:               .quad            .Lx127_0_s
.Lx127_0_s:             .string          "myfunc"
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx128_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n56_coerce_numeric_α
n55_lit_integer_β:      add              rsp, 16
                        add              rsp, 32;                   jmp   n52_statement_begin_β
.Lx128_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n56_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # call
                        cmp              eax, 5;                    je    .Lx130_1
                        cmp              eax, 3;                    jne   .Lx130_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3;                    jne   .Lx130_0
.Lx130_1:               mov              rax, qword ptr [rsp + 32]                      # call
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # call
                        mov              qword ptr [rsp + 8], rax;  jmp   n57_coerce_numeric_α
.Lx130_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                                # lit_integer
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 149
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n57_coerce_numeric_α
n56_coerce_numeric_β:   add              rsp, 16;                   jmp   n55_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n57_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5;                    je    .Lx132_1
                        cmp              eax, 3;                    jne   .Lx132_0
                        mov              eax, dword ptr [rsp + 48]                      # call
                        cmp              eax, 3;                    jne   .Lx132_0
.Lx132_1:               mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax;  jmp   n58_cmp_test_α
.Lx132_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                                # call
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 150
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n58_cmp_test_α
n57_coerce_numeric_β:   add              rsp, 16;                   jmp   n56_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n58_cmp_test_α:         sub              rsp, 16
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
                        test             eax, eax;                  jne   .Lx134_240
                        add              rsp, 16;                   jmp   n57_coerce_numeric_β
.Lx134_240:                                                         jmp   n59_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_end_α:    add              rsp, 96;                   jmp   n60_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1011/001: first definition myfunc(3)=6' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_begin_α:                                              jmp   n61_lit_string_α
n60_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 43
                        mov              rax, qword ptr [rip + .Lx139_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n62_assign_α
.Lx139_0:               .quad            .Lx139_0_s
.Lx139_0_s:             .string          "FAIL 1011/001: first definition myfunc(3)=6"
#-----------------------------------------------------------------------------------------------------------------------
n62_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx140_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n63_statement_end_α
.Lx140_0:               .quad            .Lx140_0_s
.Lx140_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n63_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_begin_α:                                              jmp   n65_statement_end_α
n64_statement_begin_β:                                              jmp   n66_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_end_α:                                                jmp   n66_statement_begin_α
#=======================================================================================================================
#         DIFFER(DEFINE('myfunc(myfunc)', 'myfunc2'))                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_begin_α:                                              jmp   n67_lit_string_α
n66_statement_begin_β:                                              jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx149_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n68_call_α
.Lx149_0:               .quad            .Lx149_0_s
.Lx149_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n68_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd151:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd151]
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
                        cmp              eax, 104;                  jne   .Lx150_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n66_statement_begin_β
.Lx150_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n69_statement_end_α
n68_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n66_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_end_α:    add              rsp, 32;                   jmp   n70_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1011/002: define returns function name' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_begin_α:                                              jmp   n71_lit_string_α
n70_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 43
                        mov              rax, qword ptr [rip + .Lx156_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n72_assign_α
.Lx156_0:               .quad            .Lx156_0_s
.Lx156_0_s:             .string          "FAIL 1011/002: define returns function name"
#-----------------------------------------------------------------------------------------------------------------------
n72_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx157_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n73_statement_end_α
.Lx157_0:               .quad            .Lx157_0_s
.Lx157_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#=======================================================================================================================
# e002    :(myfunc2_end)
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_begin_α:                                              jmp   n75_statement_end_α
n74_statement_begin_β:                                              jmp   n76_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n75_statement_end_α:                                                jmp   n76_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n76_statement_begin_α:                                              jmp   n77_statement_end_α
n76_statement_begin_β:                                              jmp   n78_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_end_α:                                                jmp   n78_statement_begin_α
#=======================================================================================================================
#         NE(myfunc(4), 24)          :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_begin_α:                                              jmp   n79_lit_integer_α
n78_statement_begin_β:                                              jmp   n90_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx170_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n80_call_α
.Lx170_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n80_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx172_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + myfunc_alpha]; jmp   rax
.Lx172_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx172_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n78_statement_begin_β
.Lx172_240:                                                         jmp   n81_lit_integer_α
n80_call_β:                                                         jmp   n78_statement_begin_β
.Lx172_0:               .quad            .Lx172_0_s
.Lx172_0_s:             .string          "myfunc"
#-----------------------------------------------------------------------------------------------------------------------
n81_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx173_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n82_coerce_numeric_α
n81_lit_integer_β:      add              rsp, 16
                        add              rsp, 32;                   jmp   n78_statement_begin_β
.Lx173_0:               .quad            24
#-----------------------------------------------------------------------------------------------------------------------
n82_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # call
                        cmp              eax, 5;                    je    .Lx175_1
                        cmp              eax, 3;                    jne   .Lx175_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3;                    jne   .Lx175_0
.Lx175_1:               mov              rax, qword ptr [rsp + 32]                      # call
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # call
                        mov              qword ptr [rsp + 8], rax;  jmp   n83_coerce_numeric_α
.Lx175_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                                # lit_integer
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 149
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n83_coerce_numeric_α
n82_coerce_numeric_β:   add              rsp, 16;                   jmp   n81_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n83_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5;                    je    .Lx177_1
                        cmp              eax, 3;                    jne   .Lx177_0
                        mov              eax, dword ptr [rsp + 48]                      # call
                        cmp              eax, 3;                    jne   .Lx177_0
.Lx177_1:               mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax;  jmp   n84_cmp_test_α
.Lx177_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                                # call
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 150
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n84_cmp_test_α
n83_coerce_numeric_β:   add              rsp, 16;                   jmp   n82_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n84_cmp_test_α:         sub              rsp, 16
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
                        test             eax, eax;                  jne   .Lx179_240
                        add              rsp, 16;                   jmp   n83_coerce_numeric_β
.Lx179_240:                                                         jmp   n85_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n85_statement_end_α:    add              rsp, 96;                   jmp   n86_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1011/003: redefined myfunc(4)=24'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n86_statement_begin_α:                                              jmp   n87_lit_string_α
n86_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n87_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx184_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n88_assign_α
.Lx184_0:               .quad            .Lx184_0_s
.Lx184_0_s:             .string          "FAIL 1011/003: redefined myfunc(4)=24"
#-----------------------------------------------------------------------------------------------------------------------
n88_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx185_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n89_statement_end_α
.Lx185_0:               .quad            .Lx185_0_s
.Lx185_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_begin_α:                                              jmp   n91_statement_end_α
n90_statement_begin_β:                                              jmp   n92_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_end_α:                                                jmp   n92_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1011_func_redefine (3/3)'
#-----------------------------------------------------------------------------------------------------------------------
n92_statement_begin_α:                                              jmp   n93_lit_string_α
n92_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n93_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 29
                        mov              rax, qword ptr [rip + .Lx194_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n94_assign_α
.Lx194_0:               .quad            .Lx194_0_s
.Lx194_0_s:             .string          "PASS 1011_func_redefine (3/3)"
#-----------------------------------------------------------------------------------------------------------------------
n94_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx195_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n95_statement_end_α
.Lx195_0:               .quad            .Lx195_0_s
.Lx195_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n95_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n96_goto_α:                                                         jmp   n97_statement_begin_α
n96_goto_β:                                                         jmp   main_ω
#=======================================================================================================================
# myfunc  myfunc = n * 2                                 :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n97_statement_begin_α:                                              jmp   n98_var_α
n97_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n98_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n99_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n99_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx202_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n100_binop_α
n99_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n97_statement_begin_β
.Lx202_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n100_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_mul@PLT
                        cmp              eax, 104;                  jne   .Lx203_240
                        add              rsp, 16;                   jmp   n99_lit_integer_β
.Lx203_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n101_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n101_assign_α:          mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # myfunc
                        mov              qword ptr [r9 + 8], rdx;   jmp   n102_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:                                               jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n103_goto_α:                                                        jmp   n50_statement_begin_α
n103_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n104_goto_α:                                                        jmp   n64_statement_begin_α
n104_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n105_goto_α:                                                        jmp   n74_statement_begin_α
n105_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n106_goto_α:                                                        jmp   n76_statement_begin_α
n106_goto_β:                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n107_goto_α:                                                        jmp   n90_statement_begin_α
n107_goto_β:                                                        jmp   main_ω
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
