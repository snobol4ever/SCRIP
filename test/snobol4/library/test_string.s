                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             module_init
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
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        sub              rsp, 0
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:   mov              r11, 1
                        mov              r10, 0;                              jmp   n1_statement_end_α
n0_statement_begin_β:   mov              r11, 1;                              jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:     mov              r11, 2
                        mov              r10, 1;                              jmp   n2_statement_begin_α
n1_statement_end_β:     mov              r11, 2;                              jmp   n2_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'endswith ok'          :(ew1)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:   mov              r11, 3
                        mov              r10, 2;                              jmp   n3_define_α
n2_statement_begin_β:   mov              r11, 3;                              jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_define_α:            mov              r11, 4
                        mov              rdi, qword ptr [rip + .Lx384_0]
                        mov              rsi, qword ptr [rip + .Lx384_1]
                        mov              edx, 3
                        mov              ecx, 3
                        mov              r8d, 0
                        lea              r9, [rip + n5_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx384_0]
                        lea              rsi, [rip + pad_left_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n4_statement_end_α
n3_define_β:            mov              r11, 4;                              jmp   n2_statement_begin_β
.Lx384_0:               .quad            .Lx384_0_s
.Lx384_0_s:             .string          "pad_left"
.Lx384_1:               .quad            .Lx384_1_s
.Lx384_1_s:             .string          "s,n,c"
                                                                              jmp   .Lx385_245
#-----------------------------------------------------------------------------------------------------------------------
pad_left_α:             sub              rsp, 96
                        mov              rax, qword ptr [r9 + 0]              # pad_left
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        cmp              rdx, 0;                              jbe   .Lx385_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx385_41
.Lx385_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx385_41:              cmp              rdx, 1;                              jbe   .Lx385_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 32]             # n
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx385_42
.Lx385_11:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx385_42:              cmp              rdx, 2;                              jbe   .Lx385_12
                        mov              rdi, qword ptr [rcx + 40]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 48]             # c
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx385_43
.Lx385_12:              mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx385_43:              lea              rcx, [rip + pad_left_γ]
                        lea              rax, [rip + pad_left_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n5_statement_begin_α];   jmp   rax
pad_left_γ:             mov              rdi, qword ptr [r9 + 0]              # pad_left
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rdx, 0;                              jbe   .Lx385_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx385_110
.Lx385_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx385_110:             cmp              rdx, 1;                              jbe   .Lx385_81
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 32], rax             # n
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx385_111
.Lx385_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 40], rax
.Lx385_111:             cmp              rdx, 2;                              jbe   .Lx385_82
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 48], rax             # c
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx385_112
.Lx385_82:              mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 56], rax
.Lx385_112:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 96
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
pad_left_ω:             mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax              # pad_left
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rdx, 0;                              jbe   .Lx385_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx385_180
.Lx385_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx385_180:             cmp              rdx, 1;                              jbe   .Lx385_151
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 32], rax             # n
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx385_181
.Lx385_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 40], rax
.Lx385_181:             cmp              rdx, 2;                              jbe   .Lx385_152
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 48], rax             # c
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx385_182
.Lx385_152:             mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 56], rax
.Lx385_182:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 96
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx385_245:
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_end_α:     mov              r11, 5
                        mov              r10, 2;                              jmp   n33_statement_begin_α
n4_statement_end_β:     mov              r11, 5;                              jmp   n33_statement_begin_α
#=======================================================================================================================
# pad_left  <stmt 3, line 39: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_begin_α:   mov              r11, 6
                        mov              r10, 3;                              jmp   n6_var_α
n5_statement_begin_β:   mov              r11, 6;                              jmp   n12_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:               sub              rsp, 16
                        mov              r11, 7
                        mov              rax, qword ptr [r9 + 48]             # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n7_lit_string_α
n6_var_β:               mov              r11, 7
                        add              rsp, 16;                             jmp   n5_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_string_α:        sub              rsp, 16
                        mov              r11, 8
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx391_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n8_ident_α
n7_lit_string_β:        mov              r11, 8
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n5_statement_begin_β
.Lx391_0:               .quad            .Lx391_0_s
.Lx391_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n8_ident_α:             sub              rsp, 16
                        mov              r11, 9
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            jne   .Lx393_240
                        add              rsp, 16;                             jmp   n7_lit_string_β
.Lx393_240:                                                                   jmp   n9_lit_string_α
n8_ident_β:             mov              r11, 9
                        add              rsp, 16;                             jmp   n7_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_string_α:        sub              rsp, 16
                        mov              r11, 10
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx394_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n10_assign_α
n9_lit_string_β:        mov              r11, 10
                        add              rsp, 16;                             jmp   n8_ident_β
.Lx394_0:               .quad            .Lx394_0_s
.Lx394_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n10_assign_α:           mov              r11, 11
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # c
                        mov              qword ptr [r9 + 56], rdx;            jmp   n11_statement_end_α
n10_assign_β:           mov              r11, 11;                             jmp   n5_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:    mov              r11, 12
                        mov              r10, 3
                        add              rsp, 64;                             jmp   n12_statement_begin_α
n11_statement_end_β:    mov              r11, 12
                        add              rsp, 64;                             jmp   n12_statement_begin_α
#=======================================================================================================================
#         <stmt 4, line 40: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:  mov              r11, 13
                        mov              r10, 4;                              jmp   n13_var_α
n12_statement_begin_β:  mov              r11, 13;                             jmp   n22_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n13_var_α:              sub              rsp, 16
                        mov              r11, 14
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n14_call_α
n13_var_β:              mov              r11, 14
                        add              rsp, 16;                             jmp   n12_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n14_call_α:             sub              rsp, 16
                        mov              r11, 15
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd402:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd402]
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
                        cmp              al, 104;                             jne   .Lx401_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n12_statement_begin_β
.Lx401_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n15_var_α
n14_call_β:             mov              r11, 15
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n12_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n15_var_α:              sub              rsp, 16
                        mov              r11, 16
                        mov              rax, qword ptr [r9 + 32]             # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n16_coerce_numeric_α
n15_var_β:              mov              r11, 16
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n12_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n16_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 17
                        mov              eax, dword ptr [rsp + 32]            # call
                        cmp              al, 5;                               je    .Lx405_1
                        cmp              al, 3;                               jne   .Lx405_0
                        mov              eax, dword ptr [rsp + 16]            # var
                        cmp              al, 3;                               jne   .Lx405_0
.Lx405_1:               mov              rax, qword ptr [rsp + 32]            # call
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # call
                        mov              qword ptr [rsp + 8], rax;            jmp   n17_coerce_numeric_α
.Lx405_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 109
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n17_coerce_numeric_α
n16_coerce_numeric_β:   mov              r11, 17
                        add              rsp, 16;                             jmp   n15_var_β
#-----------------------------------------------------------------------------------------------------------------------
n17_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 18
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              al, 5;                               je    .Lx407_1
                        cmp              al, 3;                               jne   .Lx407_0
                        mov              eax, dword ptr [rsp + 48]            # call
                        cmp              al, 3;                               jne   .Lx407_0
.Lx407_1:               mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n18_cmp_test_α
.Lx407_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # call
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 110
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n18_cmp_test_α
n17_coerce_numeric_β:   mov              r11, 18
                        add              rsp, 16;                             jmp   n16_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n18_cmp_test_α:         sub              rsp, 16
                        mov              r11, 19
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx409_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jge   .Lx409_239
                        add              rsp, 16;                             jmp   n17_coerce_numeric_β
.Lx409_239:                                                                   jmp   n19_var_α
.Lx409_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_cmp_d@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            jns   .Lx409_240
                        add              rsp, 16;                             jmp   n17_coerce_numeric_β
.Lx409_240:                                                                   jmp   n19_var_α
n18_cmp_test_β:         mov              r11, 19
                        add              rsp, 16;                             jmp   n17_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n19_var_α:              sub              rsp, 16
                        mov              r11, 20
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n20_assign_α
n19_var_β:              mov              r11, 20
                        add              rsp, 16;                             jmp   n18_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n20_assign_α:           mov              r11, 21
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # pad_left
                        mov              qword ptr [r9 + 8], rdx;             jmp   n21_statement_end_α
n20_assign_β:           mov              r11, 21;                             jmp   n12_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:    mov              r11, 22
                        mov              r10, 4
                        add              rsp, 112;                            jmp   RETURN
n21_statement_end_β:    mov              r11, 22
                        add              rsp, 112;                            jmp   n22_statement_begin_α
#=======================================================================================================================
#         startswith('foobar', 'bar')     :S(bad_sw2)
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:  mov              r11, 23
                        mov              r10, 5;                              jmp   n23_var_α
n22_statement_begin_β:  mov              r11, 23;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n23_var_α:              sub              rsp, 16
                        mov              r11, 24
                        mov              rax, qword ptr [r9 + 48]             # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n24_var_α
n23_var_β:              mov              r11, 24
                        add              rsp, 16;                             jmp   n22_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n24_var_α:              sub              rsp, 16
                        mov              r11, 25
                        mov              rax, qword ptr [r9 + 32]             # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n25_var_α
n24_var_β:              mov              r11, 25
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n22_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n25_var_α:              sub              rsp, 16
                        mov              r11, 26
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n26_call_α
n25_var_β:              mov              r11, 26
                        add              rsp, 16;                             jmp   n24_var_β
#-----------------------------------------------------------------------------------------------------------------------
n26_call_α:             sub              rsp, 16
                        mov              r11, 27
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd420:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd420]
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
                        cmp              al, 104;                             jne   .Lx419_240
                        add              rsp, 16;                             jmp   n25_var_β
.Lx419_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n27_binop_α
n26_call_β:             mov              r11, 27
                        add              rsp, 16;                             jmp   n25_var_β
#-----------------------------------------------------------------------------------------------------------------------
n27_binop_α:            sub              rsp, 16
                        mov              r11, 28
                        mov              eax, dword ptr [rsp + 48]            # var
                        mov              ecx, dword ptr [rsp + 16]            # call
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx421_2
                        mov              rax, qword ptr [rsp + 56]            # var
                        mov              rdx, qword ptr [rsp + 24]            # call
                        sub              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx421_7
.Lx421_2:               and              edx, 1;                              jz    .Lx421_0
                        mov              rsi, qword ptr [rsp + 56]            # var
                        mov              rdi, qword ptr [rsp + 24]            # call
                        cmp              al, 5;                               je    .Lx421_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx421_4
.Lx421_3:               movq             xmm0, rsi
.Lx421_4:               cmp              cl, 5;                               je    .Lx421_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx421_6
.Lx421_5:               movq             xmm1, rdi
.Lx421_6:               subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx421_7:                                                                     jmp   n28_call_α
.Lx421_0:               mov              rdi, qword ptr [rsp + 48]            # var
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_sub@PLT
                        cmp              al, 104;                             jne   .Lx421_240
                        add              rsp, 32;                             jmp   n25_var_β
.Lx421_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n28_call_α
n27_binop_β:            mov              r11, 28
                        add              rsp, 32;                             jmp   n25_var_β
#-----------------------------------------------------------------------------------------------------------------------
n28_call_α:             sub              rsp, 16
                        mov              r11, 29
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
.Lrkfnzd423:            .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd423]
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
                        cmp              al, 104;                             jne   .Lx422_240
                        add              rsp, 16;                             jmp   n27_binop_β
.Lx422_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n29_var_α
n28_call_β:             mov              r11, 29
                        add              rsp, 16;                             jmp   n27_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n29_var_α:              sub              rsp, 16
                        mov              r11, 30
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n30_binop_α
n29_var_β:              mov              r11, 30
                        add              rsp, 32;                             jmp   n27_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n30_binop_α:            sub              rsp, 16
                        mov              r11, 31
                        mov              rdi, qword ptr [rsp + 32]            # call
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n31_assign_α
n30_binop_β:            mov              r11, 31
                        add              rsp, 16;                             jmp   n29_var_β
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_α:           mov              r11, 32
                        mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # pad_left
                        mov              qword ptr [r9 + 8], rdx;             jmp   n32_statement_end_α
n31_assign_β:           mov              r11, 32;                             jmp   n22_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:    mov              r11, 33
                        mov              r10, 5
                        add              rsp, 128;                            jmp   RETURN
n32_statement_end_β:    mov              r11, 33
                        add              rsp, 128;                            jmp   RETURN
#=======================================================================================================================
# pad_left_end  <stmt 6, line 43: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:  mov              r11, 34
                        mov              r10, 6;                              jmp   n34_statement_end_α
n33_statement_begin_β:  mov              r11, 34;                             jmp   n35_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:    mov              r11, 35
                        mov              r10, 6;                              jmp   n35_statement_begin_α
n34_statement_end_β:    mov              r11, 35;                             jmp   n35_statement_begin_α
#=======================================================================================================================
#         <stmt 7, line 44: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:  mov              r11, 36
                        mov              r10, 7;                              jmp   n36_define_α
n35_statement_begin_β:  mov              r11, 36;                             jmp   n66_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n36_define_α:           mov              r11, 37
                        mov              rdi, qword ptr [rip + .Lx436_0]
                        mov              rsi, qword ptr [rip + .Lx436_1]
                        mov              edx, 3
                        mov              ecx, 3
                        mov              r8d, 0
                        lea              r9, [rip + n38_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx436_0]
                        lea              rsi, [rip + pad_right_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n37_statement_end_α
n36_define_β:           mov              r11, 37;                             jmp   n35_statement_begin_β
.Lx436_0:               .quad            .Lx436_0_s
.Lx436_0_s:             .string          "pad_right"
.Lx436_1:               .quad            .Lx436_1_s
.Lx436_1_s:             .string          "s,n,c"
                                                                              jmp   .Lx437_245
#-----------------------------------------------------------------------------------------------------------------------
pad_right_α:            sub              rsp, 96
                        mov              rax, qword ptr [r9 + 64]             # pad_right
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        cmp              rdx, 0;                              jbe   .Lx437_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx437_41
.Lx437_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx437_41:              cmp              rdx, 1;                              jbe   .Lx437_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 32]             # n
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx437_42
.Lx437_11:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx437_42:              cmp              rdx, 2;                              jbe   .Lx437_12
                        mov              rdi, qword ptr [rcx + 40]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 48]             # c
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx437_43
.Lx437_12:              mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx437_43:              lea              rcx, [rip + pad_right_γ]
                        lea              rax, [rip + pad_right_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n38_statement_begin_α];  jmp   rax
pad_right_γ:            mov              rdi, qword ptr [r9 + 64]             # pad_right
                        mov              rsi, qword ptr [r9 + 72]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        cmp              rdx, 0;                              jbe   .Lx437_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx437_110
.Lx437_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx437_110:             cmp              rdx, 1;                              jbe   .Lx437_81
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 32], rax             # n
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx437_111
.Lx437_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 40], rax
.Lx437_111:             cmp              rdx, 2;                              jbe   .Lx437_82
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 48], rax             # c
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx437_112
.Lx437_82:              mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 56], rax
.Lx437_112:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 96
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
pad_right_ω:            mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax             # pad_right
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        cmp              rdx, 0;                              jbe   .Lx437_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx437_180
.Lx437_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx437_180:             cmp              rdx, 1;                              jbe   .Lx437_151
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 32], rax             # n
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx437_181
.Lx437_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 40], rax
.Lx437_181:             cmp              rdx, 2;                              jbe   .Lx437_152
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 48], rax             # c
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx437_182
.Lx437_152:             mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 56], rax
.Lx437_182:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 96
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx437_245:
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_end_α:    mov              r11, 38
                        mov              r10, 7;                              jmp   n66_statement_begin_α
n37_statement_end_β:    mov              r11, 38;                             jmp   n66_statement_begin_α
#=======================================================================================================================
# pad_right  <stmt 8, line 45: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_begin_α:  mov              r11, 39
                        mov              r10, 8;                              jmp   n39_var_α
n38_statement_begin_β:  mov              r11, 39;                             jmp   n45_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n39_var_α:              sub              rsp, 16
                        mov              r11, 40
                        mov              rax, qword ptr [r9 + 48]             # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n40_lit_string_α
n39_var_β:              mov              r11, 40
                        add              rsp, 16;                             jmp   n38_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n40_lit_string_α:       sub              rsp, 16
                        mov              r11, 41
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx443_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n41_ident_α
n40_lit_string_β:       mov              r11, 41
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n38_statement_begin_β
.Lx443_0:               .quad            .Lx443_0_s
.Lx443_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n41_ident_α:            sub              rsp, 16
                        mov              r11, 42
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            jne   .Lx445_240
                        add              rsp, 16;                             jmp   n40_lit_string_β
.Lx445_240:                                                                   jmp   n42_lit_string_α
n41_ident_β:            mov              r11, 42
                        add              rsp, 16;                             jmp   n40_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_string_α:       sub              rsp, 16
                        mov              r11, 43
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx446_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n43_assign_α
n42_lit_string_β:       mov              r11, 43
                        add              rsp, 16;                             jmp   n41_ident_β
.Lx446_0:               .quad            .Lx446_0_s
.Lx446_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n43_assign_α:           mov              r11, 44
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # c
                        mov              qword ptr [r9 + 56], rdx;            jmp   n44_statement_end_α
n43_assign_β:           mov              r11, 44;                             jmp   n38_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_end_α:    mov              r11, 45
                        mov              r10, 8
                        add              rsp, 64;                             jmp   n45_statement_begin_α
n44_statement_end_β:    mov              r11, 45
                        add              rsp, 64;                             jmp   n45_statement_begin_α
#=======================================================================================================================
#         OUTPUT = index('foobar', 'xyz') ;* 0
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_begin_α:  mov              r11, 46
                        mov              r10, 9;                              jmp   n46_var_α
n45_statement_begin_β:  mov              r11, 46;                             jmp   n55_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n46_var_α:              sub              rsp, 16
                        mov              r11, 47
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n47_call_α
n46_var_β:              mov              r11, 47
                        add              rsp, 16;                             jmp   n45_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n47_call_α:             sub              rsp, 16
                        mov              r11, 48
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd454:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd454]
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
                        cmp              al, 104;                             jne   .Lx453_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n45_statement_begin_β
.Lx453_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n48_var_α
n47_call_β:             mov              r11, 48
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n45_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n48_var_α:              sub              rsp, 16
                        mov              r11, 49
                        mov              rax, qword ptr [r9 + 32]             # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n49_coerce_numeric_α
n48_var_β:              mov              r11, 49
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n45_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n49_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 50
                        mov              eax, dword ptr [rsp + 32]            # call
                        cmp              al, 5;                               je    .Lx457_1
                        cmp              al, 3;                               jne   .Lx457_0
                        mov              eax, dword ptr [rsp + 16]            # var
                        cmp              al, 3;                               jne   .Lx457_0
.Lx457_1:               mov              rax, qword ptr [rsp + 32]            # call
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # call
                        mov              qword ptr [rsp + 8], rax;            jmp   n50_coerce_numeric_α
.Lx457_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 109
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n50_coerce_numeric_α
n49_coerce_numeric_β:   mov              r11, 50
                        add              rsp, 16;                             jmp   n48_var_β
#-----------------------------------------------------------------------------------------------------------------------
n50_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 51
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              al, 5;                               je    .Lx459_1
                        cmp              al, 3;                               jne   .Lx459_0
                        mov              eax, dword ptr [rsp + 48]            # call
                        cmp              al, 3;                               jne   .Lx459_0
.Lx459_1:               mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n51_cmp_test_α
.Lx459_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # call
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 110
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n51_cmp_test_α
n50_coerce_numeric_β:   mov              r11, 51
                        add              rsp, 16;                             jmp   n49_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n51_cmp_test_α:         sub              rsp, 16
                        mov              r11, 52
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx461_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jge   .Lx461_239
                        add              rsp, 16;                             jmp   n50_coerce_numeric_β
.Lx461_239:                                                                   jmp   n52_var_α
.Lx461_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_cmp_d@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            jns   .Lx461_240
                        add              rsp, 16;                             jmp   n50_coerce_numeric_β
.Lx461_240:                                                                   jmp   n52_var_α
n51_cmp_test_β:         mov              r11, 52
                        add              rsp, 16;                             jmp   n50_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n52_var_α:              sub              rsp, 16
                        mov              r11, 53
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n53_assign_α
n52_var_β:              mov              r11, 53
                        add              rsp, 16;                             jmp   n51_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n53_assign_α:           mov              r11, 54
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax             # pad_right
                        mov              qword ptr [r9 + 72], rdx;            jmp   n54_statement_end_α
n53_assign_β:           mov              r11, 54;                             jmp   n45_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_end_α:    mov              r11, 55
                        mov              r10, 9
                        add              rsp, 112;                            jmp   RETURN
n54_statement_end_β:    mov              r11, 55
                        add              rsp, 112;                            jmp   n55_statement_begin_α
#=======================================================================================================================
#         <stmt 10, line 47: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_begin_α:  mov              r11, 56
                        mov              r10, 10;                             jmp   n56_var_α
n55_statement_begin_β:  mov              r11, 56;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n56_var_α:              sub              rsp, 16
                        mov              r11, 57
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n57_var_α
n56_var_β:              mov              r11, 57
                        add              rsp, 16;                             jmp   n55_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n57_var_α:              sub              rsp, 16
                        mov              r11, 58
                        mov              rax, qword ptr [r9 + 48]             # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n58_var_α
n57_var_β:              mov              r11, 58
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n55_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n58_var_α:              sub              rsp, 16
                        mov              r11, 59
                        mov              rax, qword ptr [r9 + 32]             # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n59_var_α
n58_var_β:              mov              r11, 59
                        add              rsp, 16;                             jmp   n57_var_β
#-----------------------------------------------------------------------------------------------------------------------
n59_var_α:              sub              rsp, 16
                        mov              r11, 60
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n60_call_α
n59_var_β:              mov              r11, 60
                        add              rsp, 16;                             jmp   n58_var_β
#-----------------------------------------------------------------------------------------------------------------------
n60_call_α:             sub              rsp, 16
                        mov              r11, 61
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd473:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd473]
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
                        cmp              al, 104;                             jne   .Lx472_240
                        add              rsp, 16;                             jmp   n59_var_β
.Lx472_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n61_binop_α
n60_call_β:             mov              r11, 61
                        add              rsp, 16;                             jmp   n59_var_β
#-----------------------------------------------------------------------------------------------------------------------
n61_binop_α:            sub              rsp, 16
                        mov              r11, 62
                        mov              eax, dword ptr [rsp + 48]            # var
                        mov              ecx, dword ptr [rsp + 16]            # call
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx474_2
                        mov              rax, qword ptr [rsp + 56]            # var
                        mov              rdx, qword ptr [rsp + 24]            # call
                        sub              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx474_7
.Lx474_2:               and              edx, 1;                              jz    .Lx474_0
                        mov              rsi, qword ptr [rsp + 56]            # var
                        mov              rdi, qword ptr [rsp + 24]            # call
                        cmp              al, 5;                               je    .Lx474_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx474_4
.Lx474_3:               movq             xmm0, rsi
.Lx474_4:               cmp              cl, 5;                               je    .Lx474_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx474_6
.Lx474_5:               movq             xmm1, rdi
.Lx474_6:               subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx474_7:                                                                     jmp   n62_call_α
.Lx474_0:               mov              rdi, qword ptr [rsp + 48]            # var
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_sub@PLT
                        cmp              al, 104;                             jne   .Lx474_240
                        add              rsp, 32;                             jmp   n59_var_β
.Lx474_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n62_call_α
n61_binop_β:            mov              r11, 62
                        add              rsp, 32;                             jmp   n59_var_β
#-----------------------------------------------------------------------------------------------------------------------
n62_call_α:             sub              rsp, 16
                        mov              r11, 63
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
.Lrkfnzd476:            .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd476]
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
                        cmp              al, 104;                             jne   .Lx475_240
                        add              rsp, 16;                             jmp   n61_binop_β
.Lx475_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n63_binop_α
n62_call_β:             mov              r11, 63
                        add              rsp, 16;                             jmp   n61_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n63_binop_α:            sub              rsp, 16
                        mov              r11, 64
                        mov              rdi, qword ptr [rsp + 112]           # var
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 16]            # call
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n64_assign_α
n63_binop_β:            mov              r11, 64
                        add              rsp, 32;                             jmp   n61_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n64_assign_α:           mov              r11, 65
                        mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax             # pad_right
                        mov              qword ptr [r9 + 72], rdx;            jmp   n65_statement_end_α
n64_assign_β:           mov              r11, 65;                             jmp   n55_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_end_α:    mov              r11, 66
                        mov              r10, 10
                        add              rsp, 128;                            jmp   RETURN
n65_statement_end_β:    mov              r11, 66
                        add              rsp, 128;                            jmp   RETURN
#=======================================================================================================================
# pad_right_end  <stmt 11, line 49: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_begin_α:  mov              r11, 67
                        mov              r10, 11;                             jmp   n67_statement_end_α
n66_statement_begin_β:  mov              r11, 67;                             jmp   n68_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_end_α:    mov              r11, 68
                        mov              r10, 11;                             jmp   n68_statement_begin_α
n67_statement_end_β:    mov              r11, 68;                             jmp   n68_statement_begin_α
#=======================================================================================================================
#         <stmt 12, line 50: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_begin_α:  mov              r11, 69
                        mov              r10, 12;                             jmp   n69_define_α
n68_statement_begin_β:  mov              r11, 69;                             jmp   n102_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n69_define_α:           mov              r11, 70
                        mov              rdi, qword ptr [rip + .Lx488_0]
                        mov              rsi, qword ptr [rip + .Lx488_1]
                        mov              edx, 3
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n71_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx488_0]
                        lea              rsi, [rip + ltrim_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n70_statement_end_α
n69_define_β:           mov              r11, 70;                             jmp   n68_statement_begin_β
.Lx488_0:               .quad            .Lx488_0_s
.Lx488_0_s:             .string          "ltrim"
.Lx488_1:               .quad            .Lx488_1_s
.Lx488_1_s:             .string          "s,ws,r"
                                                                              jmp   .Lx489_245
#-----------------------------------------------------------------------------------------------------------------------
ltrim_α:                sub              rsp, 96
                        mov              rax, qword ptr [r9 + 96]             # ws
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              rax, qword ptr [r9 + 112]            # r
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 112], 0
                        mov              qword ptr [r9 + 120], 0
                        mov              rax, qword ptr [r9 + 80]             # ltrim
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
                        mov              qword ptr [rsp + 64], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        cmp              rdx, 0;                              jbe   .Lx489_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx489_41
.Lx489_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx489_41:              lea              rcx, [rip + ltrim_γ]
                        lea              rax, [rip + ltrim_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n71_statement_begin_α];  jmp   rax
ltrim_γ:                mov              rdi, qword ptr [r9 + 80]             # ltrim
                        mov              rsi, qword ptr [r9 + 88]
                        mov              rcx, qword ptr [rsp + 64]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax            # r
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax             # ws
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rdx, 0;                              jbe   .Lx489_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx489_110
.Lx489_80:              mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 24], rax
.Lx489_110:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 96
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
ltrim_ω:                mov              rcx, qword ptr [rsp + 64]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 80], rax             # ltrim
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 112], rax            # r
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 120], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax             # ws
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rdx, 0;                              jbe   .Lx489_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx489_180
.Lx489_150:             mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 24], rax
.Lx489_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 96
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx489_245:
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_end_α:    mov              r11, 71
                        mov              r10, 12;                             jmp   n102_statement_begin_α
n70_statement_end_β:    mov              r11, 71;                             jmp   n102_statement_begin_α
#=======================================================================================================================
# ltrim  <stmt 13, line 51: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_begin_α:  mov              r11, 72
                        mov              r10, 13;                             jmp   n72_lit_string_α
n71_statement_begin_β:  mov              r11, 72;                             jmp   n84_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n72_lit_string_α:       sub              rsp, 16
                        mov              r11, 73
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx494_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n73_lit_integer_α
n72_lit_string_β:       mov              r11, 73
                        add              rsp, 16;                             jmp   n71_statement_begin_β
.Lx494_0:               .quad            .Lx494_0_s
.Lx494_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n73_lit_integer_α:      sub              rsp, 16
                        mov              r11, 74
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx495_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n74_call_α
n73_lit_integer_β:      mov              r11, 74
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n71_statement_begin_β
.Lx495_0:               .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n74_call_α:             sub              rsp, 16
                        mov              r11, 75
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd75:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd75]
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
                        cmp              al, 104;                             jne   .Lx496_240
                        add              rsp, 16;                             jmp   n73_lit_integer_β
.Lx496_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n75_binop_α
n74_call_β:             mov              r11, 75
                        add              rsp, 16;                             jmp   n73_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n75_binop_α:            sub              rsp, 16
                        mov              r11, 76
                        mov              rdi, qword ptr [rsp + 48]            # lit_string
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n76_lit_integer_α
n75_binop_β:            mov              r11, 76
                        add              rsp, 32;                             jmp   n73_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n76_lit_integer_α:      sub              rsp, 16
                        mov              r11, 77
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx498_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n77_call_α
n76_lit_integer_β:      mov              r11, 77
                        add              rsp, 16;                             jmp   n75_binop_β
.Lx498_0:               .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n77_call_α:             sub              rsp, 16
                        mov              r11, 78
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd78:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd78]
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
                        cmp              al, 104;                             jne   .Lx499_240
                        add              rsp, 16;                             jmp   n76_lit_integer_β
.Lx499_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n78_binop_α
n77_call_β:             mov              r11, 78
                        add              rsp, 16;                             jmp   n76_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n78_binop_α:            sub              rsp, 16
                        mov              r11, 79
                        mov              rdi, qword ptr [rsp + 48]            # binop
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n79_lit_integer_α
n78_binop_β:            mov              r11, 79
                        add              rsp, 32;                             jmp   n76_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_integer_α:      sub              rsp, 16
                        mov              r11, 80
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx501_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n80_call_α
n79_lit_integer_β:      mov              r11, 80
                        add              rsp, 16;                             jmp   n78_binop_β
.Lx501_0:               .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n80_call_α:             sub              rsp, 16
                        mov              r11, 81
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd81:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd81]
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
                        cmp              al, 104;                             jne   .Lx502_240
                        add              rsp, 16;                             jmp   n79_lit_integer_β
.Lx502_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n81_binop_α
n80_call_β:             mov              r11, 81
                        add              rsp, 16;                             jmp   n79_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n81_binop_α:            sub              rsp, 16
                        mov              r11, 82
                        mov              rdi, qword ptr [rsp + 48]            # binop
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n82_assign_α
n81_binop_β:            mov              r11, 82
                        add              rsp, 32;                             jmp   n79_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n82_assign_α:           mov              r11, 83
                        mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax             # ws
                        mov              qword ptr [r9 + 104], rdx;           jmp   n83_statement_end_α
n82_assign_β:           mov              r11, 83;                             jmp   n71_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n83_statement_end_α:    mov              r11, 84
                        mov              r10, 13
                        add              rsp, 160;                            jmp   n84_statement_begin_α
n83_statement_end_β:    mov              r11, 84
                        add              rsp, 160;                            jmp   n84_statement_begin_α
#=======================================================================================================================
#         <stmt 14, line 52: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n84_statement_begin_α:  mov              r11, 85
                        mov              r10, 14;                             jmp   n85_var_α
n84_statement_begin_β:  mov              r11, 85;                             jmp   n98_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n85_var_α:              sub              rsp, 16
                        mov              r11, 86
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n86_match_begin_α
n85_var_β:              mov              r11, 86
                        add              rsp, 16;                             jmp   n98_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n86_match_begin_α:      mov              r11, 87
                        mov              rdi, qword ptr [rsp + 0]             # var
                        mov              rsi, qword ptr [rsp + 8]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 40
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
.Lx511_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx511_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n87_match_pos_α
n86_match_begin_β:      mov              r11, 87
.Lx511_13:              lea              rsp, [rbp + -72]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx511_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx511_1
                                                                              jmp   .Lx511_0
.Lx511_1:
n86_match_begin_af:     mov              r11, 87
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
                        pop              rbp;                                 jmp   n95_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n87_match_pos_α:        mov              r11, 88
                        mov              rax, 0
                        cmp              r14d, eax;                           jne   n86_match_begin_β
                                                                              jmp   n88_match_alternate_α
n87_match_pos_β:        mov              r11, 88;                             jmp   n86_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n88_match_alternate_α:  mov              r11, 89
                        sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx514_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n97_match_span_α
.Lx514_21:              lea              rax, [rip + .Lx514_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n96_match_lit_α
n88_match_alternate_s0: mov              r11, 89
                        lea              rax, [rip + .Lx514_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n88_match_alternate_as
n88_match_alternate_s1: mov              r11, 89
                        lea              rax, [rip + .Lx514_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n88_match_alternate_as
.Lx514_40:                                                                    jmp   n97_match_span_β
.Lx514_41:                                                                    jmp   n96_match_lit_β
n88_match_alternate_as: mov              r11, 89;                             jmp   n89_match_assign_save_α
n88_match_alternate_β:  mov              r11, 89
                        mov              rax, qword ptr [rsp + 8];            jmp   rax
n88_match_alternate_af: mov              r11, 89
                        mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx514_19:              add              rsp, 32;                             jmp   n87_match_pos_β
#-----------------------------------------------------------------------------------------------------------------------
n89_match_assign_save_α:
                        sub              rsp, 16
                        mov              r11, 90
                        mov              dword ptr [rsp + 0], r14d;           jmp   n90_match_rem_α
n89_match_assign_save_β:
                        mov              r11, 90
                        add              rsp, 16;                             jmp   n88_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n90_match_rem_α:        sub              rsp, 16
                        mov              r11, 91
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d;                          jmp   n91_match_assign_cond_α
n90_match_rem_β:        mov              r11, 91
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n88_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n91_match_assign_cond_α:
                        mov              r11, 92
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n92_match_end_α
n91_match_assign_cond_β:
                        mov              r11, 92
                        sub              r12, 24;                             jmp   n90_match_rem_β
#-----------------------------------------------------------------------------------------------------------------------
n92_match_end_α:        mov              r11, 93
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              eax, dword ptr [rbp + -40]           # repl_start
                        mov              dword ptr [rbp + -48], eax
                        mov              qword ptr [rbp + -56], r14           # repl_end
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
                        test             rax, rax;                            je    .Lx521_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n86_match_begin_af
.Lx521_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              eax, dword ptr [rbp + -48]           # repl_start
                        mov              dword ptr [r12 + 0], eax
                        mov              rax, qword ptr [rbp + -56]           # repl_end
                        mov              qword ptr [r12 + 8], rax
                        add              r12, 16
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n93_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n93_lit_string_α:       sub              rsp, 16
                        mov              r11, 94
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx522_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n94_match_replace_α
n93_lit_string_β:       mov              r11, 94
                        add              rsp, 16
                        add              rsp, 48;                             jmp   n98_statement_begin_α
.Lx522_0:               .quad            .Lx522_0_s
.Lx522_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n94_match_replace_α:    mov              r11, 95
                        mov              rdi, qword ptr [rip + .Lx524_0]
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
                        add              rsp, 16;                             jmp   .Lx524_1
.Lx524_0:               .quad            .Lx524_0_s
.Lx524_0_s:             .string          "s"
.Lx524_1:                                                                     jmp   n95_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n95_statement_end_α:    mov              r11, 96
                        mov              r10, 14
                        add              rsp, 16;                             jmp   n98_statement_begin_α
n95_statement_end_β:    mov              r11, 96
                        add              rsp, 64;                             jmp   n98_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n96_match_lit_α:        mov              r11, 97;                             jmp   n88_match_alternate_s1
n96_match_lit_β:        mov              r11, 97;                             jmp   n88_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n97_match_span_α:       mov              r11, 98
                        lea              rdi, [rip + .S1]
                        lea              rsi, [rbp + -80]
                        lea              rdx, [rbp + -72]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_pat_prim_str@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            js    n88_match_alternate_af
                        mov              r8, qword ptr [rbp + -80]
                        mov              dword ptr [rbp + -80], 0
.Lx530_0:               mov              eax, r14d
                        add              eax, dword ptr [rbp + -80]
                        cmp              eax, r15d;                           jge   .Lx530_1
                        movsxd           rcx, eax
                        movzx            esi, byte ptr [r13+rcx]
                        mov              eax, dword ptr [rbp + -72]
                        mov              edx, 0
.Lx530_2:               cmp              edx, eax;                            jge   .Lx530_1
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi;                            je    .Lx530_3
                        add              edx, 1;                              jmp   .Lx530_2
.Lx530_3:               add              dword ptr [rbp + -80], 1;            jmp   .Lx530_0
.Lx530_1:               mov              eax, dword ptr [rbp + -80]
                        test             eax, eax;                            jle   n88_match_alternate_af
                        mov              edx, r14d
                        mov              dword ptr [rbp + -76], edx
                        add              edx, eax
                        mov              r14d, edx;                           jmp   n88_match_alternate_s0
n97_match_span_β:       mov              r11, 98
                        mov              r14d, dword ptr [rbp + -76];         jmp   n88_match_alternate_af
#=======================================================================================================================
#         <stmt 15, line 53: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n98_statement_begin_α:  mov              r11, 99
                        mov              r10, 15;                             jmp   n99_var_α
n98_statement_begin_β:  mov              r11, 99;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n99_var_α:              sub              rsp, 16
                        mov              r11, 100
                        mov              rax, qword ptr [r9 + 112]            # r
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n100_assign_α
n99_var_β:              mov              r11, 100
                        add              rsp, 16;                             jmp   n98_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n100_assign_α:          mov              r11, 101
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax             # ltrim
                        mov              qword ptr [r9 + 88], rdx;            jmp   n101_statement_end_α
n100_assign_β:          mov              r11, 101;                            jmp   n98_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n101_statement_end_α:   mov              r11, 102
                        mov              r10, 15
                        add              rsp, 16;                             jmp   RETURN
n101_statement_end_β:   mov              r11, 102
                        add              rsp, 16;                             jmp   RETURN
#=======================================================================================================================
# ltrim_end  <stmt 16, line 55: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_begin_α: mov              r11, 103
                        mov              r10, 16;                             jmp   n103_statement_end_α
n102_statement_begin_β: mov              r11, 103;                            jmp   n104_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_end_α:   mov              r11, 104
                        mov              r10, 16;                             jmp   n104_statement_begin_α
n103_statement_end_β:   mov              r11, 104;                            jmp   n104_statement_begin_α
#=======================================================================================================================
#         <stmt 17, line 56: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_begin_α: mov              r11, 105
                        mov              r10, 17;                             jmp   n105_define_α
n104_statement_begin_β: mov              r11, 105;                            jmp   n160_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n105_define_α:          mov              r11, 106
                        mov              rdi, qword ptr [rip + .Lx544_0]
                        mov              rsi, qword ptr [rip + .Lx544_1]
                        mov              edx, 4
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n107_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx544_0]
                        lea              rsi, [rip + rtrim_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n106_statement_end_α
n105_define_β:          mov              r11, 106;                            jmp   n104_statement_begin_β
.Lx544_0:               .quad            .Lx544_0_s
.Lx544_0_s:             .string          "rtrim"
.Lx544_1:               .quad            .Lx544_1_s
.Lx544_1_s:             .string          "s,ws,i,ch"
                                                                              jmp   .Lx545_245
#-----------------------------------------------------------------------------------------------------------------------
rtrim_α:                sub              rsp, 112
                        mov              rax, qword ptr [r9 + 96]             # ws
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              rax, qword ptr [r9 + 144]            # i
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 144], 0
                        mov              qword ptr [r9 + 152], 0
                        mov              rax, qword ptr [r9 + 160]            # ch
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 160], 0
                        mov              qword ptr [r9 + 168], 0
                        mov              rax, qword ptr [r9 + 128]            # rtrim
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
                        mov              qword ptr [rsp + 80], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 112]
                        cmp              rdx, 0;                              jbe   .Lx545_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx545_41
.Lx545_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 96], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 104], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx545_41:              lea              rcx, [rip + rtrim_γ]
                        lea              rax, [rip + rtrim_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n107_statement_begin_α]; jmp   rax
rtrim_γ:                mov              rdi, qword ptr [r9 + 128]            # rtrim
                        mov              rsi, qword ptr [r9 + 136]
                        mov              rcx, qword ptr [rsp + 80]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 112]
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 160], rax            # ch
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 168], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 144], rax            # i
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 152], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax             # ws
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rdx, 0;                              jbe   .Lx545_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx545_110
.Lx545_80:              mov              rax, qword ptr [rsp + 96]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [r9 + 24], rax
.Lx545_110:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 112
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
rtrim_ω:                mov              rcx, qword ptr [rsp + 80]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 112]
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax            # rtrim
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 160], rax            # ch
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 168], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 144], rax            # i
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 152], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax             # ws
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        cmp              rdx, 0;                              jbe   .Lx545_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx545_180
.Lx545_150:             mov              rax, qword ptr [rsp + 96]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [r9 + 24], rax
.Lx545_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 112
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx545_245:
#-----------------------------------------------------------------------------------------------------------------------
n106_statement_end_α:   mov              r11, 107
                        mov              r10, 17;                             jmp   n160_statement_begin_α
n106_statement_end_β:   mov              r11, 107;                            jmp   n160_statement_begin_α
#=======================================================================================================================
# rtrim  <stmt 18, line 57: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n107_statement_begin_α: mov              r11, 108
                        mov              r10, 18;                             jmp   n108_lit_string_α
n107_statement_begin_β: mov              r11, 108;                            jmp   n120_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n108_lit_string_α:      sub              rsp, 16
                        mov              r11, 109
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx550_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n109_lit_integer_α
n108_lit_string_β:      mov              r11, 109
                        add              rsp, 16;                             jmp   n107_statement_begin_β
.Lx550_0:               .quad            .Lx550_0_s
.Lx550_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n109_lit_integer_α:     sub              rsp, 16
                        mov              r11, 110
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx551_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n110_call_α
n109_lit_integer_β:     mov              r11, 110
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n107_statement_begin_β
.Lx551_0:               .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n110_call_α:            sub              rsp, 16
                        mov              r11, 111
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd111:        .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd111]
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
                        cmp              al, 104;                             jne   .Lx552_240
                        add              rsp, 16;                             jmp   n109_lit_integer_β
.Lx552_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n111_binop_α
n110_call_β:            mov              r11, 111
                        add              rsp, 16;                             jmp   n109_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n111_binop_α:           sub              rsp, 16
                        mov              r11, 112
                        mov              rdi, qword ptr [rsp + 48]            # lit_string
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n112_lit_integer_α
n111_binop_β:           mov              r11, 112
                        add              rsp, 32;                             jmp   n109_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n112_lit_integer_α:     sub              rsp, 16
                        mov              r11, 113
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx554_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n113_call_α
n112_lit_integer_β:     mov              r11, 113
                        add              rsp, 16;                             jmp   n111_binop_β
.Lx554_0:               .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n113_call_α:            sub              rsp, 16
                        mov              r11, 114
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd114:        .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd114]
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
                        cmp              al, 104;                             jne   .Lx555_240
                        add              rsp, 16;                             jmp   n112_lit_integer_β
.Lx555_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n114_binop_α
n113_call_β:            mov              r11, 114
                        add              rsp, 16;                             jmp   n112_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n114_binop_α:           sub              rsp, 16
                        mov              r11, 115
                        mov              rdi, qword ptr [rsp + 48]            # binop
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n115_lit_integer_α
n114_binop_β:           mov              r11, 115
                        add              rsp, 32;                             jmp   n112_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n115_lit_integer_α:     sub              rsp, 16
                        mov              r11, 116
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx557_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n116_call_α
n115_lit_integer_β:     mov              r11, 116
                        add              rsp, 16;                             jmp   n114_binop_β
.Lx557_0:               .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n116_call_α:            sub              rsp, 16
                        mov              r11, 117
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd117:        .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd117]
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
                        cmp              al, 104;                             jne   .Lx558_240
                        add              rsp, 16;                             jmp   n115_lit_integer_β
.Lx558_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n117_binop_α
n116_call_β:            mov              r11, 117
                        add              rsp, 16;                             jmp   n115_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n117_binop_α:           sub              rsp, 16
                        mov              r11, 118
                        mov              rdi, qword ptr [rsp + 48]            # binop
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n118_assign_α
n117_binop_β:           mov              r11, 118
                        add              rsp, 32;                             jmp   n115_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n118_assign_α:          mov              r11, 119
                        mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax             # ws
                        mov              qword ptr [r9 + 104], rdx;           jmp   n119_statement_end_α
n118_assign_β:          mov              r11, 119;                            jmp   n107_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n119_statement_end_α:   mov              r11, 120
                        mov              r10, 18
                        add              rsp, 160;                            jmp   n120_statement_begin_α
n119_statement_end_β:   mov              r11, 120
                        add              rsp, 160;                            jmp   n120_statement_begin_α
#=======================================================================================================================
#         <stmt 19, line 58: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n120_statement_begin_α: mov              r11, 121
                        mov              r10, 19;                             jmp   n121_var_α
n120_statement_begin_β: mov              r11, 121;                            jmp   n125_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n121_var_α:             sub              rsp, 16
                        mov              r11, 122
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n122_call_α
n121_var_β:             mov              r11, 122
                        add              rsp, 16;                             jmp   n120_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n122_call_α:            sub              rsp, 16
                        mov              r11, 123
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd567:            .string          "SIZE"
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
                        cmp              al, 104;                             jne   .Lx566_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n120_statement_begin_β
.Lx566_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n123_assign_α
n122_call_β:            mov              r11, 123
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n120_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n123_assign_α:          mov              r11, 124
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # i
                        mov              qword ptr [r9 + 152], rdx;           jmp   n124_statement_end_α
n123_assign_β:          mov              r11, 124;                            jmp   n120_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n124_statement_end_α:   mov              r11, 125
                        mov              r10, 19
                        add              rsp, 32;                             jmp   n125_statement_begin_α
n124_statement_end_β:   mov              r11, 125
                        add              rsp, 32;                             jmp   n125_statement_begin_α
#=======================================================================================================================
# rtrim0  <stmt 20, line 59: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n125_statement_begin_α: mov              r11, 126
                        mov              r10, 20;                             jmp   n126_var_α
n125_statement_begin_β: mov              r11, 126;                            jmp   n132_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n126_var_α:             sub              rsp, 16
                        mov              r11, 127
                        mov              rax, qword ptr [r9 + 144]            # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n127_lit_integer_α
n126_var_β:             mov              r11, 127
                        add              rsp, 16;                             jmp   n125_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n127_lit_integer_α:     sub              rsp, 16
                        mov              r11, 128
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx574_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n128_coerce_numeric_α
n127_lit_integer_β:     mov              r11, 128
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n125_statement_begin_β
.Lx574_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n128_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 129
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              al, 5;                               je    .Lx576_1
                        cmp              al, 3;                               jne   .Lx576_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx576_0
.Lx576_1:               mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n129_coerce_numeric_α
.Lx576_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 118
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n129_coerce_numeric_α
n128_coerce_numeric_β:  mov              r11, 129
                        add              rsp, 16;                             jmp   n127_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n129_coerce_numeric_α:  sub              rsp, 16
                        mov              r11, 130
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx578_1
                        cmp              al, 3;                               jne   .Lx578_0
                        mov              eax, dword ptr [rsp + 48]            # var
                        cmp              al, 3;                               jne   .Lx578_0
.Lx578_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n130_cmp_test_α
.Lx578_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 119
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n130_cmp_test_α
n129_coerce_numeric_β:  mov              r11, 130
                        add              rsp, 16;                             jmp   n128_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n130_cmp_test_α:        sub              rsp, 16
                        mov              r11, 131
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx580_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jle   .Lx580_239
                        add              rsp, 16;                             jmp   n129_coerce_numeric_β
.Lx580_239:                                                                   jmp   n131_statement_end_α
.Lx580_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_cmp_d@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            jle   .Lx580_240
                        add              rsp, 16;                             jmp   n129_coerce_numeric_β
.Lx580_240:                                                                   jmp   n131_statement_end_α
n130_cmp_test_β:        mov              r11, 131
                        add              rsp, 16;                             jmp   n129_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n131_statement_end_α:   mov              r11, 132
                        mov              r10, 20
                        add              rsp, 80;                             jmp   n153_statement_begin_α
n131_statement_end_β:   mov              r11, 132
                        add              rsp, 80;                             jmp   n132_statement_begin_α
#=======================================================================================================================
#         <stmt 21, line 60: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n132_statement_begin_α: mov              r11, 133
                        mov              r10, 21;                             jmp   n133_var_α
n132_statement_begin_β: mov              r11, 133;                            jmp   n139_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n133_var_α:             sub              rsp, 16
                        mov              r11, 134
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n134_var_α
n133_var_β:             mov              r11, 134
                        add              rsp, 16;                             jmp   n132_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n134_var_α:             sub              rsp, 16
                        mov              r11, 135
                        mov              rax, qword ptr [r9 + 144]            # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n135_lit_integer_α
n134_var_β:             mov              r11, 135
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n132_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n135_lit_integer_α:     sub              rsp, 16
                        mov              r11, 136
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx587_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n136_call_α
n135_lit_integer_β:     mov              r11, 136
                        add              rsp, 16;                             jmp   n134_var_β
.Lx587_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n136_call_α:            sub              rsp, 16
                        mov              r11, 137
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
.Lrkfnzd589:            .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd589]
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
                        cmp              al, 104;                             jne   .Lx588_240
                        add              rsp, 16;                             jmp   n135_lit_integer_β
.Lx588_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n137_assign_α
n136_call_β:            mov              r11, 137
                        add              rsp, 16;                             jmp   n135_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n137_assign_α:          mov              r11, 138
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 160], rax            # ch
                        mov              qword ptr [r9 + 168], rdx;           jmp   n138_statement_end_α
n137_assign_β:          mov              r11, 138;                            jmp   n132_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n138_statement_end_α:   mov              r11, 139
                        mov              r10, 21
                        add              rsp, 64;                             jmp   n139_statement_begin_α
n138_statement_end_β:   mov              r11, 139
                        add              rsp, 64;                             jmp   n139_statement_begin_α
#=======================================================================================================================
#         <stmt 22, line 61: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n139_statement_begin_α: mov              r11, 140
                        mov              r10, 22;                             jmp   n140_var_α
n139_statement_begin_β: mov              r11, 140;                            jmp   n153_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n140_var_α:             sub              rsp, 16
                        mov              r11, 141
                        mov              rax, qword ptr [r9 + 160]            # ch
                        mov              rdx, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n141_var_α
n140_var_β:             mov              r11, 141
                        add              rsp, 16;                             jmp   n153_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n141_var_α:             sub              rsp, 16
                        mov              r11, 142
                        mov              rax, qword ptr [r9 + 96]             # ws
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n142_coerce_string_α
n141_var_β:             mov              r11, 142
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n153_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n142_coerce_string_α:   sub              rsp, 16
                        mov              r11, 143
                        lea              rdi, [rsp + 16]                      # var
                        lea              rsi, [rsp + 0]                       # result
                        mov              rdx, 3866683
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_str_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n143_match_begin_α
n142_coerce_string_β:   mov              r11, 143
                        add              rsp, 16;                             jmp   n141_var_β
#-----------------------------------------------------------------------------------------------------------------------
n143_match_begin_α:     mov              r11, 144
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
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
.Lx600_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx600_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n144_match_any_α
n143_match_begin_β:     mov              r11, 144
.Lx600_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx600_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx600_1
                                                                              jmp   .Lx600_0
.Lx600_1:
n143_match_begin_af:    mov              r11, 144
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
                        pop              rbp;                                 jmp   n142_coerce_string_β
#-----------------------------------------------------------------------------------------------------------------------
n144_match_any_α:       mov              r11, 145
                        mov              eax, r14d
                        cmp              eax, r15d;                           jge   n143_match_begin_β
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        mov              edi, esi
                        mov              rsi, qword ptr [rsp + 72]            # coerce_string
                        mov              edx, dword ptr [rsp + 68]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_sg_member@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            je    n143_match_begin_β
                        add              r14d, 1;                             jmp   n145_match_end_α
n144_match_any_β:       mov              r11, 145
                        sub              r14d, 1;                             jmp   n143_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n145_match_end_α:       mov              r11, 146
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
                        test             rax, rax;                            je    .Lx604_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n143_match_begin_af
.Lx604_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n146_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n146_statement_end_α:   mov              r11, 147
                        mov              r10, 22
                        add              rsp, 48;                             jmp   n147_statement_begin_α
n146_statement_end_β:   mov              r11, 147
                        add              rsp, 48;                             jmp   n153_statement_begin_α
#=======================================================================================================================
#         <stmt 23, line 62: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n147_statement_begin_α: mov              r11, 148
                        mov              r10, 23;                             jmp   n148_var_α
n147_statement_begin_β: mov              r11, 148;                            jmp   n125_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n148_var_α:             sub              rsp, 16
                        mov              r11, 149
                        mov              rax, qword ptr [r9 + 144]            # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n149_lit_integer_α
n148_var_β:             mov              r11, 149
                        add              rsp, 16;                             jmp   n147_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n149_lit_integer_α:     sub              rsp, 16
                        mov              r11, 150
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx610_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n150_binop_α
n149_lit_integer_β:     mov              r11, 150
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n147_statement_begin_β
.Lx610_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n150_binop_α:           sub              rsp, 16
                        mov              r11, 151
                        mov              ecx, dword ptr [rsp + 32]            # var
                        mov              rax, qword ptr [rsp + 40]
                        cmp              cl, 3;                               jne   .Lx611_2
                        sub              rax, 1
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx611_7
.Lx611_2:               mov              eax, ecx
                        mov              edx, ecx
                        and              edx, 1;                              jz    .Lx611_0
                        mov              rsi, qword ptr [rsp + 40]            # var
                        mov              rdi, 1
                        cmp              al, 5;                               je    .Lx611_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx611_4
.Lx611_3:               movq             xmm0, rsi
.Lx611_4:               cvtsi2sd         xmm1, rdi
                        subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx611_7:                                                                     jmp   n151_assign_α
.Lx611_0:               mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_sub@PLT
                        cmp              al, 104;                             jne   .Lx611_240
                        add              rsp, 16;                             jmp   n149_lit_integer_β
.Lx611_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n151_assign_α
n150_binop_β:           mov              r11, 151
                        add              rsp, 16;                             jmp   n149_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n151_assign_α:          mov              r11, 152
                        mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # i
                        mov              qword ptr [r9 + 152], rdx;           jmp   n152_statement_end_α
n151_assign_β:          mov              r11, 152;                            jmp   n147_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n152_statement_end_α:   mov              r11, 153
                        mov              r10, 23
                        add              rsp, 48;                             jmp   n125_statement_begin_α
n152_statement_end_β:   mov              r11, 153
                        add              rsp, 48;                             jmp   n125_statement_begin_α
#=======================================================================================================================
# rtrim1  <stmt 24, line 63: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n153_statement_begin_α: mov              r11, 154
                        mov              r10, 24;                             jmp   n154_var_α
n153_statement_begin_β: mov              r11, 154;                            jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n154_var_α:             sub              rsp, 16
                        mov              r11, 155
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n155_lit_integer_α
n154_var_β:             mov              r11, 155
                        add              rsp, 16;                             jmp   n153_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n155_lit_integer_α:     sub              rsp, 16
                        mov              r11, 156
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx618_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n156_var_α
n155_lit_integer_β:     mov              r11, 156
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n153_statement_begin_β
.Lx618_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n156_var_α:             sub              rsp, 16
                        mov              r11, 157
                        mov              rax, qword ptr [r9 + 144]            # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n157_call_α
n156_var_β:             mov              r11, 157
                        add              rsp, 16;                             jmp   n155_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n157_call_α:            sub              rsp, 16
                        mov              r11, 158
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
.Lrkfnzd621:            .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd621]
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
                        cmp              al, 104;                             jne   .Lx620_240
                        add              rsp, 16;                             jmp   n156_var_β
.Lx620_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n158_assign_α
n157_call_β:            mov              r11, 158
                        add              rsp, 16;                             jmp   n156_var_β
#-----------------------------------------------------------------------------------------------------------------------
n158_assign_α:          mov              r11, 159
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax            # rtrim
                        mov              qword ptr [r9 + 136], rdx;           jmp   n159_statement_end_α
n158_assign_β:          mov              r11, 159;                            jmp   n153_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n159_statement_end_α:   mov              r11, 160
                        mov              r10, 24
                        add              rsp, 64;                             jmp   RETURN
n159_statement_end_β:   mov              r11, 160
                        add              rsp, 64;                             jmp   RETURN
#=======================================================================================================================
# rtrim_end  <stmt 25, line 65: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n160_statement_begin_α: mov              r11, 161
                        mov              r10, 25;                             jmp   n161_statement_end_α
n160_statement_begin_β: mov              r11, 161;                            jmp   n162_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n161_statement_end_α:   mov              r11, 162
                        mov              r10, 25;                             jmp   n162_statement_begin_α
n161_statement_end_β:   mov              r11, 162;                            jmp   n162_statement_begin_α
#=======================================================================================================================
#         <stmt 26, line 66: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n162_statement_begin_α: mov              r11, 163
                        mov              r10, 26;                             jmp   n163_define_α
n162_statement_begin_β: mov              r11, 163;                            jmp   n171_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n163_define_α:          mov              r11, 164
                        mov              rdi, qword ptr [rip + .Lx632_0]
                        mov              rsi, qword ptr [rip + .Lx632_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n165_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx632_0]
                        lea              rsi, [rip + trimws_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n164_statement_end_α
n163_define_β:          mov              r11, 164;                            jmp   n162_statement_begin_β
.Lx632_0:               .quad            .Lx632_0_s
.Lx632_0_s:             .string          "trimws"
.Lx632_1:               .quad            .Lx632_1_s
.Lx632_1_s:             .string          "s"
                                                                              jmp   .Lx633_245
#-----------------------------------------------------------------------------------------------------------------------
trimws_α:               sub              rsp, 64
                        mov              rax, qword ptr [r9 + 176]            # trimws
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 176], 0
                        mov              qword ptr [r9 + 184], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx633_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx633_41
.Lx633_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx633_41:              lea              rcx, [rip + trimws_γ]
                        lea              rax, [rip + trimws_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n165_statement_begin_α]; jmp   rax
trimws_γ:               mov              rdi, qword ptr [r9 + 176]            # trimws
                        mov              rsi, qword ptr [r9 + 184]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 176], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 184], rax
                        cmp              rdx, 0;                              jbe   .Lx633_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx633_110
.Lx633_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx633_110:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 64
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
trimws_ω:               mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 176], rax            # trimws
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 184], rax
                        cmp              rdx, 0;                              jbe   .Lx633_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx633_180
.Lx633_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx633_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx633_245:
#-----------------------------------------------------------------------------------------------------------------------
n164_statement_end_α:   mov              r11, 165
                        mov              r10, 26;                             jmp   n171_statement_begin_α
n164_statement_end_β:   mov              r11, 165;                            jmp   n171_statement_begin_α
#=======================================================================================================================
# trimws  <stmt 27, line 67: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n165_statement_begin_α: mov              r11, 166
                        mov              r10, 27;                             jmp   n166_var_α
n165_statement_begin_β: mov              r11, 166;                            jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n166_var_α:             sub              rsp, 16
                        mov              r11, 167
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n167_call_α
n166_var_β:             mov              r11, 167
                        add              rsp, 16;                             jmp   n165_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n167_call_α:            sub              rsp, 16
                        mov              r11, 168
                        lea              rcx, [rip + .Lsig640z]
                        lea              rax, [rip + rtrim_α];                jmp   rax
.Lsig640z:              .quad            1
                        .quad            .Lx640_2
                        .quad            .Lx640_2
                        .quad            16
.Lx640_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx640_29
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
.Lx640_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx640_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n165_statement_begin_β
.Lx640_240:                                                                   jmp   n168_call_α
n167_call_β:            mov              r11, 168;                            jmp   n165_statement_begin_β
.Lx640_0:               .quad            .Lx640_0_s
.Lx640_0_s:             .string          "rtrim"
#-----------------------------------------------------------------------------------------------------------------------
n168_call_α:            sub              rsp, 16
                        mov              r11, 169
                        lea              rcx, [rip + .Lsig642z]
                        lea              rax, [rip + ltrim_α];                jmp   rax
.Lsig642z:              .quad            1
                        .quad            .Lx642_2
                        .quad            .Lx642_2
                        .quad            16
.Lx642_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx642_29
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
.Lx642_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx642_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n165_statement_begin_β
.Lx642_240:                                                                   jmp   n169_assign_α
n168_call_β:            mov              r11, 169;                            jmp   n165_statement_begin_β
.Lx642_0:               .quad            .Lx642_0_s
.Lx642_0_s:             .string          "ltrim"
#-----------------------------------------------------------------------------------------------------------------------
n169_assign_α:          mov              r11, 170
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 176], rax            # trimws
                        mov              qword ptr [r9 + 184], rdx;           jmp   n170_statement_end_α
n169_assign_β:          mov              r11, 170;                            jmp   n165_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n170_statement_end_α:   mov              r11, 171
                        mov              r10, 27
                        add              rsp, 48;                             jmp   RETURN
n170_statement_end_β:   mov              r11, 171
                        add              rsp, 48;                             jmp   RETURN
#=======================================================================================================================
# trimws_end  <stmt 28, line 69: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n171_statement_begin_α: mov              r11, 172
                        mov              r10, 28;                             jmp   n172_statement_end_α
n171_statement_begin_β: mov              r11, 172;                            jmp   n173_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n172_statement_end_α:   mov              r11, 173
                        mov              r10, 28;                             jmp   n173_statement_begin_α
n172_statement_end_β:   mov              r11, 173;                            jmp   n173_statement_begin_α
#=======================================================================================================================
#         <stmt 29, line 70: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n173_statement_begin_α: mov              r11, 174
                        mov              r10, 29;                             jmp   n174_define_α
n173_statement_begin_β: mov              r11, 174;                            jmp   n182_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n174_define_α:          mov              r11, 175
                        mov              rdi, qword ptr [rip + .Lx653_0]
                        mov              rsi, qword ptr [rip + .Lx653_1]
                        mov              edx, 2
                        mov              ecx, 2
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
                        mov              rdi, qword ptr [rip + .Lx653_0]
                        lea              rsi, [rip + repeat_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n175_statement_end_α
n174_define_β:          mov              r11, 175;                            jmp   n173_statement_begin_β
.Lx653_0:               .quad            .Lx653_0_s
.Lx653_0_s:             .string          "repeat"
.Lx653_1:               .quad            .Lx653_1_s
.Lx653_1_s:             .string          "s,n"
                                                                              jmp   .Lx654_245
#-----------------------------------------------------------------------------------------------------------------------
repeat_α:               sub              rsp, 80
                        mov              rax, qword ptr [r9 + 192]            # repeat
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 192], 0
                        mov              qword ptr [r9 + 200], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        cmp              rdx, 0;                              jbe   .Lx654_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx654_41
.Lx654_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx654_41:              cmp              rdx, 1;                              jbe   .Lx654_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 32]             # n
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx654_42
.Lx654_11:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx654_42:              lea              rcx, [rip + repeat_γ]
                        lea              rax, [rip + repeat_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n176_statement_begin_α]; jmp   rax
repeat_γ:               mov              rdi, qword ptr [r9 + 192]            # repeat
                        mov              rsi, qword ptr [r9 + 200]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 192], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 200], rax
                        cmp              rdx, 0;                              jbe   .Lx654_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx654_110
.Lx654_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx654_110:             cmp              rdx, 1;                              jbe   .Lx654_81
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 32], rax             # n
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx654_111
.Lx654_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 40], rax
.Lx654_111:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 80
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
repeat_ω:               mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 192], rax            # repeat
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 200], rax
                        cmp              rdx, 0;                              jbe   .Lx654_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx654_180
.Lx654_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx654_180:             cmp              rdx, 1;                              jbe   .Lx654_151
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 32], rax             # n
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx654_181
.Lx654_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 40], rax
.Lx654_181:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 80
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx654_245:
#-----------------------------------------------------------------------------------------------------------------------
n175_statement_end_α:   mov              r11, 176
                        mov              r10, 29;                             jmp   n182_statement_begin_α
n175_statement_end_β:   mov              r11, 176;                            jmp   n182_statement_begin_α
#=======================================================================================================================
# repeat  <stmt 30, line 71: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n176_statement_begin_α: mov              r11, 177
                        mov              r10, 30;                             jmp   n177_var_α
n176_statement_begin_β: mov              r11, 177;                            jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n177_var_α:             sub              rsp, 16
                        mov              r11, 178
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n178_var_α
n177_var_β:             mov              r11, 178
                        add              rsp, 16;                             jmp   n176_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n178_var_α:             sub              rsp, 16
                        mov              r11, 179
                        mov              rax, qword ptr [r9 + 32]             # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n179_call_α
n178_var_β:             mov              r11, 179
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n176_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n179_call_α:            sub              rsp, 16
                        mov              r11, 180
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
.Lrkfnzd662:            .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd662]
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
                        cmp              al, 104;                             jne   .Lx661_240
                        add              rsp, 16;                             jmp   n178_var_β
.Lx661_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n180_assign_α
n179_call_β:            mov              r11, 180
                        add              rsp, 16;                             jmp   n178_var_β
#-----------------------------------------------------------------------------------------------------------------------
n180_assign_α:          mov              r11, 181
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 192], rax            # repeat
                        mov              qword ptr [r9 + 200], rdx;           jmp   n181_statement_end_α
n180_assign_β:          mov              r11, 181;                            jmp   n176_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n181_statement_end_α:   mov              r11, 182
                        mov              r10, 30
                        add              rsp, 48;                             jmp   RETURN
n181_statement_end_β:   mov              r11, 182
                        add              rsp, 48;                             jmp   RETURN
#=======================================================================================================================
# repeat_end  <stmt 31, line 73: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n182_statement_begin_α: mov              r11, 183
                        mov              r10, 31;                             jmp   n183_statement_end_α
n182_statement_begin_β: mov              r11, 183;                            jmp   n184_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n183_statement_end_α:   mov              r11, 184
                        mov              r10, 31;                             jmp   n184_statement_begin_α
n183_statement_end_β:   mov              r11, 184;                            jmp   n184_statement_begin_α
#=======================================================================================================================
#         <stmt 32, line 74: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n184_statement_begin_α: mov              r11, 185
                        mov              r10, 32;                             jmp   n185_define_α
n184_statement_begin_β: mov              r11, 185;                            jmp   n198_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n185_define_α:          mov              r11, 186
                        mov              rdi, qword ptr [rip + .Lx673_0]
                        mov              rsi, qword ptr [rip + .Lx673_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + n187_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx673_0]
                        lea              rsi, [rip + contains_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n186_statement_end_α
n185_define_β:          mov              r11, 186;                            jmp   n184_statement_begin_β
.Lx673_0:               .quad            .Lx673_0_s
.Lx673_0_s:             .string          "contains"
.Lx673_1:               .quad            .Lx673_1_s
.Lx673_1_s:             .string          "s,t"
                                                                              jmp   .Lx674_245
#-----------------------------------------------------------------------------------------------------------------------
contains_α:             sub              rsp, 80
                        mov              rax, qword ptr [r9 + 208]            # contains
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 208], 0
                        mov              qword ptr [r9 + 216], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        cmp              rdx, 0;                              jbe   .Lx674_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx674_41
.Lx674_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx674_41:              cmp              rdx, 1;                              jbe   .Lx674_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 224]            # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx674_42
.Lx674_11:              mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx674_42:              lea              rcx, [rip + contains_γ]
                        lea              rax, [rip + contains_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n187_statement_begin_α]; jmp   rax
contains_γ:             mov              rdi, qword ptr [r9 + 208]            # contains
                        mov              rsi, qword ptr [r9 + 216]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 208], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 216], rax
                        cmp              rdx, 0;                              jbe   .Lx674_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx674_110
.Lx674_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx674_110:             cmp              rdx, 1;                              jbe   .Lx674_81
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 224], rax            # t
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx674_111
.Lx674_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 232], rax
.Lx674_111:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 80
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
contains_ω:             mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 208], rax            # contains
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 216], rax
                        cmp              rdx, 0;                              jbe   .Lx674_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx674_180
.Lx674_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx674_180:             cmp              rdx, 1;                              jbe   .Lx674_151
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 224], rax            # t
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx674_181
.Lx674_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 232], rax
.Lx674_181:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 80
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx674_245:
#-----------------------------------------------------------------------------------------------------------------------
n186_statement_end_α:   mov              r11, 187
                        mov              r10, 32;                             jmp   n198_statement_begin_α
n186_statement_end_β:   mov              r11, 187;                            jmp   n198_statement_begin_α
#=======================================================================================================================
# contains  <stmt 33, line 75: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n187_statement_begin_α: mov              r11, 188
                        mov              r10, 33;                             jmp   n188_var_α
n187_statement_begin_β: mov              r11, 188;                            jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n188_var_α:             sub              rsp, 16
                        mov              r11, 189
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n189_var_α
n188_var_β:             mov              r11, 189
                        add              rsp, 16;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n189_var_α:             sub              rsp, 16
                        mov              r11, 190
                        mov              rax, qword ptr [r9 + 224]            # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n190_coerce_string_α
n189_var_β:             mov              r11, 190
                        add              rsp, 16
                        add              rsp, 16;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n190_coerce_string_α:   sub              rsp, 16
                        mov              r11, 191
                        lea              rdi, [rsp + 16]                      # var
                        lea              rsi, [rsp + 0]                       # result
                        mov              rdx, 4522053
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_str_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n191_var_α
n190_coerce_string_β:   mov              r11, 191
                        add              rsp, 16;                             jmp   n189_var_β
#-----------------------------------------------------------------------------------------------------------------------
n191_var_α:             sub              rsp, 16
                        mov              r11, 192
                        mov              rax, qword ptr [r9 + 224]            # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n192_assign_α
n191_var_β:             mov              r11, 192
                        add              rsp, 16;                             jmp   n190_coerce_string_β
#-----------------------------------------------------------------------------------------------------------------------
n192_assign_α:          mov              r11, 193
                        mov              rsi, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx684_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n193_match_begin_α
n192_assign_β:          mov              r11, 193;                            jmp   n191_var_β
.Lx684_0:               .quad            .Lx684_0_s
.Lx684_0_s:             .string          "PATV$0"
#-----------------------------------------------------------------------------------------------------------------------
n193_match_begin_α:     mov              r11, 194
                        mov              rdi, qword ptr [rsp + 48]            # var
                        mov              rsi, qword ptr [rsp + 56]
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
.Lx686_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx686_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n194_match_break_α
n193_match_begin_β:     mov              r11, 194
.Lx686_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx686_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx686_1
                                                                              jmp   .Lx686_0
.Lx686_1:
n193_match_begin_af:    mov              r11, 194
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
                        pop              rbp;                                 jmp   n192_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n194_match_break_α:     sub              rsp, 16
                        mov              r11, 195
                        mov              edi, r14d
                        mov              rsi, qword ptr [rsp + 104]           # coerce_string
                        mov              edx, dword ptr [rsp + 100]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_sg_scan_member@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              eax, r15d;                           jl    .Lx688_240
                        add              rsp, 16;                             jmp   n193_match_begin_β
.Lx688_240:             mov              dword ptr [rsp + 0], r14d
                        mov              r14d, eax;                           jmp   n195_match_defer_α
n194_match_break_β:     mov              r11, 195
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16;                             jmp   n193_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n195_match_defer_α:     mov              r11, 196
                        lea              rdi, [rip + .S2]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx689_14
                        mov              rax, qword ptr [rdx + 0]
.Lx689_14:              test             rax, rax;                            jz    .Lx689_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx689_5]
                        push             rcx
                        lea              rcx, [rip + .Lx689_4]
                        push             rcx;                                 jmp   rax
.Lx689_4:                                                                     jmp   n196_match_end_α
.Lx689_5:                                                                     jmp   n194_match_break_β
.Lx689_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S2]
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
                        test             eax, eax;                            js    n194_match_break_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx689_6]
                        push             rcx
                        push             rax;                                 jmp   n196_match_end_α
.Lx689_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n194_match_break_β
n195_match_defer_β:     mov              r11, 196
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx689_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx689_12
                                                                              jmp   rax
.Lx689_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n196_match_end_α:       mov              r11, 197
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
                        test             rax, rax;                            je    .Lx691_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n193_match_begin_af
.Lx691_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n197_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n197_statement_end_α:   mov              r11, 198
                        mov              r10, 33
                        add              rsp, 64;                             jmp   RETURN
n197_statement_end_β:   mov              r11, 198
                        add              rsp, 80;                             jmp   FRETURN
#=======================================================================================================================
# contains_end  <stmt 34, line 77: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n198_statement_begin_α: mov              r11, 199
                        mov              r10, 34;                             jmp   n199_statement_end_α
n198_statement_begin_β: mov              r11, 199;                            jmp   n200_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n199_statement_end_α:   mov              r11, 200
                        mov              r10, 34;                             jmp   n200_statement_begin_α
n199_statement_end_β:   mov              r11, 200;                            jmp   n200_statement_begin_α
#=======================================================================================================================
#         <stmt 35, line 78: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n200_statement_begin_α: mov              r11, 201
                        mov              r10, 35;                             jmp   n201_define_α
n200_statement_begin_β: mov              r11, 201;                            jmp   n212_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n201_define_α:          mov              r11, 202
                        mov              rdi, qword ptr [rip + .Lx701_0]
                        mov              rsi, qword ptr [rip + .Lx701_1]
                        mov              edx, 2
                        mov              ecx, 2
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
                        mov              rdi, qword ptr [rip + .Lx701_0]
                        lea              rsi, [rip + startswith_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n202_statement_end_α
n201_define_β:          mov              r11, 202;                            jmp   n200_statement_begin_β
.Lx701_0:               .quad            .Lx701_0_s
.Lx701_0_s:             .string          "startswith"
.Lx701_1:               .quad            .Lx701_1_s
.Lx701_1_s:             .string          "s,t"
                                                                              jmp   .Lx702_245
#-----------------------------------------------------------------------------------------------------------------------
startswith_α:           sub              rsp, 80
                        mov              rax, qword ptr [r9 + 240]            # startswith
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 248]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 240], 0
                        mov              qword ptr [r9 + 248], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        cmp              rdx, 0;                              jbe   .Lx702_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx702_41
.Lx702_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx702_41:              cmp              rdx, 1;                              jbe   .Lx702_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 224]            # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx702_42
.Lx702_11:              mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx702_42:              lea              rcx, [rip + startswith_γ]
                        lea              rax, [rip + startswith_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n203_statement_begin_α]; jmp   rax
startswith_γ:           mov              rdi, qword ptr [r9 + 240]            # startswith
                        mov              rsi, qword ptr [r9 + 248]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 240], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 248], rax
                        cmp              rdx, 0;                              jbe   .Lx702_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx702_110
.Lx702_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx702_110:             cmp              rdx, 1;                              jbe   .Lx702_81
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 224], rax            # t
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx702_111
.Lx702_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 232], rax
.Lx702_111:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 80
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
startswith_ω:           mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 240], rax            # startswith
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 248], rax
                        cmp              rdx, 0;                              jbe   .Lx702_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx702_180
.Lx702_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx702_180:             cmp              rdx, 1;                              jbe   .Lx702_151
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 224], rax            # t
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx702_181
.Lx702_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 232], rax
.Lx702_181:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 80
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx702_245:
#-----------------------------------------------------------------------------------------------------------------------
n202_statement_end_α:   mov              r11, 203
                        mov              r10, 35;                             jmp   n212_statement_begin_α
n202_statement_end_β:   mov              r11, 203;                            jmp   n212_statement_begin_α
#=======================================================================================================================
# startswith  <stmt 36, line 79: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n203_statement_begin_α: mov              r11, 204
                        mov              r10, 36;                             jmp   n204_var_α
n203_statement_begin_β: mov              r11, 204;                            jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n204_var_α:             sub              rsp, 16
                        mov              r11, 205
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n205_var_α
n204_var_β:             mov              r11, 205
                        add              rsp, 16;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n205_var_α:             sub              rsp, 16
                        mov              r11, 206
                        mov              rax, qword ptr [r9 + 224]            # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n206_assign_α
n205_var_β:             mov              r11, 206
                        add              rsp, 16
                        add              rsp, 16;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n206_assign_α:          mov              r11, 207
                        mov              rsi, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx709_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n207_match_begin_α
n206_assign_β:          mov              r11, 207;                            jmp   n205_var_β
.Lx709_0:               .quad            .Lx709_0_s
.Lx709_0_s:             .string          "PATV$1"
#-----------------------------------------------------------------------------------------------------------------------
n207_match_begin_α:     mov              r11, 208
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
.Lx711_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx711_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n208_match_pos_α
n207_match_begin_β:     mov              r11, 208
.Lx711_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx711_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx711_1
                                                                              jmp   .Lx711_0
.Lx711_1:
n207_match_begin_af:    mov              r11, 208
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
                        pop              rbp;                                 jmp   n206_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n208_match_pos_α:       mov              r11, 209
                        mov              rax, 0
                        cmp              r14d, eax;                           jne   n207_match_begin_β
                                                                              jmp   n209_match_defer_α
n208_match_pos_β:       mov              r11, 209;                            jmp   n207_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n209_match_defer_α:     mov              r11, 210
                        lea              rdi, [rip + .S3]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx713_14
                        mov              rax, qword ptr [rdx + 0]
.Lx713_14:              test             rax, rax;                            jz    .Lx713_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx713_5]
                        push             rcx
                        lea              rcx, [rip + .Lx713_4]
                        push             rcx;                                 jmp   rax
.Lx713_4:                                                                     jmp   n210_match_end_α
.Lx713_5:                                                                     jmp   n207_match_begin_β
.Lx713_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S3]
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
                        test             eax, eax;                            js    n207_match_begin_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx713_6]
                        push             rcx
                        push             rax;                                 jmp   n210_match_end_α
.Lx713_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n207_match_begin_β
n209_match_defer_β:     mov              r11, 210
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx713_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx713_12
                                                                              jmp   rax
.Lx713_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n210_match_end_α:       mov              r11, 211
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
                        test             rax, rax;                            je    .Lx715_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n207_match_begin_af
.Lx715_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n211_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n211_statement_end_α:   mov              r11, 212
                        mov              r10, 36
                        add              rsp, 32;                             jmp   RETURN
n211_statement_end_β:   mov              r11, 212
                        add              rsp, 32;                             jmp   FRETURN
#=======================================================================================================================
# startswith_end  <stmt 37, line 81: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n212_statement_begin_α: mov              r11, 213
                        mov              r10, 37;                             jmp   n213_statement_end_α
n212_statement_begin_β: mov              r11, 213;                            jmp   n214_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n213_statement_end_α:   mov              r11, 214
                        mov              r10, 37;                             jmp   n214_statement_begin_α
n213_statement_end_β:   mov              r11, 214;                            jmp   n214_statement_begin_α
#=======================================================================================================================
#         <stmt 38, line 82: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n214_statement_begin_α: mov              r11, 215
                        mov              r10, 38;                             jmp   n215_define_α
n214_statement_begin_β: mov              r11, 215;                            jmp   n226_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n215_define_α:          mov              r11, 216
                        mov              rdi, qword ptr [rip + .Lx725_0]
                        mov              rsi, qword ptr [rip + .Lx725_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + n217_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx725_0]
                        lea              rsi, [rip + endswith_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n216_statement_end_α
n215_define_β:          mov              r11, 216;                            jmp   n214_statement_begin_β
.Lx725_0:               .quad            .Lx725_0_s
.Lx725_0_s:             .string          "endswith"
.Lx725_1:               .quad            .Lx725_1_s
.Lx725_1_s:             .string          "s,t"
                                                                              jmp   .Lx726_245
#-----------------------------------------------------------------------------------------------------------------------
endswith_α:             sub              rsp, 80
                        mov              rax, qword ptr [r9 + 256]            # endswith
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 264]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 256], 0
                        mov              qword ptr [r9 + 264], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        cmp              rdx, 0;                              jbe   .Lx726_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx726_41
.Lx726_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx726_41:              cmp              rdx, 1;                              jbe   .Lx726_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 224]            # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx726_42
.Lx726_11:              mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx726_42:              lea              rcx, [rip + endswith_γ]
                        lea              rax, [rip + endswith_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n217_statement_begin_α]; jmp   rax
endswith_γ:             mov              rdi, qword ptr [r9 + 256]            # endswith
                        mov              rsi, qword ptr [r9 + 264]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 256], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 264], rax
                        cmp              rdx, 0;                              jbe   .Lx726_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx726_110
.Lx726_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx726_110:             cmp              rdx, 1;                              jbe   .Lx726_81
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 224], rax            # t
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx726_111
.Lx726_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 232], rax
.Lx726_111:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 80
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
endswith_ω:             mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 256], rax            # endswith
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 264], rax
                        cmp              rdx, 0;                              jbe   .Lx726_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx726_180
.Lx726_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx726_180:             cmp              rdx, 1;                              jbe   .Lx726_151
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 224], rax            # t
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx726_181
.Lx726_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 232], rax
.Lx726_181:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 80
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx726_245:
#-----------------------------------------------------------------------------------------------------------------------
n216_statement_end_α:   mov              r11, 217
                        mov              r10, 38;                             jmp   n226_statement_begin_α
n216_statement_end_β:   mov              r11, 217;                            jmp   n226_statement_begin_α
#=======================================================================================================================
# endswith  <stmt 39, line 83: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n217_statement_begin_α: mov              r11, 218
                        mov              r10, 39;                             jmp   n218_var_α
n217_statement_begin_β: mov              r11, 218;                            jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n218_var_α:             sub              rsp, 16
                        mov              r11, 219
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n219_var_α
n218_var_β:             mov              r11, 219
                        add              rsp, 16;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n219_var_α:             sub              rsp, 16
                        mov              r11, 220
                        mov              rax, qword ptr [r9 + 224]            # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n220_assign_α
n219_var_β:             mov              r11, 220
                        add              rsp, 16
                        add              rsp, 16;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n220_assign_α:          mov              r11, 221
                        mov              rsi, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx733_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n221_match_begin_α
n220_assign_β:          mov              r11, 221;                            jmp   n219_var_β
.Lx733_0:               .quad            .Lx733_0_s
.Lx733_0_s:             .string          "PATV$2"
#-----------------------------------------------------------------------------------------------------------------------
n221_match_begin_α:     mov              r11, 222
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
.Lx735_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx735_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n222_match_defer_α
n221_match_begin_β:     mov              r11, 222
.Lx735_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx735_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx735_1
                                                                              jmp   .Lx735_0
.Lx735_1:
n221_match_begin_af:    mov              r11, 222
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
                        pop              rbp;                                 jmp   n220_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n222_match_defer_α:     mov              r11, 223
                        lea              rdi, [rip + .S4]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx736_14
                        mov              rax, qword ptr [rdx + 0]
.Lx736_14:              test             rax, rax;                            jz    .Lx736_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx736_5]
                        push             rcx
                        lea              rcx, [rip + .Lx736_4]
                        push             rcx;                                 jmp   rax
.Lx736_4:                                                                     jmp   n223_match_rpos_α
.Lx736_5:                                                                     jmp   n221_match_begin_β
.Lx736_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S4]
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
                        test             eax, eax;                            js    n221_match_begin_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx736_6]
                        push             rcx
                        push             rax;                                 jmp   n223_match_rpos_α
.Lx736_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n221_match_begin_β
n222_match_defer_β:     mov              r11, 223
                        cmp              qword ptr [rsp + 0], 0;              jne   .Lx736_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx736_12
                                                                              jmp   rax
.Lx736_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n223_match_rpos_α:      mov              r11, 224
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx;                           jne   n222_match_defer_β
                                                                              jmp   n224_match_end_α
n223_match_rpos_β:      mov              r11, 224;                            jmp   n222_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n224_match_end_α:       mov              r11, 225
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
                        test             rax, rax;                            je    .Lx739_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n221_match_begin_af
.Lx739_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n225_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n225_statement_end_α:   mov              r11, 226
                        mov              r10, 39
                        add              rsp, 32;                             jmp   RETURN
n225_statement_end_β:   mov              r11, 226
                        add              rsp, 32;                             jmp   FRETURN
#=======================================================================================================================
# endswith_end  <stmt 40, line 85: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n226_statement_begin_α: mov              r11, 227
                        mov              r10, 40;                             jmp   n227_statement_end_α
n226_statement_begin_β: mov              r11, 227;                            jmp   n228_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n227_statement_end_α:   mov              r11, 228
                        mov              r10, 40;                             jmp   n228_statement_begin_α
n227_statement_end_β:   mov              r11, 228;                            jmp   n228_statement_begin_α
#=======================================================================================================================
#         <stmt 41, line 86: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n228_statement_begin_α: mov              r11, 229
                        mov              r10, 41;                             jmp   n229_define_α
n228_statement_begin_β: mov              r11, 229;                            jmp   n256_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n229_define_α:          mov              r11, 230
                        mov              rdi, qword ptr [rip + .Lx749_0]
                        mov              rsi, qword ptr [rip + .Lx749_1]
                        mov              edx, 3
                        mov              ecx, 2
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
                        mov              rdi, qword ptr [rip + .Lx749_0]
                        lea              rsi, [rip + index_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n230_statement_end_α
n229_define_β:          mov              r11, 230;                            jmp   n228_statement_begin_β
.Lx749_0:               .quad            .Lx749_0_s
.Lx749_0_s:             .string          "index"
.Lx749_1:               .quad            .Lx749_1_s
.Lx749_1_s:             .string          "s,t,ix"
                                                                              jmp   .Lx750_245
#-----------------------------------------------------------------------------------------------------------------------
index_α:                sub              rsp, 96
                        mov              rax, qword ptr [r9 + 288]            # ix
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 288], 0
                        mov              qword ptr [r9 + 296], 0
                        mov              rax, qword ptr [r9 + 272]            # index
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 280]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 272], 0
                        mov              qword ptr [r9 + 280], 0
                        mov              qword ptr [rsp + 48], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        cmp              rdx, 0;                              jbe   .Lx750_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx750_41
.Lx750_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx750_41:              cmp              rdx, 1;                              jbe   .Lx750_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 224]            # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx750_42
.Lx750_11:              mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx750_42:              lea              rcx, [rip + index_γ]
                        lea              rax, [rip + index_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n231_statement_begin_α]; jmp   rax
index_γ:                mov              rdi, qword ptr [r9 + 272]            # index
                        mov              rsi, qword ptr [r9 + 280]
                        mov              rcx, qword ptr [rsp + 48]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 272], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 280], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 288], rax            # ix
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 296], rax
                        cmp              rdx, 0;                              jbe   .Lx750_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx750_110
.Lx750_80:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 24], rax
.Lx750_110:             cmp              rdx, 1;                              jbe   .Lx750_81
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 224], rax            # t
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx750_111
.Lx750_81:              mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 232], rax
.Lx750_111:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 96
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
index_ω:                mov              rcx, qword ptr [rsp + 48]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 272], rax            # index
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 280], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 288], rax            # ix
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 296], rax
                        cmp              rdx, 0;                              jbe   .Lx750_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx750_180
.Lx750_150:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 24], rax
.Lx750_180:             cmp              rdx, 1;                              jbe   .Lx750_151
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 224], rax            # t
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx750_181
.Lx750_151:             mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 232], rax
.Lx750_181:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 96
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx750_245:
#-----------------------------------------------------------------------------------------------------------------------
n230_statement_end_α:   mov              r11, 231
                        mov              r10, 41;                             jmp   n256_statement_begin_α
n230_statement_end_β:   mov              r11, 231;                            jmp   n256_statement_begin_α
#=======================================================================================================================
# index  <stmt 42, line 87: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n231_statement_begin_α: mov              r11, 232
                        mov              r10, 42;                             jmp   n232_lit_integer_α
n231_statement_begin_β: mov              r11, 232;                            jmp   n235_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n232_lit_integer_α:     sub              rsp, 16
                        mov              r11, 233
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx755_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n233_assign_α
n232_lit_integer_β:     mov              r11, 233
                        add              rsp, 16;                             jmp   n231_statement_begin_β
.Lx755_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n233_assign_α:          mov              r11, 234
                        mov              rax, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 272], rax            # index
                        mov              qword ptr [r9 + 280], rdx;           jmp   n234_statement_end_α
n233_assign_β:          mov              r11, 234;                            jmp   n231_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n234_statement_end_α:   mov              r11, 235
                        mov              r10, 42
                        add              rsp, 16;                             jmp   n235_statement_begin_α
n234_statement_end_β:   mov              r11, 235
                        add              rsp, 16;                             jmp   n235_statement_begin_α
#=======================================================================================================================
#         <stmt 43, line 88: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n235_statement_begin_α: mov              r11, 236
                        mov              r10, 43;                             jmp   n236_var_α
n235_statement_begin_β: mov              r11, 236;                            jmp   n239_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n236_var_α:             sub              rsp, 16
                        mov              r11, 237
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n237_assign_α
n236_var_β:             mov              r11, 237
                        add              rsp, 16;                             jmp   n235_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n237_assign_α:          mov              r11, 238
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 288], rax            # ix
                        mov              qword ptr [r9 + 296], rdx;           jmp   n238_statement_end_α
n237_assign_β:          mov              r11, 238;                            jmp   n235_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n238_statement_end_α:   mov              r11, 239
                        mov              r10, 43
                        add              rsp, 16;                             jmp   n239_statement_begin_α
n238_statement_end_β:   mov              r11, 239
                        add              rsp, 16;                             jmp   n239_statement_begin_α
#=======================================================================================================================
#         <stmt 44, line 89: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n239_statement_begin_α: mov              r11, 240
                        mov              r10, 44;                             jmp   n240_var_α
n239_statement_begin_β: mov              r11, 240;                            jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n240_var_α:             sub              rsp, 16
                        mov              r11, 241
                        mov              rax, qword ptr [r9 + 288]            # ix
                        mov              rdx, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n241_var_α
n240_var_β:             mov              r11, 241
                        add              rsp, 16;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n241_var_α:             sub              rsp, 16
                        mov              r11, 242
                        mov              rax, qword ptr [r9 + 224]            # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n242_coerce_string_α
n241_var_β:             mov              r11, 242
                        add              rsp, 16
                        add              rsp, 16;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n242_coerce_string_α:   sub              rsp, 16
                        mov              r11, 243
                        lea              rdi, [rsp + 16]                      # var
                        lea              rsi, [rsp + 0]                       # result
                        mov              rdx, 4522053
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_str_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n243_match_begin_α
n242_coerce_string_β:   mov              r11, 243
                        add              rsp, 16;                             jmp   n241_var_β
#-----------------------------------------------------------------------------------------------------------------------
n243_match_begin_α:     mov              r11, 244
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
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
.Lx772_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx772_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n244_match_assign_save_α
n243_match_begin_β:     mov              r11, 244
.Lx772_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx772_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx772_1
                                                                              jmp   .Lx772_0
.Lx772_1:
n243_match_begin_af:    mov              r11, 244
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
                        pop              rbp;                                 jmp   n242_coerce_string_β
#-----------------------------------------------------------------------------------------------------------------------
n244_match_assign_save_α:
                        sub              rsp, 16
                        mov              r11, 245
                        mov              dword ptr [rsp + 0], r14d;           jmp   n245_match_break_α
n244_match_assign_save_β:
                        mov              r11, 245
                        add              rsp, 16;                             jmp   n243_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n245_match_break_α:     sub              rsp, 16
                        mov              r11, 246
                        mov              edi, r14d
                        mov              rsi, qword ptr [rsp + 104]           # coerce_string
                        mov              edx, dword ptr [rsp + 100]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_sg_scan_member@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              eax, r15d;                           jl    .Lx776_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n243_match_begin_β
.Lx776_240:             mov              dword ptr [rsp + 0], r14d
                        mov              r14d, eax;                           jmp   n246_match_assign_cond_α
n245_match_break_β:     mov              r11, 246
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n243_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n246_match_assign_cond_α:
                        mov              r11, 247
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S5]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n247_match_end_α
n246_match_assign_cond_β:
                        mov              r11, 247
                        sub              r12, 24;                             jmp   n245_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n247_match_end_α:       mov              r11, 248
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
                        test             rax, rax;                            je    .Lx780_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n243_match_begin_af
.Lx780_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n248_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n248_statement_end_α:   mov              r11, 249
                        mov              r10, 44
                        add              rsp, 48;                             jmp   n249_statement_begin_α
n248_statement_end_β:   mov              r11, 249
                        add              rsp, 80;                             jmp   RETURN
#=======================================================================================================================
#         <stmt 45, line 90: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n249_statement_begin_α: mov              r11, 250
                        mov              r10, 45;                             jmp   n250_var_α
n249_statement_begin_β: mov              r11, 250;                            jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n250_var_α:             sub              rsp, 16
                        mov              r11, 251
                        mov              rax, qword ptr [r9 + 288]            # ix
                        mov              rdx, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n251_call_α
n250_var_β:             mov              r11, 251
                        add              rsp, 16;                             jmp   n249_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n251_call_α:            sub              rsp, 16
                        mov              r11, 252
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd787:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd787]
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
                        cmp              al, 104;                             jne   .Lx786_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n249_statement_begin_β
.Lx786_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n252_lit_integer_α
n251_call_β:            mov              r11, 252
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n249_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n252_lit_integer_α:     sub              rsp, 16
                        mov              r11, 253
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx788_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n253_binop_α
n252_lit_integer_β:     mov              r11, 253
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n249_statement_begin_β
.Lx788_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n253_binop_α:           sub              rsp, 16
                        mov              r11, 254
                        mov              ecx, dword ptr [rsp + 32]            # call
                        mov              rax, qword ptr [rsp + 40]
                        cmp              cl, 3;                               jne   .Lx789_2
                        add              rax, 1
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx789_7
.Lx789_2:               mov              eax, ecx
                        mov              edx, ecx
                        and              edx, 1;                              jz    .Lx789_0
                        mov              rsi, qword ptr [rsp + 40]            # call
                        mov              rdi, 1
                        cmp              al, 5;                               je    .Lx789_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx789_4
.Lx789_3:               movq             xmm0, rsi
.Lx789_4:               cvtsi2sd         xmm1, rdi
                        addsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx789_7:                                                                     jmp   n254_assign_α
.Lx789_0:               mov              rdi, qword ptr [rsp + 32]            # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              al, 104;                             jne   .Lx789_240
                        add              rsp, 16;                             jmp   n252_lit_integer_β
.Lx789_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n254_assign_α
n253_binop_β:           mov              r11, 254
                        add              rsp, 16;                             jmp   n252_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n254_assign_α:          mov              r11, 255
                        mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 272], rax            # index
                        mov              qword ptr [r9 + 280], rdx;           jmp   n255_statement_end_α
n254_assign_β:          mov              r11, 255;                            jmp   n249_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n255_statement_end_α:   mov              r11, 256
                        mov              r10, 45
                        add              rsp, 64;                             jmp   RETURN
n255_statement_end_β:   mov              r11, 256
                        add              rsp, 64;                             jmp   RETURN
#=======================================================================================================================
# index_end  <stmt 46, line 92: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n256_statement_begin_α: mov              r11, 257
                        mov              r10, 46;                             jmp   n257_statement_end_α
n256_statement_begin_β: mov              r11, 257;                            jmp   n258_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n257_statement_end_α:   mov              r11, 258
                        mov              r10, 46;                             jmp   n258_statement_begin_α
n257_statement_end_β:   mov              r11, 258;                            jmp   n258_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n258_statement_begin_α: mov              r11, 259
                        mov              r10, 0;                              jmp   n259_statement_end_α
n258_statement_begin_β: mov              r11, 259;                            jmp   n260_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n259_statement_end_α:   mov              r11, 260
                        mov              r10, 47;                             jmp   n260_statement_begin_α
n259_statement_end_β:   mov              r11, 260;                            jmp   n260_statement_begin_α
#=======================================================================================================================
#         <stmt 48, line 93: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n260_statement_begin_α: mov              r11, 261
                        mov              r10, 48;                             jmp   n261_lit_integer_α
n260_statement_begin_β: mov              r11, 261;                            jmp   n264_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n261_lit_integer_α:     sub              rsp, 16
                        mov              r11, 262
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx803_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n262_keyword_assign_snobol4_α
n261_lit_integer_β:     mov              r11, 262
                        add              rsp, 16;                             jmp   n260_statement_begin_β
.Lx803_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n262_keyword_assign_snobol4_α:
                        sub              rsp, 16
                        mov              r11, 263
                        mov              rdi, qword ptr [rip + .Lx804_0]
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
                        cmp              al, 104;                             jne   .Lx804_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n260_statement_begin_β
.Lx804_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n263_statement_end_α
n262_keyword_assign_snobol4_β:
                        mov              r11, 263
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n260_statement_begin_β
.Lx804_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n263_statement_end_α:   mov              r11, 264
                        mov              r10, 48;                             jmp   n264_statement_begin_α
n263_statement_end_β:   mov              r11, 264;                            jmp   n264_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n264_statement_begin_α: mov              r11, 265
                        mov              r10, 0;                              jmp   n265_statement_end_α
n264_statement_begin_β: mov              r11, 265
                        add              rsp, 32;                             jmp   n266_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n265_statement_end_α:   mov              r11, 266
                        mov              r10, 49
                        add              rsp, 32;                             jmp   n266_statement_begin_α
n265_statement_end_β:   mov              r11, 266
                        add              rsp, 32;                             jmp   n266_statement_begin_α
#=======================================================================================================================
#         <stmt 50, line 95: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n266_statement_begin_α: mov              r11, 267
                        mov              r10, 50;                             jmp   n267_lit_string_α
n266_statement_begin_β: mov              r11, 267;                            jmp   n273_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n267_lit_string_α:      sub              rsp, 16
                        mov              r11, 268
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx813_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n268_lit_integer_α
n267_lit_string_β:      mov              r11, 268
                        add              rsp, 16;                             jmp   n266_statement_begin_β
.Lx813_0:               .quad            .Lx813_0_s
.Lx813_0_s:             .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n268_lit_integer_α:     sub              rsp, 16
                        mov              r11, 269
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx814_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n269_lit_string_α
n268_lit_integer_β:     mov              r11, 269
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n266_statement_begin_β
.Lx814_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n269_lit_string_α:      sub              rsp, 16
                        mov              r11, 270
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx815_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n270_call_α
n269_lit_string_β:      mov              r11, 270
                        add              rsp, 16;                             jmp   n268_lit_integer_β
.Lx815_0:               .quad            .Lx815_0_s
.Lx815_0_s:             .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n270_call_α:            sub              rsp, 16
                        mov              r11, 271
                        lea              rcx, [rip + .Lsig817z]
                        lea              rax, [rip + pad_left_α];             jmp   rax
.Lsig817z:              .quad            3
                        .quad            .Lx817_2
                        .quad            .Lx817_2
                        .quad            48
                        .quad            32
                        .quad            16
.Lx817_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx817_29
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
.Lx817_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx817_240
                        add              rsp, 16;                             jmp   n269_lit_string_β
.Lx817_240:                                                                   jmp   n271_assign_α
n270_call_β:            mov              r11, 271;                            jmp   n269_lit_string_β
.Lx817_0:               .quad            .Lx817_0_s
.Lx817_0_s:             .string          "pad_left"
#-----------------------------------------------------------------------------------------------------------------------
n271_assign_α:          mov              r11, 272
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx818_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n272_statement_end_α
n271_assign_β:          mov              r11, 272;                            jmp   n266_statement_begin_β
.Lx818_0:               .quad            .Lx818_0_s
.Lx818_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n272_statement_end_α:   mov              r11, 273
                        mov              r10, 50
                        add              rsp, 64;                             jmp   n273_statement_begin_α
n272_statement_end_β:   mov              r11, 273
                        add              rsp, 64;                             jmp   n273_statement_begin_α
#=======================================================================================================================
#         <stmt 51, line 96: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n273_statement_begin_α: mov              r11, 274
                        mov              r10, 51;                             jmp   n274_lit_string_α
n273_statement_begin_β: mov              r11, 274;                            jmp   n280_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n274_lit_string_α:      sub              rsp, 16
                        mov              r11, 275
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx823_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n275_lit_integer_α
n274_lit_string_β:      mov              r11, 275
                        add              rsp, 16;                             jmp   n273_statement_begin_β
.Lx823_0:               .quad            .Lx823_0_s
.Lx823_0_s:             .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n275_lit_integer_α:     sub              rsp, 16
                        mov              r11, 276
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx824_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n276_lit_string_α
n275_lit_integer_β:     mov              r11, 276
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n273_statement_begin_β
.Lx824_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n276_lit_string_α:      sub              rsp, 16
                        mov              r11, 277
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx825_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n277_call_α
n276_lit_string_β:      mov              r11, 277
                        add              rsp, 16;                             jmp   n275_lit_integer_β
.Lx825_0:               .quad            .Lx825_0_s
.Lx825_0_s:             .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n277_call_α:            sub              rsp, 16
                        mov              r11, 278
                        lea              rcx, [rip + .Lsig827z]
                        lea              rax, [rip + pad_right_α];            jmp   rax
.Lsig827z:              .quad            3
                        .quad            .Lx827_2
                        .quad            .Lx827_2
                        .quad            48
                        .quad            32
                        .quad            16
.Lx827_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx827_29
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
.Lx827_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx827_240
                        add              rsp, 16;                             jmp   n276_lit_string_β
.Lx827_240:                                                                   jmp   n278_assign_α
n277_call_β:            mov              r11, 278;                            jmp   n276_lit_string_β
.Lx827_0:               .quad            .Lx827_0_s
.Lx827_0_s:             .string          "pad_right"
#-----------------------------------------------------------------------------------------------------------------------
n278_assign_α:          mov              r11, 279
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx828_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n279_statement_end_α
n278_assign_β:          mov              r11, 279;                            jmp   n273_statement_begin_β
.Lx828_0:               .quad            .Lx828_0_s
.Lx828_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n279_statement_end_α:   mov              r11, 280
                        mov              r10, 51
                        add              rsp, 64;                             jmp   n280_statement_begin_α
n279_statement_end_β:   mov              r11, 280
                        add              rsp, 64;                             jmp   n280_statement_begin_α
#=======================================================================================================================
#         <stmt 52, line 97: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n280_statement_begin_α: mov              r11, 281
                        mov              r10, 52;                             jmp   n281_lit_string_α
n280_statement_begin_β: mov              r11, 281;                            jmp   n285_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n281_lit_string_α:      sub              rsp, 16
                        mov              r11, 282
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx833_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n282_call_α
n281_lit_string_β:      mov              r11, 282
                        add              rsp, 16;                             jmp   n280_statement_begin_β
.Lx833_0:               .quad            .Lx833_0_s
.Lx833_0_s:             .string          "   hello"
#-----------------------------------------------------------------------------------------------------------------------
n282_call_α:            sub              rsp, 16
                        mov              r11, 283
                        lea              rcx, [rip + .Lsig835z]
                        lea              rax, [rip + ltrim_α];                jmp   rax
.Lsig835z:              .quad            1
                        .quad            .Lx835_2
                        .quad            .Lx835_2
                        .quad            16
.Lx835_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx835_29
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
.Lx835_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx835_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n280_statement_begin_β
.Lx835_240:                                                                   jmp   n283_assign_α
n282_call_β:            mov              r11, 283;                            jmp   n280_statement_begin_β
.Lx835_0:               .quad            .Lx835_0_s
.Lx835_0_s:             .string          "ltrim"
#-----------------------------------------------------------------------------------------------------------------------
n283_assign_α:          mov              r11, 284
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx836_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n284_statement_end_α
n283_assign_β:          mov              r11, 284;                            jmp   n280_statement_begin_β
.Lx836_0:               .quad            .Lx836_0_s
.Lx836_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n284_statement_end_α:   mov              r11, 285
                        mov              r10, 52
                        add              rsp, 32;                             jmp   n285_statement_begin_α
n284_statement_end_β:   mov              r11, 285
                        add              rsp, 32;                             jmp   n285_statement_begin_α
#=======================================================================================================================
#         <stmt 53, line 98: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n285_statement_begin_α: mov              r11, 286
                        mov              r10, 53;                             jmp   n286_lit_string_α
n285_statement_begin_β: mov              r11, 286;                            jmp   n290_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n286_lit_string_α:      sub              rsp, 16
                        mov              r11, 287
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx841_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n287_call_α
n286_lit_string_β:      mov              r11, 287
                        add              rsp, 16;                             jmp   n285_statement_begin_β
.Lx841_0:               .quad            .Lx841_0_s
.Lx841_0_s:             .string          "hello   "
#-----------------------------------------------------------------------------------------------------------------------
n287_call_α:            sub              rsp, 16
                        mov              r11, 288
                        lea              rcx, [rip + .Lsig843z]
                        lea              rax, [rip + rtrim_α];                jmp   rax
.Lsig843z:              .quad            1
                        .quad            .Lx843_2
                        .quad            .Lx843_2
                        .quad            16
.Lx843_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx843_29
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
.Lx843_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx843_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n285_statement_begin_β
.Lx843_240:                                                                   jmp   n288_assign_α
n287_call_β:            mov              r11, 288;                            jmp   n285_statement_begin_β
.Lx843_0:               .quad            .Lx843_0_s
.Lx843_0_s:             .string          "rtrim"
#-----------------------------------------------------------------------------------------------------------------------
n288_assign_α:          mov              r11, 289
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx844_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n289_statement_end_α
n288_assign_β:          mov              r11, 289;                            jmp   n285_statement_begin_β
.Lx844_0:               .quad            .Lx844_0_s
.Lx844_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n289_statement_end_α:   mov              r11, 290
                        mov              r10, 53
                        add              rsp, 32;                             jmp   n290_statement_begin_α
n289_statement_end_β:   mov              r11, 290
                        add              rsp, 32;                             jmp   n290_statement_begin_α
#=======================================================================================================================
#         <stmt 54, line 99: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n290_statement_begin_α: mov              r11, 291
                        mov              r10, 54;                             jmp   n291_lit_string_α
n290_statement_begin_β: mov              r11, 291;                            jmp   n295_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n291_lit_string_α:      sub              rsp, 16
                        mov              r11, 292
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 9
                        mov              rax, qword ptr [rip + .Lx849_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n292_call_α
n291_lit_string_β:      mov              r11, 292
                        add              rsp, 16;                             jmp   n290_statement_begin_β
.Lx849_0:               .quad            .Lx849_0_s
.Lx849_0_s:             .string          "  hello  "
#-----------------------------------------------------------------------------------------------------------------------
n292_call_α:            sub              rsp, 16
                        mov              r11, 293
                        lea              rcx, [rip + .Lsig851z]
                        lea              rax, [rip + trimws_α];               jmp   rax
.Lsig851z:              .quad            1
                        .quad            .Lx851_2
                        .quad            .Lx851_2
                        .quad            16
.Lx851_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx851_29
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
.Lx851_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx851_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n290_statement_begin_β
.Lx851_240:                                                                   jmp   n293_assign_α
n292_call_β:            mov              r11, 293;                            jmp   n290_statement_begin_β
.Lx851_0:               .quad            .Lx851_0_s
.Lx851_0_s:             .string          "trimws"
#-----------------------------------------------------------------------------------------------------------------------
n293_assign_α:          mov              r11, 294
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx852_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n294_statement_end_α
n293_assign_β:          mov              r11, 294;                            jmp   n290_statement_begin_β
.Lx852_0:               .quad            .Lx852_0_s
.Lx852_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n294_statement_end_α:   mov              r11, 295
                        mov              r10, 54
                        add              rsp, 32;                             jmp   n295_statement_begin_α
n294_statement_end_β:   mov              r11, 295
                        add              rsp, 32;                             jmp   n295_statement_begin_α
#=======================================================================================================================
#         <stmt 55, line 100: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n295_statement_begin_α: mov              r11, 296
                        mov              r10, 55;                             jmp   n296_lit_string_α
n295_statement_begin_β: mov              r11, 296;                            jmp   n301_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n296_lit_string_α:      sub              rsp, 16
                        mov              r11, 297
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx857_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n297_lit_integer_α
n296_lit_string_β:      mov              r11, 297
                        add              rsp, 16;                             jmp   n295_statement_begin_β
.Lx857_0:               .quad            .Lx857_0_s
.Lx857_0_s:             .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n297_lit_integer_α:     sub              rsp, 16
                        mov              r11, 298
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx858_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n298_call_α
n297_lit_integer_β:     mov              r11, 298
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n295_statement_begin_β
.Lx858_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n298_call_α:            sub              rsp, 16
                        mov              r11, 299
                        lea              rcx, [rip + .Lsig860z]
                        lea              rax, [rip + repeat_α];               jmp   rax
.Lsig860z:              .quad            2
                        .quad            .Lx860_2
                        .quad            .Lx860_2
                        .quad            32
                        .quad            16
.Lx860_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx860_29
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
.Lx860_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx860_240
                        add              rsp, 16;                             jmp   n297_lit_integer_β
.Lx860_240:                                                                   jmp   n299_assign_α
n298_call_β:            mov              r11, 299;                            jmp   n297_lit_integer_β
.Lx860_0:               .quad            .Lx860_0_s
.Lx860_0_s:             .string          "repeat"
#-----------------------------------------------------------------------------------------------------------------------
n299_assign_α:          mov              r11, 300
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx861_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n300_statement_end_α
n299_assign_β:          mov              r11, 300;                            jmp   n295_statement_begin_β
.Lx861_0:               .quad            .Lx861_0_s
.Lx861_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n300_statement_end_α:   mov              r11, 301
                        mov              r10, 55;                             jmp   n301_statement_begin_α
n300_statement_end_β:   mov              r11, 301;                            jmp   n301_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n301_statement_begin_α: mov              r11, 302
                        mov              r10, 0;                              jmp   n302_statement_end_α
n301_statement_begin_β: mov              r11, 302
                        add              rsp, 48;                             jmp   n303_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n302_statement_end_α:   mov              r11, 303
                        mov              r10, 56
                        add              rsp, 48;                             jmp   n303_statement_begin_α
n302_statement_end_β:   mov              r11, 303
                        add              rsp, 48;                             jmp   n303_statement_begin_α
#=======================================================================================================================
#         <stmt 57, line 102: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n303_statement_begin_α: mov              r11, 304
                        mov              r10, 57;                             jmp   n304_lit_string_α
n303_statement_begin_β: mov              r11, 304;                            jmp   n312_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n304_lit_string_α:      sub              rsp, 16
                        mov              r11, 305
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx870_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n305_lit_string_α
n304_lit_string_β:      mov              r11, 305
                        add              rsp, 16;                             jmp   n303_statement_begin_β
.Lx870_0:               .quad            .Lx870_0_s
.Lx870_0_s:             .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n305_lit_string_α:      sub              rsp, 16
                        mov              r11, 306
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx871_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n306_call_α
n305_lit_string_β:      mov              r11, 306
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n303_statement_begin_β
.Lx871_0:               .quad            .Lx871_0_s
.Lx871_0_s:             .string          "oba"
#-----------------------------------------------------------------------------------------------------------------------
n306_call_α:            sub              rsp, 16
                        mov              r11, 307
                        lea              rcx, [rip + .Lsig873z]
                        lea              rax, [rip + contains_α];             jmp   rax
.Lsig873z:              .quad            2
                        .quad            .Lx873_2
                        .quad            .Lx873_2
                        .quad            32
                        .quad            16
.Lx873_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx873_29
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
.Lx873_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx873_240
                        add              rsp, 16;                             jmp   n305_lit_string_β
.Lx873_240:                                                                   jmp   n307_statement_end_α
n306_call_β:            mov              r11, 307;                            jmp   n305_lit_string_β
.Lx873_0:               .quad            .Lx873_0_s
.Lx873_0_s:             .string          "contains"
#-----------------------------------------------------------------------------------------------------------------------
n307_statement_end_α:   mov              r11, 308
                        mov              r10, 57
                        add              rsp, 48;                             jmp   n308_statement_begin_α
n307_statement_end_β:   mov              r11, 308
                        add              rsp, 48;                             jmp   n312_statement_begin_α
#=======================================================================================================================
#         <stmt 58, line 103: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n308_statement_begin_α: mov              r11, 309
                        mov              r10, 58;                             jmp   n309_lit_string_α
n308_statement_begin_β: mov              r11, 309;                            jmp   n316_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n309_lit_string_α:      sub              rsp, 16
                        mov              r11, 310
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx878_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n310_assign_α
n309_lit_string_β:      mov              r11, 310
                        add              rsp, 16;                             jmp   n308_statement_begin_β
.Lx878_0:               .quad            .Lx878_0_s
.Lx878_0_s:             .string          "contains ok"
#-----------------------------------------------------------------------------------------------------------------------
n310_assign_α:          mov              r11, 311
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx879_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n311_statement_end_α
n310_assign_β:          mov              r11, 311;                            jmp   n308_statement_begin_β
.Lx879_0:               .quad            .Lx879_0_s
.Lx879_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n311_statement_end_α:   mov              r11, 312
                        mov              r10, 58
                        add              rsp, 16;                             jmp   n316_statement_begin_α
n311_statement_end_β:   mov              r11, 312
                        add              rsp, 16;                             jmp   n316_statement_begin_α
#=======================================================================================================================
# bad_c1  <stmt 59, line 104: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n312_statement_begin_α: mov              r11, 313
                        mov              r10, 59;                             jmp   n313_lit_string_α
n312_statement_begin_β: mov              r11, 313;                            jmp   n316_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n313_lit_string_α:      sub              rsp, 16
                        mov              r11, 314
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 14
                        mov              rax, qword ptr [rip + .Lx884_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n314_assign_α
n313_lit_string_β:      mov              r11, 314
                        add              rsp, 16;                             jmp   n312_statement_begin_β
.Lx884_0:               .quad            .Lx884_0_s
.Lx884_0_s:             .string          "FAIL: contains"
#-----------------------------------------------------------------------------------------------------------------------
n314_assign_α:          mov              r11, 315
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx885_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n315_statement_end_α
n314_assign_β:          mov              r11, 315;                            jmp   n312_statement_begin_β
.Lx885_0:               .quad            .Lx885_0_s
.Lx885_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n315_statement_end_α:   mov              r11, 316
                        mov              r10, 59
                        add              rsp, 16;                             jmp   n316_statement_begin_α
n315_statement_end_β:   mov              r11, 316
                        add              rsp, 16;                             jmp   n316_statement_begin_α
#=======================================================================================================================
# c1  <stmt 60, line 106: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n316_statement_begin_α: mov              r11, 317
                        mov              r10, 60;                             jmp   n317_statement_end_α
n316_statement_begin_β: mov              r11, 317;                            jmp   n318_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n317_statement_end_α:   mov              r11, 318
                        mov              r10, 60;                             jmp   n318_statement_begin_α
n317_statement_end_β:   mov              r11, 318;                            jmp   n318_statement_begin_α
#=======================================================================================================================
#         <stmt 61, line 106: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n318_statement_begin_α: mov              r11, 319
                        mov              r10, 61;                             jmp   n319_lit_string_α
n318_statement_begin_β: mov              r11, 319;                            jmp   n327_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n319_lit_string_α:      sub              rsp, 16
                        mov              r11, 320
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx894_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n320_lit_string_α
n319_lit_string_β:      mov              r11, 320
                        add              rsp, 16;                             jmp   n318_statement_begin_β
.Lx894_0:               .quad            .Lx894_0_s
.Lx894_0_s:             .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n320_lit_string_α:      sub              rsp, 16
                        mov              r11, 321
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx895_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n321_call_α
n320_lit_string_β:      mov              r11, 321
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n318_statement_begin_β
.Lx895_0:               .quad            .Lx895_0_s
.Lx895_0_s:             .string          "foo"
#-----------------------------------------------------------------------------------------------------------------------
n321_call_α:            sub              rsp, 16
                        mov              r11, 322
                        lea              rcx, [rip + .Lsig897z]
                        lea              rax, [rip + startswith_α];           jmp   rax
.Lsig897z:              .quad            2
                        .quad            .Lx897_2
                        .quad            .Lx897_2
                        .quad            32
                        .quad            16
.Lx897_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx897_29
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
.Lx897_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx897_240
                        add              rsp, 16;                             jmp   n320_lit_string_β
.Lx897_240:                                                                   jmp   n322_statement_end_α
n321_call_β:            mov              r11, 322;                            jmp   n320_lit_string_β
.Lx897_0:               .quad            .Lx897_0_s
.Lx897_0_s:             .string          "startswith"
#-----------------------------------------------------------------------------------------------------------------------
n322_statement_end_α:   mov              r11, 323
                        mov              r10, 61
                        add              rsp, 48;                             jmp   n323_statement_begin_α
n322_statement_end_β:   mov              r11, 323
                        add              rsp, 48;                             jmp   n327_statement_begin_α
#=======================================================================================================================
#         <stmt 62, line 107: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n323_statement_begin_α: mov              r11, 324
                        mov              r10, 62;                             jmp   n324_lit_string_α
n323_statement_begin_β: mov              r11, 324;                            jmp   n331_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n324_lit_string_α:      sub              rsp, 16
                        mov              r11, 325
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 13
                        mov              rax, qword ptr [rip + .Lx902_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n325_assign_α
n324_lit_string_β:      mov              r11, 325
                        add              rsp, 16;                             jmp   n323_statement_begin_β
.Lx902_0:               .quad            .Lx902_0_s
.Lx902_0_s:             .string          "startswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n325_assign_α:          mov              r11, 326
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx903_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n326_statement_end_α
n325_assign_β:          mov              r11, 326;                            jmp   n323_statement_begin_β
.Lx903_0:               .quad            .Lx903_0_s
.Lx903_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n326_statement_end_α:   mov              r11, 327
                        mov              r10, 62
                        add              rsp, 16;                             jmp   n331_statement_begin_α
n326_statement_end_β:   mov              r11, 327
                        add              rsp, 16;                             jmp   n331_statement_begin_α
#=======================================================================================================================
# bad_sw1  <stmt 63, line 108: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n327_statement_begin_α: mov              r11, 328
                        mov              r10, 63;                             jmp   n328_lit_string_α
n327_statement_begin_β: mov              r11, 328;                            jmp   n331_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n328_lit_string_α:      sub              rsp, 16
                        mov              r11, 329
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 16
                        mov              rax, qword ptr [rip + .Lx908_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n329_assign_α
n328_lit_string_β:      mov              r11, 329
                        add              rsp, 16;                             jmp   n327_statement_begin_β
.Lx908_0:               .quad            .Lx908_0_s
.Lx908_0_s:             .string          "FAIL: startswith"
#-----------------------------------------------------------------------------------------------------------------------
n329_assign_α:          mov              r11, 330
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx909_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n330_statement_end_α
n329_assign_β:          mov              r11, 330;                            jmp   n327_statement_begin_β
.Lx909_0:               .quad            .Lx909_0_s
.Lx909_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n330_statement_end_α:   mov              r11, 331
                        mov              r10, 63
                        add              rsp, 16;                             jmp   n331_statement_begin_α
n330_statement_end_β:   mov              r11, 331
                        add              rsp, 16;                             jmp   n331_statement_begin_α
#=======================================================================================================================
# sw1  <stmt 64, line 110: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n331_statement_begin_α: mov              r11, 332
                        mov              r10, 64;                             jmp   n332_statement_end_α
n331_statement_begin_β: mov              r11, 332;                            jmp   n333_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n332_statement_end_α:   mov              r11, 333
                        mov              r10, 64;                             jmp   n333_statement_begin_α
n332_statement_end_β:   mov              r11, 333;                            jmp   n333_statement_begin_α
#=======================================================================================================================
#         <stmt 65, line 110: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n333_statement_begin_α: mov              r11, 334
                        mov              r10, 65;                             jmp   n334_lit_string_α
n333_statement_begin_β: mov              r11, 334;                            jmp   n342_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n334_lit_string_α:      sub              rsp, 16
                        mov              r11, 335
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx918_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n335_lit_string_α
n334_lit_string_β:      mov              r11, 335
                        add              rsp, 16;                             jmp   n333_statement_begin_β
.Lx918_0:               .quad            .Lx918_0_s
.Lx918_0_s:             .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n335_lit_string_α:      sub              rsp, 16
                        mov              r11, 336
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx919_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n336_call_α
n335_lit_string_β:      mov              r11, 336
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n333_statement_begin_β
.Lx919_0:               .quad            .Lx919_0_s
.Lx919_0_s:             .string          "bar"
#-----------------------------------------------------------------------------------------------------------------------
n336_call_α:            sub              rsp, 16
                        mov              r11, 337
                        lea              rcx, [rip + .Lsig921z]
                        lea              rax, [rip + endswith_α];             jmp   rax
.Lsig921z:              .quad            2
                        .quad            .Lx921_2
                        .quad            .Lx921_2
                        .quad            32
                        .quad            16
.Lx921_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx921_29
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
.Lx921_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx921_240
                        add              rsp, 16;                             jmp   n335_lit_string_β
.Lx921_240:                                                                   jmp   n337_statement_end_α
n336_call_β:            mov              r11, 337;                            jmp   n335_lit_string_β
.Lx921_0:               .quad            .Lx921_0_s
.Lx921_0_s:             .string          "endswith"
#-----------------------------------------------------------------------------------------------------------------------
n337_statement_end_α:   mov              r11, 338
                        mov              r10, 65
                        add              rsp, 48;                             jmp   n338_statement_begin_α
n337_statement_end_β:   mov              r11, 338
                        add              rsp, 48;                             jmp   n342_statement_begin_α
#=======================================================================================================================
#         <stmt 66, line 111: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n338_statement_begin_α: mov              r11, 339
                        mov              r10, 66;                             jmp   n339_lit_string_α
n338_statement_begin_β: mov              r11, 339;                            jmp   n346_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n339_lit_string_α:      sub              rsp, 16
                        mov              r11, 340
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx926_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n340_assign_α
n339_lit_string_β:      mov              r11, 340
                        add              rsp, 16;                             jmp   n338_statement_begin_β
.Lx926_0:               .quad            .Lx926_0_s
.Lx926_0_s:             .string          "endswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n340_assign_α:          mov              r11, 341
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx927_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n341_statement_end_α
n340_assign_β:          mov              r11, 341;                            jmp   n338_statement_begin_β
.Lx927_0:               .quad            .Lx927_0_s
.Lx927_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n341_statement_end_α:   mov              r11, 342
                        mov              r10, 66
                        add              rsp, 16;                             jmp   n346_statement_begin_α
n341_statement_end_β:   mov              r11, 342
                        add              rsp, 16;                             jmp   n346_statement_begin_α
#=======================================================================================================================
# bad_ew1  <stmt 67, line 112: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n342_statement_begin_α: mov              r11, 343
                        mov              r10, 67;                             jmp   n343_lit_string_α
n342_statement_begin_β: mov              r11, 343;                            jmp   n346_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n343_lit_string_α:      sub              rsp, 16
                        mov              r11, 344
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 14
                        mov              rax, qword ptr [rip + .Lx932_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n344_assign_α
n343_lit_string_β:      mov              r11, 344
                        add              rsp, 16;                             jmp   n342_statement_begin_β
.Lx932_0:               .quad            .Lx932_0_s
.Lx932_0_s:             .string          "FAIL: endswith"
#-----------------------------------------------------------------------------------------------------------------------
n344_assign_α:          mov              r11, 345
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx933_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n345_statement_end_α
n344_assign_β:          mov              r11, 345;                            jmp   n342_statement_begin_β
.Lx933_0:               .quad            .Lx933_0_s
.Lx933_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n345_statement_end_α:   mov              r11, 346
                        mov              r10, 67
                        add              rsp, 16;                             jmp   n346_statement_begin_α
n345_statement_end_β:   mov              r11, 346
                        add              rsp, 16;                             jmp   n346_statement_begin_α
#=======================================================================================================================
# ew1  <stmt 68, line 114: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n346_statement_begin_α: mov              r11, 347
                        mov              r10, 68;                             jmp   n347_statement_end_α
n346_statement_begin_β: mov              r11, 347;                            jmp   n348_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n347_statement_end_α:   mov              r11, 348
                        mov              r10, 68;                             jmp   n348_statement_begin_α
n347_statement_end_β:   mov              r11, 348;                            jmp   n348_statement_begin_α
#=======================================================================================================================
#         <stmt 69, line 114: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n348_statement_begin_α: mov              r11, 349
                        mov              r10, 69;                             jmp   n349_lit_string_α
n348_statement_begin_β: mov              r11, 349;                            jmp   n353_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n349_lit_string_α:      sub              rsp, 16
                        mov              r11, 350
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx942_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n350_lit_string_α
n349_lit_string_β:      mov              r11, 350
                        add              rsp, 16;                             jmp   n348_statement_begin_β
.Lx942_0:               .quad            .Lx942_0_s
.Lx942_0_s:             .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n350_lit_string_α:      sub              rsp, 16
                        mov              r11, 351
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx943_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n351_call_α
n350_lit_string_β:      mov              r11, 351
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n348_statement_begin_β
.Lx943_0:               .quad            .Lx943_0_s
.Lx943_0_s:             .string          "bar"
#-----------------------------------------------------------------------------------------------------------------------
n351_call_α:            sub              rsp, 16
                        mov              r11, 352
                        lea              rcx, [rip + .Lsig945z]
                        lea              rax, [rip + startswith_α];           jmp   rax
.Lsig945z:              .quad            2
                        .quad            .Lx945_2
                        .quad            .Lx945_2
                        .quad            32
                        .quad            16
.Lx945_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx945_29
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
.Lx945_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx945_240
                        add              rsp, 16;                             jmp   n350_lit_string_β
.Lx945_240:                                                                   jmp   n352_statement_end_α
n351_call_β:            mov              r11, 352;                            jmp   n350_lit_string_β
.Lx945_0:               .quad            .Lx945_0_s
.Lx945_0_s:             .string          "startswith"
#-----------------------------------------------------------------------------------------------------------------------
n352_statement_end_α:   mov              r11, 353
                        mov              r10, 69
                        add              rsp, 48;                             jmp   n357_statement_begin_α
n352_statement_end_β:   mov              r11, 353
                        add              rsp, 48;                             jmp   n353_statement_begin_α
#=======================================================================================================================
#         <stmt 70, line 115: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n353_statement_begin_α: mov              r11, 354
                        mov              r10, 70;                             jmp   n354_lit_string_α
n353_statement_begin_β: mov              r11, 354;                            jmp   n361_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n354_lit_string_α:      sub              rsp, 16
                        mov              r11, 355
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 16
                        mov              rax, qword ptr [rip + .Lx950_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n355_assign_α
n354_lit_string_β:      mov              r11, 355
                        add              rsp, 16;                             jmp   n353_statement_begin_β
.Lx950_0:               .quad            .Lx950_0_s
.Lx950_0_s:             .string          "no startswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n355_assign_α:          mov              r11, 356
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx951_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n356_statement_end_α
n355_assign_β:          mov              r11, 356;                            jmp   n353_statement_begin_β
.Lx951_0:               .quad            .Lx951_0_s
.Lx951_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n356_statement_end_α:   mov              r11, 357
                        mov              r10, 70
                        add              rsp, 16;                             jmp   n361_statement_begin_α
n356_statement_end_β:   mov              r11, 357
                        add              rsp, 16;                             jmp   n361_statement_begin_α
#=======================================================================================================================
# bad_sw2  <stmt 71, line 116: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n357_statement_begin_α: mov              r11, 358
                        mov              r10, 71;                             jmp   n358_lit_string_α
n357_statement_begin_β: mov              r11, 358;                            jmp   n361_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n358_lit_string_α:      sub              rsp, 16
                        mov              r11, 359
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx956_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n359_assign_α
n358_lit_string_β:      mov              r11, 359
                        add              rsp, 16;                             jmp   n357_statement_begin_β
.Lx956_0:               .quad            .Lx956_0_s
.Lx956_0_s:             .string          "FAIL: startswith matched wrong"
#-----------------------------------------------------------------------------------------------------------------------
n359_assign_α:          mov              r11, 360
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx957_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n360_statement_end_α
n359_assign_β:          mov              r11, 360;                            jmp   n357_statement_begin_β
.Lx957_0:               .quad            .Lx957_0_s
.Lx957_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n360_statement_end_α:   mov              r11, 361
                        mov              r10, 71
                        add              rsp, 16;                             jmp   n361_statement_begin_α
n360_statement_end_β:   mov              r11, 361
                        add              rsp, 16;                             jmp   n361_statement_begin_α
#=======================================================================================================================
# sw2  <stmt 72, line 118: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n361_statement_begin_α: mov              r11, 362
                        mov              r10, 72;                             jmp   n362_statement_end_α
n361_statement_begin_β: mov              r11, 362;                            jmp   n363_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n362_statement_end_α:   mov              r11, 363
                        mov              r10, 72;                             jmp   n363_statement_begin_α
n362_statement_end_β:   mov              r11, 363;                            jmp   n363_statement_begin_α
#=======================================================================================================================
#         <stmt 73, line 118: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n363_statement_begin_α: mov              r11, 364
                        mov              r10, 73;                             jmp   n364_lit_string_α
n363_statement_begin_β: mov              r11, 364;                            jmp   n369_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n364_lit_string_α:      sub              rsp, 16
                        mov              r11, 365
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx966_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n365_lit_string_α
n364_lit_string_β:      mov              r11, 365
                        add              rsp, 16;                             jmp   n363_statement_begin_β
.Lx966_0:               .quad            .Lx966_0_s
.Lx966_0_s:             .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n365_lit_string_α:      sub              rsp, 16
                        mov              r11, 366
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx967_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n366_call_α
n365_lit_string_β:      mov              r11, 366
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n363_statement_begin_β
.Lx967_0:               .quad            .Lx967_0_s
.Lx967_0_s:             .string          "oba"
#-----------------------------------------------------------------------------------------------------------------------
n366_call_α:            sub              rsp, 16
                        mov              r11, 367
                        lea              rcx, [rip + .Lsig969z]
                        lea              rax, [rip + index_α];                jmp   rax
.Lsig969z:              .quad            2
                        .quad            .Lx969_2
                        .quad            .Lx969_2
                        .quad            32
                        .quad            16
.Lx969_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx969_29
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
.Lx969_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx969_240
                        add              rsp, 16;                             jmp   n365_lit_string_β
.Lx969_240:                                                                   jmp   n367_assign_α
n366_call_β:            mov              r11, 367;                            jmp   n365_lit_string_β
.Lx969_0:               .quad            .Lx969_0_s
.Lx969_0_s:             .string          "index"
#-----------------------------------------------------------------------------------------------------------------------
n367_assign_α:          mov              r11, 368
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx970_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n368_statement_end_α
n367_assign_β:          mov              r11, 368;                            jmp   n363_statement_begin_β
.Lx970_0:               .quad            .Lx970_0_s
.Lx970_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n368_statement_end_α:   mov              r11, 369
                        mov              r10, 73
                        add              rsp, 48;                             jmp   n369_statement_begin_α
n368_statement_end_β:   mov              r11, 369
                        add              rsp, 48;                             jmp   n369_statement_begin_α
#=======================================================================================================================
#         <stmt 74, line 119: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n369_statement_begin_α: mov              r11, 370
                        mov              r10, 74;                             jmp   n370_lit_string_α
n369_statement_begin_β: mov              r11, 370;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n370_lit_string_α:      sub              rsp, 16
                        mov              r11, 371
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx975_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n371_lit_string_α
n370_lit_string_β:      mov              r11, 371
                        add              rsp, 16;                             jmp   n369_statement_begin_β
.Lx975_0:               .quad            .Lx975_0_s
.Lx975_0_s:             .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n371_lit_string_α:      sub              rsp, 16
                        mov              r11, 372
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx976_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n372_call_α
n371_lit_string_β:      mov              r11, 372
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n369_statement_begin_β
.Lx976_0:               .quad            .Lx976_0_s
.Lx976_0_s:             .string          "xyz"
#-----------------------------------------------------------------------------------------------------------------------
n372_call_α:            sub              rsp, 16
                        mov              r11, 373
                        lea              rcx, [rip + .Lsig978z]
                        lea              rax, [rip + index_α];                jmp   rax
.Lsig978z:              .quad            2
                        .quad            .Lx978_2
                        .quad            .Lx978_2
                        .quad            32
                        .quad            16
.Lx978_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx978_29
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
.Lx978_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx978_240
                        add              rsp, 16;                             jmp   n371_lit_string_β
.Lx978_240:                                                                   jmp   n373_assign_α
n372_call_β:            mov              r11, 373;                            jmp   n371_lit_string_β
.Lx978_0:               .quad            .Lx978_0_s
.Lx978_0_s:             .string          "index"
#-----------------------------------------------------------------------------------------------------------------------
n373_assign_α:          mov              r11, 374
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx979_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n374_statement_end_α
n373_assign_β:          mov              r11, 374;                            jmp   n369_statement_begin_β
.Lx979_0:               .quad            .Lx979_0_s
.Lx979_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n374_statement_end_α:   mov              r11, 375
                        mov              r10, 74
                        add              rsp, 48;                             jmp   main_γ
n374_statement_end_β:   mov              r11, 375
                        add              rsp, 48;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 mov              r11, 376
                        pop              rcx
                        add              rsp, 8;                              jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                mov              r11, 377
                        add              rsp, 8
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
module_init:
                        sub              rsp, 8
                        .section         .rodata
.Lseala10:              .string          "pad_left"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            pad_left_α
                        lea              rdi, [rip + .Lseala10]
                        mov              rsi, qword ptr [rip + pad_left_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala11:              .string          "pad_right"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            pad_right_α
                        lea              rdi, [rip + .Lseala11]
                        mov              rsi, qword ptr [rip + pad_right_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala12:              .string          "ltrim"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            ltrim_α
                        lea              rdi, [rip + .Lseala12]
                        mov              rsi, qword ptr [rip + ltrim_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala13:              .string          "rtrim"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            rtrim_α
                        lea              rdi, [rip + .Lseala13]
                        mov              rsi, qword ptr [rip + rtrim_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala14:              .string          "trimws"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            trimws_α
                        lea              rdi, [rip + .Lseala14]
                        mov              rsi, qword ptr [rip + trimws_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala15:              .string          "repeat"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            repeat_α
                        lea              rdi, [rip + .Lseala15]
                        mov              rsi, qword ptr [rip + repeat_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala16:              .string          "contains"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            contains_α
                        lea              rdi, [rip + .Lseala16]
                        mov              rsi, qword ptr [rip + contains_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala17:              .string          "startswith"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            startswith_α
                        lea              rdi, [rip + .Lseala17]
                        mov              rsi, qword ptr [rip + startswith_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala18:              .string          "endswith"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            endswith_α
                        lea              rdi, [rip + .Lseala18]
                        mov              rsi, qword ptr [rip + endswith_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala19:              .string          "index"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            index_α
                        lea              rdi, [rip + .Lseala19]
                        mov              rsi, qword ptr [rip + index_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.S0:                    .string          "r"
.S1:                    .string          "ws"
.S2:                    .string          "PATV$0"
.S3:                    .string          "PATV$1"
.S4:                    .string          "PATV$2"
.S5:                    .string          "ix"
                        .text
                        .section         .note.GNU-stack,"",@progbits
