                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ispos_α:
proc_LBL__ispos_α_body:
#=======================================================================================================================
# ispos   GT(x, 0)                                                   :S(RETURN)F(FRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_var_α
n0_statement_begin_β:                                               jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # x
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n2_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx10_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n3_coerce_numeric_α
n2_lit_integer_β:       add              rsp, 16
                        add              rsp, 16;                   jmp   n0_statement_begin_β
.Lx10_0:                .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n3_coerce_numeric_α:    sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5;                    je    .Lx12_1
                        cmp              eax, 3;                    jne   .Lx12_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3;                    jne   .Lx12_0
.Lx12_1:                mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax;  jmp   n4_coerce_numeric_α
.Lx12_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                                # lit_integer
                        lea              rdx, [rsp + 0]                                 # result
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
                        mov              r11,  qword ptr [r11 + 64]; jmp   n4_coerce_numeric_α
n3_coerce_numeric_β:    add              rsp, 16;                   jmp   n2_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n4_coerce_numeric_α:    sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5;                    je    .Lx14_1
                        cmp              eax, 3;                    jne   .Lx14_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3;                    jne   .Lx14_0
.Lx14_1:                mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax;  jmp   n5_cmp_test_α
.Lx14_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                                # var
                        lea              rdx, [rsp + 0]                                 # result
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
                        test             eax, eax;                  jg    .Lx16_240
                        add              rsp, 16;                   jmp   n4_coerce_numeric_β
.Lx16_240:                                                          jmp   n6_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:     add              rsp, 80;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ispos_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ispos_β:
                                                                    jmp   proc_LBL__ispos_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ispos_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ispos_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_ispos_α
proc_ispos_α:
proc_ispos_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n19_save_restore_α:                                                 jmp   n20_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n20_goto_deferred_α:    lea              rax, [rip + proc_LBL__ispos_α]; jmp   rax
                                                                    jmp   n21_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n21_save_restore_α:
ispos_alpha:            mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 16
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx27_2
                        mov              eax, 16
.Lx27_2:                sub              rsp, 64
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 0]                        # ispos
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0;                    jbe   .Lx27_10
                        mov              rax, qword ptr [r8 + 48]
                        mov              rdx, qword ptr [r9 + 16]                       # x
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 48], rdx
                        mov              rax, qword ptr [r8 + 56]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 56], rdx;  jmp   .Lx27_41
.Lx27_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx27_41:               lea              r10, [rip + ispos_gamma]
                        lea              r11, [rip + ispos_omega]
                        lea              rax, [rip + proc_LBL__ispos_α]; jmp   rax
ispos_gamma:            mov              rdi, qword ptr [r9 + 0]                        # ispos
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx27_75
                        mov              eax, 16
.Lx27_75:               lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rcx, 0;                    jbe   .Lx27_80
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # x
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx27_110
.Lx27_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx27_110:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                    jbe   .Lx27_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx27_3:                mov              rcx, qword ptr [r8 + 32]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
ispos_omega:            mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 1;                    jbe   .Lx27_145
                        mov              eax, 16
.Lx27_145:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax                        # ispos
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rcx, 0;                    jbe   .Lx27_150
                        mov              rax, qword ptr [r8 + 48]
                        mov              qword ptr [r9 + 16], rax                       # x
                        mov              rax, qword ptr [r8 + 56]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx27_180
.Lx27_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx27_180:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 96
                        cmp              rcx, 1;                    jbe   .Lx27_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx27_4:                mov              rcx, qword ptr [r8 + 40]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_ispos_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_ispos_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_ispos_β:
                                                                    jmp   proc_ispos_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_ispos_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_ispos_ω:
                                                                    jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__ispos"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__ispos_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 336
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "ispos"
.Lgvan1:                .string          "x"
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
#         DEFINE('ispos(x)')                                          :(ispos_end)
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_begin_α:                                              jmp   n29_func_activate_α
n28_statement_begin_β:                                              jmp   n31_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n29_func_activate_α:    mov              rdi, qword ptr [rip + .Lx73_0]
                        mov              rsi, qword ptr [rip + .Lx73_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + proc_ispos_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n30_statement_end_α
n29_func_activate_β:                                                jmp   n28_statement_begin_β
.Lx73_0:                .quad            .Lx73_0_s
.Lx73_0_s:              .string          "ispos"
.Lx73_1:                .quad            .Lx73_1_s
.Lx73_1_s:              .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_end_α:                                                jmp   n31_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_begin_α:                                              jmp   n32_statement_end_α
n31_statement_begin_β:                                              jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:                                                jmp   n33_statement_begin_α
#=======================================================================================================================
#         ispos(5)                                                    :S(A)F(B)
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:                                              jmp   n34_lit_integer_α
n33_statement_begin_β:                                              jmp   n43_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx82_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n35_call_α
.Lx82_0:                .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n35_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx84_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + ispos_alpha];  jmp   rax
.Lx84_2:                mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx84_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n33_statement_begin_β
.Lx84_240:                                                          jmp   n36_statement_end_α
n35_call_β:                                                         jmp   n33_statement_begin_β
.Lx84_0:                .quad            .Lx84_0_s
.Lx84_0_s:              .string          "ispos"
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:    add              rsp, 32;                   jmp   n37_statement_begin_α
#=======================================================================================================================
# A       OUTPUT = 'positive'
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_begin_α:                                              jmp   n38_lit_string_α
n37_statement_begin_β:                                              jmp   n41_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx89_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n39_assign_α
.Lx89_0:                .quad            .Lx89_0_s
.Lx89_0_s:              .string          "positive"
#-----------------------------------------------------------------------------------------------------------------------
n39_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx90_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n40_statement_end_α
.Lx90_0:                .quad            .Lx90_0_s
.Lx90_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_end_α:    add              rsp, 16;                   jmp   n41_statement_begin_α
#=======================================================================================================================
#         :(NEXT)
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_begin_α:                                              jmp   n42_statement_end_α
n41_statement_begin_β:                                              jmp   n47_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_end_α:                                                jmp   n47_statement_begin_α
#=======================================================================================================================
# B       OUTPUT = 'wrong'
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_begin_α:                                              jmp   n44_lit_string_α
n43_statement_begin_β:                                              jmp   n47_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n44_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx99_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n45_assign_α
.Lx99_0:                .quad            .Lx99_0_s
.Lx99_0_s:              .string          "wrong"
#-----------------------------------------------------------------------------------------------------------------------
n45_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx100_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n46_statement_end_α
.Lx100_0:               .quad            .Lx100_0_s
.Lx100_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_end_α:    add              rsp, 16;                   jmp   n47_statement_begin_α
#=======================================================================================================================
# NEXT    ispos(-3)                                                   :S(C)F(D)
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_begin_α:                                              jmp   n48_lit_integer_α
n47_statement_begin_β:                                              jmp   n58_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx105_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n49_unop_α
.Lx105_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n49_unop_α:             sub              rsp, 16
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
                        mov              qword ptr [rsp + 8], rdx;  jmp   n50_call_α
n49_unop_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n47_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n50_call_α:             sub              rsp, 16
                        sub              rsp, 48
                        mov              rax, qword ptr [rsp + 64]                      # unop
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              eax, 1
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx108_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + ispos_alpha];  jmp   rax
.Lx108_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx108_240
                        add              rsp, 16;                   jmp   n49_unop_β
.Lx108_240:                                                         jmp   n51_statement_end_α
n50_call_β:                                                         jmp   n49_unop_β
.Lx108_0:               .quad            .Lx108_0_s
.Lx108_0_s:             .string          "ispos"
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_end_α:    add              rsp, 48;                   jmp   n52_statement_begin_α
#=======================================================================================================================
# C       OUTPUT = 'wrong'
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_begin_α:                                              jmp   n53_lit_string_α
n52_statement_begin_β:                                              jmp   n56_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n53_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx113_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n54_assign_α
.Lx113_0:               .quad            .Lx113_0_s
.Lx113_0_s:             .string          "wrong"
#-----------------------------------------------------------------------------------------------------------------------
n54_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx114_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n55_statement_end_α
.Lx114_0:               .quad            .Lx114_0_s
.Lx114_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_end_α:    add              rsp, 16;                   jmp   n56_statement_begin_α
#=======================================================================================================================
#         :(END)
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_begin_α:                                              jmp   n57_statement_end_α
n56_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_end_α:                                                jmp   main_γ
#=======================================================================================================================
# D       OUTPUT = 'not positive'
#-----------------------------------------------------------------------------------------------------------------------
n58_statement_begin_α:                                              jmp   n59_lit_string_α
n58_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n59_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 12
                        mov              rax, qword ptr [rip + .Lx123_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n60_assign_α
.Lx123_0:               .quad            .Lx123_0_s
.Lx123_0_s:             .string          "not positive"
#-----------------------------------------------------------------------------------------------------------------------
n60_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx124_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n61_statement_end_α
.Lx124_0:               .quad            .Lx124_0_s
.Lx124_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n62_goto_α:                                                         jmp   n31_statement_begin_α
n62_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n63_goto_α:                                                         jmp   n37_statement_begin_α
n63_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n64_goto_α:                                                         jmp   n43_statement_begin_α
n64_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n65_goto_α:                                                         jmp   n47_statement_begin_α
n65_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n66_goto_α:                                                         jmp   n52_statement_begin_α
n66_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n67_goto_α:                                                         jmp   n58_statement_begin_α
n67_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                lea              rdi, [rip + .S1]
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
.S1:                    .string          "BOMB-FRETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
                        .text
                        .section         .note.GNU-stack,"",@progbits
