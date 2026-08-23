                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
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
.Lgvan0:                .string          "N"
.Lgvan1:                .string          "S"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
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
#          &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:   mov              r11, 3
                        mov              r10, 2;                              jmp   n3_lit_integer_α
n2_statement_begin_β:   mov              r11, 3;                              jmp   n6_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:       sub              rsp, 16
                        mov              r11, 4
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx49_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_keyword_assign_snobol4_α
.Lx49_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n4_keyword_assign_snobol4_α:
                        sub              rsp, 16
                        mov              r11, 5
                        mov              rdi, qword ptr [rip + .Lx50_0]
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
                        cmp              al, 104;                             jne   .Lx50_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n2_statement_begin_β
.Lx50_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n5_statement_end_α
.Lx50_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_end_α:     mov              r11, 6
                        mov              r10, 2
                        add              rsp, 32;                             jmp   n6_statement_begin_α
#=======================================================================================================================
#          N        =  0
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_begin_α:   mov              r11, 7
                        mov              r10, 3;                              jmp   n7_lit_integer_α
n6_statement_begin_β:   mov              r11, 7;                              jmp   n10_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_integer_α:       sub              rsp, 16
                        mov              r11, 8
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx55_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n8_assign_α
.Lx55_0:                .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n8_assign_α:            mov              r11, 9
                        mov              rax, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # N
                        mov              qword ptr [r9 + 8], rdx;             jmp   n9_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_end_α:     mov              r11, 10
                        mov              r10, 3
                        add              rsp, 16;                             jmp   n10_statement_begin_α
#=======================================================================================================================
# LOOP     S        =  INPUT                                  :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_begin_α:  mov              r11, 11
                        mov              r10, 4;                              jmp   n11_var_α
n10_statement_begin_β:  mov              r11, 11;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:              sub              rsp, 16
                        mov              r11, 12
                        mov              rdi, qword ptr [rip + .Lx61_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_GET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx61_240
                        add              rsp, 16;                             jmp   n10_statement_begin_β
.Lx61_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n12_assign_α
.Lx61_0:                .quad            .Lx61_0_s
.Lx61_0_s:              .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n12_assign_α:           mov              r11, 13
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax             # S
                        mov              qword ptr [r9 + 24], rdx;            jmp   n13_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_end_α:    mov              r11, 14
                        mov              r10, 4
                        add              rsp, 16;                             jmp   n14_statement_begin_α
#=======================================================================================================================
#          OUTPUT   =  DUPL(' ', (80 - SIZE(S)) / 2) S
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_begin_α:  mov              r11, 15
                        mov              r10, 5;                              jmp   n15_lit_string_α
n14_statement_begin_β:  mov              r11, 15;                             jmp   n27_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_string_α:       sub              rsp, 16
                        mov              r11, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx67_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n16_lit_integer_α
.Lx67_0:                .quad            .Lx67_0_s
.Lx67_0_s:              .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:      sub              rsp, 16
                        mov              r11, 17
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx68_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n17_var_α
n16_lit_integer_β:      mov              r11, 17
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n14_statement_begin_β
.Lx68_0:                .quad            80
#-----------------------------------------------------------------------------------------------------------------------
n17_var_α:              sub              rsp, 16
                        mov              r11, 18
                        mov              rax, qword ptr [r9 + 16]             # S
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n18_call_α
n17_var_β:              mov              r11, 18
                        add              rsp, 16;                             jmp   n16_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n18_call_α:             sub              rsp, 16
                        mov              r11, 19
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd71:             .string          "SIZE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd71]
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
                        cmp              al, 104;                             jne   .Lx70_240
                        add              rsp, 16;                             jmp   n17_var_β
.Lx70_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n19_binop_α
n18_call_β:             mov              r11, 19
                        add              rsp, 16;                             jmp   n17_var_β
#-----------------------------------------------------------------------------------------------------------------------
n19_binop_α:            sub              rsp, 16
                        mov              r11, 20
                        mov              eax, dword ptr [rsp + 16]            # call
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              al, 3;                               jne   .Lx72_2
                        mov              rax, 80
                        sub              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx72_7
.Lx72_2:                mov              ecx, eax
                        mov              edx, eax
                        and              edx, 1;                              jz    .Lx72_0
                        mov              rsi, 80
                        mov              rdi, qword ptr [rsp + 24]            # call
                        cvtsi2sd         xmm0, rsi
                        cmp              cl, 5;                               je    .Lx72_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx72_6
.Lx72_5:                movq             xmm1, rdi
.Lx72_6:                subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx72_7:                                                                      jmp   n20_lit_integer_α
.Lx72_0:                mov              rdi, qword ptr [rsp + 48]            # lit_integer
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]            # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_sub@PLT
                        cmp              al, 104;                             jne   .Lx72_240
                        add              rsp, 32;                             jmp   n17_var_β
.Lx72_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n20_lit_integer_α
n19_binop_β:            mov              r11, 20
                        add              rsp, 32;                             jmp   n17_var_β
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_integer_α:      sub              rsp, 16
                        mov              r11, 21
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx73_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n21_binop_α
n20_lit_integer_β:      mov              r11, 21
                        add              rsp, 16;                             jmp   n19_binop_β
.Lx73_0:                .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n21_binop_α:            sub              rsp, 16
                        mov              r11, 22
                        mov              rdi, qword ptr [rsp + 32]            # binop
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_div@PLT
                        cmp              al, 104;                             jne   .Lx74_240
                        add              rsp, 16;                             jmp   n20_lit_integer_β
.Lx74_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n22_call_α
n21_binop_β:            mov              r11, 22
                        add              rsp, 16;                             jmp   n20_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n22_call_α:             sub              rsp, 16
                        mov              r11, 23
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 144]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 152]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lrkfnzd76:             .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd76]
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
                        cmp              al, 104;                             jne   .Lx75_240
                        add              rsp, 16;                             jmp   n21_binop_β
.Lx75_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n23_var_α
n22_call_β:             mov              r11, 23
                        add              rsp, 16;                             jmp   n21_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n23_var_α:              sub              rsp, 16
                        mov              r11, 24
                        mov              rax, qword ptr [r9 + 16]             # S
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n24_binop_α
n23_var_β:              mov              r11, 24
                        add              rsp, 32;                             jmp   n21_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n24_binop_α:            sub              rsp, 16
                        mov              r11, 25
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
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n25_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n25_assign_α:           mov              r11, 26
                        mov              rsi, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx79_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n26_statement_end_α
.Lx79_0:                .quad            .Lx79_0_s
.Lx79_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_end_α:    mov              r11, 27
                        mov              r10, 5
                        add              rsp, 160;                            jmp   n27_statement_begin_α
#=======================================================================================================================
#          N        =  REMDR(N + 1, 3)
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_begin_α:  mov              r11, 28
                        mov              r10, 6;                              jmp   n28_var_α
n27_statement_begin_β:  mov              r11, 28;                             jmp   n35_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n28_var_α:              sub              rsp, 16
                        mov              r11, 29
                        mov              rax, qword ptr [r9 + 0]              # N
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n29_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:      sub              rsp, 16
                        mov              r11, 30
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx85_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n30_binop_α
n29_lit_integer_β:      mov              r11, 30
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n27_statement_begin_β
.Lx85_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n30_binop_α:            sub              rsp, 16
                        mov              r11, 31
                        mov              ecx, dword ptr [rsp + 32]            # var
                        mov              rax, qword ptr [rsp + 40]
                        cmp              cl, 3;                               jne   .Lx86_2
                        add              rax, 1
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx86_7
.Lx86_2:                mov              eax, ecx
                        mov              edx, ecx
                        and              edx, 1;                              jz    .Lx86_0
                        mov              rsi, qword ptr [rsp + 40]            # var
                        mov              rdi, 1
                        cmp              al, 5;                               je    .Lx86_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx86_4
.Lx86_3:                movq             xmm0, rsi
.Lx86_4:                cvtsi2sd         xmm1, rdi
                        addsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx86_7:                                                                      jmp   n31_lit_integer_α
.Lx86_0:                mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              al, 104;                             jne   .Lx86_240
                        add              rsp, 16;                             jmp   n29_lit_integer_β
.Lx86_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n31_lit_integer_α
n30_binop_β:            mov              r11, 31
                        add              rsp, 16;                             jmp   n29_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_integer_α:      sub              rsp, 16
                        mov              r11, 32
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx87_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n32_call_α
n31_lit_integer_β:      mov              r11, 32
                        add              rsp, 16;                             jmp   n30_binop_β
.Lx87_0:                .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n32_call_α:             sub              rsp, 16
                        mov              r11, 33
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
.Lrkfnzd89:             .string          "REMDR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd89]
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
                        cmp              al, 104;                             jne   .Lx88_240
                        add              rsp, 16;                             jmp   n31_lit_integer_β
.Lx88_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n33_assign_α
n32_call_β:             mov              r11, 33
                        add              rsp, 16;                             jmp   n31_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n33_assign_α:           mov              r11, 34
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # N
                        mov              qword ptr [r9 + 8], rdx;             jmp   n34_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:    mov              r11, 35
                        mov              r10, 6
                        add              rsp, 80;                             jmp   n35_statement_begin_α
#=======================================================================================================================
#          OUTPUT   =  EQ(N, 0)                               :(LOOP)
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:  mov              r11, 36
                        mov              r10, 7;                              jmp   n36_var_α
n35_statement_begin_β:  mov              r11, 36;                             jmp   n10_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n36_var_α:              sub              rsp, 16
                        mov              r11, 37
                        mov              rax, qword ptr [r9 + 0]              # N
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n37_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_integer_α:      sub              rsp, 16
                        mov              r11, 38
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx96_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n38_coerce_numeric_α
n37_lit_integer_β:      mov              r11, 38
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n35_statement_begin_β
.Lx96_0:                .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n38_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 39
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              al, 5;                               je    .Lx98_1
                        cmp              al, 3;                               jne   .Lx98_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx98_0
.Lx98_1:                mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n39_coerce_numeric_α
.Lx98_0:                lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 101
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n39_coerce_numeric_α
n38_coerce_numeric_β:   mov              r11, 39
                        add              rsp, 16;                             jmp   n37_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n39_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 40
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx100_1
                        cmp              al, 3;                               jne   .Lx100_0
                        mov              eax, dword ptr [rsp + 48]            # var
                        cmp              al, 3;                               jne   .Lx100_0
.Lx100_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n40_cmp_test_α
.Lx100_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 102
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n40_cmp_test_α
n39_coerce_numeric_β:   mov              r11, 40
                        add              rsp, 16;                             jmp   n38_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n40_cmp_test_α:         sub              rsp, 16
                        mov              r11, 41
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx102_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            je    .Lx102_239
                        add              rsp, 16;                             jmp   n39_coerce_numeric_β
.Lx102_239:             mov              qword ptr [rsp + 0], 0               # result
                        mov              qword ptr [rsp + 8], 0;              jmp   n41_assign_α
.Lx102_0:               lea              rdi, [rsp + 32]                      # coerce_numeric
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_cmp_d@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            je    .Lx102_240
                        add              rsp, 16;                             jmp   n39_coerce_numeric_β
.Lx102_240:             mov              qword ptr [rsp + 0], 0               # result
                        mov              qword ptr [rsp + 8], 0;              jmp   n41_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n41_assign_α:           mov              r11, 42
                        mov              rsi, qword ptr [rsp + 0]             # cmp_test
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx103_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n42_statement_end_α
.Lx103_0:               .quad            .Lx103_0_s
.Lx103_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_end_α:    mov              r11, 43
                        mov              r10, 7
                        add              rsp, 80;                             jmp   n10_statement_begin_α
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
                        .section         .note.GNU-stack,"",@progbits
