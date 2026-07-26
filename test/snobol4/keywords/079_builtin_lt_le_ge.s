                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        xor              esi, esi
                        call             main_α
                        xor              eax, eax
                        add              rsp, 24
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_α:
                        .global          main_α
                        .global          main_β
                        .global          main_γ
                        .global          main_ω
                        sub              rsp, 728
                        mov              rdi, rsp
                        mov              ecx, 728
                        xor              eax, eax
                        rep stosb
                        mov              r12, qword ptr [1879048192]
                        mov              [rsp + 720], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#         LT(3, 5)                                                    :S(A)F(END)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n0_α:
                        mov              qword ptr [rbp + 224], 6
                        mov              rax, qword ptr [rip + .Lx1_0]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   xchain0_n1_α
.Lx1_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n1_α:
                        mov              qword ptr [rbp + 192], 6
                        mov              rax, qword ptr [rip + .Lx2_0]
                        mov              qword ptr [rbp + 200], rax
                                                                                        jmp   xchain0_n2_α
.Lx2_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
# IR_COERCE_NUMERIC
xchain0_n2_α:
                        mov              eax, dword ptr [rbp + 224]
                        cmp              eax, 7
                                                                                        je    .Lx4_1
                        cmp              eax, 6
                                                                                        jne   .Lx4_0
                        mov              eax, dword ptr [rbp + 192]
                        cmp              eax, 6
                                                                                        jne   .Lx4_0
.Lx4_1:
                        mov              rax, qword ptr [rbp + 224]
                        mov              qword ptr [rbp + 160], rax
                        mov              rax, qword ptr [rbp + 232]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   xchain0_n3_α
.Lx4_0:
                        lea              rdi, [rbp + 224]
                        lea              rsi, [rbp + 192]
                        lea              rdx, [rbp + 160]
                        mov              rcx, 147
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   xchain0_n3_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_COERCE_NUMERIC
xchain0_n3_α:
                        mov              eax, dword ptr [rbp + 192]
                        cmp              eax, 7
                                                                                        je    .Lx6_1
                        cmp              eax, 6
                                                                                        jne   .Lx6_0
                        mov              eax, dword ptr [rbp + 224]
                        cmp              eax, 6
                                                                                        jne   .Lx6_0
.Lx6_1:
                        mov              rax, qword ptr [rbp + 192]
                        mov              qword ptr [rbp + 128], rax
                        mov              rax, qword ptr [rbp + 200]
                        mov              qword ptr [rbp + 136], rax
                                                                                        jmp   xchain0_n4_α
.Lx6_0:
                        lea              rdi, [rbp + 192]
                        lea              rsi, [rbp + 224]
                        lea              rdx, [rbp + 128]
                        mov              rcx, 148
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   xchain0_n4_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_CMP_TEST
xchain0_n4_α:
                        lea              rdi, [rbp + 160]
                        lea              rsi, [rbp + 128]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   main_γ
                        mov              qword ptr [rbp + 96], 0
                        mov              qword ptr [rbp + 104], 0
                                                                                        jmp   xchain0_n5_α
#=======================================================================================================================
# A       OUTPUT = '3 < 5'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n5_α:
                        mov              qword ptr [rbp + 272], 1
                        mov              rax, qword ptr [rip + .Lx9_0]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   xchain0_n6_α
.Lx9_0:
                        .quad            .Lx9_0_s
.Lx9_0_s:
                        .string          "3 < 5"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n6_α:
                        mov              rsi, qword ptr [rbp + 272]
                        mov              rdx, qword ptr [rbp + 280]
                        mov              rdi, qword ptr [rip + .Lx10_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                                                                                        jmp   xchain0_n7_α
.Lx10_0:
                        .quad            .Lx10_0_s
.Lx10_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         LE(5, 5)                                                    :S(B)F(END)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n7_α:
                        mov              qword ptr [rbp + 432], 6
                        mov              rax, qword ptr [rip + .Lx11_0]
                        mov              qword ptr [rbp + 440], rax
                                                                                        jmp   xchain0_n8_α
.Lx11_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n8_α:
                        mov              qword ptr [rbp + 400], 6
                        mov              rax, qword ptr [rip + .Lx12_0]
                        mov              qword ptr [rbp + 408], rax
                                                                                        jmp   xchain0_n9_α
.Lx12_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
# IR_COERCE_NUMERIC
xchain0_n9_α:
                        mov              eax, dword ptr [rbp + 432]
                        cmp              eax, 7
                                                                                        je    .Lx14_1
                        cmp              eax, 6
                                                                                        jne   .Lx14_0
                        mov              eax, dword ptr [rbp + 400]
                        cmp              eax, 6
                                                                                        jne   .Lx14_0
.Lx14_1:
                        mov              rax, qword ptr [rbp + 432]
                        mov              qword ptr [rbp + 368], rax
                        mov              rax, qword ptr [rbp + 440]
                        mov              qword ptr [rbp + 376], rax
                                                                                        jmp   xchain0_n10_α
.Lx14_0:
                        lea              rdi, [rbp + 432]
                        lea              rsi, [rbp + 400]
                        lea              rdx, [rbp + 368]
                        mov              rcx, 118
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   xchain0_n10_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_COERCE_NUMERIC
xchain0_n10_α:
                        mov              eax, dword ptr [rbp + 400]
                        cmp              eax, 7
                                                                                        je    .Lx16_1
                        cmp              eax, 6
                                                                                        jne   .Lx16_0
                        mov              eax, dword ptr [rbp + 432]
                        cmp              eax, 6
                                                                                        jne   .Lx16_0
.Lx16_1:
                        mov              rax, qword ptr [rbp + 400]
                        mov              qword ptr [rbp + 336], rax
                        mov              rax, qword ptr [rbp + 408]
                        mov              qword ptr [rbp + 344], rax
                                                                                        jmp   xchain0_n11_α
.Lx16_0:
                        lea              rdi, [rbp + 400]
                        lea              rsi, [rbp + 432]
                        lea              rdx, [rbp + 336]
                        mov              rcx, 119
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   xchain0_n11_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_CMP_TEST
xchain0_n11_α:
                        lea              rdi, [rbp + 368]
                        lea              rsi, [rbp + 336]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jg    main_γ
                        mov              qword ptr [rbp + 304], 0
                        mov              qword ptr [rbp + 312], 0
                                                                                        jmp   xchain0_n12_α
#=======================================================================================================================
# B       OUTPUT = '5 <= 5'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n12_α:
                        mov              qword ptr [rbp + 480], 1
                        mov              rax, qword ptr [rip + .Lx19_0]
                        mov              qword ptr [rbp + 488], rax
                                                                                        jmp   xchain0_n13_α
.Lx19_0:
                        .quad            .Lx19_0_s
.Lx19_0_s:
                        .string          "5 <= 5"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n13_α:
                        mov              rsi, qword ptr [rbp + 480]
                        mov              rdx, qword ptr [rbp + 488]
                        mov              rdi, qword ptr [rip + .Lx20_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 464], rax
                        mov              qword ptr [rbp + 472], rdx
                                                                                        jmp   xchain0_n14_α
.Lx20_0:
                        .quad            .Lx20_0_s
.Lx20_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         GE(7, 5)                                                    :S(C)F(END)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n14_α:
                        mov              qword ptr [rbp + 640], 6
                        mov              rax, qword ptr [rip + .Lx21_0]
                        mov              qword ptr [rbp + 648], rax
                                                                                        jmp   xchain0_n15_α
.Lx21_0:
                        .quad            7
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n15_α:
                        mov              qword ptr [rbp + 608], 6
                        mov              rax, qword ptr [rip + .Lx22_0]
                        mov              qword ptr [rbp + 616], rax
                                                                                        jmp   xchain0_n16_α
.Lx22_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
# IR_COERCE_NUMERIC
xchain0_n16_α:
                        mov              eax, dword ptr [rbp + 640]
                        cmp              eax, 7
                                                                                        je    .Lx24_1
                        cmp              eax, 6
                                                                                        jne   .Lx24_0
                        mov              eax, dword ptr [rbp + 608]
                        cmp              eax, 6
                                                                                        jne   .Lx24_0
.Lx24_1:
                        mov              rax, qword ptr [rbp + 640]
                        mov              qword ptr [rbp + 576], rax
                        mov              rax, qword ptr [rbp + 648]
                        mov              qword ptr [rbp + 584], rax
                                                                                        jmp   xchain0_n17_α
.Lx24_0:
                        lea              rdi, [rbp + 640]
                        lea              rsi, [rbp + 608]
                        lea              rdx, [rbp + 576]
                        mov              rcx, 109
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   xchain0_n17_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_COERCE_NUMERIC
xchain0_n17_α:
                        mov              eax, dword ptr [rbp + 608]
                        cmp              eax, 7
                                                                                        je    .Lx26_1
                        cmp              eax, 6
                                                                                        jne   .Lx26_0
                        mov              eax, dword ptr [rbp + 640]
                        cmp              eax, 6
                                                                                        jne   .Lx26_0
.Lx26_1:
                        mov              rax, qword ptr [rbp + 608]
                        mov              qword ptr [rbp + 544], rax
                        mov              rax, qword ptr [rbp + 616]
                        mov              qword ptr [rbp + 552], rax
                                                                                        jmp   xchain0_n18_α
.Lx26_0:
                        lea              rdi, [rbp + 608]
                        lea              rsi, [rbp + 640]
                        lea              rdx, [rbp + 544]
                        mov              rcx, 110
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   xchain0_n18_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_CMP_TEST
xchain0_n18_α:
                        lea              rdi, [rbp + 576]
                        lea              rsi, [rbp + 544]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        js    main_γ
                        mov              qword ptr [rbp + 512], 0
                        mov              qword ptr [rbp + 520], 0
                                                                                        jmp   xchain0_n19_α
#=======================================================================================================================
# C       OUTPUT = '7 >= 5'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n19_α:
                        mov              qword ptr [rbp + 688], 1
                        mov              rax, qword ptr [rip + .Lx29_0]
                        mov              qword ptr [rbp + 696], rax
                                                                                        jmp   xchain0_n20_α
.Lx29_0:
                        .quad            .Lx29_0_s
.Lx29_0_s:
                        .string          "7 >= 5"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n20_α:
                        mov              rsi, qword ptr [rbp + 688]
                        mov              rdx, qword ptr [rbp + 696]
                        mov              rdi, qword ptr [rip + .Lx30_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 672], rax
                        mov              qword ptr [rbp + 680], rdx
                                                                                        jmp   main_γ
.Lx30_0:
                        .quad            .Lx30_0_s
.Lx30_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 720]
                        add              rsp, 728
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              dword ptr [rsp+0], 99
                        mov              dword ptr [rsp+4], 0
                        mov              qword ptr [rsp+8], 0
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 720]
                        add              rsp, 728
                        ret
                        .section         .note.GNU-stack,"",@progbits
