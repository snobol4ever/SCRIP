                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             module_init
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
n0_statement_begin_α:   mov              r11, 1
                        mov              r10, 0;                              jmp   n1_statement_end_α
n0_statement_begin_β:   mov              r11, 1;                              jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:     mov              r11, 2
                        mov              r10, 1;                              jmp   n2_statement_begin_α
#=======================================================================================================================
#         stack_push(99)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:   mov              r11, 3
                        mov              r10, 2;                              jmp   n3_lit_string_α
n2_statement_begin_β:   mov              r11, 3;                              jmp   n6_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:        sub              rsp, 16
                        mov              r11, 4
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 17
                        mov              rax, qword ptr [rip + .Lx289_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_call_α
.Lx289_0:               .quad            .Lx289_0_s
.Lx289_0_s:             .string          "slink(snext,sval)"
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:              sub              rsp, 16
                        mov              r11, 5
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd291:            .string          "DATA"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd291]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 262164
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx290_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n2_statement_begin_β
.Lx290_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n5_statement_end_α
n4_call_β:              mov              r11, 5
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n2_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_end_α:     mov              r11, 6
                        mov              r10, 2;                              jmp   n6_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_begin_α:   mov              r11, 7
                        mov              r10, 0;                              jmp   n7_statement_end_α
n6_statement_begin_β:   mov              r11, 7
                        add              rsp, 32;                             jmp   n8_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_end_α:     mov              r11, 8
                        mov              r10, 3
                        add              rsp, 32;                             jmp   n8_statement_begin_α
#=======================================================================================================================
#         OUTPUT = myvar                  ;* 99
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_begin_α:   mov              r11, 9
                        mov              r10, 4;                              jmp   n9_define_α
n8_statement_begin_β:   mov              r11, 9;                              jmp   n15_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n9_define_α:            mov              r11, 10
                        mov              rdi, qword ptr [rip + .Lx301_0]
                        mov              rsi, qword ptr [rip + .Lx301_1]
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
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx301_0]
                        lea              rsi, [rip + stack_init_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n10_statement_end_α
n9_define_β:            mov              r11, 10;                             jmp   n8_statement_begin_β
.Lx301_0:               .quad            .Lx301_0_s
.Lx301_0_s:             .string          "stack_init"
.Lx301_1:               .quad            .Lx301_1_s
.Lx301_1_s:             .string          ""
                                                                              jmp   .Lx302_245
#-----------------------------------------------------------------------------------------------------------------------
stack_init_α:           sub              rsp, 48
                        mov              rax, qword ptr [r9 + 0]              # stack_init
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        lea              rcx, [rip + stack_init_γ]
                        lea              rax, [rip + stack_init_ω]
                        push             rax
                        push             rcx
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
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx302_245:
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_end_α:    mov              r11, 11
                        mov              r10, 4;                              jmp   n15_statement_begin_α
#=======================================================================================================================
# stack_init  <stmt 5, line 54: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_begin_α:  mov              r11, 12
                        mov              r10, 5;                              jmp   n12_lit_string_α
n11_statement_begin_β:  mov              r11, 12;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_string_α:       sub              rsp, 16
                        mov              r11, 13
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx307_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n13_assign_α
.Lx307_0:               .quad            .Lx307_0_s
.Lx307_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n13_assign_α:           mov              r11, 14
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # stk
                        mov              qword ptr [r9 + 152], rdx;           jmp   n14_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_end_α:    mov              r11, 15
                        mov              r10, 5
                        add              rsp, 16;                             jmp   RETURN
#=======================================================================================================================
# stack_init_end  <stmt 6, line 56: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_begin_α:  mov              r11, 16
                        mov              r10, 6;                              jmp   n16_statement_end_α
n15_statement_begin_β:  mov              r11, 16;                             jmp   n17_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_end_α:    mov              r11, 17
                        mov              r10, 6;                              jmp   n17_statement_begin_α
#=======================================================================================================================
#         subject = 'hello world'
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_begin_α:  mov              r11, 18
                        mov              r10, 7;                              jmp   n18_define_α
n17_statement_begin_β:  mov              r11, 18;                             jmp   n31_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n18_define_α:           mov              r11, 19
                        mov              rdi, qword ptr [rip + .Lx318_0]
                        mov              rsi, qword ptr [rip + .Lx318_1]
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
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx318_0]
                        lea              rsi, [rip + stack_push_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n19_statement_end_α
n18_define_β:           mov              r11, 19;                             jmp   n17_statement_begin_β
.Lx318_0:               .quad            .Lx318_0_s
.Lx318_0_s:             .string          "stack_push"
.Lx318_1:               .quad            .Lx318_1_s
.Lx318_1_s:             .string          "x"
                                                                              jmp   .Lx319_245
#-----------------------------------------------------------------------------------------------------------------------
stack_push_α:           sub              rsp, 64
                        mov              rax, qword ptr [r9 + 16]             # stack_push
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx319_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 32]             # x
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx319_41
.Lx319_10:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx319_41:              lea              rcx, [rip + stack_push_γ]
                        lea              rax, [rip + stack_push_ω]
                        push             rax
                        push             rcx
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
                        cmp              rdx, 0;                              jbe   .Lx319_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 32], rax             # x
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx319_110
.Lx319_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 40], rax
.Lx319_110:             mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx319_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 32], rax             # x
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx319_180
.Lx319_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 40], rax
.Lx319_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx319_245:
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_end_α:    mov              r11, 20
                        mov              r10, 7;                              jmp   n31_statement_begin_α
#=======================================================================================================================
# stack_push  <stmt 8, line 58: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_begin_α:  mov              r11, 21
                        mov              r10, 8;                              jmp   n21_var_α
n20_statement_begin_β:  mov              r11, 21;                             jmp   n26_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n21_var_α:              sub              rsp, 16
                        mov              r11, 22
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n22_var_α
#-----------------------------------------------------------------------------------------------------------------------
n22_var_α:              sub              rsp, 16
                        mov              r11, 23
                        mov              rax, qword ptr [r9 + 32]             # x
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n23_call_α
n22_var_β:              mov              r11, 23
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n20_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n23_call_α:             sub              rsp, 16
                        mov              r11, 24
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
.Lrkfnzd327:            .string          "slink"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd327]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              ecx, 327680
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx326_240
                        add              rsp, 16;                             jmp   n22_var_β
.Lx326_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n24_assign_α
n23_call_β:             mov              r11, 24
                        add              rsp, 16;                             jmp   n22_var_β
#-----------------------------------------------------------------------------------------------------------------------
n24_assign_α:           mov              r11, 25
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # stk
                        mov              qword ptr [r9 + 152], rdx;           jmp   n25_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_end_α:    mov              r11, 26
                        mov              r10, 8
                        add              rsp, 48;                             jmp   n26_statement_begin_α
#=======================================================================================================================
#         stack_push(w1)
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_begin_α:  mov              r11, 27
                        mov              r10, 9;                              jmp   n27_var_α
n26_statement_begin_β:  mov              r11, 27;                             jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n27_var_α:              sub              rsp, 16
                        mov              r11, 28
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n28_field_var_α
#-----------------------------------------------------------------------------------------------------------------------
n28_field_var_α:        sub              rsp, 16
                        mov              r11, 29
                        mov              rdi, qword ptr [rip + .Lx334_0]
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
                        cmp              al, 104;                             jne   .Lx334_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n26_statement_begin_β
.Lx334_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n29_assign_α
.Lx334_0:               .quad            .Lx334_0_s
.Lx334_0_s:             .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n29_assign_α:           mov              r11, 30
                        mov              rax, qword ptr [rsp + 0]             # field_var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax             # stack_push
                        mov              qword ptr [r9 + 24], rdx;            jmp   n30_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_end_α:    mov              r11, 31
                        mov              r10, 9
                        add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# stack_push_end  <stmt 10, line 61: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_begin_α:  mov              r11, 32
                        mov              r10, 10;                             jmp   n32_statement_end_α
n31_statement_begin_β:  mov              r11, 32;                             jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:    mov              r11, 33
                        mov              r10, 10;                             jmp   n33_statement_begin_α
#=======================================================================================================================
#         OUTPUT = stack_pop()            ;* hello
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:  mov              r11, 34
                        mov              r10, 11;                             jmp   n34_define_α
n33_statement_begin_β:  mov              r11, 34;                             jmp   n72_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n34_define_α:           mov              r11, 35
                        mov              rdi, qword ptr [rip + .Lx345_0]
                        mov              rsi, qword ptr [rip + .Lx345_1]
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
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx345_0]
                        lea              rsi, [rip + stack_pop_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n35_statement_end_α
n34_define_β:           mov              r11, 35;                             jmp   n33_statement_begin_β
.Lx345_0:               .quad            .Lx345_0_s
.Lx345_0_s:             .string          "stack_pop"
.Lx345_1:               .quad            .Lx345_1_s
.Lx345_1_s:             .string          "var"
                                                                              jmp   .Lx346_245
#-----------------------------------------------------------------------------------------------------------------------
stack_pop_α:            sub              rsp, 64
                        mov              rax, qword ptr [r9 + 48]             # stack_pop
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx346_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 64]             # var
                        mov              qword ptr [r9 + 64], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 72]
                        mov              qword ptr [r9 + 72], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx346_41
.Lx346_10:              mov              rax, qword ptr [r9 + 64]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
.Lx346_41:              lea              rcx, [rip + stack_pop_γ]
                        lea              rax, [rip + stack_pop_ω]
                        push             rax
                        push             rcx
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
                        cmp              rdx, 0;                              jbe   .Lx346_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 64], rax             # var
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx346_110
.Lx346_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 72], rax
.Lx346_110:             mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx346_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 64], rax             # var
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx346_180
.Lx346_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 72], rax
.Lx346_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx346_245:
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_end_α:    mov              r11, 36
                        mov              r10, 11;                             jmp   n72_statement_begin_α
#=======================================================================================================================
# stack_pop  <stmt 12, line 63: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_begin_α:  mov              r11, 37
                        mov              r10, 12;                             jmp   n37_var_α
n36_statement_begin_β:  mov              r11, 37;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n37_var_α:              sub              rsp, 16
                        mov              r11, 38
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n38_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_string_α:       sub              rsp, 16
                        mov              r11, 39
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx352_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n39_differ_α
n38_lit_string_β:       mov              r11, 39
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n36_statement_begin_β
.Lx352_0:               .quad            .Lx352_0_s
.Lx352_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n39_differ_α:           sub              rsp, 16
                        mov              r11, 40
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            je    .Lx354_240
                        add              rsp, 16;                             jmp   n38_lit_string_β
.Lx354_240:                                                                   jmp   n40_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_end_α:    mov              r11, 41
                        mov              r10, 12
                        add              rsp, 48;                             jmp   n41_statement_begin_α
#=======================================================================================================================
#         <stmt 13, line 64: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n41_statement_begin_α:  mov              r11, 42
                        mov              r10, 13;                             jmp   n42_var_α
n41_statement_begin_β:  mov              r11, 42;                             jmp   n56_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n42_var_α:              sub              rsp, 16
                        mov              r11, 43
                        mov              rax, qword ptr [r9 + 64]             # var
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n43_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n43_lit_string_α:       sub              rsp, 16
                        mov              r11, 44
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx360_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n44_ident_α
n43_lit_string_β:       mov              r11, 44
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n41_statement_begin_β
.Lx360_0:               .quad            .Lx360_0_s
.Lx360_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n44_ident_α:            sub              rsp, 16
                        mov              r11, 45
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            jne   .Lx362_240
                        add              rsp, 16;                             jmp   n43_lit_string_β
.Lx362_240:                                                                   jmp   n45_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_end_α:    mov              r11, 46
                        mov              r10, 13
                        add              rsp, 48;                             jmp   n46_statement_begin_α
#=======================================================================================================================
#         <stmt 14, line 65: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_begin_α:  mov              r11, 47
                        mov              r10, 14;                             jmp   n47_var_α
n46_statement_begin_β:  mov              r11, 47;                             jmp   n51_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n47_var_α:              sub              rsp, 16
                        mov              r11, 48
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n48_call_α
#-----------------------------------------------------------------------------------------------------------------------
n48_call_α:             sub              rsp, 16
                        mov              r11, 49
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd369:            .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd369]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 262144
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx368_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n46_statement_begin_β
.Lx368_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n49_assign_α
n48_call_β:             mov              r11, 49
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n46_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n49_assign_α:           mov              r11, 50
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # stack_pop
                        mov              qword ptr [r9 + 56], rdx;            jmp   n50_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_end_α:    mov              r11, 51
                        mov              r10, 14
                        add              rsp, 32;                             jmp   n51_statement_begin_α
#=======================================================================================================================
#         <stmt 15, line 66: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_begin_α:  mov              r11, 52
                        mov              r10, 15;                             jmp   n52_var_α
n51_statement_begin_β:  mov              r11, 52;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n52_var_α:              sub              rsp, 16
                        mov              r11, 53
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n53_call_α
#-----------------------------------------------------------------------------------------------------------------------
n53_call_α:             sub              rsp, 16
                        mov              r11, 54
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd377:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd377]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 327680
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx376_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n51_statement_begin_β
.Lx376_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n54_assign_α
n53_call_β:             mov              r11, 54
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n51_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n54_assign_α:           mov              r11, 55
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # stk
                        mov              qword ptr [r9 + 152], rdx;           jmp   n55_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_end_α:    mov              r11, 56
                        mov              r10, 15
                        add              rsp, 32;                             jmp   RETURN
#=======================================================================================================================
# stack_pop1  <stmt 16, line 67: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n56_statement_begin_α:  mov              r11, 57
                        mov              r10, 16;                             jmp   n57_var_α
n56_statement_begin_β:  mov              r11, 57;                             jmp   n63_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n57_var_α:              sub              rsp, 16
                        mov              r11, 58
                        mov              rax, qword ptr [r9 + 64]             # var
                        mov              rdx, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n58_call_α
#-----------------------------------------------------------------------------------------------------------------------
n58_call_α:             sub              rsp, 16
                        mov              r11, 59
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd385:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd385]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 524341
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx384_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n56_statement_begin_β
.Lx384_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n59_var_α
n58_call_β:             mov              r11, 59
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n56_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n59_var_α:              sub              rsp, 16
                        mov              r11, 60
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n60_call_α
n59_var_β:              mov              r11, 60
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n56_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n60_call_α:             sub              rsp, 16
                        mov              r11, 61
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd388:            .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd388]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 262144
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx387_240
                        add              rsp, 16;                             jmp   n59_var_β
.Lx387_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n61_assign_var_α
n60_call_β:             mov              r11, 61
                        add              rsp, 16;                             jmp   n59_var_β
#-----------------------------------------------------------------------------------------------------------------------
n61_assign_var_α:       sub              rsp, 16
                        mov              r11, 62
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
                        cmp              al, 104;                             jne   .Lx389_240
                        add              rsp, 32;                             jmp   n59_var_β
.Lx389_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n62_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n62_statement_end_α:    mov              r11, 63
                        mov              r10, 16
                        add              rsp, 80;                             jmp   n63_statement_begin_α
#=======================================================================================================================
#         <stmt 17, line 68: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n63_statement_begin_α:  mov              r11, 64
                        mov              r10, 17;                             jmp   n64_var_α
n63_statement_begin_β:  mov              r11, 64;                             jmp   n68_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n64_var_α:              sub              rsp, 16
                        mov              r11, 65
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n65_call_α
#-----------------------------------------------------------------------------------------------------------------------
n65_call_α:             sub              rsp, 16
                        mov              r11, 66
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd396:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd396]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 327680
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx395_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n63_statement_begin_β
.Lx395_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n66_assign_α
n65_call_β:             mov              r11, 66
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n63_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n66_assign_α:           mov              r11, 67
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 144], rax            # stk
                        mov              qword ptr [r9 + 152], rdx;           jmp   n67_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_end_α:    mov              r11, 68
                        mov              r10, 17
                        add              rsp, 32;                             jmp   n68_statement_begin_α
#=======================================================================================================================
#         <stmt 18, line 69: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_begin_α:  mov              r11, 69
                        mov              r10, 18;                             jmp   n69_lit_name_α
n68_statement_begin_β:  mov              r11, 69;                             jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_name_α:         sub              rsp, 16
                        mov              r11, 70
                        mov              qword ptr [rsp + 0], 40              # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx402_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n70_assign_α
.Lx402_0:               .quad            .Lx402_0_s
.Lx402_0_s:             .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n70_assign_α:           mov              r11, 71
                        mov              rax, qword ptr [rsp + 0]             # lit_name
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # stack_pop
                        mov              qword ptr [r9 + 56], rdx;            jmp   n71_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_end_α:    mov              r11, 72
                        mov              r10, 18
                        add              rsp, 16;                             jmp   NRETURN
#=======================================================================================================================
# stack_pop_end  <stmt 19, line 71: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_begin_α:  mov              r11, 73
                        mov              r10, 19;                             jmp   n73_statement_end_α
n72_statement_begin_β:  mov              r11, 73;                             jmp   n74_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_end_α:    mov              r11, 74
                        mov              r10, 19;                             jmp   n74_statement_begin_α
#=======================================================================================================================
#         <stmt 20, line 72: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_begin_α:  mov              r11, 75
                        mov              r10, 20;                             jmp   n75_define_α
n74_statement_begin_β:  mov              r11, 75;                             jmp   n87_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n75_define_α:           mov              r11, 76
                        mov              rdi, qword ptr [rip + .Lx413_0]
                        mov              rsi, qword ptr [rip + .Lx413_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n77_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx413_0]
                        lea              rsi, [rip + stack_peek_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n76_statement_end_α
n75_define_β:           mov              r11, 76;                             jmp   n74_statement_begin_β
.Lx413_0:               .quad            .Lx413_0_s
.Lx413_0_s:             .string          "stack_peek"
.Lx413_1:               .quad            .Lx413_1_s
.Lx413_1_s:             .string          ""
                                                                              jmp   .Lx414_245
#-----------------------------------------------------------------------------------------------------------------------
stack_peek_α:           sub              rsp, 48
                        mov              rax, qword ptr [r9 + 80]             # stack_peek
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 88]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 80], 0
                        mov              qword ptr [r9 + 88], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        lea              rcx, [rip + stack_peek_γ]
                        lea              rax, [rip + stack_peek_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n77_statement_begin_α];  jmp   rax
stack_peek_γ:           mov              rdi, qword ptr [r9 + 80]
                        mov              rsi, qword ptr [r9 + 88]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 80], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 88], rax
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
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx414_245:
#-----------------------------------------------------------------------------------------------------------------------
n76_statement_end_α:    mov              r11, 77
                        mov              r10, 20;                             jmp   n87_statement_begin_α
#=======================================================================================================================
# stack_peek  <stmt 21, line 73: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_begin_α:  mov              r11, 78
                        mov              r10, 21;                             jmp   n78_var_α
n77_statement_begin_β:  mov              r11, 78;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n78_var_α:              sub              rsp, 16
                        mov              r11, 79
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n79_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_string_α:       sub              rsp, 16
                        mov              r11, 80
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx420_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n80_differ_α
n79_lit_string_β:       mov              r11, 80
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n77_statement_begin_β
.Lx420_0:               .quad            .Lx420_0_s
.Lx420_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n80_differ_α:           sub              rsp, 16
                        mov              r11, 81
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            je    .Lx422_240
                        add              rsp, 16;                             jmp   n79_lit_string_β
.Lx422_240:                                                                   jmp   n81_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_end_α:    mov              r11, 82
                        mov              r10, 21
                        add              rsp, 48;                             jmp   n82_statement_begin_α
#=======================================================================================================================
#         <stmt 22, line 74: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n82_statement_begin_α:  mov              r11, 83
                        mov              r10, 22;                             jmp   n83_var_α
n82_statement_begin_β:  mov              r11, 83;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n83_var_α:              sub              rsp, 16
                        mov              r11, 84
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n84_call_α
#-----------------------------------------------------------------------------------------------------------------------
n84_call_α:             sub              rsp, 16
                        mov              r11, 85
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd429:            .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd429]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 262144
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx428_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n82_statement_begin_β
.Lx428_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n85_assign_α
n84_call_β:             mov              r11, 85
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n82_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n85_assign_α:           mov              r11, 86
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax             # stack_peek
                        mov              qword ptr [r9 + 88], rdx;            jmp   n86_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n86_statement_end_α:    mov              r11, 87
                        mov              r10, 22
                        add              rsp, 32;                             jmp   RETURN
#=======================================================================================================================
# stack_peek_end  <stmt 23, line 76: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n87_statement_begin_α:  mov              r11, 88
                        mov              r10, 23;                             jmp   n88_statement_end_α
n87_statement_begin_β:  mov              r11, 88;                             jmp   n89_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_end_α:    mov              r11, 89
                        mov              r10, 23;                             jmp   n89_statement_begin_α
#=======================================================================================================================
#         <stmt 24, line 77: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_begin_α:  mov              r11, 90
                        mov              r10, 24;                             jmp   n90_define_α
n89_statement_begin_β:  mov              r11, 90;                             jmp   n102_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n90_define_α:           mov              r11, 91
                        mov              rdi, qword ptr [rip + .Lx440_0]
                        mov              rsi, qword ptr [rip + .Lx440_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n92_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx440_0]
                        lea              rsi, [rip + stack_top_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n91_statement_end_α
n90_define_β:           mov              r11, 91;                             jmp   n89_statement_begin_β
.Lx440_0:               .quad            .Lx440_0_s
.Lx440_0_s:             .string          "stack_top"
.Lx440_1:               .quad            .Lx440_1_s
.Lx440_1_s:             .string          ""
                                                                              jmp   .Lx441_245
#-----------------------------------------------------------------------------------------------------------------------
stack_top_α:            sub              rsp, 48
                        mov              rax, qword ptr [r9 + 96]             # stack_top
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 104]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 96], 0
                        mov              qword ptr [r9 + 104], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        lea              rcx, [rip + stack_top_γ]
                        lea              rax, [rip + stack_top_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n92_statement_begin_α];  jmp   rax
stack_top_γ:            mov              rdi, qword ptr [r9 + 96]
                        mov              rsi, qword ptr [r9 + 104]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 96], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 104], rax
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
                        mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 48
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx441_245:
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_end_α:    mov              r11, 92
                        mov              r10, 24;                             jmp   n102_statement_begin_α
#=======================================================================================================================
# stack_top  <stmt 25, line 78: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n92_statement_begin_α:  mov              r11, 93
                        mov              r10, 25;                             jmp   n93_var_α
n92_statement_begin_β:  mov              r11, 93;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n93_var_α:              sub              rsp, 16
                        mov              r11, 94
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n94_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n94_lit_string_α:       sub              rsp, 16
                        mov              r11, 95
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx447_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n95_differ_α
n94_lit_string_β:       mov              r11, 95
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n92_statement_begin_β
.Lx447_0:               .quad            .Lx447_0_s
.Lx447_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n95_differ_α:           sub              rsp, 16
                        mov              r11, 96
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            je    .Lx449_240
                        add              rsp, 16;                             jmp   n94_lit_string_β
.Lx449_240:                                                                   jmp   n96_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n96_statement_end_α:    mov              r11, 97
                        mov              r10, 25
                        add              rsp, 48;                             jmp   n97_statement_begin_α
#=======================================================================================================================
#         <stmt 26, line 79: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n97_statement_begin_α:  mov              r11, 98
                        mov              r10, 26;                             jmp   n98_var_α
n97_statement_begin_β:  mov              r11, 98;                             jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n98_var_α:              sub              rsp, 16
                        mov              r11, 99
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n99_field_var_α
#-----------------------------------------------------------------------------------------------------------------------
n99_field_var_α:        sub              rsp, 16
                        mov              r11, 100
                        mov              rdi, qword ptr [rip + .Lx455_0]
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
                        cmp              al, 104;                             jne   .Lx455_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n97_statement_begin_β
.Lx455_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n100_assign_α
.Lx455_0:               .quad            .Lx455_0_s
.Lx455_0_s:             .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n100_assign_α:          mov              r11, 101
                        mov              rax, qword ptr [rsp + 0]             # field_var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax             # stack_top
                        mov              qword ptr [r9 + 104], rdx;           jmp   n101_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n101_statement_end_α:   mov              r11, 102
                        mov              r10, 26
                        add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# stack_top_end  <stmt 27, line 81: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_begin_α: mov              r11, 103
                        mov              r10, 27;                             jmp   n103_statement_end_α
n102_statement_begin_β: mov              r11, 103;                            jmp   n104_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_end_α:   mov              r11, 104
                        mov              r10, 27;                             jmp   n104_statement_begin_α
#=======================================================================================================================
#         <stmt 28, line 82: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_begin_α: mov              r11, 105
                        mov              r10, 28;                             jmp   n105_define_α
n104_statement_begin_β: mov              r11, 105;                            jmp   n131_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n105_define_α:          mov              r11, 106
                        mov              rdi, qword ptr [rip + .Lx466_0]
                        mov              rsi, qword ptr [rip + .Lx466_1]
                        mov              edx, 1
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n107_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx466_0]
                        lea              rsi, [rip + stack_depth_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n106_statement_end_α
n105_define_β:          mov              r11, 106;                            jmp   n104_statement_begin_β
.Lx466_0:               .quad            .Lx466_0_s
.Lx466_0_s:             .string          "stack_depth"
.Lx466_1:               .quad            .Lx466_1_s
.Lx466_1_s:             .string          "sd"
                                                                              jmp   .Lx467_245
#-----------------------------------------------------------------------------------------------------------------------
stack_depth_α:          sub              rsp, 64
                        mov              rax, qword ptr [r9 + 112]            # stack_depth
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 112], 0
                        mov              qword ptr [r9 + 120], 0
                        mov              qword ptr [rsp + 32], rcx
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        cmp              rdx, 0;                              jbe   .Lx467_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 128]            # sd
                        mov              qword ptr [r9 + 128], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 136]
                        mov              qword ptr [r9 + 136], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx467_41
.Lx467_10:              mov              rax, qword ptr [r9 + 128]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
.Lx467_41:              lea              rcx, [rip + stack_depth_γ]
                        lea              rax, [rip + stack_depth_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n107_statement_begin_α]; jmp   rax
stack_depth_γ:          mov              rdi, qword ptr [r9 + 112]            # stack_depth
                        mov              rsi, qword ptr [r9 + 120]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rdx, 0;                              jbe   .Lx467_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 128], rax            # sd
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx467_110
.Lx467_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
.Lx467_110:             mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx467_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 128], rax            # sd
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx467_180
.Lx467_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
.Lx467_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx467_245:
#-----------------------------------------------------------------------------------------------------------------------
n106_statement_end_α:   mov              r11, 107
                        mov              r10, 28;                             jmp   n131_statement_begin_α
#=======================================================================================================================
# stack_depth  <stmt 29, line 83: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n107_statement_begin_α: mov              r11, 108
                        mov              r10, 29;                             jmp   n108_lit_integer_α
n107_statement_begin_β: mov              r11, 108;                            jmp   n111_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n108_lit_integer_α:     sub              rsp, 16
                        mov              r11, 109
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx472_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n109_assign_α
.Lx472_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n109_assign_α:          mov              r11, 110
                        mov              rax, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # stack_depth
                        mov              qword ptr [r9 + 120], rdx;           jmp   n110_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n110_statement_end_α:   mov              r11, 111
                        mov              r10, 29
                        add              rsp, 16;                             jmp   n111_statement_begin_α
#=======================================================================================================================
#         <stmt 30, line 84: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n111_statement_begin_α: mov              r11, 112
                        mov              r10, 30;                             jmp   n112_var_α
n111_statement_begin_β: mov              r11, 112;                            jmp   n115_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n112_var_α:             sub              rsp, 16
                        mov              r11, 113
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n113_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n113_assign_α:          mov              r11, 114
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax            # sd
                        mov              qword ptr [r9 + 136], rdx;           jmp   n114_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n114_statement_end_α:   mov              r11, 115
                        mov              r10, 30
                        add              rsp, 16;                             jmp   n115_statement_begin_α
#=======================================================================================================================
# stk_dep0  <stmt 31, line 85: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n115_statement_begin_α: mov              r11, 116
                        mov              r10, 31;                             jmp   n116_var_α
n115_statement_begin_β: mov              r11, 116;                            jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n116_var_α:             sub              rsp, 16
                        mov              r11, 117
                        mov              rax, qword ptr [r9 + 128]            # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n117_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n117_lit_string_α:      sub              rsp, 16
                        mov              r11, 118
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx485_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n118_differ_α
n117_lit_string_β:      mov              r11, 118
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n115_statement_begin_β
.Lx485_0:               .quad            .Lx485_0_s
.Lx485_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n118_differ_α:          sub              rsp, 16
                        mov              r11, 119
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            je    .Lx487_240
                        add              rsp, 16;                             jmp   n117_lit_string_β
.Lx487_240:                                                                   jmp   n119_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n119_statement_end_α:   mov              r11, 120
                        mov              r10, 31
                        add              rsp, 48;                             jmp   n120_statement_begin_α
#=======================================================================================================================
#         <stmt 32, line 86: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n120_statement_begin_α: mov              r11, 121
                        mov              r10, 32;                             jmp   n121_var_α
n120_statement_begin_β: mov              r11, 121;                            jmp   n126_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n121_var_α:             sub              rsp, 16
                        mov              r11, 122
                        mov              rax, qword ptr [r9 + 112]            # stack_depth
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n122_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n122_lit_integer_α:     sub              rsp, 16
                        mov              r11, 123
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx493_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n123_binop_α
n122_lit_integer_β:     mov              r11, 123
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n120_statement_begin_β
.Lx493_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n123_binop_α:           sub              rsp, 16
                        mov              r11, 124
                        mov              ecx, dword ptr [rsp + 32]            # var
                        mov              rax, qword ptr [rsp + 40]
                        cmp              cl, 3;                               jne   .Lx494_2
                        add              rax, 1
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx494_7
.Lx494_2:               mov              eax, ecx
                        mov              edx, ecx
                        and              edx, 1;                              jz    .Lx494_0
                        mov              rsi, qword ptr [rsp + 40]            # var
                        mov              rdi, 1
                        cmp              al, 5;                               je    .Lx494_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx494_4
.Lx494_3:               movq             xmm0, rsi
.Lx494_4:               cvtsi2sd         xmm1, rdi
                        addsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx494_7:                                                                     jmp   n124_assign_α
.Lx494_0:               mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_add@PLT
                        cmp              al, 104;                             jne   .Lx494_240
                        add              rsp, 16;                             jmp   n122_lit_integer_β
.Lx494_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n124_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n124_assign_α:          mov              r11, 125
                        mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # stack_depth
                        mov              qword ptr [r9 + 120], rdx;           jmp   n125_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n125_statement_end_α:   mov              r11, 126
                        mov              r10, 32
                        add              rsp, 48;                             jmp   n126_statement_begin_α
#=======================================================================================================================
#         <stmt 33, line 87: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n126_statement_begin_α: mov              r11, 127
                        mov              r10, 33;                             jmp   n127_var_α
n126_statement_begin_β: mov              r11, 127;                            jmp   n115_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n127_var_α:             sub              rsp, 16
                        mov              r11, 128
                        mov              rax, qword ptr [r9 + 128]            # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n128_call_α
#-----------------------------------------------------------------------------------------------------------------------
n128_call_α:            sub              rsp, 16
                        mov              r11, 129
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd502:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd502]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              ecx, 327680
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr_bl@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx501_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n126_statement_begin_β
.Lx501_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n129_assign_α
n128_call_β:            mov              r11, 129
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n126_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n129_assign_α:          mov              r11, 130
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax            # sd
                        mov              qword ptr [r9 + 136], rdx;           jmp   n130_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n130_statement_end_α:   mov              r11, 131
                        mov              r10, 33
                        add              rsp, 32;                             jmp   n115_statement_begin_α
#=======================================================================================================================
# stack_depth_end  <stmt 34, line 89: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n131_statement_begin_α: mov              r11, 132
                        mov              r10, 34;                             jmp   n132_statement_end_α
n131_statement_begin_β: mov              r11, 132;                            jmp   n133_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n132_statement_end_α:   mov              r11, 133
                        mov              r10, 34;                             jmp   n133_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n133_statement_begin_α: mov              r11, 134
                        mov              r10, 0;                              jmp   n134_statement_end_α
n133_statement_begin_β: mov              r11, 134;                            jmp   n135_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n134_statement_end_α:   mov              r11, 135
                        mov              r10, 35;                             jmp   n135_statement_begin_α
#=======================================================================================================================
#         <stmt 36, line 90: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n135_statement_begin_α: mov              r11, 136
                        mov              r10, 36;                             jmp   n136_lit_integer_α
n135_statement_begin_β: mov              r11, 136;                            jmp   n139_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n136_lit_integer_α:     sub              rsp, 16
                        mov              r11, 137
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx516_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n137_keyword_assign_snobol4_α
.Lx516_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n137_keyword_assign_snobol4_α:
                        sub              rsp, 16
                        mov              r11, 138
                        mov              rdi, qword ptr [rip + .Lx517_0]
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_kw_write_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              al, 104;                             jne   .Lx517_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n135_statement_begin_β
.Lx517_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n138_statement_end_α
.Lx517_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n138_statement_end_α:   mov              r11, 139
                        mov              r10, 36;                             jmp   n139_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n139_statement_begin_α: mov              r11, 140
                        mov              r10, 0;                              jmp   n140_statement_end_α
n139_statement_begin_β: mov              r11, 140
                        add              rsp, 32;                             jmp   n141_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n140_statement_end_α:   mov              r11, 141
                        mov              r10, 37
                        add              rsp, 32;                             jmp   n141_statement_begin_α
#=======================================================================================================================
#         <stmt 38, line 93: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n141_statement_begin_α: mov              r11, 142
                        mov              r10, 38;                             jmp   n142_call_α
n141_statement_begin_β: mov              r11, 142;                            jmp   n144_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n142_call_α:            sub              rsp, 16
                        mov              r11, 143
                        lea              rcx, [rip + .Lsig527z]
                        lea              rax, [rip + stack_init_α];           jmp   rax
.Lsig527z:              .quad            0
                        .quad            .Lx527_2
                        .quad            .Lx527_2
.Lx527_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx527_29
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
.Lx527_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx527_240
                        add              rsp, 16;                             jmp   n141_statement_begin_β
.Lx527_240:                                                                   jmp   n143_statement_end_α
n142_call_β:            mov              r11, 143;                            jmp   n141_statement_begin_β
.Lx527_0:               .quad            .Lx527_0_s
.Lx527_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n143_statement_end_α:   mov              r11, 144
                        mov              r10, 38
                        add              rsp, 16;                             jmp   n144_statement_begin_α
#=======================================================================================================================
#         <stmt 39, line 94: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n144_statement_begin_α: mov              r11, 145
                        mov              r10, 39;                             jmp   n145_lit_string_α
n144_statement_begin_β: mov              r11, 145;                            jmp   n148_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n145_lit_string_α:      sub              rsp, 16
                        mov              r11, 146
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx532_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n146_call_α
.Lx532_0:               .quad            .Lx532_0_s
.Lx532_0_s:             .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
n146_call_α:            sub              rsp, 16
                        mov              r11, 147
                        lea              rcx, [rip + .Lsig534z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig534z:              .quad            1
                        .quad            .Lx534_2
                        .quad            .Lx534_2
                        .quad            16
.Lx534_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx534_29
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
.Lx534_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx534_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n144_statement_begin_β
.Lx534_240:                                                                   jmp   n147_statement_end_α
n146_call_β:            mov              r11, 147;                            jmp   n144_statement_begin_β
.Lx534_0:               .quad            .Lx534_0_s
.Lx534_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n147_statement_end_α:   mov              r11, 148
                        mov              r10, 39
                        add              rsp, 32;                             jmp   n148_statement_begin_α
#=======================================================================================================================
#         <stmt 40, line 95: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n148_statement_begin_α: mov              r11, 149
                        mov              r10, 40;                             jmp   n149_lit_string_α
n148_statement_begin_β: mov              r11, 149;                            jmp   n152_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n149_lit_string_α:      sub              rsp, 16
                        mov              r11, 150
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx539_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n150_call_α
.Lx539_0:               .quad            .Lx539_0_s
.Lx539_0_s:             .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n150_call_α:            sub              rsp, 16
                        mov              r11, 151
                        lea              rcx, [rip + .Lsig541z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig541z:              .quad            1
                        .quad            .Lx541_2
                        .quad            .Lx541_2
                        .quad            16
.Lx541_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx541_29
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
.Lx541_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx541_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n148_statement_begin_β
.Lx541_240:                                                                   jmp   n151_statement_end_α
n150_call_β:            mov              r11, 151;                            jmp   n148_statement_begin_β
.Lx541_0:               .quad            .Lx541_0_s
.Lx541_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n151_statement_end_α:   mov              r11, 152
                        mov              r10, 40
                        add              rsp, 32;                             jmp   n152_statement_begin_α
#=======================================================================================================================
#         <stmt 41, line 96: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n152_statement_begin_α: mov              r11, 153
                        mov              r10, 41;                             jmp   n153_lit_string_α
n152_statement_begin_β: mov              r11, 153;                            jmp   n156_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n153_lit_string_α:      sub              rsp, 16
                        mov              r11, 154
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx546_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n154_call_α
.Lx546_0:               .quad            .Lx546_0_s
.Lx546_0_s:             .string          "c"
#-----------------------------------------------------------------------------------------------------------------------
n154_call_α:            sub              rsp, 16
                        mov              r11, 155
                        lea              rcx, [rip + .Lsig548z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig548z:              .quad            1
                        .quad            .Lx548_2
                        .quad            .Lx548_2
                        .quad            16
.Lx548_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx548_29
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
.Lx548_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx548_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n152_statement_begin_β
.Lx548_240:                                                                   jmp   n155_statement_end_α
n154_call_β:            mov              r11, 155;                            jmp   n152_statement_begin_β
.Lx548_0:               .quad            .Lx548_0_s
.Lx548_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n155_statement_end_α:   mov              r11, 156
                        mov              r10, 41
                        add              rsp, 32;                             jmp   n156_statement_begin_α
#=======================================================================================================================
#         <stmt 42, line 97: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n156_statement_begin_α: mov              r11, 157
                        mov              r10, 42;                             jmp   n157_call_α
n156_statement_begin_β: mov              r11, 157;                            jmp   n160_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n157_call_α:            sub              rsp, 16
                        mov              r11, 158
                        lea              rcx, [rip + .Lsig554z]
                        lea              rax, [rip + stack_depth_α];          jmp   rax
.Lsig554z:              .quad            0
                        .quad            .Lx554_2
                        .quad            .Lx554_2
.Lx554_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx554_29
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
.Lx554_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx554_240
                        add              rsp, 16;                             jmp   n156_statement_begin_β
.Lx554_240:                                                                   jmp   n158_assign_α
n157_call_β:            mov              r11, 158;                            jmp   n156_statement_begin_β
.Lx554_0:               .quad            .Lx554_0_s
.Lx554_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n158_assign_α:          mov              r11, 159
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx555_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n159_statement_end_α
.Lx555_0:               .quad            .Lx555_0_s
.Lx555_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n159_statement_end_α:   mov              r11, 160
                        mov              r10, 42
                        add              rsp, 16;                             jmp   n160_statement_begin_α
#=======================================================================================================================
#         <stmt 43, line 98: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n160_statement_begin_α: mov              r11, 161
                        mov              r10, 43;                             jmp   n161_call_α
n160_statement_begin_β: mov              r11, 161;                            jmp   n164_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n161_call_α:            sub              rsp, 16
                        mov              r11, 162
                        lea              rcx, [rip + .Lsig561z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig561z:              .quad            0
                        .quad            .Lx561_2
                        .quad            .Lx561_2
.Lx561_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx561_29
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
.Lx561_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx561_240
                        add              rsp, 16;                             jmp   n160_statement_begin_β
.Lx561_240:                                                                   jmp   n162_assign_α
n161_call_β:            mov              r11, 162;                            jmp   n160_statement_begin_β
.Lx561_0:               .quad            .Lx561_0_s
.Lx561_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n162_assign_α:          mov              r11, 163
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx562_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n163_statement_end_α
.Lx562_0:               .quad            .Lx562_0_s
.Lx562_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n163_statement_end_α:   mov              r11, 164
                        mov              r10, 43
                        add              rsp, 16;                             jmp   n164_statement_begin_α
#=======================================================================================================================
#         <stmt 44, line 99: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n164_statement_begin_α: mov              r11, 165
                        mov              r10, 44;                             jmp   n165_call_α
n164_statement_begin_β: mov              r11, 165;                            jmp   n168_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n165_call_α:            sub              rsp, 16
                        mov              r11, 166
                        lea              rcx, [rip + .Lsig568z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig568z:              .quad            0
                        .quad            .Lx568_2
                        .quad            .Lx568_2
.Lx568_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx568_29
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
.Lx568_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx568_240
                        add              rsp, 16;                             jmp   n164_statement_begin_β
.Lx568_240:                                                                   jmp   n166_assign_α
n165_call_β:            mov              r11, 166;                            jmp   n164_statement_begin_β
.Lx568_0:               .quad            .Lx568_0_s
.Lx568_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n166_assign_α:          mov              r11, 167
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx569_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n167_statement_end_α
.Lx569_0:               .quad            .Lx569_0_s
.Lx569_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n167_statement_end_α:   mov              r11, 168
                        mov              r10, 44
                        add              rsp, 16;                             jmp   n168_statement_begin_α
#=======================================================================================================================
#         <stmt 45, line 100: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n168_statement_begin_α: mov              r11, 169
                        mov              r10, 45;                             jmp   n169_call_α
n168_statement_begin_β: mov              r11, 169;                            jmp   n172_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n169_call_α:            sub              rsp, 16
                        mov              r11, 170
                        lea              rcx, [rip + .Lsig575z]
                        lea              rax, [rip + stack_depth_α];          jmp   rax
.Lsig575z:              .quad            0
                        .quad            .Lx575_2
                        .quad            .Lx575_2
.Lx575_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx575_29
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
.Lx575_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx575_240
                        add              rsp, 16;                             jmp   n168_statement_begin_β
.Lx575_240:                                                                   jmp   n170_assign_α
n169_call_β:            mov              r11, 170;                            jmp   n168_statement_begin_β
.Lx575_0:               .quad            .Lx575_0_s
.Lx575_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n170_assign_α:          mov              r11, 171
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx576_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n171_statement_end_α
.Lx576_0:               .quad            .Lx576_0_s
.Lx576_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n171_statement_end_α:   mov              r11, 172
                        mov              r10, 45
                        add              rsp, 16;                             jmp   n172_statement_begin_α
#=======================================================================================================================
#         <stmt 46, line 101: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n172_statement_begin_α: mov              r11, 173
                        mov              r10, 46;                             jmp   n173_call_α
n172_statement_begin_β: mov              r11, 173;                            jmp   n176_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n173_call_α:            sub              rsp, 16
                        mov              r11, 174
                        lea              rcx, [rip + .Lsig582z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig582z:              .quad            0
                        .quad            .Lx582_2
                        .quad            .Lx582_2
.Lx582_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx582_29
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
.Lx582_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx582_240
                        add              rsp, 16;                             jmp   n172_statement_begin_β
.Lx582_240:                                                                   jmp   n174_assign_α
n173_call_β:            mov              r11, 174;                            jmp   n172_statement_begin_β
.Lx582_0:               .quad            .Lx582_0_s
.Lx582_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n174_assign_α:          mov              r11, 175
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx583_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n175_statement_end_α
.Lx583_0:               .quad            .Lx583_0_s
.Lx583_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n175_statement_end_α:   mov              r11, 176
                        mov              r10, 46
                        add              rsp, 16;                             jmp   n176_statement_begin_α
#=======================================================================================================================
#         <stmt 47, line 102: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n176_statement_begin_α: mov              r11, 177
                        mov              r10, 47;                             jmp   n177_call_α
n176_statement_begin_β: mov              r11, 177;                            jmp   n180_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n177_call_α:            sub              rsp, 16
                        mov              r11, 178
                        lea              rcx, [rip + .Lsig589z]
                        lea              rax, [rip + stack_depth_α];          jmp   rax
.Lsig589z:              .quad            0
                        .quad            .Lx589_2
                        .quad            .Lx589_2
.Lx589_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx589_29
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
.Lx589_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx589_240
                        add              rsp, 16;                             jmp   n176_statement_begin_β
.Lx589_240:                                                                   jmp   n178_assign_α
n177_call_β:            mov              r11, 178;                            jmp   n176_statement_begin_β
.Lx589_0:               .quad            .Lx589_0_s
.Lx589_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n178_assign_α:          mov              r11, 179
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx590_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n179_statement_end_α
.Lx590_0:               .quad            .Lx590_0_s
.Lx590_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n179_statement_end_α:   mov              r11, 180
                        mov              r10, 47;                             jmp   n180_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n180_statement_begin_α: mov              r11, 181
                        mov              r10, 0;                              jmp   n181_statement_end_α
n180_statement_begin_β: mov              r11, 181
                        add              rsp, 16;                             jmp   n182_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n181_statement_end_α:   mov              r11, 182
                        mov              r10, 48
                        add              rsp, 16;                             jmp   n182_statement_begin_α
#=======================================================================================================================
#         <stmt 49, line 105: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n182_statement_begin_α: mov              r11, 183
                        mov              r10, 49;                             jmp   n183_call_α
n182_statement_begin_β: mov              r11, 183;                            jmp   n185_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n183_call_α:            sub              rsp, 16
                        mov              r11, 184
                        lea              rcx, [rip + .Lsig600z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig600z:              .quad            0
                        .quad            .Lx600_2
                        .quad            .Lx600_2
.Lx600_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx600_29
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
.Lx600_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx600_240
                        add              rsp, 16;                             jmp   n182_statement_begin_β
.Lx600_240:                                                                   jmp   n184_statement_end_α
n183_call_β:            mov              r11, 184;                            jmp   n182_statement_begin_β
.Lx600_0:               .quad            .Lx600_0_s
.Lx600_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n184_statement_end_α:   mov              r11, 185
                        mov              r10, 49
                        add              rsp, 16;                             jmp   n189_statement_begin_α
#=======================================================================================================================
#         <stmt 50, line 106: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n185_statement_begin_α: mov              r11, 186
                        mov              r10, 50;                             jmp   n186_lit_string_α
n185_statement_begin_β: mov              r11, 186;                            jmp   n193_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n186_lit_string_α:      sub              rsp, 16
                        mov              r11, 187
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx605_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n187_assign_α
.Lx605_0:               .quad            .Lx605_0_s
.Lx605_0_s:             .string          "empty ok"
#-----------------------------------------------------------------------------------------------------------------------
n187_assign_α:          mov              r11, 188
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx606_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n188_statement_end_α
.Lx606_0:               .quad            .Lx606_0_s
.Lx606_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n188_statement_end_α:   mov              r11, 189
                        mov              r10, 50
                        add              rsp, 16;                             jmp   n193_statement_begin_α
#=======================================================================================================================
# bad_s1  <stmt 51, line 107: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n189_statement_begin_α: mov              r11, 190
                        mov              r10, 51;                             jmp   n190_lit_string_α
n189_statement_begin_β: mov              r11, 190;                            jmp   n193_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n190_lit_string_α:      sub              rsp, 16
                        mov              r11, 191
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx611_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n191_assign_α
.Lx611_0:               .quad            .Lx611_0_s
.Lx611_0_s:             .string          "FAIL: empty pop should FRETURN"
#-----------------------------------------------------------------------------------------------------------------------
n191_assign_α:          mov              r11, 192
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx612_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n192_statement_end_α
.Lx612_0:               .quad            .Lx612_0_s
.Lx612_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n192_statement_end_α:   mov              r11, 193
                        mov              r10, 51
                        add              rsp, 16;                             jmp   n193_statement_begin_α
#=======================================================================================================================
# s1  <stmt 52, line 109: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n193_statement_begin_α: mov              r11, 194
                        mov              r10, 52;                             jmp   n194_statement_end_α
n193_statement_begin_β: mov              r11, 194;                            jmp   n195_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n194_statement_end_α:   mov              r11, 195
                        mov              r10, 52;                             jmp   n195_statement_begin_α
#=======================================================================================================================
#         <stmt 53, line 111: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n195_statement_begin_α: mov              r11, 196
                        mov              r10, 53;                             jmp   n196_call_α
n195_statement_begin_β: mov              r11, 196;                            jmp   n198_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n196_call_α:            sub              rsp, 16
                        mov              r11, 197
                        lea              rcx, [rip + .Lsig622z]
                        lea              rax, [rip + stack_init_α];           jmp   rax
.Lsig622z:              .quad            0
                        .quad            .Lx622_2
                        .quad            .Lx622_2
.Lx622_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx622_29
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
.Lx622_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx622_240
                        add              rsp, 16;                             jmp   n195_statement_begin_β
.Lx622_240:                                                                   jmp   n197_statement_end_α
n196_call_β:            mov              r11, 197;                            jmp   n195_statement_begin_β
.Lx622_0:               .quad            .Lx622_0_s
.Lx622_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n197_statement_end_α:   mov              r11, 198
                        mov              r10, 53
                        add              rsp, 16;                             jmp   n198_statement_begin_α
#=======================================================================================================================
#         <stmt 54, line 112: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n198_statement_begin_α: mov              r11, 199
                        mov              r10, 54;                             jmp   n199_lit_string_α
n198_statement_begin_β: mov              r11, 199;                            jmp   n202_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n199_lit_string_α:      sub              rsp, 16
                        mov              r11, 200
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx627_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n200_call_α
.Lx627_0:               .quad            .Lx627_0_s
.Lx627_0_s:             .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n200_call_α:            sub              rsp, 16
                        mov              r11, 201
                        lea              rcx, [rip + .Lsig629z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig629z:              .quad            1
                        .quad            .Lx629_2
                        .quad            .Lx629_2
                        .quad            16
.Lx629_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx629_29
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
.Lx629_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx629_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n198_statement_begin_β
.Lx629_240:                                                                   jmp   n201_statement_end_α
n200_call_β:            mov              r11, 201;                            jmp   n198_statement_begin_β
.Lx629_0:               .quad            .Lx629_0_s
.Lx629_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n201_statement_end_α:   mov              r11, 202
                        mov              r10, 54
                        add              rsp, 32;                             jmp   n202_statement_begin_α
#=======================================================================================================================
#         <stmt 55, line 113: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n202_statement_begin_α: mov              r11, 203
                        mov              r10, 55;                             jmp   n203_call_α
n202_statement_begin_β: mov              r11, 203;                            jmp   n206_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n203_call_α:            sub              rsp, 16
                        mov              r11, 204
                        lea              rcx, [rip + .Lsig635z]
                        lea              rax, [rip + stack_peek_α];           jmp   rax
.Lsig635z:              .quad            0
                        .quad            .Lx635_2
                        .quad            .Lx635_2
.Lx635_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx635_29
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
.Lx635_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx635_240
                        add              rsp, 16;                             jmp   n202_statement_begin_β
.Lx635_240:                                                                   jmp   n204_assign_α
n203_call_β:            mov              r11, 204;                            jmp   n202_statement_begin_β
.Lx635_0:               .quad            .Lx635_0_s
.Lx635_0_s:             .string          "stack_peek"
#-----------------------------------------------------------------------------------------------------------------------
n204_assign_α:          mov              r11, 205
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx636_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n205_statement_end_α
.Lx636_0:               .quad            .Lx636_0_s
.Lx636_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n205_statement_end_α:   mov              r11, 206
                        mov              r10, 55
                        add              rsp, 16;                             jmp   n206_statement_begin_α
#=======================================================================================================================
#         <stmt 56, line 114: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n206_statement_begin_α: mov              r11, 207
                        mov              r10, 56;                             jmp   n207_call_α
n206_statement_begin_β: mov              r11, 207;                            jmp   n210_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n207_call_α:            sub              rsp, 16
                        mov              r11, 208
                        lea              rcx, [rip + .Lsig642z]
                        lea              rax, [rip + stack_depth_α];          jmp   rax
.Lsig642z:              .quad            0
                        .quad            .Lx642_2
                        .quad            .Lx642_2
.Lx642_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx642_29
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
.Lx642_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx642_240
                        add              rsp, 16;                             jmp   n206_statement_begin_β
.Lx642_240:                                                                   jmp   n208_assign_α
n207_call_β:            mov              r11, 208;                            jmp   n206_statement_begin_β
.Lx642_0:               .quad            .Lx642_0_s
.Lx642_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n208_assign_α:          mov              r11, 209
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx643_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n209_statement_end_α
.Lx643_0:               .quad            .Lx643_0_s
.Lx643_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n209_statement_end_α:   mov              r11, 210
                        mov              r10, 56
                        add              rsp, 16;                             jmp   n210_statement_begin_α
#=======================================================================================================================
#         <stmt 57, line 115: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n210_statement_begin_α: mov              r11, 211
                        mov              r10, 57;                             jmp   n211_call_α
n210_statement_begin_β: mov              r11, 211;                            jmp   n214_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n211_call_α:            sub              rsp, 16
                        mov              r11, 212
                        lea              rcx, [rip + .Lsig649z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig649z:              .quad            0
                        .quad            .Lx649_2
                        .quad            .Lx649_2
.Lx649_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx649_29
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
.Lx649_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx649_240
                        add              rsp, 16;                             jmp   n210_statement_begin_β
.Lx649_240:                                                                   jmp   n212_assign_α
n211_call_β:            mov              r11, 212;                            jmp   n210_statement_begin_β
.Lx649_0:               .quad            .Lx649_0_s
.Lx649_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n212_assign_α:          mov              r11, 213
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx650_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n213_statement_end_α
.Lx650_0:               .quad            .Lx650_0_s
.Lx650_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n213_statement_end_α:   mov              r11, 214
                        mov              r10, 57;                             jmp   n214_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n214_statement_begin_α: mov              r11, 215
                        mov              r10, 0;                              jmp   n215_statement_end_α
n214_statement_begin_β: mov              r11, 215
                        add              rsp, 16;                             jmp   n216_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n215_statement_end_α:   mov              r11, 216
                        mov              r10, 58
                        add              rsp, 16;                             jmp   n216_statement_begin_α
#=======================================================================================================================
#         <stmt 59, line 118: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n216_statement_begin_α: mov              r11, 217
                        mov              r10, 59;                             jmp   n217_call_α
n216_statement_begin_β: mov              r11, 217;                            jmp   n219_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n217_call_α:            sub              rsp, 16
                        mov              r11, 218
                        lea              rcx, [rip + .Lsig660z]
                        lea              rax, [rip + stack_init_α];           jmp   rax
.Lsig660z:              .quad            0
                        .quad            .Lx660_2
                        .quad            .Lx660_2
.Lx660_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx660_29
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
.Lx660_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx660_240
                        add              rsp, 16;                             jmp   n216_statement_begin_β
.Lx660_240:                                                                   jmp   n218_statement_end_α
n217_call_β:            mov              r11, 218;                            jmp   n216_statement_begin_β
.Lx660_0:               .quad            .Lx660_0_s
.Lx660_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n218_statement_end_α:   mov              r11, 219
                        mov              r10, 59
                        add              rsp, 16;                             jmp   n219_statement_begin_α
#=======================================================================================================================
#         <stmt 60, line 119: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n219_statement_begin_α: mov              r11, 220
                        mov              r10, 60;                             jmp   n220_lit_integer_α
n219_statement_begin_β: mov              r11, 220;                            jmp   n223_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n220_lit_integer_α:     sub              rsp, 16
                        mov              r11, 221
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx665_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n221_call_α
.Lx665_0:               .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n221_call_α:            sub              rsp, 16
                        mov              r11, 222
                        lea              rcx, [rip + .Lsig667z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig667z:              .quad            1
                        .quad            .Lx667_2
                        .quad            .Lx667_2
                        .quad            16
.Lx667_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx667_29
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
.Lx667_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx667_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n219_statement_begin_β
.Lx667_240:                                                                   jmp   n222_statement_end_α
n221_call_β:            mov              r11, 222;                            jmp   n219_statement_begin_β
.Lx667_0:               .quad            .Lx667_0_s
.Lx667_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n222_statement_end_α:   mov              r11, 223
                        mov              r10, 60
                        add              rsp, 32;                             jmp   n223_statement_begin_α
#=======================================================================================================================
#         <stmt 61, line 120: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n223_statement_begin_α: mov              r11, 224
                        mov              r10, 61;                             jmp   n224_lit_integer_α
n223_statement_begin_β: mov              r11, 224;                            jmp   n227_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n224_lit_integer_α:     sub              rsp, 16
                        mov              r11, 225
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx672_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n225_call_α
.Lx672_0:               .quad            99
#-----------------------------------------------------------------------------------------------------------------------
n225_call_α:            sub              rsp, 16
                        mov              r11, 226
                        lea              rcx, [rip + .Lsig674z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig674z:              .quad            1
                        .quad            .Lx674_2
                        .quad            .Lx674_2
                        .quad            16
.Lx674_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx674_29
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
.Lx674_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx674_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n223_statement_begin_β
.Lx674_240:                                                                   jmp   n226_statement_end_α
n225_call_β:            mov              r11, 226;                            jmp   n223_statement_begin_β
.Lx674_0:               .quad            .Lx674_0_s
.Lx674_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n226_statement_end_α:   mov              r11, 227
                        mov              r10, 61
                        add              rsp, 32;                             jmp   n227_statement_begin_α
#=======================================================================================================================
#         <stmt 62, line 121: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n227_statement_begin_α: mov              r11, 228
                        mov              r10, 62;                             jmp   n228_lit_string_α
n227_statement_begin_β: mov              r11, 228;                            jmp   n231_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n228_lit_string_α:      sub              rsp, 16
                        mov              r11, 229
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx679_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n229_call_α
.Lx679_0:               .quad            .Lx679_0_s
.Lx679_0_s:             .string          "myvar"
#-----------------------------------------------------------------------------------------------------------------------
n229_call_α:            sub              rsp, 16
                        mov              r11, 230
                        lea              rcx, [rip + .Lsig681z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig681z:              .quad            1
                        .quad            .Lx681_2
                        .quad            .Lx681_2
                        .quad            16
.Lx681_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx681_29
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
.Lx681_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx681_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n227_statement_begin_β
.Lx681_240:                                                                   jmp   n230_statement_end_α
n229_call_β:            mov              r11, 230;                            jmp   n227_statement_begin_β
.Lx681_0:               .quad            .Lx681_0_s
.Lx681_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n230_statement_end_α:   mov              r11, 231
                        mov              r10, 62
                        add              rsp, 32;                             jmp   n231_statement_begin_α
#=======================================================================================================================
#         <stmt 63, line 122: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n231_statement_begin_α: mov              r11, 232
                        mov              r10, 63;                             jmp   n232_var_α
n231_statement_begin_β: mov              r11, 232;                            jmp   n235_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n232_var_α:             sub              rsp, 16
                        mov              r11, 233
                        mov              rax, qword ptr [r9 + 176]            # myvar
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n233_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n233_assign_α:          mov              r11, 234
                        mov              rsi, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx687_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n234_statement_end_α
.Lx687_0:               .quad            .Lx687_0_s
.Lx687_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n234_statement_end_α:   mov              r11, 235
                        mov              r10, 63;                             jmp   n235_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n235_statement_begin_α: mov              r11, 236
                        mov              r10, 0;                              jmp   n236_statement_end_α
n235_statement_begin_β: mov              r11, 236
                        add              rsp, 16;                             jmp   n237_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n236_statement_end_α:   mov              r11, 237
                        mov              r10, 64
                        add              rsp, 16;                             jmp   n237_statement_begin_α
#=======================================================================================================================
#         <stmt 65, line 125: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n237_statement_begin_α: mov              r11, 238
                        mov              r10, 65;                             jmp   n238_call_α
n237_statement_begin_β: mov              r11, 238;                            jmp   n240_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n238_call_α:            sub              rsp, 16
                        mov              r11, 239
                        lea              rcx, [rip + .Lsig697z]
                        lea              rax, [rip + stack_init_α];           jmp   rax
.Lsig697z:              .quad            0
                        .quad            .Lx697_2
                        .quad            .Lx697_2
.Lx697_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx697_29
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
.Lx697_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx697_240
                        add              rsp, 16;                             jmp   n237_statement_begin_β
.Lx697_240:                                                                   jmp   n239_statement_end_α
n238_call_β:            mov              r11, 239;                            jmp   n237_statement_begin_β
.Lx697_0:               .quad            .Lx697_0_s
.Lx697_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n239_statement_end_α:   mov              r11, 240
                        mov              r10, 65
                        add              rsp, 16;                             jmp   n240_statement_begin_α
#=======================================================================================================================
#         <stmt 66, line 126: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n240_statement_begin_α: mov              r11, 241
                        mov              r10, 66;                             jmp   n241_lit_string_α
n240_statement_begin_β: mov              r11, 241;                            jmp   n244_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n241_lit_string_α:      sub              rsp, 16
                        mov              r11, 242
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx702_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n242_assign_α
.Lx702_0:               .quad            .Lx702_0_s
.Lx702_0_s:             .string          "hello world"
#-----------------------------------------------------------------------------------------------------------------------
n242_assign_α:          mov              r11, 243
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 192], rax            # subject
                        mov              qword ptr [r9 + 200], rdx;           jmp   n243_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n243_statement_end_α:   mov              r11, 244
                        mov              r10, 66
                        add              rsp, 16;                             jmp   n244_statement_begin_α
#=======================================================================================================================
#         <stmt 67, line 127: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n244_statement_begin_α: mov              r11, 245
                        mov              r10, 67;                             jmp   n245_var_α
n244_statement_begin_β: mov              r11, 245;                            jmp   n276_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n245_var_α:             sub              rsp, 16
                        mov              r11, 246
                        mov              rax, qword ptr [r9 + 192]            # subject
                        mov              rdx, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n246_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n246_match_begin_α:     mov              r11, 247
                        mov              rdi, qword ptr [rsp + 0]             # var
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
.Lx710_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx710_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n247_match_assign_save_α
n246_match_begin_β:     mov              r11, 247
.Lx710_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx710_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx710_1
                                                                              jmp   .Lx710_0
.Lx710_1:
n246_match_begin_af:    mov              r11, 247
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
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
                        add              rsp, 16;                             jmp   n276_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n247_match_assign_save_α:
                        sub              rsp, 16
                        mov              r11, 248
                        mov              dword ptr [rsp + 0], r14d;           jmp   n248_match_break_α
n247_match_assign_save_β:
                        mov              r11, 248
                        add              rsp, 16;                             jmp   n246_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n248_match_break_α:     sub              rsp, 16
                        mov              r11, 249
                        movsxd           rcx, r14d
.Lx714_0:               cmp              ecx, r15d;                           jl    .Lx714_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n246_match_begin_β
.Lx714_240:             movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32;                             je    .Lx714_1
                        add              ecx, 1;                              jmp   .Lx714_0
.Lx714_1:               mov              dword ptr [rsp + 0], r14d
                        mov              r14d, ecx;                           jmp   n249_match_assign_cond_α
n248_match_break_β:     mov              r11, 249
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n246_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n249_match_assign_cond_α:
                        mov              r11, 250
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n250_match_lit_α
n249_match_assign_cond_β:
                        mov              r11, 250
                        sub              r12, 24;                             jmp   n248_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n250_match_lit_α:       mov              r11, 251
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n249_match_assign_cond_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 32;                             jne   n249_match_assign_cond_β
                        add              r14d, 1;                             jmp   n251_match_assign_save_α
n250_match_lit_β:       mov              r11, 251
                        sub              r14d, 1;                             jmp   n249_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n251_match_assign_save_α:
                        sub              rsp, 16
                        mov              r11, 252
                        mov              dword ptr [rsp + 0], r14d;           jmp   n252_match_rem_α
n251_match_assign_save_β:
                        mov              r11, 252
                        add              rsp, 16;                             jmp   n250_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n252_match_rem_α:       sub              rsp, 16
                        mov              r11, 253
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d;                          jmp   n253_match_assign_cond_α
n252_match_rem_β:       mov              r11, 253
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n250_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n253_match_assign_cond_α:
                        mov              r11, 254
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n254_match_end_α
n253_match_assign_cond_β:
                        mov              r11, 254
                        sub              r12, 24;                             jmp   n252_match_rem_β
#-----------------------------------------------------------------------------------------------------------------------
n254_match_end_α:       mov              r11, 255
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              eax, dword ptr [rbp + -40]           # repl_start
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
                        test             rax, rax;                            je    .Lx725_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n246_match_begin_af
.Lx725_13:              add              rsp, 16
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
                        pop              rbp;                                 jmp   n255_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n255_lit_string_α:      sub              rsp, 16
                        mov              r11, 256
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx726_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n256_match_replace_α
.Lx726_0:               .quad            .Lx726_0_s
.Lx726_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n256_match_replace_α:   mov              r11, 257
                        mov              rdi, qword ptr [rip + .Lx728_0]
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
                        add              rsp, 16;                             jmp   .Lx728_1
.Lx728_0:               .quad            .Lx728_0_s
.Lx728_0_s:             .string          "subject"
.Lx728_1:                                                                     jmp   n257_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n257_statement_end_α:   mov              r11, 258
                        mov              r10, 67
                        add              rsp, 16;                             jmp   n258_statement_begin_α
#=======================================================================================================================
#         <stmt 68, line 128: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n258_statement_begin_α: mov              r11, 259
                        mov              r10, 68;                             jmp   n259_var_α
n258_statement_begin_β: mov              r11, 259;                            jmp   n262_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n259_var_α:             sub              rsp, 16
                        mov              r11, 260
                        mov              rax, qword ptr [r9 + 208]            # w1
                        mov              rdx, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n260_call_α
#-----------------------------------------------------------------------------------------------------------------------
n260_call_α:            sub              rsp, 16
                        mov              r11, 261
                        lea              rcx, [rip + .Lsig735z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig735z:              .quad            1
                        .quad            .Lx735_2
                        .quad            .Lx735_2
                        .quad            16
.Lx735_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx735_29
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
.Lx735_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx735_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n258_statement_begin_β
.Lx735_240:                                                                   jmp   n261_statement_end_α
n260_call_β:            mov              r11, 261;                            jmp   n258_statement_begin_β
.Lx735_0:               .quad            .Lx735_0_s
.Lx735_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n261_statement_end_α:   mov              r11, 262
                        mov              r10, 68
                        add              rsp, 32;                             jmp   n262_statement_begin_α
#=======================================================================================================================
#         <stmt 69, line 129: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n262_statement_begin_α: mov              r11, 263
                        mov              r10, 69;                             jmp   n263_var_α
n262_statement_begin_β: mov              r11, 263;                            jmp   n266_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n263_var_α:             sub              rsp, 16
                        mov              r11, 264
                        mov              rax, qword ptr [r9 + 224]            # w2
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n264_call_α
#-----------------------------------------------------------------------------------------------------------------------
n264_call_α:            sub              rsp, 16
                        mov              r11, 265
                        lea              rcx, [rip + .Lsig742z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig742z:              .quad            1
                        .quad            .Lx742_2
                        .quad            .Lx742_2
                        .quad            16
.Lx742_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx742_29
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
.Lx742_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx742_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n262_statement_begin_β
.Lx742_240:                                                                   jmp   n265_statement_end_α
n264_call_β:            mov              r11, 265;                            jmp   n262_statement_begin_β
.Lx742_0:               .quad            .Lx742_0_s
.Lx742_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n265_statement_end_α:   mov              r11, 266
                        mov              r10, 69
                        add              rsp, 32;                             jmp   n266_statement_begin_α
#=======================================================================================================================
#         <stmt 70, line 130: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n266_statement_begin_α: mov              r11, 267
                        mov              r10, 70;                             jmp   n267_call_α
n266_statement_begin_β: mov              r11, 267;                            jmp   n270_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n267_call_α:            sub              rsp, 16
                        mov              r11, 268
                        lea              rcx, [rip + .Lsig748z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig748z:              .quad            0
                        .quad            .Lx748_2
                        .quad            .Lx748_2
.Lx748_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx748_29
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
.Lx748_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx748_240
                        add              rsp, 16;                             jmp   n266_statement_begin_β
.Lx748_240:                                                                   jmp   n268_assign_α
n267_call_β:            mov              r11, 268;                            jmp   n266_statement_begin_β
.Lx748_0:               .quad            .Lx748_0_s
.Lx748_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n268_assign_α:          mov              r11, 269
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx749_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n269_statement_end_α
.Lx749_0:               .quad            .Lx749_0_s
.Lx749_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n269_statement_end_α:   mov              r11, 270
                        mov              r10, 70
                        add              rsp, 16;                             jmp   n270_statement_begin_α
#=======================================================================================================================
#         <stmt 71, line 131: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n270_statement_begin_α: mov              r11, 271
                        mov              r10, 71;                             jmp   n271_call_α
n270_statement_begin_β: mov              r11, 271;                            jmp   n274_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n271_call_α:            sub              rsp, 16
                        mov              r11, 272
                        lea              rcx, [rip + .Lsig755z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig755z:              .quad            0
                        .quad            .Lx755_2
                        .quad            .Lx755_2
.Lx755_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx755_29
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
.Lx755_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx755_240
                        add              rsp, 16;                             jmp   n270_statement_begin_β
.Lx755_240:                                                                   jmp   n272_assign_α
n271_call_β:            mov              r11, 272;                            jmp   n270_statement_begin_β
.Lx755_0:               .quad            .Lx755_0_s
.Lx755_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n272_assign_α:          mov              r11, 273
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx756_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n273_statement_end_α
.Lx756_0:               .quad            .Lx756_0_s
.Lx756_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n273_statement_end_α:   mov              r11, 274
                        mov              r10, 71
                        add              rsp, 16;                             jmp   n274_statement_begin_α
#=======================================================================================================================
#         <stmt 72, line 132: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n274_statement_begin_α: mov              r11, 275
                        mov              r10, 72;                             jmp   n275_statement_end_α
n274_statement_begin_β: mov              r11, 275;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n275_statement_end_α:   mov              r11, 276
                        mov              r10, 72;                             jmp   main_γ
#=======================================================================================================================
# bad_s2  <stmt 73, line 133: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n276_statement_begin_α: mov              r11, 277
                        mov              r10, 73;                             jmp   n277_lit_string_α
n276_statement_begin_β: mov              r11, 277;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n277_lit_string_α:      sub              rsp, 16
                        mov              r11, 278
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 26
                        mov              rax, qword ptr [rip + .Lx765_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n278_assign_α
.Lx765_0:               .quad            .Lx765_0_s
.Lx765_0_s:             .string          "FAIL: pattern match failed"
#-----------------------------------------------------------------------------------------------------------------------
n278_assign_α:          mov              r11, 279
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx766_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n279_statement_end_α
.Lx766_0:               .quad            .Lx766_0_s
.Lx766_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n279_statement_end_α:   mov              r11, 280
                        mov              r10, 73
                        add              rsp, 16;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 mov              r11, 281
                        pop              rcx
                        add              rsp, 8;                              jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                mov              r11, 282
                        add              rsp, 8
                        pop              rcx;                                 jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
NRETURN:                mov              r11, 283
                        mov              rax, qword ptr [rip + rt_g_ret_by_name@GOTPCREL]
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
module_init:
                        sub              rsp, 8
                        .section         .rodata
.Lclassspec0:           .string          "slink(snext,sval)"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lclassspec0]
                        call             record_register@PLT
                        .section         .rodata
.Lseala6:               .string          "stack_init"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            stack_init_α
                        lea              rdi, [rip + .Lseala6]
                        mov              rsi, qword ptr [rip + stack_init_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala7:               .string          "stack_push"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            stack_push_α
                        lea              rdi, [rip + .Lseala7]
                        mov              rsi, qword ptr [rip + stack_push_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala8:               .string          "stack_pop"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            stack_pop_α
                        lea              rdi, [rip + .Lseala8]
                        mov              rsi, qword ptr [rip + stack_pop_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala9:               .string          "stack_peek"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            stack_peek_α
                        lea              rdi, [rip + .Lseala9]
                        mov              rsi, qword ptr [rip + stack_peek_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala10:              .string          "stack_top"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            stack_top_α
                        lea              rdi, [rip + .Lseala10]
                        mov              rsi, qword ptr [rip + stack_top_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala11:              .string          "stack_depth"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            stack_depth_α
                        lea              rdi, [rip + .Lseala11]
                        mov              rsi, qword ptr [rip + stack_depth_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.S0:                    .string          "w1"
.S1:                    .string          "w2"
                        .text
                        .section         .note.GNU-stack,"",@progbits
