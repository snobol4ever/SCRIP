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
.Lgvan0:                .string          "upcase"
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
#         DEFINE('upcase(s)')                                         :(upcase_end)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:   mov              r11, 1
                        mov              r10, 1;                              jmp   n1_define_α
n0_statement_begin_β:   mov              r11, 1;                              jmp   n10_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_define_α:            mov              r11, 2
                        mov              rdi, qword ptr [rip + .Lx26_0]
                        mov              rsi, qword ptr [rip + .Lx26_1]
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
                        mov              rdi, qword ptr [rip + .Lx26_0]
                        lea              rsi, [rip + upcase_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n2_statement_end_α
n1_define_β:            mov              r11, 2;                              jmp   n0_statement_begin_β
.Lx26_0:                .quad            .Lx26_0_s
.Lx26_0_s:              .string          "upcase"
.Lx26_1:                .quad            .Lx26_1_s
.Lx26_1_s:              .string          "s"
                                                                              jmp   .Lx27_245
#-----------------------------------------------------------------------------------------------------------------------
upcase_α:               sub              rsp, 64
                        mov              rax, qword ptr [r9 + 0]              # upcase
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx27_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # s
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx27_41
.Lx27_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx27_41:               lea              rcx, [rip + upcase_γ]
                        lea              rax, [rip + upcase_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n3_statement_begin_α];   jmp   rax
upcase_γ:               mov              rdi, qword ptr [r9 + 0]              # upcase
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rdx, 0;                              jbe   .Lx27_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx27_110
.Lx27_80:               mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx27_110:              mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 64
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
upcase_ω:               mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax              # upcase
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rdx, 0;                              jbe   .Lx27_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 16], rax             # s
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx27_180
.Lx27_150:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 24], rax
.Lx27_180:              mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx27_245:
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_end_α:     mov              r11, 3
                        mov              r10, 1;                              jmp   n10_statement_begin_α
#=======================================================================================================================
# upcase  upcase = REPLACE(s, &LCASE, &UCASE)                       :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n3_statement_begin_α:   mov              r11, 4
                        mov              r10, 2;                              jmp   n4_var_α
n3_statement_begin_β:   mov              r11, 4;                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n4_var_α:               sub              rsp, 16
                        mov              r11, 5
                        mov              rax, qword ptr [r9 + 16]             # s
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n5_keyword_snobol4_α
#-----------------------------------------------------------------------------------------------------------------------
n5_keyword_snobol4_α:   sub              rsp, 16
                        mov              r11, 6
                        mov              rdi, qword ptr [rip + .Lx33_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_kw_read_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n6_keyword_snobol4_α
.Lx33_0:                .quad            22
#-----------------------------------------------------------------------------------------------------------------------
n6_keyword_snobol4_α:   sub              rsp, 16
                        mov              r11, 7
                        mov              rdi, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_kw_read_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n7_call_α
.Lx34_0:                .quad            21
#-----------------------------------------------------------------------------------------------------------------------
n7_call_α:              sub              rsp, 16
                        mov              r11, 8
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
.Lrkfnzd36:             .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd36]
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
                        cmp              al, 104;                             jne   .Lx35_240
                        add              rsp, 16
                        add              rsp, 48;                             jmp   n3_statement_begin_β
.Lx35_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n8_assign_α
n7_call_β:              mov              r11, 8
                        add              rsp, 16
                        add              rsp, 48;                             jmp   n3_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n8_assign_α:            mov              r11, 9
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # upcase
                        mov              qword ptr [r9 + 8], rdx;             jmp   n9_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_end_α:     mov              r11, 10
                        mov              r10, 2
                        add              rsp, 64;                             jmp   RETURN
#=======================================================================================================================
# upcase_end  <stmt 3, line 5: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_begin_α:  mov              r11, 11
                        mov              r10, 3;                              jmp   n11_statement_end_α
n10_statement_begin_β:  mov              r11, 11;                             jmp   n12_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:    mov              r11, 12
                        mov              r10, 3;                              jmp   n12_statement_begin_α
#=======================================================================================================================
#         OUTPUT = upcase('hello')
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:  mov              r11, 13
                        mov              r10, 4;                              jmp   n13_lit_string_α
n12_statement_begin_β:  mov              r11, 13;                             jmp   n17_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_string_α:       sub              rsp, 16
                        mov              r11, 14
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx46_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n14_call_α
.Lx46_0:                .quad            .Lx46_0_s
.Lx46_0_s:              .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n14_call_α:             sub              rsp, 16
                        mov              r11, 15
                        lea              rcx, [rip + .Lsig48z]
                        lea              rax, [rip + upcase_α];               jmp   rax
.Lsig48z:               .quad            1
                        .quad            .Lx48_2
                        .quad            .Lx48_2
                        .quad            16
.Lx48_2:                mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx48_29
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
.Lx48_29:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx48_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n12_statement_begin_β
.Lx48_240:                                                                    jmp   n15_assign_α
n14_call_β:             mov              r11, 15;                             jmp   n12_statement_begin_β
.Lx48_0:                .quad            .Lx48_0_s
.Lx48_0_s:              .string          "upcase"
#-----------------------------------------------------------------------------------------------------------------------
n15_assign_α:           mov              r11, 16
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx49_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n16_statement_end_α
.Lx49_0:                .quad            .Lx49_0_s
.Lx49_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_end_α:    mov              r11, 17
                        mov              r10, 4
                        add              rsp, 32;                             jmp   n17_statement_begin_α
#=======================================================================================================================
#         OUTPUT = upcase('world')
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_begin_α:  mov              r11, 18
                        mov              r10, 5;                              jmp   n18_lit_string_α
n17_statement_begin_β:  mov              r11, 18;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_string_α:       sub              rsp, 16
                        mov              r11, 19
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx54_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n19_call_α
.Lx54_0:                .quad            .Lx54_0_s
.Lx54_0_s:              .string          "world"
#-----------------------------------------------------------------------------------------------------------------------
n19_call_α:             sub              rsp, 16
                        mov              r11, 20
                        lea              rcx, [rip + .Lsig56z]
                        lea              rax, [rip + upcase_α];               jmp   rax
.Lsig56z:               .quad            1
                        .quad            .Lx56_2
                        .quad            .Lx56_2
                        .quad            16
.Lx56_2:                mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx56_29
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
.Lx56_29:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx56_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n17_statement_begin_β
.Lx56_240:                                                                    jmp   n20_assign_α
n19_call_β:             mov              r11, 20;                             jmp   n17_statement_begin_β
.Lx56_0:                .quad            .Lx56_0_s
.Lx56_0_s:              .string          "upcase"
#-----------------------------------------------------------------------------------------------------------------------
n20_assign_α:           mov              r11, 21
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx57_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n21_statement_end_α
.Lx57_0:                .quad            .Lx57_0_s
.Lx57_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:    mov              r11, 22
                        mov              r10, 5
                        add              rsp, 32;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 mov              r11, 23
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
.Lseala1:               .string          "upcase"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            upcase_α
                        lea              rdi, [rip + .Lseala1]
                        mov              rsi, qword ptr [rip + upcase_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        add              rsp, 8
                        ret
                        .section         .note.GNU-stack,"",@progbits
