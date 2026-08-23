                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             module_init
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
                        .section         .rodata
.Lgvan0:                .string          "double"
.Lgvan1:                .string          "s"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .section         .text
                        .intel_syntax    noprefix
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         DEFINE('double(s)')                                         :(double_end)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:   mov              r11, 1
                        mov              r10, 1;                              jmp   n1_define_α
n0_statement_begin_β:   mov              r11, 1;                              jmp   n9_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_define_α:            mov              r11, 2
                        mov              rdi, qword ptr [rip + .Lx25_0]
                        mov              rsi, qword ptr [rip + .Lx25_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n3_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx25_0]
                        lea              rsi, [rip + double_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n2_statement_end_α
n1_define_β:            mov              r11, 2;                              jmp   n0_statement_begin_β
.Lx25_0:                .quad            .Lx25_0_s
.Lx25_0_s:              .string          "double"
.Lx25_1:                .quad            .Lx25_1_s
.Lx25_1_s:              .string          "s"
                                                                              jmp   .Lx26_245
#-----------------------------------------------------------------------------------------------------------------------
double_α:               sub              rsp, 64
                        mov              rax, qword ptr [r9 + 0]              # double
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx26_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx26_41
.Lx26_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx26_41:               lea              rcx, [rip + double_γ]
                        lea              rax, [rip + double_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n3_statement_begin_α];   jmp   rax
double_γ:               mov              rdi, qword ptr [r9 + 0]              # double
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rdx, 0;                              jbe   .Lx26_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx26_110
.Lx26_80:               mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx26_110:              mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 64
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
double_ω:               mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax              # double
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rdx, 0;                              jbe   .Lx26_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx26_180
.Lx26_150:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx26_180:              mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx26_245:
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_end_α:     mov              r11, 3
                        mov              r10, 1;                              jmp   n9_statement_begin_α
#=======================================================================================================================
# double  double = 2 * s                                             :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n3_statement_begin_α:   mov              r11, 4
                        mov              r10, 2;                              jmp   n4_lit_integer_α
n3_statement_begin_β:   mov              r11, 4;                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_integer_α:       sub              rsp, 16
                        mov              r11, 5
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx31_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n5_var_α
.Lx31_0:                .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n5_var_α:               sub              rsp, 16
                        mov              r11, 6
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n6_binop_α
n5_var_β:               mov              r11, 6
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n3_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n6_binop_α:             sub              rsp, 16
                        mov              r11, 7
                        mov              eax, dword ptr [rsp + 16]            # var
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              al, 3;                               jne   .Lx33_2
                        mov              rax, 2
                        imul             rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx33_7
.Lx33_2:                mov              ecx, eax
                        mov              edx, eax
                        and              edx, 1;                              jz    .Lx33_0
                        mov              rsi, 2
                        mov              rdi, qword ptr [rsp + 24]            # var
                        cvtsi2sd         xmm0, rsi
                        cmp              cl, 5;                               je    .Lx33_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx33_6
.Lx33_5:                movq             xmm1, rdi
.Lx33_6:                mulsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx33_7:                                                                      jmp   n7_assign_α
.Lx33_0:                mov              rdi, qword ptr [rsp + 32]            # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_mul@PLT
                        cmp              al, 104;                             jne   .Lx33_240
                        add              rsp, 16;                             jmp   n5_var_β
.Lx33_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n7_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n7_assign_α:            mov              r11, 8
                        mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # double
                        mov              qword ptr [r9 + 8], rdx;             jmp   n8_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_end_α:     mov              r11, 9
                        mov              r10, 2
                        add              rsp, 48;                             jmp   RETURN
#=======================================================================================================================
# double_end  <stmt 3, line 5: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_begin_α:   mov              r11, 10
                        mov              r10, 3;                              jmp   n10_statement_end_α
n9_statement_begin_β:   mov              r11, 10;                             jmp   n11_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_end_α:    mov              r11, 11
                        mov              r10, 3;                              jmp   n11_statement_begin_α
#=======================================================================================================================
#         OUTPUT = double(5)
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_begin_α:  mov              r11, 12
                        mov              r10, 4;                              jmp   n12_lit_integer_α
n11_statement_begin_β:  mov              r11, 12;                             jmp   n16_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_integer_α:      sub              rsp, 16
                        mov              r11, 13
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx43_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n13_call_α
.Lx43_0:                .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n13_call_α:             sub              rsp, 16
                        mov              r11, 14
                        lea              rcx, [rip + .Lsig45z]
                        lea              rax, [rip + double_α];               jmp   rax
.Lsig45z:               .quad            1
                        .quad            .Lx45_2
                        .quad            .Lx45_2
                        .quad            16
.Lx45_2:                mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx45_29
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
.Lx45_29:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx45_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n11_statement_begin_β
.Lx45_240:                                                                    jmp   n14_assign_α
n13_call_β:             mov              r11, 14;                             jmp   n11_statement_begin_β
.Lx45_0:                .quad            .Lx45_0_s
.Lx45_0_s:              .string          "double"
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_α:           mov              r11, 15
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx46_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n15_statement_end_α
.Lx46_0:                .quad            .Lx46_0_s
.Lx46_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:    mov              r11, 16
                        mov              r10, 4
                        add              rsp, 32;                             jmp   n16_statement_begin_α
#=======================================================================================================================
#         OUTPUT = double(21)
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:  mov              r11, 17
                        mov              r10, 5;                              jmp   n17_lit_integer_α
n16_statement_begin_β:  mov              r11, 17;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:      sub              rsp, 16
                        mov              r11, 18
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx51_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n18_call_α
.Lx51_0:                .quad            21
#-----------------------------------------------------------------------------------------------------------------------
n18_call_α:             sub              rsp, 16
                        mov              r11, 19
                        lea              rcx, [rip + .Lsig53z]
                        lea              rax, [rip + double_α];               jmp   rax
.Lsig53z:               .quad            1
                        .quad            .Lx53_2
                        .quad            .Lx53_2
                        .quad            16
.Lx53_2:                mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx53_29
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
.Lx53_29:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx53_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n16_statement_begin_β
.Lx53_240:                                                                    jmp   n19_assign_α
n18_call_β:             mov              r11, 19;                             jmp   n16_statement_begin_β
.Lx53_0:                .quad            .Lx53_0_s
.Lx53_0_s:              .string          "double"
#-----------------------------------------------------------------------------------------------------------------------
n19_assign_α:           mov              r11, 20
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx54_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n20_statement_end_α
.Lx54_0:                .quad            .Lx54_0_s
.Lx54_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_end_α:    mov              r11, 21
                        mov              r10, 5
                        add              rsp, 32;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 mov              r11, 22
                        pop              rcx
                        add              rsp, 8;                              jmp   rcx
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
module_init:
                        sub              rsp, 8
                        .section         .rodata
.Lseala1:               .string          "double"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            double_α
                        lea              rdi, [rip + .Lseala1]
                        mov              rsi, qword ptr [rip + double_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        add              rsp, 8
                        ret
                        .section         .note.GNU-stack,"",@progbits
