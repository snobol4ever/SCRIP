                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             module_init
                        mov              edi, 6
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 6
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
                        .section         .rodata
.Lgvan0:                .string          "roman"
.Lgvan1:                .string          "n"
.Lgvan2:                .string          "s"
.Lgvan3:                .string          "v"
.Lgvan4:                .string          "r"
.Lgvan5:                .string          "i"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .quad            .Lgvan4
                        .quad            .Lgvan5
                        .section         .text
                        .intel_syntax    noprefix
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         DEFINE('roman(n)s,v,r,i')                                     :(roman_end)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                                         jmp   n1_define_α
n0_statement_begin_β:                                                         jmp   n248_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_define_α:            mov              rdi, qword ptr [rip + .Lx284_0]
                        mov              rsi, qword ptr [rip + .Lx284_1]
                        mov              edx, 5
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n3_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx284_0]
                        lea              rsi, [rip + roman_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n2_statement_end_α
n1_define_β:                                                                  jmp   n0_statement_begin_β
.Lx284_0:               .quad            .Lx284_0_s
.Lx284_0_s:             .string          "roman"
.Lx284_1:               .quad            .Lx284_1_s
.Lx284_1_s:             .string          "n,s,v,r,i"
                                                                              jmp   .Lx285_245
#-----------------------------------------------------------------------------------------------------------------------
roman_α:                sub              rsp, 128
                        mov              rax, qword ptr [r9 + 32]             # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
                        mov              rax, qword ptr [r9 + 48]             # v
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
                        mov              rax, qword ptr [r9 + 64]             # r
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
                        mov              rax, qword ptr [r9 + 80]             # i
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
                        mov              rax, qword ptr [r9 + 0]              # roman
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 96], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 128]
                        cmp              rdx, 0;                              jbe   .Lx285_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 16]             # n
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx285_41
.Lx285_10:              mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [rsp + 112], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 120], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx285_41:              lea              r10, [rip + roman_γ]
                        lea              r11, [rip + roman_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n3_statement_begin_α];   jmp   rax
roman_γ:                mov              rdi, qword ptr [r9 + 0]              # roman
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 96]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 128]
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 8], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 80], rax             # i
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 64], rax             # r
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 72], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax             # v
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax             # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        cmp              rdx, 0;                              jbe   .Lx285_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # n
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx285_110
.Lx285_80:              mov              rax, qword ptr [rsp + 112]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [r9 + 24], rax
.Lx285_110:             mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 128
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
roman_ω:                mov              rcx, qword ptr [rsp + 96]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 128]
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [r9 + 0], rax              # roman
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [r9 + 8], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 80], rax             # i
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 88], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 64], rax             # r
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 72], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 48], rax             # v
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 56], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 32], rax             # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 40], rax
                        cmp              rdx, 0;                              jbe   .Lx285_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 16], rax             # n
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 24], rax;            jmp   .Lx285_180
.Lx285_150:             mov              rax, qword ptr [rsp + 112]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [r9 + 24], rax
.Lx285_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 128
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx285_245:
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_end_α:                                                           jmp   n248_statement_begin_α
#=======================================================================================================================
# roman   s = ''
#-----------------------------------------------------------------------------------------------------------------------
n3_statement_begin_α:                                                         jmp   n4_lit_string_α
n3_statement_begin_β:                                                         jmp   n7_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx290_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n5_assign_α
.Lx290_0:               .quad            .Lx290_0_s
.Lx290_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n5_assign_α:            mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax             # s
                        mov              qword ptr [r9 + 40], rdx;            jmp   n6_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:     add              rsp, 16;                             jmp   n7_statement_begin_α
#=======================================================================================================================
#         v = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_begin_α:                                                         jmp   n8_lit_integer_α
n7_statement_begin_β:                                                         jmp   n12_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx296_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n9_call_α
.Lx296_0:               .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n9_call_α:              sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd298:            .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd298]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx297_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n7_statement_begin_β
.Lx297_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n10_assign_α
n9_call_β:              add              rsp, 16
                        add              rsp, 16;                             jmp   n7_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n10_assign_α:           mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # v
                        mov              qword ptr [r9 + 56], rdx;            jmp   n11_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:    add              rsp, 32;                             jmp   n12_statement_begin_α
#=======================================================================================================================
#         v<1> = 1000
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:                                                        jmp   n13_var_α
n12_statement_begin_β:                                                        jmp   n19_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n13_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # v
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n14_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx305_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n15_subscript_α
n14_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n12_statement_begin_β
.Lx305_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n15_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx306_240
                        add              rsp, 16;                             jmp   n14_lit_integer_β
.Lx306_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n16_lit_integer_α
n15_subscript_β:        add              rsp, 16;                             jmp   n14_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx307_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n17_assign_var_α
n16_lit_integer_β:      add              rsp, 16;                             jmp   n15_subscript_β
.Lx307_0:               .quad            1000
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx308_240
                        add              rsp, 16;                             jmp   n16_lit_integer_β
.Lx308_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n18_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_end_α:    add              rsp, 80;                             jmp   n19_statement_begin_α
#=======================================================================================================================
#         v<2> = 900
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_begin_α:                                                        jmp   n20_var_α
n19_statement_begin_β:                                                        jmp   n26_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n20_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # v
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n21_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx314_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n22_subscript_α
n21_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n19_statement_begin_β
.Lx314_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n22_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx315_240
                        add              rsp, 16;                             jmp   n21_lit_integer_β
.Lx315_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n23_lit_integer_α
n22_subscript_β:        add              rsp, 16;                             jmp   n21_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx316_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n24_assign_var_α
n23_lit_integer_β:      add              rsp, 16;                             jmp   n22_subscript_β
.Lx316_0:               .quad            900
#-----------------------------------------------------------------------------------------------------------------------
n24_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx317_240
                        add              rsp, 16;                             jmp   n23_lit_integer_β
.Lx317_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n25_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_end_α:    add              rsp, 80;                             jmp   n26_statement_begin_α
#=======================================================================================================================
#         v<3> = 500
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_begin_α:                                                        jmp   n27_var_α
n26_statement_begin_β:                                                        jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n27_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # v
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n28_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n28_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx323_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n29_subscript_α
n28_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n26_statement_begin_β
.Lx323_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n29_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx324_240
                        add              rsp, 16;                             jmp   n28_lit_integer_β
.Lx324_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n30_lit_integer_α
n29_subscript_β:        add              rsp, 16;                             jmp   n28_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx325_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n31_assign_var_α
n30_lit_integer_β:      add              rsp, 16;                             jmp   n29_subscript_β
.Lx325_0:               .quad            500
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx326_240
                        add              rsp, 16;                             jmp   n30_lit_integer_β
.Lx326_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n32_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:    add              rsp, 80;                             jmp   n33_statement_begin_α
#=======================================================================================================================
#         v<4> = 400
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:                                                        jmp   n34_var_α
n33_statement_begin_β:                                                        jmp   n40_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n34_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # v
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n35_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx332_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n36_subscript_α
n35_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n33_statement_begin_β
.Lx332_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n36_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx333_240
                        add              rsp, 16;                             jmp   n35_lit_integer_β
.Lx333_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n37_lit_integer_α
n36_subscript_β:        add              rsp, 16;                             jmp   n35_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx334_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n38_assign_var_α
n37_lit_integer_β:      add              rsp, 16;                             jmp   n36_subscript_β
.Lx334_0:               .quad            400
#-----------------------------------------------------------------------------------------------------------------------
n38_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx335_240
                        add              rsp, 16;                             jmp   n37_lit_integer_β
.Lx335_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n39_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_end_α:    add              rsp, 80;                             jmp   n40_statement_begin_α
#=======================================================================================================================
#         v<5> = 100
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_begin_α:                                                        jmp   n41_var_α
n40_statement_begin_β:                                                        jmp   n47_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n41_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # v
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n42_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx341_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n43_subscript_α
n42_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n40_statement_begin_β
.Lx341_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n43_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx342_240
                        add              rsp, 16;                             jmp   n42_lit_integer_β
.Lx342_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n44_lit_integer_α
n43_subscript_β:        add              rsp, 16;                             jmp   n42_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n44_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx343_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n45_assign_var_α
n44_lit_integer_β:      add              rsp, 16;                             jmp   n43_subscript_β
.Lx343_0:               .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n45_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx344_240
                        add              rsp, 16;                             jmp   n44_lit_integer_β
.Lx344_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n46_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_end_α:    add              rsp, 80;                             jmp   n47_statement_begin_α
#=======================================================================================================================
#         v<6> = 90
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_begin_α:                                                        jmp   n48_var_α
n47_statement_begin_β:                                                        jmp   n54_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n48_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # v
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n49_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx350_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n50_subscript_α
n49_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n47_statement_begin_β
.Lx350_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n50_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx351_240
                        add              rsp, 16;                             jmp   n49_lit_integer_β
.Lx351_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n51_lit_integer_α
n50_subscript_β:        add              rsp, 16;                             jmp   n49_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n51_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx352_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n52_assign_var_α
n51_lit_integer_β:      add              rsp, 16;                             jmp   n50_subscript_β
.Lx352_0:               .quad            90
#-----------------------------------------------------------------------------------------------------------------------
n52_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx353_240
                        add              rsp, 16;                             jmp   n51_lit_integer_β
.Lx353_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n53_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_end_α:    add              rsp, 80;                             jmp   n54_statement_begin_α
#=======================================================================================================================
#         v<7> = 50
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_begin_α:                                                        jmp   n55_var_α
n54_statement_begin_β:                                                        jmp   n61_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n55_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # v
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n56_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n56_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx359_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n57_subscript_α
n56_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n54_statement_begin_β
.Lx359_0:               .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n57_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx360_240
                        add              rsp, 16;                             jmp   n56_lit_integer_β
.Lx360_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n58_lit_integer_α
n57_subscript_β:        add              rsp, 16;                             jmp   n56_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx361_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n59_assign_var_α
n58_lit_integer_β:      add              rsp, 16;                             jmp   n57_subscript_β
.Lx361_0:               .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n59_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx362_240
                        add              rsp, 16;                             jmp   n58_lit_integer_β
.Lx362_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n60_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_end_α:    add              rsp, 80;                             jmp   n61_statement_begin_α
#=======================================================================================================================
#         v<8> = 40
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_begin_α:                                                        jmp   n62_var_α
n61_statement_begin_β:                                                        jmp   n68_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n62_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # v
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n63_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n63_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx368_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n64_subscript_α
n63_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n61_statement_begin_β
.Lx368_0:               .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n64_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx369_240
                        add              rsp, 16;                             jmp   n63_lit_integer_β
.Lx369_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n65_lit_integer_α
n64_subscript_β:        add              rsp, 16;                             jmp   n63_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n65_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx370_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n66_assign_var_α
n65_lit_integer_β:      add              rsp, 16;                             jmp   n64_subscript_β
.Lx370_0:               .quad            40
#-----------------------------------------------------------------------------------------------------------------------
n66_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx371_240
                        add              rsp, 16;                             jmp   n65_lit_integer_β
.Lx371_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n67_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_end_α:    add              rsp, 80;                             jmp   n68_statement_begin_α
#=======================================================================================================================
#         v<9> = 10
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_begin_α:                                                        jmp   n69_var_α
n68_statement_begin_β:                                                        jmp   n75_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n69_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # v
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n70_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n70_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx377_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n71_subscript_α
n70_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n68_statement_begin_β
.Lx377_0:               .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n71_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx378_240
                        add              rsp, 16;                             jmp   n70_lit_integer_β
.Lx378_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n72_lit_integer_α
n71_subscript_β:        add              rsp, 16;                             jmp   n70_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n72_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx379_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n73_assign_var_α
n72_lit_integer_β:      add              rsp, 16;                             jmp   n71_subscript_β
.Lx379_0:               .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n73_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx380_240
                        add              rsp, 16;                             jmp   n72_lit_integer_β
.Lx380_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n74_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_end_α:    add              rsp, 80;                             jmp   n75_statement_begin_α
#=======================================================================================================================
#         v<10> = 9
#-----------------------------------------------------------------------------------------------------------------------
n75_statement_begin_α:                                                        jmp   n76_var_α
n75_statement_begin_β:                                                        jmp   n82_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n76_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # v
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n77_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n77_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx386_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n78_subscript_α
n77_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n75_statement_begin_β
.Lx386_0:               .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n78_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx387_240
                        add              rsp, 16;                             jmp   n77_lit_integer_β
.Lx387_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n79_lit_integer_α
n78_subscript_β:        add              rsp, 16;                             jmp   n77_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx388_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n80_assign_var_α
n79_lit_integer_β:      add              rsp, 16;                             jmp   n78_subscript_β
.Lx388_0:               .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n80_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx389_240
                        add              rsp, 16;                             jmp   n79_lit_integer_β
.Lx389_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n81_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_end_α:    add              rsp, 80;                             jmp   n82_statement_begin_α
#=======================================================================================================================
#         v<11> = 5
#-----------------------------------------------------------------------------------------------------------------------
n82_statement_begin_α:                                                        jmp   n83_var_α
n82_statement_begin_β:                                                        jmp   n89_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n83_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # v
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n84_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n84_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx395_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n85_subscript_α
n84_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n82_statement_begin_β
.Lx395_0:               .quad            11
#-----------------------------------------------------------------------------------------------------------------------
n85_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx396_240
                        add              rsp, 16;                             jmp   n84_lit_integer_β
.Lx396_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n86_lit_integer_α
n85_subscript_β:        add              rsp, 16;                             jmp   n84_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx397_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n87_assign_var_α
n86_lit_integer_β:      add              rsp, 16;                             jmp   n85_subscript_β
.Lx397_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n87_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx398_240
                        add              rsp, 16;                             jmp   n86_lit_integer_β
.Lx398_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n88_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_end_α:    add              rsp, 80;                             jmp   n89_statement_begin_α
#=======================================================================================================================
#         v<12> = 4
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_begin_α:                                                        jmp   n90_var_α
n89_statement_begin_β:                                                        jmp   n96_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n90_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # v
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n91_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n91_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx404_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n92_subscript_α
n91_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n89_statement_begin_β
.Lx404_0:               .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n92_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx405_240
                        add              rsp, 16;                             jmp   n91_lit_integer_β
.Lx405_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n93_lit_integer_α
n92_subscript_β:        add              rsp, 16;                             jmp   n91_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n93_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx406_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n94_assign_var_α
n93_lit_integer_β:      add              rsp, 16;                             jmp   n92_subscript_β
.Lx406_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n94_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx407_240
                        add              rsp, 16;                             jmp   n93_lit_integer_β
.Lx407_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n95_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n95_statement_end_α:    add              rsp, 80;                             jmp   n96_statement_begin_α
#=======================================================================================================================
#         v<13> = 1
#-----------------------------------------------------------------------------------------------------------------------
n96_statement_begin_α:                                                        jmp   n97_var_α
n96_statement_begin_β:                                                        jmp   n103_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n97_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # v
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n98_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n98_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx413_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n99_subscript_α
n98_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n96_statement_begin_β
.Lx413_0:               .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n99_subscript_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx414_240
                        add              rsp, 16;                             jmp   n98_lit_integer_β
.Lx414_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n100_lit_integer_α
n99_subscript_β:        add              rsp, 16;                             jmp   n98_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n100_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx415_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n101_assign_var_α
n100_lit_integer_β:     add              rsp, 16;                             jmp   n99_subscript_β
.Lx415_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n101_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx416_240
                        add              rsp, 16;                             jmp   n100_lit_integer_β
.Lx416_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n102_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:   add              rsp, 80;                             jmp   n103_statement_begin_α
#=======================================================================================================================
#         r = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_begin_α:                                                       jmp   n104_lit_integer_α
n103_statement_begin_β:                                                       jmp   n108_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n104_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx421_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n105_call_α
.Lx421_0:               .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n105_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd423:            .string          "ARRAY"
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
                        cmp              al, 104;                             jne   .Lx422_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n103_statement_begin_β
.Lx422_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n106_assign_α
n105_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n103_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n106_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax             # r
                        mov              qword ptr [r9 + 72], rdx;            jmp   n107_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n107_statement_end_α:   add              rsp, 32;                             jmp   n108_statement_begin_α
#=======================================================================================================================
#         r<1> = 'M'
#-----------------------------------------------------------------------------------------------------------------------
n108_statement_begin_α:                                                       jmp   n109_var_α
n108_statement_begin_β:                                                       jmp   n115_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n109_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # r
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n110_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n110_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx430_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n111_subscript_α
n110_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n108_statement_begin_β
.Lx430_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n111_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx431_240
                        add              rsp, 16;                             jmp   n110_lit_integer_β
.Lx431_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n112_lit_string_α
n111_subscript_β:       add              rsp, 16;                             jmp   n110_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n112_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx432_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n113_assign_var_α
n112_lit_string_β:      add              rsp, 16;                             jmp   n111_subscript_β
.Lx432_0:               .quad            .Lx432_0_s
.Lx432_0_s:             .string          "M"
#-----------------------------------------------------------------------------------------------------------------------
n113_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx433_240
                        add              rsp, 16;                             jmp   n112_lit_string_β
.Lx433_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n114_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n114_statement_end_α:   add              rsp, 80;                             jmp   n115_statement_begin_α
#=======================================================================================================================
#         r<2> = 'CM'
#-----------------------------------------------------------------------------------------------------------------------
n115_statement_begin_α:                                                       jmp   n116_var_α
n115_statement_begin_β:                                                       jmp   n122_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n116_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # r
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n117_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n117_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx439_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n118_subscript_α
n117_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n115_statement_begin_β
.Lx439_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n118_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx440_240
                        add              rsp, 16;                             jmp   n117_lit_integer_β
.Lx440_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n119_lit_string_α
n118_subscript_β:       add              rsp, 16;                             jmp   n117_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n119_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx441_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n120_assign_var_α
n119_lit_string_β:      add              rsp, 16;                             jmp   n118_subscript_β
.Lx441_0:               .quad            .Lx441_0_s
.Lx441_0_s:             .string          "CM"
#-----------------------------------------------------------------------------------------------------------------------
n120_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx442_240
                        add              rsp, 16;                             jmp   n119_lit_string_β
.Lx442_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n121_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n121_statement_end_α:   add              rsp, 80;                             jmp   n122_statement_begin_α
#=======================================================================================================================
#         r<3> = 'D'
#-----------------------------------------------------------------------------------------------------------------------
n122_statement_begin_α:                                                       jmp   n123_var_α
n122_statement_begin_β:                                                       jmp   n129_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n123_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # r
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n124_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n124_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx448_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n125_subscript_α
n124_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n122_statement_begin_β
.Lx448_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n125_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx449_240
                        add              rsp, 16;                             jmp   n124_lit_integer_β
.Lx449_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n126_lit_string_α
n125_subscript_β:       add              rsp, 16;                             jmp   n124_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n126_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx450_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n127_assign_var_α
n126_lit_string_β:      add              rsp, 16;                             jmp   n125_subscript_β
.Lx450_0:               .quad            .Lx450_0_s
.Lx450_0_s:             .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n127_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx451_240
                        add              rsp, 16;                             jmp   n126_lit_string_β
.Lx451_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n128_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n128_statement_end_α:   add              rsp, 80;                             jmp   n129_statement_begin_α
#=======================================================================================================================
#         r<4> = 'CD'
#-----------------------------------------------------------------------------------------------------------------------
n129_statement_begin_α:                                                       jmp   n130_var_α
n129_statement_begin_β:                                                       jmp   n136_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n130_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # r
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n131_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n131_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx457_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n132_subscript_α
n131_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n129_statement_begin_β
.Lx457_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n132_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx458_240
                        add              rsp, 16;                             jmp   n131_lit_integer_β
.Lx458_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n133_lit_string_α
n132_subscript_β:       add              rsp, 16;                             jmp   n131_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n133_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx459_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n134_assign_var_α
n133_lit_string_β:      add              rsp, 16;                             jmp   n132_subscript_β
.Lx459_0:               .quad            .Lx459_0_s
.Lx459_0_s:             .string          "CD"
#-----------------------------------------------------------------------------------------------------------------------
n134_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx460_240
                        add              rsp, 16;                             jmp   n133_lit_string_β
.Lx460_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n135_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n135_statement_end_α:   add              rsp, 80;                             jmp   n136_statement_begin_α
#=======================================================================================================================
#         r<5> = 'C'
#-----------------------------------------------------------------------------------------------------------------------
n136_statement_begin_α:                                                       jmp   n137_var_α
n136_statement_begin_β:                                                       jmp   n143_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n137_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # r
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n138_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n138_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx466_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n139_subscript_α
n138_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n136_statement_begin_β
.Lx466_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n139_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx467_240
                        add              rsp, 16;                             jmp   n138_lit_integer_β
.Lx467_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n140_lit_string_α
n139_subscript_β:       add              rsp, 16;                             jmp   n138_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n140_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx468_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n141_assign_var_α
n140_lit_string_β:      add              rsp, 16;                             jmp   n139_subscript_β
.Lx468_0:               .quad            .Lx468_0_s
.Lx468_0_s:             .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n141_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx469_240
                        add              rsp, 16;                             jmp   n140_lit_string_β
.Lx469_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n142_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n142_statement_end_α:   add              rsp, 80;                             jmp   n143_statement_begin_α
#=======================================================================================================================
#         r<6> = 'XC'
#-----------------------------------------------------------------------------------------------------------------------
n143_statement_begin_α:                                                       jmp   n144_var_α
n143_statement_begin_β:                                                       jmp   n150_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n144_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # r
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n145_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n145_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx475_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n146_subscript_α
n145_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n143_statement_begin_β
.Lx475_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n146_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx476_240
                        add              rsp, 16;                             jmp   n145_lit_integer_β
.Lx476_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n147_lit_string_α
n146_subscript_β:       add              rsp, 16;                             jmp   n145_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n147_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx477_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n148_assign_var_α
n147_lit_string_β:      add              rsp, 16;                             jmp   n146_subscript_β
.Lx477_0:               .quad            .Lx477_0_s
.Lx477_0_s:             .string          "XC"
#-----------------------------------------------------------------------------------------------------------------------
n148_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx478_240
                        add              rsp, 16;                             jmp   n147_lit_string_β
.Lx478_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n149_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n149_statement_end_α:   add              rsp, 80;                             jmp   n150_statement_begin_α
#=======================================================================================================================
#         r<7> = 'L'
#-----------------------------------------------------------------------------------------------------------------------
n150_statement_begin_α:                                                       jmp   n151_var_α
n150_statement_begin_β:                                                       jmp   n157_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n151_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # r
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n152_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n152_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx484_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n153_subscript_α
n152_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n150_statement_begin_β
.Lx484_0:               .quad            7
#-----------------------------------------------------------------------------------------------------------------------
n153_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx485_240
                        add              rsp, 16;                             jmp   n152_lit_integer_β
.Lx485_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n154_lit_string_α
n153_subscript_β:       add              rsp, 16;                             jmp   n152_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n154_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx486_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n155_assign_var_α
n154_lit_string_β:      add              rsp, 16;                             jmp   n153_subscript_β
.Lx486_0:               .quad            .Lx486_0_s
.Lx486_0_s:             .string          "L"
#-----------------------------------------------------------------------------------------------------------------------
n155_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx487_240
                        add              rsp, 16;                             jmp   n154_lit_string_β
.Lx487_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n156_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n156_statement_end_α:   add              rsp, 80;                             jmp   n157_statement_begin_α
#=======================================================================================================================
#         r<8> = 'XL'
#-----------------------------------------------------------------------------------------------------------------------
n157_statement_begin_α:                                                       jmp   n158_var_α
n157_statement_begin_β:                                                       jmp   n164_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n158_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # r
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n159_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n159_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx493_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n160_subscript_α
n159_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n157_statement_begin_β
.Lx493_0:               .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n160_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx494_240
                        add              rsp, 16;                             jmp   n159_lit_integer_β
.Lx494_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n161_lit_string_α
n160_subscript_β:       add              rsp, 16;                             jmp   n159_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n161_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx495_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n162_assign_var_α
n161_lit_string_β:      add              rsp, 16;                             jmp   n160_subscript_β
.Lx495_0:               .quad            .Lx495_0_s
.Lx495_0_s:             .string          "XL"
#-----------------------------------------------------------------------------------------------------------------------
n162_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx496_240
                        add              rsp, 16;                             jmp   n161_lit_string_β
.Lx496_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n163_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n163_statement_end_α:   add              rsp, 80;                             jmp   n164_statement_begin_α
#=======================================================================================================================
#         r<9> = 'X'
#-----------------------------------------------------------------------------------------------------------------------
n164_statement_begin_α:                                                       jmp   n165_var_α
n164_statement_begin_β:                                                       jmp   n171_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n165_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # r
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n166_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n166_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx502_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n167_subscript_α
n166_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n164_statement_begin_β
.Lx502_0:               .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n167_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx503_240
                        add              rsp, 16;                             jmp   n166_lit_integer_β
.Lx503_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n168_lit_string_α
n167_subscript_β:       add              rsp, 16;                             jmp   n166_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n168_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx504_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n169_assign_var_α
n168_lit_string_β:      add              rsp, 16;                             jmp   n167_subscript_β
.Lx504_0:               .quad            .Lx504_0_s
.Lx504_0_s:             .string          "X"
#-----------------------------------------------------------------------------------------------------------------------
n169_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx505_240
                        add              rsp, 16;                             jmp   n168_lit_string_β
.Lx505_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n170_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n170_statement_end_α:   add              rsp, 80;                             jmp   n171_statement_begin_α
#=======================================================================================================================
#         r<10> = 'IX'
#-----------------------------------------------------------------------------------------------------------------------
n171_statement_begin_α:                                                       jmp   n172_var_α
n171_statement_begin_β:                                                       jmp   n178_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n172_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # r
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n173_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n173_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx511_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n174_subscript_α
n173_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n171_statement_begin_β
.Lx511_0:               .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n174_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx512_240
                        add              rsp, 16;                             jmp   n173_lit_integer_β
.Lx512_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n175_lit_string_α
n174_subscript_β:       add              rsp, 16;                             jmp   n173_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n175_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx513_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n176_assign_var_α
n175_lit_string_β:      add              rsp, 16;                             jmp   n174_subscript_β
.Lx513_0:               .quad            .Lx513_0_s
.Lx513_0_s:             .string          "IX"
#-----------------------------------------------------------------------------------------------------------------------
n176_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx514_240
                        add              rsp, 16;                             jmp   n175_lit_string_β
.Lx514_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n177_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n177_statement_end_α:   add              rsp, 80;                             jmp   n178_statement_begin_α
#=======================================================================================================================
#         r<11> = 'V'
#-----------------------------------------------------------------------------------------------------------------------
n178_statement_begin_α:                                                       jmp   n179_var_α
n178_statement_begin_β:                                                       jmp   n185_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n179_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # r
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n180_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n180_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx520_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n181_subscript_α
n180_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n178_statement_begin_β
.Lx520_0:               .quad            11
#-----------------------------------------------------------------------------------------------------------------------
n181_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx521_240
                        add              rsp, 16;                             jmp   n180_lit_integer_β
.Lx521_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n182_lit_string_α
n181_subscript_β:       add              rsp, 16;                             jmp   n180_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n182_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx522_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n183_assign_var_α
n182_lit_string_β:      add              rsp, 16;                             jmp   n181_subscript_β
.Lx522_0:               .quad            .Lx522_0_s
.Lx522_0_s:             .string          "V"
#-----------------------------------------------------------------------------------------------------------------------
n183_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx523_240
                        add              rsp, 16;                             jmp   n182_lit_string_β
.Lx523_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n184_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n184_statement_end_α:   add              rsp, 80;                             jmp   n185_statement_begin_α
#=======================================================================================================================
#         r<12> = 'IV'
#-----------------------------------------------------------------------------------------------------------------------
n185_statement_begin_α:                                                       jmp   n186_var_α
n185_statement_begin_β:                                                       jmp   n192_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n186_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # r
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n187_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n187_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx529_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n188_subscript_α
n187_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n185_statement_begin_β
.Lx529_0:               .quad            12
#-----------------------------------------------------------------------------------------------------------------------
n188_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx530_240
                        add              rsp, 16;                             jmp   n187_lit_integer_β
.Lx530_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n189_lit_string_α
n188_subscript_β:       add              rsp, 16;                             jmp   n187_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n189_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx531_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n190_assign_var_α
n189_lit_string_β:      add              rsp, 16;                             jmp   n188_subscript_β
.Lx531_0:               .quad            .Lx531_0_s
.Lx531_0_s:             .string          "IV"
#-----------------------------------------------------------------------------------------------------------------------
n190_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx532_240
                        add              rsp, 16;                             jmp   n189_lit_string_β
.Lx532_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n191_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n191_statement_end_α:   add              rsp, 80;                             jmp   n192_statement_begin_α
#=======================================================================================================================
#         r<13> = 'I'
#-----------------------------------------------------------------------------------------------------------------------
n192_statement_begin_α:                                                       jmp   n193_var_α
n192_statement_begin_β:                                                       jmp   n199_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n193_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # r
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n194_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n194_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx538_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n195_subscript_α
n194_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n192_statement_begin_β
.Lx538_0:               .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n195_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx539_240
                        add              rsp, 16;                             jmp   n194_lit_integer_β
.Lx539_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n196_lit_string_α
n195_subscript_β:       add              rsp, 16;                             jmp   n194_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n196_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx540_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n197_assign_var_α
n196_lit_string_β:      add              rsp, 16;                             jmp   n195_subscript_β
.Lx540_0:               .quad            .Lx540_0_s
.Lx540_0_s:             .string          "I"
#-----------------------------------------------------------------------------------------------------------------------
n197_assign_var_α:      sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # subscript
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx541_240
                        add              rsp, 16;                             jmp   n196_lit_string_β
.Lx541_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n198_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n198_statement_end_α:   add              rsp, 80;                             jmp   n199_statement_begin_α
#=======================================================================================================================
#         i = 1
#-----------------------------------------------------------------------------------------------------------------------
n199_statement_begin_α:                                                       jmp   n200_lit_integer_α
n199_statement_begin_β:                                                       jmp   n203_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n200_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx546_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n201_assign_α
.Lx546_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n201_assign_α:          mov              rax, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax             # i
                        mov              qword ptr [r9 + 88], rdx;            jmp   n202_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n202_statement_end_α:   add              rsp, 16;                             jmp   n203_statement_begin_α
#=======================================================================================================================
# RLOOP   GT(n, 0)                                                       :F(RDONE)
#-----------------------------------------------------------------------------------------------------------------------
n203_statement_begin_α:                                                       jmp   n204_var_α
n203_statement_begin_β:                                                       jmp   n244_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n204_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n205_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n205_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx553_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n206_coerce_numeric_α
n205_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n203_statement_begin_β
.Lx553_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n206_coerce_numeric_α:  sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              al, 5;                               je    .Lx555_1
                        cmp              al, 3;                               jne   .Lx555_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx555_0
.Lx555_1:               mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n207_coerce_numeric_α
.Lx555_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 111
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n207_coerce_numeric_α
n206_coerce_numeric_β:  add              rsp, 16;                             jmp   n205_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n207_coerce_numeric_α:  sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx557_1
                        cmp              al, 3;                               jne   .Lx557_0
                        mov              eax, dword ptr [rsp + 48]            # var
                        cmp              al, 3;                               jne   .Lx557_0
.Lx557_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n208_cmp_test_α
.Lx557_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 112
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n208_cmp_test_α
n207_coerce_numeric_β:  add              rsp, 16;                             jmp   n206_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n208_cmp_test_α:        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx559_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jg    .Lx559_239
                        add              rsp, 16;                             jmp   n207_coerce_numeric_β
.Lx559_239:                                                                   jmp   n209_statement_end_α
.Lx559_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_cmp_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            jg    .Lx559_240
                        add              rsp, 16;                             jmp   n207_coerce_numeric_β
.Lx559_240:                                                                   jmp   n209_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n209_statement_end_α:   add              rsp, 80;                             jmp   n210_statement_begin_α
#=======================================================================================================================
#         GE(n, v<i>)                                                    :F(RNEXT)
#-----------------------------------------------------------------------------------------------------------------------
n210_statement_begin_α:                                                       jmp   n211_var_α
n210_statement_begin_β:                                                       jmp   n238_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n211_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n212_var_α
#-----------------------------------------------------------------------------------------------------------------------
n212_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # v
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n213_var_α
n212_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n210_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n213_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 80]             # i
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n214_subscript_α
n213_var_β:             add              rsp, 16;                             jmp   n212_var_β
#-----------------------------------------------------------------------------------------------------------------------
n214_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx567_240
                        add              rsp, 16;                             jmp   n213_var_β
.Lx567_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n215_deref_α
n214_subscript_β:       add              rsp, 16;                             jmp   n213_var_β
#-----------------------------------------------------------------------------------------------------------------------
n215_deref_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx568_240
                        add              rsp, 16;                             jmp   n214_subscript_β
.Lx568_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n216_coerce_numeric_α
n215_deref_β:           add              rsp, 16;                             jmp   n214_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n216_coerce_numeric_α:  sub              rsp, 16
                        mov              eax, dword ptr [rsp + 80]            # var
                        cmp              al, 5;                               je    .Lx570_1
                        cmp              al, 3;                               jne   .Lx570_0
                        mov              eax, dword ptr [rsp + 16]            # deref
                        cmp              al, 3;                               jne   .Lx570_0
.Lx570_1:               mov              rax, qword ptr [rsp + 80]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 88]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n217_coerce_numeric_α
.Lx570_0:               lea              rdi, [rsp + 80]
                        lea              rsi, [rsp + 16]                      # deref
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 109
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n217_coerce_numeric_α
n216_coerce_numeric_β:  add              rsp, 16;                             jmp   n215_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n217_coerce_numeric_α:  sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # deref
                        cmp              al, 5;                               je    .Lx572_1
                        cmp              al, 3;                               jne   .Lx572_0
                        mov              eax, dword ptr [rsp + 96]            # var
                        cmp              al, 3;                               jne   .Lx572_0
.Lx572_1:               mov              rax, qword ptr [rsp + 32]            # deref
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # deref
                        mov              qword ptr [rsp + 8], rax;            jmp   n218_cmp_test_α
.Lx572_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 96]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 110
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n218_cmp_test_α
n217_coerce_numeric_β:  add              rsp, 16;                             jmp   n216_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n218_cmp_test_α:        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx574_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jge   .Lx574_239
                        add              rsp, 16;                             jmp   n217_coerce_numeric_β
.Lx574_239:                                                                   jmp   n219_statement_end_α
.Lx574_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_cmp_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            jns   .Lx574_240
                        add              rsp, 16;                             jmp   n217_coerce_numeric_β
.Lx574_240:                                                                   jmp   n219_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n219_statement_end_α:   add              rsp, 128;                            jmp   n220_statement_begin_α
#=======================================================================================================================
#         s = s r<i>
#-----------------------------------------------------------------------------------------------------------------------
n220_statement_begin_α:                                                       jmp   n221_var_α
n220_statement_begin_β:                                                       jmp   n229_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n221_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]             # s
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n222_var_α
#-----------------------------------------------------------------------------------------------------------------------
n222_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # r
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n223_var_α
n222_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n220_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n223_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 80]             # i
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n224_subscript_α
n223_var_β:             add              rsp, 16;                             jmp   n222_var_β
#-----------------------------------------------------------------------------------------------------------------------
n224_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx582_240
                        add              rsp, 16;                             jmp   n223_var_β
.Lx582_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n225_deref_α
n224_subscript_β:       add              rsp, 16;                             jmp   n223_var_β
#-----------------------------------------------------------------------------------------------------------------------
n225_deref_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx583_240
                        add              rsp, 16;                             jmp   n224_subscript_β
.Lx583_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n226_binop_α
n225_deref_β:           add              rsp, 16;                             jmp   n224_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n226_binop_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]            # var
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]            # deref
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n227_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n227_assign_α:          mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax             # s
                        mov              qword ptr [r9 + 40], rdx;            jmp   n228_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n228_statement_end_α:   add              rsp, 96;                             jmp   n229_statement_begin_α
#=======================================================================================================================
#         n = n - v<i>                                                   :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n229_statement_begin_α:                                                       jmp   n230_var_α
n229_statement_begin_β:                                                       jmp   n203_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n230_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n231_var_α
#-----------------------------------------------------------------------------------------------------------------------
n231_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]             # v
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n232_var_α
n231_var_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n229_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n232_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 80]             # i
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n233_subscript_α
n232_var_β:             add              rsp, 16;                             jmp   n231_var_β
#-----------------------------------------------------------------------------------------------------------------------
n233_subscript_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_subscript_var_container_only@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx593_240
                        add              rsp, 16;                             jmp   n232_var_β
.Lx593_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n234_deref_α
n233_subscript_β:       add              rsp, 16;                             jmp   n232_var_β
#-----------------------------------------------------------------------------------------------------------------------
n234_deref_α:           sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]            # subscript
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx594_240
                        add              rsp, 16;                             jmp   n233_subscript_β
.Lx594_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n235_binop_α
n234_deref_β:           add              rsp, 16;                             jmp   n233_subscript_β
#-----------------------------------------------------------------------------------------------------------------------
n235_binop_α:           sub              rsp, 16
                        mov              eax, dword ptr [rsp + 80]            # var
                        mov              ecx, dword ptr [rsp + 16]            # deref
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx595_2
                        mov              rax, qword ptr [rsp + 88]            # var
                        mov              rdx, qword ptr [rsp + 24]            # deref
                        sub              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx595_7
.Lx595_2:               and              edx, 1;                              jz    .Lx595_0
                        mov              rsi, qword ptr [rsp + 88]            # var
                        mov              rdi, qword ptr [rsp + 24]            # deref
                        cmp              al, 5;                               je    .Lx595_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx595_4
.Lx595_3:               movq             xmm0, rsi
.Lx595_4:               cmp              cl, 5;                               je    .Lx595_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx595_6
.Lx595_5:               movq             xmm1, rdi
.Lx595_6:               subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx595_7:                                                                     jmp   n236_assign_α
.Lx595_0:               mov              rdi, qword ptr [rsp + 80]            # var
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]            # deref
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_sub@PLT
                        cmp              al, 104;                             jne   .Lx595_240
                        add              rsp, 16;                             jmp   n234_deref_β
.Lx595_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n236_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n236_assign_α:          mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax             # n
                        mov              qword ptr [r9 + 24], rdx;            jmp   n237_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n237_statement_end_α:   add              rsp, 96;                             jmp   n203_statement_begin_α
#=======================================================================================================================
# RNEXT   i = i + 1                                                      :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n238_statement_begin_α:                                                       jmp   n239_var_α
n238_statement_begin_β:                                                       jmp   n203_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n239_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 80]             # i
                        mov              rdx, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n240_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n240_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx602_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n241_binop_α
n240_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n238_statement_begin_β
.Lx602_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n241_binop_α:           sub              rsp, 16
                        mov              ecx, dword ptr [rsp + 32]            # var
                        mov              rax, qword ptr [rsp + 40]
                        cmp              cl, 3;                               jne   .Lx603_2
                        add              rax, 1
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx603_7
.Lx603_2:               mov              eax, ecx
                        mov              edx, ecx
                        and              edx, 1;                              jz    .Lx603_0
                        mov              rsi, qword ptr [rsp + 40]            # var
                        mov              rdi, 1
                        cmp              al, 5;                               je    .Lx603_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx603_4
.Lx603_3:               movq             xmm0, rsi
.Lx603_4:               cvtsi2sd         xmm1, rdi
                        addsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx603_7:                                                                     jmp   n242_assign_α
.Lx603_0:               mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_add@PLT
                        cmp              al, 104;                             jne   .Lx603_240
                        add              rsp, 16;                             jmp   n240_lit_integer_β
.Lx603_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n242_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n242_assign_α:          mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax             # i
                        mov              qword ptr [r9 + 88], rdx;            jmp   n243_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n243_statement_end_α:   add              rsp, 48;                             jmp   n203_statement_begin_α
#=======================================================================================================================
# RDONE   roman = s                                                      :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n244_statement_begin_α:                                                       jmp   n245_var_α
n244_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n245_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]             # s
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n246_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n246_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # roman
                        mov              qword ptr [r9 + 8], rdx;             jmp   n247_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n247_statement_end_α:   add              rsp, 16;                             jmp   RETURN
#=======================================================================================================================
# roman_end  <stmt 38, line 40: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n248_statement_begin_α:                                                       jmp   n249_statement_end_α
n248_statement_begin_β:                                                       jmp   n250_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n249_statement_end_α:                                                         jmp   n250_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(1)
#-----------------------------------------------------------------------------------------------------------------------
n250_statement_begin_α:                                                       jmp   n251_lit_integer_α
n250_statement_begin_β:                                                       jmp   n255_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n251_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx619_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n252_call_α
.Lx619_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n252_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig621z]
                        lea              rax, [rip + roman_α];                jmp   rax
.Lsig621z:              .quad            1
                        .quad            .Lx621_2
                        .quad            .Lx621_2
                        .quad            16
.Lx621_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx621_29
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
.Lx621_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx621_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n250_statement_begin_β
.Lx621_240:                                                                   jmp   n253_assign_α
n252_call_β:                                                                  jmp   n250_statement_begin_β
.Lx621_0:               .quad            .Lx621_0_s
.Lx621_0_s:             .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n253_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx622_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n254_statement_end_α
.Lx622_0:               .quad            .Lx622_0_s
.Lx622_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n254_statement_end_α:   add              rsp, 32;                             jmp   n255_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(4)
#-----------------------------------------------------------------------------------------------------------------------
n255_statement_begin_α:                                                       jmp   n256_lit_integer_α
n255_statement_begin_β:                                                       jmp   n260_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n256_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx627_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n257_call_α
.Lx627_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n257_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig629z]
                        lea              rax, [rip + roman_α];                jmp   rax
.Lsig629z:              .quad            1
                        .quad            .Lx629_2
                        .quad            .Lx629_2
                        .quad            16
.Lx629_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx629_29
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
.Lx629_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx629_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n255_statement_begin_β
.Lx629_240:                                                                   jmp   n258_assign_α
n257_call_β:                                                                  jmp   n255_statement_begin_β
.Lx629_0:               .quad            .Lx629_0_s
.Lx629_0_s:             .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n258_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx630_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n259_statement_end_α
.Lx630_0:               .quad            .Lx630_0_s
.Lx630_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n259_statement_end_α:   add              rsp, 32;                             jmp   n260_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(9)
#-----------------------------------------------------------------------------------------------------------------------
n260_statement_begin_α:                                                       jmp   n261_lit_integer_α
n260_statement_begin_β:                                                       jmp   n265_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n261_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx635_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n262_call_α
.Lx635_0:               .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n262_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig637z]
                        lea              rax, [rip + roman_α];                jmp   rax
.Lsig637z:              .quad            1
                        .quad            .Lx637_2
                        .quad            .Lx637_2
                        .quad            16
.Lx637_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx637_29
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
.Lx637_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx637_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n260_statement_begin_β
.Lx637_240:                                                                   jmp   n263_assign_α
n262_call_β:                                                                  jmp   n260_statement_begin_β
.Lx637_0:               .quad            .Lx637_0_s
.Lx637_0_s:             .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n263_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx638_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n264_statement_end_α
.Lx638_0:               .quad            .Lx638_0_s
.Lx638_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n264_statement_end_α:   add              rsp, 32;                             jmp   n265_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(42)
#-----------------------------------------------------------------------------------------------------------------------
n265_statement_begin_α:                                                       jmp   n266_lit_integer_α
n265_statement_begin_β:                                                       jmp   n270_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n266_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx643_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n267_call_α
.Lx643_0:               .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n267_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig645z]
                        lea              rax, [rip + roman_α];                jmp   rax
.Lsig645z:              .quad            1
                        .quad            .Lx645_2
                        .quad            .Lx645_2
                        .quad            16
.Lx645_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx645_29
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
.Lx645_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx645_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n265_statement_begin_β
.Lx645_240:                                                                   jmp   n268_assign_α
n267_call_β:                                                                  jmp   n265_statement_begin_β
.Lx645_0:               .quad            .Lx645_0_s
.Lx645_0_s:             .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n268_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx646_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n269_statement_end_α
.Lx646_0:               .quad            .Lx646_0_s
.Lx646_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n269_statement_end_α:   add              rsp, 32;                             jmp   n270_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(1999)
#-----------------------------------------------------------------------------------------------------------------------
n270_statement_begin_α:                                                       jmp   n271_lit_integer_α
n270_statement_begin_β:                                                       jmp   n275_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n271_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx651_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n272_call_α
.Lx651_0:               .quad            1999
#-----------------------------------------------------------------------------------------------------------------------
n272_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig653z]
                        lea              rax, [rip + roman_α];                jmp   rax
.Lsig653z:              .quad            1
                        .quad            .Lx653_2
                        .quad            .Lx653_2
                        .quad            16
.Lx653_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx653_29
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
.Lx653_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx653_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n270_statement_begin_β
.Lx653_240:                                                                   jmp   n273_assign_α
n272_call_β:                                                                  jmp   n270_statement_begin_β
.Lx653_0:               .quad            .Lx653_0_s
.Lx653_0_s:             .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n273_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx654_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n274_statement_end_α
.Lx654_0:               .quad            .Lx654_0_s
.Lx654_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n274_statement_end_α:   add              rsp, 32;                             jmp   n275_statement_begin_α
#=======================================================================================================================
#         OUTPUT = roman(2024)
#-----------------------------------------------------------------------------------------------------------------------
n275_statement_begin_α:                                                       jmp   n276_lit_integer_α
n275_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n276_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx659_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n277_call_α
.Lx659_0:               .quad            2024
#-----------------------------------------------------------------------------------------------------------------------
n277_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig661z]
                        lea              rax, [rip + roman_α];                jmp   rax
.Lsig661z:              .quad            1
                        .quad            .Lx661_2
                        .quad            .Lx661_2
                        .quad            16
.Lx661_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx661_29
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
.Lx661_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx661_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n275_statement_begin_β
.Lx661_240:                                                                   jmp   n278_assign_α
n277_call_β:                                                                  jmp   n275_statement_begin_β
.Lx661_0:               .quad            .Lx661_0_s
.Lx661_0_s:             .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n278_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx662_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n279_statement_end_α
.Lx662_0:               .quad            .Lx662_0_s
.Lx662_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n279_statement_end_α:   add              rsp, 32;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 pop              rcx
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
.Lseala1:               .string          "roman"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            roman_α
                        lea              rdi, [rip + .Lseala1]
                        mov              rsi, qword ptr [rip + roman_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        add              rsp, 8
                        ret
                        .section         .note.GNU-stack,"",@progbits
