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
.Lgvan0:                .string          "a"
.Lgvan1:                .string          "b"
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
#         a = ARRAY(3)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:   mov              r11, 3
                        mov              r10, 2;                              jmp   n3_lit_integer_α
n2_statement_begin_β:   mov              r11, 3;                              jmp   n7_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:       sub              rsp, 16
                        mov              r11, 4
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx169_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_call_α
.Lx169_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:              sub              rsp, 16
                        mov              r11, 5
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd171:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd171]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx170_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n2_statement_begin_β
.Lx170_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n5_assign_α
n4_call_β:              mov              r11, 5
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n2_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_assign_α:            mov              r11, 6
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # a
                        mov              qword ptr [r9 + 8], rdx;             jmp   n6_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:     mov              r11, 7
                        mov              r10, 2;                              jmp   n7_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_begin_α:   mov              r11, 8
                        mov              r10, 0;                              jmp   n8_statement_end_α
n7_statement_begin_β:   mov              r11, 8
                        add              rsp, 32;                             jmp   n9_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_end_α:     mov              r11, 9
                        mov              r10, 3
                        add              rsp, 32;                             jmp   n9_statement_begin_α
#=======================================================================================================================
#         DIFFER(a<1>)               :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_begin_α:   mov              r11, 10
                        mov              r10, 4;                              jmp   n10_var_α
n9_statement_begin_β:   mov              r11, 10;                             jmp   n20_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:              sub              rsp, 16
                        mov              r11, 11
                        mov              rax, qword ptr [r9 + 0]              # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n11_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:      sub              rsp, 16
                        mov              r11, 12
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx182_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n12_subscript_α
n11_lit_integer_β:      mov              r11, 12
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n9_statement_begin_β
.Lx182_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n12_subscript_α:        sub              rsp, 16
                        mov              r11, 13
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx183_240
                        add              rsp, 16;                             jmp   n11_lit_integer_β
.Lx183_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n13_deref_α
n12_subscript_β:        mov              r11, 13
                        add              rsp, 16;                             jmp   n11_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n13_deref_α:            sub              rsp, 16
                        mov              r11, 14
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx184_240
                        add              rsp, 16;                             jmp   n12_subscript_β
.Lx184_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n14_call_α
n13_deref_β:            mov              r11, 14
                        add              rsp, 16;                             jmp   n12_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n14_call_α:             sub              rsp, 16
                        mov              r11, 15
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd186:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd186]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx185_240
                        add              rsp, 16;                             jmp   n13_deref_β
.Lx185_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n15_statement_end_α
n14_call_β:             mov              r11, 15
                        add              rsp, 16;                             jmp   n13_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:    mov              r11, 16
                        mov              r10, 4
                        add              rsp, 80;                             jmp   n16_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/001: array element init null' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:  mov              r11, 17
                        mov              r10, 5;                              jmp   n17_lit_string_α
n16_statement_begin_β:  mov              r11, 17;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_string_α:       sub              rsp, 16
                        mov              r11, 18
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 38
                        mov              rax, qword ptr [rip + .Lx191_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n18_assign_α
.Lx191_0:               .quad            .Lx191_0_s
.Lx191_0_s:             .string          "FAIL 1110/001: array element init null"
#-----------------------------------------------------------------------------------------------------------------------
n18_assign_α:           mov              r11, 19
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx192_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n19_statement_end_α
.Lx192_0:               .quad            .Lx192_0_s
.Lx192_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_end_α:    mov              r11, 20
                        mov              r10, 5
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e001  <stmt 6, line 10: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_begin_α:  mov              r11, 21
                        mov              r10, 6;                              jmp   n21_statement_end_α
n20_statement_begin_β:  mov              r11, 21;                             jmp   n22_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:    mov              r11, 22
                        mov              r10, 6;                              jmp   n22_statement_begin_α
#=======================================================================================================================
#         a<2> = 4.5
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:  mov              r11, 23
                        mov              r10, 7;                              jmp   n23_var_α
n22_statement_begin_β:  mov              r11, 23;                             jmp   n29_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n23_var_α:              sub              rsp, 16
                        mov              r11, 24
                        mov              rax, qword ptr [r9 + 0]              # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n24_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_integer_α:      sub              rsp, 16
                        mov              r11, 25
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx202_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n25_subscript_α
n24_lit_integer_β:      mov              r11, 25
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n22_statement_begin_β
.Lx202_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n25_subscript_α:        sub              rsp, 16
                        mov              r11, 26
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx203_240
                        add              rsp, 16;                             jmp   n24_lit_integer_β
.Lx203_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n26_lit_real_α
n25_subscript_β:        mov              r11, 26
                        add              rsp, 16;                             jmp   n24_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_real_α:         sub              rsp, 16
                        mov              r11, 27
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx204_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n27_assign_var_α
.Lx204_0:               .quad            4616752568008179712
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_var_α:       sub              rsp, 16
                        mov              r11, 28
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_real
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx205_240
                        add              rsp, 32;                             jmp   n25_subscript_β
.Lx205_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n28_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:    mov              r11, 29
                        mov              r10, 7
                        add              rsp, 80;                             jmp   n29_statement_begin_α
#=======================================================================================================================
#         DIFFER(a<2>, 4.5)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_begin_α:  mov              r11, 30
                        mov              r10, 8;                              jmp   n30_var_α
n29_statement_begin_β:  mov              r11, 30;                             jmp   n41_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n30_var_α:              sub              rsp, 16
                        mov              r11, 31
                        mov              rax, qword ptr [r9 + 0]              # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n31_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_integer_α:      sub              rsp, 16
                        mov              r11, 32
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx211_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n32_subscript_α
n31_lit_integer_β:      mov              r11, 32
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n29_statement_begin_β
.Lx211_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n32_subscript_α:        sub              rsp, 16
                        mov              r11, 33
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx212_240
                        add              rsp, 16;                             jmp   n31_lit_integer_β
.Lx212_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n33_deref_α
n32_subscript_β:        mov              r11, 33
                        add              rsp, 16;                             jmp   n31_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n33_deref_α:            sub              rsp, 16
                        mov              r11, 34
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx213_240
                        add              rsp, 16;                             jmp   n32_subscript_β
.Lx213_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n34_lit_real_α
n33_deref_β:            mov              r11, 34
                        add              rsp, 16;                             jmp   n32_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_real_α:         sub              rsp, 16
                        mov              r11, 35
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx214_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n35_call_α
.Lx214_0:               .quad            4616752568008179712
#-----------------------------------------------------------------------------------------------------------------------
n35_call_α:             sub              rsp, 16
                        mov              r11, 36
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
.Lrkfnzd216:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd216]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx215_240
                        add              rsp, 32;                             jmp   n33_deref_β
.Lx215_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n36_statement_end_α
n35_call_β:             mov              r11, 36
                        add              rsp, 32;                             jmp   n33_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:    mov              r11, 37
                        mov              r10, 8
                        add              rsp, 96;                             jmp   n37_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/002: array assign/read'    :(END)
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_begin_α:  mov              r11, 38
                        mov              r10, 9;                              jmp   n38_lit_string_α
n37_statement_begin_β:  mov              r11, 38;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_string_α:       sub              rsp, 16
                        mov              r11, 39
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 32
                        mov              rax, qword ptr [rip + .Lx221_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n39_assign_α
.Lx221_0:               .quad            .Lx221_0_s
.Lx221_0_s:             .string          "FAIL 1110/002: array assign/read"
#-----------------------------------------------------------------------------------------------------------------------
n39_assign_α:           mov              r11, 40
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx222_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n40_statement_end_α
.Lx222_0:               .quad            .Lx222_0_s
.Lx222_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_end_α:    mov              r11, 41
                        mov              r10, 9
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e002  <stmt 10, line 16: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_begin_α:  mov              r11, 42
                        mov              r10, 10;                             jmp   n42_statement_end_α
n41_statement_begin_β:  mov              r11, 42;                             jmp   n43_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_end_α:    mov              r11, 43
                        mov              r10, 10;                             jmp   n43_statement_begin_α
#=======================================================================================================================
#         a<4>                       :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_begin_α:  mov              r11, 44
                        mov              r10, 11;                             jmp   n44_var_α
n43_statement_begin_β:  mov              r11, 44;                             jmp   n53_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n44_var_α:              sub              rsp, 16
                        mov              r11, 45
                        mov              rax, qword ptr [r9 + 0]              # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n45_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_integer_α:      sub              rsp, 16
                        mov              r11, 46
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx232_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n46_subscript_α
n45_lit_integer_β:      mov              r11, 46
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n43_statement_begin_β
.Lx232_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n46_subscript_α:        sub              rsp, 16
                        mov              r11, 47
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx233_240
                        add              rsp, 16;                             jmp   n45_lit_integer_β
.Lx233_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n47_deref_α
n46_subscript_β:        mov              r11, 47
                        add              rsp, 16;                             jmp   n45_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n47_deref_α:            sub              rsp, 16
                        mov              r11, 48
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx234_240
                        add              rsp, 16;                             jmp   n46_subscript_β
.Lx234_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n48_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_end_α:    mov              r11, 49
                        mov              r10, 11
                        add              rsp, 64;                             jmp   n49_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/003: OOB high should fail' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_begin_α:  mov              r11, 50
                        mov              r10, 12;                             jmp   n50_lit_string_α
n49_statement_begin_β:  mov              r11, 50;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_string_α:       sub              rsp, 16
                        mov              r11, 51
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 35
                        mov              rax, qword ptr [rip + .Lx239_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n51_assign_α
.Lx239_0:               .quad            .Lx239_0_s
.Lx239_0_s:             .string          "FAIL 1110/003: OOB high should fail"
#-----------------------------------------------------------------------------------------------------------------------
n51_assign_α:           mov              r11, 52
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx240_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n52_statement_end_α
.Lx240_0:               .quad            .Lx240_0_s
.Lx240_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_end_α:    mov              r11, 53
                        mov              r10, 12
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e003  <stmt 13, line 21: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_begin_α:  mov              r11, 54
                        mov              r10, 13;                             jmp   n54_statement_end_α
n53_statement_begin_β:  mov              r11, 54;                             jmp   n55_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_end_α:    mov              r11, 55
                        mov              r10, 13;                             jmp   n55_statement_begin_α
#=======================================================================================================================
#         a<0>                       :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_begin_α:  mov              r11, 56
                        mov              r10, 14;                             jmp   n56_var_α
n55_statement_begin_β:  mov              r11, 56;                             jmp   n65_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n56_var_α:              sub              rsp, 16
                        mov              r11, 57
                        mov              rax, qword ptr [r9 + 0]              # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n57_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_integer_α:      sub              rsp, 16
                        mov              r11, 58
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx250_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n58_subscript_α
n57_lit_integer_β:      mov              r11, 58
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n55_statement_begin_β
.Lx250_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n58_subscript_α:        sub              rsp, 16
                        mov              r11, 59
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx251_240
                        add              rsp, 16;                             jmp   n57_lit_integer_β
.Lx251_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n59_deref_α
n58_subscript_β:        mov              r11, 59
                        add              rsp, 16;                             jmp   n57_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n59_deref_α:            sub              rsp, 16
                        mov              r11, 60
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx252_240
                        add              rsp, 16;                             jmp   n58_subscript_β
.Lx252_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n60_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_end_α:    mov              r11, 61
                        mov              r10, 14
                        add              rsp, 64;                             jmp   n61_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/004: OOB zero should fail' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_begin_α:  mov              r11, 62
                        mov              r10, 15;                             jmp   n62_lit_string_α
n61_statement_begin_β:  mov              r11, 62;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n62_lit_string_α:       sub              rsp, 16
                        mov              r11, 63
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 35
                        mov              rax, qword ptr [rip + .Lx257_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n63_assign_α
.Lx257_0:               .quad            .Lx257_0_s
.Lx257_0_s:             .string          "FAIL 1110/004: OOB zero should fail"
#-----------------------------------------------------------------------------------------------------------------------
n63_assign_α:           mov              r11, 64
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx258_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n64_statement_end_α
.Lx258_0:               .quad            .Lx258_0_s
.Lx258_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_end_α:    mov              r11, 65
                        mov              r10, 15
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e004  <stmt 16, line 26: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_begin_α:  mov              r11, 66
                        mov              r10, 16;                             jmp   n66_statement_end_α
n65_statement_begin_β:  mov              r11, 66;                             jmp   n67_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_end_α:    mov              r11, 67
                        mov              r10, 16;                             jmp   n67_statement_begin_α
#=======================================================================================================================
#         DIFFER(PROTOTYPE(a), 3)                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_begin_α:  mov              r11, 68
                        mov              r10, 17;                             jmp   n68_var_α
n67_statement_begin_β:  mov              r11, 68;                             jmp   n77_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n68_var_α:              sub              rsp, 16
                        mov              r11, 69
                        mov              rax, qword ptr [r9 + 0]              # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n69_call_α
#-----------------------------------------------------------------------------------------------------------------------
n69_call_α:             sub              rsp, 16
                        mov              r11, 70
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd269:            .string          "PROTOTYPE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd269]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx268_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n67_statement_begin_β
.Lx268_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n70_lit_integer_α
n69_call_β:             mov              r11, 70
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n67_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n70_lit_integer_α:      sub              rsp, 16
                        mov              r11, 71
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx270_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n71_call_α
n70_lit_integer_β:      mov              r11, 71
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n67_statement_begin_β
.Lx270_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n71_call_α:             sub              rsp, 16
                        mov              r11, 72
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
.Lrkfnzd272:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd272]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx271_240
                        add              rsp, 16;                             jmp   n70_lit_integer_β
.Lx271_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n72_statement_end_α
n71_call_β:             mov              r11, 72
                        add              rsp, 16;                             jmp   n70_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_end_α:    mov              r11, 73
                        mov              r10, 17
                        add              rsp, 64;                             jmp   n73_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/005: prototype(array(3)) is integer 3' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_begin_α:  mov              r11, 74
                        mov              r10, 18;                             jmp   n74_lit_string_α
n73_statement_begin_β:  mov              r11, 74;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_string_α:       sub              rsp, 16
                        mov              r11, 75
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 47
                        mov              rax, qword ptr [rip + .Lx277_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n75_assign_α
.Lx277_0:               .quad            .Lx277_0_s
.Lx277_0_s:             .string          "FAIL 1110/005: prototype(array(3)) is integer 3"
#-----------------------------------------------------------------------------------------------------------------------
n75_assign_α:           mov              r11, 76
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx278_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n76_statement_end_α
.Lx278_0:               .quad            .Lx278_0_s
.Lx278_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n76_statement_end_α:    mov              r11, 77
                        mov              r10, 18
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e005  <stmt 19, line 31: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_begin_α:  mov              r11, 78
                        mov              r10, 19;                             jmp   n78_statement_end_α
n77_statement_begin_β:  mov              r11, 78;                             jmp   n79_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_end_α:    mov              r11, 79
                        mov              r10, 19;                             jmp   n79_statement_begin_α
#=======================================================================================================================
#         b = ARRAY('3')
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_begin_α:  mov              r11, 80
                        mov              r10, 20;                             jmp   n80_lit_string_α
n79_statement_begin_β:  mov              r11, 80;                             jmp   n84_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n80_lit_string_α:       sub              rsp, 16
                        mov              r11, 81
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx287_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n81_call_α
.Lx287_0:               .quad            .Lx287_0_s
.Lx287_0_s:             .string          "3"
#-----------------------------------------------------------------------------------------------------------------------
n81_call_α:             sub              rsp, 16
                        mov              r11, 82
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd289:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd289]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx288_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n79_statement_begin_β
.Lx288_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n82_assign_α
n81_call_β:             mov              r11, 82
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n79_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n82_assign_α:           mov              r11, 83
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax             # b
                        mov              qword ptr [r9 + 24], rdx;            jmp   n83_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n83_statement_end_α:    mov              r11, 84
                        mov              r10, 20
                        add              rsp, 32;                             jmp   n84_statement_begin_α
#=======================================================================================================================
#         b<2> = 'x'
#-----------------------------------------------------------------------------------------------------------------------
n84_statement_begin_α:  mov              r11, 85
                        mov              r10, 21;                             jmp   n85_var_α
n84_statement_begin_β:  mov              r11, 85;                             jmp   n91_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n85_var_α:              sub              rsp, 16
                        mov              r11, 86
                        mov              rax, qword ptr [r9 + 16]             # b
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n86_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_integer_α:      sub              rsp, 16
                        mov              r11, 87
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx296_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n87_subscript_α
n86_lit_integer_β:      mov              r11, 87
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n84_statement_begin_β
.Lx296_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n87_subscript_α:        sub              rsp, 16
                        mov              r11, 88
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx297_240
                        add              rsp, 16;                             jmp   n86_lit_integer_β
.Lx297_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n88_lit_string_α
n87_subscript_β:        mov              r11, 88
                        add              rsp, 16;                             jmp   n86_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n88_lit_string_α:       sub              rsp, 16
                        mov              r11, 89
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx298_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n89_assign_var_α
n88_lit_string_β:       mov              r11, 89
                        add              rsp, 16;                             jmp   n87_subscript_β
.Lx298_0:               .quad            .Lx298_0_s
.Lx298_0_s:             .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n89_assign_var_α:       sub              rsp, 16
                        mov              r11, 90
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx299_240
                        add              rsp, 16;                             jmp   n88_lit_string_β
.Lx299_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n90_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_end_α:    mov              r11, 91
                        mov              r10, 21
                        add              rsp, 80;                             jmp   n91_statement_begin_α
#=======================================================================================================================
#         DIFFER(b<2>, 'x')                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_begin_α:  mov              r11, 92
                        mov              r10, 22;                             jmp   n92_var_α
n91_statement_begin_β:  mov              r11, 92;                             jmp   n103_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n92_var_α:              sub              rsp, 16
                        mov              r11, 93
                        mov              rax, qword ptr [r9 + 16]             # b
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n93_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n93_lit_integer_α:      sub              rsp, 16
                        mov              r11, 94
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx305_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n94_subscript_α
n93_lit_integer_β:      mov              r11, 94
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n91_statement_begin_β
.Lx305_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n94_subscript_α:        sub              rsp, 16
                        mov              r11, 95
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx306_240
                        add              rsp, 16;                             jmp   n93_lit_integer_β
.Lx306_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n95_deref_α
n94_subscript_β:        mov              r11, 95
                        add              rsp, 16;                             jmp   n93_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n95_deref_α:            sub              rsp, 16
                        mov              r11, 96
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx307_240
                        add              rsp, 16;                             jmp   n94_subscript_β
.Lx307_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n96_lit_string_α
n95_deref_β:            mov              r11, 96
                        add              rsp, 16;                             jmp   n94_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n96_lit_string_α:       sub              rsp, 16
                        mov              r11, 97
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx308_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n97_call_α
n96_lit_string_β:       mov              r11, 97
                        add              rsp, 16;                             jmp   n95_deref_β
.Lx308_0:               .quad            .Lx308_0_s
.Lx308_0_s:             .string          "x"
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
.Lrkfnzd310:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd310]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx309_240
                        add              rsp, 16;                             jmp   n96_lit_string_β
.Lx309_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n98_statement_end_α
n97_call_β:             mov              r11, 98
                        add              rsp, 16;                             jmp   n96_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n98_statement_end_α:    mov              r11, 99
                        mov              r10, 22
                        add              rsp, 96;                             jmp   n99_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/006: array from string dim' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n99_statement_begin_α:  mov              r11, 100
                        mov              r10, 23;                             jmp   n100_lit_string_α
n99_statement_begin_β:  mov              r11, 100;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n100_lit_string_α:      sub              rsp, 16
                        mov              r11, 101
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx315_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n101_assign_α
.Lx315_0:               .quad            .Lx315_0_s
.Lx315_0_s:             .string          "FAIL 1110/006: array from string dim"
#-----------------------------------------------------------------------------------------------------------------------
n101_assign_α:          mov              r11, 102
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx316_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n102_statement_end_α
.Lx316_0:               .quad            .Lx316_0_s
.Lx316_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:   mov              r11, 103
                        mov              r10, 23
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e006  <stmt 24, line 38: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_begin_α: mov              r11, 104
                        mov              r10, 24;                             jmp   n104_statement_end_α
n103_statement_begin_β: mov              r11, 104;                            jmp   n105_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_end_α:   mov              r11, 105
                        mov              r10, 24;                             jmp   n105_statement_begin_α
#=======================================================================================================================
#         DIFFER(PROTOTYPE(b), 3)                   :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n105_statement_begin_α: mov              r11, 106
                        mov              r10, 25;                             jmp   n106_var_α
n105_statement_begin_β: mov              r11, 106;                            jmp   n115_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n106_var_α:             sub              rsp, 16
                        mov              r11, 107
                        mov              rax, qword ptr [r9 + 16]             # b
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n107_call_α
#-----------------------------------------------------------------------------------------------------------------------
n107_call_α:            sub              rsp, 16
                        mov              r11, 108
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd327:            .string          "PROTOTYPE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd327]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx326_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n105_statement_begin_β
.Lx326_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n108_lit_integer_α
n107_call_β:            mov              r11, 108
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n105_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n108_lit_integer_α:     sub              rsp, 16
                        mov              r11, 109
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx328_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n109_call_α
n108_lit_integer_β:     mov              r11, 109
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n105_statement_begin_β
.Lx328_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n109_call_α:            sub              rsp, 16
                        mov              r11, 110
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
.Lrkfnzd330:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd330]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx329_240
                        add              rsp, 16;                             jmp   n108_lit_integer_β
.Lx329_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n110_statement_end_α
n109_call_β:            mov              r11, 110
                        add              rsp, 16;                             jmp   n108_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n110_statement_end_α:   mov              r11, 111
                        mov              r10, 25
                        add              rsp, 64;                             jmp   n111_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/007: prototype string-dim is integer' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n111_statement_begin_α: mov              r11, 112
                        mov              r10, 26;                             jmp   n112_lit_string_α
n111_statement_begin_β: mov              r11, 112;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n112_lit_string_α:      sub              rsp, 16
                        mov              r11, 113
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 46
                        mov              rax, qword ptr [rip + .Lx335_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n113_assign_α
.Lx335_0:               .quad            .Lx335_0_s
.Lx335_0_s:             .string          "FAIL 1110/007: prototype string-dim is integer"
#-----------------------------------------------------------------------------------------------------------------------
n113_assign_α:          mov              r11, 114
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx336_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n114_statement_end_α
.Lx336_0:               .quad            .Lx336_0_s
.Lx336_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n114_statement_end_α:   mov              r11, 115
                        mov              r10, 26
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e007  <stmt 27, line 43: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n115_statement_begin_α: mov              r11, 116
                        mov              r10, 27;                             jmp   n116_statement_end_α
n115_statement_begin_β: mov              r11, 116;                            jmp   n117_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n116_statement_end_α:   mov              r11, 117
                        mov              r10, 27;                             jmp   n117_statement_begin_α
#=======================================================================================================================
#         a<1> = 3.14
#-----------------------------------------------------------------------------------------------------------------------
n117_statement_begin_α: mov              r11, 118
                        mov              r10, 28;                             jmp   n118_var_α
n117_statement_begin_β: mov              r11, 118;                            jmp   n124_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n118_var_α:             sub              rsp, 16
                        mov              r11, 119
                        mov              rax, qword ptr [r9 + 0]              # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n119_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n119_lit_integer_α:     sub              rsp, 16
                        mov              r11, 120
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx346_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n120_subscript_α
n119_lit_integer_β:     mov              r11, 120
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n117_statement_begin_β
.Lx346_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n120_subscript_α:       sub              rsp, 16
                        mov              r11, 121
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx347_240
                        add              rsp, 16;                             jmp   n119_lit_integer_β
.Lx347_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n121_lit_real_α
n120_subscript_β:       mov              r11, 121
                        add              rsp, 16;                             jmp   n119_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n121_lit_real_α:        sub              rsp, 16
                        mov              r11, 122
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx348_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n122_assign_var_α
.Lx348_0:               .quad            4614253070214989087
#-----------------------------------------------------------------------------------------------------------------------
n122_assign_var_α:      sub              rsp, 16
                        mov              r11, 123
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_real
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx349_240
                        add              rsp, 32;                             jmp   n120_subscript_β
.Lx349_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n123_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n123_statement_end_α:   mov              r11, 124
                        mov              r10, 28
                        add              rsp, 80;                             jmp   n124_statement_begin_α
#=======================================================================================================================
#         DIFFER(a<1>, 3.14)                   :f(e008)
#-----------------------------------------------------------------------------------------------------------------------
n124_statement_begin_α: mov              r11, 125
                        mov              r10, 29;                             jmp   n125_var_α
n124_statement_begin_β: mov              r11, 125;                            jmp   n136_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n125_var_α:             sub              rsp, 16
                        mov              r11, 126
                        mov              rax, qword ptr [r9 + 0]              # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n126_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n126_lit_integer_α:     sub              rsp, 16
                        mov              r11, 127
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx355_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n127_subscript_α
n126_lit_integer_β:     mov              r11, 127
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n124_statement_begin_β
.Lx355_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n127_subscript_α:       sub              rsp, 16
                        mov              r11, 128
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx356_240
                        add              rsp, 16;                             jmp   n126_lit_integer_β
.Lx356_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n128_deref_α
n127_subscript_β:       mov              r11, 128
                        add              rsp, 16;                             jmp   n126_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n128_deref_α:           sub              rsp, 16
                        mov              r11, 129
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx357_240
                        add              rsp, 16;                             jmp   n127_subscript_β
.Lx357_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n129_lit_real_α
n128_deref_β:           mov              r11, 129
                        add              rsp, 16;                             jmp   n127_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n129_lit_real_α:        sub              rsp, 16
                        mov              r11, 130
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              rax, qword ptr [rip + .Lx358_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n130_call_α
.Lx358_0:               .quad            4614253070214989087
#-----------------------------------------------------------------------------------------------------------------------
n130_call_α:            sub              rsp, 16
                        mov              r11, 131
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
.Lrkfnzd360:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd360]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx359_240
                        add              rsp, 32;                             jmp   n128_deref_β
.Lx359_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n131_statement_end_α
n130_call_β:            mov              r11, 131
                        add              rsp, 32;                             jmp   n128_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n131_statement_end_α:   mov              r11, 132
                        mov              r10, 29
                        add              rsp, 96;                             jmp   n132_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/008: array stores real'    :(END)
#-----------------------------------------------------------------------------------------------------------------------
n132_statement_begin_α: mov              r11, 133
                        mov              r10, 30;                             jmp   n133_lit_string_α
n132_statement_begin_β: mov              r11, 133;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n133_lit_string_α:      sub              rsp, 16
                        mov              r11, 134
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 32
                        mov              rax, qword ptr [rip + .Lx365_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n134_assign_α
.Lx365_0:               .quad            .Lx365_0_s
.Lx365_0_s:             .string          "FAIL 1110/008: array stores real"
#-----------------------------------------------------------------------------------------------------------------------
n134_assign_α:          mov              r11, 135
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx366_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n135_statement_end_α
.Lx366_0:               .quad            .Lx366_0_s
.Lx366_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n135_statement_end_α:   mov              r11, 136
                        mov              r10, 30
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e008  <stmt 31, line 49: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n136_statement_begin_α: mov              r11, 137
                        mov              r10, 31;                             jmp   n137_statement_end_α
n136_statement_begin_β: mov              r11, 137;                            jmp   n138_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n137_statement_end_α:   mov              r11, 138
                        mov              r10, 31;                             jmp   n138_statement_begin_α
#=======================================================================================================================
#         a<3> = 'z'
#-----------------------------------------------------------------------------------------------------------------------
n138_statement_begin_α: mov              r11, 139
                        mov              r10, 32;                             jmp   n139_var_α
n138_statement_begin_β: mov              r11, 139;                            jmp   n145_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n139_var_α:             sub              rsp, 16
                        mov              r11, 140
                        mov              rax, qword ptr [r9 + 0]              # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n140_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n140_lit_integer_α:     sub              rsp, 16
                        mov              r11, 141
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx376_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n141_subscript_α
n140_lit_integer_β:     mov              r11, 141
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n138_statement_begin_β
.Lx376_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n141_subscript_α:       sub              rsp, 16
                        mov              r11, 142
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx377_240
                        add              rsp, 16;                             jmp   n140_lit_integer_β
.Lx377_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n142_lit_string_α
n141_subscript_β:       mov              r11, 142
                        add              rsp, 16;                             jmp   n140_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n142_lit_string_α:      sub              rsp, 16
                        mov              r11, 143
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx378_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n143_assign_var_α
n142_lit_string_β:      mov              r11, 143
                        add              rsp, 16;                             jmp   n141_subscript_β
.Lx378_0:               .quad            .Lx378_0_s
.Lx378_0_s:             .string          "z"
#-----------------------------------------------------------------------------------------------------------------------
n143_assign_var_α:      sub              rsp, 16
                        mov              r11, 144
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx379_240
                        add              rsp, 16;                             jmp   n142_lit_string_β
.Lx379_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n144_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n144_statement_end_α:   mov              r11, 145
                        mov              r10, 32
                        add              rsp, 80;                             jmp   n145_statement_begin_α
#=======================================================================================================================
#         DIFFER(a<3>, 'z')                   :f(e009)
#-----------------------------------------------------------------------------------------------------------------------
n145_statement_begin_α: mov              r11, 146
                        mov              r10, 33;                             jmp   n146_var_α
n145_statement_begin_β: mov              r11, 146;                            jmp   n157_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n146_var_α:             sub              rsp, 16
                        mov              r11, 147
                        mov              rax, qword ptr [r9 + 0]              # a
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n147_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n147_lit_integer_α:     sub              rsp, 16
                        mov              r11, 148
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx385_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n148_subscript_α
n147_lit_integer_β:     mov              r11, 148
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n145_statement_begin_β
.Lx385_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n148_subscript_α:       sub              rsp, 16
                        mov              r11, 149
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx386_240
                        add              rsp, 16;                             jmp   n147_lit_integer_β
.Lx386_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n149_deref_α
n148_subscript_β:       mov              r11, 149
                        add              rsp, 16;                             jmp   n147_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n149_deref_α:           sub              rsp, 16
                        mov              r11, 150
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx387_240
                        add              rsp, 16;                             jmp   n148_subscript_β
.Lx387_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n150_lit_string_α
n149_deref_β:           mov              r11, 150
                        add              rsp, 16;                             jmp   n148_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n150_lit_string_α:      sub              rsp, 16
                        mov              r11, 151
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx388_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n151_call_α
n150_lit_string_β:      mov              r11, 151
                        add              rsp, 16;                             jmp   n149_deref_β
.Lx388_0:               .quad            .Lx388_0_s
.Lx388_0_s:             .string          "z"
#-----------------------------------------------------------------------------------------------------------------------
n151_call_α:            sub              rsp, 16
                        mov              r11, 152
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
.Lrkfnzd390:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd390]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx389_240
                        add              rsp, 16;                             jmp   n150_lit_string_β
.Lx389_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n152_statement_end_α
n151_call_β:            mov              r11, 152
                        add              rsp, 16;                             jmp   n150_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n152_statement_end_α:   mov              r11, 153
                        mov              r10, 33
                        add              rsp, 96;                             jmp   n153_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1110/009: array stores string'  :(END)
#-----------------------------------------------------------------------------------------------------------------------
n153_statement_begin_α: mov              r11, 154
                        mov              r10, 34;                             jmp   n154_lit_string_α
n153_statement_begin_β: mov              r11, 154;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n154_lit_string_α:      sub              rsp, 16
                        mov              r11, 155
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx395_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n155_assign_α
.Lx395_0:               .quad            .Lx395_0_s
.Lx395_0_s:             .string          "FAIL 1110/009: array stores string"
#-----------------------------------------------------------------------------------------------------------------------
n155_assign_α:          mov              r11, 156
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx396_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n156_statement_end_α
.Lx396_0:               .quad            .Lx396_0_s
.Lx396_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n156_statement_end_α:   mov              r11, 157
                        mov              r10, 34
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e009  <stmt 35, line 55: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n157_statement_begin_α: mov              r11, 158
                        mov              r10, 35;                             jmp   n158_statement_end_α
n157_statement_begin_β: mov              r11, 158;                            jmp   n159_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n158_statement_end_α:   mov              r11, 159
                        mov              r10, 35;                             jmp   n159_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1110_array_1d (9/9)'
#-----------------------------------------------------------------------------------------------------------------------
n159_statement_begin_α: mov              r11, 160
                        mov              r10, 36;                             jmp   n160_lit_string_α
n159_statement_begin_β: mov              r11, 160;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n160_lit_string_α:      sub              rsp, 16
                        mov              r11, 161
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 24
                        mov              rax, qword ptr [rip + .Lx405_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n161_assign_α
.Lx405_0:               .quad            .Lx405_0_s
.Lx405_0_s:             .string          "PASS 1110_array_1d (9/9)"
#-----------------------------------------------------------------------------------------------------------------------
n161_assign_α:          mov              r11, 162
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx406_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n162_statement_end_α
.Lx406_0:               .quad            .Lx406_0_s
.Lx406_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n162_statement_end_α:   mov              r11, 163
                        mov              r10, 36
                        add              rsp, 16;                             jmp   main_γ
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
