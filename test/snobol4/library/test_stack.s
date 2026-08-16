                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             main_init
                        mov              edi, 15
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 15
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
main_init:
                        sub              rsp, 8
                        .section         .rodata
.Lclassspec0:           .string          "slink(snext,sval)"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lclassspec0]
                        call             record_register@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "stack_init"
.Lgvan1:                .string          "stack_push"
.Lgvan2:                .string          "x"
.Lgvan3:                .string          "stack_pop"
.Lgvan4:                .string          "var"
.Lgvan5:                .string          "stack_peek"
.Lgvan6:                .string          "stack_top"
.Lgvan7:                .string          "stack_depth"
.Lgvan8:                .string          "sd"
.Lgvan9:                .string          "stk"
.Lgvan10:               .string          "dummy"
.Lgvan11:               .string          "myvar"
.Lgvan12:               .string          "subject"
.Lgvan13:               .string          "w1"
.Lgvan14:               .string          "w2"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .quad            .Lgvan4
                        .quad            .Lgvan5
                        .quad            .Lgvan6
                        .quad            .Lgvan7
                        .quad            .Lgvan8
                        .quad            .Lgvan9
                        .quad            .Lgvan10
                        .quad            .Lgvan11
                        .quad            .Lgvan12
                        .quad            .Lgvan13
                        .quad            .Lgvan14
                        .section         .text
                        .intel_syntax    noprefix
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                                         jmp   n1_statement_end_α
n0_statement_begin_β:                                                         jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:                                                           jmp   n2_statement_begin_α
#=======================================================================================================================
#         stack_push(99)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                                         jmp   n3_lit_string_α
n2_statement_begin_β:                                                         jmp   n6_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx286_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_call_α
.Lx286_0:               .quad            .Lx286_0_s
.Lx286_0_s:             .string          "slink(snext,sval)"
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:              sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd288:            .string          "DATA"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd288]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx287_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n2_statement_begin_β
.Lx287_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n5_statement_end_α
n4_call_β:              add              rsp, 16
                        add              rsp, 16;                             jmp   n2_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_end_α:                                                           jmp   n6_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_begin_α:                                                         jmp   n7_statement_end_α
n6_statement_begin_β:   add              rsp, 32;                             jmp   n8_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_end_α:     add              rsp, 32;                             jmp   n8_statement_begin_α
#=======================================================================================================================
#         OUTPUT = myvar                  ;* 99
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_begin_α:                                                         jmp   n9_define_α
n8_statement_begin_β:                                                         jmp   n15_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n9_define_α:            mov              rdi, qword ptr [rip + .Lx298_0]
                        mov              rsi, qword ptr [rip + .Lx298_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n11_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n10_statement_end_α
n9_define_β:                                                                  jmp   n8_statement_begin_β
.Lx298_0:               .quad            .Lx298_0_s
.Lx298_0_s:             .string          "stack_init"
.Lx298_1:               .quad            .Lx298_1_s
.Lx298_1_s:             .string          ""
                                                                              jmp   .Lx299_245
#-----------------------------------------------------------------------------------------------------------------------
stack_init_α:           sub              rsp, 48
                        mov              rax, qword ptr [r9 + 0]              # stack_init
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        lea              r10, [rip + stack_init_γ]
                        lea              r11, [rip + stack_init_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n11_statement_begin_α];  jmp   rax
stack_init_γ:           mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
stack_init_ω:           mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx299_245:
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_end_α:                                                          jmp   n15_statement_begin_α
#=======================================================================================================================
# stack_init  <stmt 5, line 54: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_begin_α:                                                        jmp   n12_lit_string_α
n11_statement_begin_β:                                                        jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx304_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n13_assign_α
.Lx304_0:               .quad            .Lx304_0_s
.Lx304_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n13_assign_α:           mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # stk
                        mov              qword ptr [r9 + 152], rdx;           jmp   n14_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_end_α:    add              rsp, 16;                             jmp   RETURN
#=======================================================================================================================
# stack_init_end  <stmt 6, line 56: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_begin_α:                                                        jmp   n16_statement_end_α
n15_statement_begin_β:                                                        jmp   n17_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_end_α:                                                          jmp   n17_statement_begin_α
#=======================================================================================================================
#         subject = 'hello world'
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_begin_α:                                                        jmp   n18_define_α
n17_statement_begin_β:                                                        jmp   n31_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n18_define_α:           mov              rdi, qword ptr [rip + .Lx315_0]
                        mov              rsi, qword ptr [rip + .Lx315_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n20_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n19_statement_end_α
n18_define_β:                                                                 jmp   n17_statement_begin_β
.Lx315_0:               .quad            .Lx315_0_s
.Lx315_0_s:             .string          "stack_push"
.Lx315_1:               .quad            .Lx315_1_s
.Lx315_1_s:             .string          "x"
                                                                              jmp   .Lx316_245
#-----------------------------------------------------------------------------------------------------------------------
stack_push_α:           sub              rsp, 64
                        mov              rax, qword ptr [r9 + 16]             # stack_push
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx316_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 32]             # x
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx316_41
.Lx316_10:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx316_41:              lea              r10, [rip + stack_push_γ]
                        lea              r11, [rip + stack_push_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n20_statement_begin_α];  jmp   rax
stack_push_γ:           mov              rdi, qword ptr [r9 + 16]             # stack_push
                        mov              rsi, qword ptr [r9 + 24]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        cmp              rdx, 0;                              jbe   .Lx316_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 32], rax             # x
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx316_110
.Lx316_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 40], rax
.Lx316_110:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 64
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
stack_push_ω:           mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax             # stack_push
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        cmp              rdx, 0;                              jbe   .Lx316_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 32], rax             # x
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx316_180
.Lx316_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 40], rax
.Lx316_180:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx316_245:
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_end_α:                                                          jmp   n31_statement_begin_α
#=======================================================================================================================
# stack_push  <stmt 8, line 58: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_begin_α:                                                        jmp   n21_var_α
n20_statement_begin_β:                                                        jmp   n26_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n21_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n22_var_α
#-----------------------------------------------------------------------------------------------------------------------
n22_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]             # x
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n23_call_α
n22_var_β:              add              rsp, 16
                        add              rsp, 16;                             jmp   n20_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n23_call_α:             sub              rsp, 16
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
.Lrkfnzd324:            .string          "slink"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd324]
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
                        cmp              eax, 104;                            jne   .Lx323_240
                        add              rsp, 16;                             jmp   n22_var_β
.Lx323_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n24_assign_α
n23_call_β:             add              rsp, 16;                             jmp   n22_var_β
#-----------------------------------------------------------------------------------------------------------------------
n24_assign_α:           mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # stk
                        mov              qword ptr [r9 + 152], rdx;           jmp   n25_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_end_α:    add              rsp, 48;                             jmp   n26_statement_begin_α
#=======================================================================================================================
#         stack_push(w1)
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_begin_α:                                                        jmp   n27_var_α
n26_statement_begin_β:                                                        jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n27_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n28_field_var_α
#-----------------------------------------------------------------------------------------------------------------------
n28_field_var_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx331_0]
                        mov              rsi, qword ptr [rsp + 16]            # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_field_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              eax, 104;                            jne   .Lx331_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n26_statement_begin_β
.Lx331_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n29_assign_α
.Lx331_0:               .quad            .Lx331_0_s
.Lx331_0_s:             .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n29_assign_α:           mov              rax, qword ptr [rsp + 0]             # field_var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax             # stack_push
                        mov              qword ptr [r9 + 24], rdx;            jmp   n30_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_end_α:    add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# stack_push_end  <stmt 10, line 61: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_begin_α:                                                        jmp   n32_statement_end_α
n31_statement_begin_β:                                                        jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:                                                          jmp   n33_statement_begin_α
#=======================================================================================================================
#         OUTPUT = stack_pop()            ;* hello
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:                                                        jmp   n34_define_α
n33_statement_begin_β:                                                        jmp   n71_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n34_define_α:           mov              rdi, qword ptr [rip + .Lx342_0]
                        mov              rsi, qword ptr [rip + .Lx342_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n36_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n35_statement_end_α
n34_define_β:                                                                 jmp   n33_statement_begin_β
.Lx342_0:               .quad            .Lx342_0_s
.Lx342_0_s:             .string          "stack_pop"
.Lx342_1:               .quad            .Lx342_1_s
.Lx342_1_s:             .string          "var"
                                                                              jmp   .Lx343_245
#-----------------------------------------------------------------------------------------------------------------------
stack_pop_α:            sub              rsp, 64
                        mov              rax, qword ptr [r9 + 48]             # stack_pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx343_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 64]             # var
                        mov              qword ptr [r9 + 64], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 72]
                        mov              qword ptr [r9 + 72], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx343_41
.Lx343_10:              mov              rax, qword ptr [r9 + 64]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
.Lx343_41:              lea              r10, [rip + stack_pop_γ]
                        lea              r11, [rip + stack_pop_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n36_statement_begin_α];  jmp   rax
stack_pop_γ:            mov              rdi, qword ptr [r9 + 48]             # stack_pop
                        mov              rsi, qword ptr [r9 + 56]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 48], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 56], rax
                        cmp              rdx, 0;                              jbe   .Lx343_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 64], rax             # var
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx343_110
.Lx343_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 72], rax
.Lx343_110:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 64
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
stack_pop_ω:            mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 48], rax             # stack_pop
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 56], rax
                        cmp              rdx, 0;                              jbe   .Lx343_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 64], rax             # var
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx343_180
.Lx343_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 72], rax
.Lx343_180:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx343_245:
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_end_α:                                                          jmp   n71_statement_begin_α
#=======================================================================================================================
# stack_pop  <stmt 12, line 63: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_begin_α:                                                        jmp   n37_var_α
n36_statement_begin_β:                                                        jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n37_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n38_call_α
#-----------------------------------------------------------------------------------------------------------------------
n38_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd350:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd350]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx349_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n36_statement_begin_β
.Lx349_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n39_statement_end_α
n38_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n36_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_end_α:    add              rsp, 32;                             jmp   n40_statement_begin_α
#=======================================================================================================================
#         <stmt 13, line 64: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_begin_α:                                                        jmp   n41_var_α
n40_statement_begin_β:                                                        jmp   n54_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n41_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # var
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n42_call_α
#-----------------------------------------------------------------------------------------------------------------------
n42_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd357:            .string          "IDENT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd357]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx356_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n40_statement_begin_β
.Lx356_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n43_statement_end_α
n42_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n40_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_end_α:    add              rsp, 32;                             jmp   n44_statement_begin_α
#=======================================================================================================================
#         <stmt 14, line 65: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_begin_α:                                                        jmp   n45_var_α
n44_statement_begin_β:                                                        jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n45_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n46_call_α
#-----------------------------------------------------------------------------------------------------------------------
n46_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd364:            .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd364]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx363_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n44_statement_begin_β
.Lx363_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n47_assign_α
n46_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n44_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n47_assign_α:           mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # stack_pop
                        mov              qword ptr [r9 + 56], rdx;            jmp   n48_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_end_α:    add              rsp, 32;                             jmp   n49_statement_begin_α
#=======================================================================================================================
#         <stmt 15, line 66: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_begin_α:                                                        jmp   n50_var_α
n49_statement_begin_β:                                                        jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n50_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n51_call_α
#-----------------------------------------------------------------------------------------------------------------------
n51_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd372:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd372]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx371_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n49_statement_begin_β
.Lx371_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n52_assign_α
n51_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n49_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n52_assign_α:           mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # stk
                        mov              qword ptr [r9 + 152], rdx;           jmp   n53_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_end_α:    add              rsp, 32;                             jmp   RETURN
#=======================================================================================================================
# stack_pop1  <stmt 16, line 67: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_begin_α:                                                        jmp   n55_var_α
n54_statement_begin_β:                                                        jmp   n61_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n55_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 64]             # var
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n56_call_α
#-----------------------------------------------------------------------------------------------------------------------
n56_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd380:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd380]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx379_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n54_statement_begin_β
.Lx379_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n57_var_α
n56_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n54_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n57_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n58_call_α
n57_var_β:              add              rsp, 16
                        add              rsp, 32;                             jmp   n54_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n58_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd383:            .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd383]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx382_240
                        add              rsp, 16;                             jmp   n57_var_β
.Lx382_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n59_assign_var_α
n58_call_β:             add              rsp, 16;                             jmp   n57_var_β
#-----------------------------------------------------------------------------------------------------------------------
n59_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 48]            # call
                        mov              rsi, qword ptr [rsp + 56]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              eax, 104;                            jne   .Lx384_240
                        add              rsp, 32;                             jmp   n57_var_β
.Lx384_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n60_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_end_α:    add              rsp, 80;                             jmp   n61_statement_begin_α
#=======================================================================================================================
#         <stmt 17, line 68: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_begin_α:                                                        jmp   n62_var_α
n61_statement_begin_β:                                                        jmp   n66_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n62_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n63_call_α
#-----------------------------------------------------------------------------------------------------------------------
n63_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd391:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd391]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx390_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n61_statement_begin_β
.Lx390_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n64_assign_α
n63_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n61_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n64_assign_α:           mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # stk
                        mov              qword ptr [r9 + 152], rdx;           jmp   n65_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_end_α:    add              rsp, 32;                             jmp   n66_statement_begin_α
#=======================================================================================================================
#         <stmt 18, line 69: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_begin_α:                                                        jmp   n67_lit_string_α
n66_statement_begin_β:                                                        jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx397_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n68_call_α
.Lx397_0:               .quad            .Lx397_0_s
.Lx397_0_s:             .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n68_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd399:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd399]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx398_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n66_statement_begin_β
.Lx398_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n69_assign_α
n68_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n66_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n69_assign_α:           mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # stack_pop
                        mov              qword ptr [r9 + 56], rdx;            jmp   n70_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_end_α:    add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# stack_pop_end  <stmt 19, line 71: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_begin_α:                                                        jmp   n72_statement_end_α
n71_statement_begin_β:                                                        jmp   n73_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_end_α:                                                          jmp   n73_statement_begin_α
#=======================================================================================================================
#         <stmt 20, line 72: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_begin_α:                                                        jmp   n74_define_α
n73_statement_begin_β:                                                        jmp   n85_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n74_define_α:           mov              rdi, qword ptr [rip + .Lx410_0]
                        mov              rsi, qword ptr [rip + .Lx410_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n76_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n75_statement_end_α
n74_define_β:                                                                 jmp   n73_statement_begin_β
.Lx410_0:               .quad            .Lx410_0_s
.Lx410_0_s:             .string          "stack_peek"
.Lx410_1:               .quad            .Lx410_1_s
.Lx410_1_s:             .string          ""
                                                                              jmp   .Lx411_245
#-----------------------------------------------------------------------------------------------------------------------
stack_peek_α:           sub              rsp, 48
                        mov              rax, qword ptr [r9 + 80]             # stack_peek
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        lea              r10, [rip + stack_peek_γ]
                        lea              r11, [rip + stack_peek_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n76_statement_begin_α];  jmp   rax
stack_peek_γ:           mov              rdi, qword ptr [r9 + 80]
                        mov              rsi, qword ptr [r9 + 88]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
stack_peek_ω:           mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx411_245:
#-----------------------------------------------------------------------------------------------------------------------
n75_statement_end_α:                                                          jmp   n85_statement_begin_α
#=======================================================================================================================
# stack_peek  <stmt 21, line 73: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n76_statement_begin_α:                                                        jmp   n77_var_α
n76_statement_begin_β:                                                        jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n77_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n78_call_α
#-----------------------------------------------------------------------------------------------------------------------
n78_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd418:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd418]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx417_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n76_statement_begin_β
.Lx417_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n79_statement_end_α
n78_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n76_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_end_α:    add              rsp, 32;                             jmp   n80_statement_begin_α
#=======================================================================================================================
#         <stmt 22, line 74: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n80_statement_begin_α:                                                        jmp   n81_var_α
n80_statement_begin_β:                                                        jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n81_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n82_call_α
#-----------------------------------------------------------------------------------------------------------------------
n82_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd425:            .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd425]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx424_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n80_statement_begin_β
.Lx424_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n83_assign_α
n82_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n80_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n83_assign_α:           mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax             # stack_peek
                        mov              qword ptr [r9 + 88], rdx;            jmp   n84_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n84_statement_end_α:    add              rsp, 32;                             jmp   RETURN
#=======================================================================================================================
# stack_peek_end  <stmt 23, line 76: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n85_statement_begin_α:                                                        jmp   n86_statement_end_α
n85_statement_begin_β:                                                        jmp   n87_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n86_statement_end_α:                                                          jmp   n87_statement_begin_α
#=======================================================================================================================
#         <stmt 24, line 77: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n87_statement_begin_α:                                                        jmp   n88_define_α
n87_statement_begin_β:                                                        jmp   n99_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n88_define_α:           mov              rdi, qword ptr [rip + .Lx436_0]
                        mov              rsi, qword ptr [rip + .Lx436_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n90_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n89_statement_end_α
n88_define_β:                                                                 jmp   n87_statement_begin_β
.Lx436_0:               .quad            .Lx436_0_s
.Lx436_0_s:             .string          "stack_top"
.Lx436_1:               .quad            .Lx436_1_s
.Lx436_1_s:             .string          ""
                                                                              jmp   .Lx437_245
#-----------------------------------------------------------------------------------------------------------------------
stack_top_α:            sub              rsp, 48
                        mov              rax, qword ptr [r9 + 96]             # stack_top
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        lea              r10, [rip + stack_top_γ]
                        lea              r11, [rip + stack_top_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n90_statement_begin_α];  jmp   rax
stack_top_γ:            mov              rdi, qword ptr [r9 + 96]
                        mov              rsi, qword ptr [r9 + 104]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 48
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
stack_top_ω:            mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx437_245:
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_end_α:                                                          jmp   n99_statement_begin_α
#=======================================================================================================================
# stack_top  <stmt 25, line 78: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_begin_α:                                                        jmp   n91_var_α
n90_statement_begin_β:                                                        jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n91_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n92_call_α
#-----------------------------------------------------------------------------------------------------------------------
n92_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd444:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd444]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx443_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n90_statement_begin_β
.Lx443_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n93_statement_end_α
n92_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n90_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n93_statement_end_α:    add              rsp, 32;                             jmp   n94_statement_begin_α
#=======================================================================================================================
#         <stmt 26, line 79: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n94_statement_begin_α:                                                        jmp   n95_var_α
n94_statement_begin_β:                                                        jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n95_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n96_field_var_α
#-----------------------------------------------------------------------------------------------------------------------
n96_field_var_α:        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx450_0]
                        mov              rsi, qword ptr [rsp + 16]            # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_field_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              eax, 104;                            jne   .Lx450_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n94_statement_begin_β
.Lx450_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n97_assign_α
.Lx450_0:               .quad            .Lx450_0_s
.Lx450_0_s:             .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n97_assign_α:           mov              rax, qword ptr [rsp + 0]             # field_var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax             # stack_top
                        mov              qword ptr [r9 + 104], rdx;           jmp   n98_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n98_statement_end_α:    add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# stack_top_end  <stmt 27, line 81: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n99_statement_begin_α:                                                        jmp   n100_statement_end_α
n99_statement_begin_β:                                                        jmp   n101_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n100_statement_end_α:                                                         jmp   n101_statement_begin_α
#=======================================================================================================================
#         <stmt 28, line 82: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n101_statement_begin_α:                                                       jmp   n102_define_α
n101_statement_begin_β:                                                       jmp   n127_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n102_define_α:          mov              rdi, qword ptr [rip + .Lx461_0]
                        mov              rsi, qword ptr [rip + .Lx461_1]
                        mov              edx, 1
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n104_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n103_statement_end_α
n102_define_β:                                                                jmp   n101_statement_begin_β
.Lx461_0:               .quad            .Lx461_0_s
.Lx461_0_s:             .string          "stack_depth"
.Lx461_1:               .quad            .Lx461_1_s
.Lx461_1_s:             .string          "sd"
                                                                              jmp   .Lx462_245
#-----------------------------------------------------------------------------------------------------------------------
stack_depth_α:          sub              rsp, 64
                        mov              rax, qword ptr [r9 + 112]            # stack_depth
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 112], 0
                        mov              qword ptr [r9 + 120], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx462_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 128]            # sd
                        mov              qword ptr [r9 + 128], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 136]
                        mov              qword ptr [r9 + 136], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx462_41
.Lx462_10:              mov              rax, qword ptr [r9 + 128]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
.Lx462_41:              lea              r10, [rip + stack_depth_γ]
                        lea              r11, [rip + stack_depth_ω]
                        push             r11
                        push             r10
                        lea              rax, [rip + n104_statement_begin_α]; jmp   rax
stack_depth_γ:          mov              rdi, qword ptr [r9 + 112]            # stack_depth
                        mov              rsi, qword ptr [r9 + 120]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rdx, 0;                              jbe   .Lx462_80
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 128], rax            # sd
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx462_110
.Lx462_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
.Lx462_110:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 8]
                        add              rsp, 64
                        mov              rax, rdi
                        mov              rdx, rsi;                            jmp   rcx
stack_depth_ω:          mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax            # stack_depth
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rdx, 0;                              jbe   .Lx462_150
                        mov              r11, qword ptr [rcx + 24]
                        add              r11, r8
                        mov              rax, qword ptr [r11 + 0]
                        mov              qword ptr [r9 + 128], rax            # sd
                        mov              rax, qword ptr [r11 + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx462_180
.Lx462_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
.Lx462_180:             mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx462_245:
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_end_α:                                                         jmp   n127_statement_begin_α
#=======================================================================================================================
# stack_depth  <stmt 29, line 83: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_begin_α:                                                       jmp   n105_lit_integer_α
n104_statement_begin_β:                                                       jmp   n108_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n105_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx467_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n106_assign_α
.Lx467_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n106_assign_α:          mov              rax, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # stack_depth
                        mov              qword ptr [r9 + 120], rdx;           jmp   n107_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n107_statement_end_α:   add              rsp, 16;                             jmp   n108_statement_begin_α
#=======================================================================================================================
#         <stmt 30, line 84: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n108_statement_begin_α:                                                       jmp   n109_var_α
n108_statement_begin_β:                                                       jmp   n112_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n109_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n110_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n110_assign_α:          mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax            # sd
                        mov              qword ptr [r9 + 136], rdx;           jmp   n111_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n111_statement_end_α:   add              rsp, 16;                             jmp   n112_statement_begin_α
#=======================================================================================================================
# stk_dep0  <stmt 31, line 85: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n112_statement_begin_α:                                                       jmp   n113_var_α
n112_statement_begin_β:                                                       jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n113_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 128]            # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n114_call_α
#-----------------------------------------------------------------------------------------------------------------------
n114_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd481:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd481]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx480_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n112_statement_begin_β
.Lx480_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n115_statement_end_α
n114_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n112_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n115_statement_end_α:   add              rsp, 32;                             jmp   n116_statement_begin_α
#=======================================================================================================================
#         <stmt 32, line 86: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n116_statement_begin_α:                                                       jmp   n117_var_α
n116_statement_begin_β:                                                       jmp   n122_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n117_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 112]            # stack_depth
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n118_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n118_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx487_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n119_binop_α
n118_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n116_statement_begin_β
.Lx487_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n119_binop_α:           sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # var
                        mov              ecx, dword ptr [rsp + 16]            # lit_integer
                        mov              edx, eax
                        and              edx, ecx
                        cmp              edx, 3;                              jne   .Lx488_2
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              rdx, qword ptr [rsp + 24]            # lit_integer
                        add              rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx488_7
.Lx488_2:               and              edx, 1;                              jz    .Lx488_0
                        mov              rsi, qword ptr [rsp + 40]            # var
                        mov              rdi, qword ptr [rsp + 24]            # lit_integer
                        cmp              eax, 5;                              je    .Lx488_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx488_4
.Lx488_3:               movq             xmm0, rsi
.Lx488_4:               cmp              ecx, 5;                              je    .Lx488_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx488_6
.Lx488_5:               movq             xmm1, rdi
.Lx488_6:               addsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx488_7:                                                                     jmp   n120_assign_α
.Lx488_0:               mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                            jne   .Lx488_240
                        add              rsp, 16;                             jmp   n118_lit_integer_β
.Lx488_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n120_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n120_assign_α:          mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # stack_depth
                        mov              qword ptr [r9 + 120], rdx;           jmp   n121_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n121_statement_end_α:   add              rsp, 48;                             jmp   n122_statement_begin_α
#=======================================================================================================================
#         <stmt 33, line 87: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n122_statement_begin_α:                                                       jmp   n123_var_α
n122_statement_begin_β:                                                       jmp   n112_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n123_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 128]            # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n124_call_α
#-----------------------------------------------------------------------------------------------------------------------
n124_call_α:            sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd496:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd496]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx495_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n122_statement_begin_β
.Lx495_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n125_assign_α
n124_call_β:            add              rsp, 16
                        add              rsp, 16;                             jmp   n122_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n125_assign_α:          mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax            # sd
                        mov              qword ptr [r9 + 136], rdx;           jmp   n126_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n126_statement_end_α:   add              rsp, 32;                             jmp   n112_statement_begin_α
#=======================================================================================================================
# stack_depth_end  <stmt 34, line 89: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n127_statement_begin_α:                                                       jmp   n128_statement_end_α
n127_statement_begin_β:                                                       jmp   n129_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n128_statement_end_α:                                                         jmp   n129_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n129_statement_begin_α:                                                       jmp   n130_statement_end_α
n129_statement_begin_β:                                                       jmp   n131_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n130_statement_end_α:                                                         jmp   n131_statement_begin_α
#=======================================================================================================================
#         <stmt 36, line 90: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n131_statement_begin_α:                                                       jmp   n132_lit_string_α
n131_statement_begin_β:                                                       jmp   n136_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n132_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx510_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n133_lit_integer_α
.Lx510_0:               .quad            .Lx510_0_s
.Lx510_0_s:             .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n133_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx511_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n134_call_α
n133_lit_integer_β:     add              rsp, 16
                        add              rsp, 16;                             jmp   n131_statement_begin_β
.Lx511_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n134_call_α:            sub              rsp, 16
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
.Lrkfnzd513:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd513]
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
                        cmp              eax, 104;                            jne   .Lx512_240
                        add              rsp, 16;                             jmp   n133_lit_integer_β
.Lx512_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n135_statement_end_α
n134_call_β:            add              rsp, 16;                             jmp   n133_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n135_statement_end_α:                                                         jmp   n136_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n136_statement_begin_α:                                                       jmp   n137_statement_end_α
n136_statement_begin_β: add              rsp, 48;                             jmp   n138_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n137_statement_end_α:   add              rsp, 48;                             jmp   n138_statement_begin_α
#=======================================================================================================================
#         <stmt 38, line 93: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n138_statement_begin_α:                                                       jmp   n139_call_α
n138_statement_begin_β:                                                       jmp   n141_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n139_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig523z]
                        lea              rax, [rip + stack_init_α];           jmp   rax
.Lsig523z:              .quad            0
                        .quad            .Lx523_2
                        .quad            .Lx523_2
.Lx523_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx523_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx523_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx523_240
                        add              rsp, 16;                             jmp   n138_statement_begin_β
.Lx523_240:                                                                   jmp   n140_statement_end_α
n139_call_β:                                                                  jmp   n138_statement_begin_β
.Lx523_0:               .quad            .Lx523_0_s
.Lx523_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n140_statement_end_α:   add              rsp, 16;                             jmp   n141_statement_begin_α
#=======================================================================================================================
#         <stmt 39, line 94: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n141_statement_begin_α:                                                       jmp   n142_lit_string_α
n141_statement_begin_β:                                                       jmp   n145_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n142_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx528_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n143_call_α
.Lx528_0:               .quad            .Lx528_0_s
.Lx528_0_s:             .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
n143_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig530z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig530z:              .quad            1
                        .quad            .Lx530_2
                        .quad            .Lx530_2
                        .quad            16
.Lx530_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx530_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx530_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx530_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n141_statement_begin_β
.Lx530_240:                                                                   jmp   n144_statement_end_α
n143_call_β:                                                                  jmp   n141_statement_begin_β
.Lx530_0:               .quad            .Lx530_0_s
.Lx530_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n144_statement_end_α:   add              rsp, 32;                             jmp   n145_statement_begin_α
#=======================================================================================================================
#         <stmt 40, line 95: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n145_statement_begin_α:                                                       jmp   n146_lit_string_α
n145_statement_begin_β:                                                       jmp   n149_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n146_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx535_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n147_call_α
.Lx535_0:               .quad            .Lx535_0_s
.Lx535_0_s:             .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n147_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig537z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig537z:              .quad            1
                        .quad            .Lx537_2
                        .quad            .Lx537_2
                        .quad            16
.Lx537_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx537_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx537_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx537_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n145_statement_begin_β
.Lx537_240:                                                                   jmp   n148_statement_end_α
n147_call_β:                                                                  jmp   n145_statement_begin_β
.Lx537_0:               .quad            .Lx537_0_s
.Lx537_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n148_statement_end_α:   add              rsp, 32;                             jmp   n149_statement_begin_α
#=======================================================================================================================
#         <stmt 41, line 96: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n149_statement_begin_α:                                                       jmp   n150_lit_string_α
n149_statement_begin_β:                                                       jmp   n153_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n150_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx542_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n151_call_α
.Lx542_0:               .quad            .Lx542_0_s
.Lx542_0_s:             .string          "c"
#-----------------------------------------------------------------------------------------------------------------------
n151_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig544z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig544z:              .quad            1
                        .quad            .Lx544_2
                        .quad            .Lx544_2
                        .quad            16
.Lx544_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx544_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx544_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx544_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n149_statement_begin_β
.Lx544_240:                                                                   jmp   n152_statement_end_α
n151_call_β:                                                                  jmp   n149_statement_begin_β
.Lx544_0:               .quad            .Lx544_0_s
.Lx544_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n152_statement_end_α:   add              rsp, 32;                             jmp   n153_statement_begin_α
#=======================================================================================================================
#         <stmt 42, line 97: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n153_statement_begin_α:                                                       jmp   n154_call_α
n153_statement_begin_β:                                                       jmp   n157_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n154_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig550z]
                        lea              rax, [rip + stack_depth_α];          jmp   rax
.Lsig550z:              .quad            0
                        .quad            .Lx550_2
                        .quad            .Lx550_2
.Lx550_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx550_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx550_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx550_240
                        add              rsp, 16;                             jmp   n153_statement_begin_β
.Lx550_240:                                                                   jmp   n155_assign_α
n154_call_β:                                                                  jmp   n153_statement_begin_β
.Lx550_0:               .quad            .Lx550_0_s
.Lx550_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n155_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx551_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n156_statement_end_α
.Lx551_0:               .quad            .Lx551_0_s
.Lx551_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n156_statement_end_α:   add              rsp, 16;                             jmp   n157_statement_begin_α
#=======================================================================================================================
#         <stmt 43, line 98: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n157_statement_begin_α:                                                       jmp   n158_call_α
n157_statement_begin_β:                                                       jmp   n161_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n158_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig557z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig557z:              .quad            0
                        .quad            .Lx557_2
                        .quad            .Lx557_2
.Lx557_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx557_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx557_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx557_240
                        add              rsp, 16;                             jmp   n157_statement_begin_β
.Lx557_240:                                                                   jmp   n159_assign_α
n158_call_β:                                                                  jmp   n157_statement_begin_β
.Lx557_0:               .quad            .Lx557_0_s
.Lx557_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n159_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx558_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n160_statement_end_α
.Lx558_0:               .quad            .Lx558_0_s
.Lx558_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n160_statement_end_α:   add              rsp, 16;                             jmp   n161_statement_begin_α
#=======================================================================================================================
#         <stmt 44, line 99: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n161_statement_begin_α:                                                       jmp   n162_call_α
n161_statement_begin_β:                                                       jmp   n165_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n162_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig564z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig564z:              .quad            0
                        .quad            .Lx564_2
                        .quad            .Lx564_2
.Lx564_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx564_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx564_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx564_240
                        add              rsp, 16;                             jmp   n161_statement_begin_β
.Lx564_240:                                                                   jmp   n163_assign_α
n162_call_β:                                                                  jmp   n161_statement_begin_β
.Lx564_0:               .quad            .Lx564_0_s
.Lx564_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n163_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx565_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n164_statement_end_α
.Lx565_0:               .quad            .Lx565_0_s
.Lx565_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n164_statement_end_α:   add              rsp, 16;                             jmp   n165_statement_begin_α
#=======================================================================================================================
#         <stmt 45, line 100: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n165_statement_begin_α:                                                       jmp   n166_call_α
n165_statement_begin_β:                                                       jmp   n169_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n166_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig571z]
                        lea              rax, [rip + stack_depth_α];          jmp   rax
.Lsig571z:              .quad            0
                        .quad            .Lx571_2
                        .quad            .Lx571_2
.Lx571_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx571_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx571_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx571_240
                        add              rsp, 16;                             jmp   n165_statement_begin_β
.Lx571_240:                                                                   jmp   n167_assign_α
n166_call_β:                                                                  jmp   n165_statement_begin_β
.Lx571_0:               .quad            .Lx571_0_s
.Lx571_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n167_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx572_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n168_statement_end_α
.Lx572_0:               .quad            .Lx572_0_s
.Lx572_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n168_statement_end_α:   add              rsp, 16;                             jmp   n169_statement_begin_α
#=======================================================================================================================
#         <stmt 46, line 101: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n169_statement_begin_α:                                                       jmp   n170_call_α
n169_statement_begin_β:                                                       jmp   n173_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n170_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig578z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig578z:              .quad            0
                        .quad            .Lx578_2
                        .quad            .Lx578_2
.Lx578_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx578_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx578_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx578_240
                        add              rsp, 16;                             jmp   n169_statement_begin_β
.Lx578_240:                                                                   jmp   n171_assign_α
n170_call_β:                                                                  jmp   n169_statement_begin_β
.Lx578_0:               .quad            .Lx578_0_s
.Lx578_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n171_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx579_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n172_statement_end_α
.Lx579_0:               .quad            .Lx579_0_s
.Lx579_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n172_statement_end_α:   add              rsp, 16;                             jmp   n173_statement_begin_α
#=======================================================================================================================
#         <stmt 47, line 102: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n173_statement_begin_α:                                                       jmp   n174_call_α
n173_statement_begin_β:                                                       jmp   n177_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n174_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig585z]
                        lea              rax, [rip + stack_depth_α];          jmp   rax
.Lsig585z:              .quad            0
                        .quad            .Lx585_2
                        .quad            .Lx585_2
.Lx585_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx585_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx585_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx585_240
                        add              rsp, 16;                             jmp   n173_statement_begin_β
.Lx585_240:                                                                   jmp   n175_assign_α
n174_call_β:                                                                  jmp   n173_statement_begin_β
.Lx585_0:               .quad            .Lx585_0_s
.Lx585_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n175_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx586_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n176_statement_end_α
.Lx586_0:               .quad            .Lx586_0_s
.Lx586_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n176_statement_end_α:                                                         jmp   n177_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n177_statement_begin_α:                                                       jmp   n178_statement_end_α
n177_statement_begin_β: add              rsp, 16;                             jmp   n179_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n178_statement_end_α:   add              rsp, 16;                             jmp   n179_statement_begin_α
#=======================================================================================================================
#         <stmt 49, line 105: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n179_statement_begin_α:                                                       jmp   n180_call_α
n179_statement_begin_β:                                                       jmp   n182_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n180_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig596z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig596z:              .quad            0
                        .quad            .Lx596_2
                        .quad            .Lx596_2
.Lx596_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx596_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx596_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx596_240
                        add              rsp, 16;                             jmp   n179_statement_begin_β
.Lx596_240:                                                                   jmp   n181_statement_end_α
n180_call_β:                                                                  jmp   n179_statement_begin_β
.Lx596_0:               .quad            .Lx596_0_s
.Lx596_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n181_statement_end_α:   add              rsp, 16;                             jmp   n186_statement_begin_α
#=======================================================================================================================
#         <stmt 50, line 106: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n182_statement_begin_α:                                                       jmp   n183_lit_string_α
n182_statement_begin_β:                                                       jmp   n190_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n183_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx601_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n184_assign_α
.Lx601_0:               .quad            .Lx601_0_s
.Lx601_0_s:             .string          "empty ok"
#-----------------------------------------------------------------------------------------------------------------------
n184_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx602_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n185_statement_end_α
.Lx602_0:               .quad            .Lx602_0_s
.Lx602_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n185_statement_end_α:   add              rsp, 16;                             jmp   n190_statement_begin_α
#=======================================================================================================================
# bad_s1  <stmt 51, line 107: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n186_statement_begin_α:                                                       jmp   n187_lit_string_α
n186_statement_begin_β:                                                       jmp   n190_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n187_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx607_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n188_assign_α
.Lx607_0:               .quad            .Lx607_0_s
.Lx607_0_s:             .string          "FAIL: empty pop should FRETURN"
#-----------------------------------------------------------------------------------------------------------------------
n188_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx608_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n189_statement_end_α
.Lx608_0:               .quad            .Lx608_0_s
.Lx608_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n189_statement_end_α:   add              rsp, 16;                             jmp   n190_statement_begin_α
#=======================================================================================================================
# s1  <stmt 52, line 109: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n190_statement_begin_α:                                                       jmp   n191_statement_end_α
n190_statement_begin_β:                                                       jmp   n192_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n191_statement_end_α:                                                         jmp   n192_statement_begin_α
#=======================================================================================================================
#         <stmt 53, line 111: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n192_statement_begin_α:                                                       jmp   n193_call_α
n192_statement_begin_β:                                                       jmp   n195_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n193_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig618z]
                        lea              rax, [rip + stack_init_α];           jmp   rax
.Lsig618z:              .quad            0
                        .quad            .Lx618_2
                        .quad            .Lx618_2
.Lx618_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx618_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx618_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx618_240
                        add              rsp, 16;                             jmp   n192_statement_begin_β
.Lx618_240:                                                                   jmp   n194_statement_end_α
n193_call_β:                                                                  jmp   n192_statement_begin_β
.Lx618_0:               .quad            .Lx618_0_s
.Lx618_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n194_statement_end_α:   add              rsp, 16;                             jmp   n195_statement_begin_α
#=======================================================================================================================
#         <stmt 54, line 112: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n195_statement_begin_α:                                                       jmp   n196_lit_string_α
n195_statement_begin_β:                                                       jmp   n199_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n196_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx623_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n197_call_α
.Lx623_0:               .quad            .Lx623_0_s
.Lx623_0_s:             .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n197_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig625z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig625z:              .quad            1
                        .quad            .Lx625_2
                        .quad            .Lx625_2
                        .quad            16
.Lx625_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx625_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx625_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx625_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n195_statement_begin_β
.Lx625_240:                                                                   jmp   n198_statement_end_α
n197_call_β:                                                                  jmp   n195_statement_begin_β
.Lx625_0:               .quad            .Lx625_0_s
.Lx625_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n198_statement_end_α:   add              rsp, 32;                             jmp   n199_statement_begin_α
#=======================================================================================================================
#         <stmt 55, line 113: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n199_statement_begin_α:                                                       jmp   n200_call_α
n199_statement_begin_β:                                                       jmp   n203_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n200_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig631z]
                        lea              rax, [rip + stack_peek_α];           jmp   rax
.Lsig631z:              .quad            0
                        .quad            .Lx631_2
                        .quad            .Lx631_2
.Lx631_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx631_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx631_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx631_240
                        add              rsp, 16;                             jmp   n199_statement_begin_β
.Lx631_240:                                                                   jmp   n201_assign_α
n200_call_β:                                                                  jmp   n199_statement_begin_β
.Lx631_0:               .quad            .Lx631_0_s
.Lx631_0_s:             .string          "stack_peek"
#-----------------------------------------------------------------------------------------------------------------------
n201_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx632_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n202_statement_end_α
.Lx632_0:               .quad            .Lx632_0_s
.Lx632_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n202_statement_end_α:   add              rsp, 16;                             jmp   n203_statement_begin_α
#=======================================================================================================================
#         <stmt 56, line 114: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n203_statement_begin_α:                                                       jmp   n204_call_α
n203_statement_begin_β:                                                       jmp   n207_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n204_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig638z]
                        lea              rax, [rip + stack_depth_α];          jmp   rax
.Lsig638z:              .quad            0
                        .quad            .Lx638_2
                        .quad            .Lx638_2
.Lx638_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx638_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx638_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx638_240
                        add              rsp, 16;                             jmp   n203_statement_begin_β
.Lx638_240:                                                                   jmp   n205_assign_α
n204_call_β:                                                                  jmp   n203_statement_begin_β
.Lx638_0:               .quad            .Lx638_0_s
.Lx638_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n205_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx639_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n206_statement_end_α
.Lx639_0:               .quad            .Lx639_0_s
.Lx639_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n206_statement_end_α:   add              rsp, 16;                             jmp   n207_statement_begin_α
#=======================================================================================================================
#         <stmt 57, line 115: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n207_statement_begin_α:                                                       jmp   n208_call_α
n207_statement_begin_β:                                                       jmp   n211_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n208_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig645z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig645z:              .quad            0
                        .quad            .Lx645_2
                        .quad            .Lx645_2
.Lx645_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx645_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx645_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx645_240
                        add              rsp, 16;                             jmp   n207_statement_begin_β
.Lx645_240:                                                                   jmp   n209_assign_α
n208_call_β:                                                                  jmp   n207_statement_begin_β
.Lx645_0:               .quad            .Lx645_0_s
.Lx645_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n209_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx646_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n210_statement_end_α
.Lx646_0:               .quad            .Lx646_0_s
.Lx646_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n210_statement_end_α:                                                         jmp   n211_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n211_statement_begin_α:                                                       jmp   n212_statement_end_α
n211_statement_begin_β: add              rsp, 16;                             jmp   n213_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n212_statement_end_α:   add              rsp, 16;                             jmp   n213_statement_begin_α
#=======================================================================================================================
#         <stmt 59, line 118: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n213_statement_begin_α:                                                       jmp   n214_call_α
n213_statement_begin_β:                                                       jmp   n216_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n214_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig656z]
                        lea              rax, [rip + stack_init_α];           jmp   rax
.Lsig656z:              .quad            0
                        .quad            .Lx656_2
                        .quad            .Lx656_2
.Lx656_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx656_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx656_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx656_240
                        add              rsp, 16;                             jmp   n213_statement_begin_β
.Lx656_240:                                                                   jmp   n215_statement_end_α
n214_call_β:                                                                  jmp   n213_statement_begin_β
.Lx656_0:               .quad            .Lx656_0_s
.Lx656_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n215_statement_end_α:   add              rsp, 16;                             jmp   n216_statement_begin_α
#=======================================================================================================================
#         <stmt 60, line 119: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n216_statement_begin_α:                                                       jmp   n217_lit_integer_α
n216_statement_begin_β:                                                       jmp   n220_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n217_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx661_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n218_call_α
.Lx661_0:               .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n218_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig663z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig663z:              .quad            1
                        .quad            .Lx663_2
                        .quad            .Lx663_2
                        .quad            16
.Lx663_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx663_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx663_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx663_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n216_statement_begin_β
.Lx663_240:                                                                   jmp   n219_statement_end_α
n218_call_β:                                                                  jmp   n216_statement_begin_β
.Lx663_0:               .quad            .Lx663_0_s
.Lx663_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n219_statement_end_α:   add              rsp, 32;                             jmp   n220_statement_begin_α
#=======================================================================================================================
#         <stmt 61, line 120: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n220_statement_begin_α:                                                       jmp   n221_lit_integer_α
n220_statement_begin_β:                                                       jmp   n224_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n221_lit_integer_α:     sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx668_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n222_call_α
.Lx668_0:               .quad            99
#-----------------------------------------------------------------------------------------------------------------------
n222_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig670z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig670z:              .quad            1
                        .quad            .Lx670_2
                        .quad            .Lx670_2
                        .quad            16
.Lx670_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx670_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx670_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx670_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n220_statement_begin_β
.Lx670_240:                                                                   jmp   n223_statement_end_α
n222_call_β:                                                                  jmp   n220_statement_begin_β
.Lx670_0:               .quad            .Lx670_0_s
.Lx670_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n223_statement_end_α:   add              rsp, 32;                             jmp   n224_statement_begin_α
#=======================================================================================================================
#         <stmt 62, line 121: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n224_statement_begin_α:                                                       jmp   n225_lit_string_α
n224_statement_begin_β:                                                       jmp   n228_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n225_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx675_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n226_call_α
.Lx675_0:               .quad            .Lx675_0_s
.Lx675_0_s:             .string          "myvar"
#-----------------------------------------------------------------------------------------------------------------------
n226_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig677z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig677z:              .quad            1
                        .quad            .Lx677_2
                        .quad            .Lx677_2
                        .quad            16
.Lx677_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx677_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx677_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx677_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n224_statement_begin_β
.Lx677_240:                                                                   jmp   n227_statement_end_α
n226_call_β:                                                                  jmp   n224_statement_begin_β
.Lx677_0:               .quad            .Lx677_0_s
.Lx677_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n227_statement_end_α:   add              rsp, 32;                             jmp   n228_statement_begin_α
#=======================================================================================================================
#         <stmt 63, line 122: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n228_statement_begin_α:                                                       jmp   n229_var_α
n228_statement_begin_β:                                                       jmp   n232_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n229_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 176]            # myvar
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n230_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n230_assign_α:          mov              rsi, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx683_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n231_statement_end_α
.Lx683_0:               .quad            .Lx683_0_s
.Lx683_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n231_statement_end_α:                                                         jmp   n232_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n232_statement_begin_α:                                                       jmp   n233_statement_end_α
n232_statement_begin_β: add              rsp, 16;                             jmp   n234_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n233_statement_end_α:   add              rsp, 16;                             jmp   n234_statement_begin_α
#=======================================================================================================================
#         <stmt 65, line 125: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n234_statement_begin_α:                                                       jmp   n235_call_α
n234_statement_begin_β:                                                       jmp   n237_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n235_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig693z]
                        lea              rax, [rip + stack_init_α];           jmp   rax
.Lsig693z:              .quad            0
                        .quad            .Lx693_2
                        .quad            .Lx693_2
.Lx693_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx693_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx693_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx693_240
                        add              rsp, 16;                             jmp   n234_statement_begin_β
.Lx693_240:                                                                   jmp   n236_statement_end_α
n235_call_β:                                                                  jmp   n234_statement_begin_β
.Lx693_0:               .quad            .Lx693_0_s
.Lx693_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n236_statement_end_α:   add              rsp, 16;                             jmp   n237_statement_begin_α
#=======================================================================================================================
#         <stmt 66, line 126: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n237_statement_begin_α:                                                       jmp   n238_lit_string_α
n237_statement_begin_β:                                                       jmp   n241_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n238_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx698_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n239_assign_α
.Lx698_0:               .quad            .Lx698_0_s
.Lx698_0_s:             .string          "hello world"
#-----------------------------------------------------------------------------------------------------------------------
n239_assign_α:          mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 192], rax            # subject
                        mov              qword ptr [r9 + 200], rdx;           jmp   n240_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n240_statement_end_α:   add              rsp, 16;                             jmp   n241_statement_begin_α
#=======================================================================================================================
#         <stmt 67, line 127: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n241_statement_begin_α:                                                       jmp   n242_var_α
n241_statement_begin_β:                                                       jmp   n273_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n242_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 192]            # subject
                        mov              rdx, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n243_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n243_match_begin_α:     mov              rdi, qword ptr [rsp + 0]             # var
                        mov              rsi, qword ptr [rsp + 8]
                        push             rbp
                        mov              rbp, rsp
                        push             r12                                  # cas_mark
                        push             r13                                  # outer_Σ
                        push             r14                                  # outer_δ
                        push             r15                                  # outer_Δ
                        sub              rsp, 24
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx706_0:               mov              r14d, dword ptr [rbp + -40];         jmp   n244_match_assign_save_α
n243_match_begin_β:     lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx706_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx706_1
                                                                              jmp   .Lx706_0
.Lx706_1:
n243_match_begin_af:    mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              qword ptr [rip + rtccb+56], r10
                        call             rt_match_ctx_restore@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 16;                             jmp   n273_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n244_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n245_match_break_α
n244_match_assign_save_β:
                        add              rsp, 16;                             jmp   n243_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n245_match_break_α:     sub              rsp, 16
                        movsxd           rcx, r14d
.Lx710_0:               cmp              ecx, r15d;                           jl    .Lx710_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n243_match_begin_β
.Lx710_240:             movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32;                             je    .Lx710_1
                        add              ecx, 1;                              jmp   .Lx710_0
.Lx710_1:               mov              dword ptr [rsp + 0], r14d
                        mov              r14d, ecx;                           jmp   n246_match_assign_cond_α
n245_match_break_β:     mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n243_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n246_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n247_match_lit_α
n246_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n245_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n247_match_lit_α:       mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n246_match_assign_cond_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 32;                             jne   n246_match_assign_cond_β
                        add              r14d, 1;                             jmp   n248_match_assign_save_α
n247_match_lit_β:       sub              r14d, 1;                             jmp   n246_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n248_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d;           jmp   n249_match_rem_α
n248_match_assign_save_β:
                        add              rsp, 16;                             jmp   n247_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n249_match_rem_α:       sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d;                          jmp   n250_match_assign_cond_α
n249_match_rem_β:       mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n247_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n250_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n251_match_end_α
n250_match_assign_cond_β:
                        sub              r12, 24;                             jmp   n249_match_rem_β
#-----------------------------------------------------------------------------------------------------------------------
n251_match_end_α:       mov              eax, dword ptr [rbp + -40]           # repl_start
                        mov              dword ptr [rbp + -48], eax
                        mov              qword ptr [rbp + -56], r14           # repl_end
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rbp + -8]            # cas_mark
                        mov              rax, qword ptr [rbp + -32]           # outer_Δ
                        mov              rcx, qword ptr [rbp + -16]           # outer_Σ
                        push             rax
                        push             rcx
                        lea              rcx, [rsp + 0]
                        mov              rsi, r12
                        mov              rdx, r13
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              eax, dword ptr [rbp + -48]           # repl_start
                        mov              dword ptr [r12 + 0], eax
                        mov              rax, qword ptr [rbp + -56]           # repl_end
                        mov              qword ptr [r12 + 8], rax
                        add              r12, 16
                        mov              rsp, rbp                             # frame_whack
                        pop              rbp;                                 jmp   n252_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n252_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx722_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n253_match_replace_α
.Lx722_0:               .quad            .Lx722_0_s
.Lx722_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n253_match_replace_α:   mov              rdi, qword ptr [rip + .Lx724_0]
                        mov              rsi, qword ptr [rsp + 16]            # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              ecx, dword ptr [r12 + -16]           # repl_start
                        mov              r8, qword ptr [r12 + -8]             # repl_end
                        sub              r12, 16
                        lea              r9, [rsp + 0]                        # lit_string
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_match_replace@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16;                             jmp   .Lx724_1
.Lx724_0:               .quad            .Lx724_0_s
.Lx724_0_s:             .string          "subject"
.Lx724_1:                                                                     jmp   n254_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n254_statement_end_α:   add              rsp, 16;                             jmp   n255_statement_begin_α
#=======================================================================================================================
#         <stmt 68, line 128: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n255_statement_begin_α:                                                       jmp   n256_var_α
n255_statement_begin_β:                                                       jmp   n259_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n256_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 208]            # w1
                        mov              rdx, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n257_call_α
#-----------------------------------------------------------------------------------------------------------------------
n257_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig731z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig731z:              .quad            1
                        .quad            .Lx731_2
                        .quad            .Lx731_2
                        .quad            16
.Lx731_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx731_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx731_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx731_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n255_statement_begin_β
.Lx731_240:                                                                   jmp   n258_statement_end_α
n257_call_β:                                                                  jmp   n255_statement_begin_β
.Lx731_0:               .quad            .Lx731_0_s
.Lx731_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n258_statement_end_α:   add              rsp, 32;                             jmp   n259_statement_begin_α
#=======================================================================================================================
#         <stmt 69, line 129: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n259_statement_begin_α:                                                       jmp   n260_var_α
n259_statement_begin_β:                                                       jmp   n263_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n260_var_α:             sub              rsp, 16
                        mov              rax, qword ptr [r9 + 224]            # w2
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n261_call_α
#-----------------------------------------------------------------------------------------------------------------------
n261_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig738z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig738z:              .quad            1
                        .quad            .Lx738_2
                        .quad            .Lx738_2
                        .quad            16
.Lx738_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx738_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx738_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx738_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n259_statement_begin_β
.Lx738_240:                                                                   jmp   n262_statement_end_α
n261_call_β:                                                                  jmp   n259_statement_begin_β
.Lx738_0:               .quad            .Lx738_0_s
.Lx738_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n262_statement_end_α:   add              rsp, 32;                             jmp   n263_statement_begin_α
#=======================================================================================================================
#         <stmt 70, line 130: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n263_statement_begin_α:                                                       jmp   n264_call_α
n263_statement_begin_β:                                                       jmp   n267_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n264_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig744z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig744z:              .quad            0
                        .quad            .Lx744_2
                        .quad            .Lx744_2
.Lx744_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx744_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx744_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx744_240
                        add              rsp, 16;                             jmp   n263_statement_begin_β
.Lx744_240:                                                                   jmp   n265_assign_α
n264_call_β:                                                                  jmp   n263_statement_begin_β
.Lx744_0:               .quad            .Lx744_0_s
.Lx744_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n265_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx745_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n266_statement_end_α
.Lx745_0:               .quad            .Lx745_0_s
.Lx745_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n266_statement_end_α:   add              rsp, 16;                             jmp   n267_statement_begin_α
#=======================================================================================================================
#         <stmt 71, line 131: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n267_statement_begin_α:                                                       jmp   n268_call_α
n267_statement_begin_β:                                                       jmp   n271_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n268_call_α:            sub              rsp, 16
                        lea              rcx, [rip + .Lsig751z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig751z:              .quad            0
                        .quad            .Lx751_2
                        .quad            .Lx751_2
.Lx751_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx751_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx751_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                            jne   .Lx751_240
                        add              rsp, 16;                             jmp   n267_statement_begin_β
.Lx751_240:                                                                   jmp   n269_assign_α
n268_call_β:                                                                  jmp   n267_statement_begin_β
.Lx751_0:               .quad            .Lx751_0_s
.Lx751_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n269_assign_α:          mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx752_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n270_statement_end_α
.Lx752_0:               .quad            .Lx752_0_s
.Lx752_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n270_statement_end_α:   add              rsp, 16;                             jmp   n271_statement_begin_α
#=======================================================================================================================
#         <stmt 72, line 132: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n271_statement_begin_α:                                                       jmp   n272_statement_end_α
n271_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n272_statement_end_α:                                                         jmp   main_γ
#=======================================================================================================================
# bad_s2  <stmt 73, line 133: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n273_statement_begin_α:                                                       jmp   n274_lit_string_α
n273_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n274_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 26
                        mov              rax, qword ptr [rip + .Lx761_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n275_assign_α
.Lx761_0:               .quad            .Lx761_0_s
.Lx761_0_s:             .string          "FAIL: pattern match failed"
#-----------------------------------------------------------------------------------------------------------------------
n275_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx762_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n276_statement_end_α
.Lx762_0:               .quad            .Lx762_0_s
.Lx762_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n276_statement_end_α:   add              rsp, 16;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 pop              rcx
                        add              rsp, 8;                              jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                add              rsp, 8
                        pop              rcx;                                 jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
NRETURN:                mov              rax, qword ptr [rip + rt_g_ret_by_name@GOTPCREL]
                        mov              dword ptr [rax + 0], 1;              jmp   RETURN
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
                        .section         .rodata
.S0:                    .string          "w1"
.S1:                    .string          "w2"
                        .text
                        .section         .note.GNU-stack,"",@progbits
