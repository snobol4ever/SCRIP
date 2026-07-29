                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_palindrome_α
proc_palindrome_α:
                        .global          proc_palindrome_α
                        .global          proc_palindrome_β
                        .global          proc_palindrome_γ
                        .global          proc_palindrome_ω
                        sub              rsp, 640
                        mov              [rsp + 616], rcx
                        mov              [rsp + 624], rdx
                        mov              [rsp + 632], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              esi, 576
                        mov              edx, 608
                        call             rt_jmp_frame_lexprep2@PLT
proc_palindrome_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 560], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 568], rax
                                                                                        jmp   n1_call_builtin_icon_α
#-----------------------------------------------------------------------------------------------------------------------
n1_call_builtin_icon_α:
                        mov              rax, qword ptr [rbp + 560]
                        mov              qword ptr [rbp + 528], rax
                        mov              rax, qword ptr [rbp + 568]
                        mov              qword ptr [rbp + 536], rax
                        .section         .rodata
.Lrkfn39:               .string          "map"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn39]
                        lea              rsi, [rbp + 528]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                        cmp              eax, 99
                                                                                        je    n2_lit_integer_α
                                                                                        jmp   n3_assign_α
n1_call_builtin_icon_β:
                                                                                        jmp   n2_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:
                        mov              qword ptr [rbp + 496], 6
                        mov              rax, qword ptr [rip + .Lx40_0]
                        mov              qword ptr [rbp + 504], rax
                                                                                        jmp   n4_assign_α
.Lx40_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n3_assign_α:
                        mov              rax, qword ptr [rbp + 512]
                        mov              rdx, qword ptr [rbp + 520]
                        mov              qword ptr [rbp + 16], rax
                        mov              qword ptr [rbp + 24], rdx
                                                                                        jmp   n2_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n4_assign_α:
                        mov              rax, qword ptr [rbp + 496]
                        mov              rdx, qword ptr [rbp + 504]
                        mov              qword ptr [rbp + 576], rax
                        mov              qword ptr [rbp + 584], rdx
                                                                                        jmp   n5_var_α
#-----------------------------------------------------------------------------------------------------------------------
n5_var_α:
                        mov              rax, qword ptr [rbp + 16]
                        mov              qword ptr [rbp + 480], rax
                        mov              rax, qword ptr [rbp + 24]
                        mov              qword ptr [rbp + 488], rax
                                                                                        jmp   n6_unop_α
#-----------------------------------------------------------------------------------------------------------------------
n6_unop_α:
                        mov              rdi, qword ptr [rbp + 16]
                        mov              rsi, qword ptr [rbp + 24]
                        call             rt_size_d@PLT
                        mov              qword ptr [rbp + 464], rax
                        mov              qword ptr [rbp + 472], rdx
                                                                                        jmp   n8_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n7_var_α:
                        mov              rax, qword ptr [rbp + 576]
                        mov              qword ptr [rbp + 64], rax
                        mov              rax, qword ptr [rbp + 584]
                        mov              qword ptr [rbp + 72], rax
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n8_assign_α:
                        mov              rax, qword ptr [rbp + 464]
                        mov              rdx, qword ptr [rbp + 472]
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                                                                                        jmp   n7_var_α
#-----------------------------------------------------------------------------------------------------------------------
n9_var_α:
                        mov              rax, qword ptr [rbp + 592]
                        mov              qword ptr [rbp + 80], rax
                        mov              rax, qword ptr [rbp + 600]
                        mov              qword ptr [rbp + 88], rax
                                                                                        jmp   n11_binop_test_α
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:
                        mov              qword ptr [rbp + 32], 1
                        mov              rax, qword ptr [rip + .Lx51_0]
                        mov              qword ptr [rbp + 40], rax
                                                                                        jmp   n12_return_α
.Lx51_0:
                        .quad            .Lx51_0_s
.Lx51_0_s:
                        .string          "yes"
#-----------------------------------------------------------------------------------------------------------------------
n11_binop_test_α:
                        mov              eax, dword ptr [rbp + 576]
                        cmp              eax, 100
                                                                                        je    .Lx52_0
                        mov              eax, dword ptr [rbp + 592]
                        cmp              eax, 100
                                                                                        je    .Lx52_0
                        mov              eax, dword ptr [rbp + 576]
                        cmp              eax, 6
                                                                                        jne   .Lx52_2
                        mov              eax, dword ptr [rbp + 592]
                        cmp              eax, 6
                                                                                        jne   .Lx52_2
.Lx52_1:
                        mov              rax, qword ptr [rbp + 584]
                        mov              rcx, qword ptr [rbp + 600]
                        cmp              rax, rcx
                                                                                        jge   n10_lit_string_α
                        mov              rcx, qword ptr [rbp + 592]
                        mov              qword ptr [rbp + 48], rcx
                        mov              rcx, qword ptr [rbp + 600]
                        mov              qword ptr [rbp + 56], rcx
                                                                                        jmp   n13_disjunction_α
.Lx52_0:
                        mov              rdi, qword ptr [rbp + 576]
                        mov              rsi, qword ptr [rbp + 584]
                        mov              rdx, qword ptr [rbp + 592]
                        mov              rcx, qword ptr [rbp + 600]
                        mov              r8d, 5
                        lea              r9, [rbp + 48]
                        call             rt_relop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx52_1
                        cmp              eax, 1
                                                                                        je    n10_lit_string_α
                                                                                        jmp   n13_disjunction_α
.Lx52_2:
                        mov              rdi, qword ptr [rbp + 576]
                        mov              rsi, qword ptr [rbp + 584]
                        mov              rdx, qword ptr [rbp + 592]
                        mov              rcx, qword ptr [rbp + 600]
                        mov              r8d, 5
                        call             rt_jct_relop@PLT
                        test             eax, eax
                                                                                        jz    n10_lit_string_α
                        mov              rax, qword ptr [rbp + 592]
                        mov              qword ptr [rbp + 48], rax
                        mov              rax, qword ptr [rbp + 600]
                        mov              qword ptr [rbp + 56], rax
                                                                                        jmp   n13_disjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n12_return_α:
                        mov              rax, qword ptr [rbp + 32]
                        mov              rdx, qword ptr [rbp + 40]
                        mov              qword ptr [rbp + 0], rax
                        mov              qword ptr [rbp + 8], rdx
                                                                                        jmp   proc_palindrome_γ
#-----------------------------------------------------------------------------------------------------------------------
n13_disjunction_α:
                        mov              qword ptr [rbp + 272], 0
                        mov              qword ptr [rbp + 280], 0
                        mov              dword ptr [rbp + 288], 0
                                                                                        jmp   n15_var_ref_α
n13_disjunction_as:
                        mov              eax, dword ptr [rbp + 288]
                        cmp              eax, 0
                                                                                        jne   .Lx55_0
                                                                                        jmp   n14_var_α
.Lx55_0:
                                                                                        jmp   n14_var_α
n13_disjunction_β:
                        mov              eax, dword ptr [rbp + 288]
                                                                                        jmp   n14_var_α
n13_disjunction_af:
                        add              dword ptr [rbp + 288], 1
                        mov              eax, dword ptr [rbp + 288]
                                                                                        jmp   n14_var_α
#-----------------------------------------------------------------------------------------------------------------------
n14_var_α:
                        mov              rax, qword ptr [rbp + 576]
                        mov              qword ptr [rbp + 240], rax
                        mov              rax, qword ptr [rbp + 584]
                        mov              qword ptr [rbp + 248], rax
                                                                                        jmp   n16_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n15_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                                                                                        jmp   n18_var_α
n15_var_ref_β:
                                                                                        jmp   n13_disjunction_af
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:
                        mov              qword ptr [rbp + 256], 6
                        mov              rax, qword ptr [rip + .Lx60_0]
                        mov              qword ptr [rbp + 264], rax
                                                                                        jmp   n19_op75_α
.Lx60_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n17_var_α:
                        mov              rax, qword ptr [rbp + 592]
                        mov              qword ptr [rbp + 160], rax
                        mov              rax, qword ptr [rbp + 600]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   n20_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n18_var_α:
                        mov              rax, qword ptr [rbp + 576]
                        mov              qword ptr [rbp + 352], rax
                        mov              rax, qword ptr [rbp + 584]
                        mov              qword ptr [rbp + 360], rax
                                                                                        jmp   n21_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n19_op75_α:
                        mov              eax, dword ptr [rbp + 576]
                        cmp              eax, 7
                                                                                        je    .Lx66_1
                        cmp              eax, 6
                                                                                        jne   .Lx66_0
                        mov              eax, dword ptr [rbp + 256]
                        cmp              eax, 6
                                                                                        jne   .Lx66_0
.Lx66_1:
                        mov              rax, qword ptr [rbp + 576]
                        mov              qword ptr [rbp + 224], rax
                        mov              rax, qword ptr [rbp + 584]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n22_binop_α
.Lx66_0:
                        lea              rdi, [rbp + 576]
                        lea              rsi, [rbp + 256]
                        lea              rdx, [rbp + 224]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n22_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_integer_α:
                        mov              qword ptr [rbp + 176], 6
                        mov              rax, qword ptr [rip + .Lx67_0]
                        mov              qword ptr [rbp + 184], rax
                                                                                        jmp   n23_op75_α
.Lx67_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n21_subscript_α:
                        mov              rdi, qword ptr [rbp + 336]
                        mov              rsi, qword ptr [rbp + 344]
                        mov              rdx, qword ptr [rbp + 352]
                        mov              rcx, qword ptr [rbp + 360]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n13_disjunction_af
                        mov              qword ptr [rbp + 368], rax
                        mov              qword ptr [rbp + 376], rdx
                                                                                        jmp   n24_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n22_binop_α:
                        mov              eax, dword ptr [rbp + 224]
                        cmp              eax, 6
                                                                                        jne   .Lx69_0
                        mov              rax, qword ptr [rbp + 232]
                        mov              rcx, 1
                        add              rax, rcx
                        mov              qword ptr [rbp + 208], 6
                        mov              qword ptr [rbp + 216], rax
                                                                                        jmp   n25_assign_α
.Lx69_0:
                        mov              rdi, qword ptr [rbp + 224]
                        mov              rsi, qword ptr [rbp + 232]
                        mov              rdx, qword ptr [rbp + 256]
                        mov              rcx, qword ptr [rbp + 264]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n17_var_α
                        mov              qword ptr [rbp + 208], rax
                        mov              qword ptr [rbp + 216], rdx
                                                                                        jmp   n25_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n23_op75_α:
                        mov              eax, dword ptr [rbp + 592]
                        cmp              eax, 7
                                                                                        je    .Lx71_1
                        cmp              eax, 6
                                                                                        jne   .Lx71_0
                        mov              eax, dword ptr [rbp + 176]
                        cmp              eax, 6
                                                                                        jne   .Lx71_0
.Lx71_1:
                        mov              rax, qword ptr [rbp + 592]
                        mov              qword ptr [rbp + 144], rax
                        mov              rax, qword ptr [rbp + 600]
                        mov              qword ptr [rbp + 152], rax
                                                                                        jmp   n26_binop_α
.Lx71_0:
                        lea              rdi, [rbp + 592]
                        lea              rsi, [rbp + 176]
                        lea              rdx, [rbp + 144]
                        mov              rcx, 0
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n26_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n24_deref_α:
                        mov              rdi, qword ptr [rbp + 368]
                        mov              rsi, qword ptr [rbp + 376]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n13_disjunction_af
                        mov              qword ptr [rbp + 384], rax
                        mov              qword ptr [rbp + 392], rdx
                                                                                        jmp   n27_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n25_assign_α:
                        mov              rax, qword ptr [rbp + 208]
                        mov              rdx, qword ptr [rbp + 216]
                        mov              qword ptr [rbp + 576], rax
                        mov              qword ptr [rbp + 584], rdx
                                                                                        jmp   n17_var_α
#-----------------------------------------------------------------------------------------------------------------------
n26_binop_α:
                        mov              eax, dword ptr [rbp + 144]
                        cmp              eax, 6
                                                                                        jne   .Lx74_0
                        mov              rax, qword ptr [rbp + 152]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rbp + 128], 6
                        mov              qword ptr [rbp + 136], rax
                                                                                        jmp   n28_assign_α
.Lx74_0:
                        mov              rdi, qword ptr [rbp + 144]
                        mov              rsi, qword ptr [rbp + 152]
                        mov              rdx, qword ptr [rbp + 176]
                        mov              rcx, qword ptr [rbp + 184]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n7_var_α
                        mov              qword ptr [rbp + 128], rax
                        mov              qword ptr [rbp + 136], rdx
                                                                                        jmp   n28_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n27_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                                                                                        jmp   n29_var_α
#-----------------------------------------------------------------------------------------------------------------------
n28_assign_α:
                        mov              rax, qword ptr [rbp + 128]
                        mov              rdx, qword ptr [rbp + 136]
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                        mov              qword ptr [rbp + 112], rax
                        mov              qword ptr [rbp + 120], rdx
                                                                                        jmp   n30_conjunction_α
#-----------------------------------------------------------------------------------------------------------------------
n29_var_α:
                        mov              rax, qword ptr [rbp + 592]
                        mov              qword ptr [rbp + 416], rax
                        mov              rax, qword ptr [rbp + 600]
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   n31_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n30_conjunction_α:
                        mov              rax, qword ptr [rbp + 112]
                        mov              qword ptr [rbp + 96], rax
                        mov              rax, qword ptr [rbp + 120]
                        mov              qword ptr [rbp + 104], rax
                                                                                        jmp   n7_var_α
n30_conjunction_β:
                                                                                        jmp   n7_var_α
#-----------------------------------------------------------------------------------------------------------------------
n31_subscript_α:
                        mov              rdi, qword ptr [rbp + 400]
                        mov              rsi, qword ptr [rbp + 408]
                        mov              rdx, qword ptr [rbp + 416]
                        mov              rcx, qword ptr [rbp + 424]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n13_disjunction_af
                        mov              qword ptr [rbp + 432], rax
                        mov              qword ptr [rbp + 440], rdx
                                                                                        jmp   n32_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n32_deref_α:
                        mov              rdi, qword ptr [rbp + 432]
                        mov              rsi, qword ptr [rbp + 440]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n13_disjunction_af
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                                                                                        jmp   n33_binop_test_α
#-----------------------------------------------------------------------------------------------------------------------
n33_binop_test_α:
                        mov              rdi, qword ptr [rbp + 384]
                        mov              rsi, qword ptr [rbp + 392]
                        mov              rdx, qword ptr [rbp + 448]
                        mov              rcx, qword ptr [rbp + 456]
                        mov              r8d, 17
                        call             rt_jct_relop@PLT
                        test             eax, eax
                                                                                        jz    n13_disjunction_af
                        mov              rdi, qword ptr [rbp + 448]
                        mov              rsi, qword ptr [rbp + 456]
                        call             rt_str_coerce@PLT
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   n34_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:
                        mov              qword ptr [rbp + 304], 1
                        mov              rax, qword ptr [rip + .Lx84_0]
                        mov              qword ptr [rbp + 312], rax
                                                                                        jmp   n35_return_α
.Lx84_0:
                        .quad            .Lx84_0_s
.Lx84_0_s:
                        .string          "no"
#-----------------------------------------------------------------------------------------------------------------------
n35_return_α:
                        mov              rax, qword ptr [rbp + 304]
                        mov              rdx, qword ptr [rbp + 312]
                        mov              qword ptr [rbp + 0], rax
                        mov              qword ptr [rbp + 8], rdx
                                                                                        jmp   proc_palindrome_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_palindrome_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_palindrome_β:
                                                                                        jmp   proc_palindrome_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_palindrome_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 616]
                        lea              rsp, [rbp + 640]
                        mov              rbp, [rbp + 632]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_palindrome_ω:
                        mov              rax, [rbp + 624]
                        lea              rsp, [rbp + 640]
                        mov              rbp, [rbp + 632]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_palindrome_dcα:
                        pop              r11
                        sub              rsp, 656
                        mov              qword ptr [rsp + 632], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rbp + 608], r11
                        lea              rax, [rip + .Lx86_2]
                        mov              qword ptr [rbp + 616], rax
                        lea              rax, [rip + .Lx86_3]
                        mov              qword ptr [rbp + 624], rax
                        mov              qword ptr [rbp + 16], rsi
                        mov              rdi, rbp
                        mov              esi, 576
                        mov              edx, 608
                        mov              ecx, 1
                        mov              r8d, 1
                        mov              r9d, 0
                        call             rt_pl_dc_prep@PLT
                                                                                        jmp   proc_palindrome_α_body
.Lx86_2:
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, rsp
                        add              rcx, -640
                        mov              r11, qword ptr [rsp + -32]
                        mov              rbp, qword ptr [rsp + -8]
                        add              rsp, 16
                        push             r11
                                                                                        jmp   rt_pl_dc_leave_γ@PLT
.Lx86_3:
                        mov              rdi, qword ptr [rsp + 0]
                        mov              rsi, rsp
                        add              rsi, -640
                        mov              r11, qword ptr [rsp + -32]
                        mov              rbp, qword ptr [rsp + -8]
                        add              rsp, 16
                        push             r11
                                                                                        jmp   rt_pl_dc_leave_ω@PLT
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "palindrome"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_palindrome_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 608
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_palindrome_dcα]
                        call             rt_proc_set_dcfn@PLT
                        add              rsp, 8
                        ret
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
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
                        sub              rsp, 344
                        mov              rdi, rsp
                        mov              ecx, 344
                        xor              eax, eax
                        rep stosb
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n87_lit_string_α:
                        mov              qword ptr [rsp + 320], 1
                        mov              rax, qword ptr [rip + .Lx96_0]
                        mov              qword ptr [rsp + 328], rax
                                                                                        jmp   n88_call_proc_staged_α
.Lx96_0:
                        .quad            .Lx96_0_s
.Lx96_0_s:
                        .string          "racecar"
#-----------------------------------------------------------------------------------------------------------------------
n88_call_proc_staged_α:
                        lea              rsi, [rsp + 320]
                        call             proc_palindrome_dcα
                                                                                        jmp   .Lx98_2
.Lx98_2:
                        mov              qword ptr [rsp + 272], rax
                        mov              qword ptr [rsp + 280], rdx
                        cmp              eax, 99
                                                                                        je    n90_lit_string_α
                                                                                        jmp   n89_call_builtin_icon_α
n88_call_proc_staged_β:
                                                                                        jmp   n90_lit_string_α
.Lx98_0:
                        .quad            .Lx98_0_s
.Lx98_0_s:
                        .string          "palindrome"
#-----------------------------------------------------------------------------------------------------------------------
n89_call_builtin_icon_α:
                        mov              rax, qword ptr [rsp + 272]
                        mov              qword ptr [rsp + 240], rax
                        mov              rax, qword ptr [rsp + 280]
                        mov              qword ptr [rsp + 248], rax
                        .section         .rodata
.Lrkfn100:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn100]
                        lea              rsi, [rsp + 240]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 224], rax
                        mov              qword ptr [rsp + 232], rdx
                        cmp              eax, 99
                                                                                        je    n90_lit_string_α
                                                                                        jmp   n90_lit_string_α
n89_call_builtin_icon_β:
                                                                                        jmp   n90_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n90_lit_string_α:
                        mov              qword ptr [rsp + 208], 1
                        mov              rax, qword ptr [rip + .Lx101_0]
                        mov              qword ptr [rsp + 216], rax
                                                                                        jmp   n91_call_proc_staged_α
.Lx101_0:
                        .quad            .Lx101_0_s
.Lx101_0_s:
                        .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n91_call_proc_staged_α:
                        lea              rsi, [rsp + 208]
                        call             proc_palindrome_dcα
                                                                                        jmp   .Lx103_2
.Lx103_2:
                        mov              qword ptr [rsp + 160], rax
                        mov              qword ptr [rsp + 168], rdx
                        cmp              eax, 99
                                                                                        je    n93_lit_string_α
                                                                                        jmp   n92_call_builtin_icon_α
n91_call_proc_staged_β:
                                                                                        jmp   n93_lit_string_α
.Lx103_0:
                        .quad            .Lx103_0_s
.Lx103_0_s:
                        .string          "palindrome"
#-----------------------------------------------------------------------------------------------------------------------
n92_call_builtin_icon_α:
                        mov              rax, qword ptr [rsp + 160]
                        mov              qword ptr [rsp + 128], rax
                        mov              rax, qword ptr [rsp + 168]
                        mov              qword ptr [rsp + 136], rax
                        .section         .rodata
.Lrkfn105:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn105]
                        lea              rsi, [rsp + 128]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 112], rax
                        mov              qword ptr [rsp + 120], rdx
                        cmp              eax, 99
                                                                                        je    n93_lit_string_α
                                                                                        jmp   n93_lit_string_α
n92_call_builtin_icon_β:
                                                                                        jmp   n93_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n93_lit_string_α:
                        mov              qword ptr [rsp + 96], 1
                        mov              rax, qword ptr [rip + .Lx106_0]
                        mov              qword ptr [rsp + 104], rax
                                                                                        jmp   n94_call_proc_staged_α
.Lx106_0:
                        .quad            .Lx106_0_s
.Lx106_0_s:
                        .string          "level"
#-----------------------------------------------------------------------------------------------------------------------
n94_call_proc_staged_α:
                        lea              rsi, [rsp + 96]
                        call             proc_palindrome_dcα
                                                                                        jmp   .Lx108_2
.Lx108_2:
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 99
                                                                                        je    main_ω
                                                                                        jmp   n95_call_builtin_icon_α
n94_call_proc_staged_β:
                                                                                        jmp   main_ω
.Lx108_0:
                        .quad            .Lx108_0_s
.Lx108_0_s:
                        .string          "palindrome"
#-----------------------------------------------------------------------------------------------------------------------
n95_call_builtin_icon_α:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], rax
                        .section         .rodata
.Lrkfn110:              .string          "write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn110]
                        lea              rsi, [rsp + 16]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 99
                                                                                        je    main_ω
                                                                                        jmp   main_ω
n95_call_builtin_icon_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 344
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 344
                        ret
                        .section         .note.GNU-stack,"",@progbits
