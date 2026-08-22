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
n0_statement_begin_α:                                                         jmp   n1_statement_end_α
n0_statement_begin_β:                                                         jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:                                                           jmp   n2_statement_begin_α
n1_statement_end_β:                                                           jmp   n2_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'endswith ok'          :(ew1)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                                         jmp   n3_define_α
n2_statement_begin_β:                                                         jmp   n34_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_define_α:            mov              rdi, qword ptr [rip + .Lx386_0]
                        mov              rsi, qword ptr [rip + .Lx386_1]
                        mov              edx, 3
                        mov              ecx, 3
                        mov              r8d, 0
                        lea              r9, [rip + n5_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx386_0]
                        lea              rsi, [rip + pad_left_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n4_statement_end_α
n3_define_β:                                                                  jmp   n2_statement_begin_β
.Lx386_0:               .quad            .Lx386_0_s
.Lx386_0_s:             .string          "pad_left"
.Lx386_1:               .quad            .Lx386_1_s
.Lx386_1_s:             .string          "s,n,c"
                                                                              jmp   .Lx387_245
#-----------------------------------------------------------------------------------------------------------------------
pad_left_α:             sub              rsp, 96
                        mov              rax, qword ptr [r9 + 0]              # pad_left
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        cmp              rdx, 0;                              jbe   .Lx387_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx387_41
.Lx387_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx387_41:              cmp              rdx, 1;                              jbe   .Lx387_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 32]             # n
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx387_42
.Lx387_11:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx387_42:              cmp              rdx, 2;                              jbe   .Lx387_12
                        mov              rdi, qword ptr [rcx + 40]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 48]             # c
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx387_43
.Lx387_12:              mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx387_43:              lea              r10, [rip + pad_left_γ]
                        lea              r11, [rip + pad_left_ω]
                        push             r11
                        push             r10
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
                        cmp              rdx, 0;                              jbe   .Lx387_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx387_110
.Lx387_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx387_110:             cmp              rdx, 1;                              jbe   .Lx387_81
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 32], rax             # n
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx387_111
.Lx387_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 40], rax
.Lx387_111:             cmp              rdx, 2;                              jbe   .Lx387_82
                        mov              r11, qword ptr [rcx + 40]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 48], rax             # c
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx387_112
.Lx387_82:              mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 56], rax
.Lx387_112:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx387_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx387_180
.Lx387_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx387_180:             cmp              rdx, 1;                              jbe   .Lx387_151
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 32], rax             # n
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx387_181
.Lx387_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 40], rax
.Lx387_181:             cmp              rdx, 2;                              jbe   .Lx387_152
                        mov              r11, qword ptr [rcx + 40]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 48], rax             # c
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx387_182
.Lx387_152:             mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 56], rax
.Lx387_182:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 96
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx387_245:
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_end_α:                                                           jmp   n34_statement_begin_α
n4_statement_end_β:                                                           jmp   n34_statement_begin_α
#=======================================================================================================================
# pad_left  <stmt 3, line 39: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_begin_α:                                                         jmp   n6_var_α
n5_statement_begin_β:                                                         jmp   n12_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n7_call_α
n6_var_β:               add              rsp, 16;                             jmp   n5_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n7_call_α:              sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd394:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd394]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx393_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n5_statement_begin_β
.Lx393_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n8_lit_string_α
n7_call_β:              add              rsp, 16
                        add              rsp, 16;                             jmp   n5_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx395_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n9_binop_α
n8_lit_string_β:        add              rsp, 16
                        add              rsp, 32;                             jmp   n5_statement_begin_β
.Lx395_0:               .quad            .Lx395_0_s
.Lx395_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n9_binop_α:             sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n10_assign_α
n9_binop_β:             add              rsp, 16;                             jmp   n8_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n10_assign_α:           mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # c
                        mov              qword ptr [r9 + 56], rdx;            jmp   n11_statement_end_α
n10_assign_β:                                                                 jmp   n5_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:    add              rsp, 64;                             jmp   n12_statement_begin_α
n11_statement_end_β:    add              rsp, 64;                             jmp   n12_statement_begin_α
#=======================================================================================================================
#         <stmt 4, line 40: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:                                                        jmp   n13_var_α
n12_statement_begin_β:                                                        jmp   n23_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n13_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n14_call_α
n13_var_β:              add              rsp, 16;                             jmp   n12_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n14_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd404:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd404]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx403_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n12_statement_begin_β
.Lx403_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n15_var_α
n14_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n12_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n15_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]             # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n16_coerce_numeric_α
n15_var_β:              add              rsp, 16
                        add              rsp, 32;                             jmp   n12_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n16_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # call
                        cmp              eax, 5;                              je    .Lx407_1
                        cmp              eax, 3;                              jne   .Lx407_0
                        mov              eax, dword ptr [rsp + 16]            # var
                        cmp              eax, 3;                              jne   .Lx407_0
.Lx407_1:               mov              rax, qword ptr [rsp + 32]            # call
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # call
                        mov              qword ptr [rsp + 8], rax;            jmp   n17_coerce_numeric_α
.Lx407_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 109
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n17_coerce_numeric_α
n16_coerce_numeric_β:   add              rsp, 16;                             jmp   n15_var_β
#-----------------------------------------------------------------------------------------------------------------------
n17_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              eax, 5;                              je    .Lx409_1
                        cmp              eax, 3;                              jne   .Lx409_0
                        mov              eax, dword ptr [rsp + 48]            # call
                        cmp              eax, 3;                              jne   .Lx409_0
.Lx409_1:               mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n18_cmp_test_α
.Lx409_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # call
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 110
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n18_cmp_test_α
n17_coerce_numeric_β:   add              rsp, 16;                             jmp   n16_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n18_cmp_test_α:         sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              edx, 3;                              jne   .Lx411_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jge   .Lx411_239
                        add              rsp, 16;                             jmp   n17_coerce_numeric_β
.Lx411_239:             mov              qword ptr [rsp + 0], 0               # result
                        mov              qword ptr [rsp + 8], 0;              jmp   n19_var_α
.Lx411_0:               lea              rdi, [rsp + 32]                      # coerce_numeric
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_cmp_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            jns   .Lx411_240
                        add              rsp, 16;                             jmp   n17_coerce_numeric_β
.Lx411_240:             mov              qword ptr [rsp + 0], 0               # result
                        mov              qword ptr [rsp + 8], 0;              jmp   n19_var_α
n18_cmp_test_β:         add              rsp, 16;                             jmp   n17_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n19_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n20_binop_α
n19_var_β:              add              rsp, 16;                             jmp   n18_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n20_binop_α:            sub              rsp, 16
                        mov              rax, qword ptr [rsp + 16]            # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n21_assign_α
n20_binop_β:            add              rsp, 16;                             jmp   n19_var_β
#-----------------------------------------------------------------------------------------------------------------------
n21_assign_α:           mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # pad_left
                        mov              qword ptr [r9 + 8], rdx;             jmp   n22_statement_end_α
n21_assign_β:                                                                 jmp   n12_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_end_α:    add              rsp, 128;                            jmp   RETURN
n22_statement_end_β:    add              rsp, 128;                            jmp   n23_statement_begin_α
#=======================================================================================================================
#         startswith('foobar', 'bar')     :S(bad_sw2)
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_begin_α:                                                        jmp   n24_var_α
n23_statement_begin_β:                                                        jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n24_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n25_var_α
n24_var_β:              add              rsp, 16;                             jmp   n23_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n25_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]             # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n26_var_α
n25_var_β:              add              rsp, 16
                        add              rsp, 16;                             jmp   n23_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n26_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n27_call_α
n26_var_β:              add              rsp, 16;                             jmp   n25_var_β
#-----------------------------------------------------------------------------------------------------------------------
n27_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd423:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd423]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx422_240
                        add              rsp, 16;                             jmp   n26_var_β
.Lx422_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n28_binop_α
n27_call_β:             add              rsp, 16;                             jmp   n26_var_β
#-----------------------------------------------------------------------------------------------------------------------
n28_binop_α:            sub              rsp, 16
                        mov              eax, dword ptr [rsp + 48]            # var
                        mov              ecx, dword ptr [rsp + 16]            # call
                        mov              edx, eax
                        and              edx, ecx
                        cmp              edx, 3;                              jne   .Lx424_2
                        mov              rax, qword ptr [rsp + 56]            # var
                        mov              rdx, qword ptr [rsp + 24]            # call
                        sub              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx424_7
.Lx424_2:               and              edx, 1;                              jz    .Lx424_0
                        mov              rsi, qword ptr [rsp + 56]            # var
                        mov              rdi, qword ptr [rsp + 24]            # call
                        cmp              eax, 5;                              je    .Lx424_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx424_4
.Lx424_3:               movq             xmm0, rsi
.Lx424_4:               cmp              ecx, 5;                              je    .Lx424_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx424_6
.Lx424_5:               movq             xmm1, rdi
.Lx424_6:               subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx424_7:                                                                     jmp   n29_call_α
.Lx424_0:               mov              rdi, qword ptr [rsp + 48]            # var
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_sub@PLT
                        cmp              eax, 104;                            jne   .Lx424_240
                        add              rsp, 32;                             jmp   n26_var_β
.Lx424_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n29_call_α
n28_binop_β:            add              rsp, 32;                             jmp   n26_var_β
#-----------------------------------------------------------------------------------------------------------------------
n29_call_α:             sub              rsp, 16
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
.Lrkfnzd426:            .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd426]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              eax, 104;                            jne   .Lx425_240
                        add              rsp, 16;                             jmp   n28_binop_β
.Lx425_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n30_var_α
n29_call_β:             add              rsp, 16;                             jmp   n28_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n30_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n31_binop_α
n30_var_β:              add              rsp, 32;                             jmp   n28_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n31_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n32_assign_α
n31_binop_β:            add              rsp, 16;                             jmp   n30_var_β
#-----------------------------------------------------------------------------------------------------------------------
n32_assign_α:           mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # pad_left
                        mov              qword ptr [r9 + 8], rdx;             jmp   n33_statement_end_α
n32_assign_β:                                                                 jmp   n23_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_end_α:    add              rsp, 128;                            jmp   RETURN
n33_statement_end_β:    add              rsp, 128;                            jmp   RETURN
#=======================================================================================================================
# pad_left_end  <stmt 6, line 43: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_begin_α:                                                        jmp   n35_statement_end_α
n34_statement_begin_β:                                                        jmp   n36_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_end_α:                                                          jmp   n36_statement_begin_α
n35_statement_end_β:                                                          jmp   n36_statement_begin_α
#=======================================================================================================================
#         <stmt 7, line 44: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_begin_α:                                                        jmp   n37_define_α
n36_statement_begin_β:                                                        jmp   n68_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n37_define_α:           mov              rdi, qword ptr [rip + .Lx439_0]
                        mov              rsi, qword ptr [rip + .Lx439_1]
                        mov              edx, 3
                        mov              ecx, 3
                        mov              r8d, 0
                        lea              r9, [rip + n39_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx439_0]
                        lea              rsi, [rip + pad_right_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n38_statement_end_α
n37_define_β:                                                                 jmp   n36_statement_begin_β
.Lx439_0:               .quad            .Lx439_0_s
.Lx439_0_s:             .string          "pad_right"
.Lx439_1:               .quad            .Lx439_1_s
.Lx439_1_s:             .string          "s,n,c"
                                                                              jmp   .Lx440_245
#-----------------------------------------------------------------------------------------------------------------------
pad_right_α:            sub              rsp, 96
                        mov              rax, qword ptr [r9 + 64]             # pad_right
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        cmp              rdx, 0;                              jbe   .Lx440_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx440_41
.Lx440_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx440_41:              cmp              rdx, 1;                              jbe   .Lx440_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 32]             # n
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx440_42
.Lx440_11:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx440_42:              cmp              rdx, 2;                              jbe   .Lx440_12
                        mov              rdi, qword ptr [rcx + 40]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 48]             # c
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx440_43
.Lx440_12:              mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx440_43:              lea              r10, [rip + pad_right_γ]
                        lea              r11, [rip + pad_right_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n39_statement_begin_α];  jmp   rax
pad_right_γ:            mov              rdi, qword ptr [r9 + 64]             # pad_right
                        mov              rsi, qword ptr [r9 + 72]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        cmp              rdx, 0;                              jbe   .Lx440_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx440_110
.Lx440_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx440_110:             cmp              rdx, 1;                              jbe   .Lx440_81
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 32], rax             # n
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx440_111
.Lx440_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 40], rax
.Lx440_111:             cmp              rdx, 2;                              jbe   .Lx440_82
                        mov              r11, qword ptr [rcx + 40]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 48], rax             # c
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx440_112
.Lx440_82:              mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 56], rax
.Lx440_112:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx440_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx440_180
.Lx440_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx440_180:             cmp              rdx, 1;                              jbe   .Lx440_151
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 32], rax             # n
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx440_181
.Lx440_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 40], rax
.Lx440_181:             cmp              rdx, 2;                              jbe   .Lx440_152
                        mov              r11, qword ptr [rcx + 40]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 48], rax             # c
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx440_182
.Lx440_152:             mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 56], rax
.Lx440_182:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 96
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx440_245:
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_end_α:                                                          jmp   n68_statement_begin_α
n38_statement_end_β:                                                          jmp   n68_statement_begin_α
#=======================================================================================================================
# pad_right  <stmt 8, line 45: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_begin_α:                                                        jmp   n40_var_α
n39_statement_begin_β:                                                        jmp   n46_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n40_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n41_call_α
n40_var_β:              add              rsp, 16;                             jmp   n39_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n41_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd447:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd447]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx446_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n39_statement_begin_β
.Lx446_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n42_lit_string_α
n41_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n39_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx448_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n43_binop_α
n42_lit_string_β:       add              rsp, 16
                        add              rsp, 32;                             jmp   n39_statement_begin_β
.Lx448_0:               .quad            .Lx448_0_s
.Lx448_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n43_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n44_assign_α
n43_binop_β:            add              rsp, 16;                             jmp   n42_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n44_assign_α:           mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # c
                        mov              qword ptr [r9 + 56], rdx;            jmp   n45_statement_end_α
n44_assign_β:                                                                 jmp   n39_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_end_α:    add              rsp, 64;                             jmp   n46_statement_begin_α
n45_statement_end_β:    add              rsp, 64;                             jmp   n46_statement_begin_α
#=======================================================================================================================
#         OUTPUT = index('foobar', 'xyz') ;* 0
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_begin_α:                                                        jmp   n47_var_α
n46_statement_begin_β:                                                        jmp   n57_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n47_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n48_call_α
n47_var_β:              add              rsp, 16;                             jmp   n46_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n48_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd457:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd457]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx456_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n46_statement_begin_β
.Lx456_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n49_var_α
n48_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n46_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n49_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]             # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n50_coerce_numeric_α
n49_var_β:              add              rsp, 16
                        add              rsp, 32;                             jmp   n46_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n50_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # call
                        cmp              eax, 5;                              je    .Lx460_1
                        cmp              eax, 3;                              jne   .Lx460_0
                        mov              eax, dword ptr [rsp + 16]            # var
                        cmp              eax, 3;                              jne   .Lx460_0
.Lx460_1:               mov              rax, qword ptr [rsp + 32]            # call
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # call
                        mov              qword ptr [rsp + 8], rax;            jmp   n51_coerce_numeric_α
.Lx460_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 109
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n51_coerce_numeric_α
n50_coerce_numeric_β:   add              rsp, 16;                             jmp   n49_var_β
#-----------------------------------------------------------------------------------------------------------------------
n51_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              eax, 5;                              je    .Lx462_1
                        cmp              eax, 3;                              jne   .Lx462_0
                        mov              eax, dword ptr [rsp + 48]            # call
                        cmp              eax, 3;                              jne   .Lx462_0
.Lx462_1:               mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n52_cmp_test_α
.Lx462_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # call
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 110
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n52_cmp_test_α
n51_coerce_numeric_β:   add              rsp, 16;                             jmp   n50_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n52_cmp_test_α:         sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              edx, 3;                              jne   .Lx464_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jge   .Lx464_239
                        add              rsp, 16;                             jmp   n51_coerce_numeric_β
.Lx464_239:             mov              qword ptr [rsp + 0], 0               # result
                        mov              qword ptr [rsp + 8], 0;              jmp   n53_var_α
.Lx464_0:               lea              rdi, [rsp + 32]                      # coerce_numeric
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_cmp_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            jns   .Lx464_240
                        add              rsp, 16;                             jmp   n51_coerce_numeric_β
.Lx464_240:             mov              qword ptr [rsp + 0], 0               # result
                        mov              qword ptr [rsp + 8], 0;              jmp   n53_var_α
n52_cmp_test_β:         add              rsp, 16;                             jmp   n51_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n53_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n54_binop_α
n53_var_β:              add              rsp, 16;                             jmp   n52_cmp_test_β
#-----------------------------------------------------------------------------------------------------------------------
n54_binop_α:            sub              rsp, 16
                        mov              rax, qword ptr [rsp + 16]            # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n55_assign_α
n54_binop_β:            add              rsp, 16;                             jmp   n53_var_β
#-----------------------------------------------------------------------------------------------------------------------
n55_assign_α:           mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax             # pad_right
                        mov              qword ptr [r9 + 72], rdx;            jmp   n56_statement_end_α
n55_assign_β:                                                                 jmp   n46_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_end_α:    add              rsp, 128;                            jmp   RETURN
n56_statement_end_β:    add              rsp, 128;                            jmp   n57_statement_begin_α
#=======================================================================================================================
#         <stmt 10, line 47: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_begin_α:                                                        jmp   n58_var_α
n57_statement_begin_β:                                                        jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n58_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n59_var_α
n58_var_β:              add              rsp, 16;                             jmp   n57_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n59_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # c
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n60_var_α
n59_var_β:              add              rsp, 16
                        add              rsp, 16;                             jmp   n57_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n60_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]             # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n61_var_α
n60_var_β:              add              rsp, 16;                             jmp   n59_var_β
#-----------------------------------------------------------------------------------------------------------------------
n61_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n62_call_α
n61_var_β:              add              rsp, 16;                             jmp   n60_var_β
#-----------------------------------------------------------------------------------------------------------------------
n62_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd477:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd477]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx476_240
                        add              rsp, 16;                             jmp   n61_var_β
.Lx476_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n63_binop_α
n62_call_β:             add              rsp, 16;                             jmp   n61_var_β
#-----------------------------------------------------------------------------------------------------------------------
n63_binop_α:            sub              rsp, 16
                        mov              eax, dword ptr [rsp + 48]            # var
                        mov              ecx, dword ptr [rsp + 16]            # call
                        mov              edx, eax
                        and              edx, ecx
                        cmp              edx, 3;                              jne   .Lx478_2
                        mov              rax, qword ptr [rsp + 56]            # var
                        mov              rdx, qword ptr [rsp + 24]            # call
                        sub              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx478_7
.Lx478_2:               and              edx, 1;                              jz    .Lx478_0
                        mov              rsi, qword ptr [rsp + 56]            # var
                        mov              rdi, qword ptr [rsp + 24]            # call
                        cmp              eax, 5;                              je    .Lx478_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx478_4
.Lx478_3:               movq             xmm0, rsi
.Lx478_4:               cmp              ecx, 5;                              je    .Lx478_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx478_6
.Lx478_5:               movq             xmm1, rdi
.Lx478_6:               subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx478_7:                                                                     jmp   n64_call_α
.Lx478_0:               mov              rdi, qword ptr [rsp + 48]            # var
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_sub@PLT
                        cmp              eax, 104;                            jne   .Lx478_240
                        add              rsp, 32;                             jmp   n61_var_β
.Lx478_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n64_call_α
n63_binop_β:            add              rsp, 32;                             jmp   n61_var_β
#-----------------------------------------------------------------------------------------------------------------------
n64_call_α:             sub              rsp, 16
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
.Lrkfnzd480:            .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd480]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              eax, 104;                            jne   .Lx479_240
                        add              rsp, 16;                             jmp   n63_binop_β
.Lx479_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n65_binop_α
n64_call_β:             add              rsp, 16;                             jmp   n63_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n65_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]           # var
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 16]            # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n66_assign_α
n65_binop_β:            add              rsp, 32;                             jmp   n63_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n66_assign_α:           mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax             # pad_right
                        mov              qword ptr [r9 + 72], rdx;            jmp   n67_statement_end_α
n66_assign_β:                                                                 jmp   n57_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_end_α:    add              rsp, 128;                            jmp   RETURN
n67_statement_end_β:    add              rsp, 128;                            jmp   RETURN
#=======================================================================================================================
# pad_right_end  <stmt 11, line 49: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_begin_α:                                                        jmp   n69_statement_end_α
n68_statement_begin_β:                                                        jmp   n70_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_end_α:                                                          jmp   n70_statement_begin_α
n69_statement_end_β:                                                          jmp   n70_statement_begin_α
#=======================================================================================================================
#         <stmt 12, line 50: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_begin_α:                                                        jmp   n71_define_α
n70_statement_begin_β:                                                        jmp   n104_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n71_define_α:           mov              rdi, qword ptr [rip + .Lx492_0]
                        mov              rsi, qword ptr [rip + .Lx492_1]
                        mov              edx, 3
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n73_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx492_0]
                        lea              rsi, [rip + ltrim_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n72_statement_end_α
n71_define_β:                                                                 jmp   n70_statement_begin_β
.Lx492_0:               .quad            .Lx492_0_s
.Lx492_0_s:             .string          "ltrim"
.Lx492_1:               .quad            .Lx492_1_s
.Lx492_1_s:             .string          "s,ws,r"
                                                                              jmp   .Lx493_245
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
                        mov              qword ptr [rsp + 48], r10
                        mov              qword ptr [rsp + 56], r11
                        mov              qword ptr [rsp + 64], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        cmp              rdx, 0;                              jbe   .Lx493_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx493_41
.Lx493_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx493_41:              lea              r10, [rip + ltrim_γ]
                        lea              r11, [rip + ltrim_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n73_statement_begin_α];  jmp   rax
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
                        cmp              rdx, 0;                              jbe   .Lx493_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx493_110
.Lx493_80:              mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 24], rax
.Lx493_110:             mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx493_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx493_180
.Lx493_150:             mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 24], rax
.Lx493_180:             mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 96
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx493_245:
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_end_α:                                                          jmp   n104_statement_begin_α
n72_statement_end_β:                                                          jmp   n104_statement_begin_α
#=======================================================================================================================
# ltrim  <stmt 13, line 51: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_begin_α:                                                        jmp   n74_lit_string_α
n73_statement_begin_β:                                                        jmp   n86_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx498_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n75_lit_integer_α
n74_lit_string_β:       add              rsp, 16;                             jmp   n73_statement_begin_β
.Lx498_0:               .quad            .Lx498_0_s
.Lx498_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n75_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx499_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n76_call_α
n75_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n73_statement_begin_β
.Lx499_0:               .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n76_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd77:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd77]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx500_240
                        add              rsp, 16;                             jmp   n75_lit_integer_β
.Lx500_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n77_binop_α
n76_call_β:             add              rsp, 16;                             jmp   n75_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n77_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]            # lit_string
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n78_lit_integer_α
n77_binop_β:            add              rsp, 32;                             jmp   n75_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n78_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx502_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n79_call_α
n78_lit_integer_β:      add              rsp, 16;                             jmp   n77_binop_β
.Lx502_0:               .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n79_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd80:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd80]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx503_240
                        add              rsp, 16;                             jmp   n78_lit_integer_β
.Lx503_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n80_binop_α
n79_call_β:             add              rsp, 16;                             jmp   n78_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n80_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]            # binop
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n81_lit_integer_α
n80_binop_β:            add              rsp, 32;                             jmp   n78_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n81_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx505_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n82_call_α
n81_lit_integer_β:      add              rsp, 16;                             jmp   n80_binop_β
.Lx505_0:               .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n82_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd83:         .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd83]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx506_240
                        add              rsp, 16;                             jmp   n81_lit_integer_β
.Lx506_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n83_binop_α
n82_call_β:             add              rsp, 16;                             jmp   n81_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n83_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]            # binop
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n84_assign_α
n83_binop_β:            add              rsp, 32;                             jmp   n81_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n84_assign_α:           mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax             # ws
                        mov              qword ptr [r9 + 104], rdx;           jmp   n85_statement_end_α
n84_assign_β:                                                                 jmp   n73_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n85_statement_end_α:    add              rsp, 160;                            jmp   n86_statement_begin_α
n85_statement_end_β:    add              rsp, 160;                            jmp   n86_statement_begin_α
#=======================================================================================================================
#         <stmt 14, line 52: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n86_statement_begin_α:                                                        jmp   n87_var_α
n86_statement_begin_β:                                                        jmp   n100_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n87_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n88_match_begin_α
n87_var_β:              add              rsp, 16;                             jmp   n100_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n88_match_begin_α:      mov              rdi, qword ptr [rsp + 0]             # var
                        mov              rsi, qword ptr [rsp + 8]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 40
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx515_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx515_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n89_match_pos_α
n88_match_begin_β:
.Lx515_13:              lea              rsp, [rbp + -72]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx515_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx515_1
                                                                              jmp   .Lx515_0
.Lx515_1:
n88_match_begin_af:     mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13                             # sig
                        mov              rsi, r15                             # len
                        call             rt_match_ctx_restore@PLT
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   n97_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n89_match_pos_α:        mov              rax, 0
                        cmp              r14d, eax;                           jne   n88_match_begin_β
                                                                              jmp   n90_match_alternate_α
n89_match_pos_β:                                                              jmp   n88_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n90_match_alternate_α:  sub              rsp, 32
                        mov              dword ptr [rsp + 0], r14d
                        lea              rax, [rip + .Lx518_21]
                        mov              qword ptr [rsp + 16], rax;           jmp   n99_match_span_α
.Lx518_21:              lea              rax, [rip + .Lx518_19]
                        mov              qword ptr [rsp + 16], rax;           jmp   n98_match_lit_α
n90_match_alternate_s0: lea              rax, [rip + .Lx518_40]
                        mov              qword ptr [rsp + 8], rax;            jmp   n90_match_alternate_as
n90_match_alternate_s1: lea              rax, [rip + .Lx518_41]
                        mov              qword ptr [rsp + 8], rax;            jmp   n90_match_alternate_as
.Lx518_40:                                                                    jmp   n99_match_span_β
.Lx518_41:                                                                    jmp   n98_match_lit_β
n90_match_alternate_as:                                                       jmp   n91_match_assign_save_α
n90_match_alternate_β:  mov              rax, qword ptr [rsp + 8];            jmp   rax
n90_match_alternate_af: mov              r14d, dword ptr [rsp + 0]
                        mov              rax, qword ptr [rsp + 16];           jmp   rax
.Lx518_19:              add              rsp, 32;                             jmp   n89_match_pos_β
#-----------------------------------------------------------------------------------------------------------------------
n91_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n92_match_rem_α
n91_match_assign_save_β:
                        add              rsp, 16;                             jmp   n90_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n92_match_rem_α:        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d;                          jmp   n93_match_assign_cond_α
n92_match_rem_β:        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n90_match_alternate_β
#-----------------------------------------------------------------------------------------------------------------------
n93_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n94_match_end_α
n93_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n92_match_rem_β
#-----------------------------------------------------------------------------------------------------------------------
n94_match_end_α:        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
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
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx525_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n88_match_begin_af
.Lx525_13:              add              rsp, 16
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
                        pop              rbp;                                 jmp   n95_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n95_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx526_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n96_match_replace_α
n95_lit_string_β:       add              rsp, 16
                        add              rsp, 48;                             jmp   n100_statement_begin_α
.Lx526_0:               .quad            .Lx526_0_s
.Lx526_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n96_match_replace_α:    mov              rdi, qword ptr [rip + .Lx528_0]
                        mov              rsi, qword ptr [rsp + 16]            # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              ecx, dword ptr [r12 + -16]           # repl_start
                        mov              r8, qword ptr [r12 + -8]             # repl_end
                        sub              r12, 16
                        lea              r9, [rsp + 0]                        # lit_string
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_replace@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16;                             jmp   .Lx528_1
.Lx528_0:               .quad            .Lx528_0_s
.Lx528_0_s:             .string          "s"
.Lx528_1:                                                                     jmp   n97_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n97_statement_end_α:    add              rsp, 16;                             jmp   n100_statement_begin_α
n97_statement_end_β:    add              rsp, 64;                             jmp   n100_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n98_match_lit_α:                                                              jmp   n90_match_alternate_s1
n98_match_lit_β:                                                              jmp   n90_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n99_match_span_α:       lea              rdi, [rip + .S1]
                        lea              rsi, [rbp + -80]
                        lea              rdx, [rbp + -72]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_pat_prim_str@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            js    n90_match_alternate_af
                        mov              r8, qword ptr [rbp + -80]
                        mov              dword ptr [rbp + -80], 0
.Lx534_0:               mov              eax, r14d
                        add              eax, dword ptr [rbp + -80]
                        cmp              eax, r15d;                           jge   .Lx534_1
                        movsxd           rcx, eax
                        movzx            esi, byte ptr [r13+rcx]
                        mov              eax, dword ptr [rbp + -72]
                        mov              edx, 0
.Lx534_2:               cmp              edx, eax;                            jge   .Lx534_1
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi;                            je    .Lx534_3
                        add              edx, 1;                              jmp   .Lx534_2
.Lx534_3:               add              dword ptr [rbp + -80], 1;            jmp   .Lx534_0
.Lx534_1:               mov              eax, dword ptr [rbp + -80]
                        test             eax, eax;                            jle   n90_match_alternate_af
                        mov              edx, r14d
                        mov              dword ptr [rbp + -76], edx
                        add              edx, eax
                        mov              r14d, edx;                           jmp   n90_match_alternate_s0
n99_match_span_β:       mov              r14d, dword ptr [rbp + -76];         jmp   n90_match_alternate_af
#=======================================================================================================================
#         <stmt 15, line 53: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n100_statement_begin_α:                                                       jmp   n101_var_α
n100_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n101_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 112]            # r
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n102_assign_α
n101_var_β:             add              rsp, 16;                             jmp   n100_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n102_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax             # ltrim
                        mov              qword ptr [r9 + 88], rdx;            jmp   n103_statement_end_α
n102_assign_β:                                                                jmp   n100_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_end_α:   add              rsp, 16;                             jmp   RETURN
n103_statement_end_β:   add              rsp, 16;                             jmp   RETURN
#=======================================================================================================================
# ltrim_end  <stmt 16, line 55: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_begin_α:                                                       jmp   n105_statement_end_α
n104_statement_begin_β:                                                       jmp   n106_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n105_statement_end_α:                                                         jmp   n106_statement_begin_α
n105_statement_end_β:                                                         jmp   n106_statement_begin_α
#=======================================================================================================================
#         <stmt 17, line 56: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n106_statement_begin_α:                                                       jmp   n107_define_α
n106_statement_begin_β:                                                       jmp   n162_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n107_define_α:          mov              rdi, qword ptr [rip + .Lx548_0]
                        mov              rsi, qword ptr [rip + .Lx548_1]
                        mov              edx, 4
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n109_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx548_0]
                        lea              rsi, [rip + rtrim_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n108_statement_end_α
n107_define_β:                                                                jmp   n106_statement_begin_β
.Lx548_0:               .quad            .Lx548_0_s
.Lx548_0_s:             .string          "rtrim"
.Lx548_1:               .quad            .Lx548_1_s
.Lx548_1_s:             .string          "s,ws,i,ch"
                                                                              jmp   .Lx549_245
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
                        mov              qword ptr [rsp + 64], r10
                        mov              qword ptr [rsp + 72], r11
                        mov              qword ptr [rsp + 80], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 112]
                        cmp              rdx, 0;                              jbe   .Lx549_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx549_41
.Lx549_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 96], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 104], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx549_41:              lea              r10, [rip + rtrim_γ]
                        lea              r11, [rip + rtrim_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n109_statement_begin_α]; jmp   rax
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
                        cmp              rdx, 0;                              jbe   .Lx549_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx549_110
.Lx549_80:              mov              rax, qword ptr [rsp + 96]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [r9 + 24], rax
.Lx549_110:             mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx549_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx549_180
.Lx549_150:             mov              rax, qword ptr [rsp + 96]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [r9 + 24], rax
.Lx549_180:             mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 112
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx549_245:
#-----------------------------------------------------------------------------------------------------------------------
n108_statement_end_α:                                                         jmp   n162_statement_begin_α
n108_statement_end_β:                                                         jmp   n162_statement_begin_α
#=======================================================================================================================
# rtrim  <stmt 18, line 57: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n109_statement_begin_α:                                                       jmp   n110_lit_string_α
n109_statement_begin_β:                                                       jmp   n122_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n110_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx554_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n111_lit_integer_α
n110_lit_string_β:      add              rsp, 16;                             jmp   n109_statement_begin_β
.Lx554_0:               .quad            .Lx554_0_s
.Lx554_0_s:             .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n111_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx555_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n112_call_α
n111_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n109_statement_begin_β
.Lx555_0:               .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n112_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd113:        .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd113]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx556_240
                        add              rsp, 16;                             jmp   n111_lit_integer_β
.Lx556_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n113_binop_α
n112_call_β:            add              rsp, 16;                             jmp   n111_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n113_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]            # lit_string
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n114_lit_integer_α
n113_binop_β:           add              rsp, 32;                             jmp   n111_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n114_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx558_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n115_call_α
n114_lit_integer_β:     add              rsp, 16;                             jmp   n113_binop_β
.Lx558_0:               .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n115_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd116:        .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd116]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx559_240
                        add              rsp, 16;                             jmp   n114_lit_integer_β
.Lx559_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n116_binop_α
n115_call_β:            add              rsp, 16;                             jmp   n114_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n116_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]            # binop
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n117_lit_integer_α
n116_binop_β:           add              rsp, 32;                             jmp   n114_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n117_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx561_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n118_call_α
n117_lit_integer_β:     add              rsp, 16;                             jmp   n116_binop_β
.Lx561_0:               .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n118_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lbynamefnzd119:        .string          "CHAR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefnzd119]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx562_240
                        add              rsp, 16;                             jmp   n117_lit_integer_β
.Lx562_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n119_binop_α
n118_call_β:            add              rsp, 16;                             jmp   n117_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n119_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]            # binop
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n120_assign_α
n119_binop_β:           add              rsp, 32;                             jmp   n117_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n120_assign_α:          mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax             # ws
                        mov              qword ptr [r9 + 104], rdx;           jmp   n121_statement_end_α
n120_assign_β:                                                                jmp   n109_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n121_statement_end_α:   add              rsp, 160;                            jmp   n122_statement_begin_α
n121_statement_end_β:   add              rsp, 160;                            jmp   n122_statement_begin_α
#=======================================================================================================================
#         <stmt 19, line 58: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n122_statement_begin_α:                                                       jmp   n123_var_α
n122_statement_begin_β:                                                       jmp   n127_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n123_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n124_call_α
n123_var_β:             add              rsp, 16;                             jmp   n122_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n124_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd571:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd571]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx570_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n122_statement_begin_β
.Lx570_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n125_assign_α
n124_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n122_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n125_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # i
                        mov              qword ptr [r9 + 152], rdx;           jmp   n126_statement_end_α
n125_assign_β:                                                                jmp   n122_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n126_statement_end_α:   add              rsp, 32;                             jmp   n127_statement_begin_α
n126_statement_end_β:   add              rsp, 32;                             jmp   n127_statement_begin_α
#=======================================================================================================================
# rtrim0  <stmt 20, line 59: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n127_statement_begin_α:                                                       jmp   n128_var_α
n127_statement_begin_β:                                                       jmp   n134_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n128_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n129_lit_integer_α
n128_var_β:             add              rsp, 16;                             jmp   n127_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n129_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx578_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n130_coerce_numeric_α
n129_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n127_statement_begin_β
.Lx578_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n130_coerce_numeric_α:  sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              eax, 5;                              je    .Lx580_1
                        cmp              eax, 3;                              jne   .Lx580_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              eax, 3;                              jne   .Lx580_0
.Lx580_1:               mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n131_coerce_numeric_α
.Lx580_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 118
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n131_coerce_numeric_α
n130_coerce_numeric_β:  add              rsp, 16;                             jmp   n129_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n131_coerce_numeric_α:  sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              eax, 5;                              je    .Lx582_1
                        cmp              eax, 3;                              jne   .Lx582_0
                        mov              eax, dword ptr [rsp + 48]            # var
                        cmp              eax, 3;                              jne   .Lx582_0
.Lx582_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n132_cmp_test_α
.Lx582_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 119
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n132_cmp_test_α
n131_coerce_numeric_β:  add              rsp, 16;                             jmp   n130_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n132_cmp_test_α:        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              edx, 3;                              jne   .Lx584_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jle   .Lx584_239
                        add              rsp, 16;                             jmp   n131_coerce_numeric_β
.Lx584_239:                                                                   jmp   n133_statement_end_α
.Lx584_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_cmp_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            jle   .Lx584_240
                        add              rsp, 16;                             jmp   n131_coerce_numeric_β
.Lx584_240:                                                                   jmp   n133_statement_end_α
n132_cmp_test_β:        add              rsp, 16;                             jmp   n131_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n133_statement_end_α:   add              rsp, 80;                             jmp   n155_statement_begin_α
n133_statement_end_β:   add              rsp, 80;                             jmp   n134_statement_begin_α
#=======================================================================================================================
#         <stmt 21, line 60: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n134_statement_begin_α:                                                       jmp   n135_var_α
n134_statement_begin_β:                                                       jmp   n141_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n135_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n136_var_α
n135_var_β:             add              rsp, 16;                             jmp   n134_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n136_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n137_lit_integer_α
n136_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n134_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n137_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx591_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n138_call_α
n137_lit_integer_β:     add              rsp, 16;                             jmp   n136_var_β
.Lx591_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n138_call_α:            sub              rsp, 16
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
.Lrkfnzd593:            .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd593]
                        lea              rsi, [rsp + 0]
                        mov              edx, 3
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 48
                        cmp              eax, 104;                            jne   .Lx592_240
                        add              rsp, 16;                             jmp   n137_lit_integer_β
.Lx592_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n139_assign_α
n138_call_β:            add              rsp, 16;                             jmp   n137_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n139_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 160], rax            # ch
                        mov              qword ptr [r9 + 168], rdx;           jmp   n140_statement_end_α
n139_assign_β:                                                                jmp   n134_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n140_statement_end_α:   add              rsp, 64;                             jmp   n141_statement_begin_α
n140_statement_end_β:   add              rsp, 64;                             jmp   n141_statement_begin_α
#=======================================================================================================================
#         <stmt 22, line 61: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n141_statement_begin_α:                                                       jmp   n142_var_α
n141_statement_begin_β:                                                       jmp   n155_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n142_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 160]            # ch
                        mov              rdx, qword ptr [r9 + 168]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n143_var_α
n142_var_β:             add              rsp, 16;                             jmp   n155_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n143_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 96]             # ws
                        mov              rdx, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n144_coerce_string_α
n143_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n155_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n144_coerce_string_α:   sub              rsp, 16
                        lea              rdi, [rsp + 16]                      # var
                        lea              rsi, [rsp + 0]                       # result
                        mov              rdx, 3866683
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_str_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n145_match_begin_α
n144_coerce_string_β:   add              rsp, 16;                             jmp   n143_var_β
#-----------------------------------------------------------------------------------------------------------------------
n145_match_begin_α:     mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 24
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx604_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx604_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n146_match_any_α
n145_match_begin_β:
.Lx604_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx604_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx604_1
                                                                              jmp   .Lx604_0
.Lx604_1:
n145_match_begin_af:    mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13                             # sig
                        mov              rsi, r15                             # len
                        call             rt_match_ctx_restore@PLT
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   n144_coerce_string_β
#-----------------------------------------------------------------------------------------------------------------------
n146_match_any_α:       mov              eax, r14d
                        cmp              eax, r15d;                           jge   n145_match_begin_β
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        mov              edi, esi
                        mov              rsi, qword ptr [rsp + 72]            # coerce_string
                        mov              edx, dword ptr [rsp + 68]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_sg_member@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            je    n145_match_begin_β
                        add              r14d, 1;                             jmp   n147_match_end_α
n146_match_any_β:       sub              r14d, 1;                             jmp   n145_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n147_match_end_α:       mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
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
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx608_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n145_match_begin_af
.Lx608_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n148_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n148_statement_end_α:   add              rsp, 48;                             jmp   n149_statement_begin_α
n148_statement_end_β:   add              rsp, 48;                             jmp   n155_statement_begin_α
#=======================================================================================================================
#         <stmt 23, line 62: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n149_statement_begin_α:                                                       jmp   n150_var_α
n149_statement_begin_β:                                                       jmp   n127_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n150_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n151_lit_integer_α
n150_var_β:             add              rsp, 16;                             jmp   n149_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n151_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx614_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n152_binop_α
n151_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n149_statement_begin_β
.Lx614_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n152_binop_α:           sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              eax, 3;                              jne   .Lx615_2
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, 1
                        sub              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx615_7
.Lx615_2:               mov              edx, eax
                        and              edx, 1;                              jz    .Lx615_0
                        mov              rsi, qword ptr [rsp + 40]            # var
                        mov              rdi, 1
                        cmp              eax, 5;                              je    .Lx615_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx615_4
.Lx615_3:               movq             xmm0, rsi
.Lx615_4:               cvtsi2sd         xmm1, rdi
                        subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx615_7:                                                                     jmp   n153_assign_α
.Lx615_0:               mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_sub@PLT
                        cmp              eax, 104;                            jne   .Lx615_240
                        add              rsp, 16;                             jmp   n151_lit_integer_β
.Lx615_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n153_assign_α
n152_binop_β:           add              rsp, 16;                             jmp   n151_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n153_assign_α:          mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # i
                        mov              qword ptr [r9 + 152], rdx;           jmp   n154_statement_end_α
n153_assign_β:                                                                jmp   n149_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n154_statement_end_α:   add              rsp, 48;                             jmp   n127_statement_begin_α
n154_statement_end_β:   add              rsp, 48;                             jmp   n127_statement_begin_α
#=======================================================================================================================
# rtrim1  <stmt 24, line 63: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n155_statement_begin_α:                                                       jmp   n156_var_α
n155_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n156_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n157_lit_integer_α
n156_var_β:             add              rsp, 16;                             jmp   n155_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n157_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx622_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n158_var_α
n157_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n155_statement_begin_β
.Lx622_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n158_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # i
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n159_call_α
n158_var_β:             add              rsp, 16;                             jmp   n157_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n159_call_α:            sub              rsp, 16
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
.Lrkfnzd625:            .string          "SUBSTR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd625]
                        lea              rsi, [rsp + 0]
                        mov              edx, 3
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 48
                        cmp              eax, 104;                            jne   .Lx624_240
                        add              rsp, 16;                             jmp   n158_var_β
.Lx624_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n160_assign_α
n159_call_β:            add              rsp, 16;                             jmp   n158_var_β
#-----------------------------------------------------------------------------------------------------------------------
n160_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax            # rtrim
                        mov              qword ptr [r9 + 136], rdx;           jmp   n161_statement_end_α
n160_assign_β:                                                                jmp   n155_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n161_statement_end_α:   add              rsp, 64;                             jmp   RETURN
n161_statement_end_β:   add              rsp, 64;                             jmp   RETURN
#=======================================================================================================================
# rtrim_end  <stmt 25, line 65: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n162_statement_begin_α:                                                       jmp   n163_statement_end_α
n162_statement_begin_β:                                                       jmp   n164_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n163_statement_end_α:                                                         jmp   n164_statement_begin_α
n163_statement_end_β:                                                         jmp   n164_statement_begin_α
#=======================================================================================================================
#         <stmt 26, line 66: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n164_statement_begin_α:                                                       jmp   n165_define_α
n164_statement_begin_β:                                                       jmp   n173_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n165_define_α:          mov              rdi, qword ptr [rip + .Lx636_0]
                        mov              rsi, qword ptr [rip + .Lx636_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n167_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx636_0]
                        lea              rsi, [rip + trimws_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n166_statement_end_α
n165_define_β:                                                                jmp   n164_statement_begin_β
.Lx636_0:               .quad            .Lx636_0_s
.Lx636_0_s:             .string          "trimws"
.Lx636_1:               .quad            .Lx636_1_s
.Lx636_1_s:             .string          "s"
                                                                              jmp   .Lx637_245
#-----------------------------------------------------------------------------------------------------------------------
trimws_α:               sub              rsp, 64
                        mov              rax, qword ptr [r9 + 176]            # trimws
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 176], 0
                        mov              qword ptr [r9 + 184], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx637_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx637_41
.Lx637_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx637_41:              lea              r10, [rip + trimws_γ]
                        lea              r11, [rip + trimws_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n167_statement_begin_α]; jmp   rax
trimws_γ:               mov              rdi, qword ptr [r9 + 176]            # trimws
                        mov              rsi, qword ptr [r9 + 184]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 176], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 184], rax
                        cmp              rdx, 0;                              jbe   .Lx637_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx637_110
.Lx637_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx637_110:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx637_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx637_180
.Lx637_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx637_180:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx637_245:
#-----------------------------------------------------------------------------------------------------------------------
n166_statement_end_α:                                                         jmp   n173_statement_begin_α
n166_statement_end_β:                                                         jmp   n173_statement_begin_α
#=======================================================================================================================
# trimws  <stmt 27, line 67: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n167_statement_begin_α:                                                       jmp   n168_var_α
n167_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n168_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n169_call_α
n168_var_β:             add              rsp, 16;                             jmp   n167_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n169_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig644z]
                        lea              rax, [rip + rtrim_α];                jmp   rax
.Lsig644z:              .quad            1
                        .quad            .Lx644_2
                        .quad            .Lx644_2
                        .quad            16
.Lx644_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx644_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx644_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx644_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n167_statement_begin_β
.Lx644_240:                                                                   jmp   n170_call_α
n169_call_β:                                                                  jmp   n167_statement_begin_β
.Lx644_0:               .quad            .Lx644_0_s
.Lx644_0_s:             .string          "rtrim"
#-----------------------------------------------------------------------------------------------------------------------
n170_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig646z]
                        lea              rax, [rip + ltrim_α];                jmp   rax
.Lsig646z:              .quad            1
                        .quad            .Lx646_2
                        .quad            .Lx646_2
                        .quad            16
.Lx646_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx646_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx646_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx646_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n167_statement_begin_β
.Lx646_240:                                                                   jmp   n171_assign_α
n170_call_β:                                                                  jmp   n167_statement_begin_β
.Lx646_0:               .quad            .Lx646_0_s
.Lx646_0_s:             .string          "ltrim"
#-----------------------------------------------------------------------------------------------------------------------
n171_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 176], rax            # trimws
                        mov              qword ptr [r9 + 184], rdx;           jmp   n172_statement_end_α
n171_assign_β:                                                                jmp   n167_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n172_statement_end_α:   add              rsp, 48;                             jmp   RETURN
n172_statement_end_β:   add              rsp, 48;                             jmp   RETURN
#=======================================================================================================================
# trimws_end  <stmt 28, line 69: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n173_statement_begin_α:                                                       jmp   n174_statement_end_α
n173_statement_begin_β:                                                       jmp   n175_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n174_statement_end_α:                                                         jmp   n175_statement_begin_α
n174_statement_end_β:                                                         jmp   n175_statement_begin_α
#=======================================================================================================================
#         <stmt 29, line 70: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n175_statement_begin_α:                                                       jmp   n176_define_α
n175_statement_begin_β:                                                       jmp   n184_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n176_define_α:          mov              rdi, qword ptr [rip + .Lx657_0]
                        mov              rsi, qword ptr [rip + .Lx657_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + n178_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx657_0]
                        lea              rsi, [rip + repeat_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n177_statement_end_α
n176_define_β:                                                                jmp   n175_statement_begin_β
.Lx657_0:               .quad            .Lx657_0_s
.Lx657_0_s:             .string          "repeat"
.Lx657_1:               .quad            .Lx657_1_s
.Lx657_1_s:             .string          "s,n"
                                                                              jmp   .Lx658_245
#-----------------------------------------------------------------------------------------------------------------------
repeat_α:               sub              rsp, 80
                        mov              rax, qword ptr [r9 + 192]            # repeat
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 192], 0
                        mov              qword ptr [r9 + 200], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        cmp              rdx, 0;                              jbe   .Lx658_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx658_41
.Lx658_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx658_41:              cmp              rdx, 1;                              jbe   .Lx658_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 32]             # n
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx658_42
.Lx658_11:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx658_42:              lea              r10, [rip + repeat_γ]
                        lea              r11, [rip + repeat_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n178_statement_begin_α]; jmp   rax
repeat_γ:               mov              rdi, qword ptr [r9 + 192]            # repeat
                        mov              rsi, qword ptr [r9 + 200]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 192], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 200], rax
                        cmp              rdx, 0;                              jbe   .Lx658_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx658_110
.Lx658_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx658_110:             cmp              rdx, 1;                              jbe   .Lx658_81
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 32], rax             # n
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx658_111
.Lx658_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 40], rax
.Lx658_111:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx658_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx658_180
.Lx658_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx658_180:             cmp              rdx, 1;                              jbe   .Lx658_151
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 32], rax             # n
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx658_181
.Lx658_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 40], rax
.Lx658_181:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 80
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx658_245:
#-----------------------------------------------------------------------------------------------------------------------
n177_statement_end_α:                                                         jmp   n184_statement_begin_α
n177_statement_end_β:                                                         jmp   n184_statement_begin_α
#=======================================================================================================================
# repeat  <stmt 30, line 71: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n178_statement_begin_α:                                                       jmp   n179_var_α
n178_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n179_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n180_var_α
n179_var_β:             add              rsp, 16;                             jmp   n178_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n180_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]             # n
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n181_call_α
n180_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n178_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n181_call_α:            sub              rsp, 16
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
.Lrkfnzd666:            .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd666]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              eax, 104;                            jne   .Lx665_240
                        add              rsp, 16;                             jmp   n180_var_β
.Lx665_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n182_assign_α
n181_call_β:            add              rsp, 16;                             jmp   n180_var_β
#-----------------------------------------------------------------------------------------------------------------------
n182_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 192], rax            # repeat
                        mov              qword ptr [r9 + 200], rdx;           jmp   n183_statement_end_α
n182_assign_β:                                                                jmp   n178_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n183_statement_end_α:   add              rsp, 48;                             jmp   RETURN
n183_statement_end_β:   add              rsp, 48;                             jmp   RETURN
#=======================================================================================================================
# repeat_end  <stmt 31, line 73: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n184_statement_begin_α:                                                       jmp   n185_statement_end_α
n184_statement_begin_β:                                                       jmp   n186_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n185_statement_end_α:                                                         jmp   n186_statement_begin_α
n185_statement_end_β:                                                         jmp   n186_statement_begin_α
#=======================================================================================================================
#         <stmt 32, line 74: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n186_statement_begin_α:                                                       jmp   n187_define_α
n186_statement_begin_β:                                                       jmp   n200_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n187_define_α:          mov              rdi, qword ptr [rip + .Lx677_0]
                        mov              rsi, qword ptr [rip + .Lx677_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + n189_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx677_0]
                        lea              rsi, [rip + contains_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n188_statement_end_α
n187_define_β:                                                                jmp   n186_statement_begin_β
.Lx677_0:               .quad            .Lx677_0_s
.Lx677_0_s:             .string          "contains"
.Lx677_1:               .quad            .Lx677_1_s
.Lx677_1_s:             .string          "s,t"
                                                                              jmp   .Lx678_245
#-----------------------------------------------------------------------------------------------------------------------
contains_α:             sub              rsp, 80
                        mov              rax, qword ptr [r9 + 208]            # contains
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 208], 0
                        mov              qword ptr [r9 + 216], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        cmp              rdx, 0;                              jbe   .Lx678_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx678_41
.Lx678_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx678_41:              cmp              rdx, 1;                              jbe   .Lx678_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 224]            # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx678_42
.Lx678_11:              mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx678_42:              lea              r10, [rip + contains_γ]
                        lea              r11, [rip + contains_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n189_statement_begin_α]; jmp   rax
contains_γ:             mov              rdi, qword ptr [r9 + 208]            # contains
                        mov              rsi, qword ptr [r9 + 216]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 208], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 216], rax
                        cmp              rdx, 0;                              jbe   .Lx678_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx678_110
.Lx678_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx678_110:             cmp              rdx, 1;                              jbe   .Lx678_81
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 224], rax            # t
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx678_111
.Lx678_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 232], rax
.Lx678_111:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx678_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx678_180
.Lx678_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx678_180:             cmp              rdx, 1;                              jbe   .Lx678_151
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 224], rax            # t
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx678_181
.Lx678_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 232], rax
.Lx678_181:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 80
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx678_245:
#-----------------------------------------------------------------------------------------------------------------------
n188_statement_end_α:                                                         jmp   n200_statement_begin_α
n188_statement_end_β:                                                         jmp   n200_statement_begin_α
#=======================================================================================================================
# contains  <stmt 33, line 75: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n189_statement_begin_α:                                                       jmp   n190_var_α
n189_statement_begin_β:                                                       jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n190_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n191_var_α
n190_var_β:             add              rsp, 16;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n191_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]            # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n192_coerce_string_α
n191_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n192_coerce_string_α:   sub              rsp, 16
                        lea              rdi, [rsp + 16]                      # var
                        lea              rsi, [rsp + 0]                       # result
                        mov              rdx, 4522053
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_str_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n193_var_α
n192_coerce_string_β:   add              rsp, 16;                             jmp   n191_var_β
#-----------------------------------------------------------------------------------------------------------------------
n193_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]            # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n194_assign_α
n193_var_β:             add              rsp, 16;                             jmp   n192_coerce_string_β
#-----------------------------------------------------------------------------------------------------------------------
n194_assign_α:          mov              rsi, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx688_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n195_match_begin_α
n194_assign_β:                                                                jmp   n193_var_β
.Lx688_0:               .quad            .Lx688_0_s
.Lx688_0_s:             .string          "PATV$0"
#-----------------------------------------------------------------------------------------------------------------------
n195_match_begin_α:     mov              rdi, qword ptr [rsp + 48]            # var
                        mov              rsi, qword ptr [rsp + 56]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 24
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx690_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx690_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n196_match_break_α
n195_match_begin_β:
.Lx690_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx690_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx690_1
                                                                              jmp   .Lx690_0
.Lx690_1:
n195_match_begin_af:    mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13                             # sig
                        mov              rsi, r15                             # len
                        call             rt_match_ctx_restore@PLT
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   n194_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n196_match_break_α:     sub              rsp, 16
                        mov              edi, r14d
                        mov              rsi, qword ptr [rsp + 104]           # coerce_string
                        mov              edx, dword ptr [rsp + 100]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_sg_scan_member@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              eax, r15d;                           jl    .Lx692_240
                        add              rsp, 16;                             jmp   n195_match_begin_β
.Lx692_240:             mov              dword ptr [rsp + 0], r14d
                        mov              r14d, eax;                           jmp   n197_match_defer_α
n196_match_break_β:     mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16;                             jmp   n195_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n197_match_defer_α:     lea              rdi, [rip + .S2]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx693_14
                        mov              rax, qword ptr [rdx + 0]
.Lx693_14:              test             rax, rax;                            jz    .Lx693_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx693_5]
                        push             rcx
                        lea              rcx, [rip + .Lx693_4]
                        push             rcx;                                 jmp   rax
.Lx693_4:                                                                     jmp   n198_match_end_α
.Lx693_5:                                                                     jmp   n196_match_break_β
.Lx693_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S2]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n196_match_break_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx693_6]
                        push             rcx
                        push             rax;                                 jmp   n198_match_end_α
.Lx693_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n196_match_break_β
n197_match_defer_β:     cmp              qword ptr [rsp + 0], 0;              jne   .Lx693_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx693_12
                                                                              jmp   rax
.Lx693_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n198_match_end_α:       mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
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
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx695_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n195_match_begin_af
.Lx695_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n199_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n199_statement_end_α:   add              rsp, 64;                             jmp   RETURN
n199_statement_end_β:   add              rsp, 80;                             jmp   FRETURN
#=======================================================================================================================
# contains_end  <stmt 34, line 77: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n200_statement_begin_α:                                                       jmp   n201_statement_end_α
n200_statement_begin_β:                                                       jmp   n202_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n201_statement_end_α:                                                         jmp   n202_statement_begin_α
n201_statement_end_β:                                                         jmp   n202_statement_begin_α
#=======================================================================================================================
#         <stmt 35, line 78: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n202_statement_begin_α:                                                       jmp   n203_define_α
n202_statement_begin_β:                                                       jmp   n214_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n203_define_α:          mov              rdi, qword ptr [rip + .Lx705_0]
                        mov              rsi, qword ptr [rip + .Lx705_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + n205_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx705_0]
                        lea              rsi, [rip + startswith_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n204_statement_end_α
n203_define_β:                                                                jmp   n202_statement_begin_β
.Lx705_0:               .quad            .Lx705_0_s
.Lx705_0_s:             .string          "startswith"
.Lx705_1:               .quad            .Lx705_1_s
.Lx705_1_s:             .string          "s,t"
                                                                              jmp   .Lx706_245
#-----------------------------------------------------------------------------------------------------------------------
startswith_α:           sub              rsp, 80
                        mov              rax, qword ptr [r9 + 240]            # startswith
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 248]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 240], 0
                        mov              qword ptr [r9 + 248], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        cmp              rdx, 0;                              jbe   .Lx706_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx706_41
.Lx706_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx706_41:              cmp              rdx, 1;                              jbe   .Lx706_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 224]            # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx706_42
.Lx706_11:              mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx706_42:              lea              r10, [rip + startswith_γ]
                        lea              r11, [rip + startswith_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n205_statement_begin_α]; jmp   rax
startswith_γ:           mov              rdi, qword ptr [r9 + 240]            # startswith
                        mov              rsi, qword ptr [r9 + 248]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 240], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 248], rax
                        cmp              rdx, 0;                              jbe   .Lx706_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx706_110
.Lx706_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx706_110:             cmp              rdx, 1;                              jbe   .Lx706_81
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 224], rax            # t
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx706_111
.Lx706_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 232], rax
.Lx706_111:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx706_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx706_180
.Lx706_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx706_180:             cmp              rdx, 1;                              jbe   .Lx706_151
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 224], rax            # t
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx706_181
.Lx706_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 232], rax
.Lx706_181:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 80
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx706_245:
#-----------------------------------------------------------------------------------------------------------------------
n204_statement_end_α:                                                         jmp   n214_statement_begin_α
n204_statement_end_β:                                                         jmp   n214_statement_begin_α
#=======================================================================================================================
# startswith  <stmt 36, line 79: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n205_statement_begin_α:                                                       jmp   n206_var_α
n205_statement_begin_β:                                                       jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n206_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n207_var_α
n206_var_β:             add              rsp, 16;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n207_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]            # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n208_assign_α
n207_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n208_assign_α:          mov              rsi, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx713_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n209_match_begin_α
n208_assign_β:                                                                jmp   n207_var_β
.Lx713_0:               .quad            .Lx713_0_s
.Lx713_0_s:             .string          "PATV$1"
#-----------------------------------------------------------------------------------------------------------------------
n209_match_begin_α:     mov              rdi, qword ptr [rsp + 16]            # var
                        mov              rsi, qword ptr [rsp + 24]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 24
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx715_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx715_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n210_match_pos_α
n209_match_begin_β:
.Lx715_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx715_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx715_1
                                                                              jmp   .Lx715_0
.Lx715_1:
n209_match_begin_af:    mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13                             # sig
                        mov              rsi, r15                             # len
                        call             rt_match_ctx_restore@PLT
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   n208_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n210_match_pos_α:       mov              rax, 0
                        cmp              r14d, eax;                           jne   n209_match_begin_β
                                                                              jmp   n211_match_defer_α
n210_match_pos_β:                                                             jmp   n209_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n211_match_defer_α:     lea              rdi, [rip + .S3]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx717_14
                        mov              rax, qword ptr [rdx + 0]
.Lx717_14:              test             rax, rax;                            jz    .Lx717_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx717_5]
                        push             rcx
                        lea              rcx, [rip + .Lx717_4]
                        push             rcx;                                 jmp   rax
.Lx717_4:                                                                     jmp   n212_match_end_α
.Lx717_5:                                                                     jmp   n209_match_begin_β
.Lx717_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S3]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n209_match_begin_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx717_6]
                        push             rcx
                        push             rax;                                 jmp   n212_match_end_α
.Lx717_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n209_match_begin_β
n211_match_defer_β:     cmp              qword ptr [rsp + 0], 0;              jne   .Lx717_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx717_12
                                                                              jmp   rax
.Lx717_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n212_match_end_α:       mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
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
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx719_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n209_match_begin_af
.Lx719_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n213_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n213_statement_end_α:   add              rsp, 32;                             jmp   RETURN
n213_statement_end_β:   add              rsp, 32;                             jmp   FRETURN
#=======================================================================================================================
# startswith_end  <stmt 37, line 81: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n214_statement_begin_α:                                                       jmp   n215_statement_end_α
n214_statement_begin_β:                                                       jmp   n216_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n215_statement_end_α:                                                         jmp   n216_statement_begin_α
n215_statement_end_β:                                                         jmp   n216_statement_begin_α
#=======================================================================================================================
#         <stmt 38, line 82: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n216_statement_begin_α:                                                       jmp   n217_define_α
n216_statement_begin_β:                                                       jmp   n228_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n217_define_α:          mov              rdi, qword ptr [rip + .Lx729_0]
                        mov              rsi, qword ptr [rip + .Lx729_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + n219_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx729_0]
                        lea              rsi, [rip + endswith_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n218_statement_end_α
n217_define_β:                                                                jmp   n216_statement_begin_β
.Lx729_0:               .quad            .Lx729_0_s
.Lx729_0_s:             .string          "endswith"
.Lx729_1:               .quad            .Lx729_1_s
.Lx729_1_s:             .string          "s,t"
                                                                              jmp   .Lx730_245
#-----------------------------------------------------------------------------------------------------------------------
endswith_α:             sub              rsp, 80
                        mov              rax, qword ptr [r9 + 256]            # endswith
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 264]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 256], 0
                        mov              qword ptr [r9 + 264], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        cmp              rdx, 0;                              jbe   .Lx730_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx730_41
.Lx730_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx730_41:              cmp              rdx, 1;                              jbe   .Lx730_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 224]            # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx730_42
.Lx730_11:              mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx730_42:              lea              r10, [rip + endswith_γ]
                        lea              r11, [rip + endswith_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n219_statement_begin_α]; jmp   rax
endswith_γ:             mov              rdi, qword ptr [r9 + 256]            # endswith
                        mov              rsi, qword ptr [r9 + 264]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 80]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 256], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 264], rax
                        cmp              rdx, 0;                              jbe   .Lx730_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx730_110
.Lx730_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx730_110:             cmp              rdx, 1;                              jbe   .Lx730_81
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 224], rax            # t
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx730_111
.Lx730_81:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 232], rax
.Lx730_111:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx730_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx730_180
.Lx730_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx730_180:             cmp              rdx, 1;                              jbe   .Lx730_151
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 224], rax            # t
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx730_181
.Lx730_151:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 232], rax
.Lx730_181:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 80
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx730_245:
#-----------------------------------------------------------------------------------------------------------------------
n218_statement_end_α:                                                         jmp   n228_statement_begin_α
n218_statement_end_β:                                                         jmp   n228_statement_begin_α
#=======================================================================================================================
# endswith  <stmt 39, line 83: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n219_statement_begin_α:                                                       jmp   n220_var_α
n219_statement_begin_β:                                                       jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n220_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n221_var_α
n220_var_β:             add              rsp, 16;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n221_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]            # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n222_assign_α
n221_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n222_assign_α:          mov              rsi, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx737_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n223_match_begin_α
n222_assign_β:                                                                jmp   n221_var_β
.Lx737_0:               .quad            .Lx737_0_s
.Lx737_0_s:             .string          "PATV$2"
#-----------------------------------------------------------------------------------------------------------------------
n223_match_begin_α:     mov              rdi, qword ptr [rsp + 16]            # var
                        mov              rsi, qword ptr [rsp + 24]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 24
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx739_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx739_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n224_match_defer_α
n223_match_begin_β:
.Lx739_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx739_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx739_1
                                                                              jmp   .Lx739_0
.Lx739_1:
n223_match_begin_af:    mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13                             # sig
                        mov              rsi, r15                             # len
                        call             rt_match_ctx_restore@PLT
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   n222_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n224_match_defer_α:     lea              rdi, [rip + .S4]
                        xor              esi, esi
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_get_pat_dtp@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdx, rax
                        test             rax, rax;                            je    .Lx740_14
                        mov              rax, qword ptr [rdx + 0]
.Lx740_14:              test             rax, rax;                            jz    .Lx740_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx740_5]
                        push             rcx
                        lea              rcx, [rip + .Lx740_4]
                        push             rcx;                                 jmp   rax
.Lx740_4:                                                                     jmp   n225_match_rpos_α
.Lx740_5:                                                                     jmp   n223_match_begin_β
.Lx740_0:               push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S4]
                        mov              esi, r14d
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_defer_run_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        test             eax, eax;                            js    n223_match_begin_β
                        mov              ecx, r14d
                        mov              r14d, eax
                        lea              rax, [rip + .Lx740_6]
                        push             rcx
                        push             rax;                                 jmp   n225_match_rpos_α
.Lx740_6:               add              rsp, 8
                        pop              rax
                        mov              r14d, eax;                           jmp   n223_match_begin_β
n224_match_defer_β:     cmp              qword ptr [rsp + 0], 0;              jne   .Lx740_12
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL]
                        mov              rax, qword ptr [rcx + 248]
                        test             rax, rax;                            je    .Lx740_12
                                                                              jmp   rax
.Lx740_12:                                                                    jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n225_match_rpos_α:      mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx;                           jne   n224_match_defer_β
                                                                              jmp   n226_match_end_α
n225_match_rpos_β:                                                            jmp   n224_match_defer_β
#-----------------------------------------------------------------------------------------------------------------------
n226_match_end_α:       mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
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
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx743_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n223_match_begin_af
.Lx743_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n227_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n227_statement_end_α:   add              rsp, 32;                             jmp   RETURN
n227_statement_end_β:   add              rsp, 32;                             jmp   FRETURN
#=======================================================================================================================
# endswith_end  <stmt 40, line 85: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n228_statement_begin_α:                                                       jmp   n229_statement_end_α
n228_statement_begin_β:                                                       jmp   n230_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n229_statement_end_α:                                                         jmp   n230_statement_begin_α
n229_statement_end_β:                                                         jmp   n230_statement_begin_α
#=======================================================================================================================
#         <stmt 41, line 86: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n230_statement_begin_α:                                                       jmp   n231_define_α
n230_statement_begin_β:                                                       jmp   n258_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n231_define_α:          mov              rdi, qword ptr [rip + .Lx753_0]
                        mov              rsi, qword ptr [rip + .Lx753_1]
                        mov              edx, 3
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + n233_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx753_0]
                        lea              rsi, [rip + index_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n232_statement_end_α
n231_define_β:                                                                jmp   n230_statement_begin_β
.Lx753_0:               .quad            .Lx753_0_s
.Lx753_0_s:             .string          "index"
.Lx753_1:               .quad            .Lx753_1_s
.Lx753_1_s:             .string          "s,t,ix"
                                                                              jmp   .Lx754_245
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
                        mov              qword ptr [rsp + 32], r10
                        mov              qword ptr [rsp + 40], r11
                        mov              qword ptr [rsp + 48], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 96]
                        cmp              rdx, 0;                              jbe   .Lx754_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx754_41
.Lx754_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx754_41:              cmp              rdx, 1;                              jbe   .Lx754_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 224]            # t
                        mov              qword ptr [r9 + 224], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 232]
                        mov              qword ptr [r9 + 232], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx754_42
.Lx754_11:              mov              rax, qword ptr [r9 + 224]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [r9 + 224], 0
                        mov              qword ptr [r9 + 232], 0
.Lx754_42:              lea              r10, [rip + index_γ]
                        lea              r11, [rip + index_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n233_statement_begin_α]; jmp   rax
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
                        cmp              rdx, 0;                              jbe   .Lx754_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx754_110
.Lx754_80:              mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 24], rax
.Lx754_110:             cmp              rdx, 1;                              jbe   .Lx754_81
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 224], rax            # t
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx754_111
.Lx754_81:              mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 232], rax
.Lx754_111:             mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx754_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx754_180
.Lx754_150:             mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 24], rax
.Lx754_180:             cmp              rdx, 1;                              jbe   .Lx754_151
                        mov              r11, qword ptr [rcx + 32]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 224], rax            # t
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 232], rax;           jmp   .Lx754_181
.Lx754_151:             mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [r9 + 224], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [r9 + 232], rax
.Lx754_181:             mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 96
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx754_245:
#-----------------------------------------------------------------------------------------------------------------------
n232_statement_end_α:                                                         jmp   n258_statement_begin_α
n232_statement_end_β:                                                         jmp   n258_statement_begin_α
#=======================================================================================================================
# index  <stmt 42, line 87: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n233_statement_begin_α:                                                       jmp   n234_lit_integer_α
n233_statement_begin_β:                                                       jmp   n237_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n234_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx759_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n235_assign_α
n234_lit_integer_β:     add              rsp, 16;                             jmp   n233_statement_begin_β
.Lx759_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n235_assign_α:          mov              rax, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 272], rax            # index
                        mov              qword ptr [r9 + 280], rdx;           jmp   n236_statement_end_α
n235_assign_β:                                                                jmp   n233_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n236_statement_end_α:   add              rsp, 16;                             jmp   n237_statement_begin_α
n236_statement_end_β:   add              rsp, 16;                             jmp   n237_statement_begin_α
#=======================================================================================================================
#         <stmt 43, line 88: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n237_statement_begin_α:                                                       jmp   n238_var_α
n237_statement_begin_β:                                                       jmp   n241_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n238_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n239_assign_α
n238_var_β:             add              rsp, 16;                             jmp   n237_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n239_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 288], rax            # ix
                        mov              qword ptr [r9 + 296], rdx;           jmp   n240_statement_end_α
n239_assign_β:                                                                jmp   n237_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n240_statement_end_α:   add              rsp, 16;                             jmp   n241_statement_begin_α
n240_statement_end_β:   add              rsp, 16;                             jmp   n241_statement_begin_α
#=======================================================================================================================
#         <stmt 44, line 89: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n241_statement_begin_α:                                                       jmp   n242_var_α
n241_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n242_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 288]            # ix
                        mov              rdx, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n243_var_α
n242_var_β:             add              rsp, 16;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n243_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]            # t
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n244_coerce_string_α
n243_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n244_coerce_string_α:   sub              rsp, 16
                        lea              rdi, [rsp + 16]                      # var
                        lea              rsi, [rsp + 0]                       # result
                        mov              rdx, 4522053
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_str_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n245_match_begin_α
n244_coerce_string_β:   add              rsp, 16;                             jmp   n243_var_β
#-----------------------------------------------------------------------------------------------------------------------
n245_match_begin_α:     mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 24
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx776_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx776_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n246_match_assign_save_α
n245_match_begin_β:
.Lx776_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx776_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx776_1
                                                                              jmp   .Lx776_0
.Lx776_1:
n245_match_begin_af:    mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13                             # sig
                        mov              rsi, r15                             # len
                        call             rt_match_ctx_restore@PLT
                        mov              rsp, rbp
                        pop              rbp;                                 jmp   n244_coerce_string_β
#-----------------------------------------------------------------------------------------------------------------------
n246_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n247_match_break_α
n246_match_assign_save_β:
                        add              rsp, 16;                             jmp   n245_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n247_match_break_α:     sub              rsp, 16
                        mov              edi, r14d
                        mov              rsi, qword ptr [rsp + 104]           # coerce_string
                        mov              edx, dword ptr [rsp + 100]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_sg_scan_member@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              eax, r15d;                           jl    .Lx780_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n245_match_begin_β
.Lx780_240:             mov              dword ptr [rsp + 0], r14d
                        mov              r14d, eax;                           jmp   n248_match_assign_cond_α
n247_match_break_β:     mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n245_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n248_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S5]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n249_match_end_α
n248_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n247_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n249_match_end_α:       mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
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
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx784_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n245_match_begin_af
.Lx784_13:              add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n250_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n250_statement_end_α:   add              rsp, 48;                             jmp   n251_statement_begin_α
n250_statement_end_β:   add              rsp, 80;                             jmp   RETURN
#=======================================================================================================================
#         <stmt 45, line 90: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n251_statement_begin_α:                                                       jmp   n252_var_α
n251_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n252_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 288]            # ix
                        mov              rdx, qword ptr [r9 + 296]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n253_call_α
n252_var_β:             add              rsp, 16;                             jmp   n251_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n253_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd791:            .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd791]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx790_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n251_statement_begin_β
.Lx790_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n254_lit_integer_α
n253_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n251_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n254_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx792_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n255_binop_α
n254_lit_integer_β:     add              rsp, 16
                        add              rsp, 32;                             jmp   n251_statement_begin_β
.Lx792_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n255_binop_α:           sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # call
                        cmp              eax, 3;                              jne   .Lx793_2
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, 1
                        add              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx793_7
.Lx793_2:               mov              edx, eax
                        and              edx, 1;                              jz    .Lx793_0
                        mov              rsi, qword ptr [rsp + 40]            # call
                        mov              rdi, 1
                        cmp              eax, 5;                              je    .Lx793_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx793_4
.Lx793_3:               movq             xmm0, rsi
.Lx793_4:               cvtsi2sd         xmm1, rdi
                        addsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx793_7:                                                                     jmp   n256_assign_α
.Lx793_0:               mov              rdi, qword ptr [rsp + 32]            # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx793_240
                        add              rsp, 16;                             jmp   n254_lit_integer_β
.Lx793_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n256_assign_α
n255_binop_β:           add              rsp, 16;                             jmp   n254_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n256_assign_α:          mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 272], rax            # index
                        mov              qword ptr [r9 + 280], rdx;           jmp   n257_statement_end_α
n256_assign_β:                                                                jmp   n251_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n257_statement_end_α:   add              rsp, 64;                             jmp   RETURN
n257_statement_end_β:   add              rsp, 64;                             jmp   RETURN
#=======================================================================================================================
# index_end  <stmt 46, line 92: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n258_statement_begin_α:                                                       jmp   n259_statement_end_α
n258_statement_begin_β:                                                       jmp   n260_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n259_statement_end_α:                                                         jmp   n260_statement_begin_α
n259_statement_end_β:                                                         jmp   n260_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n260_statement_begin_α:                                                       jmp   n261_statement_end_α
n260_statement_begin_β:                                                       jmp   n262_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n261_statement_end_α:                                                         jmp   n262_statement_begin_α
n261_statement_end_β:                                                         jmp   n262_statement_begin_α
#=======================================================================================================================
#         <stmt 48, line 93: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n262_statement_begin_α:                                                       jmp   n263_lit_integer_α
n262_statement_begin_β:                                                       jmp   n266_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n263_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx807_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n264_keyword_assign_snobol4_α
n263_lit_integer_β:     add              rsp, 16;                             jmp   n262_statement_begin_β
.Lx807_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n264_keyword_assign_snobol4_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx808_0]
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_kw_write_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              eax, 104;                            jne   .Lx808_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n262_statement_begin_β
.Lx808_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n265_statement_end_α
n264_keyword_assign_snobol4_β:
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n262_statement_begin_β
.Lx808_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n265_statement_end_α:                                                         jmp   n266_statement_begin_α
n265_statement_end_β:                                                         jmp   n266_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n266_statement_begin_α:                                                       jmp   n267_statement_end_α
n266_statement_begin_β: add              rsp, 32;                             jmp   n268_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n267_statement_end_α:   add              rsp, 32;                             jmp   n268_statement_begin_α
n267_statement_end_β:   add              rsp, 32;                             jmp   n268_statement_begin_α
#=======================================================================================================================
#         <stmt 50, line 95: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n268_statement_begin_α:                                                       jmp   n269_lit_string_α
n268_statement_begin_β:                                                       jmp   n275_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n269_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx817_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n270_lit_integer_α
n269_lit_string_β:      add              rsp, 16;                             jmp   n268_statement_begin_β
.Lx817_0:               .quad            .Lx817_0_s
.Lx817_0_s:             .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n270_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx818_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n271_lit_string_α
n270_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n268_statement_begin_β
.Lx818_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n271_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx819_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n272_call_α
n271_lit_string_β:      add              rsp, 16;                             jmp   n270_lit_integer_β
.Lx819_0:               .quad            .Lx819_0_s
.Lx819_0_s:             .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n272_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig821z]
                        lea              rax, [rip + pad_left_α];             jmp   rax
.Lsig821z:              .quad            3
                        .quad            .Lx821_2
                        .quad            .Lx821_2
                        .quad            48
                        .quad            32
                        .quad            16
.Lx821_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx821_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx821_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx821_240
                        add              rsp, 16;                             jmp   n271_lit_string_β
.Lx821_240:                                                                   jmp   n273_assign_α
n272_call_β:                                                                  jmp   n271_lit_string_β
.Lx821_0:               .quad            .Lx821_0_s
.Lx821_0_s:             .string          "pad_left"
#-----------------------------------------------------------------------------------------------------------------------
n273_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx822_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n274_statement_end_α
n273_assign_β:                                                                jmp   n268_statement_begin_β
.Lx822_0:               .quad            .Lx822_0_s
.Lx822_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n274_statement_end_α:   add              rsp, 64;                             jmp   n275_statement_begin_α
n274_statement_end_β:   add              rsp, 64;                             jmp   n275_statement_begin_α
#=======================================================================================================================
#         <stmt 51, line 96: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n275_statement_begin_α:                                                       jmp   n276_lit_string_α
n275_statement_begin_β:                                                       jmp   n282_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n276_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx827_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n277_lit_integer_α
n276_lit_string_β:      add              rsp, 16;                             jmp   n275_statement_begin_β
.Lx827_0:               .quad            .Lx827_0_s
.Lx827_0_s:             .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n277_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx828_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n278_lit_string_α
n277_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n275_statement_begin_β
.Lx828_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n278_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx829_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n279_call_α
n278_lit_string_β:      add              rsp, 16;                             jmp   n277_lit_integer_β
.Lx829_0:               .quad            .Lx829_0_s
.Lx829_0_s:             .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n279_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig831z]
                        lea              rax, [rip + pad_right_α];            jmp   rax
.Lsig831z:              .quad            3
                        .quad            .Lx831_2
                        .quad            .Lx831_2
                        .quad            48
                        .quad            32
                        .quad            16
.Lx831_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx831_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx831_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx831_240
                        add              rsp, 16;                             jmp   n278_lit_string_β
.Lx831_240:                                                                   jmp   n280_assign_α
n279_call_β:                                                                  jmp   n278_lit_string_β
.Lx831_0:               .quad            .Lx831_0_s
.Lx831_0_s:             .string          "pad_right"
#-----------------------------------------------------------------------------------------------------------------------
n280_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx832_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n281_statement_end_α
n280_assign_β:                                                                jmp   n275_statement_begin_β
.Lx832_0:               .quad            .Lx832_0_s
.Lx832_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n281_statement_end_α:   add              rsp, 64;                             jmp   n282_statement_begin_α
n281_statement_end_β:   add              rsp, 64;                             jmp   n282_statement_begin_α
#=======================================================================================================================
#         <stmt 52, line 97: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n282_statement_begin_α:                                                       jmp   n283_lit_string_α
n282_statement_begin_β:                                                       jmp   n287_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n283_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx837_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n284_call_α
n283_lit_string_β:      add              rsp, 16;                             jmp   n282_statement_begin_β
.Lx837_0:               .quad            .Lx837_0_s
.Lx837_0_s:             .string          "   hello"
#-----------------------------------------------------------------------------------------------------------------------
n284_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig839z]
                        lea              rax, [rip + ltrim_α];                jmp   rax
.Lsig839z:              .quad            1
                        .quad            .Lx839_2
                        .quad            .Lx839_2
                        .quad            16
.Lx839_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx839_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx839_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx839_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n282_statement_begin_β
.Lx839_240:                                                                   jmp   n285_assign_α
n284_call_β:                                                                  jmp   n282_statement_begin_β
.Lx839_0:               .quad            .Lx839_0_s
.Lx839_0_s:             .string          "ltrim"
#-----------------------------------------------------------------------------------------------------------------------
n285_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx840_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n286_statement_end_α
n285_assign_β:                                                                jmp   n282_statement_begin_β
.Lx840_0:               .quad            .Lx840_0_s
.Lx840_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n286_statement_end_α:   add              rsp, 32;                             jmp   n287_statement_begin_α
n286_statement_end_β:   add              rsp, 32;                             jmp   n287_statement_begin_α
#=======================================================================================================================
#         <stmt 53, line 98: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n287_statement_begin_α:                                                       jmp   n288_lit_string_α
n287_statement_begin_β:                                                       jmp   n292_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n288_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx845_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n289_call_α
n288_lit_string_β:      add              rsp, 16;                             jmp   n287_statement_begin_β
.Lx845_0:               .quad            .Lx845_0_s
.Lx845_0_s:             .string          "hello   "
#-----------------------------------------------------------------------------------------------------------------------
n289_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig847z]
                        lea              rax, [rip + rtrim_α];                jmp   rax
.Lsig847z:              .quad            1
                        .quad            .Lx847_2
                        .quad            .Lx847_2
                        .quad            16
.Lx847_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx847_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx847_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx847_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n287_statement_begin_β
.Lx847_240:                                                                   jmp   n290_assign_α
n289_call_β:                                                                  jmp   n287_statement_begin_β
.Lx847_0:               .quad            .Lx847_0_s
.Lx847_0_s:             .string          "rtrim"
#-----------------------------------------------------------------------------------------------------------------------
n290_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx848_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n291_statement_end_α
n290_assign_β:                                                                jmp   n287_statement_begin_β
.Lx848_0:               .quad            .Lx848_0_s
.Lx848_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n291_statement_end_α:   add              rsp, 32;                             jmp   n292_statement_begin_α
n291_statement_end_β:   add              rsp, 32;                             jmp   n292_statement_begin_α
#=======================================================================================================================
#         <stmt 54, line 99: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n292_statement_begin_α:                                                       jmp   n293_lit_string_α
n292_statement_begin_β:                                                       jmp   n297_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n293_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 9
                        mov              rax, qword ptr [rip + .Lx853_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n294_call_α
n293_lit_string_β:      add              rsp, 16;                             jmp   n292_statement_begin_β
.Lx853_0:               .quad            .Lx853_0_s
.Lx853_0_s:             .string          "  hello  "
#-----------------------------------------------------------------------------------------------------------------------
n294_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig855z]
                        lea              rax, [rip + trimws_α];               jmp   rax
.Lsig855z:              .quad            1
                        .quad            .Lx855_2
                        .quad            .Lx855_2
                        .quad            16
.Lx855_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx855_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx855_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx855_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n292_statement_begin_β
.Lx855_240:                                                                   jmp   n295_assign_α
n294_call_β:                                                                  jmp   n292_statement_begin_β
.Lx855_0:               .quad            .Lx855_0_s
.Lx855_0_s:             .string          "trimws"
#-----------------------------------------------------------------------------------------------------------------------
n295_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx856_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n296_statement_end_α
n295_assign_β:                                                                jmp   n292_statement_begin_β
.Lx856_0:               .quad            .Lx856_0_s
.Lx856_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n296_statement_end_α:   add              rsp, 32;                             jmp   n297_statement_begin_α
n296_statement_end_β:   add              rsp, 32;                             jmp   n297_statement_begin_α
#=======================================================================================================================
#         <stmt 55, line 100: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n297_statement_begin_α:                                                       jmp   n298_lit_string_α
n297_statement_begin_β:                                                       jmp   n303_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n298_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx861_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n299_lit_integer_α
n298_lit_string_β:      add              rsp, 16;                             jmp   n297_statement_begin_β
.Lx861_0:               .quad            .Lx861_0_s
.Lx861_0_s:             .string          "hi"
#-----------------------------------------------------------------------------------------------------------------------
n299_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx862_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n300_call_α
n299_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n297_statement_begin_β
.Lx862_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n300_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig864z]
                        lea              rax, [rip + repeat_α];               jmp   rax
.Lsig864z:              .quad            2
                        .quad            .Lx864_2
                        .quad            .Lx864_2
                        .quad            32
                        .quad            16
.Lx864_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx864_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx864_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx864_240
                        add              rsp, 16;                             jmp   n299_lit_integer_β
.Lx864_240:                                                                   jmp   n301_assign_α
n300_call_β:                                                                  jmp   n299_lit_integer_β
.Lx864_0:               .quad            .Lx864_0_s
.Lx864_0_s:             .string          "repeat"
#-----------------------------------------------------------------------------------------------------------------------
n301_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx865_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n302_statement_end_α
n301_assign_β:                                                                jmp   n297_statement_begin_β
.Lx865_0:               .quad            .Lx865_0_s
.Lx865_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n302_statement_end_α:                                                         jmp   n303_statement_begin_α
n302_statement_end_β:                                                         jmp   n303_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n303_statement_begin_α:                                                       jmp   n304_statement_end_α
n303_statement_begin_β: add              rsp, 48;                             jmp   n305_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n304_statement_end_α:   add              rsp, 48;                             jmp   n305_statement_begin_α
n304_statement_end_β:   add              rsp, 48;                             jmp   n305_statement_begin_α
#=======================================================================================================================
#         <stmt 57, line 102: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n305_statement_begin_α:                                                       jmp   n306_lit_string_α
n305_statement_begin_β:                                                       jmp   n314_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n306_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx874_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n307_lit_string_α
n306_lit_string_β:      add              rsp, 16;                             jmp   n305_statement_begin_β
.Lx874_0:               .quad            .Lx874_0_s
.Lx874_0_s:             .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n307_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx875_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n308_call_α
n307_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n305_statement_begin_β
.Lx875_0:               .quad            .Lx875_0_s
.Lx875_0_s:             .string          "oba"
#-----------------------------------------------------------------------------------------------------------------------
n308_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig877z]
                        lea              rax, [rip + contains_α];             jmp   rax
.Lsig877z:              .quad            2
                        .quad            .Lx877_2
                        .quad            .Lx877_2
                        .quad            32
                        .quad            16
.Lx877_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx877_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx877_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx877_240
                        add              rsp, 16;                             jmp   n307_lit_string_β
.Lx877_240:                                                                   jmp   n309_statement_end_α
n308_call_β:                                                                  jmp   n307_lit_string_β
.Lx877_0:               .quad            .Lx877_0_s
.Lx877_0_s:             .string          "contains"
#-----------------------------------------------------------------------------------------------------------------------
n309_statement_end_α:   add              rsp, 48;                             jmp   n310_statement_begin_α
n309_statement_end_β:   add              rsp, 48;                             jmp   n314_statement_begin_α
#=======================================================================================================================
#         <stmt 58, line 103: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n310_statement_begin_α:                                                       jmp   n311_lit_string_α
n310_statement_begin_β:                                                       jmp   n318_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n311_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx882_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n312_assign_α
n311_lit_string_β:      add              rsp, 16;                             jmp   n310_statement_begin_β
.Lx882_0:               .quad            .Lx882_0_s
.Lx882_0_s:             .string          "contains ok"
#-----------------------------------------------------------------------------------------------------------------------
n312_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx883_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n313_statement_end_α
n312_assign_β:                                                                jmp   n310_statement_begin_β
.Lx883_0:               .quad            .Lx883_0_s
.Lx883_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n313_statement_end_α:   add              rsp, 16;                             jmp   n318_statement_begin_α
n313_statement_end_β:   add              rsp, 16;                             jmp   n318_statement_begin_α
#=======================================================================================================================
# bad_c1  <stmt 59, line 104: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n314_statement_begin_α:                                                       jmp   n315_lit_string_α
n314_statement_begin_β:                                                       jmp   n318_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n315_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 14
                        mov              rax, qword ptr [rip + .Lx888_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n316_assign_α
n315_lit_string_β:      add              rsp, 16;                             jmp   n314_statement_begin_β
.Lx888_0:               .quad            .Lx888_0_s
.Lx888_0_s:             .string          "FAIL: contains"
#-----------------------------------------------------------------------------------------------------------------------
n316_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx889_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n317_statement_end_α
n316_assign_β:                                                                jmp   n314_statement_begin_β
.Lx889_0:               .quad            .Lx889_0_s
.Lx889_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n317_statement_end_α:   add              rsp, 16;                             jmp   n318_statement_begin_α
n317_statement_end_β:   add              rsp, 16;                             jmp   n318_statement_begin_α
#=======================================================================================================================
# c1  <stmt 60, line 106: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n318_statement_begin_α:                                                       jmp   n319_statement_end_α
n318_statement_begin_β:                                                       jmp   n320_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n319_statement_end_α:                                                         jmp   n320_statement_begin_α
n319_statement_end_β:                                                         jmp   n320_statement_begin_α
#=======================================================================================================================
#         <stmt 61, line 106: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n320_statement_begin_α:                                                       jmp   n321_lit_string_α
n320_statement_begin_β:                                                       jmp   n329_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n321_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx898_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n322_lit_string_α
n321_lit_string_β:      add              rsp, 16;                             jmp   n320_statement_begin_β
.Lx898_0:               .quad            .Lx898_0_s
.Lx898_0_s:             .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n322_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx899_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n323_call_α
n322_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n320_statement_begin_β
.Lx899_0:               .quad            .Lx899_0_s
.Lx899_0_s:             .string          "foo"
#-----------------------------------------------------------------------------------------------------------------------
n323_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig901z]
                        lea              rax, [rip + startswith_α];           jmp   rax
.Lsig901z:              .quad            2
                        .quad            .Lx901_2
                        .quad            .Lx901_2
                        .quad            32
                        .quad            16
.Lx901_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx901_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx901_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx901_240
                        add              rsp, 16;                             jmp   n322_lit_string_β
.Lx901_240:                                                                   jmp   n324_statement_end_α
n323_call_β:                                                                  jmp   n322_lit_string_β
.Lx901_0:               .quad            .Lx901_0_s
.Lx901_0_s:             .string          "startswith"
#-----------------------------------------------------------------------------------------------------------------------
n324_statement_end_α:   add              rsp, 48;                             jmp   n325_statement_begin_α
n324_statement_end_β:   add              rsp, 48;                             jmp   n329_statement_begin_α
#=======================================================================================================================
#         <stmt 62, line 107: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n325_statement_begin_α:                                                       jmp   n326_lit_string_α
n325_statement_begin_β:                                                       jmp   n333_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n326_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 13
                        mov              rax, qword ptr [rip + .Lx906_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n327_assign_α
n326_lit_string_β:      add              rsp, 16;                             jmp   n325_statement_begin_β
.Lx906_0:               .quad            .Lx906_0_s
.Lx906_0_s:             .string          "startswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n327_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx907_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n328_statement_end_α
n327_assign_β:                                                                jmp   n325_statement_begin_β
.Lx907_0:               .quad            .Lx907_0_s
.Lx907_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n328_statement_end_α:   add              rsp, 16;                             jmp   n333_statement_begin_α
n328_statement_end_β:   add              rsp, 16;                             jmp   n333_statement_begin_α
#=======================================================================================================================
# bad_sw1  <stmt 63, line 108: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n329_statement_begin_α:                                                       jmp   n330_lit_string_α
n329_statement_begin_β:                                                       jmp   n333_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n330_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 16
                        mov              rax, qword ptr [rip + .Lx912_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n331_assign_α
n330_lit_string_β:      add              rsp, 16;                             jmp   n329_statement_begin_β
.Lx912_0:               .quad            .Lx912_0_s
.Lx912_0_s:             .string          "FAIL: startswith"
#-----------------------------------------------------------------------------------------------------------------------
n331_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx913_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n332_statement_end_α
n331_assign_β:                                                                jmp   n329_statement_begin_β
.Lx913_0:               .quad            .Lx913_0_s
.Lx913_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n332_statement_end_α:   add              rsp, 16;                             jmp   n333_statement_begin_α
n332_statement_end_β:   add              rsp, 16;                             jmp   n333_statement_begin_α
#=======================================================================================================================
# sw1  <stmt 64, line 110: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n333_statement_begin_α:                                                       jmp   n334_statement_end_α
n333_statement_begin_β:                                                       jmp   n335_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n334_statement_end_α:                                                         jmp   n335_statement_begin_α
n334_statement_end_β:                                                         jmp   n335_statement_begin_α
#=======================================================================================================================
#         <stmt 65, line 110: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n335_statement_begin_α:                                                       jmp   n336_lit_string_α
n335_statement_begin_β:                                                       jmp   n344_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n336_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx922_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n337_lit_string_α
n336_lit_string_β:      add              rsp, 16;                             jmp   n335_statement_begin_β
.Lx922_0:               .quad            .Lx922_0_s
.Lx922_0_s:             .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n337_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx923_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n338_call_α
n337_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n335_statement_begin_β
.Lx923_0:               .quad            .Lx923_0_s
.Lx923_0_s:             .string          "bar"
#-----------------------------------------------------------------------------------------------------------------------
n338_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig925z]
                        lea              rax, [rip + endswith_α];             jmp   rax
.Lsig925z:              .quad            2
                        .quad            .Lx925_2
                        .quad            .Lx925_2
                        .quad            32
                        .quad            16
.Lx925_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx925_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx925_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx925_240
                        add              rsp, 16;                             jmp   n337_lit_string_β
.Lx925_240:                                                                   jmp   n339_statement_end_α
n338_call_β:                                                                  jmp   n337_lit_string_β
.Lx925_0:               .quad            .Lx925_0_s
.Lx925_0_s:             .string          "endswith"
#-----------------------------------------------------------------------------------------------------------------------
n339_statement_end_α:   add              rsp, 48;                             jmp   n340_statement_begin_α
n339_statement_end_β:   add              rsp, 48;                             jmp   n344_statement_begin_α
#=======================================================================================================================
#         <stmt 66, line 111: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n340_statement_begin_α:                                                       jmp   n341_lit_string_α
n340_statement_begin_β:                                                       jmp   n348_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n341_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx930_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n342_assign_α
n341_lit_string_β:      add              rsp, 16;                             jmp   n340_statement_begin_β
.Lx930_0:               .quad            .Lx930_0_s
.Lx930_0_s:             .string          "endswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n342_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx931_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n343_statement_end_α
n342_assign_β:                                                                jmp   n340_statement_begin_β
.Lx931_0:               .quad            .Lx931_0_s
.Lx931_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n343_statement_end_α:   add              rsp, 16;                             jmp   n348_statement_begin_α
n343_statement_end_β:   add              rsp, 16;                             jmp   n348_statement_begin_α
#=======================================================================================================================
# bad_ew1  <stmt 67, line 112: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n344_statement_begin_α:                                                       jmp   n345_lit_string_α
n344_statement_begin_β:                                                       jmp   n348_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n345_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 14
                        mov              rax, qword ptr [rip + .Lx936_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n346_assign_α
n345_lit_string_β:      add              rsp, 16;                             jmp   n344_statement_begin_β
.Lx936_0:               .quad            .Lx936_0_s
.Lx936_0_s:             .string          "FAIL: endswith"
#-----------------------------------------------------------------------------------------------------------------------
n346_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx937_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n347_statement_end_α
n346_assign_β:                                                                jmp   n344_statement_begin_β
.Lx937_0:               .quad            .Lx937_0_s
.Lx937_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n347_statement_end_α:   add              rsp, 16;                             jmp   n348_statement_begin_α
n347_statement_end_β:   add              rsp, 16;                             jmp   n348_statement_begin_α
#=======================================================================================================================
# ew1  <stmt 68, line 114: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n348_statement_begin_α:                                                       jmp   n349_statement_end_α
n348_statement_begin_β:                                                       jmp   n350_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n349_statement_end_α:                                                         jmp   n350_statement_begin_α
n349_statement_end_β:                                                         jmp   n350_statement_begin_α
#=======================================================================================================================
#         <stmt 69, line 114: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n350_statement_begin_α:                                                       jmp   n351_lit_string_α
n350_statement_begin_β:                                                       jmp   n355_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n351_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx946_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n352_lit_string_α
n351_lit_string_β:      add              rsp, 16;                             jmp   n350_statement_begin_β
.Lx946_0:               .quad            .Lx946_0_s
.Lx946_0_s:             .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n352_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx947_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n353_call_α
n352_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n350_statement_begin_β
.Lx947_0:               .quad            .Lx947_0_s
.Lx947_0_s:             .string          "bar"
#-----------------------------------------------------------------------------------------------------------------------
n353_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig949z]
                        lea              rax, [rip + startswith_α];           jmp   rax
.Lsig949z:              .quad            2
                        .quad            .Lx949_2
                        .quad            .Lx949_2
                        .quad            32
                        .quad            16
.Lx949_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx949_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx949_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx949_240
                        add              rsp, 16;                             jmp   n352_lit_string_β
.Lx949_240:                                                                   jmp   n354_statement_end_α
n353_call_β:                                                                  jmp   n352_lit_string_β
.Lx949_0:               .quad            .Lx949_0_s
.Lx949_0_s:             .string          "startswith"
#-----------------------------------------------------------------------------------------------------------------------
n354_statement_end_α:   add              rsp, 48;                             jmp   n359_statement_begin_α
n354_statement_end_β:   add              rsp, 48;                             jmp   n355_statement_begin_α
#=======================================================================================================================
#         <stmt 70, line 115: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n355_statement_begin_α:                                                       jmp   n356_lit_string_α
n355_statement_begin_β:                                                       jmp   n363_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n356_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 16
                        mov              rax, qword ptr [rip + .Lx954_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n357_assign_α
n356_lit_string_β:      add              rsp, 16;                             jmp   n355_statement_begin_β
.Lx954_0:               .quad            .Lx954_0_s
.Lx954_0_s:             .string          "no startswith ok"
#-----------------------------------------------------------------------------------------------------------------------
n357_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx955_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n358_statement_end_α
n357_assign_β:                                                                jmp   n355_statement_begin_β
.Lx955_0:               .quad            .Lx955_0_s
.Lx955_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n358_statement_end_α:   add              rsp, 16;                             jmp   n363_statement_begin_α
n358_statement_end_β:   add              rsp, 16;                             jmp   n363_statement_begin_α
#=======================================================================================================================
# bad_sw2  <stmt 71, line 116: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n359_statement_begin_α:                                                       jmp   n360_lit_string_α
n359_statement_begin_β:                                                       jmp   n363_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n360_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx960_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n361_assign_α
n360_lit_string_β:      add              rsp, 16;                             jmp   n359_statement_begin_β
.Lx960_0:               .quad            .Lx960_0_s
.Lx960_0_s:             .string          "FAIL: startswith matched wrong"
#-----------------------------------------------------------------------------------------------------------------------
n361_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx961_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n362_statement_end_α
n361_assign_β:                                                                jmp   n359_statement_begin_β
.Lx961_0:               .quad            .Lx961_0_s
.Lx961_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n362_statement_end_α:   add              rsp, 16;                             jmp   n363_statement_begin_α
n362_statement_end_β:   add              rsp, 16;                             jmp   n363_statement_begin_α
#=======================================================================================================================
# sw2  <stmt 72, line 118: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n363_statement_begin_α:                                                       jmp   n364_statement_end_α
n363_statement_begin_β:                                                       jmp   n365_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n364_statement_end_α:                                                         jmp   n365_statement_begin_α
n364_statement_end_β:                                                         jmp   n365_statement_begin_α
#=======================================================================================================================
#         <stmt 73, line 118: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n365_statement_begin_α:                                                       jmp   n366_lit_string_α
n365_statement_begin_β:                                                       jmp   n371_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n366_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx970_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n367_lit_string_α
n366_lit_string_β:      add              rsp, 16;                             jmp   n365_statement_begin_β
.Lx970_0:               .quad            .Lx970_0_s
.Lx970_0_s:             .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n367_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx971_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n368_call_α
n367_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n365_statement_begin_β
.Lx971_0:               .quad            .Lx971_0_s
.Lx971_0_s:             .string          "oba"
#-----------------------------------------------------------------------------------------------------------------------
n368_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig973z]
                        lea              rax, [rip + index_α];                jmp   rax
.Lsig973z:              .quad            2
                        .quad            .Lx973_2
                        .quad            .Lx973_2
                        .quad            32
                        .quad            16
.Lx973_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx973_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx973_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx973_240
                        add              rsp, 16;                             jmp   n367_lit_string_β
.Lx973_240:                                                                   jmp   n369_assign_α
n368_call_β:                                                                  jmp   n367_lit_string_β
.Lx973_0:               .quad            .Lx973_0_s
.Lx973_0_s:             .string          "index"
#-----------------------------------------------------------------------------------------------------------------------
n369_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx974_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n370_statement_end_α
n369_assign_β:                                                                jmp   n365_statement_begin_β
.Lx974_0:               .quad            .Lx974_0_s
.Lx974_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n370_statement_end_α:   add              rsp, 48;                             jmp   n371_statement_begin_α
n370_statement_end_β:   add              rsp, 48;                             jmp   n371_statement_begin_α
#=======================================================================================================================
#         <stmt 74, line 119: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n371_statement_begin_α:                                                       jmp   n372_lit_string_α
n371_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n372_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx979_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n373_lit_string_α
n372_lit_string_β:      add              rsp, 16;                             jmp   n371_statement_begin_β
.Lx979_0:               .quad            .Lx979_0_s
.Lx979_0_s:             .string          "foobar"
#-----------------------------------------------------------------------------------------------------------------------
n373_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx980_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n374_call_α
n373_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n371_statement_begin_β
.Lx980_0:               .quad            .Lx980_0_s
.Lx980_0_s:             .string          "xyz"
#-----------------------------------------------------------------------------------------------------------------------
n374_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig982z]
                        lea              rax, [rip + index_α];                jmp   rax
.Lsig982z:              .quad            2
                        .quad            .Lx982_2
                        .quad            .Lx982_2
                        .quad            32
                        .quad            16
.Lx982_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx982_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx982_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx982_240
                        add              rsp, 16;                             jmp   n373_lit_string_β
.Lx982_240:                                                                   jmp   n375_assign_α
n374_call_β:                                                                  jmp   n373_lit_string_β
.Lx982_0:               .quad            .Lx982_0_s
.Lx982_0_s:             .string          "index"
#-----------------------------------------------------------------------------------------------------------------------
n375_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx983_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n376_statement_end_α
n375_assign_β:                                                                jmp   n371_statement_begin_β
.Lx983_0:               .quad            .Lx983_0_s
.Lx983_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n376_statement_end_α:   add              rsp, 48;                             jmp   main_γ
n376_statement_end_β:   add              rsp, 48;                             jmp   main_γ
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
