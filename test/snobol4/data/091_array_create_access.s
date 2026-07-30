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
                        mov              rax, qword ptr [rip + .Lx33_0]
                        mov              qword ptr [rsp + 72], rax
                                                                                        jmp   n1_call_α
.Lx33_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n1_call_α:
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                        .section         .rodata
.Lrkfn35:               .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn35]
                        lea              rsi, [rsp + 32]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rdx
                        cmp              eax, 99
                                                                                        je    n3_var_α
                                                                                        jmp   n2_assign_α
n1_call_β:
                                                                                        jmp   n3_var_α
#-----------------------------------------------------------------------------------------------------------------------
n2_assign_α:
                        mov              rax, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n3_var_α
#=======================================================================================================================
#         A<1> = 'first'
#-----------------------------------------------------------------------------------------------------------------------
n3_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 80], rax
                        mov              qword ptr [rsp + 88], rdx
                                                                                        jmp   n4_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_integer_α:
                        mov              qword ptr [rsp + 96], 6
                        mov              rax, qword ptr [rip + .Lx38_0]
                        mov              qword ptr [rsp + 104], rax
                                                                                        jmp   n6_subscript_α
.Lx38_0:
                        .quad            1
#=======================================================================================================================
#         A<3> = 'third'
#-----------------------------------------------------------------------------------------------------------------------
n5_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 160], rax
                        mov              qword ptr [rsp + 168], rdx
                                                                                        jmp   n7_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n6_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 80]
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 96]
                        mov              rcx, qword ptr [rsp + 104]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx40_240
                        add              rsp, 16
                                                                                        jmp   n5_var_α
.Lx40_240:
                        mov              qword ptr [rsp + 112], rax
                        mov              qword ptr [rsp + 120], rdx
                        add              rsp, 16
                                                                                        jmp   n9_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_integer_α:
                        mov              qword ptr [rsp + 176], 6
                        mov              rax, qword ptr [rip + .Lx41_0]
                        mov              qword ptr [rsp + 184], rax
                                                                                        jmp   n10_subscript_α
.Lx41_0:
                        .quad            3
#=======================================================================================================================
#         A<5> = 'fifth'
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 240], rax
                        mov              qword ptr [rsp + 248], rdx
                                                                                        jmp   n11_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_string_α:
                        mov              qword ptr [rsp + 128], 1
                        mov              dword ptr [rsp + 132], 5
                        mov              rax, qword ptr [rip + .Lx43_0]
                        mov              qword ptr [rsp + 136], rax
                                                                                        jmp   n13_assign_var_α
.Lx43_0:
                        .quad            .Lx43_0_s
.Lx43_0_s:
                        .string          "first"
#-----------------------------------------------------------------------------------------------------------------------
n10_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 160]
                        mov              rsi, qword ptr [rsp + 168]
                        mov              rdx, qword ptr [rsp + 176]
                        mov              rcx, qword ptr [rsp + 184]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx44_240
                        add              rsp, 16
                                                                                        jmp   n8_var_α
.Lx44_240:
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rdx
                        add              rsp, 16
                                                                                        jmp   n14_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:
                        mov              qword ptr [rsp + 256], 6
                        mov              rax, qword ptr [rip + .Lx45_0]
                        mov              qword ptr [rsp + 264], rax
                                                                                        jmp   n15_subscript_α
.Lx45_0:
                        .quad            5
#=======================================================================================================================
#         OUTPUT = A<1>
#-----------------------------------------------------------------------------------------------------------------------
n12_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 320], rax
                        mov              qword ptr [rsp + 328], rdx
                                                                                        jmp   n16_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n13_assign_var_α:
                        mov              rdi, qword ptr [rsp + 112]
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 128]
                        mov              rcx, qword ptr [rsp + 136]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n5_var_α
                        mov              qword ptr [rsp + 144], rax
                        mov              qword ptr [rsp + 152], rdx
                                                                                        jmp   n5_var_α
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_string_α:
                        mov              qword ptr [rsp + 208], 1
                        mov              dword ptr [rsp + 212], 5
                        mov              rax, qword ptr [rip + .Lx48_0]
                        mov              qword ptr [rsp + 216], rax
                                                                                        jmp   n18_assign_var_α
.Lx48_0:
                        .quad            .Lx48_0_s
.Lx48_0_s:
                        .string          "third"
#-----------------------------------------------------------------------------------------------------------------------
n15_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 240]
                        mov              rsi, qword ptr [rsp + 248]
                        mov              rdx, qword ptr [rsp + 256]
                        mov              rcx, qword ptr [rsp + 264]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx49_240
                        add              rsp, 16
                                                                                        jmp   n12_var_α
.Lx49_240:
                        mov              qword ptr [rsp + 272], rax
                        mov              qword ptr [rsp + 280], rdx
                        add              rsp, 16
                                                                                        jmp   n19_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:
                        mov              qword ptr [rsp + 336], 6
                        mov              rax, qword ptr [rip + .Lx50_0]
                        mov              qword ptr [rsp + 344], rax
                                                                                        jmp   n20_subscript_α
.Lx50_0:
                        .quad            1
#=======================================================================================================================
#         OUTPUT = A<3>
#-----------------------------------------------------------------------------------------------------------------------
n17_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                                                                                        jmp   n21_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n18_assign_var_α:
                        mov              rdi, qword ptr [rsp + 192]
                        mov              rsi, qword ptr [rsp + 200]
                        mov              rdx, qword ptr [rsp + 208]
                        mov              rcx, qword ptr [rsp + 216]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n8_var_α
                        mov              qword ptr [rsp + 224], rax
                        mov              qword ptr [rsp + 232], rdx
                                                                                        jmp   n8_var_α
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:
                        mov              qword ptr [rsp + 288], 1
                        mov              dword ptr [rsp + 292], 5
                        mov              rax, qword ptr [rip + .Lx53_0]
                        mov              qword ptr [rsp + 296], rax
                                                                                        jmp   n23_assign_var_α
.Lx53_0:
                        .quad            .Lx53_0_s
.Lx53_0_s:
                        .string          "fifth"
#-----------------------------------------------------------------------------------------------------------------------
n20_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 320]
                        mov              rsi, qword ptr [rsp + 328]
                        mov              rdx, qword ptr [rsp + 336]
                        mov              rcx, qword ptr [rsp + 344]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx54_240
                        add              rsp, 16
                                                                                        jmp   n17_var_α
.Lx54_240:
                        mov              qword ptr [rsp + 352], rax
                        mov              qword ptr [rsp + 360], rdx
                        add              rsp, 16
                                                                                        jmp   n24_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n21_lit_integer_α:
                        mov              qword ptr [rsp + 400], 6
                        mov              rax, qword ptr [rip + .Lx55_0]
                        mov              qword ptr [rsp + 408], rax
                                                                                        jmp   n25_subscript_α
.Lx55_0:
                        .quad            3
#=======================================================================================================================
#         OUTPUT = A<5>
#-----------------------------------------------------------------------------------------------------------------------
n22_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 448], rax
                        mov              qword ptr [rsp + 456], rdx
                                                                                        jmp   n26_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n23_assign_var_α:
                        mov              rdi, qword ptr [rsp + 272]
                        mov              rsi, qword ptr [rsp + 280]
                        mov              rdx, qword ptr [rsp + 288]
                        mov              rcx, qword ptr [rsp + 296]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n12_var_α
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rdx
                                                                                        jmp   n12_var_α
#-----------------------------------------------------------------------------------------------------------------------
n24_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 352]
                        mov              rsi, qword ptr [rsp + 360]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx58_240
                        add              rsp, 16
                                                                                        jmp   n17_var_α
.Lx58_240:
                        mov              qword ptr [rsp + 368], rax
                        mov              qword ptr [rsp + 376], rdx
                        add              rsp, 16
                                                                                        jmp   n27_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n25_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 384]
                        mov              rsi, qword ptr [rsp + 392]
                        mov              rdx, qword ptr [rsp + 400]
                        mov              rcx, qword ptr [rsp + 408]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx59_240
                        add              rsp, 16
                                                                                        jmp   n22_var_α
.Lx59_240:
                        mov              qword ptr [rsp + 416], rax
                        mov              qword ptr [rsp + 424], rdx
                        add              rsp, 16
                                                                                        jmp   n28_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_integer_α:
                        mov              qword ptr [rsp + 464], 6
                        mov              rax, qword ptr [rip + .Lx60_0]
                        mov              qword ptr [rsp + 472], rax
                                                                                        jmp   n29_subscript_α
.Lx60_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_α:
                        mov              rsi, qword ptr [rsp + 368]
                        mov              rdx, qword ptr [rsp + 376]
                        mov              rdi, qword ptr [rip + .Lx61_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n17_var_α
.Lx61_0:
                        .quad            .Lx61_0_s
.Lx61_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n28_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 416]
                        mov              rsi, qword ptr [rsp + 424]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx62_240
                        add              rsp, 16
                                                                                        jmp   n22_var_α
.Lx62_240:
                        mov              qword ptr [rsp + 432], rax
                        mov              qword ptr [rsp + 440], rdx
                        add              rsp, 16
                                                                                        jmp   n30_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n29_subscript_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 448]
                        mov              rsi, qword ptr [rsp + 456]
                        mov              rdx, qword ptr [rsp + 464]
                        mov              rcx, qword ptr [rsp + 472]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx63_240
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx63_240:
                        mov              qword ptr [rsp + 480], rax
                        mov              qword ptr [rsp + 488], rdx
                        add              rsp, 16
                                                                                        jmp   n31_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n30_assign_α:
                        mov              rsi, qword ptr [rsp + 432]
                        mov              rdx, qword ptr [rsp + 440]
                        mov              rdi, qword ptr [rip + .Lx64_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n22_var_α
.Lx64_0:
                        .quad            .Lx64_0_s
.Lx64_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n31_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 480]
                        mov              rsi, qword ptr [rsp + 488]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx65_240
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx65_240:
                        mov              qword ptr [rsp + 496], rax
                        mov              qword ptr [rsp + 504], rdx
                        add              rsp, 16
                                                                                        jmp   n32_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n32_assign_α:
                        mov              rsi, qword ptr [rsp + 496]
                        mov              rdx, qword ptr [rsp + 504]
                        mov              rdi, qword ptr [rip + .Lx66_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   main_γ
.Lx66_0:
                        .quad            .Lx66_0_s
.Lx66_0_s:
                        .string          "OUTPUT"
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
