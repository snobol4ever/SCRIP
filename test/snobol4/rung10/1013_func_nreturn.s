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
.Lgvan0:                .string          "ref_a"
.Lgvan1:                .string          "a"
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
#         DEFINE('ref_a()')                              :(ref_a_end)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:   mov              r11, 3
                        mov              r10, 2;                              jmp   n3_define_α
n2_statement_begin_β:   mov              r11, 3;                              jmp   n10_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_define_α:            mov              r11, 4
                        mov              rdi, qword ptr [rip + .Lx67_0]
                        mov              rsi, qword ptr [rip + .Lx67_1]
                        mov              edx, 0
                        mov              ecx, 0
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
                        mov              rdi, qword ptr [rip + .Lx67_0]
                        lea              rsi, [rip + ref_a_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n4_statement_end_α
n3_define_β:            mov              r11, 4;                              jmp   n2_statement_begin_β
.Lx67_0:                .quad            .Lx67_0_s
.Lx67_0_s:              .string          "ref_a"
.Lx67_1:                .quad            .Lx67_1_s
.Lx67_1_s:              .string          ""
                                                                              jmp   .Lx68_245
#-----------------------------------------------------------------------------------------------------------------------
ref_a_α:                sub              rsp, 48
                        mov              rax, qword ptr [r9 + 0]              # ref_a
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        lea              rcx, [rip + ref_a_γ]
                        lea              rax, [rip + ref_a_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n5_statement_begin_α];   jmp   rax
ref_a_γ:                mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
ref_a_ω:                mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx68_245:
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_end_α:     mov              r11, 5
                        mov              r10, 2;                              jmp   n10_statement_begin_α
#=======================================================================================================================
# ref_a   ref_a = .a                                     :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_begin_α:   mov              r11, 6
                        mov              r10, 3;                              jmp   n6_lit_string_α
n5_statement_begin_β:   mov              r11, 6;                              jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_string_α:        sub              rsp, 16
                        mov              r11, 7
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx73_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n7_call_α
.Lx73_0:                .quad            .Lx73_0_s
.Lx73_0_s:              .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
n7_call_α:              sub              rsp, 16
                        mov              r11, 8
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd75:             .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd75]
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
                        cmp              al, 104;                             jne   .Lx74_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n5_statement_begin_β
.Lx74_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n8_assign_α
n7_call_β:              mov              r11, 8
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n5_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n8_assign_α:            mov              r11, 9
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # ref_a
                        mov              qword ptr [r9 + 8], rdx;             jmp   n9_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_end_α:     mov              r11, 10
                        mov              r10, 3
                        add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# ref_a_end  <stmt 4, line 7: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_begin_α:  mov              r11, 11
                        mov              r10, 4;                              jmp   n11_statement_end_α
n10_statement_begin_β:  mov              r11, 11;                             jmp   n12_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:    mov              r11, 12
                        mov              r10, 4;                              jmp   n12_statement_begin_α
#=======================================================================================================================
#         a = 27
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:  mov              r11, 13
                        mov              r10, 5;                              jmp   n13_lit_integer_α
n12_statement_begin_β:  mov              r11, 13;                             jmp   n16_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:      sub              rsp, 16
                        mov              r11, 14
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx85_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n14_assign_α
.Lx85_0:                .quad            27
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_α:           mov              r11, 15
                        mov              rax, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax             # a
                        mov              qword ptr [r9 + 24], rdx;            jmp   n15_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:    mov              r11, 16
                        mov              r10, 5;                              jmp   n16_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:  mov              r11, 17
                        mov              r10, 0;                              jmp   n17_statement_end_α
n16_statement_begin_β:  mov              r11, 17
                        add              rsp, 16;                             jmp   n18_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_end_α:    mov              r11, 18
                        mov              r10, 6
                        add              rsp, 16;                             jmp   n18_statement_begin_α
#=======================================================================================================================
#         DIFFER(ref_a(), 27)                            :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_begin_α:  mov              r11, 19
                        mov              r10, 7;                              jmp   n19_call_α
n18_statement_begin_β:  mov              r11, 19;                             jmp   n27_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n19_call_α:             sub              rsp, 16
                        mov              r11, 20
                        lea              rcx, [rip + .Lsig96z]
                        lea              rax, [rip + ref_a_α];                jmp   rax
.Lsig96z:               .quad            0
                        .quad            .Lx96_2
                        .quad            .Lx96_2
.Lx96_2:                mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx96_29
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
.Lx96_29:               mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx96_240
                        add              rsp, 16;                             jmp   n18_statement_begin_β
.Lx96_240:                                                                    jmp   n20_lit_integer_α
n19_call_β:             mov              r11, 20;                             jmp   n18_statement_begin_β
.Lx96_0:                .quad            .Lx96_0_s
.Lx96_0_s:              .string          "ref_a"
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_integer_α:      sub              rsp, 16
                        mov              r11, 21
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx97_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n21_differ_α
n20_lit_integer_β:      mov              r11, 21
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n18_statement_begin_β
.Lx97_0:                .quad            27
#-----------------------------------------------------------------------------------------------------------------------
n21_differ_α:           sub              rsp, 16
                        mov              r11, 22
                        mov              rdi, qword ptr [rsp + 32]            # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            je    .Lx99_240
                        add              rsp, 16;                             jmp   n20_lit_integer_β
.Lx99_240:                                                                    jmp   n22_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_end_α:    mov              r11, 23
                        mov              r10, 7
                        add              rsp, 48;                             jmp   n23_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1013/001: nreturn read gives value' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_begin_α:  mov              r11, 24
                        mov              r10, 8;                              jmp   n24_lit_string_α
n23_statement_begin_β:  mov              r11, 24;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_string_α:       sub              rsp, 16
                        mov              r11, 25
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 39
                        mov              rax, qword ptr [rip + .Lx104_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n25_assign_α
.Lx104_0:               .quad            .Lx104_0_s
.Lx104_0_s:             .string          "FAIL 1013/001: nreturn read gives value"
#-----------------------------------------------------------------------------------------------------------------------
n25_assign_α:           mov              r11, 26
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx105_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n26_statement_end_α
.Lx105_0:               .quad            .Lx105_0_s
.Lx105_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_end_α:    mov              r11, 27
                        mov              r10, 8
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e001  <stmt 9, line 14: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_begin_α:  mov              r11, 28
                        mov              r10, 9;                              jmp   n28_statement_end_α
n27_statement_begin_β:  mov              r11, 28;                             jmp   n29_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:    mov              r11, 29
                        mov              r10, 9;                              jmp   n29_statement_begin_α
#=======================================================================================================================
#         ref_a() = 26                                   :s(e002)
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_begin_α:  mov              r11, 30
                        mov              r10, 10;                             jmp   n30_lit_string_α
n29_statement_begin_β:  mov              r11, 30;                             jmp   n36_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_string_α:       sub              rsp, 16
                        mov              r11, 31
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx114_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n31_call_α
.Lx114_0:               .quad            .Lx114_0_s
.Lx114_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n31_call_α:             sub              rsp, 16
                        mov              r11, 32
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd116:            .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd116]
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
                        cmp              al, 104;                             jne   .Lx115_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n29_statement_begin_β
.Lx115_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n32_call_α
n31_call_β:             mov              r11, 32
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n29_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n32_call_α:             sub              rsp, 16
                        mov              r11, 33
                        lea              rcx, [rip + .Lsig118z]
                        lea              rax, [rip + ref_a_α];                jmp   rax
.Lsig118z:              .quad            0
                        .quad            .Lx118_2
                        .quad            .Lx118_2
.Lx118_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx118_29
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
.Lx118_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx118_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n29_statement_begin_β
.Lx118_240:                                                                   jmp   n33_lit_integer_α
n32_call_β:             mov              r11, 33;                             jmp   n29_statement_begin_β
.Lx118_0:               .quad            .Lx118_0_s
.Lx118_0_s:             .string          "ref_a"
#-----------------------------------------------------------------------------------------------------------------------
n33_lit_integer_α:      sub              rsp, 16
                        mov              r11, 34
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx119_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n34_assign_var_α
n33_lit_integer_β:      mov              r11, 34
                        add              rsp, 16
                        add              rsp, 48;                             jmp   n29_statement_begin_β
.Lx119_0:               .quad            26
#-----------------------------------------------------------------------------------------------------------------------
n34_assign_var_α:       sub              rsp, 16
                        mov              r11, 35
                        mov              rdi, qword ptr [rsp + 32]            # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx120_240
                        add              rsp, 16;                             jmp   n33_lit_integer_β
.Lx120_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n35_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_end_α:    mov              r11, 36
                        mov              r10, 10
                        add              rsp, 80;                             jmp   n40_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1013/002: nreturn lvalue assign failed' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_begin_α:  mov              r11, 37
                        mov              r10, 11;                             jmp   n37_lit_string_α
n36_statement_begin_β:  mov              r11, 37;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_string_α:       sub              rsp, 16
                        mov              r11, 38
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 43
                        mov              rax, qword ptr [rip + .Lx125_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n38_assign_α
.Lx125_0:               .quad            .Lx125_0_s
.Lx125_0_s:             .string          "FAIL 1013/002: nreturn lvalue assign failed"
#-----------------------------------------------------------------------------------------------------------------------
n38_assign_α:           mov              r11, 39
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx126_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n39_statement_end_α
.Lx126_0:               .quad            .Lx126_0_s
.Lx126_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_end_α:    mov              r11, 40
                        mov              r10, 11
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e002  <stmt 12, line 19: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_begin_α:  mov              r11, 41
                        mov              r10, 12;                             jmp   n41_statement_end_α
n40_statement_begin_β:  mov              r11, 41;                             jmp   n42_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_end_α:    mov              r11, 42
                        mov              r10, 12;                             jmp   n42_statement_begin_α
#=======================================================================================================================
#         DIFFER(a, 26)                                  :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_begin_α:  mov              r11, 43
                        mov              r10, 13;                             jmp   n43_var_α
n42_statement_begin_β:  mov              r11, 43;                             jmp   n51_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n43_var_α:              sub              rsp, 16
                        mov              r11, 44
                        mov              rax, qword ptr [r9 + 16]             # a
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n44_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n44_lit_integer_α:      sub              rsp, 16
                        mov              r11, 45
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx136_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n45_differ_α
n44_lit_integer_β:      mov              r11, 45
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n42_statement_begin_β
.Lx136_0:               .quad            26
#-----------------------------------------------------------------------------------------------------------------------
n45_differ_α:           sub              rsp, 16
                        mov              r11, 46
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            je    .Lx138_240
                        add              rsp, 16;                             jmp   n44_lit_integer_β
.Lx138_240:                                                                   jmp   n46_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_end_α:    mov              r11, 47
                        mov              r10, 13
                        add              rsp, 48;                             jmp   n47_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1013/003: a updated via nreturn'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_begin_α:  mov              r11, 48
                        mov              r10, 14;                             jmp   n48_lit_string_α
n47_statement_begin_β:  mov              r11, 48;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_string_α:       sub              rsp, 16
                        mov              r11, 49
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx143_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n49_assign_α
.Lx143_0:               .quad            .Lx143_0_s
.Lx143_0_s:             .string          "FAIL 1013/003: a updated via nreturn"
#-----------------------------------------------------------------------------------------------------------------------
n49_assign_α:           mov              r11, 50
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx144_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n50_statement_end_α
.Lx144_0:               .quad            .Lx144_0_s
.Lx144_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_end_α:    mov              r11, 51
                        mov              r10, 14
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e003  <stmt 15, line 23: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_begin_α:  mov              r11, 52
                        mov              r10, 15;                             jmp   n52_statement_end_α
n51_statement_begin_β:  mov              r11, 52;                             jmp   n53_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_end_α:    mov              r11, 53
                        mov              r10, 15;                             jmp   n53_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1013_func_nreturn (3/3)'
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_begin_α:  mov              r11, 54
                        mov              r10, 16;                             jmp   n54_lit_string_α
n53_statement_begin_β:  mov              r11, 54;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_string_α:       sub              rsp, 16
                        mov              r11, 55
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 28
                        mov              rax, qword ptr [rip + .Lx153_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n55_assign_α
.Lx153_0:               .quad            .Lx153_0_s
.Lx153_0_s:             .string          "PASS 1013_func_nreturn (3/3)"
#-----------------------------------------------------------------------------------------------------------------------
n55_assign_α:           mov              r11, 56
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx154_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n56_statement_end_α
.Lx154_0:               .quad            .Lx154_0_s
.Lx154_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_end_α:    mov              r11, 57
                        mov              r10, 16
                        add              rsp, 16;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 mov              r11, 58
                        pop              rcx
                        add              rsp, 8;                              jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                mov              r11, 59
                        add              rsp, 8
                        pop              rcx;                                 jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
NRETURN:                mov              r11, 60
                        mov              rax, qword ptr [rip + rt_g_ret_by_name@GOTPCREL]
                        mov              dword ptr [rax + 0], 1;              jmp   RETURN
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
.Lseala1:               .string          "ref_a"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            ref_a_α
                        lea              rdi, [rip + .Lseala1]
                        mov              rsi, qword ptr [rip + ref_a_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        add              rsp, 8
                        ret
                        .section         .note.GNU-stack,"",@progbits
