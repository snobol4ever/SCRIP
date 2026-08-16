                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                                         jmp   n1_statement_end_α
n0_statement_begin_β:                                                         jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:                                                           jmp   n2_statement_begin_α
#=======================================================================================================================
#         DIFFER(3 + 2, 5)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                                         jmp   n3_lit_integer_α
n2_statement_begin_β:                                                         jmp   n13_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx129_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_lit_integer_α
.Lx129_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx130_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n5_binop_α
n4_lit_integer_β:       add              rsp, 16
                        add              rsp, 16;                             jmp   n2_statement_begin_β
.Lx130_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n5_binop_α:             sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              edx, 3;                              jne   .Lx131_2
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        add              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx131_7
.Lx131_2:               and              edx, 1;                              jz    .Lx131_0
                        mov              rsi, qword ptr [rsp + 40]            # lit_integer
                        mov              rdi, qword ptr [rsp + 24]
                        cmp              eax, 5;                              je    .Lx131_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx131_4
.Lx131_3:               movq             xmm0, rsi
.Lx131_4:               cmp              ecx, 5;                              je    .Lx131_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx131_6
.Lx131_5:               movq             xmm1, rdi
.Lx131_6:               addsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx131_7:                                                                     jmp   n6_lit_integer_α
.Lx131_0:               mov              rdi, qword ptr [rsp + 32]            # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx131_240
                        add              rsp, 16;                             jmp   n4_lit_integer_β
.Lx131_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n6_lit_integer_α
n5_binop_β:             add              rsp, 16;                             jmp   n4_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx132_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n7_call_α
n6_lit_integer_β:       add              rsp, 16;                             jmp   n5_binop_β
.Lx132_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n7_call_α:              sub              rsp, 16
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
.Lrkfnzd134:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd134]
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
                        cmp              eax, 104;                            jne   .Lx133_240
                        add              rsp, 16;                             jmp   n6_lit_integer_β
.Lx133_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n8_statement_end_α
n7_call_β:              add              rsp, 16;                             jmp   n6_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_end_α:     add              rsp, 80;                             jmp   n9_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/001: 3+2'                   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n9_statement_begin_α:                                                         jmp   n10_lit_string_α
n9_statement_begin_β:                                                         jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx139_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n11_assign_α
.Lx139_0:               .quad            .Lx139_0_s
.Lx139_0_s:             .string          "FAIL 410/001: 3+2"
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx140_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n12_statement_end_α
.Lx140_0:               .quad            .Lx140_0_s
.Lx140_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e001  <stmt 4, line 7: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_begin_α:                                                        jmp   n14_statement_end_α
n13_statement_begin_β:                                                        jmp   n15_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_end_α:                                                          jmp   n15_statement_begin_α
#=======================================================================================================================
#         DIFFER(3 - 2, 1)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_begin_α:                                                        jmp   n16_lit_integer_α
n15_statement_begin_β:                                                        jmp   n26_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx149_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n17_lit_integer_α
.Lx149_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx150_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n18_binop_α
n17_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n15_statement_begin_β
.Lx150_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n18_binop_α:            sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              edx, 3;                              jne   .Lx151_2
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        sub              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx151_7
.Lx151_2:               and              edx, 1;                              jz    .Lx151_0
                        mov              rsi, qword ptr [rsp + 40]            # lit_integer
                        mov              rdi, qword ptr [rsp + 24]
                        cmp              eax, 5;                              je    .Lx151_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx151_4
.Lx151_3:               movq             xmm0, rsi
.Lx151_4:               cmp              ecx, 5;                              je    .Lx151_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx151_6
.Lx151_5:               movq             xmm1, rdi
.Lx151_6:               subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx151_7:                                                                     jmp   n19_lit_integer_α
.Lx151_0:               mov              rdi, qword ptr [rsp + 32]            # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_sub@PLT
                        cmp              eax, 104;                            jne   .Lx151_240
                        add              rsp, 16;                             jmp   n17_lit_integer_β
.Lx151_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n19_lit_integer_α
n18_binop_β:            add              rsp, 16;                             jmp   n17_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx152_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n20_call_α
n19_lit_integer_β:      add              rsp, 16;                             jmp   n18_binop_β
.Lx152_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n20_call_α:             sub              rsp, 16
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
.Lrkfnzd154:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd154]
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
                        cmp              eax, 104;                            jne   .Lx153_240
                        add              rsp, 16;                             jmp   n19_lit_integer_β
.Lx153_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n21_statement_end_α
n20_call_β:             add              rsp, 16;                             jmp   n19_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:    add              rsp, 80;                             jmp   n22_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/002: 3-2'                   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:                                                        jmp   n23_lit_string_α
n22_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx159_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n24_assign_α
.Lx159_0:               .quad            .Lx159_0_s
.Lx159_0_s:             .string          "FAIL 410/002: 3-2"
#-----------------------------------------------------------------------------------------------------------------------
n24_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx160_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n25_statement_end_α
.Lx160_0:               .quad            .Lx160_0_s
.Lx160_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e002  <stmt 7, line 10: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_begin_α:                                                        jmp   n27_statement_end_α
n26_statement_begin_β:                                                        jmp   n28_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_end_α:                                                          jmp   n28_statement_begin_α
#=======================================================================================================================
#         DIFFER(3 * 2, 6)                   :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_begin_α:                                                        jmp   n29_lit_integer_α
n28_statement_begin_β:                                                        jmp   n39_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx169_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n30_lit_integer_α
.Lx169_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx170_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n31_binop_α
n30_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n28_statement_begin_β
.Lx170_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n31_binop_α:            sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              edx, 3;                              jne   .Lx171_2
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        imul             rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx171_7
.Lx171_2:               and              edx, 1;                              jz    .Lx171_0
                        mov              rsi, qword ptr [rsp + 40]            # lit_integer
                        mov              rdi, qword ptr [rsp + 24]
                        cmp              eax, 5;                              je    .Lx171_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx171_4
.Lx171_3:               movq             xmm0, rsi
.Lx171_4:               cmp              ecx, 5;                              je    .Lx171_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx171_6
.Lx171_5:               movq             xmm1, rdi
.Lx171_6:               mulsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx171_7:                                                                     jmp   n32_lit_integer_α
.Lx171_0:               mov              rdi, qword ptr [rsp + 32]            # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_mul@PLT
                        cmp              eax, 104;                            jne   .Lx171_240
                        add              rsp, 16;                             jmp   n30_lit_integer_β
.Lx171_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n32_lit_integer_α
n31_binop_β:            add              rsp, 16;                             jmp   n30_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx172_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n33_call_α
n32_lit_integer_β:      add              rsp, 16;                             jmp   n31_binop_β
.Lx172_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n33_call_α:             sub              rsp, 16
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
.Lrkfnzd174:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd174]
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
                        cmp              eax, 104;                            jne   .Lx173_240
                        add              rsp, 16;                             jmp   n32_lit_integer_β
.Lx173_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n34_statement_end_α
n33_call_β:             add              rsp, 16;                             jmp   n32_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:    add              rsp, 80;                             jmp   n35_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/003: 3*2'                   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:                                                        jmp   n36_lit_string_α
n35_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx179_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n37_assign_α
.Lx179_0:               .quad            .Lx179_0_s
.Lx179_0_s:             .string          "FAIL 410/003: 3*2"
#-----------------------------------------------------------------------------------------------------------------------
n37_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx180_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n38_statement_end_α
.Lx180_0:               .quad            .Lx180_0_s
.Lx180_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e003  <stmt 10, line 13: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_begin_α:                                                        jmp   n40_statement_end_α
n39_statement_begin_β:                                                        jmp   n41_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_end_α:                                                          jmp   n41_statement_begin_α
#=======================================================================================================================
#         DIFFER(5 / 2, 2)                   :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_begin_α:                                                        jmp   n42_lit_integer_α
n41_statement_begin_β:                                                        jmp   n52_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx189_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n43_lit_integer_α
.Lx189_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx190_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n44_binop_α
n43_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n41_statement_begin_β
.Lx190_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n44_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_div@PLT
                        cmp              eax, 104;                            jne   .Lx191_240
                        add              rsp, 16;                             jmp   n43_lit_integer_β
.Lx191_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n45_lit_integer_α
n44_binop_β:            add              rsp, 16;                             jmp   n43_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx192_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n46_call_α
n45_lit_integer_β:      add              rsp, 16;                             jmp   n44_binop_β
.Lx192_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n46_call_α:             sub              rsp, 16
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
.Lrkfnzd194:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd194]
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
                        cmp              eax, 104;                            jne   .Lx193_240
                        add              rsp, 16;                             jmp   n45_lit_integer_β
.Lx193_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n47_statement_end_α
n46_call_β:             add              rsp, 16;                             jmp   n45_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_end_α:    add              rsp, 80;                             jmp   n48_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/004: 5/2 integer division'  :(END)
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_begin_α:                                                        jmp   n49_lit_string_α
n48_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx199_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n50_assign_α
.Lx199_0:               .quad            .Lx199_0_s
.Lx199_0_s:             .string          "FAIL 410/004: 5/2 integer division"
#-----------------------------------------------------------------------------------------------------------------------
n50_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx200_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n51_statement_end_α
.Lx200_0:               .quad            .Lx200_0_s
.Lx200_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e004  <stmt 13, line 16: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_begin_α:                                                        jmp   n53_statement_end_α
n52_statement_begin_β:                                                        jmp   n54_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_end_α:                                                          jmp   n54_statement_begin_α
#=======================================================================================================================
#         DIFFER(2 ** 3, 8)                   :f(e005)
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_begin_α:                                                        jmp   n55_lit_integer_α
n54_statement_begin_β:                                                        jmp   n65_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx209_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n56_lit_integer_α
.Lx209_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n56_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx210_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n57_binop_α
n56_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n54_statement_begin_β
.Lx210_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n57_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]            # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_pow@PLT
                        cmp              eax, 104;                            jne   .Lx211_240
                        add              rsp, 16;                             jmp   n56_lit_integer_β
.Lx211_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n58_lit_integer_α
n57_binop_β:            add              rsp, 16;                             jmp   n56_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n58_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx212_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n59_call_α
n58_lit_integer_β:      add              rsp, 16;                             jmp   n57_binop_β
.Lx212_0:               .quad            8
#-----------------------------------------------------------------------------------------------------------------------
n59_call_α:             sub              rsp, 16
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
.Lrkfnzd214:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd214]
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
                        cmp              eax, 104;                            jne   .Lx213_240
                        add              rsp, 16;                             jmp   n58_lit_integer_β
.Lx213_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n60_statement_end_α
n59_call_β:             add              rsp, 16;                             jmp   n58_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_end_α:    add              rsp, 80;                             jmp   n61_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/005: 2**3'                  :(END)
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_begin_α:                                                        jmp   n62_lit_string_α
n61_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n62_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 18
                        mov              rax, qword ptr [rip + .Lx219_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n63_assign_α
.Lx219_0:               .quad            .Lx219_0_s
.Lx219_0_s:             .string          "FAIL 410/005: 2**3"
#-----------------------------------------------------------------------------------------------------------------------
n63_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx220_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n64_statement_end_α
.Lx220_0:               .quad            .Lx220_0_s
.Lx220_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e005  <stmt 16, line 19: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_begin_α:                                                        jmp   n66_statement_end_α
n65_statement_begin_β:                                                        jmp   n67_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_end_α:                                                          jmp   n67_statement_begin_α
#=======================================================================================================================
#         DIFFER('3' + 2, 5)                   :f(e006)
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_begin_α:                                                        jmp   n68_lit_string_α
n67_statement_begin_β:                                                        jmp   n78_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n68_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx229_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n69_lit_integer_α
.Lx229_0:               .quad            .Lx229_0_s
.Lx229_0_s:             .string          "3"
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx230_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n70_binop_α
n69_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n67_statement_begin_β
.Lx230_0:               .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n70_binop_α:            sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_string
                        mov              ecx, dword ptr [rsp + 16]            # lit_integer
                        mov              edx, eax
                        and              edx, ecx
                        cmp              edx, 3;                              jne   .Lx231_2
                        mov              rax, qword ptr [rsp + 40]            # lit_string
                        mov              rdx, qword ptr [rsp + 24]            # lit_integer
                        add              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx231_7
.Lx231_2:               and              edx, 1;                              jz    .Lx231_0
                        mov              rsi, qword ptr [rsp + 40]            # lit_string
                        mov              rdi, qword ptr [rsp + 24]            # lit_integer
                        cmp              eax, 5;                              je    .Lx231_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx231_4
.Lx231_3:               movq             xmm0, rsi
.Lx231_4:               cmp              ecx, 5;                              je    .Lx231_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx231_6
.Lx231_5:               movq             xmm1, rdi
.Lx231_6:               addsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx231_7:                                                                     jmp   n71_lit_integer_α
.Lx231_0:               mov              rdi, qword ptr [rsp + 32]            # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx231_240
                        add              rsp, 16;                             jmp   n69_lit_integer_β
.Lx231_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n71_lit_integer_α
n70_binop_β:            add              rsp, 16;                             jmp   n69_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx232_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n72_call_α
n71_lit_integer_β:      add              rsp, 16;                             jmp   n70_binop_β
.Lx232_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n72_call_α:             sub              rsp, 16
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
.Lrkfnzd234:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd234]
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
                        cmp              eax, 104;                            jne   .Lx233_240
                        add              rsp, 16;                             jmp   n71_lit_integer_β
.Lx233_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n73_statement_end_α
n72_call_β:             add              rsp, 16;                             jmp   n71_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_end_α:    add              rsp, 80;                             jmp   n74_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/006: string+int coerce'     :(END)
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_begin_α:                                                        jmp   n75_lit_string_α
n74_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n75_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx239_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n76_assign_α
.Lx239_0:               .quad            .Lx239_0_s
.Lx239_0_s:             .string          "FAIL 410/006: string+int coerce"
#-----------------------------------------------------------------------------------------------------------------------
n76_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx240_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n77_statement_end_α
.Lx240_0:               .quad            .Lx240_0_s
.Lx240_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e006  <stmt 19, line 24: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_begin_α:                                                        jmp   n79_statement_end_α
n78_statement_begin_β:                                                        jmp   n80_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_end_α:                                                          jmp   n80_statement_begin_α
#=======================================================================================================================
#         DIFFER(3 + '-2', 1)                   :f(e007)
#-----------------------------------------------------------------------------------------------------------------------
n80_statement_begin_α:                                                        jmp   n81_lit_integer_α
n80_statement_begin_β:                                                        jmp   n91_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n81_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx249_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n82_lit_string_α
.Lx249_0:               .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n82_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 2
                        mov              rax, qword ptr [rip + .Lx250_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n83_binop_α
n82_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                             jmp   n80_statement_begin_β
.Lx250_0:               .quad            .Lx250_0_s
.Lx250_0_s:             .string          "-2"
#-----------------------------------------------------------------------------------------------------------------------
n83_binop_α:            sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        mov              ecx, dword ptr [rsp + 16]            # lit_string
                        mov              edx, eax
                        and              edx, ecx
                        cmp              edx, 3;                              jne   .Lx251_2
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              rdx, qword ptr [rsp + 24]            # lit_string
                        add              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx251_7
.Lx251_2:               and              edx, 1;                              jz    .Lx251_0
                        mov              rsi, qword ptr [rsp + 40]            # lit_integer
                        mov              rdi, qword ptr [rsp + 24]            # lit_string
                        cmp              eax, 5;                              je    .Lx251_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx251_4
.Lx251_3:               movq             xmm0, rsi
.Lx251_4:               cmp              ecx, 5;                              je    .Lx251_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx251_6
.Lx251_5:               movq             xmm1, rdi
.Lx251_6:               addsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx251_7:                                                                     jmp   n84_lit_integer_α
.Lx251_0:               mov              rdi, qword ptr [rsp + 32]            # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx251_240
                        add              rsp, 16;                             jmp   n82_lit_string_β
.Lx251_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n84_lit_integer_α
n83_binop_β:            add              rsp, 16;                             jmp   n82_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n84_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx252_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n85_call_α
n84_lit_integer_β:      add              rsp, 16;                             jmp   n83_binop_β
.Lx252_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n85_call_α:             sub              rsp, 16
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
.Lrkfnzd254:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd254]
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
                        cmp              eax, 104;                            jne   .Lx253_240
                        add              rsp, 16;                             jmp   n84_lit_integer_β
.Lx253_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n86_statement_end_α
n85_call_β:             add              rsp, 16;                             jmp   n84_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n86_statement_end_α:    add              rsp, 80;                             jmp   n87_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/007: int+neg-string'        :(END)
#-----------------------------------------------------------------------------------------------------------------------
n87_statement_begin_α:                                                        jmp   n88_lit_string_α
n87_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n88_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 28
                        mov              rax, qword ptr [rip + .Lx259_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n89_assign_α
.Lx259_0:               .quad            .Lx259_0_s
.Lx259_0_s:             .string          "FAIL 410/007: int+neg-string"
#-----------------------------------------------------------------------------------------------------------------------
n89_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx260_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n90_statement_end_α
.Lx260_0:               .quad            .Lx260_0_s
.Lx260_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e007  <stmt 22, line 27: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_begin_α:                                                        jmp   n92_statement_end_α
n91_statement_begin_β:                                                        jmp   n93_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n92_statement_end_α:                                                          jmp   n93_statement_begin_α
#=======================================================================================================================
#         DIFFER('1' + '0', 1)                   :f(e008)
#-----------------------------------------------------------------------------------------------------------------------
n93_statement_begin_α:                                                        jmp   n94_lit_string_α
n93_statement_begin_β:                                                        jmp   n104_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n94_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx269_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n95_lit_string_α
.Lx269_0:               .quad            .Lx269_0_s
.Lx269_0_s:             .string          "1"
#-----------------------------------------------------------------------------------------------------------------------
n95_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx270_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n96_binop_α
n95_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                             jmp   n93_statement_begin_β
.Lx270_0:               .quad            .Lx270_0_s
.Lx270_0_s:             .string          "0"
#-----------------------------------------------------------------------------------------------------------------------
n96_binop_α:            sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_string
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              edx, 3;                              jne   .Lx271_2
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        add              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx271_7
.Lx271_2:               and              edx, 1;                              jz    .Lx271_0
                        mov              rsi, qword ptr [rsp + 40]            # lit_string
                        mov              rdi, qword ptr [rsp + 24]
                        cmp              eax, 5;                              je    .Lx271_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx271_4
.Lx271_3:               movq             xmm0, rsi
.Lx271_4:               cmp              ecx, 5;                              je    .Lx271_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx271_6
.Lx271_5:               movq             xmm1, rdi
.Lx271_6:               addsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx271_7:                                                                     jmp   n97_lit_integer_α
.Lx271_0:               mov              rdi, qword ptr [rsp + 32]            # lit_string
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx271_240
                        add              rsp, 16;                             jmp   n95_lit_string_β
.Lx271_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n97_lit_integer_α
n96_binop_β:            add              rsp, 16;                             jmp   n95_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n97_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx272_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n98_call_α
n97_lit_integer_β:      add              rsp, 16;                             jmp   n96_binop_β
.Lx272_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n98_call_α:             sub              rsp, 16
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
.Lrkfnzd274:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd274]
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
                        cmp              eax, 104;                            jne   .Lx273_240
                        add              rsp, 16;                             jmp   n97_lit_integer_β
.Lx273_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n99_statement_end_α
n98_call_β:             add              rsp, 16;                             jmp   n97_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n99_statement_end_α:    add              rsp, 80;                             jmp   n100_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/008: string+string coerce'  :(END)
#-----------------------------------------------------------------------------------------------------------------------
n100_statement_begin_α:                                                       jmp   n101_lit_string_α
n100_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n101_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 34
                        mov              rax, qword ptr [rip + .Lx279_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n102_assign_α
.Lx279_0:               .quad            .Lx279_0_s
.Lx279_0_s:             .string          "FAIL 410/008: string+string coerce"
#-----------------------------------------------------------------------------------------------------------------------
n102_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx280_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n103_statement_end_α
.Lx280_0:               .quad            .Lx280_0_s
.Lx280_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_end_α:   add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e008  <stmt 25, line 30: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_begin_α:                                                       jmp   n105_statement_end_α
n104_statement_begin_β:                                                       jmp   n106_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n105_statement_end_α:                                                         jmp   n106_statement_begin_α
#=======================================================================================================================
#         DIFFER(5 + '', 5)                   :f(e009)
#-----------------------------------------------------------------------------------------------------------------------
n106_statement_begin_α:                                                       jmp   n107_lit_integer_α
n106_statement_begin_β:                                                       jmp   n117_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n107_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx289_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n108_lit_string_α
.Lx289_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n108_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx290_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n109_binop_α
n108_lit_string_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n106_statement_begin_β
.Lx290_0:               .quad            .Lx290_0_s
.Lx290_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n109_binop_α:           sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        mov              ecx, dword ptr [rsp + 16]            # lit_string
                        mov              edx, eax
                        and              edx, ecx
                        cmp              edx, 3;                              jne   .Lx291_2
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              rdx, qword ptr [rsp + 24]            # lit_string
                        add              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx291_7
.Lx291_2:               and              edx, 1;                              jz    .Lx291_0
                        mov              rsi, qword ptr [rsp + 40]            # lit_integer
                        mov              rdi, qword ptr [rsp + 24]            # lit_string
                        cmp              eax, 5;                              je    .Lx291_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx291_4
.Lx291_3:               movq             xmm0, rsi
.Lx291_4:               cmp              ecx, 5;                              je    .Lx291_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx291_6
.Lx291_5:               movq             xmm1, rdi
.Lx291_6:               addsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx291_7:                                                                     jmp   n110_lit_integer_α
.Lx291_0:               mov              rdi, qword ptr [rsp + 32]            # lit_integer
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx291_240
                        add              rsp, 16;                             jmp   n108_lit_string_β
.Lx291_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n110_lit_integer_α
n109_binop_β:           add              rsp, 16;                             jmp   n108_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n110_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx292_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n111_call_α
n110_lit_integer_β:     add              rsp, 16;                             jmp   n109_binop_β
.Lx292_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n111_call_α:            sub              rsp, 16
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
.Lrkfnzd294:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd294]
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
                        cmp              eax, 104;                            jne   .Lx293_240
                        add              rsp, 16;                             jmp   n110_lit_integer_β
.Lx293_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n112_statement_end_α
n111_call_β:            add              rsp, 16;                             jmp   n110_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n112_statement_end_α:   add              rsp, 80;                             jmp   n113_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 410/009: null addend is zero'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n113_statement_begin_α:                                                       jmp   n114_lit_string_α
n113_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n114_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 33
                        mov              rax, qword ptr [rip + .Lx299_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n115_assign_α
.Lx299_0:               .quad            .Lx299_0_s
.Lx299_0_s:             .string          "FAIL 410/009: null addend is zero"
#-----------------------------------------------------------------------------------------------------------------------
n115_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx300_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n116_statement_end_α
.Lx300_0:               .quad            .Lx300_0_s
.Lx300_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n116_statement_end_α:   add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e009  <stmt 28, line 35: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n117_statement_begin_α:                                                       jmp   n118_statement_end_α
n117_statement_begin_β:                                                       jmp   n119_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n118_statement_end_α:                                                         jmp   n119_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 410_arith_int (9/9)'
#-----------------------------------------------------------------------------------------------------------------------
n119_statement_begin_α:                                                       jmp   n120_lit_string_α
n119_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n120_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 24
                        mov              rax, qword ptr [rip + .Lx309_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n121_assign_α
.Lx309_0:               .quad            .Lx309_0_s
.Lx309_0_s:             .string          "PASS 410_arith_int (9/9)"
#-----------------------------------------------------------------------------------------------------------------------
n121_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx310_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n122_statement_end_α
.Lx310_0:               .quad            .Lx310_0_s
.Lx310_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n122_statement_end_α:   add              rsp, 16;                             jmp   main_γ
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
