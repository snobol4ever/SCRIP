                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
FN__EXPR$0:
EXPR$0_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_define_α:            mov              r11, 1;                              jmp   n1_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_string_α:        sub              rsp, 16
                        mov              r11, 2
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx5_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n2_assign_α
.Lx5_0:                 .quad            .Lx5_0_s
.Lx5_0_s:               .string          "abcdef"
#-----------------------------------------------------------------------------------------------------------------------
n2_assign_α:            mov              r11, 3
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 64], rax             # EXPR$0
                        mov              qword ptr [r9 + 72], rdx
                        add              rsp, 16;                             jmp   EXPR$0_γ
#-----------------------------------------------------------------------------------------------------------------------
EXPR$0_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
EXPR$0_β:
                                                                              jmp   EXPR$0_ω
#-----------------------------------------------------------------------------------------------------------------------
EXPR$0_γ:
                                                                              jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
EXPR$0_ω:
                                                                              jmp   qword ptr [rsp + 8]
#-----------------------------------------------------------------------------------------------------------------------
FN__EXPR$1:
EXPR$1_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n7_define_α:            mov              r11, 4;                              jmp   n8_var_α
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:               sub              rsp, 16
                        mov              r11, 5
                        mov              rax, qword ptr [r9 + 16]             # q
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n9_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n9_assign_α:            mov              r11, 6
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax             # EXPR$1
                        mov              qword ptr [r9 + 88], rdx
                        add              rsp, 16;                             jmp   EXPR$1_γ
#-----------------------------------------------------------------------------------------------------------------------
EXPR$1_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
EXPR$1_β:
                                                                              jmp   EXPR$1_ω
#-----------------------------------------------------------------------------------------------------------------------
EXPR$1_γ:
                                                                              jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
EXPR$1_ω:
                                                                              jmp   qword ptr [rsp + 8]
#-----------------------------------------------------------------------------------------------------------------------
FN__EXPR$2:
EXPR$2_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n14_define_α:           mov              r11, 7;                              jmp   n15_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_integer_α:      sub              rsp, 16
                        mov              r11, 8
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx21_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n16_lit_integer_α
.Lx21_0:                .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:      sub              rsp, 16
                        mov              r11, 9
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx22_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n17_call_α
.Lx22_0:                .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n17_call_α:             sub              rsp, 16
                        mov              r11, 10
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
.Lrkfnzd24:             .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd24]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx23_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   EXPR$2_ω
.Lx23_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n18_assign_α
n17_call_β:             mov              r11, 10
                        add              rsp, 16
                        add              rsp, 32;                             jmp   EXPR$2_ω
#-----------------------------------------------------------------------------------------------------------------------
n18_assign_α:           mov              r11, 11
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax             # EXPR$2
                        mov              qword ptr [r9 + 104], rdx
                        add              rsp, 48;                             jmp   EXPR$2_γ
#-----------------------------------------------------------------------------------------------------------------------
EXPR$2_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
EXPR$2_β:
                                                                              jmp   EXPR$2_ω
#-----------------------------------------------------------------------------------------------------------------------
EXPR$2_γ:
                                                                              jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
EXPR$2_ω:
                                                                              jmp   qword ptr [rsp + 8]
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             module_init
                        mov              edi, 7
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 7
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
                        .section         .rodata
.Lgvan0:                .string          "expr"
.Lgvan1:                .string          "q"
.Lgvan2:                .string          "sexp"
.Lgvan3:                .string          "fexp"
.Lgvan4:                .string          "EXPR$0"
.Lgvan5:                .string          "EXPR$1"
.Lgvan6:                .string          "EXPR$2"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .quad            .Lgvan4
                        .quad            .Lgvan5
                        .quad            .Lgvan6
                        .section         .text
                        .intel_syntax    noprefix
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_begin_α:  mov              r11, 12
                        mov              r10, 0;                              jmp   n27_statement_end_α
n26_statement_begin_β:  mov              r11, 12;                             jmp   n28_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_end_α:    mov              r11, 13
                        mov              r10, 1;                              jmp   n28_statement_begin_α
#=======================================================================================================================
#         expr = *('abc' 'def')
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_begin_α:  mov              r11, 14
                        mov              r10, 2;                              jmp   n29_lit_string_α
n28_statement_begin_β:  mov              r11, 14;                             jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_string_α:       sub              rsp, 16
                        mov              r11, 15
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx91_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n30_call_α
.Lx91_0:                .quad            .Lx91_0_s
.Lx91_0_s:              .string          "EXPR$0"
#-----------------------------------------------------------------------------------------------------------------------
n30_call_α:             sub              rsp, 16
                        mov              r11, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd93:             .string          "SNO$MKEXPR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd93]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx92_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n28_statement_begin_β
.Lx92_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n31_assign_α
n30_call_β:             mov              r11, 16
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n28_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_α:           mov              r11, 17
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # expr
                        mov              qword ptr [r9 + 8], rdx;             jmp   n32_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:    mov              r11, 18
                        mov              r10, 2
                        add              rsp, 32;                             jmp   n33_statement_begin_α
#=======================================================================================================================
#         DIFFER(EVAL(expr), 'abcdef')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:  mov              r11, 19
                        mov              r10, 3;                              jmp   n34_var_α
n33_statement_begin_β:  mov              r11, 19;                             jmp   n43_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n34_var_α:              sub              rsp, 16
                        mov              r11, 20
                        mov              rax, qword ptr [r9 + 0]              # expr
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n35_call_α
#-----------------------------------------------------------------------------------------------------------------------
n35_call_α:             sub              rsp, 16
                        mov              r11, 21
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd101:            .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd101]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx100_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n33_statement_begin_β
.Lx100_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n36_lit_string_α
n35_call_β:             mov              r11, 21
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n33_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_string_α:       sub              rsp, 16
                        mov              r11, 22
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx102_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n37_call_α
n36_lit_string_β:       mov              r11, 22
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n33_statement_begin_β
.Lx102_0:               .quad            .Lx102_0_s
.Lx102_0_s:             .string          "abcdef"
#-----------------------------------------------------------------------------------------------------------------------
n37_call_α:             sub              rsp, 16
                        mov              r11, 23
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
.Lrkfnzd104:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd104]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx103_240
                        add              rsp, 16;                             jmp   n36_lit_string_β
.Lx103_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n38_statement_end_α
n37_call_β:             mov              r11, 23
                        add              rsp, 16;                             jmp   n36_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_end_α:    mov              r11, 24
                        mov              r10, 3
                        add              rsp, 64;                             jmp   n39_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1016/001: eval concat expr'     :(END)
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_begin_α:  mov              r11, 25
                        mov              r10, 4;                              jmp   n40_lit_string_α
n39_statement_begin_β:  mov              r11, 25;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n40_lit_string_α:       sub              rsp, 16
                        mov              r11, 26
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 31
                        mov              rax, qword ptr [rip + .Lx109_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n41_assign_α
.Lx109_0:               .quad            .Lx109_0_s
.Lx109_0_s:             .string          "FAIL 1016/001: eval concat expr"
#-----------------------------------------------------------------------------------------------------------------------
n41_assign_α:           mov              r11, 27
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx110_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n42_statement_end_α
.Lx110_0:               .quad            .Lx110_0_s
.Lx110_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_end_α:    mov              r11, 28
                        mov              r10, 4
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e001  <stmt 5, line 9: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_begin_α:  mov              r11, 29
                        mov              r10, 5;                              jmp   n44_statement_end_α
n43_statement_begin_β:  mov              r11, 29;                             jmp   n45_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_end_α:    mov              r11, 30
                        mov              r10, 5;                              jmp   n45_statement_begin_α
#=======================================================================================================================
#         q = 'qqq'
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_begin_α:  mov              r11, 31
                        mov              r10, 6;                              jmp   n46_lit_string_α
n45_statement_begin_β:  mov              r11, 31;                             jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_string_α:       sub              rsp, 16
                        mov              r11, 32
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx119_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n47_assign_α
.Lx119_0:               .quad            .Lx119_0_s
.Lx119_0_s:             .string          "qqq"
#-----------------------------------------------------------------------------------------------------------------------
n47_assign_α:           mov              r11, 33
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax             # q
                        mov              qword ptr [r9 + 24], rdx;            jmp   n48_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_end_α:    mov              r11, 34
                        mov              r10, 6
                        add              rsp, 16;                             jmp   n49_statement_begin_α
#=======================================================================================================================
#         sexp = *q
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_begin_α:  mov              r11, 35
                        mov              r10, 7;                              jmp   n50_lit_string_α
n49_statement_begin_β:  mov              r11, 35;                             jmp   n54_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_string_α:       sub              rsp, 16
                        mov              r11, 36
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx125_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n51_call_α
.Lx125_0:               .quad            .Lx125_0_s
.Lx125_0_s:             .string          "EXPR$1"
#-----------------------------------------------------------------------------------------------------------------------
n51_call_α:             sub              rsp, 16
                        mov              r11, 37
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd127:            .string          "SNO$MKEXPR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd127]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx126_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n49_statement_begin_β
.Lx126_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n52_assign_α
n51_call_β:             mov              r11, 37
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n49_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n52_assign_α:           mov              r11, 38
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax             # sexp
                        mov              qword ptr [r9 + 40], rdx;            jmp   n53_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_end_α:    mov              r11, 39
                        mov              r10, 7
                        add              rsp, 32;                             jmp   n54_statement_begin_α
#=======================================================================================================================
#         DIFFER(EVAL(sexp), 'qqq')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_begin_α:  mov              r11, 40
                        mov              r10, 8;                              jmp   n55_var_α
n54_statement_begin_β:  mov              r11, 40;                             jmp   n64_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n55_var_α:              sub              rsp, 16
                        mov              r11, 41
                        mov              rax, qword ptr [r9 + 32]             # sexp
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n56_call_α
#-----------------------------------------------------------------------------------------------------------------------
n56_call_α:             sub              rsp, 16
                        mov              r11, 42
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd135:            .string          "EVAL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd135]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx134_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n54_statement_begin_β
.Lx134_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n57_lit_string_α
n56_call_β:             mov              r11, 42
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n54_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_string_α:       sub              rsp, 16
                        mov              r11, 43
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx136_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n58_call_α
n57_lit_string_β:       mov              r11, 43
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n54_statement_begin_β
.Lx136_0:               .quad            .Lx136_0_s
.Lx136_0_s:             .string          "qqq"
#-----------------------------------------------------------------------------------------------------------------------
n58_call_α:             sub              rsp, 16
                        mov              r11, 44
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
.Lrkfnzd138:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd138]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx137_240
                        add              rsp, 16;                             jmp   n57_lit_string_β
.Lx137_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n59_statement_end_α
n58_call_β:             mov              r11, 44
                        add              rsp, 16;                             jmp   n57_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_end_α:    mov              r11, 45
                        mov              r10, 8
                        add              rsp, 64;                             jmp   n60_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1016/002: eval var ref'         :(END)
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_begin_α:  mov              r11, 46
                        mov              r10, 9;                              jmp   n61_lit_string_α
n60_statement_begin_β:  mov              r11, 46;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_string_α:       sub              rsp, 16
                        mov              r11, 47
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 27
                        mov              rax, qword ptr [rip + .Lx143_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n62_assign_α
.Lx143_0:               .quad            .Lx143_0_s
.Lx143_0_s:             .string          "FAIL 1016/002: eval var ref"
#-----------------------------------------------------------------------------------------------------------------------
n62_assign_α:           mov              r11, 48
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx144_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n63_statement_end_α
.Lx144_0:               .quad            .Lx144_0_s
.Lx144_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n63_statement_end_α:    mov              r11, 49
                        mov              r10, 9
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e002  <stmt 10, line 16: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_begin_α:  mov              r11, 50
                        mov              r10, 10;                             jmp   n65_statement_end_α
n64_statement_begin_β:  mov              r11, 50;                             jmp   n66_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_end_α:    mov              r11, 51
                        mov              r10, 10;                             jmp   n66_statement_begin_α
#=======================================================================================================================
#         fexp = *IDENT(1, 2)
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_begin_α:  mov              r11, 52
                        mov              r10, 11;                             jmp   n67_lit_string_α
n66_statement_begin_β:  mov              r11, 52;                             jmp   n71_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_string_α:       sub              rsp, 16
                        mov              r11, 53
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 6
                        mov              rax, qword ptr [rip + .Lx153_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n68_call_α
.Lx153_0:               .quad            .Lx153_0_s
.Lx153_0_s:             .string          "EXPR$2"
#-----------------------------------------------------------------------------------------------------------------------
n68_call_α:             sub              rsp, 16
                        mov              r11, 54
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd155:            .string          "SNO$MKEXPR"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd155]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx154_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n66_statement_begin_β
.Lx154_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n69_assign_α
n68_call_β:             mov              r11, 54
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n66_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n69_assign_α:           mov              r11, 55
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # fexp
                        mov              qword ptr [r9 + 56], rdx;            jmp   n70_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_end_α:    mov              r11, 56
                        mov              r10, 11
                        add              rsp, 32;                             jmp   n71_statement_begin_α
#=======================================================================================================================
#         EVAL(fexp)                                     :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_begin_α:  mov              r11, 57
                        mov              r10, 12;                             jmp   n72_var_α
n71_statement_begin_β:  mov              r11, 57;                             jmp   n79_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n72_var_α:              sub              rsp, 16
                        mov              r11, 58
                        mov              rax, qword ptr [r9 + 48]             # fexp
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n73_call_α
#-----------------------------------------------------------------------------------------------------------------------
n73_call_α:             sub              rsp, 16
                        mov              r11, 59
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd163:            .string          "EVAL"
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
                        add              rsp, 16;                             jmp   n71_statement_begin_β
.Lx162_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n74_statement_end_α
n73_call_β:             mov              r11, 59
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n71_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_end_α:    mov              r11, 60
                        mov              r10, 12
                        add              rsp, 32;                             jmp   n75_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1016/003: eval failing expr should fail' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n75_statement_begin_α:  mov              r11, 61
                        mov              r10, 13;                             jmp   n76_lit_string_α
n75_statement_begin_β:  mov              r11, 61;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n76_lit_string_α:       sub              rsp, 16
                        mov              r11, 62
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 44
                        mov              rax, qword ptr [rip + .Lx168_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n77_assign_α
.Lx168_0:               .quad            .Lx168_0_s
.Lx168_0_s:             .string          "FAIL 1016/003: eval failing expr should fail"
#-----------------------------------------------------------------------------------------------------------------------
n77_assign_α:           mov              r11, 63
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx169_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n78_statement_end_α
.Lx169_0:               .quad            .Lx169_0_s
.Lx169_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_end_α:    mov              r11, 64
                        mov              r10, 13
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e003  <stmt 14, line 22: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_begin_α:  mov              r11, 65
                        mov              r10, 14;                             jmp   n80_statement_end_α
n79_statement_begin_β:  mov              r11, 65;                             jmp   n81_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n80_statement_end_α:    mov              r11, 66
                        mov              r10, 14;                             jmp   n81_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1016_eval (3/3)'
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_begin_α:  mov              r11, 67
                        mov              r10, 15;                             jmp   n82_lit_string_α
n81_statement_begin_β:  mov              r11, 67;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n82_lit_string_α:       sub              rsp, 16
                        mov              r11, 68
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 20
                        mov              rax, qword ptr [rip + .Lx178_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n83_assign_α
.Lx178_0:               .quad            .Lx178_0_s
.Lx178_0_s:             .string          "PASS 1016_eval (3/3)"
#-----------------------------------------------------------------------------------------------------------------------
n83_assign_α:           mov              r11, 69
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx179_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n84_statement_end_α
.Lx179_0:               .quad            .Lx179_0_s
.Lx179_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n84_statement_end_α:    mov              r11, 70
                        mov              r10, 15
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
module_init:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "EXPR$0"
                        .align           8
.Lstartup_pnames0:
                        .quad            0
                        .align           8
.Lstartup_prec0:
                        .quad            .Lstartup_pname0
                        .quad            FN__EXPR$0
                        .quad            0
                        .quad            0
                        .quad            .Lstartup_pnames0
                        .long            0
                        .long            0
                        .long            48
                        .long            17
                        .long            0
                        .long            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_prec0]
                        call             rt_proc_register_rec@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "EXPR$1"
                        .align           8
.Lstartup_pnames1:
                        .quad            0
                        .align           8
.Lstartup_prec1:
                        .quad            .Lstartup_pname1
                        .quad            FN__EXPR$1
                        .quad            0
                        .quad            0
                        .quad            .Lstartup_pnames1
                        .long            0
                        .long            0
                        .long            48
                        .long            17
                        .long            0
                        .long            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_prec1]
                        call             rt_proc_register_rec@PLT
                        .section         .rodata
.Lstartup_pname2:       .string          "EXPR$2"
                        .align           8
.Lstartup_pnames2:
                        .quad            0
                        .align           8
.Lstartup_prec2:
                        .quad            .Lstartup_pname2
                        .quad            FN__EXPR$2
                        .quad            0
                        .quad            0
                        .quad            .Lstartup_pnames2
                        .long            0
                        .long            0
                        .long            128
                        .long            17
                        .long            0
                        .long            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_prec2]
                        call             rt_proc_register_rec@PLT
                        add              rsp, 8
                        ret
                        .section         .note.GNU-stack,"",@progbits
