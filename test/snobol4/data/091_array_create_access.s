                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "A"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 1
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 1
                        call             gva_register@PLT
                        xor              esi, esi
                        call             main_α
                        xor              eax, eax
                        add              rsp, 24
                        ret
main_α:
#=======================================================================================================================
                        .global          main_α
                        .global          main_β
                        .global          main_γ
                        .global          main_ω
                        sub              rsp, 65544
                        mov              rdi, rsp
                        mov              ecx, 65544
                        xor              eax, eax
                        rep stosb
                        mov              qword ptr [rsp + 1016], rsp
                        mov              r12, qword ptr [1879048192]
                        mov              [rsp + 65536], rbp
                        mov              rbp, rsp
main_α_body:
#         A = ARRAY(5)
# IR_LIT_INTEGER
xchain0_n0_α:
                        mov              qword ptr [rbp + 160], 6
                        mov              rax, qword ptr [rip + .Lx1_0]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   xchain0_n1_α
.Lx1_0:
                        .quad            5
xchain0_n1_α:
# BOX IR_CALL ARRAY(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
                        mov              rax, qword ptr [rbp + 160]
                        mov              qword ptr [rbp + 128], rax
                        mov              rax, qword ptr [rbp + 168]
                        mov              qword ptr [rbp + 136], rax
                        .section         .rodata
.Lrkfn3:                .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn3]
                        lea              rsi, [rbp + 128]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 112], rax
                        mov              qword ptr [rbp + 120], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n3_α
                                                                                        jmp   xchain0_n2_α
xchain0_n1_β:
                                                                                        jmp   xchain0_n3_α
# IR_ASSIGN gva
xchain0_n2_α:
                        mov              rax, qword ptr [rbp + 112]
                        mov              rdx, qword ptr [rbp + 120]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                                                                                        jmp   xchain0_n3_α
#         A<1> = 'first'
# IR_VAR
xchain0_n3_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                                                                                        jmp   xchain0_n4_α
# IR_LIT_INTEGER
xchain0_n4_α:
                        mov              qword ptr [rbp + 224], 6
                        mov              rax, qword ptr [rip + .Lx6_0]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   xchain0_n6_α
.Lx6_0:
                        .quad            1
#         A<3> = 'third'
# IR_VAR
xchain0_n5_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                                                                                        jmp   xchain0_n7_α
# IR_SUBSCRIPT x[i] variable
xchain0_n6_α:
                        mov              rdi, qword ptr [rbp + 192]
                        mov              rsi, qword ptr [rbp + 200]
                        mov              rdx, qword ptr [rbp + 224]
                        mov              rcx, qword ptr [rbp + 232]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    xchain0_n5_α
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                                                                                        jmp   xchain0_n9_α
# IR_LIT_INTEGER
xchain0_n7_α:
                        mov              qword ptr [rbp + 368], 6
                        mov              rax, qword ptr [rip + .Lx9_0]
                        mov              qword ptr [rbp + 376], rax
                                                                                        jmp   xchain0_n10_α
.Lx9_0:
                        .quad            3
#         A<5> = 'fifth'
# IR_VAR
xchain0_n8_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                                                                                        jmp   xchain0_n11_α
# IR_LIT_STRING
xchain0_n9_α:
                        mov              qword ptr [rbp + 288], 1
                        mov              rax, qword ptr [rip + .Lx11_0]
                        mov              qword ptr [rbp + 296], rax
                                                                                        jmp   xchain0_n13_α
.Lx11_0:
                        .quad            .Lx11_0_s
.Lx11_0_s:
                        .string          "first"
# IR_SUBSCRIPT x[i] variable
xchain0_n10_α:
                        mov              rdi, qword ptr [rbp + 336]
                        mov              rsi, qword ptr [rbp + 344]
                        mov              rdx, qword ptr [rbp + 368]
                        mov              rcx, qword ptr [rbp + 376]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    xchain0_n8_α
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                                                                                        jmp   xchain0_n14_α
# IR_LIT_INTEGER
xchain0_n11_α:
                        mov              qword ptr [rbp + 512], 6
                        mov              rax, qword ptr [rip + .Lx13_0]
                        mov              qword ptr [rbp + 520], rax
                                                                                        jmp   xchain0_n15_α
.Lx13_0:
                        .quad            5
#         OUTPUT = A<1>
# IR_VAR
xchain0_n12_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 640], rax
                        mov              qword ptr [rbp + 648], rdx
                                                                                        jmp   xchain0_n16_α
# IR_ASSIGN_VAR
xchain0_n13_α:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 288]
                        mov              rcx, qword ptr [rbp + 296]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    xchain0_n5_α
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   xchain0_n5_α
# IR_LIT_STRING
xchain0_n14_α:
                        mov              qword ptr [rbp + 432], 1
                        mov              rax, qword ptr [rip + .Lx16_0]
                        mov              qword ptr [rbp + 440], rax
                                                                                        jmp   xchain0_n18_α
.Lx16_0:
                        .quad            .Lx16_0_s
.Lx16_0_s:
                        .string          "third"
# IR_SUBSCRIPT x[i] variable
xchain0_n15_α:
                        mov              rdi, qword ptr [rbp + 480]
                        mov              rsi, qword ptr [rbp + 488]
                        mov              rdx, qword ptr [rbp + 512]
                        mov              rcx, qword ptr [rbp + 520]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    xchain0_n12_α
                        mov              qword ptr [rbp + 544], rax
                        mov              qword ptr [rbp + 552], rdx
                                                                                        jmp   xchain0_n19_α
# IR_LIT_INTEGER
xchain0_n16_α:
                        mov              qword ptr [rbp + 672], 6
                        mov              rax, qword ptr [rip + .Lx18_0]
                        mov              qword ptr [rbp + 680], rax
                                                                                        jmp   xchain0_n20_α
.Lx18_0:
                        .quad            1
#         OUTPUT = A<3>
# IR_VAR
xchain0_n17_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 768], rax
                        mov              qword ptr [rbp + 776], rdx
                                                                                        jmp   xchain0_n21_α
# IR_ASSIGN_VAR
xchain0_n18_α:
                        mov              rdi, qword ptr [rbp + 400]
                        mov              rsi, qword ptr [rbp + 408]
                        mov              rdx, qword ptr [rbp + 432]
                        mov              rcx, qword ptr [rbp + 440]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    xchain0_n8_α
                        mov              qword ptr [rbp + 464], rax
                        mov              qword ptr [rbp + 472], rdx
                                                                                        jmp   xchain0_n8_α
# IR_LIT_STRING
xchain0_n19_α:
                        mov              qword ptr [rbp + 576], 1
                        mov              rax, qword ptr [rip + .Lx21_0]
                        mov              qword ptr [rbp + 584], rax
                                                                                        jmp   xchain0_n23_α
.Lx21_0:
                        .quad            .Lx21_0_s
.Lx21_0_s:
                        .string          "fifth"
# IR_SUBSCRIPT x[i] variable
xchain0_n20_α:
                        mov              rdi, qword ptr [rbp + 640]
                        mov              rsi, qword ptr [rbp + 648]
                        mov              rdx, qword ptr [rbp + 672]
                        mov              rcx, qword ptr [rbp + 680]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    xchain0_n17_α
                        mov              qword ptr [rbp + 704], rax
                        mov              qword ptr [rbp + 712], rdx
                                                                                        jmp   xchain0_n24_α
# IR_LIT_INTEGER
xchain0_n21_α:
                        mov              qword ptr [rbp + 800], 6
                        mov              rax, qword ptr [rip + .Lx23_0]
                        mov              qword ptr [rbp + 808], rax
                                                                                        jmp   xchain0_n25_α
.Lx23_0:
                        .quad            3
#         OUTPUT = A<5>
# IR_VAR
xchain0_n22_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 896], rax
                        mov              qword ptr [rbp + 904], rdx
                                                                                        jmp   xchain0_n26_α
# IR_ASSIGN_VAR
xchain0_n23_α:
                        mov              rdi, qword ptr [rbp + 544]
                        mov              rsi, qword ptr [rbp + 552]
                        mov              rdx, qword ptr [rbp + 576]
                        mov              rcx, qword ptr [rbp + 584]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    xchain0_n12_α
                        mov              qword ptr [rbp + 608], rax
                        mov              qword ptr [rbp + 616], rdx
                                                                                        jmp   xchain0_n12_α
# IR_DEREF variable -> value
xchain0_n24_α:
                        mov              rdi, qword ptr [rbp + 704]
                        mov              rsi, qword ptr [rbp + 712]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    xchain0_n17_α
                        mov              qword ptr [rbp + 736], rax
                        mov              qword ptr [rbp + 744], rdx
                                                                                        jmp   xchain0_n27_α
# IR_SUBSCRIPT x[i] variable
xchain0_n25_α:
                        mov              rdi, qword ptr [rbp + 768]
                        mov              rsi, qword ptr [rbp + 776]
                        mov              rdx, qword ptr [rbp + 800]
                        mov              rcx, qword ptr [rbp + 808]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    xchain0_n22_α
                        mov              qword ptr [rbp + 832], rax
                        mov              qword ptr [rbp + 840], rdx
                                                                                        jmp   xchain0_n28_α
# IR_LIT_INTEGER
xchain0_n26_α:
                        mov              qword ptr [rbp + 928], 6
                        mov              rax, qword ptr [rip + .Lx28_0]
                        mov              qword ptr [rbp + 936], rax
                                                                                        jmp   xchain0_n29_α
.Lx28_0:
                        .quad            5
# IR_ASSIGN global
xchain0_n27_α:
                        mov              rsi, qword ptr [rbp + 736]
                        mov              rdx, qword ptr [rbp + 744]
                        mov              rdi, qword ptr [rip + .Lx29_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 624], rax
                        mov              qword ptr [rbp + 632], rdx
                                                                                        jmp   xchain0_n17_α
.Lx29_0:
                        .quad            .Lx29_0_s
.Lx29_0_s:
                        .string          "OUTPUT"
# IR_DEREF variable -> value
xchain0_n28_α:
                        mov              rdi, qword ptr [rbp + 832]
                        mov              rsi, qword ptr [rbp + 840]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    xchain0_n22_α
                        mov              qword ptr [rbp + 864], rax
                        mov              qword ptr [rbp + 872], rdx
                                                                                        jmp   xchain0_n30_α
# IR_SUBSCRIPT x[i] variable
xchain0_n29_α:
                        mov              rdi, qword ptr [rbp + 896]
                        mov              rsi, qword ptr [rbp + 904]
                        mov              rdx, qword ptr [rbp + 928]
                        mov              rcx, qword ptr [rbp + 936]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    main_γ
                        mov              qword ptr [rbp + 960], rax
                        mov              qword ptr [rbp + 968], rdx
                                                                                        jmp   xchain0_n31_α
# IR_ASSIGN global
xchain0_n30_α:
                        mov              rsi, qword ptr [rbp + 864]
                        mov              rdx, qword ptr [rbp + 872]
                        mov              rdi, qword ptr [rip + .Lx32_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 752], rax
                        mov              qword ptr [rbp + 760], rdx
                                                                                        jmp   xchain0_n22_α
.Lx32_0:
                        .quad            .Lx32_0_s
.Lx32_0_s:
                        .string          "OUTPUT"
# IR_DEREF variable -> value
xchain0_n31_α:
                        mov              rdi, qword ptr [rbp + 960]
                        mov              rsi, qword ptr [rbp + 968]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    main_γ
                        mov              qword ptr [rbp + 992], rax
                        mov              qword ptr [rbp + 1000], rdx
                                                                                        jmp   xchain0_n32_α
# IR_ASSIGN global
xchain0_n32_α:
                        mov              rsi, qword ptr [rbp + 992]
                        mov              rdx, qword ptr [rbp + 1000]
                        mov              rdi, qword ptr [rip + .Lx34_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 880], rax
                        mov              qword ptr [rbp + 888], rdx
                                                                                        jmp   main_γ
.Lx34_0:
                        .quad            .Lx34_0_s
.Lx34_0_s:
                        .string          "OUTPUT"
main_β:
                                                                                        jmp   main_ω
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, qword ptr [rbp + 1016]
                        mov              rbp, [rsp + 65536]
                        add              rsp, 65544
                        ret
main_ω:
                        mov              rsp, qword ptr [rbp + 1016]
                        mov              dword ptr [rsp+0], 99
                        mov              dword ptr [rsp+4], 0
                        mov              qword ptr [rsp+8], 0
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 65536]
                        add              rsp, 65544
                        ret
