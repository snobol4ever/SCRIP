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
n2_statement_begin_β:   mov              r11, 3;                              jmp   n58_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_define_α:            mov              r11, 4
                        mov              rdi, qword ptr [rip + .Lx151_0]
                        mov              rsi, qword ptr [rip + .Lx151_1]
                        mov              edx, 6
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
                        mov              rdi, qword ptr [rip + .Lx151_0]
                        lea              rsi, [rip + lfunc_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n4_statement_end_α
n3_define_β:            mov              r11, 4;                              jmp   n2_statement_begin_β
.Lx151_0:               .quad            .Lx151_0_s
.Lx151_0_s:             .string          "lfunc"
.Lx151_1:               .quad            .Lx151_1_s
.Lx151_1_s:             .string          "a,b,c,d,e,f"
                                                                              jmp   .Lx152_245
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
                        cmp              rdx, 0;                              jbe   .Lx152_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # a
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx152_41
.Lx152_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 96], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 104], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx152_41:              cmp              rdx, 1;                              jbe   .Lx152_11
                        mov              rdi, qword ptr [rcx + 32]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 32]             # b
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx152_42
.Lx152_11:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [rsp + 112], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 120], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx152_42:              cmp              rdx, 2;                              jbe   .Lx152_12
                        mov              rdi, qword ptr [rcx + 40]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 48]             # c
                        mov              qword ptr [r9 + 48], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 56]
                        mov              qword ptr [r9 + 56], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx152_43
.Lx152_12:              mov              rax, qword ptr [r9 + 48]
                        mov              qword ptr [rsp + 128], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 136], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
.Lx152_43:              lea              rcx, [rip + lfunc_γ]
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
                        cmp              rdx, 0;                              jbe   .Lx152_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # a
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx152_110
.Lx152_80:              mov              rax, qword ptr [rsp + 96]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [r9 + 24], rax
.Lx152_110:             cmp              rdx, 1;                              jbe   .Lx152_81
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 32], rax             # b
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx152_111
.Lx152_81:              mov              rax, qword ptr [rsp + 112]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [r9 + 40], rax
.Lx152_111:             cmp              rdx, 2;                              jbe   .Lx152_82
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 48], rax             # c
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx152_112
.Lx152_82:              mov              rax, qword ptr [rsp + 128]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [r9 + 56], rax
.Lx152_112:             mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx152_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # a
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx152_180
.Lx152_150:             mov              rax, qword ptr [rsp + 96]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [r9 + 24], rax
.Lx152_180:             cmp              rdx, 1;                              jbe   .Lx152_151
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 32], rax             # b
                        mov              rax, qword ptr [rcx + 32]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx152_181
.Lx152_151:             mov              rax, qword ptr [rsp + 112]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [r9 + 40], rax
.Lx152_181:             cmp              rdx, 2;                              jbe   .Lx152_152
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 48], rax             # c
                        mov              rax, qword ptr [rcx + 40]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 56], rax;            jmp   .Lx152_182
.Lx152_152:             mov              rax, qword ptr [rsp + 128]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [r9 + 56], rax
.Lx152_182:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 144
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx152_245:
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_end_α:     mov              r11, 5
                        mov              r10, 2;                              jmp   n58_statement_begin_α
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
                        mov              rax, qword ptr [rip + .Lx162_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n10_ident_α
n9_lit_string_β:        mov              r11, 10
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n7_statement_begin_β
.Lx162_0:               .quad            .Lx162_0_s
.Lx162_0_s:             .string          "p"
#-----------------------------------------------------------------------------------------------------------------------
n10_ident_α:            sub              rsp, 16
                        mov              r11, 11
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
                        test             eax, eax;                            jne   .Lx164_240
                        add              rsp, 16;                             jmp   n9_lit_string_β
.Lx164_240:                                                                   jmp   n11_statement_end_α
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
                        mov              rax, qword ptr [rip + .Lx169_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n14_assign_α
.Lx169_0:               .quad            .Lx169_0_s
.Lx169_0_s:             .string          "FAIL 1012/001: arg a should be p"
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_α:           mov              r11, 15
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx170_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n15_statement_end_α
.Lx170_0:               .quad            .Lx170_0_s
.Lx170_0_s:             .string          "OUTPUT"
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
                        mov              rax, qword ptr [rip + .Lx176_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n19_ident_α
n18_lit_string_β:       mov              r11, 19
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n16_statement_begin_β
.Lx176_0:               .quad            .Lx176_0_s
.Lx176_0_s:             .string          "q"
#-----------------------------------------------------------------------------------------------------------------------
n19_ident_α:            sub              rsp, 16
                        mov              r11, 20
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
                        test             eax, eax;                            jne   .Lx178_240
                        add              rsp, 16;                             jmp   n18_lit_string_β
.Lx178_240:                                                                   jmp   n20_statement_end_α
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
                        mov              rax, qword ptr [rip + .Lx183_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n23_assign_α
.Lx183_0:               .quad            .Lx183_0_s
.Lx183_0_s:             .string          "FAIL 1012/002: arg b should be q"
#-----------------------------------------------------------------------------------------------------------------------
n23_assign_α:           mov              r11, 24
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx184_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n24_statement_end_α
.Lx184_0:               .quad            .Lx184_0_s
.Lx184_0_s:             .string          "OUTPUT"
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
n27_statement_begin_β:  mov              r11, 28;                             jmp   n36_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n28_var_α:              sub              rsp, 16
                        mov              r11, 29
                        mov              rax, qword ptr [r9 + 64]             # d
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n29_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_string_α:       sub              rsp, 16
                        mov              r11, 30
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx194_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n30_differ_α
n29_lit_string_β:       mov              r11, 30
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n27_statement_begin_β
.Lx194_0:               .quad            .Lx194_0_s
.Lx194_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n30_differ_α:           sub              rsp, 16
                        mov              r11, 31
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
                        test             eax, eax;                            je    .Lx196_240
                        add              rsp, 16;                             jmp   n29_lit_string_β
.Lx196_240:                                                                   jmp   n31_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_end_α:    mov              r11, 32
                        mov              r10, 9
                        add              rsp, 48;                             jmp   n32_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/003: local d should be null' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_begin_α:  mov              r11, 33
                        mov              r10, 10;                             jmp   n33_lit_string_α
n32_statement_begin_β:  mov              r11, 33;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_string_α:       sub              rsp, 16
                        mov              r11, 34
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx201_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n34_assign_α
.Lx201_0:               .quad            .Lx201_0_s
.Lx201_0_s:             .string          "FAIL 1012/003: local d should be null"
#-----------------------------------------------------------------------------------------------------------------------
n34_assign_α:           mov              r11, 35
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx202_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n35_statement_end_α
.Lx202_0:               .quad            .Lx202_0_s
.Lx202_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_end_α:    mov              r11, 36
                        mov              r10, 10
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e003  <stmt 11, line 16: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_begin_α:  mov              r11, 37
                        mov              r10, 11;                             jmp   n37_statement_end_α
n36_statement_begin_β:  mov              r11, 37;                             jmp   n38_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_end_α:    mov              r11, 38
                        mov              r10, 11;                             jmp   n38_statement_begin_α
#=======================================================================================================================
#         a = 'aa' ; b = 'bb' ; d = 'dd'
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_begin_α:  mov              r11, 39
                        mov              r10, 12;                             jmp   n39_lit_string_α
n38_statement_begin_β:  mov              r11, 39;                             jmp   n42_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n39_lit_string_α:       sub              rsp, 16
                        mov              r11, 40
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx211_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n40_assign_α
.Lx211_0:               .quad            .Lx211_0_s
.Lx211_0_s:             .string          "aa"
#-----------------------------------------------------------------------------------------------------------------------
n40_assign_α:           mov              r11, 41
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax             # a
                        mov              qword ptr [r9 + 24], rdx;            jmp   n41_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_end_α:    mov              r11, 42
                        mov              r10, 12
                        add              rsp, 16;                             jmp   n42_statement_begin_α
#=======================================================================================================================
#         a = 'aa' ; b = 'bb' ; d = 'dd'
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_begin_α:  mov              r11, 43
                        mov              r10, 13;                             jmp   n43_lit_string_α
n42_statement_begin_β:  mov              r11, 43;                             jmp   n46_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_string_α:       sub              rsp, 16
                        mov              r11, 44
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx217_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n44_assign_α
.Lx217_0:               .quad            .Lx217_0_s
.Lx217_0_s:             .string          "bb"
#-----------------------------------------------------------------------------------------------------------------------
n44_assign_α:           mov              r11, 45
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax             # b
                        mov              qword ptr [r9 + 40], rdx;            jmp   n45_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_end_α:    mov              r11, 46
                        mov              r10, 13
                        add              rsp, 16;                             jmp   n46_statement_begin_α
#=======================================================================================================================
#         a = 'aa' ; b = 'bb' ; d = 'dd'
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_begin_α:  mov              r11, 47
                        mov              r10, 14;                             jmp   n47_lit_string_α
n46_statement_begin_β:  mov              r11, 47;                             jmp   n50_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_string_α:       sub              rsp, 16
                        mov              r11, 48
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx223_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n48_assign_α
.Lx223_0:               .quad            .Lx223_0_s
.Lx223_0_s:             .string          "dd"
#-----------------------------------------------------------------------------------------------------------------------
n48_assign_α:           mov              r11, 49
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax             # d
                        mov              qword ptr [r9 + 72], rdx;            jmp   n49_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_end_α:    mov              r11, 50
                        mov              r10, 14
                        add              rsp, 16;                             jmp   n50_statement_begin_α
#=======================================================================================================================
#         lfunc = a b d                                  :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_begin_α:  mov              r11, 51
                        mov              r10, 15;                             jmp   n51_var_α
n50_statement_begin_β:  mov              r11, 51;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n51_var_α:              sub              rsp, 16
                        mov              r11, 52
                        mov              rax, qword ptr [r9 + 16]             # a
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n52_var_α
#-----------------------------------------------------------------------------------------------------------------------
n52_var_α:              sub              rsp, 16
                        mov              r11, 53
                        mov              rax, qword ptr [r9 + 32]             # b
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n53_binop_α
n52_var_β:              mov              r11, 53
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n50_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n53_binop_α:            sub              rsp, 16
                        mov              r11, 54
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n54_var_α
n53_binop_β:            mov              r11, 54
                        add              rsp, 16;                             jmp   n52_var_β
#-----------------------------------------------------------------------------------------------------------------------
n54_var_α:              sub              rsp, 16
                        mov              r11, 55
                        mov              rax, qword ptr [r9 + 64]             # d
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n55_binop_α
n54_var_β:              mov              r11, 55
                        add              rsp, 16;                             jmp   n53_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n55_binop_α:            sub              rsp, 16
                        mov              r11, 56
                        mov              rdi, qword ptr [rsp + 32]            # binop
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n56_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n56_assign_α:           mov              r11, 57
                        mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # lfunc
                        mov              qword ptr [r9 + 8], rdx;             jmp   n57_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_end_α:    mov              r11, 58
                        mov              r10, 15
                        add              rsp, 80;                             jmp   RETURN
#=======================================================================================================================
# lfunc_end  <stmt 16, line 20: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n58_statement_begin_α:  mov              r11, 59
                        mov              r10, 16;                             jmp   n59_statement_end_α
n58_statement_begin_β:  mov              r11, 59;                             jmp   n60_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_end_α:    mov              r11, 60
                        mov              r10, 16;                             jmp   n60_statement_begin_α
#=======================================================================================================================
#         a = 'global_a'
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_begin_α:  mov              r11, 61
                        mov              r10, 17;                             jmp   n61_lit_string_α
n60_statement_begin_β:  mov              r11, 61;                             jmp   n64_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_string_α:       sub              rsp, 16
                        mov              r11, 62
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx243_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n62_assign_α
.Lx243_0:               .quad            .Lx243_0_s
.Lx243_0_s:             .string          "global_a"
#-----------------------------------------------------------------------------------------------------------------------
n62_assign_α:           mov              r11, 63
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax             # a
                        mov              qword ptr [r9 + 24], rdx;            jmp   n63_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n63_statement_end_α:    mov              r11, 64
                        mov              r10, 17
                        add              rsp, 16;                             jmp   n64_statement_begin_α
#=======================================================================================================================
#         d = 'global_d'
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_begin_α:  mov              r11, 65
                        mov              r10, 18;                             jmp   n65_lit_string_α
n64_statement_begin_β:  mov              r11, 65;                             jmp   n68_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n65_lit_string_α:       sub              rsp, 16
                        mov              r11, 66
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx249_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n66_assign_α
.Lx249_0:               .quad            .Lx249_0_s
.Lx249_0_s:             .string          "global_d"
#-----------------------------------------------------------------------------------------------------------------------
n66_assign_α:           mov              r11, 67
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax             # d
                        mov              qword ptr [r9 + 72], rdx;            jmp   n67_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_end_α:    mov              r11, 68
                        mov              r10, 18;                             jmp   n68_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_begin_α:  mov              r11, 69
                        mov              r10, 0;                              jmp   n69_statement_end_α
n68_statement_begin_β:  mov              r11, 69
                        add              rsp, 16;                             jmp   n70_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_end_α:    mov              r11, 70
                        mov              r10, 19
                        add              rsp, 16;                             jmp   n70_statement_begin_α
#=======================================================================================================================
#         DIFFER(lfunc('p', 'q', 'r'), 'aabbdd')                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_begin_α:  mov              r11, 71
                        mov              r10, 20;                             jmp   n71_lit_string_α
n70_statement_begin_β:  mov              r11, 71;                             jmp   n82_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_string_α:       sub              rsp, 16
                        mov              r11, 72
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx259_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n72_lit_string_α
.Lx259_0:               .quad            .Lx259_0_s
.Lx259_0_s:             .string          "p"
#-----------------------------------------------------------------------------------------------------------------------
n72_lit_string_α:       sub              rsp, 16
                        mov              r11, 73
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx260_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n73_lit_string_α
n72_lit_string_β:       mov              r11, 73
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n70_statement_begin_β
.Lx260_0:               .quad            .Lx260_0_s
.Lx260_0_s:             .string          "q"
#-----------------------------------------------------------------------------------------------------------------------
n73_lit_string_α:       sub              rsp, 16
                        mov              r11, 74
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx261_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n74_call_α
n73_lit_string_β:       mov              r11, 74
                        add              rsp, 16;                             jmp   n72_lit_string_β
.Lx261_0:               .quad            .Lx261_0_s
.Lx261_0_s:             .string          "r"
#-----------------------------------------------------------------------------------------------------------------------
n74_call_α:             sub              rsp, 16
                        mov              r11, 75
                        lea              rcx, [rip + .Lsig263z]
                        lea              rax, [rip + lfunc_α];                jmp   rax
.Lsig263z:              .quad            3
                        .quad            .Lx263_2
                        .quad            .Lx263_2
                        .quad            48
                        .quad            32
                        .quad            16
.Lx263_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx263_29
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
.Lx263_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx263_240
                        add              rsp, 16;                             jmp   n73_lit_string_β
.Lx263_240:                                                                   jmp   n75_lit_string_α
n74_call_β:             mov              r11, 75;                             jmp   n73_lit_string_β
.Lx263_0:               .quad            .Lx263_0_s
.Lx263_0_s:             .string          "lfunc"
#-----------------------------------------------------------------------------------------------------------------------
n75_lit_string_α:       sub              rsp, 16
                        mov              r11, 76
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx264_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n76_differ_α
n75_lit_string_β:       mov              r11, 76
                        add              rsp, 32;                             jmp   n73_lit_string_β
.Lx264_0:               .quad            .Lx264_0_s
.Lx264_0_s:             .string          "aabbdd"
#-----------------------------------------------------------------------------------------------------------------------
n76_differ_α:           sub              rsp, 16
                        mov              r11, 77
                        mov              rdi, qword ptr [rsp + 32]            # call
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
                        test             eax, eax;                            je    .Lx266_240
                        add              rsp, 16;                             jmp   n75_lit_string_β
.Lx266_240:                                                                   jmp   n77_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_end_α:    mov              r11, 78
                        mov              r10, 20
                        add              rsp, 96;                             jmp   n78_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/004: lfunc return value'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_begin_α:  mov              r11, 79
                        mov              r10, 21;                             jmp   n79_lit_string_α
n78_statement_begin_β:  mov              r11, 79;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_string_α:       sub              rsp, 16
                        mov              r11, 80
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx271_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n80_assign_α
.Lx271_0:               .quad            .Lx271_0_s
.Lx271_0_s:             .string          "FAIL 1012/004: lfunc return value"
#-----------------------------------------------------------------------------------------------------------------------
n80_assign_α:           mov              r11, 81
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx272_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n81_statement_end_α
.Lx272_0:               .quad            .Lx272_0_s
.Lx272_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_end_α:    mov              r11, 82
                        mov              r10, 21
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e004  <stmt 22, line 28: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n82_statement_begin_α:  mov              r11, 83
                        mov              r10, 22;                             jmp   n83_statement_end_α
n82_statement_begin_β:  mov              r11, 83;                             jmp   n84_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n83_statement_end_α:    mov              r11, 84
                        mov              r10, 22;                             jmp   n84_statement_begin_α
#=======================================================================================================================
#         IDENT(a, 'global_a')                           :s(e005)
#-----------------------------------------------------------------------------------------------------------------------
n84_statement_begin_α:  mov              r11, 85
                        mov              r10, 23;                             jmp   n85_var_α
n84_statement_begin_β:  mov              r11, 85;                             jmp   n89_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n85_var_α:              sub              rsp, 16
                        mov              r11, 86
                        mov              rax, qword ptr [r9 + 16]             # a
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n86_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_string_α:       sub              rsp, 16
                        mov              r11, 87
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx282_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n87_ident_α
n86_lit_string_β:       mov              r11, 87
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n84_statement_begin_β
.Lx282_0:               .quad            .Lx282_0_s
.Lx282_0_s:             .string          "global_a"
#-----------------------------------------------------------------------------------------------------------------------
n87_ident_α:            sub              rsp, 16
                        mov              r11, 88
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
                        test             eax, eax;                            jne   .Lx284_240
                        add              rsp, 16;                             jmp   n86_lit_string_β
.Lx284_240:                                                                   jmp   n88_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_end_α:    mov              r11, 89
                        mov              r10, 23
                        add              rsp, 48;                             jmp   n93_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/005: global a not clobbered' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_begin_α:  mov              r11, 90
                        mov              r10, 24;                             jmp   n90_lit_string_α
n89_statement_begin_β:  mov              r11, 90;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n90_lit_string_α:       sub              rsp, 16
                        mov              r11, 91
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx289_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n91_assign_α
.Lx289_0:               .quad            .Lx289_0_s
.Lx289_0_s:             .string          "FAIL 1012/005: global a not clobbered"
#-----------------------------------------------------------------------------------------------------------------------
n91_assign_α:           mov              r11, 92
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx290_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n92_statement_end_α
.Lx290_0:               .quad            .Lx290_0_s
.Lx290_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n92_statement_end_α:    mov              r11, 93
                        mov              r10, 24
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e005  <stmt 25, line 33: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n93_statement_begin_α:  mov              r11, 94
                        mov              r10, 25;                             jmp   n94_statement_end_α
n93_statement_begin_β:  mov              r11, 94;                             jmp   n95_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n94_statement_end_α:    mov              r11, 95
                        mov              r10, 25;                             jmp   n95_statement_begin_α
#=======================================================================================================================
#         IDENT(d, 'global_d')                           :s(e006)
#-----------------------------------------------------------------------------------------------------------------------
n95_statement_begin_α:  mov              r11, 96
                        mov              r10, 26;                             jmp   n96_var_α
n95_statement_begin_β:  mov              r11, 96;                             jmp   n100_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n96_var_α:              sub              rsp, 16
                        mov              r11, 97
                        mov              rax, qword ptr [r9 + 64]             # d
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n97_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n97_lit_string_α:       sub              rsp, 16
                        mov              r11, 98
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx300_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n98_ident_α
n97_lit_string_β:       mov              r11, 98
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n95_statement_begin_β
.Lx300_0:               .quad            .Lx300_0_s
.Lx300_0_s:             .string          "global_d"
#-----------------------------------------------------------------------------------------------------------------------
n98_ident_α:            sub              rsp, 16
                        mov              r11, 99
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
                        test             eax, eax;                            jne   .Lx302_240
                        add              rsp, 16;                             jmp   n97_lit_string_β
.Lx302_240:                                                                   jmp   n99_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n99_statement_end_α:    mov              r11, 100
                        mov              r10, 26
                        add              rsp, 48;                             jmp   n104_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/006: global d not clobbered' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n100_statement_begin_α: mov              r11, 101
                        mov              r10, 27;                             jmp   n101_lit_string_α
n100_statement_begin_β: mov              r11, 101;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n101_lit_string_α:      sub              rsp, 16
                        mov              r11, 102
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx307_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n102_assign_α
.Lx307_0:               .quad            .Lx307_0_s
.Lx307_0_s:             .string          "FAIL 1012/006: global d not clobbered"
#-----------------------------------------------------------------------------------------------------------------------
n102_assign_α:          mov              r11, 103
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx308_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n103_statement_end_α
.Lx308_0:               .quad            .Lx308_0_s
.Lx308_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_end_α:   mov              r11, 104
                        mov              r10, 27
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e006  <stmt 28, line 38: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_begin_α: mov              r11, 105
                        mov              r10, 28;                             jmp   n105_statement_end_α
n104_statement_begin_β: mov              r11, 105;                            jmp   n106_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n105_statement_end_α:   mov              r11, 106
                        mov              r10, 28;                             jmp   n106_statement_begin_α
#=======================================================================================================================
#         DEFINE('checklocal()x')                        :(cl_end)
#-----------------------------------------------------------------------------------------------------------------------
n106_statement_begin_α: mov              r11, 107
                        mov              r10, 29;                             jmp   n107_define_α
n106_statement_begin_β: mov              r11, 107;                            jmp   n126_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n107_define_α:          mov              r11, 108
                        mov              rdi, qword ptr [rip + .Lx318_0]
                        mov              rsi, qword ptr [rip + .Lx318_1]
                        mov              edx, 1
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n109_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx318_0]
                        lea              rsi, [rip + checklocal_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n108_statement_end_α
n107_define_β:          mov              r11, 108;                            jmp   n106_statement_begin_β
.Lx318_0:               .quad            .Lx318_0_s
.Lx318_0_s:             .string          "checklocal"
.Lx318_1:               .quad            .Lx318_1_s
.Lx318_1_s:             .string          "x"
                                                                              jmp   .Lx319_245
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
                        cmp              rdx, 0;                              jbe   .Lx319_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 128]            # x
                        mov              qword ptr [r9 + 128], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 136]
                        mov              qword ptr [r9 + 136], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx319_41
.Lx319_10:              mov              rax, qword ptr [r9 + 128]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
.Lx319_41:              lea              rcx, [rip + checklocal_γ]
                        lea              rax, [rip + checklocal_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n109_statement_begin_α]; jmp   rax
checklocal_γ:           mov              rdi, qword ptr [r9 + 112]            # checklocal
                        mov              rsi, qword ptr [r9 + 120]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rdx, 0;                              jbe   .Lx319_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 128], rax            # x
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx319_110
.Lx319_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
.Lx319_110:             mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx319_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 128], rax            # x
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx319_180
.Lx319_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
.Lx319_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx319_245:
#-----------------------------------------------------------------------------------------------------------------------
n108_statement_end_α:   mov              r11, 109
                        mov              r10, 29;                             jmp   n126_statement_begin_α
#=======================================================================================================================
# checklocal  <stmt 30, line 42: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n109_statement_begin_α: mov              r11, 110
                        mov              r10, 30;                             jmp   n110_statement_end_α
n109_statement_begin_β: mov              r11, 110;                            jmp   n111_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n110_statement_end_α:   mov              r11, 111
                        mov              r10, 30;                             jmp   n111_statement_begin_α
#=======================================================================================================================
#         DIFFER(x)                  :f(e007_inner)
#-----------------------------------------------------------------------------------------------------------------------
n111_statement_begin_α: mov              r11, 112
                        mov              r10, 31;                             jmp   n112_var_α
n111_statement_begin_β: mov              r11, 112;                            jmp   n120_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n112_var_α:             sub              rsp, 16
                        mov              r11, 113
                        mov              rax, qword ptr [r9 + 128]            # x
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n113_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n113_lit_string_α:      sub              rsp, 16
                        mov              r11, 114
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx329_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n114_differ_α
n113_lit_string_β:      mov              r11, 114
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n111_statement_begin_β
.Lx329_0:               .quad            .Lx329_0_s
.Lx329_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n114_differ_α:          sub              rsp, 16
                        mov              r11, 115
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
                        test             eax, eax;                            je    .Lx331_240
                        add              rsp, 16;                             jmp   n113_lit_string_β
.Lx331_240:                                                                   jmp   n115_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n115_statement_end_α:   mov              r11, 116
                        mov              r10, 31
                        add              rsp, 48;                             jmp   n116_statement_begin_α
#=======================================================================================================================
#         checklocal = 'local-not-null'                  :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n116_statement_begin_α: mov              r11, 117
                        mov              r10, 32;                             jmp   n117_lit_string_α
n116_statement_begin_β: mov              r11, 117;                            jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n117_lit_string_α:      sub              rsp, 16
                        mov              r11, 118
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 14
                        mov              rax, qword ptr [rip + .Lx336_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n118_assign_α
.Lx336_0:               .quad            .Lx336_0_s
.Lx336_0_s:             .string          "local-not-null"
#-----------------------------------------------------------------------------------------------------------------------
n118_assign_α:          mov              r11, 119
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # checklocal
                        mov              qword ptr [r9 + 120], rdx;           jmp   n119_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n119_statement_end_α:   mov              r11, 120
                        mov              r10, 32
                        add              rsp, 16;                             jmp   RETURN
#=======================================================================================================================
# e007_inner  <stmt 33, line 45: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n120_statement_begin_α: mov              r11, 121
                        mov              r10, 33;                             jmp   n121_statement_end_α
n120_statement_begin_β: mov              r11, 121;                            jmp   n122_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n121_statement_end_α:   mov              r11, 122
                        mov              r10, 33;                             jmp   n122_statement_begin_α
#=======================================================================================================================
#         checklocal =                                   :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n122_statement_begin_α: mov              r11, 123
                        mov              r10, 34;                             jmp   n123_lit_string_α
n122_statement_begin_β: mov              r11, 123;                            jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n123_lit_string_α:      sub              rsp, 16
                        mov              r11, 124
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx346_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n124_assign_α
.Lx346_0:               .quad            .Lx346_0_s
.Lx346_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n124_assign_α:          mov              r11, 125
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # checklocal
                        mov              qword ptr [r9 + 120], rdx;           jmp   n125_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n125_statement_end_α:   mov              r11, 126
                        mov              r10, 34
                        add              rsp, 16;                             jmp   RETURN
#=======================================================================================================================
# cl_end  <stmt 35, line 47: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n126_statement_begin_α: mov              r11, 127
                        mov              r10, 35;                             jmp   n127_statement_end_α
n126_statement_begin_β: mov              r11, 127;                            jmp   n128_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n127_statement_end_α:   mov              r11, 128
                        mov              r10, 35;                             jmp   n128_statement_begin_α
#=======================================================================================================================
#         DIFFER(checklocal())       :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n128_statement_begin_α: mov              r11, 129
                        mov              r10, 36;                             jmp   n129_call_α
n128_statement_begin_β: mov              r11, 129;                            jmp   n137_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n129_call_α:            sub              rsp, 16
                        mov              r11, 130
                        lea              rcx, [rip + .Lsig357z]
                        lea              rax, [rip + checklocal_α];           jmp   rax
.Lsig357z:              .quad            0
                        .quad            .Lx357_2
                        .quad            .Lx357_2
.Lx357_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx357_29
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
.Lx357_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx357_240
                        add              rsp, 16;                             jmp   n128_statement_begin_β
.Lx357_240:                                                                   jmp   n130_lit_string_α
n129_call_β:            mov              r11, 130;                            jmp   n128_statement_begin_β
.Lx357_0:               .quad            .Lx357_0_s
.Lx357_0_s:             .string          "checklocal"
#-----------------------------------------------------------------------------------------------------------------------
n130_lit_string_α:      sub              rsp, 16
                        mov              r11, 131
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx358_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n131_differ_α
n130_lit_string_β:      mov              r11, 131
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n128_statement_begin_β
.Lx358_0:               .quad            .Lx358_0_s
.Lx358_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n131_differ_α:          sub              rsp, 16
                        mov              r11, 132
                        mov              rdi, qword ptr [rsp + 32]            # call
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
                        test             eax, eax;                            je    .Lx360_240
                        add              rsp, 16;                             jmp   n130_lit_string_β
.Lx360_240:                                                                   jmp   n132_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n132_statement_end_α:   mov              r11, 133
                        mov              r10, 36
                        add              rsp, 48;                             jmp   n133_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1012/007: local null on fresh call' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n133_statement_begin_α: mov              r11, 134
                        mov              r10, 37;                             jmp   n134_lit_string_α
n133_statement_begin_β: mov              r11, 134;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n134_lit_string_α:      sub              rsp, 16
                        mov              r11, 135
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 39
                        mov              rax, qword ptr [rip + .Lx365_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n135_assign_α
.Lx365_0:               .quad            .Lx365_0_s
.Lx365_0_s:             .string          "FAIL 1012/007: local null on fresh call"
#-----------------------------------------------------------------------------------------------------------------------
n135_assign_α:          mov              r11, 136
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx366_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n136_statement_end_α
.Lx366_0:               .quad            .Lx366_0_s
.Lx366_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n136_statement_end_α:   mov              r11, 137
                        mov              r10, 37
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e007  <stmt 38, line 50: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n137_statement_begin_α: mov              r11, 138
                        mov              r10, 38;                             jmp   n138_statement_end_α
n137_statement_begin_β: mov              r11, 138;                            jmp   n139_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n138_statement_end_α:   mov              r11, 139
                        mov              r10, 38;                             jmp   n139_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1012_func_locals (7/7)'
#-----------------------------------------------------------------------------------------------------------------------
n139_statement_begin_α: mov              r11, 140
                        mov              r10, 39;                             jmp   n140_lit_string_α
n139_statement_begin_β: mov              r11, 140;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n140_lit_string_α:      sub              rsp, 16
                        mov              r11, 141
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 27
                        mov              rax, qword ptr [rip + .Lx375_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n141_assign_α
.Lx375_0:               .quad            .Lx375_0_s
.Lx375_0_s:             .string          "PASS 1012_func_locals (7/7)"
#-----------------------------------------------------------------------------------------------------------------------
n141_assign_α:          mov              r11, 142
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx376_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n142_statement_end_α
.Lx376_0:               .quad            .Lx376_0_s
.Lx376_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n142_statement_end_α:   mov              r11, 143
                        mov              r10, 39
                        add              rsp, 16;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 mov              r11, 144
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
