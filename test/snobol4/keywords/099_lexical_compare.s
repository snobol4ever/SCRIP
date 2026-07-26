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
                        sub              rsp, 65544
                        mov              rdi, rsp
                        mov              ecx, 65544
                        xor              eax, eax
                        rep stosb
                        mov              qword ptr [rsp + 680], rsp
                        mov              r12, qword ptr [1879048192]
                        mov              [rsp + 65536], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#         LGT('b', 'a')                                               :S(A)F(END)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n0_α:
                        mov              qword ptr [rbp + 160], 1
                        mov              rax, qword ptr [rip + .Lx1_0]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   xchain0_n1_α
.Lx1_0:
                        .quad            .Lx1_0_s
.Lx1_0_s:
                        .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n1_α:
                        mov              qword ptr [rbp + 128], 1
                        mov              rax, qword ptr [rip + .Lx2_0]
                        mov              qword ptr [rbp + 136], rax
                                                                                        jmp   xchain0_n2_α
.Lx2_0:
                        .quad            .Lx2_0_s
.Lx2_0_s:
                        .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
# IR_CMP_TEST
xchain0_n2_α:
                        lea              rdi, [rbp + 160]
                        lea              rsi, [rbp + 128]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jle   main_γ
                        mov              qword ptr [rbp + 96], 0
                        mov              qword ptr [rbp + 104], 0
                                                                                        jmp   xchain0_n3_α
#=======================================================================================================================
# A       OUTPUT = 'b > a'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n3_α:
                        mov              qword ptr [rbp + 208], 1
                        mov              rax, qword ptr [rip + .Lx5_0]
                        mov              qword ptr [rbp + 216], rax
                                                                                        jmp   xchain0_n4_α
.Lx5_0:
                        .quad            .Lx5_0_s
.Lx5_0_s:
                        .string          "b > a"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n4_α:
                        mov              rsi, qword ptr [rbp + 208]
                        mov              rdx, qword ptr [rbp + 216]
                        mov              rdi, qword ptr [rip + .Lx6_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                                                                                        jmp   xchain0_n5_α
.Lx6_0:
                        .quad            .Lx6_0_s
.Lx6_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         LLT('a', 'b')                                               :S(B)F(END)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n5_α:
                        mov              qword ptr [rbp + 304], 1
                        mov              rax, qword ptr [rip + .Lx7_0]
                        mov              qword ptr [rbp + 312], rax
                                                                                        jmp   xchain0_n6_α
.Lx7_0:
                        .quad            .Lx7_0_s
.Lx7_0_s:
                        .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n6_α:
                        mov              qword ptr [rbp + 272], 1
                        mov              rax, qword ptr [rip + .Lx8_0]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   xchain0_n7_α
.Lx8_0:
                        .quad            .Lx8_0_s
.Lx8_0_s:
                        .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
# IR_CMP_TEST
xchain0_n7_α:
                        lea              rdi, [rbp + 304]
                        lea              rsi, [rbp + 272]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   main_γ
                        mov              qword ptr [rbp + 240], 0
                        mov              qword ptr [rbp + 248], 0
                                                                                        jmp   xchain0_n8_α
#=======================================================================================================================
# B       OUTPUT = 'a < b'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n8_α:
                        mov              qword ptr [rbp + 352], 1
                        mov              rax, qword ptr [rip + .Lx11_0]
                        mov              qword ptr [rbp + 360], rax
                                                                                        jmp   xchain0_n9_α
.Lx11_0:
                        .quad            .Lx11_0_s
.Lx11_0_s:
                        .string          "a < b"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n9_α:
                        mov              rsi, qword ptr [rbp + 352]
                        mov              rdx, qword ptr [rbp + 360]
                        mov              rdi, qword ptr [rip + .Lx12_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                                                                                        jmp   xchain0_n10_α
.Lx12_0:
                        .quad            .Lx12_0_s
.Lx12_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         LEQ('cat', 'cat')                                           :S(C)F(END)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n10_α:
                        mov              qword ptr [rbp + 448], 1
                        mov              rax, qword ptr [rip + .Lx13_0]
                        mov              qword ptr [rbp + 456], rax
                                                                                        jmp   xchain0_n11_α
.Lx13_0:
                        .quad            .Lx13_0_s
.Lx13_0_s:
                        .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n11_α:
                        mov              qword ptr [rbp + 416], 1
                        mov              rax, qword ptr [rip + .Lx14_0]
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   xchain0_n12_α
.Lx14_0:
                        .quad            .Lx14_0_s
.Lx14_0_s:
                        .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
# IR_CMP_TEST
xchain0_n12_α:
                        lea              rdi, [rbp + 448]
                        lea              rsi, [rbp + 416]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jne   main_γ
                        mov              qword ptr [rbp + 384], 0
                        mov              qword ptr [rbp + 392], 0
                                                                                        jmp   xchain0_n13_α
#=======================================================================================================================
# C       OUTPUT = 'cat = cat'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n13_α:
                        mov              qword ptr [rbp + 496], 1
                        mov              rax, qword ptr [rip + .Lx17_0]
                        mov              qword ptr [rbp + 504], rax
                                                                                        jmp   xchain0_n14_α
.Lx17_0:
                        .quad            .Lx17_0_s
.Lx17_0_s:
                        .string          "cat = cat"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n14_α:
                        mov              rsi, qword ptr [rbp + 496]
                        mov              rdx, qword ptr [rbp + 504]
                        mov              rdi, qword ptr [rip + .Lx18_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                                                                                        jmp   xchain0_n15_α
.Lx18_0:
                        .quad            .Lx18_0_s
.Lx18_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         LNE('cat', 'dog')                                           :S(D)F(END)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n15_α:
                        mov              qword ptr [rbp + 592], 1
                        mov              rax, qword ptr [rip + .Lx19_0]
                        mov              qword ptr [rbp + 600], rax
                                                                                        jmp   xchain0_n16_α
.Lx19_0:
                        .quad            .Lx19_0_s
.Lx19_0_s:
                        .string          "cat"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n16_α:
                        mov              qword ptr [rbp + 560], 1
                        mov              rax, qword ptr [rip + .Lx20_0]
                        mov              qword ptr [rbp + 568], rax
                                                                                        jmp   xchain0_n17_α
.Lx20_0:
                        .quad            .Lx20_0_s
.Lx20_0_s:
                        .string          "dog"
#-----------------------------------------------------------------------------------------------------------------------
# IR_CMP_TEST
xchain0_n17_α:
                        lea              rdi, [rbp + 592]
                        lea              rsi, [rbp + 560]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        je    main_γ
                        mov              qword ptr [rbp + 528], 0
                        mov              qword ptr [rbp + 536], 0
                                                                                        jmp   xchain0_n18_α
#=======================================================================================================================
# D       OUTPUT = 'cat != dog'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n18_α:
                        mov              qword ptr [rbp + 640], 1
                        mov              rax, qword ptr [rip + .Lx23_0]
                        mov              qword ptr [rbp + 648], rax
                                                                                        jmp   xchain0_n19_α
.Lx23_0:
                        .quad            .Lx23_0_s
.Lx23_0_s:
                        .string          "cat != dog"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN global
xchain0_n19_α:
                        mov              rsi, qword ptr [rbp + 640]
                        mov              rdx, qword ptr [rbp + 648]
                        mov              rdi, qword ptr [rip + .Lx24_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 624], rax
                        mov              qword ptr [rbp + 632], rdx
                                                                                        jmp   main_γ
.Lx24_0:
                        .quad            .Lx24_0_s
.Lx24_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, qword ptr [rbp + 680]
                        mov              rbp, [rsp + 65536]
                        add              rsp, 65544
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, qword ptr [rbp + 680]
                        mov              dword ptr [rsp+0], 99
                        mov              dword ptr [rsp+4], 0
                        mov              qword ptr [rsp+8], 0
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 65536]
                        add              rsp, 65544
                        ret
                        .section         .note.GNU-stack,"",@progbits
