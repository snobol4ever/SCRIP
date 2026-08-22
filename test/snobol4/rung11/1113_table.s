                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 3
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 3
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
                        .section         .rodata
.Lgvan0:                .string          "t"
.Lgvan1:                .string          "ta"
.Lgvan2:                .string          "ata"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
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
#         t = TABLE(10)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:   mov              r11, 3
                        mov              r10, 2;                              jmp   n3_lit_integer_α
n2_statement_begin_β:   mov              r11, 3;                              jmp   n7_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:       sub              rsp, 16
                        mov              r11, 4
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx161_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_call_α
.Lx161_0:               .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:              sub              rsp, 16
                        mov              r11, 5
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd163:            .string          "TABLE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd163]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx162_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n2_statement_begin_β
.Lx162_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n5_assign_α
n4_call_β:              mov              r11, 5
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n2_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_assign_α:            mov              r11, 6
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # t
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
#         DIFFER(t<'cat'>)           :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_begin_α:   mov              r11, 10
                        mov              r10, 4;                              jmp   n10_var_α
n9_statement_begin_β:   mov              r11, 10;                             jmp   n20_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:              sub              rsp, 16
                        mov              r11, 11
                        mov              rax, qword ptr [r9 + 0]              # t
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n11_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_string_α:       sub              rsp, 16
                        mov              r11, 12
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx174_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n12_subscript_α
n11_lit_string_β:       mov              r11, 12
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n9_statement_begin_β
.Lx174_0:               .quad            .Lx174_0_s
.Lx174_0_s:             .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
n12_subscript_α:        sub              rsp, 16
                        mov              r11, 13
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx175_240
                        add              rsp, 16;                             jmp   n11_lit_string_β
.Lx175_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n13_deref_α
n12_subscript_β:        mov              r11, 13
                        add              rsp, 16;                             jmp   n11_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n13_deref_α:            sub              rsp, 16
                        mov              r11, 14
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx176_240
                        add              rsp, 16;                             jmp   n12_subscript_β
.Lx176_240:             mov              qword ptr [rsp + 0], rax             # result
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
.Lrkfnzd178:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd178]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx177_240
                        add              rsp, 16;                             jmp   n13_deref_β
.Lx177_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n15_statement_end_α
n14_call_β:             mov              r11, 15
                        add              rsp, 16;                             jmp   n13_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:    mov              r11, 16
                        mov              r10, 4
                        add              rsp, 80;                             jmp   n16_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1113/001: absent key is null'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_begin_α:  mov              r11, 17
                        mov              r10, 5;                              jmp   n17_lit_string_α
n16_statement_begin_β:  mov              r11, 17;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_string_α:       sub              rsp, 16
                        mov              r11, 18
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx183_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n18_assign_α
.Lx183_0:               .quad            .Lx183_0_s
.Lx183_0_s:             .string          "FAIL 1113/001: absent key is null"
#-----------------------------------------------------------------------------------------------------------------------
n18_assign_α:           mov              r11, 19
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx184_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n19_statement_end_α
.Lx184_0:               .quad            .Lx184_0_s
.Lx184_0_s:             .string          "OUTPUT"
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
#         t<'cat'> = 'dog'
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:  mov              r11, 23
                        mov              r10, 7;                              jmp   n23_var_α
n22_statement_begin_β:  mov              r11, 23;                             jmp   n29_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n23_var_α:              sub              rsp, 16
                        mov              r11, 24
                        mov              rax, qword ptr [r9 + 0]              # t
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n24_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_string_α:       sub              rsp, 16
                        mov              r11, 25
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx194_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n25_subscript_α
n24_lit_string_β:       mov              r11, 25
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n22_statement_begin_β
.Lx194_0:               .quad            .Lx194_0_s
.Lx194_0_s:             .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
n25_subscript_α:        sub              rsp, 16
                        mov              r11, 26
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx195_240
                        add              rsp, 16;                             jmp   n24_lit_string_β
.Lx195_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n26_lit_string_α
n25_subscript_β:        mov              r11, 26
                        add              rsp, 16;                             jmp   n24_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_string_α:       sub              rsp, 16
                        mov              r11, 27
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx196_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n27_assign_var_α
n26_lit_string_β:       mov              r11, 27
                        add              rsp, 16;                             jmp   n25_subscript_β
.Lx196_0:               .quad            .Lx196_0_s
.Lx196_0_s:             .string          "dog"
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_var_α:       sub              rsp, 16
                        mov              r11, 28
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx197_240
                        add              rsp, 16;                             jmp   n26_lit_string_β
.Lx197_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n28_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:    mov              r11, 29
                        mov              r10, 7
                        add              rsp, 80;                             jmp   n29_statement_begin_α
#=======================================================================================================================
#         DIFFER(t<'cat'>, 'dog')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_begin_α:  mov              r11, 30
                        mov              r10, 8;                              jmp   n30_var_α
n29_statement_begin_β:  mov              r11, 30;                             jmp   n41_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n30_var_α:              sub              rsp, 16
                        mov              r11, 31
                        mov              rax, qword ptr [r9 + 0]              # t
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n31_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n31_lit_string_α:       sub              rsp, 16
                        mov              r11, 32
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx203_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n32_subscript_α
n31_lit_string_β:       mov              r11, 32
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n29_statement_begin_β
.Lx203_0:               .quad            .Lx203_0_s
.Lx203_0_s:             .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
n32_subscript_α:        sub              rsp, 16
                        mov              r11, 33
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx204_240
                        add              rsp, 16;                             jmp   n31_lit_string_β
.Lx204_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n33_deref_α
n32_subscript_β:        mov              r11, 33
                        add              rsp, 16;                             jmp   n31_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n33_deref_α:            sub              rsp, 16
                        mov              r11, 34
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx205_240
                        add              rsp, 16;                             jmp   n32_subscript_β
.Lx205_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n34_lit_string_α
n33_deref_β:            mov              r11, 34
                        add              rsp, 16;                             jmp   n32_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:       sub              rsp, 16
                        mov              r11, 35
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx206_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n35_call_α
n34_lit_string_β:       mov              r11, 35
                        add              rsp, 16;                             jmp   n33_deref_β
.Lx206_0:               .quad            .Lx206_0_s
.Lx206_0_s:             .string          "dog"
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
.Lrkfnzd208:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd208]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx207_240
                        add              rsp, 16;                             jmp   n34_lit_string_β
.Lx207_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n36_statement_end_α
n35_call_β:             mov              r11, 36
                        add              rsp, 16;                             jmp   n34_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:    mov              r11, 37
                        mov              r10, 8
                        add              rsp, 96;                             jmp   n37_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1113/002: string key assign/read' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_begin_α:  mov              r11, 38
                        mov              r10, 9;                              jmp   n38_lit_string_α
n37_statement_begin_β:  mov              r11, 38;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_string_α:       sub              rsp, 16
                        mov              r11, 39
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx213_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n39_assign_α
.Lx213_0:               .quad            .Lx213_0_s
.Lx213_0_s:             .string          "FAIL 1113/002: string key assign/read"
#-----------------------------------------------------------------------------------------------------------------------
n39_assign_α:           mov              r11, 40
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx214_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n40_statement_end_α
.Lx214_0:               .quad            .Lx214_0_s
.Lx214_0_s:             .string          "OUTPUT"
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
#         t<7> = 45
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_begin_α:  mov              r11, 44
                        mov              r10, 11;                             jmp   n44_var_α
n43_statement_begin_β:  mov              r11, 44;                             jmp   n50_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n44_var_α:              sub              rsp, 16
                        mov              r11, 45
                        mov              rax, qword ptr [r9 + 0]              # t
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n45_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_integer_α:      sub              rsp, 16
                        mov              r11, 46
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx224_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n46_subscript_α
n45_lit_integer_β:      mov              r11, 46
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n43_statement_begin_β
.Lx224_0:               .quad            7
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
                        cmp              al, 104;                             jne   .Lx225_240
                        add              rsp, 16;                             jmp   n45_lit_integer_β
.Lx225_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n47_lit_integer_α
n46_subscript_β:        mov              r11, 47
                        add              rsp, 16;                             jmp   n45_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_integer_α:      sub              rsp, 16
                        mov              r11, 48
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx226_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n48_assign_var_α
n47_lit_integer_β:      mov              r11, 48
                        add              rsp, 16;                             jmp   n46_subscript_β
.Lx226_0:               .quad            45
#-----------------------------------------------------------------------------------------------------------------------
n48_assign_var_α:       sub              rsp, 16
                        mov              r11, 49
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx227_240
                        add              rsp, 16;                             jmp   n47_lit_integer_β
.Lx227_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n49_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_end_α:    mov              r11, 50
                        mov              r10, 11
                        add              rsp, 80;                             jmp   n50_statement_begin_α
#=======================================================================================================================
#         DIFFER(t<7>, 45)                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_begin_α:  mov              r11, 51
                        mov              r10, 12;                             jmp   n51_var_α
n50_statement_begin_β:  mov              r11, 51;                             jmp   n62_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n51_var_α:              sub              rsp, 16
                        mov              r11, 52
                        mov              rax, qword ptr [r9 + 0]              # t
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n52_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n52_lit_integer_α:      sub              rsp, 16
                        mov              r11, 53
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx233_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n53_subscript_α
n52_lit_integer_β:      mov              r11, 53
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n50_statement_begin_β
.Lx233_0:               .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n53_subscript_α:        sub              rsp, 16
                        mov              r11, 54
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx234_240
                        add              rsp, 16;                             jmp   n52_lit_integer_β
.Lx234_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n54_deref_α
n53_subscript_β:        mov              r11, 54
                        add              rsp, 16;                             jmp   n52_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n54_deref_α:            sub              rsp, 16
                        mov              r11, 55
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx235_240
                        add              rsp, 16;                             jmp   n53_subscript_β
.Lx235_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n55_lit_integer_α
n54_deref_β:            mov              r11, 55
                        add              rsp, 16;                             jmp   n53_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_integer_α:      sub              rsp, 16
                        mov              r11, 56
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx236_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n56_call_α
n55_lit_integer_β:      mov              r11, 56
                        add              rsp, 16;                             jmp   n54_deref_β
.Lx236_0:               .quad            45
#-----------------------------------------------------------------------------------------------------------------------
n56_call_α:             sub              rsp, 16
                        mov              r11, 57
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
.Lrkfnzd238:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd238]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx237_240
                        add              rsp, 16;                             jmp   n55_lit_integer_β
.Lx237_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n57_statement_end_α
n56_call_β:             mov              r11, 57
                        add              rsp, 16;                             jmp   n55_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_end_α:    mov              r11, 58
                        mov              r10, 12
                        add              rsp, 96;                             jmp   n58_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1113/003: integer key assign/read' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n58_statement_begin_α:  mov              r11, 59
                        mov              r10, 13;                             jmp   n59_lit_string_α
n58_statement_begin_β:  mov              r11, 59;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n59_lit_string_α:       sub              rsp, 16
                        mov              r11, 60
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 38
                        mov              rax, qword ptr [rip + .Lx243_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n60_assign_α
.Lx243_0:               .quad            .Lx243_0_s
.Lx243_0_s:             .string          "FAIL 1113/003: integer key assign/read"
#-----------------------------------------------------------------------------------------------------------------------
n60_assign_α:           mov              r11, 61
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx244_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n61_statement_end_α
.Lx244_0:               .quad            .Lx244_0_s
.Lx244_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_end_α:    mov              r11, 62
                        mov              r10, 13
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e003  <stmt 14, line 22: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n62_statement_begin_α:  mov              r11, 63
                        mov              r10, 14;                             jmp   n63_statement_end_α
n62_statement_begin_β:  mov              r11, 63;                             jmp   n64_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n63_statement_end_α:    mov              r11, 64
                        mov              r10, 14;                             jmp   n64_statement_begin_α
#=======================================================================================================================
#         DIFFER(t<'cat'>, 'dog')                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_begin_α:  mov              r11, 65
                        mov              r10, 15;                             jmp   n65_var_α
n64_statement_begin_β:  mov              r11, 65;                             jmp   n76_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n65_var_α:              sub              rsp, 16
                        mov              r11, 66
                        mov              rax, qword ptr [r9 + 0]              # t
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n66_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n66_lit_string_α:       sub              rsp, 16
                        mov              r11, 67
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx254_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n67_subscript_α
n66_lit_string_β:       mov              r11, 67
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n64_statement_begin_β
.Lx254_0:               .quad            .Lx254_0_s
.Lx254_0_s:             .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
n67_subscript_α:        sub              rsp, 16
                        mov              r11, 68
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx255_240
                        add              rsp, 16;                             jmp   n66_lit_string_β
.Lx255_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n68_deref_α
n67_subscript_β:        mov              r11, 68
                        add              rsp, 16;                             jmp   n66_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n68_deref_α:            sub              rsp, 16
                        mov              r11, 69
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx256_240
                        add              rsp, 16;                             jmp   n67_subscript_β
.Lx256_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n69_lit_string_α
n68_deref_β:            mov              r11, 69
                        add              rsp, 16;                             jmp   n67_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_string_α:       sub              rsp, 16
                        mov              r11, 70
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx257_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n70_call_α
n69_lit_string_β:       mov              r11, 70
                        add              rsp, 16;                             jmp   n68_deref_β
.Lx257_0:               .quad            .Lx257_0_s
.Lx257_0_s:             .string          "dog"
#-----------------------------------------------------------------------------------------------------------------------
n70_call_α:             sub              rsp, 16
                        mov              r11, 71
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
.Lrkfnzd259:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd259]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx258_240
                        add              rsp, 16;                             jmp   n69_lit_string_β
.Lx258_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n71_statement_end_α
n70_call_β:             mov              r11, 71
                        add              rsp, 16;                             jmp   n69_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_end_α:    mov              r11, 72
                        mov              r10, 15
                        add              rsp, 96;                             jmp   n72_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1113/004: string key survives int key add' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_begin_α:  mov              r11, 73
                        mov              r10, 16;                             jmp   n73_lit_string_α
n72_statement_begin_β:  mov              r11, 73;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n73_lit_string_α:       sub              rsp, 16
                        mov              r11, 74
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 46
                        mov              rax, qword ptr [rip + .Lx264_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n74_assign_α
.Lx264_0:               .quad            .Lx264_0_s
.Lx264_0_s:             .string          "FAIL 1113/004: string key survives int key add"
#-----------------------------------------------------------------------------------------------------------------------
n74_assign_α:           mov              r11, 75
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx265_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n75_statement_end_α
.Lx265_0:               .quad            .Lx265_0_s
.Lx265_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n75_statement_end_α:    mov              r11, 76
                        mov              r10, 16
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e004  <stmt 17, line 27: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n76_statement_begin_α:  mov              r11, 77
                        mov              r10, 17;                             jmp   n77_statement_end_α
n76_statement_begin_β:  mov              r11, 77;                             jmp   n78_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_end_α:    mov              r11, 78
                        mov              r10, 17;                             jmp   n78_statement_begin_α
#=======================================================================================================================
#         ta = CONVERT(t, 'array')
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_begin_α:  mov              r11, 79
                        mov              r10, 18;                             jmp   n79_var_α
n78_statement_begin_β:  mov              r11, 79;                             jmp   n84_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n79_var_α:              sub              rsp, 16
                        mov              r11, 80
                        mov              rax, qword ptr [r9 + 0]              # t
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n80_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n80_lit_string_α:       sub              rsp, 16
                        mov              r11, 81
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx275_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n81_call_α
n80_lit_string_β:       mov              r11, 81
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n78_statement_begin_β
.Lx275_0:               .quad            .Lx275_0_s
.Lx275_0_s:             .string          "array"
#-----------------------------------------------------------------------------------------------------------------------
n81_call_α:             sub              rsp, 16
                        mov              r11, 82
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
.Lrkfnzd277:            .string          "CONVERT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd277]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx276_240
                        add              rsp, 16;                             jmp   n80_lit_string_β
.Lx276_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n82_assign_α
n81_call_β:             mov              r11, 82
                        add              rsp, 16;                             jmp   n80_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n82_assign_α:           mov              r11, 83
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax             # ta
                        mov              qword ptr [r9 + 24], rdx;            jmp   n83_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n83_statement_end_α:    mov              r11, 84
                        mov              r10, 18
                        add              rsp, 48;                             jmp   n84_statement_begin_α
#=======================================================================================================================
#         DIFFER(PROTOTYPE(ta), '2,2')                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n84_statement_begin_α:  mov              r11, 85
                        mov              r10, 19;                             jmp   n85_var_α
n84_statement_begin_β:  mov              r11, 85;                             jmp   n94_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n85_var_α:              sub              rsp, 16
                        mov              r11, 86
                        mov              rax, qword ptr [r9 + 16]             # ta
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n86_call_α
#-----------------------------------------------------------------------------------------------------------------------
n86_call_α:             sub              rsp, 16
                        mov              r11, 87
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd285:            .string          "PROTOTYPE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd285]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx284_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n84_statement_begin_β
.Lx284_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n87_lit_string_α
n86_call_β:             mov              r11, 87
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n84_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n87_lit_string_α:       sub              rsp, 16
                        mov              r11, 88
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx286_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n88_call_α
n87_lit_string_β:       mov              r11, 88
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n84_statement_begin_β
.Lx286_0:               .quad            .Lx286_0_s
.Lx286_0_s:             .string          "2,2"
#-----------------------------------------------------------------------------------------------------------------------
n88_call_α:             sub              rsp, 16
                        mov              r11, 89
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
.Lrkfnzd288:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd288]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx287_240
                        add              rsp, 16;                             jmp   n87_lit_string_β
.Lx287_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n89_statement_end_α
n88_call_β:             mov              r11, 89
                        add              rsp, 16;                             jmp   n87_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_end_α:    mov              r11, 90
                        mov              r10, 19
                        add              rsp, 64;                             jmp   n90_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1113/005: table->array prototype 2,2' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_begin_α:  mov              r11, 91
                        mov              r10, 20;                             jmp   n91_lit_string_α
n90_statement_begin_β:  mov              r11, 91;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n91_lit_string_α:       sub              rsp, 16
                        mov              r11, 92
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 41
                        mov              rax, qword ptr [rip + .Lx293_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n92_assign_α
.Lx293_0:               .quad            .Lx293_0_s
.Lx293_0_s:             .string          "FAIL 1113/005: table->array prototype 2,2"
#-----------------------------------------------------------------------------------------------------------------------
n92_assign_α:           mov              r11, 93
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx294_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n93_statement_end_α
.Lx294_0:               .quad            .Lx294_0_s
.Lx294_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n93_statement_end_α:    mov              r11, 94
                        mov              r10, 20
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e005  <stmt 21, line 33: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n94_statement_begin_α:  mov              r11, 95
                        mov              r10, 21;                             jmp   n95_statement_end_α
n94_statement_begin_β:  mov              r11, 95;                             jmp   n96_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n95_statement_end_α:    mov              r11, 96
                        mov              r10, 21;                             jmp   n96_statement_begin_α
#=======================================================================================================================
#         ata = CONVERT(ta, 'table')
#-----------------------------------------------------------------------------------------------------------------------
n96_statement_begin_α:  mov              r11, 97
                        mov              r10, 22;                             jmp   n97_var_α
n96_statement_begin_β:  mov              r11, 97;                             jmp   n102_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n97_var_α:              sub              rsp, 16
                        mov              r11, 98
                        mov              rax, qword ptr [r9 + 16]             # ta
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n98_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n98_lit_string_α:       sub              rsp, 16
                        mov              r11, 99
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx304_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n99_call_α
n98_lit_string_β:       mov              r11, 99
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n96_statement_begin_β
.Lx304_0:               .quad            .Lx304_0_s
.Lx304_0_s:             .string          "table"
#-----------------------------------------------------------------------------------------------------------------------
n99_call_α:             sub              rsp, 16
                        mov              r11, 100
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
.Lrkfnzd306:            .string          "CONVERT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd306]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx305_240
                        add              rsp, 16;                             jmp   n98_lit_string_β
.Lx305_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n100_assign_α
n99_call_β:             mov              r11, 100
                        add              rsp, 16;                             jmp   n98_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n100_assign_α:          mov              r11, 101
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax             # ata
                        mov              qword ptr [r9 + 40], rdx;            jmp   n101_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n101_statement_end_α:   mov              r11, 102
                        mov              r10, 22
                        add              rsp, 48;                             jmp   n102_statement_begin_α
#=======================================================================================================================
#         DIFFER(ata<7>, 45)                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_begin_α: mov              r11, 103
                        mov              r10, 23;                             jmp   n103_var_α
n102_statement_begin_β: mov              r11, 103;                            jmp   n114_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n103_var_α:             sub              rsp, 16
                        mov              r11, 104
                        mov              rax, qword ptr [r9 + 32]             # ata
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n104_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n104_lit_integer_α:     sub              rsp, 16
                        mov              r11, 105
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx313_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n105_subscript_α
n104_lit_integer_β:     mov              r11, 105
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n102_statement_begin_β
.Lx313_0:               .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n105_subscript_α:       sub              rsp, 16
                        mov              r11, 106
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx314_240
                        add              rsp, 16;                             jmp   n104_lit_integer_β
.Lx314_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n106_deref_α
n105_subscript_β:       mov              r11, 106
                        add              rsp, 16;                             jmp   n104_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n106_deref_α:           sub              rsp, 16
                        mov              r11, 107
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx315_240
                        add              rsp, 16;                             jmp   n105_subscript_β
.Lx315_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n107_lit_integer_α
n106_deref_β:           mov              r11, 107
                        add              rsp, 16;                             jmp   n105_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n107_lit_integer_α:     sub              rsp, 16
                        mov              r11, 108
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx316_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n108_call_α
n107_lit_integer_β:     mov              r11, 108
                        add              rsp, 16;                             jmp   n106_deref_β
.Lx316_0:               .quad            45
#-----------------------------------------------------------------------------------------------------------------------
n108_call_α:            sub              rsp, 16
                        mov              r11, 109
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
.Lrkfnzd318:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd318]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx317_240
                        add              rsp, 16;                             jmp   n107_lit_integer_β
.Lx317_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n109_statement_end_α
n108_call_β:            mov              r11, 109
                        add              rsp, 16;                             jmp   n107_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n109_statement_end_α:   mov              r11, 110
                        mov              r10, 23
                        add              rsp, 96;                             jmp   n110_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1113/006: array->table int key roundtrip' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n110_statement_begin_α: mov              r11, 111
                        mov              r10, 24;                             jmp   n111_lit_string_α
n110_statement_begin_β: mov              r11, 111;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n111_lit_string_α:      sub              rsp, 16
                        mov              r11, 112
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 45
                        mov              rax, qword ptr [rip + .Lx323_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n112_assign_α
.Lx323_0:               .quad            .Lx323_0_s
.Lx323_0_s:             .string          "FAIL 1113/006: array->table int key roundtrip"
#-----------------------------------------------------------------------------------------------------------------------
n112_assign_α:          mov              r11, 113
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx324_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n113_statement_end_α
.Lx324_0:               .quad            .Lx324_0_s
.Lx324_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n113_statement_end_α:   mov              r11, 114
                        mov              r10, 24
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e006  <stmt 25, line 39: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n114_statement_begin_α: mov              r11, 115
                        mov              r10, 25;                             jmp   n115_statement_end_α
n114_statement_begin_β: mov              r11, 115;                            jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n115_statement_end_α:   mov              r11, 116
                        mov              r10, 25;                             jmp   n116_statement_begin_α
#=======================================================================================================================
#         DIFFER(ata<'cat'>, 'dog')                   :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n116_statement_begin_α: mov              r11, 117
                        mov              r10, 26;                             jmp   n117_var_α
n116_statement_begin_β: mov              r11, 117;                            jmp   n128_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n117_var_α:             sub              rsp, 16
                        mov              r11, 118
                        mov              rax, qword ptr [r9 + 32]             # ata
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n118_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n118_lit_string_α:      sub              rsp, 16
                        mov              r11, 119
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx334_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n119_subscript_α
n118_lit_string_β:      mov              r11, 119
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n116_statement_begin_β
.Lx334_0:               .quad            .Lx334_0_s
.Lx334_0_s:             .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
n119_subscript_α:       sub              rsp, 16
                        mov              r11, 120
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx335_240
                        add              rsp, 16;                             jmp   n118_lit_string_β
.Lx335_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n120_deref_α
n119_subscript_β:       mov              r11, 120
                        add              rsp, 16;                             jmp   n118_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n120_deref_α:           sub              rsp, 16
                        mov              r11, 121
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx336_240
                        add              rsp, 16;                             jmp   n119_subscript_β
.Lx336_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n121_lit_string_α
n120_deref_β:           mov              r11, 121
                        add              rsp, 16;                             jmp   n119_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n121_lit_string_α:      sub              rsp, 16
                        mov              r11, 122
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx337_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n122_call_α
n121_lit_string_β:      mov              r11, 122
                        add              rsp, 16;                             jmp   n120_deref_β
.Lx337_0:               .quad            .Lx337_0_s
.Lx337_0_s:             .string          "dog"
#-----------------------------------------------------------------------------------------------------------------------
n122_call_α:            sub              rsp, 16
                        mov              r11, 123
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
.Lrkfnzd339:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd339]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx338_240
                        add              rsp, 16;                             jmp   n121_lit_string_β
.Lx338_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n123_statement_end_α
n122_call_β:            mov              r11, 123
                        add              rsp, 16;                             jmp   n121_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n123_statement_end_α:   mov              r11, 124
                        mov              r10, 26
                        add              rsp, 96;                             jmp   n124_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1113/007: array->table string key roundtrip' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n124_statement_begin_α: mov              r11, 125
                        mov              r10, 27;                             jmp   n125_lit_string_α
n124_statement_begin_β: mov              r11, 125;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n125_lit_string_α:      sub              rsp, 16
                        mov              r11, 126
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 48
                        mov              rax, qword ptr [rip + .Lx344_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n126_assign_α
.Lx344_0:               .quad            .Lx344_0_s
.Lx344_0_s:             .string          "FAIL 1113/007: array->table string key roundtrip"
#-----------------------------------------------------------------------------------------------------------------------
n126_assign_α:          mov              r11, 127
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx345_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n127_statement_end_α
.Lx345_0:               .quad            .Lx345_0_s
.Lx345_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n127_statement_end_α:   mov              r11, 128
                        mov              r10, 27
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e007  <stmt 28, line 44: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n128_statement_begin_α: mov              r11, 129
                        mov              r10, 28;                             jmp   n129_statement_end_α
n128_statement_begin_β: mov              r11, 129;                            jmp   n130_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n129_statement_end_α:   mov              r11, 130
                        mov              r10, 28;                             jmp   n130_statement_begin_α
#=======================================================================================================================
#         t['cat'] = 'fish'
#-----------------------------------------------------------------------------------------------------------------------
n130_statement_begin_α: mov              r11, 131
                        mov              r10, 29;                             jmp   n131_var_α
n130_statement_begin_β: mov              r11, 131;                            jmp   n137_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n131_var_α:             sub              rsp, 16
                        mov              r11, 132
                        mov              rax, qword ptr [r9 + 0]              # t
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n132_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n132_lit_string_α:      sub              rsp, 16
                        mov              r11, 133
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx355_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n133_subscript_α
n132_lit_string_β:      mov              r11, 133
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n130_statement_begin_β
.Lx355_0:               .quad            .Lx355_0_s
.Lx355_0_s:             .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
n133_subscript_α:       sub              rsp, 16
                        mov              r11, 134
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx356_240
                        add              rsp, 16;                             jmp   n132_lit_string_β
.Lx356_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n134_lit_string_α
n133_subscript_β:       mov              r11, 134
                        add              rsp, 16;                             jmp   n132_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n134_lit_string_α:      sub              rsp, 16
                        mov              r11, 135
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx357_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n135_assign_var_α
n134_lit_string_β:      mov              r11, 135
                        add              rsp, 16;                             jmp   n133_subscript_β
.Lx357_0:               .quad            .Lx357_0_s
.Lx357_0_s:             .string          "fish"
#-----------------------------------------------------------------------------------------------------------------------
n135_assign_var_α:      sub              rsp, 16
                        mov              r11, 136
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx358_240
                        add              rsp, 16;                             jmp   n134_lit_string_β
.Lx358_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n136_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n136_statement_end_α:   mov              r11, 137
                        mov              r10, 29
                        add              rsp, 80;                             jmp   n137_statement_begin_α
#=======================================================================================================================
#         DIFFER(t<'cat'>, 'fish')                   :f(e008)
#-----------------------------------------------------------------------------------------------------------------------
n137_statement_begin_α: mov              r11, 138
                        mov              r10, 30;                             jmp   n138_var_α
n137_statement_begin_β: mov              r11, 138;                            jmp   n149_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n138_var_α:             sub              rsp, 16
                        mov              r11, 139
                        mov              rax, qword ptr [r9 + 0]              # t
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n139_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n139_lit_string_α:      sub              rsp, 16
                        mov              r11, 140
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx364_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n140_subscript_α
n139_lit_string_β:      mov              r11, 140
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n137_statement_begin_β
.Lx364_0:               .quad            .Lx364_0_s
.Lx364_0_s:             .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
n140_subscript_α:       sub              rsp, 16
                        mov              r11, 141
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx365_240
                        add              rsp, 16;                             jmp   n139_lit_string_β
.Lx365_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n141_deref_α
n140_subscript_β:       mov              r11, 141
                        add              rsp, 16;                             jmp   n139_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n141_deref_α:           sub              rsp, 16
                        mov              r11, 142
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx366_240
                        add              rsp, 16;                             jmp   n140_subscript_β
.Lx366_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n142_lit_string_α
n141_deref_β:           mov              r11, 142
                        add              rsp, 16;                             jmp   n140_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n142_lit_string_α:      sub              rsp, 16
                        mov              r11, 143
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx367_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n143_call_α
n142_lit_string_β:      mov              r11, 143
                        add              rsp, 16;                             jmp   n141_deref_β
.Lx367_0:               .quad            .Lx367_0_s
.Lx367_0_s:             .string          "fish"
#-----------------------------------------------------------------------------------------------------------------------
n143_call_α:            sub              rsp, 16
                        mov              r11, 144
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
.Lrkfnzd369:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd369]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx368_240
                        add              rsp, 16;                             jmp   n142_lit_string_β
.Lx368_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n144_statement_end_α
n143_call_β:            mov              r11, 144
                        add              rsp, 16;                             jmp   n142_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n144_statement_end_α:   mov              r11, 145
                        mov              r10, 30
                        add              rsp, 96;                             jmp   n145_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1113/008: [] and <> syntax equivalent' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n145_statement_begin_α: mov              r11, 146
                        mov              r10, 31;                             jmp   n146_lit_string_α
n145_statement_begin_β: mov              r11, 146;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n146_lit_string_α:      sub              rsp, 16
                        mov              r11, 147
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 42
                        mov              rax, qword ptr [rip + .Lx374_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n147_assign_α
.Lx374_0:               .quad            .Lx374_0_s
.Lx374_0_s:             .string          "FAIL 1113/008: [] and <> syntax equivalent"
#-----------------------------------------------------------------------------------------------------------------------
n147_assign_α:          mov              r11, 148
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx375_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n148_statement_end_α
.Lx375_0:               .quad            .Lx375_0_s
.Lx375_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n148_statement_end_α:   mov              r11, 149
                        mov              r10, 31
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e008  <stmt 32, line 50: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n149_statement_begin_α: mov              r11, 150
                        mov              r10, 32;                             jmp   n150_statement_end_α
n149_statement_begin_β: mov              r11, 150;                            jmp   n151_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n150_statement_end_α:   mov              r11, 151
                        mov              r10, 32;                             jmp   n151_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1113_table (8/8)'
#-----------------------------------------------------------------------------------------------------------------------
n151_statement_begin_α: mov              r11, 152
                        mov              r10, 33;                             jmp   n152_lit_string_α
n151_statement_begin_β: mov              r11, 152;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n152_lit_string_α:      sub              rsp, 16
                        mov              r11, 153
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 21
                        mov              rax, qword ptr [rip + .Lx384_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n153_assign_α
.Lx384_0:               .quad            .Lx384_0_s
.Lx384_0_s:             .string          "PASS 1113_table (8/8)"
#-----------------------------------------------------------------------------------------------------------------------
n153_assign_α:          mov              r11, 154
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx385_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n154_statement_end_α
.Lx385_0:               .quad            .Lx385_0_s
.Lx385_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n154_statement_end_α:   mov              r11, 155
                        mov              r10, 33
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
