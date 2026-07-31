                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "A"
.Lgvan1:                .string          "I"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .section         .text
                        .intel_syntax    noprefix
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
                        sub              rsp, 520
                        mov              rdi, rsp
                        mov              ecx, 520
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         A = ARRAY(5)
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        mov              qword ptr [rsp + 64], 6
                        mov              rax, qword ptr [rip + .Lx39_0]
                        mov              qword ptr [rsp + 72], rax
                                                                                        jmp   n3_call_α
.Lx39_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n4_var_α
n1_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n2_goto_α:
                                                                                        jmp   n5_var_α
n2_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n3_call_α:
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        .section         .rodata
.Lrkfn43:               .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn43]
                        lea              rsi, [rsp + 32]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rdx
                        cmp              eax, 99
                                                                                        je    n7_lit_integer_α
                                                                                        jmp   n6_assign_α
n3_call_β:
                                                                                        jmp   n7_lit_integer_α
#=======================================================================================================================
# FILL    A<I> = I * I
#-----------------------------------------------------------------------------------------------------------------------
n4_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 96], rax
                        mov              qword ptr [rsp + 104], rdx
                                                                                        jmp   n8_var_α
#=======================================================================================================================
# READ    OUTPUT = A<I>
#-----------------------------------------------------------------------------------------------------------------------
n5_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 336], rax
                        mov              qword ptr [rsp + 344], rdx
                                                                                        jmp   n10_var_α
#-----------------------------------------------------------------------------------------------------------------------
n6_assign_α:
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n7_lit_integer_α
#=======================================================================================================================
#         I = 1
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx47_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n12_assign_α
.Lx47_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 112], rax
                        mov              qword ptr [rsp + 120], rdx
                                                                                        jmp   n13_subscript_α
#=======================================================================================================================
#         I = I + 1
#-----------------------------------------------------------------------------------------------------------------------
n9_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n14_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 352], rax
                        mov              qword ptr [rsp + 360], rdx
                                                                                        jmp   n16_subscript_α
#=======================================================================================================================
#         I = I + 1
#-----------------------------------------------------------------------------------------------------------------------
n11_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n17_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n12_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   n4_var_α
#-----------------------------------------------------------------------------------------------------------------------
n13_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 112]
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 128]
                        mov              rcx, qword ptr [rsp + 136]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx53_240
                        add              rsp, 16
                                                                                        jmp   n9_var_α
.Lx53_240:
                        mov              qword ptr [rsp + 144], rax
                        mov              qword ptr [rsp + 152], rdx
                        add              rsp, 16
                                                                                        jmp   n19_var_α
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx54_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n20_binop_α
.Lx54_0:
                        .quad            1
#=======================================================================================================================
#         LE(I, 5)                                                    :S(FILL)
#-----------------------------------------------------------------------------------------------------------------------
n15_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rdx
                                                                                        jmp   n21_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n16_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 352]
                        mov              rsi, qword ptr [rsp + 360]
                        mov              rdx, qword ptr [rsp + 368]
                        mov              rcx, qword ptr [rsp + 376]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx56_240
                        add              rsp, 16
                                                                                        jmp   n11_var_α
.Lx56_240:
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                        add              rsp, 16
                                                                                        jmp   n23_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx57_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n24_binop_α
.Lx57_0:
                        .quad            1
#=======================================================================================================================
#         LE(I, 5)                                                    :S(READ)
#-----------------------------------------------------------------------------------------------------------------------
n18_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 496], rax
                        mov              qword ptr [rsp + 504], rdx
                                                                                        jmp   n25_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n19_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 160], rax
                        mov              qword ptr [rsp + 168], rdx
                                                                                        jmp   n26_var_α
#-----------------------------------------------------------------------------------------------------------------------
n20_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx60_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n15_var_α
.Lx60_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n27_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_integer_α:
                        mov              qword ptr [rsp + 288], 6
                        mov              rax, qword ptr [rip + .Lx61_0]
                        mov              qword ptr [rsp + 296], rax
                                                                                        jmp   n28_op75_α
.Lx61_0:
                        .quad            5
#=======================================================================================================================
#         I = 1
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx62_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n29_assign_α
.Lx62_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n23_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 384]
                        mov              rsi, qword ptr [rsp + 392]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx63_240
                        add              rsp, 16
                                                                                        jmp   n11_var_α
.Lx63_240:
                        mov              qword ptr [rsp + 400], rax
                        mov              qword ptr [rsp + 408], rdx
                        add              rsp, 16
                                                                                        jmp   n30_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n24_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx64_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n18_var_α
.Lx64_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n31_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_integer_α:
                        mov              qword ptr [rsp + 480], 6
                        mov              rax, qword ptr [rip + .Lx65_0]
                        mov              qword ptr [rsp + 488], rax
                                                                                        jmp   n32_op75_α
.Lx65_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n26_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 176], rax
                        mov              qword ptr [rsp + 184], rdx
                                                                                        jmp   n33_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 48
                                                                                        jmp   n15_var_α
#-----------------------------------------------------------------------------------------------------------------------
n28_op75_α:
                        mov              eax, dword ptr [rsp + 304]
                        cmp              eax, 7
                                                                                        je    .Lx69_1
                        cmp              eax, 6
                                                                                        jne   .Lx69_0
                        mov              eax, dword ptr [rsp + 288]
                        cmp              eax, 6
                                                                                        jne   .Lx69_0
.Lx69_1:
                        mov              rax, qword ptr [rsp + 304]
                        mov              qword ptr [rsp + 272], rax
                        mov              rax, qword ptr [rsp + 312]
                        mov              qword ptr [rsp + 280], rax
                                                                                        jmp   n34_op75_α
.Lx69_0:
                        lea              rdi, [rsp + 304]
                        lea              rsi, [rsp + 288]
                        lea              rdx, [rsp + 272]
                        mov              rcx, 118
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n34_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n29_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   n5_var_α
#-----------------------------------------------------------------------------------------------------------------------
n30_assign_α:
                        mov              rsi, qword ptr [rsp + 384]
                        mov              rdx, qword ptr [rsp + 392]
                        mov              rdi, qword ptr [rip + .Lx71_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n11_var_α
.Lx71_0:
                        .quad            .Lx71_0_s
.Lx71_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 48
                                                                                        jmp   n18_var_α
#-----------------------------------------------------------------------------------------------------------------------
n32_op75_α:
                        mov              eax, dword ptr [rsp + 496]
                        cmp              eax, 7
                                                                                        je    .Lx74_1
                        cmp              eax, 6
                                                                                        jne   .Lx74_0
                        mov              eax, dword ptr [rsp + 480]
                        cmp              eax, 6
                                                                                        jne   .Lx74_0
.Lx74_1:
                        mov              rax, qword ptr [rsp + 496]
                        mov              qword ptr [rsp + 464], rax
                        mov              rax, qword ptr [rsp + 504]
                        mov              qword ptr [rsp + 472], rax
                                                                                        jmp   n35_op75_α
.Lx74_0:
                        lea              rdi, [rsp + 496]
                        lea              rsi, [rsp + 480]
                        lea              rdx, [rsp + 464]
                        mov              rcx, 118
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n35_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n33_binop_α:
                        mov              eax, dword ptr [rsp + 160]
                        cmp              eax, 6
                                                                                        jne   .Lx75_0
                        mov              eax, dword ptr [rsp + 176]
                        cmp              eax, 6
                                                                                        jne   .Lx75_0
                        mov              rax, qword ptr [rsp + 168]
                        mov              rcx, qword ptr [rsp + 184]
                        imul             rax, rcx
                        mov              qword ptr [rsp + 144], 6
                        mov              qword ptr [rsp + 152], rax
                                                                                        jmp   n36_assign_var_α
.Lx75_0:
                        mov              rdi, qword ptr [rsp + 160]
                        mov              rsi, qword ptr [rsp + 168]
                        mov              rdx, qword ptr [rsp + 176]
                        mov              rcx, qword ptr [rsp + 184]
                        call             rt_mul@PLT
                        cmp              eax, 99
                                                                                        je    n9_var_α
                        mov              qword ptr [rsp + 144], rax
                        mov              qword ptr [rsp + 152], rdx
                                                                                        jmp   n36_assign_var_α
#-----------------------------------------------------------------------------------------------------------------------
n34_op75_α:
                        mov              eax, dword ptr [rsp + 288]
                        cmp              eax, 7
                                                                                        je    .Lx77_1
                        cmp              eax, 6
                                                                                        jne   .Lx77_0
                        mov              eax, dword ptr [rsp + 304]
                        cmp              eax, 6
                                                                                        jne   .Lx77_0
.Lx77_1:
                        mov              rax, qword ptr [rsp + 288]
                        mov              qword ptr [rsp + 256], rax
                        mov              rax, qword ptr [rsp + 296]
                        mov              qword ptr [rsp + 264], rax
                                                                                        jmp   n37_op77_α
.Lx77_0:
                        lea              rdi, [rsp + 288]
                        lea              rsi, [rsp + 304]
                        lea              rdx, [rsp + 256]
                        mov              rcx, 119
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n37_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n35_op75_α:
                        mov              eax, dword ptr [rsp + 480]
                        cmp              eax, 7
                                                                                        je    .Lx79_1
                        cmp              eax, 6
                                                                                        jne   .Lx79_0
                        mov              eax, dword ptr [rsp + 496]
                        cmp              eax, 6
                                                                                        jne   .Lx79_0
.Lx79_1:
                        mov              rax, qword ptr [rsp + 480]
                        mov              qword ptr [rsp + 448], rax
                        mov              rax, qword ptr [rsp + 488]
                        mov              qword ptr [rsp + 456], rax
                                                                                        jmp   n38_op77_α
.Lx79_0:
                        lea              rdi, [rsp + 480]
                        lea              rsi, [rsp + 496]
                        lea              rdx, [rsp + 448]
                        mov              rcx, 119
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n38_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n36_assign_var_α:
                        mov              rdi, qword ptr [rsp + 128]
                        mov              rsi, qword ptr [rsp + 136]
                        mov              rdx, qword ptr [rsp + 144]
                        mov              rcx, qword ptr [rsp + 152]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n9_var_α
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rdx
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n37_op77_α:
                        lea              rdi, [rsp + 272]
                        lea              rsi, [rsp + 256]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jg    n22_lit_integer_α
                                                                                        jmp   n4_var_α
#-----------------------------------------------------------------------------------------------------------------------
n38_op77_α:
                        lea              rdi, [rsp + 464]
                        lea              rsi, [rsp + 448]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jg    main_γ
                                                                                        jmp   n5_var_α
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 520
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 520
                        ret
                        .section         .note.GNU-stack,"",@progbits
