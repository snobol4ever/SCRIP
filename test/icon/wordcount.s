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
                        sub              rsp, 472
                        mov              rdi, rsp
                        mov              ecx, 472
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 464], rbp
                        mov              rbp, rsp
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        mov              qword ptr [rbp + 416], 1
                        mov              rax, qword ptr [rip + .Lx21_0]
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   n1_assign_α
.Lx21_0:
                        .quad            .Lx21_0_s
.Lx21_0_s:
                        .string          "the quick brown fox jumps over the lazy dog"
#-----------------------------------------------------------------------------------------------------------------------
n1_assign_α:
                        mov              rax, qword ptr [rbp + 416]
                        mov              rdx, qword ptr [rbp + 424]
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                                                                                        jmp   n2_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:
                        mov              qword ptr [rbp + 400], 6
                        mov              rax, qword ptr [rip + .Lx23_0]
                        mov              qword ptr [rbp + 408], rax
                                                                                        jmp   n3_assign_α
.Lx23_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n3_assign_α:
                        mov              rax, qword ptr [rbp + 400]
                        mov              rdx, qword ptr [rbp + 408]
                        mov              qword ptr [rbp + 432], rax
                        mov              qword ptr [rbp + 440], rdx
                                                                                        jmp   n4_var_α
#-----------------------------------------------------------------------------------------------------------------------
n4_var_α:
                        mov              rax, qword ptr [rbp + 448]
                        mov              qword ptr [rbp + 368], rax
                        mov              rax, qword ptr [rbp + 456]
                        mov              qword ptr [rbp + 376], rax
                                                                                        jmp   n5_scan_enter_α
#-----------------------------------------------------------------------------------------------------------------------
n5_scan_enter_α:
                        mov              rdi, qword ptr [rbp + 368]
                        mov              rsi, qword ptr [rbp + 376]
                        mov              rdx, r13
                        mov              rcx, r14
                        mov              r8, r15
                        call             rt_scan_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r14, 0
                                                                                        jmp   n7_lit_charset_α
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:
                        mov              rax, qword ptr [rbp + 432]
                        mov              qword ptr [rbp + 48], rax
                        mov              rax, qword ptr [rbp + 440]
                        mov              qword ptr [rbp + 56], rax
                                                                                        jmp   n8_call_builtin_icon_α
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_charset_α:
                        mov              qword ptr [rbp + 192], 1
                        mov              dword ptr [rbp + 196], -1
                        mov              rax, qword ptr [rip + .Lx31_0]
                        mov              qword ptr [rbp + 200], rax
                                                                                        jmp   n9_scan_upto_α
.Lx31_0:
                        .quad            .Lx31_0_s
.Lx31_0_s:
                        .string          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#-----------------------------------------------------------------------------------------------------------------------
n8_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 48]
                        mov              qword ptr [rbp + 16], rax
                        mov              rax, qword ptr [rbp + 56]
                        mov              qword ptr [rbp + 24], rax
                        .section         .rodata
.Lrkfn33:               .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn33]
                        lea              rsi, [rbp + 16]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 0], rax
                        mov              qword ptr [rbp + 8], rdx
                        cmp              eax, 99
                                                                                        je    main_ω
                                                                                        jmp   main_ω
n8_call_builtin_icon_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n9_scan_upto_α:
                        mov              qword ptr [rbp + 176], r14
.Lx35_0:
                        mov              rax, qword ptr [rbp + 176]
                        cmp              rax, r15
                                                                                        jge   n20_scan_α
                        mov              rcx, rax
                        movzx            esi, byte ptr [r13+rcx]
                        mov              rdi, qword ptr [rip + .Lx35_2]
                        push             rax
                        push             r10
                        call             strchr@PLT
                        pop              r10
                        test             rax, rax
                        pop              rax
                                                                                        je    .Lx35_1
                        mov              qword ptr [rbp + 160], 6
                        add              rax, 1
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n10_scan_tab_α
.Lx35_1:
                        inc              qword ptr [rbp + 176]
                                                                                        jmp   .Lx35_0
n9_scan_upto_β:
                        inc              qword ptr [rbp + 176]
                                                                                        jmp   .Lx35_0
.Lx35_2:
                        .quad            .Lx35_2_s
.Lx35_2_s:
                        .string          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#-----------------------------------------------------------------------------------------------------------------------
n10_scan_tab_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rbp + 168]
                        cmp              rax, 1
                                                                                        jge   .Lx37_0
                        add              rax, r15
                        add              rax, 1
.Lx37_0:
                        cmp              rax, 1
                                                                                        jge   .Lx37_239
                        add              rsp, 16
                                                                                        jmp   n9_scan_upto_β
.Lx37_239:
                        mov              rcx, r15
                        add              rcx, 1
                        cmp              rax, rcx
                                                                                        jle   .Lx37_240
                        add              rsp, 16
                                                                                        jmp   n9_scan_upto_β
.Lx37_240:
                        mov              qword ptr [rsp + 0], r14
                        mov              rdi, r13
                        mov              rsi, r14
                        mov              rdx, rax
                        sub              rdx, 1
                        mov              r14, rdx
                        push             r10
                        push             r10
                        call             rt_substr@PLT
                        pop              r10
                        pop              r10
                        mov              qword ptr [rbp + 128], rax
                        mov              qword ptr [rbp + 136], rdx
                                                                                        jmp   n11_lit_charset_α
n10_scan_tab_β:
                        mov              r14, qword ptr [rsp + 0]
                        add              rsp, 16
                                                                                        jmp   n9_scan_upto_β
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_charset_α:
                        mov              qword ptr [rbp + 352], 1
                        mov              dword ptr [rbp + 356], -1
                        mov              rax, qword ptr [rip + .Lx38_0]
                        mov              qword ptr [rbp + 360], rax
                                                                                        jmp   n12_scan_many_α
.Lx38_0:
                        .quad            .Lx38_0_s
.Lx38_0_s:
                        .string          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#-----------------------------------------------------------------------------------------------------------------------
n12_scan_many_α:
                        mov              eax, r14d
.Lx40_0:
                        cmp              eax, r15d
                                                                                        jge   .Lx40_1
                        movsxd           rcx, eax
                        movzx            esi, byte ptr [r13+rcx]
                        mov              rdi, qword ptr [rip + .Lx40_2]
                        push             rax
                        push             r10
                        call             strchr@PLT
                        pop              r10
                        test             rax, rax
                        pop              rax
                                                                                        je    .Lx40_1
                        add              eax, 1
                                                                                        jmp   .Lx40_0
.Lx40_1:
                        cmp              eax, r14d
                                                                                        je    n14_var_α
                        mov              qword ptr [rbp + 336], 6
                        movsxd           rcx, eax
                        add              rcx, 1
                        mov              qword ptr [rbp + 344], rcx
                                                                                        jmp   n13_scan_tab_α
n12_scan_many_β:
                                                                                        jmp   n14_var_α
.Lx40_2:
                        .quad            .Lx40_2_s
.Lx40_2_s:
                        .string          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#-----------------------------------------------------------------------------------------------------------------------
n13_scan_tab_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rbp + 344]
                        cmp              rax, 1
                                                                                        jge   .Lx42_0
                        add              rax, r15
                        add              rax, 1
.Lx42_0:
                        cmp              rax, 1
                                                                                        jge   .Lx42_239
                        add              rsp, 16
                                                                                        jmp   n14_var_α
.Lx42_239:
                        mov              rcx, r15
                        add              rcx, 1
                        cmp              rax, rcx
                                                                                        jle   .Lx42_240
                        add              rsp, 16
                                                                                        jmp   n14_var_α
.Lx42_240:
                        mov              qword ptr [rsp + 0], r14
                        mov              rdi, r13
                        mov              rsi, r14
                        mov              rdx, rax
                        sub              rdx, 1
                        mov              r14, rdx
                        push             r10
                        push             r10
                        call             rt_substr@PLT
                        pop              r10
                        pop              r10
                        mov              qword ptr [rbp + 304], rax
                        mov              qword ptr [rbp + 312], rdx
                                                                                        jmp   n14_var_α
n13_scan_tab_β:
                        mov              r14, qword ptr [rsp + 0]
                        add              rsp, 16
                                                                                        jmp   n14_var_α
#-----------------------------------------------------------------------------------------------------------------------
n14_var_α:
                        mov              rax, qword ptr [rbp + 432]
                        mov              qword ptr [rbp + 272], rax
                        mov              rax, qword ptr [rbp + 440]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n15_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_integer_α:
                        mov              qword ptr [rbp + 288], 6
                        mov              rax, qword ptr [rip + .Lx45_0]
                        mov              qword ptr [rbp + 296], rax
                                                                                        jmp   n16_op75_α
.Lx45_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n16_op75_α:
                        mov              eax, dword ptr [rbp + 432]
                        cmp              eax, 7
                                                                                        je    .Lx47_1
                        cmp              eax, 6
                                                                                        jne   .Lx47_0
                        mov              eax, dword ptr [rbp + 288]
                        cmp              eax, 6
                                                                                        jne   .Lx47_0
.Lx47_1:
                        mov              rax, qword ptr [rbp + 432]
                        mov              qword ptr [rbp + 256], rax
                        mov              rax, qword ptr [rbp + 440]
                        mov              qword ptr [rbp + 264], rax
                                                                                        jmp   n17_binop_α
.Lx47_0:
                        lea              rdi, [rbp + 432]
                        lea              rsi, [rbp + 288]
                        lea              rdx, [rbp + 256]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n17_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n17_binop_α:
                        mov              eax, dword ptr [rbp + 256]
                        cmp              eax, 6
                                                                                        jne   .Lx48_0
                        mov              rax, qword ptr [rbp + 264]
                        mov              rcx, 1
                        add              rax, rcx
                        mov              qword ptr [rbp + 240], 6
                        mov              qword ptr [rbp + 248], rax
                                                                                        jmp   n18_assign_α
.Lx48_0:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 288]
                        mov              rcx, qword ptr [rbp + 296]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n7_lit_charset_α
                        mov              qword ptr [rbp + 240], rax
                        mov              qword ptr [rbp + 248], rdx
                                                                                        jmp   n18_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n18_assign_α:
                        mov              rax, qword ptr [rbp + 240]
                        mov              rdx, qword ptr [rbp + 248]
                        mov              qword ptr [rbp + 432], rax
                        mov              qword ptr [rbp + 440], rdx
                        mov              qword ptr [rbp + 224], rax
                        mov              qword ptr [rbp + 232], rdx
                                                                                        jmp   n19_conjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n19_conjunction_α:
                        mov              rax, qword ptr [rbp + 224]
                        mov              qword ptr [rbp + 208], rax
                        mov              rax, qword ptr [rbp + 232]
                        mov              qword ptr [rbp + 216], rax
                                                                                        jmp   n7_lit_charset_α
n19_conjunction_β:
                                                                                        jmp   n7_lit_charset_α
#-----------------------------------------------------------------------------------------------------------------------
n20_scan_α:
                        lea              rdi, [rbp + 80]
                        call             rt_scan_leave@PLT
                        mov              r13, qword ptr [rbp + 80]
                        mov              r14, qword ptr [rbp + 88]
                        mov              r15, qword ptr [rbp + 96]
                                                                                        jmp   n6_var_α
n20_scan_β:
                                                                                        jmp   n6_var_α
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 464]
                        add              rsp, 472
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 464]
                        add              rsp, 472
                        ret
                        .section         .note.GNU-stack,"",@progbits
