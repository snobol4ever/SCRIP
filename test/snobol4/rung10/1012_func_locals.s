                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             module_init
                        mov              edi, 9
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 9
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
                        .section         .rodata
.Lgvan0:                .string          "lfunc"
.Lgvan1:                .string          "a"
.Lgvan2:                .string          "b"
.Lgvan3:                .string          "c"
.Lgvan4:                .string          "d"
.Lgvan5:                .string          "e"
.Lgvan6:                .string          "f"
.Lgvan7:                .string          "checklocal"
.Lgvan8:                .string          "x"
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
                        .section         .text
                        .intel_syntax    noprefix
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:   mov              r11, 1
                        mov              r10, 0;                              jmp   n1_statement_end_α
n0_statement_begin_β:   mov              r11, 1;                              jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:     mov              r11, 2
                        mov              r10, 1;                              jmp   n2_statement_begin_α
#=======================================================================================================================
#         DEFINE('lfunc(a,b,c)d,e,f')               :(lfunc_end)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:   mov              r11, 3
                        mov              r10, 2;                              jmp   n3_define_α
n2_statement_begin_β:   mov              r11, 3;                              jmp   n57_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_define_α:            mov              r11, 4
                        mov              rdi, qword ptr [rip + .Lx148_0]
                        mov              rsi, qword ptr [rip + .Lx148_1]
                        mov              edx, 6
                        mov              ecx, 3
                        mov              r8d, 0
                        lea              r9, [rip + n5_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx148_0]
                        lea              rsi, [rip + lfunc_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n4_statement_end_α
n3_define_β:            mov              r11, 4;                              jmp   n2_statement_begin_β
.Lx148_0:               .quad            .Lx148_0_s
.Lx148_0_s:             .string          "lfunc"
.Lx148_1:               .quad            .Lx148_1_s
.Lx148_1_s:             .string          "a,b,c,d,e,f"
                                                                              jmp   .Lx149_245
#-----------------------------------------------------------------------------------------------------------------------
lfunc_α:                sub              rsp, 144
                        mov              rax, qword ptr [r9 + 64]             # d
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
                        mov              rax, qword ptr [r9 + 80]             # e
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
                        mov              rax, qword ptr [r9 + 96]             # f
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              rax, qword ptr [r9 + 0]              # lfunc
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 80], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 144]
                        cmp              rdx, 0;                              jbe   .Lx149_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # a
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx149_41
.Lx149_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 96], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 104], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx149_41:              cmp              rdx, 1;                              jbe   .Lx149_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 32]             # b
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx149_42
.Lx149_11:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [rsp + 112], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 120], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx149_42:              cmp              rdx, 2;                              jbe   .Lx149_12
                        mov              rdi, qword ptr [rcx + 40]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 48]             # c
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx149_43
.Lx149_12:              mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [rsp + 128], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 136], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx149_43:              lea              rcx, [rip + lfunc_γ]
                        lea              rax, [rip + lfunc_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n5_statement_begin_α];   jmp   rax
lfunc_γ:                mov              rdi, qword ptr [r9 + 0]              # lfunc
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 80]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 144]
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 8], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 96], rax             # f
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 80], rax             # e
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax             # d
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        cmp              rdx, 0;                              jbe   .Lx149_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # a
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx149_110
.Lx149_80:              mov              rax, qword ptr [rsp + 96]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [r9 + 24], rax
.Lx149_110:             cmp              rdx, 1;                              jbe   .Lx149_81
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 32], rax             # b
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx149_111
.Lx149_81:              mov              rax, qword ptr [rsp + 112]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [r9 + 40], rax
.Lx149_111:             cmp              rdx, 2;                              jbe   .Lx149_82
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 48], rax             # c
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx149_112
.Lx149_82:              mov              rax, qword ptr [rsp + 128]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [r9 + 56], rax
.Lx149_112:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 144
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
lfunc_ω:                mov              rcx, qword ptr [rsp + 80]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 144]
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 0], rax              # lfunc
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 8], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 96], rax             # f
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 104], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 80], rax             # e
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 64], rax             # d
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 72], rax
                        cmp              rdx, 0;                              jbe   .Lx149_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # a
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx149_180
.Lx149_150:             mov              rax, qword ptr [rsp + 96]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [r9 + 24], rax
.Lx149_180:             cmp              rdx, 1;                              jbe   .Lx149_151
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 32], rax             # b
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx149_181
.Lx149_151:             mov              rax, qword ptr [rsp + 112]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [r9 + 40], rax
.Lx149_181:             cmp              rdx, 2;                              jbe   .Lx149_152
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 48], rax             # c
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx149_182
.Lx149_152:             mov              rax, qword ptr [rsp + 128]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [r9 + 56], rax
.Lx149_182:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 144
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx149_245:
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_end_α:     mov              r11, 5
                        mov              r10, 2;                              jmp   n57_statement_begin_α
#=======================================================================================================================
# lfunc  <stmt 3, line 6: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_begin_α:   mov              r11, 6
                        mov              r10, 3;                              jmp   n6_statement_end_α
n5_statement_begin_β:   mov              r11, 6;                              jmp   n7_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:     mov              r11, 7
                        mov              r10, 3;                              jmp   n7_statement_begin_α
#=======================================================================================================================
#         IDENT(a, 'p')              :s(e001)
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_begin_α:   mov              r11, 8
                        mov              r10, 4;                              jmp   n8_var_α
n7_statement_begin_β:   mov              r11, 8;                              jmp   n12_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:               sub              rsp, 16
                        mov              r11, 9
                        mov              rax, qword ptr [r9 + 16]             # a
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n9_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_string_α:        sub              rsp, 16
                        mov              r11, 10
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx159_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n10_call_α
n9_lit_string_β:        mov              r11, 10
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n7_statement_begin_β
.Lx159_0:               .quad            .Lx159_0_s
.Lx159_0_s:             .string          "p"
#-----------------------------------------------------------------------------------------------------------------------
n10_call_α:             sub              rsp, 16
                        mov              r11, 11
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
.Lrkfnzd161:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd161]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx160_240
                        add              rsp, 16;                             jmp   n9_lit_string_β
.Lx160_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n11_statement_end_α
n10_call_β:             mov              r11, 11
                        add              rsp, 16;                             jmp   n9_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:    mov              r11, 12
                        mov              r10, 4
                        add              rsp, 48;                             jmp   n16_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/001: arg a should be p'    :(END)
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:  mov              r11, 13
                        mov              r10, 5;                              jmp   n13_lit_string_α
n12_statement_begin_β:  mov              r11, 13;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_string_α:       sub              rsp, 16
                        mov              r11, 14
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 32
                        mov              rax, qword ptr [rip + .Lx166_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n14_assign_α
.Lx166_0:               .quad            .Lx166_0_s
.Lx166_0_s:             .string          "FAIL 1012/001: arg a should be p"
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_α:           mov              r11, 15
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx167_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n15_statement_end_α
.Lx167_0:               .quad            .Lx167_0_s
.Lx167_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:    mov              r11, 16
                        mov              r10, 5
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e001    IDENT(b, 'q')              :s(e002)
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:  mov              r11, 17
                        mov              r10, 6;                              jmp   n17_var_α
n16_statement_begin_β:  mov              r11, 17;                             jmp   n21_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n17_var_α:              sub              rsp, 16
                        mov              r11, 18
                        mov              rax, qword ptr [r9 + 32]             # b
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n18_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_string_α:       sub              rsp, 16
                        mov              r11, 19
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx173_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n19_call_α
n18_lit_string_β:       mov              r11, 19
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n16_statement_begin_β
.Lx173_0:               .quad            .Lx173_0_s
.Lx173_0_s:             .string          "q"
#-----------------------------------------------------------------------------------------------------------------------
n19_call_α:             sub              rsp, 16
                        mov              r11, 20
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
.Lrkfnzd175:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd175]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx174_240
                        add              rsp, 16;                             jmp   n18_lit_string_β
.Lx174_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n20_statement_end_α
n19_call_β:             mov              r11, 20
                        add              rsp, 16;                             jmp   n18_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_end_α:    mov              r11, 21
                        mov              r10, 6
                        add              rsp, 48;                             jmp   n25_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/002: arg b should be q'    :(END)
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_begin_α:  mov              r11, 22
                        mov              r10, 7;                              jmp   n22_lit_string_α
n21_statement_begin_β:  mov              r11, 22;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_string_α:       sub              rsp, 16
                        mov              r11, 23
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 32
                        mov              rax, qword ptr [rip + .Lx180_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n23_assign_α
.Lx180_0:               .quad            .Lx180_0_s
.Lx180_0_s:             .string          "FAIL 1012/002: arg b should be q"
#-----------------------------------------------------------------------------------------------------------------------
n23_assign_α:           mov              r11, 24
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx181_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n24_statement_end_α
.Lx181_0:               .quad            .Lx181_0_s
.Lx181_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_end_α:    mov              r11, 25
                        mov              r10, 7
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e002  <stmt 8, line 12: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_begin_α:  mov              r11, 26
                        mov              r10, 8;                              jmp   n26_statement_end_α
n25_statement_begin_β:  mov              r11, 26;                             jmp   n27_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_end_α:    mov              r11, 27
                        mov              r10, 8;                              jmp   n27_statement_begin_α
#=======================================================================================================================
#         DIFFER(d)                  :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_begin_α:  mov              r11, 28
                        mov              r10, 9;                              jmp   n28_var_α
n27_statement_begin_β:  mov              r11, 28;                             jmp   n35_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n28_var_α:              sub              rsp, 16
                        mov              r11, 29
                        mov              rax, qword ptr [r9 + 64]             # d
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n29_call_α
#-----------------------------------------------------------------------------------------------------------------------
n29_call_α:             sub              rsp, 16
                        mov              r11, 30
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd192:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd192]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx191_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n27_statement_begin_β
.Lx191_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n30_statement_end_α
n29_call_β:             mov              r11, 30
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n27_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_end_α:    mov              r11, 31
                        mov              r10, 9
                        add              rsp, 32;                             jmp   n31_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/003: local d should be null' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_begin_α:  mov              r11, 32
                        mov              r10, 10;                             jmp   n32_lit_string_α
n31_statement_begin_β:  mov              r11, 32;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_string_α:       sub              rsp, 16
                        mov              r11, 33
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx197_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n33_assign_α
.Lx197_0:               .quad            .Lx197_0_s
.Lx197_0_s:             .string          "FAIL 1012/003: local d should be null"
#-----------------------------------------------------------------------------------------------------------------------
n33_assign_α:           mov              r11, 34
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx198_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n34_statement_end_α
.Lx198_0:               .quad            .Lx198_0_s
.Lx198_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:    mov              r11, 35
                        mov              r10, 10
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e003  <stmt 11, line 16: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:  mov              r11, 36
                        mov              r10, 11;                             jmp   n36_statement_end_α
n35_statement_begin_β:  mov              r11, 36;                             jmp   n37_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:    mov              r11, 37
                        mov              r10, 11;                             jmp   n37_statement_begin_α
#=======================================================================================================================
#         a = 'aa' ; b = 'bb' ; d = 'dd'
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_begin_α:  mov              r11, 38
                        mov              r10, 12;                             jmp   n38_lit_string_α
n37_statement_begin_β:  mov              r11, 38;                             jmp   n41_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_string_α:       sub              rsp, 16
                        mov              r11, 39
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx207_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n39_assign_α
.Lx207_0:               .quad            .Lx207_0_s
.Lx207_0_s:             .string          "aa"
#-----------------------------------------------------------------------------------------------------------------------
n39_assign_α:           mov              r11, 40
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax             # a
                        mov              qword ptr [r9 + 24], rdx;            jmp   n40_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_end_α:    mov              r11, 41
                        mov              r10, 12
                        add              rsp, 16;                             jmp   n41_statement_begin_α
#=======================================================================================================================
#         a = 'aa' ; b = 'bb' ; d = 'dd'
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_begin_α:  mov              r11, 42
                        mov              r10, 13;                             jmp   n42_lit_string_α
n41_statement_begin_β:  mov              r11, 42;                             jmp   n45_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_string_α:       sub              rsp, 16
                        mov              r11, 43
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx213_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n43_assign_α
.Lx213_0:               .quad            .Lx213_0_s
.Lx213_0_s:             .string          "bb"
#-----------------------------------------------------------------------------------------------------------------------
n43_assign_α:           mov              r11, 44
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax             # b
                        mov              qword ptr [r9 + 40], rdx;            jmp   n44_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_end_α:    mov              r11, 45
                        mov              r10, 13
                        add              rsp, 16;                             jmp   n45_statement_begin_α
#=======================================================================================================================
#         a = 'aa' ; b = 'bb' ; d = 'dd'
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_begin_α:  mov              r11, 46
                        mov              r10, 14;                             jmp   n46_lit_string_α
n45_statement_begin_β:  mov              r11, 46;                             jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_string_α:       sub              rsp, 16
                        mov              r11, 47
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx219_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n47_assign_α
.Lx219_0:               .quad            .Lx219_0_s
.Lx219_0_s:             .string          "dd"
#-----------------------------------------------------------------------------------------------------------------------
n47_assign_α:           mov              r11, 48
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax             # d
                        mov              qword ptr [r9 + 72], rdx;            jmp   n48_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_end_α:    mov              r11, 49
                        mov              r10, 14
                        add              rsp, 16;                             jmp   n49_statement_begin_α
#=======================================================================================================================
#         lfunc = a b d                                  :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_begin_α:  mov              r11, 50
                        mov              r10, 15;                             jmp   n50_var_α
n49_statement_begin_β:  mov              r11, 50;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n50_var_α:              sub              rsp, 16
                        mov              r11, 51
                        mov              rax, qword ptr [r9 + 16]             # a
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n51_var_α
#-----------------------------------------------------------------------------------------------------------------------
n51_var_α:              sub              rsp, 16
                        mov              r11, 52
                        mov              rax, qword ptr [r9 + 32]             # b
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n52_binop_α
n51_var_β:              mov              r11, 52
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n49_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n52_binop_α:            sub              rsp, 16
                        mov              r11, 53
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n53_var_α
n52_binop_β:            mov              r11, 53
                        add              rsp, 16;                             jmp   n51_var_β
#-----------------------------------------------------------------------------------------------------------------------
n53_var_α:              sub              rsp, 16
                        mov              r11, 54
                        mov              rax, qword ptr [r9 + 64]             # d
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n54_binop_α
n53_var_β:              mov              r11, 54
                        add              rsp, 16;                             jmp   n52_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n54_binop_α:            sub              rsp, 16
                        mov              r11, 55
                        mov              rdi, qword ptr [rsp + 32]            # binop
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n55_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n55_assign_α:           mov              r11, 56
                        mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # lfunc
                        mov              qword ptr [r9 + 8], rdx;             jmp   n56_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_end_α:    mov              r11, 57
                        mov              r10, 15
                        add              rsp, 80;                             jmp   RETURN
#=======================================================================================================================
# lfunc_end  <stmt 16, line 20: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_begin_α:  mov              r11, 58
                        mov              r10, 16;                             jmp   n58_statement_end_α
n57_statement_begin_β:  mov              r11, 58;                             jmp   n59_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n58_statement_end_α:    mov              r11, 59
                        mov              r10, 16;                             jmp   n59_statement_begin_α
#=======================================================================================================================
#         a = 'global_a'
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_begin_α:  mov              r11, 60
                        mov              r10, 17;                             jmp   n60_lit_string_α
n59_statement_begin_β:  mov              r11, 60;                             jmp   n63_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n60_lit_string_α:       sub              rsp, 16
                        mov              r11, 61
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx239_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n61_assign_α
.Lx239_0:               .quad            .Lx239_0_s
.Lx239_0_s:             .string          "global_a"
#-----------------------------------------------------------------------------------------------------------------------
n61_assign_α:           mov              r11, 62
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax             # a
                        mov              qword ptr [r9 + 24], rdx;            jmp   n62_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n62_statement_end_α:    mov              r11, 63
                        mov              r10, 17
                        add              rsp, 16;                             jmp   n63_statement_begin_α
#=======================================================================================================================
#         d = 'global_d'
#-----------------------------------------------------------------------------------------------------------------------
n63_statement_begin_α:  mov              r11, 64
                        mov              r10, 18;                             jmp   n64_lit_string_α
n63_statement_begin_β:  mov              r11, 64;                             jmp   n67_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_string_α:       sub              rsp, 16
                        mov              r11, 65
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx245_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n65_assign_α
.Lx245_0:               .quad            .Lx245_0_s
.Lx245_0_s:             .string          "global_d"
#-----------------------------------------------------------------------------------------------------------------------
n65_assign_α:           mov              r11, 66
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax             # d
                        mov              qword ptr [r9 + 72], rdx;            jmp   n66_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_end_α:    mov              r11, 67
                        mov              r10, 18;                             jmp   n67_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_begin_α:  mov              r11, 68
                        mov              r10, 0;                              jmp   n68_statement_end_α
n67_statement_begin_β:  mov              r11, 68
                        add              rsp, 16;                             jmp   n69_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_end_α:    mov              r11, 69
                        mov              r10, 19
                        add              rsp, 16;                             jmp   n69_statement_begin_α
#=======================================================================================================================
#         DIFFER(lfunc('p', 'q', 'r'), 'aabbdd')                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_begin_α:  mov              r11, 70
                        mov              r10, 20;                             jmp   n70_lit_string_α
n69_statement_begin_β:  mov              r11, 70;                             jmp   n81_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n70_lit_string_α:       sub              rsp, 16
                        mov              r11, 71
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx255_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n71_lit_string_α
.Lx255_0:               .quad            .Lx255_0_s
.Lx255_0_s:             .string          "p"
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_string_α:       sub              rsp, 16
                        mov              r11, 72
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx256_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n72_lit_string_α
n71_lit_string_β:       mov              r11, 72
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n69_statement_begin_β
.Lx256_0:               .quad            .Lx256_0_s
.Lx256_0_s:             .string          "q"
#-----------------------------------------------------------------------------------------------------------------------
n72_lit_string_α:       sub              rsp, 16
                        mov              r11, 73
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx257_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n73_call_α
n72_lit_string_β:       mov              r11, 73
                        add              rsp, 16;                             jmp   n71_lit_string_β
.Lx257_0:               .quad            .Lx257_0_s
.Lx257_0_s:             .string          "r"
#-----------------------------------------------------------------------------------------------------------------------
n73_call_α:             sub              rsp, 16
                        mov              r11, 74
                        lea              rcx, [rip + .Lsig259z]
                        lea              rax, [rip + lfunc_α];                jmp   rax
.Lsig259z:              .quad            3
                        .quad            .Lx259_2
                        .quad            .Lx259_2
                        .quad            48
                        .quad            32
                        .quad            16
.Lx259_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx259_29
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
.Lx259_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx259_240
                        add              rsp, 16;                             jmp   n72_lit_string_β
.Lx259_240:                                                                   jmp   n74_lit_string_α
n73_call_β:             mov              r11, 74;                             jmp   n72_lit_string_β
.Lx259_0:               .quad            .Lx259_0_s
.Lx259_0_s:             .string          "lfunc"
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_string_α:       sub              rsp, 16
                        mov              r11, 75
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx260_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n75_call_α
n74_lit_string_β:       mov              r11, 75
                        add              rsp, 32;                             jmp   n72_lit_string_β
.Lx260_0:               .quad            .Lx260_0_s
.Lx260_0_s:             .string          "aabbdd"
#-----------------------------------------------------------------------------------------------------------------------
n75_call_α:             sub              rsp, 16
                        mov              r11, 76
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
.Lrkfnzd262:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd262]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx261_240
                        add              rsp, 16;                             jmp   n74_lit_string_β
.Lx261_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n76_statement_end_α
n75_call_β:             mov              r11, 76
                        add              rsp, 16;                             jmp   n74_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n76_statement_end_α:    mov              r11, 77
                        mov              r10, 20
                        add              rsp, 96;                             jmp   n77_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/004: lfunc return value'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_begin_α:  mov              r11, 78
                        mov              r10, 21;                             jmp   n78_lit_string_α
n77_statement_begin_β:  mov              r11, 78;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n78_lit_string_α:       sub              rsp, 16
                        mov              r11, 79
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx267_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n79_assign_α
.Lx267_0:               .quad            .Lx267_0_s
.Lx267_0_s:             .string          "FAIL 1012/004: lfunc return value"
#-----------------------------------------------------------------------------------------------------------------------
n79_assign_α:           mov              r11, 80
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx268_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n80_statement_end_α
.Lx268_0:               .quad            .Lx268_0_s
.Lx268_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n80_statement_end_α:    mov              r11, 81
                        mov              r10, 21
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e004  <stmt 22, line 28: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_begin_α:  mov              r11, 82
                        mov              r10, 22;                             jmp   n82_statement_end_α
n81_statement_begin_β:  mov              r11, 82;                             jmp   n83_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n82_statement_end_α:    mov              r11, 83
                        mov              r10, 22;                             jmp   n83_statement_begin_α
#=======================================================================================================================
#         IDENT(a, 'global_a')                           :s(e005)
#-----------------------------------------------------------------------------------------------------------------------
n83_statement_begin_α:  mov              r11, 84
                        mov              r10, 23;                             jmp   n84_var_α
n83_statement_begin_β:  mov              r11, 84;                             jmp   n88_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n84_var_α:              sub              rsp, 16
                        mov              r11, 85
                        mov              rax, qword ptr [r9 + 16]             # a
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n85_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n85_lit_string_α:       sub              rsp, 16
                        mov              r11, 86
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx278_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n86_call_α
n85_lit_string_β:       mov              r11, 86
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n83_statement_begin_β
.Lx278_0:               .quad            .Lx278_0_s
.Lx278_0_s:             .string          "global_a"
#-----------------------------------------------------------------------------------------------------------------------
n86_call_α:             sub              rsp, 16
                        mov              r11, 87
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
.Lrkfnzd280:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd280]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx279_240
                        add              rsp, 16;                             jmp   n85_lit_string_β
.Lx279_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n87_statement_end_α
n86_call_β:             mov              r11, 87
                        add              rsp, 16;                             jmp   n85_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n87_statement_end_α:    mov              r11, 88
                        mov              r10, 23
                        add              rsp, 48;                             jmp   n92_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/005: global a not clobbered' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_begin_α:  mov              r11, 89
                        mov              r10, 24;                             jmp   n89_lit_string_α
n88_statement_begin_β:  mov              r11, 89;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n89_lit_string_α:       sub              rsp, 16
                        mov              r11, 90
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx285_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n90_assign_α
.Lx285_0:               .quad            .Lx285_0_s
.Lx285_0_s:             .string          "FAIL 1012/005: global a not clobbered"
#-----------------------------------------------------------------------------------------------------------------------
n90_assign_α:           mov              r11, 91
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx286_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n91_statement_end_α
.Lx286_0:               .quad            .Lx286_0_s
.Lx286_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_end_α:    mov              r11, 92
                        mov              r10, 24
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e005  <stmt 25, line 33: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n92_statement_begin_α:  mov              r11, 93
                        mov              r10, 25;                             jmp   n93_statement_end_α
n92_statement_begin_β:  mov              r11, 93;                             jmp   n94_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n93_statement_end_α:    mov              r11, 94
                        mov              r10, 25;                             jmp   n94_statement_begin_α
#=======================================================================================================================
#         IDENT(d, 'global_d')                           :s(e006)
#-----------------------------------------------------------------------------------------------------------------------
n94_statement_begin_α:  mov              r11, 95
                        mov              r10, 26;                             jmp   n95_var_α
n94_statement_begin_β:  mov              r11, 95;                             jmp   n99_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n95_var_α:              sub              rsp, 16
                        mov              r11, 96
                        mov              rax, qword ptr [r9 + 64]             # d
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n96_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n96_lit_string_α:       sub              rsp, 16
                        mov              r11, 97
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx296_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n97_call_α
n96_lit_string_β:       mov              r11, 97
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n94_statement_begin_β
.Lx296_0:               .quad            .Lx296_0_s
.Lx296_0_s:             .string          "global_d"
#-----------------------------------------------------------------------------------------------------------------------
n97_call_α:             sub              rsp, 16
                        mov              r11, 98
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
.Lrkfnzd298:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd298]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx297_240
                        add              rsp, 16;                             jmp   n96_lit_string_β
.Lx297_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n98_statement_end_α
n97_call_β:             mov              r11, 98
                        add              rsp, 16;                             jmp   n96_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n98_statement_end_α:    mov              r11, 99
                        mov              r10, 26
                        add              rsp, 48;                             jmp   n103_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/006: global d not clobbered' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n99_statement_begin_α:  mov              r11, 100
                        mov              r10, 27;                             jmp   n100_lit_string_α
n99_statement_begin_β:  mov              r11, 100;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n100_lit_string_α:      sub              rsp, 16
                        mov              r11, 101
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx303_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n101_assign_α
.Lx303_0:               .quad            .Lx303_0_s
.Lx303_0_s:             .string          "FAIL 1012/006: global d not clobbered"
#-----------------------------------------------------------------------------------------------------------------------
n101_assign_α:          mov              r11, 102
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx304_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n102_statement_end_α
.Lx304_0:               .quad            .Lx304_0_s
.Lx304_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:   mov              r11, 103
                        mov              r10, 27
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e006  <stmt 28, line 38: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_begin_α: mov              r11, 104
                        mov              r10, 28;                             jmp   n104_statement_end_α
n103_statement_begin_β: mov              r11, 104;                            jmp   n105_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_end_α:   mov              r11, 105
                        mov              r10, 28;                             jmp   n105_statement_begin_α
#=======================================================================================================================
#         DEFINE('checklocal()x')                        :(cl_end)
#-----------------------------------------------------------------------------------------------------------------------
n105_statement_begin_α: mov              r11, 106
                        mov              r10, 29;                             jmp   n106_define_α
n105_statement_begin_β: mov              r11, 106;                            jmp   n124_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n106_define_α:          mov              r11, 107
                        mov              rdi, qword ptr [rip + .Lx314_0]
                        mov              rsi, qword ptr [rip + .Lx314_1]
                        mov              edx, 1
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n108_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx314_0]
                        lea              rsi, [rip + checklocal_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n107_statement_end_α
n106_define_β:          mov              r11, 107;                            jmp   n105_statement_begin_β
.Lx314_0:               .quad            .Lx314_0_s
.Lx314_0_s:             .string          "checklocal"
.Lx314_1:               .quad            .Lx314_1_s
.Lx314_1_s:             .string          "x"
                                                                              jmp   .Lx315_245
#-----------------------------------------------------------------------------------------------------------------------
checklocal_α:           sub              rsp, 64
                        mov              rax, qword ptr [r9 + 112]            # checklocal
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 112], 0
                        mov              qword ptr [r9 + 120], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx315_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 128]            # x
                        mov              qword ptr [r9 + 128], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 136]
                        mov              qword ptr [r9 + 136], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx315_41
.Lx315_10:              mov              rax, qword ptr [r9 + 128]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
.Lx315_41:              lea              rcx, [rip + checklocal_γ]
                        lea              rax, [rip + checklocal_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n108_statement_begin_α]; jmp   rax
checklocal_γ:           mov              rdi, qword ptr [r9 + 112]            # checklocal
                        mov              rsi, qword ptr [r9 + 120]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rdx, 0;                              jbe   .Lx315_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 128], rax            # x
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx315_110
.Lx315_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
.Lx315_110:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 64
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
checklocal_ω:           mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax            # checklocal
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rdx, 0;                              jbe   .Lx315_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 128], rax            # x
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx315_180
.Lx315_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
.Lx315_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx315_245:
#-----------------------------------------------------------------------------------------------------------------------
n107_statement_end_α:   mov              r11, 108
                        mov              r10, 29;                             jmp   n124_statement_begin_α
#=======================================================================================================================
# checklocal  <stmt 30, line 42: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n108_statement_begin_α: mov              r11, 109
                        mov              r10, 30;                             jmp   n109_statement_end_α
n108_statement_begin_β: mov              r11, 109;                            jmp   n110_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n109_statement_end_α:   mov              r11, 110
                        mov              r10, 30;                             jmp   n110_statement_begin_α
#=======================================================================================================================
#         DIFFER(x)                  :f(e007_inner)
#-----------------------------------------------------------------------------------------------------------------------
n110_statement_begin_α: mov              r11, 111
                        mov              r10, 31;                             jmp   n111_var_α
n110_statement_begin_β: mov              r11, 111;                            jmp   n118_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n111_var_α:             sub              rsp, 16
                        mov              r11, 112
                        mov              rax, qword ptr [r9 + 128]            # x
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n112_call_α
#-----------------------------------------------------------------------------------------------------------------------
n112_call_α:            sub              rsp, 16
                        mov              r11, 113
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd326:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd326]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx325_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n110_statement_begin_β
.Lx325_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n113_statement_end_α
n112_call_β:            mov              r11, 113
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n110_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n113_statement_end_α:   mov              r11, 114
                        mov              r10, 31
                        add              rsp, 32;                             jmp   n114_statement_begin_α
#=======================================================================================================================
#         checklocal = 'local-not-null'                  :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n114_statement_begin_α: mov              r11, 115
                        mov              r10, 32;                             jmp   n115_lit_string_α
n114_statement_begin_β: mov              r11, 115;                            jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n115_lit_string_α:      sub              rsp, 16
                        mov              r11, 116
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 14
                        mov              rax, qword ptr [rip + .Lx331_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n116_assign_α
.Lx331_0:               .quad            .Lx331_0_s
.Lx331_0_s:             .string          "local-not-null"
#-----------------------------------------------------------------------------------------------------------------------
n116_assign_α:          mov              r11, 117
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # checklocal
                        mov              qword ptr [r9 + 120], rdx;           jmp   n117_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n117_statement_end_α:   mov              r11, 118
                        mov              r10, 32
                        add              rsp, 16;                             jmp   RETURN
#=======================================================================================================================
# e007_inner  <stmt 33, line 45: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n118_statement_begin_α: mov              r11, 119
                        mov              r10, 33;                             jmp   n119_statement_end_α
n118_statement_begin_β: mov              r11, 119;                            jmp   n120_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n119_statement_end_α:   mov              r11, 120
                        mov              r10, 33;                             jmp   n120_statement_begin_α
#=======================================================================================================================
#         checklocal =                                   :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n120_statement_begin_α: mov              r11, 121
                        mov              r10, 34;                             jmp   n121_lit_string_α
n120_statement_begin_β: mov              r11, 121;                            jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n121_lit_string_α:      sub              rsp, 16
                        mov              r11, 122
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx341_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n122_assign_α
.Lx341_0:               .quad            .Lx341_0_s
.Lx341_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n122_assign_α:          mov              r11, 123
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # checklocal
                        mov              qword ptr [r9 + 120], rdx;           jmp   n123_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n123_statement_end_α:   mov              r11, 124
                        mov              r10, 34
                        add              rsp, 16;                             jmp   RETURN
#=======================================================================================================================
# cl_end  <stmt 35, line 47: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n124_statement_begin_α: mov              r11, 125
                        mov              r10, 35;                             jmp   n125_statement_end_α
n124_statement_begin_β: mov              r11, 125;                            jmp   n126_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n125_statement_end_α:   mov              r11, 126
                        mov              r10, 35;                             jmp   n126_statement_begin_α
#=======================================================================================================================
#         DIFFER(checklocal())       :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n126_statement_begin_α: mov              r11, 127
                        mov              r10, 36;                             jmp   n127_call_α
n126_statement_begin_β: mov              r11, 127;                            jmp   n134_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n127_call_α:            sub              rsp, 16
                        mov              r11, 128
                        lea              rcx, [rip + .Lsig352z]
                        lea              rax, [rip + checklocal_α];           jmp   rax
.Lsig352z:              .quad            0
                        .quad            .Lx352_2
                        .quad            .Lx352_2
.Lx352_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx352_29
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
.Lx352_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx352_240
                        add              rsp, 16;                             jmp   n126_statement_begin_β
.Lx352_240:                                                                   jmp   n128_call_α
n127_call_β:            mov              r11, 128;                            jmp   n126_statement_begin_β
.Lx352_0:               .quad            .Lx352_0_s
.Lx352_0_s:             .string          "checklocal"
#-----------------------------------------------------------------------------------------------------------------------
n128_call_α:            sub              rsp, 16
                        mov              r11, 129
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd354:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd354]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx353_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n126_statement_begin_β
.Lx353_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n129_statement_end_α
n128_call_β:            mov              r11, 129
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n126_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n129_statement_end_α:   mov              r11, 130
                        mov              r10, 36
                        add              rsp, 32;                             jmp   n130_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/007: local null on fresh call' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n130_statement_begin_α: mov              r11, 131
                        mov              r10, 37;                             jmp   n131_lit_string_α
n130_statement_begin_β: mov              r11, 131;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n131_lit_string_α:      sub              rsp, 16
                        mov              r11, 132
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 39
                        mov              rax, qword ptr [rip + .Lx359_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n132_assign_α
.Lx359_0:               .quad            .Lx359_0_s
.Lx359_0_s:             .string          "FAIL 1012/007: local null on fresh call"
#-----------------------------------------------------------------------------------------------------------------------
n132_assign_α:          mov              r11, 133
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx360_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n133_statement_end_α
.Lx360_0:               .quad            .Lx360_0_s
.Lx360_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n133_statement_end_α:   mov              r11, 134
                        mov              r10, 37
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e007  <stmt 38, line 50: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n134_statement_begin_α: mov              r11, 135
                        mov              r10, 38;                             jmp   n135_statement_end_α
n134_statement_begin_β: mov              r11, 135;                            jmp   n136_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n135_statement_end_α:   mov              r11, 136
                        mov              r10, 38;                             jmp   n136_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1012_func_locals (7/7)'
#-----------------------------------------------------------------------------------------------------------------------
n136_statement_begin_α: mov              r11, 137
                        mov              r10, 39;                             jmp   n137_lit_string_α
n136_statement_begin_β: mov              r11, 137;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n137_lit_string_α:      sub              rsp, 16
                        mov              r11, 138
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 27
                        mov              rax, qword ptr [rip + .Lx369_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n138_assign_α
.Lx369_0:               .quad            .Lx369_0_s
.Lx369_0_s:             .string          "PASS 1012_func_locals (7/7)"
#-----------------------------------------------------------------------------------------------------------------------
n138_assign_α:          mov              r11, 139
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx370_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n139_statement_end_α
.Lx370_0:               .quad            .Lx370_0_s
.Lx370_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n139_statement_end_α:   mov              r11, 140
                        mov              r10, 39
                        add              rsp, 16;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 mov              r11, 141
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
.Lseala2:               .string          "lfunc"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            lfunc_α
                        lea              rdi, [rip + .Lseala2]
                        mov              rsi, qword ptr [rip + lfunc_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala3:               .string          "checklocal"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            checklocal_α
                        lea              rdi, [rip + .Lseala3]
                        mov              rsi, qword ptr [rip + checklocal_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        add              rsp, 8
                        ret
                        .section         .note.GNU-stack,"",@progbits
