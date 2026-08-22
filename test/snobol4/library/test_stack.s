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
                        mov              rax, qword ptr [rip + .Lx290_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_call_α
.Lx290_0:               .quad            .Lx290_0_s
.Lx290_0_s:             .string          "slink(snext,sval)"
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:              sub              rsp, 16
                        mov              r11, 5
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd292:            .string          "DATA"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd292]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx291_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n2_statement_begin_β
.Lx291_240:             mov              qword ptr [rsp + 0], rax             # result
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
                        mov              rdi, qword ptr [rip + .Lx302_0]
                        mov              rsi, qword ptr [rip + .Lx302_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n11_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx302_0]
                        lea              rsi, [rip + stack_init_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n10_statement_end_α
n9_define_β:            mov              r11, 10;                             jmp   n8_statement_begin_β
.Lx302_0:               .quad            .Lx302_0_s
.Lx302_0_s:             .string          "stack_init"
.Lx302_1:               .quad            .Lx302_1_s
.Lx302_1_s:             .string          ""
                                                                              jmp   .Lx303_245
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
.Lx303_245:
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
                        mov              rax, qword ptr [rip + .Lx308_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n13_assign_α
.Lx308_0:               .quad            .Lx308_0_s
.Lx308_0_s:             .string          ""
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
                        mov              rdi, qword ptr [rip + .Lx319_0]
                        mov              rsi, qword ptr [rip + .Lx319_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n20_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx319_0]
                        lea              rsi, [rip + stack_push_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n19_statement_end_α
n18_define_β:           mov              r11, 19;                             jmp   n17_statement_begin_β
.Lx319_0:               .quad            .Lx319_0_s
.Lx319_0_s:             .string          "stack_push"
.Lx319_1:               .quad            .Lx319_1_s
.Lx319_1_s:             .string          "x"
                                                                              jmp   .Lx320_245
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
                        cmp              rdx, 0;                              jbe   .Lx320_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 32]             # x
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx320_41
.Lx320_10:              mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx320_41:              lea              rcx, [rip + stack_push_γ]
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
                        cmp              rdx, 0;                              jbe   .Lx320_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 32], rax             # x
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx320_110
.Lx320_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 40], rax
.Lx320_110:             mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx320_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 32], rax             # x
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 40], rax;            jmp   .Lx320_180
.Lx320_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 40], rax
.Lx320_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx320_245:
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
.Lrkfnzd328:            .string          "slink"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd328]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx327_240
                        add              rsp, 16;                             jmp   n22_var_β
.Lx327_240:             mov              qword ptr [rsp + 0], rax             # result
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
                        mov              rdi, qword ptr [rip + .Lx335_0]
                        mov              rsi, qword ptr [rsp + 16]            # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_field_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx335_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n26_statement_begin_β
.Lx335_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n29_assign_α
.Lx335_0:               .quad            .Lx335_0_s
.Lx335_0_s:             .string          "sval"
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
n33_statement_begin_β:  mov              r11, 34;                             jmp   n73_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n34_define_α:           mov              r11, 35
                        mov              rdi, qword ptr [rip + .Lx346_0]
                        mov              rsi, qword ptr [rip + .Lx346_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n36_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx346_0]
                        lea              rsi, [rip + stack_pop_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n35_statement_end_α
n34_define_β:           mov              r11, 35;                             jmp   n33_statement_begin_β
.Lx346_0:               .quad            .Lx346_0_s
.Lx346_0_s:             .string          "stack_pop"
.Lx346_1:               .quad            .Lx346_1_s
.Lx346_1_s:             .string          "var"
                                                                              jmp   .Lx347_245
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
                        cmp              rdx, 0;                              jbe   .Lx347_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 64]             # var
                        mov              qword ptr [r9 + 64], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 72]
                        mov              qword ptr [r9 + 72], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx347_41
.Lx347_10:              mov              rax, qword ptr [r9 + 64]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 72]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 64], 0
                        mov              qword ptr [r9 + 72], 0
.Lx347_41:              lea              rcx, [rip + stack_pop_γ]
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
                        cmp              rdx, 0;                              jbe   .Lx347_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 64], rax             # var
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx347_110
.Lx347_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 72], rax
.Lx347_110:             mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx347_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 64], rax             # var
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 72], rax;            jmp   .Lx347_180
.Lx347_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 64], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 72], rax
.Lx347_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx347_245:
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_end_α:    mov              r11, 36
                        mov              r10, 11;                             jmp   n73_statement_begin_α
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
                        mov              rax, qword ptr [rip + .Lx353_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n39_differ_α
n38_lit_string_β:       mov              r11, 39
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n36_statement_begin_β
.Lx353_0:               .quad            .Lx353_0_s
.Lx353_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n39_differ_α:           sub              rsp, 16
                        mov              r11, 40
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            je    .Lx355_240
                        add              rsp, 16;                             jmp   n38_lit_string_β
.Lx355_240:                                                                   jmp   n40_statement_end_α
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
                        mov              rax, qword ptr [rip + .Lx361_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n44_ident_α
n43_lit_string_β:       mov              r11, 44
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n41_statement_begin_β
.Lx361_0:               .quad            .Lx361_0_s
.Lx361_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n44_ident_α:            sub              rsp, 16
                        mov              r11, 45
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            jne   .Lx363_240
                        add              rsp, 16;                             jmp   n43_lit_string_β
.Lx363_240:                                                                   jmp   n45_statement_end_α
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
.Lrkfnzd370:            .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd370]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx369_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n46_statement_begin_β
.Lx369_240:             mov              qword ptr [rsp + 0], rax             # result
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
.Lrkfnzd378:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd378]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx377_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n51_statement_begin_β
.Lx377_240:             mov              qword ptr [rsp + 0], rax             # result
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
.Lrkfnzd386:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd386]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx385_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n56_statement_begin_β
.Lx385_240:             mov              qword ptr [rsp + 0], rax             # result
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
.Lrkfnzd389:            .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd389]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx388_240
                        add              rsp, 16;                             jmp   n59_var_β
.Lx388_240:             mov              qword ptr [rsp + 0], rax             # result
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
                        call             rt_assign_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx390_240
                        add              rsp, 32;                             jmp   n59_var_β
.Lx390_240:             mov              qword ptr [rsp + 0], rax             # result
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
.Lrkfnzd397:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd397]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx396_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n63_statement_begin_β
.Lx396_240:             mov              qword ptr [rsp + 0], rax             # result
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
                        mov              r10, 18;                             jmp   n69_lit_string_α
n68_statement_begin_β:  mov              r11, 69;                             jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_string_α:       sub              rsp, 16
                        mov              r11, 70
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx403_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n70_call_α
.Lx403_0:               .quad            .Lx403_0_s
.Lx403_0_s:             .string          "dummy"
#-----------------------------------------------------------------------------------------------------------------------
n70_call_α:             sub              rsp, 16
                        mov              r11, 71
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd405:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd405]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx404_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n68_statement_begin_β
.Lx404_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n71_assign_α
n70_call_β:             mov              r11, 71
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n68_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n71_assign_α:           mov              r11, 72
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax             # stack_pop
                        mov              qword ptr [r9 + 56], rdx;            jmp   n72_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_end_α:    mov              r11, 73
                        mov              r10, 18
                        add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# stack_pop_end  <stmt 19, line 71: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_begin_α:  mov              r11, 74
                        mov              r10, 19;                             jmp   n74_statement_end_α
n73_statement_begin_β:  mov              r11, 74;                             jmp   n75_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_end_α:    mov              r11, 75
                        mov              r10, 19;                             jmp   n75_statement_begin_α
#=======================================================================================================================
#         <stmt 20, line 72: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n75_statement_begin_α:  mov              r11, 76
                        mov              r10, 20;                             jmp   n76_define_α
n75_statement_begin_β:  mov              r11, 76;                             jmp   n88_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n76_define_α:           mov              r11, 77
                        mov              rdi, qword ptr [rip + .Lx416_0]
                        mov              rsi, qword ptr [rip + .Lx416_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n78_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx416_0]
                        lea              rsi, [rip + stack_peek_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n77_statement_end_α
n76_define_β:           mov              r11, 77;                             jmp   n75_statement_begin_β
.Lx416_0:               .quad            .Lx416_0_s
.Lx416_0_s:             .string          "stack_peek"
.Lx416_1:               .quad            .Lx416_1_s
.Lx416_1_s:             .string          ""
                                                                              jmp   .Lx417_245
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
                        lea              rax, [rip + n78_statement_begin_α];  jmp   rax
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
.Lx417_245:
#-----------------------------------------------------------------------------------------------------------------------
n77_statement_end_α:    mov              r11, 78
                        mov              r10, 20;                             jmp   n88_statement_begin_α
#=======================================================================================================================
# stack_peek  <stmt 21, line 73: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n78_statement_begin_α:  mov              r11, 79
                        mov              r10, 21;                             jmp   n79_var_α
n78_statement_begin_β:  mov              r11, 79;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n79_var_α:              sub              rsp, 16
                        mov              r11, 80
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n80_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n80_lit_string_α:       sub              rsp, 16
                        mov              r11, 81
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx423_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n81_differ_α
n80_lit_string_β:       mov              r11, 81
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n78_statement_begin_β
.Lx423_0:               .quad            .Lx423_0_s
.Lx423_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n81_differ_α:           sub              rsp, 16
                        mov              r11, 82
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            je    .Lx425_240
                        add              rsp, 16;                             jmp   n80_lit_string_β
.Lx425_240:                                                                   jmp   n82_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n82_statement_end_α:    mov              r11, 83
                        mov              r10, 21
                        add              rsp, 48;                             jmp   n83_statement_begin_α
#=======================================================================================================================
#         <stmt 22, line 74: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n83_statement_begin_α:  mov              r11, 84
                        mov              r10, 22;                             jmp   n84_var_α
n83_statement_begin_β:  mov              r11, 84;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n84_var_α:              sub              rsp, 16
                        mov              r11, 85
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n85_call_α
#-----------------------------------------------------------------------------------------------------------------------
n85_call_α:             sub              rsp, 16
                        mov              r11, 86
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd432:            .string          "sval"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd432]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx431_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n83_statement_begin_β
.Lx431_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n86_assign_α
n85_call_β:             mov              r11, 86
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n83_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n86_assign_α:           mov              r11, 87
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 80], rax             # stack_peek
                        mov              qword ptr [r9 + 88], rdx;            jmp   n87_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n87_statement_end_α:    mov              r11, 88
                        mov              r10, 22
                        add              rsp, 32;                             jmp   RETURN
#=======================================================================================================================
# stack_peek_end  <stmt 23, line 76: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_begin_α:  mov              r11, 89
                        mov              r10, 23;                             jmp   n89_statement_end_α
n88_statement_begin_β:  mov              r11, 89;                             jmp   n90_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_end_α:    mov              r11, 90
                        mov              r10, 23;                             jmp   n90_statement_begin_α
#=======================================================================================================================
#         <stmt 24, line 77: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_begin_α:  mov              r11, 91
                        mov              r10, 24;                             jmp   n91_define_α
n90_statement_begin_β:  mov              r11, 91;                             jmp   n103_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n91_define_α:           mov              r11, 92
                        mov              rdi, qword ptr [rip + .Lx443_0]
                        mov              rsi, qword ptr [rip + .Lx443_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n93_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx443_0]
                        lea              rsi, [rip + stack_top_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n92_statement_end_α
n91_define_β:           mov              r11, 92;                             jmp   n90_statement_begin_β
.Lx443_0:               .quad            .Lx443_0_s
.Lx443_0_s:             .string          "stack_top"
.Lx443_1:               .quad            .Lx443_1_s
.Lx443_1_s:             .string          ""
                                                                              jmp   .Lx444_245
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
                        lea              rax, [rip + n93_statement_begin_α];  jmp   rax
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
.Lx444_245:
#-----------------------------------------------------------------------------------------------------------------------
n92_statement_end_α:    mov              r11, 93
                        mov              r10, 24;                             jmp   n103_statement_begin_α
#=======================================================================================================================
# stack_top  <stmt 25, line 78: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n93_statement_begin_α:  mov              r11, 94
                        mov              r10, 25;                             jmp   n94_var_α
n93_statement_begin_β:  mov              r11, 94;                             jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n94_var_α:              sub              rsp, 16
                        mov              r11, 95
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n95_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n95_lit_string_α:       sub              rsp, 16
                        mov              r11, 96
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx450_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n96_differ_α
n95_lit_string_β:       mov              r11, 96
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n93_statement_begin_β
.Lx450_0:               .quad            .Lx450_0_s
.Lx450_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n96_differ_α:           sub              rsp, 16
                        mov              r11, 97
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            je    .Lx452_240
                        add              rsp, 16;                             jmp   n95_lit_string_β
.Lx452_240:                                                                   jmp   n97_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n97_statement_end_α:    mov              r11, 98
                        mov              r10, 25
                        add              rsp, 48;                             jmp   n98_statement_begin_α
#=======================================================================================================================
#         <stmt 26, line 79: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n98_statement_begin_α:  mov              r11, 99
                        mov              r10, 26;                             jmp   n99_var_α
n98_statement_begin_β:  mov              r11, 99;                             jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n99_var_α:              sub              rsp, 16
                        mov              r11, 100
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n100_field_var_α
#-----------------------------------------------------------------------------------------------------------------------
n100_field_var_α:       sub              rsp, 16
                        mov              r11, 101
                        mov              rdi, qword ptr [rip + .Lx458_0]
                        mov              rsi, qword ptr [rsp + 16]            # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_field_var@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx458_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n98_statement_begin_β
.Lx458_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n101_assign_α
.Lx458_0:               .quad            .Lx458_0_s
.Lx458_0_s:             .string          "sval"
#-----------------------------------------------------------------------------------------------------------------------
n101_assign_α:          mov              r11, 102
                        mov              rax, qword ptr [rsp + 0]             # field_var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 96], rax             # stack_top
                        mov              qword ptr [r9 + 104], rdx;           jmp   n102_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:   mov              r11, 103
                        mov              r10, 26
                        add              rsp, 32;                             jmp   NRETURN
#=======================================================================================================================
# stack_top_end  <stmt 27, line 81: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_begin_α: mov              r11, 104
                        mov              r10, 27;                             jmp   n104_statement_end_α
n103_statement_begin_β: mov              r11, 104;                            jmp   n105_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_end_α:   mov              r11, 105
                        mov              r10, 27;                             jmp   n105_statement_begin_α
#=======================================================================================================================
#         <stmt 28, line 82: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n105_statement_begin_α: mov              r11, 106
                        mov              r10, 28;                             jmp   n106_define_α
n105_statement_begin_β: mov              r11, 106;                            jmp   n132_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n106_define_α:          mov              r11, 107
                        mov              rdi, qword ptr [rip + .Lx469_0]
                        mov              rsi, qword ptr [rip + .Lx469_1]
                        mov              edx, 1
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + n108_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx469_0]
                        lea              rsi, [rip + stack_depth_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             bb_ab_seal_alpha@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n107_statement_end_α
n106_define_β:          mov              r11, 107;                            jmp   n105_statement_begin_β
.Lx469_0:               .quad            .Lx469_0_s
.Lx469_0_s:             .string          "stack_depth"
.Lx469_1:               .quad            .Lx469_1_s
.Lx469_1_s:             .string          "sd"
                                                                              jmp   .Lx470_245
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
                        cmp              rdx, 0;                              jbe   .Lx470_10
                        mov              rdi, qword ptr [rcx + 24]
                        add              rdi, r8
                        mov              rax, qword ptr [rdi + 0]
                        mov              rsi, qword ptr [r9 + 128]            # sd
                        mov              qword ptr [r9 + 128], rax
                        mov              qword ptr [rdi + 0], rsi
                        mov              rax, qword ptr [rdi + 8]
                        mov              rsi, qword ptr [r9 + 136]
                        mov              qword ptr [r9 + 136], rax
                        mov              qword ptr [rdi + 8], rsi;            jmp   .Lx470_41
.Lx470_10:              mov              rax, qword ptr [r9 + 128]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [r9 + 128], 0
                        mov              qword ptr [r9 + 136], 0
.Lx470_41:              lea              rcx, [rip + stack_depth_γ]
                        lea              rax, [rip + stack_depth_ω]
                        push             rax
                        push             rcx
                        lea              rax, [rip + n108_statement_begin_α]; jmp   rax
stack_depth_γ:          mov              rdi, qword ptr [r9 + 112]            # stack_depth
                        mov              rsi, qword ptr [r9 + 120]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rdx, qword ptr [rcx + 0]
                        lea              r8, [rsp + 64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 112], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 120], rax
                        cmp              rdx, 0;                              jbe   .Lx470_80
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 128], rax            # sd
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx470_110
.Lx470_80:              mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
.Lx470_110:             mov              rcx, qword ptr [rcx + 8]
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
                        cmp              rdx, 0;                              jbe   .Lx470_150
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 0]
                        mov              qword ptr [r9 + 128], rax            # sd
                        mov              rax, qword ptr [rcx + 24]
                        add              rax, r8
                        mov              rax, qword ptr [rax + 8]
                        mov              qword ptr [r9 + 136], rax;           jmp   .Lx470_180
.Lx470_150:             mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [r9 + 128], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [r9 + 136], rax
.Lx470_180:             mov              rcx, qword ptr [rcx + 16]
                        add              rsp, 64
                        mov              eax, 104
                        xor              edx, edx;                            jmp   rcx
.Lx470_245:
#-----------------------------------------------------------------------------------------------------------------------
n107_statement_end_α:   mov              r11, 108
                        mov              r10, 28;                             jmp   n132_statement_begin_α
#=======================================================================================================================
# stack_depth  <stmt 29, line 83: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n108_statement_begin_α: mov              r11, 109
                        mov              r10, 29;                             jmp   n109_lit_integer_α
n108_statement_begin_β: mov              r11, 109;                            jmp   n112_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n109_lit_integer_α:     sub              rsp, 16
                        mov              r11, 110
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx475_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n110_assign_α
.Lx475_0:               .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n110_assign_α:          mov              r11, 111
                        mov              rax, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # stack_depth
                        mov              qword ptr [r9 + 120], rdx;           jmp   n111_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n111_statement_end_α:   mov              r11, 112
                        mov              r10, 29
                        add              rsp, 16;                             jmp   n112_statement_begin_α
#=======================================================================================================================
#         <stmt 30, line 84: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n112_statement_begin_α: mov              r11, 113
                        mov              r10, 30;                             jmp   n113_var_α
n112_statement_begin_β: mov              r11, 113;                            jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n113_var_α:             sub              rsp, 16
                        mov              r11, 114
                        mov              rax, qword ptr [r9 + 144]            # stk
                        mov              rdx, qword ptr [r9 + 152]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n114_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n114_assign_α:          mov              r11, 115
                        mov              rax, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax            # sd
                        mov              qword ptr [r9 + 136], rdx;           jmp   n115_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n115_statement_end_α:   mov              r11, 116
                        mov              r10, 30
                        add              rsp, 16;                             jmp   n116_statement_begin_α
#=======================================================================================================================
# stk_dep0  <stmt 31, line 85: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n116_statement_begin_α: mov              r11, 117
                        mov              r10, 31;                             jmp   n117_var_α
n116_statement_begin_β: mov              r11, 117;                            jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n117_var_α:             sub              rsp, 16
                        mov              r11, 118
                        mov              rax, qword ptr [r9 + 128]            # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n118_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n118_lit_string_α:      sub              rsp, 16
                        mov              r11, 119
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx488_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n119_differ_α
n118_lit_string_β:      mov              r11, 119
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n116_statement_begin_β
.Lx488_0:               .quad            .Lx488_0_s
.Lx488_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n119_differ_α:          sub              rsp, 16
                        mov              r11, 120
                        mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            je    .Lx490_240
                        add              rsp, 16;                             jmp   n118_lit_string_β
.Lx490_240:                                                                   jmp   n120_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n120_statement_end_α:   mov              r11, 121
                        mov              r10, 31
                        add              rsp, 48;                             jmp   n121_statement_begin_α
#=======================================================================================================================
#         <stmt 32, line 86: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n121_statement_begin_α: mov              r11, 122
                        mov              r10, 32;                             jmp   n122_var_α
n121_statement_begin_β: mov              r11, 122;                            jmp   n127_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n122_var_α:             sub              rsp, 16
                        mov              r11, 123
                        mov              rax, qword ptr [r9 + 112]            # stack_depth
                        mov              rdx, qword ptr [r9 + 120]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n123_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n123_lit_integer_α:     sub              rsp, 16
                        mov              r11, 124
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx496_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n124_binop_α
n123_lit_integer_β:     mov              r11, 124
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n121_statement_begin_β
.Lx496_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n124_binop_α:           sub              rsp, 16
                        mov              r11, 125
                        mov              ecx, dword ptr [rsp + 32]            # var
                        mov              rax, qword ptr [rsp + 40]
                        cmp              cl, 3;                               jne   .Lx497_2
                        add              rax, 1
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx497_7
.Lx497_2:               mov              eax, ecx
                        mov              edx, ecx
                        and              edx, 1;                              jz    .Lx497_0
                        mov              rsi, qword ptr [rsp + 40]            # var
                        mov              rdi, 1
                        cmp              al, 5;                               je    .Lx497_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx497_4
.Lx497_3:               movq             xmm0, rsi
.Lx497_4:               cvtsi2sd         xmm1, rdi
                        addsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx497_7:                                                                     jmp   n125_assign_α
.Lx497_0:               mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_add@PLT
                        cmp              al, 104;                             jne   .Lx497_240
                        add              rsp, 16;                             jmp   n123_lit_integer_β
.Lx497_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n125_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n125_assign_α:          mov              r11, 126
                        mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 112], rax            # stack_depth
                        mov              qword ptr [r9 + 120], rdx;           jmp   n126_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n126_statement_end_α:   mov              r11, 127
                        mov              r10, 32
                        add              rsp, 48;                             jmp   n127_statement_begin_α
#=======================================================================================================================
#         <stmt 33, line 87: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n127_statement_begin_α: mov              r11, 128
                        mov              r10, 33;                             jmp   n128_var_α
n127_statement_begin_β: mov              r11, 128;                            jmp   n116_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n128_var_α:             sub              rsp, 16
                        mov              r11, 129
                        mov              rax, qword ptr [r9 + 128]            # sd
                        mov              rdx, qword ptr [r9 + 136]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n129_call_α
#-----------------------------------------------------------------------------------------------------------------------
n129_call_α:            sub              rsp, 16
                        mov              r11, 130
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd505:            .string          "snext"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd505]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx504_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n127_statement_begin_β
.Lx504_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n130_assign_α
n129_call_β:            mov              r11, 130
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n127_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n130_assign_α:          mov              r11, 131
                        mov              rax, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 128], rax            # sd
                        mov              qword ptr [r9 + 136], rdx;           jmp   n131_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n131_statement_end_α:   mov              r11, 132
                        mov              r10, 33
                        add              rsp, 32;                             jmp   n116_statement_begin_α
#=======================================================================================================================
# stack_depth_end  <stmt 34, line 89: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n132_statement_begin_α: mov              r11, 133
                        mov              r10, 34;                             jmp   n133_statement_end_α
n132_statement_begin_β: mov              r11, 133;                            jmp   n134_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n133_statement_end_α:   mov              r11, 134
                        mov              r10, 34;                             jmp   n134_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n134_statement_begin_α: mov              r11, 135
                        mov              r10, 0;                              jmp   n135_statement_end_α
n134_statement_begin_β: mov              r11, 135;                            jmp   n136_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n135_statement_end_α:   mov              r11, 136
                        mov              r10, 35;                             jmp   n136_statement_begin_α
#=======================================================================================================================
#         <stmt 36, line 90: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n136_statement_begin_α: mov              r11, 137
                        mov              r10, 36;                             jmp   n137_lit_integer_α
n136_statement_begin_β: mov              r11, 137;                            jmp   n140_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n137_lit_integer_α:     sub              rsp, 16
                        mov              r11, 138
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx519_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n138_keyword_assign_snobol4_α
.Lx519_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n138_keyword_assign_snobol4_α:
                        sub              rsp, 16
                        mov              r11, 139
                        mov              rdi, qword ptr [rip + .Lx520_0]
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_kw_write_idx@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        cmp              al, 104;                             jne   .Lx520_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n136_statement_begin_β
.Lx520_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n139_statement_end_α
.Lx520_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n139_statement_end_α:   mov              r11, 140
                        mov              r10, 36;                             jmp   n140_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n140_statement_begin_α: mov              r11, 141
                        mov              r10, 0;                              jmp   n141_statement_end_α
n140_statement_begin_β: mov              r11, 141
                        add              rsp, 32;                             jmp   n142_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n141_statement_end_α:   mov              r11, 142
                        mov              r10, 37
                        add              rsp, 32;                             jmp   n142_statement_begin_α
#=======================================================================================================================
#         <stmt 38, line 93: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n142_statement_begin_α: mov              r11, 143
                        mov              r10, 38;                             jmp   n143_call_α
n142_statement_begin_β: mov              r11, 143;                            jmp   n145_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n143_call_α:            sub              rsp, 16
                        mov              r11, 144
                        lea              rcx, [rip + .Lsig530z]
                        lea              rax, [rip + stack_init_α];           jmp   rax
.Lsig530z:              .quad            0
                        .quad            .Lx530_2
                        .quad            .Lx530_2
.Lx530_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx530_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx530_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx530_240
                        add              rsp, 16;                             jmp   n142_statement_begin_β
.Lx530_240:                                                                   jmp   n144_statement_end_α
n143_call_β:            mov              r11, 144;                            jmp   n142_statement_begin_β
.Lx530_0:               .quad            .Lx530_0_s
.Lx530_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n144_statement_end_α:   mov              r11, 145
                        mov              r10, 38
                        add              rsp, 16;                             jmp   n145_statement_begin_α
#=======================================================================================================================
#         <stmt 39, line 94: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n145_statement_begin_α: mov              r11, 146
                        mov              r10, 39;                             jmp   n146_lit_string_α
n145_statement_begin_β: mov              r11, 146;                            jmp   n149_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n146_lit_string_α:      sub              rsp, 16
                        mov              r11, 147
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx535_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n147_call_α
.Lx535_0:               .quad            .Lx535_0_s
.Lx535_0_s:             .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
n147_call_α:            sub              rsp, 16
                        mov              r11, 148
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
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx537_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx537_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n145_statement_begin_β
.Lx537_240:                                                                   jmp   n148_statement_end_α
n147_call_β:            mov              r11, 148;                            jmp   n145_statement_begin_β
.Lx537_0:               .quad            .Lx537_0_s
.Lx537_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n148_statement_end_α:   mov              r11, 149
                        mov              r10, 39
                        add              rsp, 32;                             jmp   n149_statement_begin_α
#=======================================================================================================================
#         <stmt 40, line 95: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n149_statement_begin_α: mov              r11, 150
                        mov              r10, 40;                             jmp   n150_lit_string_α
n149_statement_begin_β: mov              r11, 150;                            jmp   n153_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n150_lit_string_α:      sub              rsp, 16
                        mov              r11, 151
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx542_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n151_call_α
.Lx542_0:               .quad            .Lx542_0_s
.Lx542_0_s:             .string          "b"
#-----------------------------------------------------------------------------------------------------------------------
n151_call_α:            sub              rsp, 16
                        mov              r11, 152
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
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx544_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx544_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n149_statement_begin_β
.Lx544_240:                                                                   jmp   n152_statement_end_α
n151_call_β:            mov              r11, 152;                            jmp   n149_statement_begin_β
.Lx544_0:               .quad            .Lx544_0_s
.Lx544_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n152_statement_end_α:   mov              r11, 153
                        mov              r10, 40
                        add              rsp, 32;                             jmp   n153_statement_begin_α
#=======================================================================================================================
#         <stmt 41, line 96: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n153_statement_begin_α: mov              r11, 154
                        mov              r10, 41;                             jmp   n154_lit_string_α
n153_statement_begin_β: mov              r11, 154;                            jmp   n157_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n154_lit_string_α:      sub              rsp, 16
                        mov              r11, 155
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx549_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n155_call_α
.Lx549_0:               .quad            .Lx549_0_s
.Lx549_0_s:             .string          "c"
#-----------------------------------------------------------------------------------------------------------------------
n155_call_α:            sub              rsp, 16
                        mov              r11, 156
                        lea              rcx, [rip + .Lsig551z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig551z:              .quad            1
                        .quad            .Lx551_2
                        .quad            .Lx551_2
                        .quad            16
.Lx551_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx551_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx551_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx551_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n153_statement_begin_β
.Lx551_240:                                                                   jmp   n156_statement_end_α
n155_call_β:            mov              r11, 156;                            jmp   n153_statement_begin_β
.Lx551_0:               .quad            .Lx551_0_s
.Lx551_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n156_statement_end_α:   mov              r11, 157
                        mov              r10, 41
                        add              rsp, 32;                             jmp   n157_statement_begin_α
#=======================================================================================================================
#         <stmt 42, line 97: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n157_statement_begin_α: mov              r11, 158
                        mov              r10, 42;                             jmp   n158_call_α
n157_statement_begin_β: mov              r11, 158;                            jmp   n161_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n158_call_α:            sub              rsp, 16
                        mov              r11, 159
                        lea              rcx, [rip + .Lsig557z]
                        lea              rax, [rip + stack_depth_α];          jmp   rax
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
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx557_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx557_240
                        add              rsp, 16;                             jmp   n157_statement_begin_β
.Lx557_240:                                                                   jmp   n159_assign_α
n158_call_β:            mov              r11, 159;                            jmp   n157_statement_begin_β
.Lx557_0:               .quad            .Lx557_0_s
.Lx557_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n159_assign_α:          mov              r11, 160
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx558_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n160_statement_end_α
.Lx558_0:               .quad            .Lx558_0_s
.Lx558_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n160_statement_end_α:   mov              r11, 161
                        mov              r10, 42
                        add              rsp, 16;                             jmp   n161_statement_begin_α
#=======================================================================================================================
#         <stmt 43, line 98: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n161_statement_begin_α: mov              r11, 162
                        mov              r10, 43;                             jmp   n162_call_α
n161_statement_begin_β: mov              r11, 162;                            jmp   n165_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n162_call_α:            sub              rsp, 16
                        mov              r11, 163
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
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx564_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx564_240
                        add              rsp, 16;                             jmp   n161_statement_begin_β
.Lx564_240:                                                                   jmp   n163_assign_α
n162_call_β:            mov              r11, 163;                            jmp   n161_statement_begin_β
.Lx564_0:               .quad            .Lx564_0_s
.Lx564_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n163_assign_α:          mov              r11, 164
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx565_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n164_statement_end_α
.Lx565_0:               .quad            .Lx565_0_s
.Lx565_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n164_statement_end_α:   mov              r11, 165
                        mov              r10, 43
                        add              rsp, 16;                             jmp   n165_statement_begin_α
#=======================================================================================================================
#         <stmt 44, line 99: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n165_statement_begin_α: mov              r11, 166
                        mov              r10, 44;                             jmp   n166_call_α
n165_statement_begin_β: mov              r11, 166;                            jmp   n169_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n166_call_α:            sub              rsp, 16
                        mov              r11, 167
                        lea              rcx, [rip + .Lsig571z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
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
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx571_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx571_240
                        add              rsp, 16;                             jmp   n165_statement_begin_β
.Lx571_240:                                                                   jmp   n167_assign_α
n166_call_β:            mov              r11, 167;                            jmp   n165_statement_begin_β
.Lx571_0:               .quad            .Lx571_0_s
.Lx571_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n167_assign_α:          mov              r11, 168
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx572_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n168_statement_end_α
.Lx572_0:               .quad            .Lx572_0_s
.Lx572_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n168_statement_end_α:   mov              r11, 169
                        mov              r10, 44
                        add              rsp, 16;                             jmp   n169_statement_begin_α
#=======================================================================================================================
#         <stmt 45, line 100: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n169_statement_begin_α: mov              r11, 170
                        mov              r10, 45;                             jmp   n170_call_α
n169_statement_begin_β: mov              r11, 170;                            jmp   n173_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n170_call_α:            sub              rsp, 16
                        mov              r11, 171
                        lea              rcx, [rip + .Lsig578z]
                        lea              rax, [rip + stack_depth_α];          jmp   rax
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
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx578_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx578_240
                        add              rsp, 16;                             jmp   n169_statement_begin_β
.Lx578_240:                                                                   jmp   n171_assign_α
n170_call_β:            mov              r11, 171;                            jmp   n169_statement_begin_β
.Lx578_0:               .quad            .Lx578_0_s
.Lx578_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n171_assign_α:          mov              r11, 172
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx579_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n172_statement_end_α
.Lx579_0:               .quad            .Lx579_0_s
.Lx579_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n172_statement_end_α:   mov              r11, 173
                        mov              r10, 45
                        add              rsp, 16;                             jmp   n173_statement_begin_α
#=======================================================================================================================
#         <stmt 46, line 101: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n173_statement_begin_α: mov              r11, 174
                        mov              r10, 46;                             jmp   n174_call_α
n173_statement_begin_β: mov              r11, 174;                            jmp   n177_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n174_call_α:            sub              rsp, 16
                        mov              r11, 175
                        lea              rcx, [rip + .Lsig585z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
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
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx585_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx585_240
                        add              rsp, 16;                             jmp   n173_statement_begin_β
.Lx585_240:                                                                   jmp   n175_assign_α
n174_call_β:            mov              r11, 175;                            jmp   n173_statement_begin_β
.Lx585_0:               .quad            .Lx585_0_s
.Lx585_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n175_assign_α:          mov              r11, 176
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx586_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n176_statement_end_α
.Lx586_0:               .quad            .Lx586_0_s
.Lx586_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n176_statement_end_α:   mov              r11, 177
                        mov              r10, 46
                        add              rsp, 16;                             jmp   n177_statement_begin_α
#=======================================================================================================================
#         <stmt 47, line 102: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n177_statement_begin_α: mov              r11, 178
                        mov              r10, 47;                             jmp   n178_call_α
n177_statement_begin_β: mov              r11, 178;                            jmp   n181_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n178_call_α:            sub              rsp, 16
                        mov              r11, 179
                        lea              rcx, [rip + .Lsig592z]
                        lea              rax, [rip + stack_depth_α];          jmp   rax
.Lsig592z:              .quad            0
                        .quad            .Lx592_2
                        .quad            .Lx592_2
.Lx592_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx592_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx592_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx592_240
                        add              rsp, 16;                             jmp   n177_statement_begin_β
.Lx592_240:                                                                   jmp   n179_assign_α
n178_call_β:            mov              r11, 179;                            jmp   n177_statement_begin_β
.Lx592_0:               .quad            .Lx592_0_s
.Lx592_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n179_assign_α:          mov              r11, 180
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx593_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n180_statement_end_α
.Lx593_0:               .quad            .Lx593_0_s
.Lx593_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n180_statement_end_α:   mov              r11, 181
                        mov              r10, 47;                             jmp   n181_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n181_statement_begin_α: mov              r11, 182
                        mov              r10, 0;                              jmp   n182_statement_end_α
n181_statement_begin_β: mov              r11, 182
                        add              rsp, 16;                             jmp   n183_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n182_statement_end_α:   mov              r11, 183
                        mov              r10, 48
                        add              rsp, 16;                             jmp   n183_statement_begin_α
#=======================================================================================================================
#         <stmt 49, line 105: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n183_statement_begin_α: mov              r11, 184
                        mov              r10, 49;                             jmp   n184_call_α
n183_statement_begin_β: mov              r11, 184;                            jmp   n186_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n184_call_α:            sub              rsp, 16
                        mov              r11, 185
                        lea              rcx, [rip + .Lsig603z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig603z:              .quad            0
                        .quad            .Lx603_2
                        .quad            .Lx603_2
.Lx603_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx603_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx603_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx603_240
                        add              rsp, 16;                             jmp   n183_statement_begin_β
.Lx603_240:                                                                   jmp   n185_statement_end_α
n184_call_β:            mov              r11, 185;                            jmp   n183_statement_begin_β
.Lx603_0:               .quad            .Lx603_0_s
.Lx603_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n185_statement_end_α:   mov              r11, 186
                        mov              r10, 49
                        add              rsp, 16;                             jmp   n190_statement_begin_α
#=======================================================================================================================
#         <stmt 50, line 106: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n186_statement_begin_α: mov              r11, 187
                        mov              r10, 50;                             jmp   n187_lit_string_α
n186_statement_begin_β: mov              r11, 187;                            jmp   n194_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n187_lit_string_α:      sub              rsp, 16
                        mov              r11, 188
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx608_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n188_assign_α
.Lx608_0:               .quad            .Lx608_0_s
.Lx608_0_s:             .string          "empty ok"
#-----------------------------------------------------------------------------------------------------------------------
n188_assign_α:          mov              r11, 189
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx609_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n189_statement_end_α
.Lx609_0:               .quad            .Lx609_0_s
.Lx609_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n189_statement_end_α:   mov              r11, 190
                        mov              r10, 50
                        add              rsp, 16;                             jmp   n194_statement_begin_α
#=======================================================================================================================
# bad_s1  <stmt 51, line 107: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n190_statement_begin_α: mov              r11, 191
                        mov              r10, 51;                             jmp   n191_lit_string_α
n190_statement_begin_β: mov              r11, 191;                            jmp   n194_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n191_lit_string_α:      sub              rsp, 16
                        mov              r11, 192
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx614_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n192_assign_α
.Lx614_0:               .quad            .Lx614_0_s
.Lx614_0_s:             .string          "FAIL: empty pop should FRETURN"
#-----------------------------------------------------------------------------------------------------------------------
n192_assign_α:          mov              r11, 193
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx615_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n193_statement_end_α
.Lx615_0:               .quad            .Lx615_0_s
.Lx615_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n193_statement_end_α:   mov              r11, 194
                        mov              r10, 51
                        add              rsp, 16;                             jmp   n194_statement_begin_α
#=======================================================================================================================
# s1  <stmt 52, line 109: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n194_statement_begin_α: mov              r11, 195
                        mov              r10, 52;                             jmp   n195_statement_end_α
n194_statement_begin_β: mov              r11, 195;                            jmp   n196_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n195_statement_end_α:   mov              r11, 196
                        mov              r10, 52;                             jmp   n196_statement_begin_α
#=======================================================================================================================
#         <stmt 53, line 111: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n196_statement_begin_α: mov              r11, 197
                        mov              r10, 53;                             jmp   n197_call_α
n196_statement_begin_β: mov              r11, 197;                            jmp   n199_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n197_call_α:            sub              rsp, 16
                        mov              r11, 198
                        lea              rcx, [rip + .Lsig625z]
                        lea              rax, [rip + stack_init_α];           jmp   rax
.Lsig625z:              .quad            0
                        .quad            .Lx625_2
                        .quad            .Lx625_2
.Lx625_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx625_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx625_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx625_240
                        add              rsp, 16;                             jmp   n196_statement_begin_β
.Lx625_240:                                                                   jmp   n198_statement_end_α
n197_call_β:            mov              r11, 198;                            jmp   n196_statement_begin_β
.Lx625_0:               .quad            .Lx625_0_s
.Lx625_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n198_statement_end_α:   mov              r11, 199
                        mov              r10, 53
                        add              rsp, 16;                             jmp   n199_statement_begin_α
#=======================================================================================================================
#         <stmt 54, line 112: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n199_statement_begin_α: mov              r11, 200
                        mov              r10, 54;                             jmp   n200_lit_string_α
n199_statement_begin_β: mov              r11, 200;                            jmp   n203_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n200_lit_string_α:      sub              rsp, 16
                        mov              r11, 201
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx630_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n201_call_α
.Lx630_0:               .quad            .Lx630_0_s
.Lx630_0_s:             .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n201_call_α:            sub              rsp, 16
                        mov              r11, 202
                        lea              rcx, [rip + .Lsig632z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig632z:              .quad            1
                        .quad            .Lx632_2
                        .quad            .Lx632_2
                        .quad            16
.Lx632_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx632_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx632_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx632_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n199_statement_begin_β
.Lx632_240:                                                                   jmp   n202_statement_end_α
n201_call_β:            mov              r11, 202;                            jmp   n199_statement_begin_β
.Lx632_0:               .quad            .Lx632_0_s
.Lx632_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n202_statement_end_α:   mov              r11, 203
                        mov              r10, 54
                        add              rsp, 32;                             jmp   n203_statement_begin_α
#=======================================================================================================================
#         <stmt 55, line 113: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n203_statement_begin_α: mov              r11, 204
                        mov              r10, 55;                             jmp   n204_call_α
n203_statement_begin_β: mov              r11, 204;                            jmp   n207_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n204_call_α:            sub              rsp, 16
                        mov              r11, 205
                        lea              rcx, [rip + .Lsig638z]
                        lea              rax, [rip + stack_peek_α];           jmp   rax
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
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx638_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx638_240
                        add              rsp, 16;                             jmp   n203_statement_begin_β
.Lx638_240:                                                                   jmp   n205_assign_α
n204_call_β:            mov              r11, 205;                            jmp   n203_statement_begin_β
.Lx638_0:               .quad            .Lx638_0_s
.Lx638_0_s:             .string          "stack_peek"
#-----------------------------------------------------------------------------------------------------------------------
n205_assign_α:          mov              r11, 206
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx639_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n206_statement_end_α
.Lx639_0:               .quad            .Lx639_0_s
.Lx639_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n206_statement_end_α:   mov              r11, 207
                        mov              r10, 55
                        add              rsp, 16;                             jmp   n207_statement_begin_α
#=======================================================================================================================
#         <stmt 56, line 114: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n207_statement_begin_α: mov              r11, 208
                        mov              r10, 56;                             jmp   n208_call_α
n207_statement_begin_β: mov              r11, 208;                            jmp   n211_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n208_call_α:            sub              rsp, 16
                        mov              r11, 209
                        lea              rcx, [rip + .Lsig645z]
                        lea              rax, [rip + stack_depth_α];          jmp   rax
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
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx645_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx645_240
                        add              rsp, 16;                             jmp   n207_statement_begin_β
.Lx645_240:                                                                   jmp   n209_assign_α
n208_call_β:            mov              r11, 209;                            jmp   n207_statement_begin_β
.Lx645_0:               .quad            .Lx645_0_s
.Lx645_0_s:             .string          "stack_depth"
#-----------------------------------------------------------------------------------------------------------------------
n209_assign_α:          mov              r11, 210
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx646_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n210_statement_end_α
.Lx646_0:               .quad            .Lx646_0_s
.Lx646_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n210_statement_end_α:   mov              r11, 211
                        mov              r10, 56
                        add              rsp, 16;                             jmp   n211_statement_begin_α
#=======================================================================================================================
#         <stmt 57, line 115: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n211_statement_begin_α: mov              r11, 212
                        mov              r10, 57;                             jmp   n212_call_α
n211_statement_begin_β: mov              r11, 212;                            jmp   n215_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n212_call_α:            sub              rsp, 16
                        mov              r11, 213
                        lea              rcx, [rip + .Lsig652z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig652z:              .quad            0
                        .quad            .Lx652_2
                        .quad            .Lx652_2
.Lx652_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx652_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx652_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx652_240
                        add              rsp, 16;                             jmp   n211_statement_begin_β
.Lx652_240:                                                                   jmp   n213_assign_α
n212_call_β:            mov              r11, 213;                            jmp   n211_statement_begin_β
.Lx652_0:               .quad            .Lx652_0_s
.Lx652_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n213_assign_α:          mov              r11, 214
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx653_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n214_statement_end_α
.Lx653_0:               .quad            .Lx653_0_s
.Lx653_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n214_statement_end_α:   mov              r11, 215
                        mov              r10, 57;                             jmp   n215_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n215_statement_begin_α: mov              r11, 216
                        mov              r10, 0;                              jmp   n216_statement_end_α
n215_statement_begin_β: mov              r11, 216
                        add              rsp, 16;                             jmp   n217_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n216_statement_end_α:   mov              r11, 217
                        mov              r10, 58
                        add              rsp, 16;                             jmp   n217_statement_begin_α
#=======================================================================================================================
#         <stmt 59, line 118: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n217_statement_begin_α: mov              r11, 218
                        mov              r10, 59;                             jmp   n218_call_α
n217_statement_begin_β: mov              r11, 218;                            jmp   n220_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n218_call_α:            sub              rsp, 16
                        mov              r11, 219
                        lea              rcx, [rip + .Lsig663z]
                        lea              rax, [rip + stack_init_α];           jmp   rax
.Lsig663z:              .quad            0
                        .quad            .Lx663_2
                        .quad            .Lx663_2
.Lx663_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx663_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx663_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx663_240
                        add              rsp, 16;                             jmp   n217_statement_begin_β
.Lx663_240:                                                                   jmp   n219_statement_end_α
n218_call_β:            mov              r11, 219;                            jmp   n217_statement_begin_β
.Lx663_0:               .quad            .Lx663_0_s
.Lx663_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n219_statement_end_α:   mov              r11, 220
                        mov              r10, 59
                        add              rsp, 16;                             jmp   n220_statement_begin_α
#=======================================================================================================================
#         <stmt 60, line 119: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n220_statement_begin_α: mov              r11, 221
                        mov              r10, 60;                             jmp   n221_lit_integer_α
n220_statement_begin_β: mov              r11, 221;                            jmp   n224_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n221_lit_integer_α:     sub              rsp, 16
                        mov              r11, 222
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx668_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n222_call_α
.Lx668_0:               .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n222_call_α:            sub              rsp, 16
                        mov              r11, 223
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
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx670_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx670_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n220_statement_begin_β
.Lx670_240:                                                                   jmp   n223_statement_end_α
n222_call_β:            mov              r11, 223;                            jmp   n220_statement_begin_β
.Lx670_0:               .quad            .Lx670_0_s
.Lx670_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n223_statement_end_α:   mov              r11, 224
                        mov              r10, 60
                        add              rsp, 32;                             jmp   n224_statement_begin_α
#=======================================================================================================================
#         <stmt 61, line 120: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n224_statement_begin_α: mov              r11, 225
                        mov              r10, 61;                             jmp   n225_lit_integer_α
n224_statement_begin_β: mov              r11, 225;                            jmp   n228_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n225_lit_integer_α:     sub              rsp, 16
                        mov              r11, 226
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx675_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n226_call_α
.Lx675_0:               .quad            99
#-----------------------------------------------------------------------------------------------------------------------
n226_call_α:            sub              rsp, 16
                        mov              r11, 227
                        lea              rcx, [rip + .Lsig677z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
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
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx677_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx677_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n224_statement_begin_β
.Lx677_240:                                                                   jmp   n227_statement_end_α
n226_call_β:            mov              r11, 227;                            jmp   n224_statement_begin_β
.Lx677_0:               .quad            .Lx677_0_s
.Lx677_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n227_statement_end_α:   mov              r11, 228
                        mov              r10, 61
                        add              rsp, 32;                             jmp   n228_statement_begin_α
#=======================================================================================================================
#         <stmt 62, line 121: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n228_statement_begin_α: mov              r11, 229
                        mov              r10, 62;                             jmp   n229_lit_string_α
n228_statement_begin_β: mov              r11, 229;                            jmp   n232_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n229_lit_string_α:      sub              rsp, 16
                        mov              r11, 230
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx682_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n230_call_α
.Lx682_0:               .quad            .Lx682_0_s
.Lx682_0_s:             .string          "myvar"
#-----------------------------------------------------------------------------------------------------------------------
n230_call_α:            sub              rsp, 16
                        mov              r11, 231
                        lea              rcx, [rip + .Lsig684z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig684z:              .quad            1
                        .quad            .Lx684_2
                        .quad            .Lx684_2
                        .quad            16
.Lx684_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx684_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx684_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx684_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n228_statement_begin_β
.Lx684_240:                                                                   jmp   n231_statement_end_α
n230_call_β:            mov              r11, 231;                            jmp   n228_statement_begin_β
.Lx684_0:               .quad            .Lx684_0_s
.Lx684_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n231_statement_end_α:   mov              r11, 232
                        mov              r10, 62
                        add              rsp, 32;                             jmp   n232_statement_begin_α
#=======================================================================================================================
#         <stmt 63, line 122: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n232_statement_begin_α: mov              r11, 233
                        mov              r10, 63;                             jmp   n233_var_α
n232_statement_begin_β: mov              r11, 233;                            jmp   n236_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n233_var_α:             sub              rsp, 16
                        mov              r11, 234
                        mov              rax, qword ptr [r9 + 176]            # myvar
                        mov              rdx, qword ptr [r9 + 184]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n234_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n234_assign_α:          mov              r11, 235
                        mov              rsi, qword ptr [rsp + 0]             # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx690_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n235_statement_end_α
.Lx690_0:               .quad            .Lx690_0_s
.Lx690_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n235_statement_end_α:   mov              r11, 236
                        mov              r10, 63;                             jmp   n236_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n236_statement_begin_α: mov              r11, 237
                        mov              r10, 0;                              jmp   n237_statement_end_α
n236_statement_begin_β: mov              r11, 237
                        add              rsp, 16;                             jmp   n238_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n237_statement_end_α:   mov              r11, 238
                        mov              r10, 64
                        add              rsp, 16;                             jmp   n238_statement_begin_α
#=======================================================================================================================
#         <stmt 65, line 125: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n238_statement_begin_α: mov              r11, 239
                        mov              r10, 65;                             jmp   n239_call_α
n238_statement_begin_β: mov              r11, 239;                            jmp   n241_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n239_call_α:            sub              rsp, 16
                        mov              r11, 240
                        lea              rcx, [rip + .Lsig700z]
                        lea              rax, [rip + stack_init_α];           jmp   rax
.Lsig700z:              .quad            0
                        .quad            .Lx700_2
                        .quad            .Lx700_2
.Lx700_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx700_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx700_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx700_240
                        add              rsp, 16;                             jmp   n238_statement_begin_β
.Lx700_240:                                                                   jmp   n240_statement_end_α
n239_call_β:            mov              r11, 240;                            jmp   n238_statement_begin_β
.Lx700_0:               .quad            .Lx700_0_s
.Lx700_0_s:             .string          "stack_init"
#-----------------------------------------------------------------------------------------------------------------------
n240_statement_end_α:   mov              r11, 241
                        mov              r10, 65
                        add              rsp, 16;                             jmp   n241_statement_begin_α
#=======================================================================================================================
#         <stmt 66, line 126: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n241_statement_begin_α: mov              r11, 242
                        mov              r10, 66;                             jmp   n242_lit_string_α
n241_statement_begin_β: mov              r11, 242;                            jmp   n245_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n242_lit_string_α:      sub              rsp, 16
                        mov              r11, 243
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 11
                        mov              rax, qword ptr [rip + .Lx705_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n243_assign_α
.Lx705_0:               .quad            .Lx705_0_s
.Lx705_0_s:             .string          "hello world"
#-----------------------------------------------------------------------------------------------------------------------
n243_assign_α:          mov              r11, 244
                        mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 192], rax            # subject
                        mov              qword ptr [r9 + 200], rdx;           jmp   n244_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n244_statement_end_α:   mov              r11, 245
                        mov              r10, 66
                        add              rsp, 16;                             jmp   n245_statement_begin_α
#=======================================================================================================================
#         <stmt 67, line 127: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n245_statement_begin_α: mov              r11, 246
                        mov              r10, 67;                             jmp   n246_var_α
n245_statement_begin_β: mov              r11, 246;                            jmp   n277_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n246_var_α:             sub              rsp, 16
                        mov              r11, 247
                        mov              rax, qword ptr [r9 + 192]            # subject
                        mov              rdx, qword ptr [r9 + 200]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n247_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n247_match_begin_α:     mov              r11, 248
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
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              dword ptr [rbp + -40], 0             # start_δ
.Lx713_0:               mov              r14d, dword ptr [rbp + -40]
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # match_beta_cont
                        mov              rax, qword ptr [rcx + 248]
                        mov              qword ptr [rbp + -48], rax
                        lea              rax, [rip + .Lx713_13]
                        mov              qword ptr [rcx + 248], rax;          jmp   n248_match_assign_save_α
n247_match_begin_β:     mov              r11, 248
.Lx713_13:              lea              rsp, [rbp + -56]                     # retry_whack
                        add              dword ptr [rbp + -40], 1             # start_δ
                        mov              eax, dword ptr [rbp + -40]
                        cmp              eax, r15d;                           jg    .Lx713_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0;                              jne   .Lx713_1
                                                                              jmp   .Lx713_0
.Lx713_1:
n247_match_begin_af:    mov              r11, 248
                        mov              rcx, qword ptr [rip + rtccb@GOTPCREL] # mbc_restore
                        mov              rax, qword ptr [rbp + -48]
                        mov              qword ptr [rcx + 248], rax
                        mov              r12, qword ptr [rbp + -8]            # cas_mark
                        mov              r13, qword ptr [rbp + -16]           # outer_Σ
                        mov              r14, qword ptr [rbp + -24]           # outer_δ
                        mov              r15, qword ptr [rbp + -32]           # outer_Δ
                        mov              rdi, r13                             # sig
                        mov              rsi, r15                             # len
                        call             rt_match_ctx_restore@PLT
                        mov              rsp, rbp
                        pop              rbp
                        add              rsp, 16;                             jmp   n277_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n248_match_assign_save_α:
                        sub              rsp, 16
                        mov              r11, 249
                        mov              dword ptr [rsp + 0], r14d;           jmp   n249_match_break_α
n248_match_assign_save_β:
                        mov              r11, 249
                        add              rsp, 16;                             jmp   n247_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n249_match_break_α:     sub              rsp, 16
                        mov              r11, 250
                        movsxd           rcx, r14d
.Lx717_0:               cmp              ecx, r15d;                           jl    .Lx717_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n247_match_begin_β
.Lx717_240:             movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32;                             je    .Lx717_1
                        add              ecx, 1;                              jmp   .Lx717_0
.Lx717_1:               mov              dword ptr [rsp + 0], r14d
                        mov              r14d, ecx;                           jmp   n250_match_assign_cond_α
n249_match_break_β:     mov              r11, 250
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n247_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n250_match_assign_cond_α:
                        mov              r11, 251
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n251_match_lit_α
n250_match_assign_cond_β:
                        mov              r11, 251
                        sub              r12, 24;                             jmp   n249_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n251_match_lit_α:       mov              r11, 252
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d;                           jg    n250_match_assign_cond_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 32;                             jne   n250_match_assign_cond_β
                        add              r14d, 1;                             jmp   n252_match_assign_save_α
n251_match_lit_β:       mov              r11, 252
                        sub              r14d, 1;                             jmp   n250_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n252_match_assign_save_α:
                        sub              rsp, 16
                        mov              r11, 253
                        mov              dword ptr [rsp + 0], r14d;           jmp   n253_match_rem_α
n252_match_assign_save_β:
                        mov              r11, 253
                        add              rsp, 16;                             jmp   n251_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n253_match_rem_α:       sub              rsp, 16
                        mov              r11, 254
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d;                          jmp   n254_match_assign_cond_α
n253_match_rem_β:       mov              r11, 254
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n251_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n254_match_assign_cond_α:
                        mov              r11, 255
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24;                             jmp   n255_match_end_α
n254_match_assign_cond_β:
                        mov              r11, 255
                        sub              r12, 24;                             jmp   n253_match_rem_β
#-----------------------------------------------------------------------------------------------------------------------
n255_match_end_α:       mov              r11, 256
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
                        call             rt_match_end_all@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx728_13
                        add              rsp, 16
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14;                                 jmp   n247_match_begin_af
.Lx728_13:              add              rsp, 16
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
                        pop              rbp;                                 jmp   n256_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n256_lit_string_α:      sub              rsp, 16
                        mov              r11, 257
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx729_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n257_match_replace_α
.Lx729_0:               .quad            .Lx729_0_s
.Lx729_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n257_match_replace_α:   mov              r11, 258
                        mov              rdi, qword ptr [rip + .Lx731_0]
                        mov              rsi, qword ptr [rsp + 16]            # var
                        mov              rdx, qword ptr [rsp + 24]
                        mov              ecx, dword ptr [r12 + -16]           # repl_start
                        mov              r8, qword ptr [r12 + -8]             # repl_end
                        sub              r12, 16
                        lea              r9, [rsp + 0]                        # lit_string
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_match_replace@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16;                             jmp   .Lx731_1
.Lx731_0:               .quad            .Lx731_0_s
.Lx731_0_s:             .string          "subject"
.Lx731_1:                                                                     jmp   n258_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n258_statement_end_α:   mov              r11, 259
                        mov              r10, 67
                        add              rsp, 16;                             jmp   n259_statement_begin_α
#=======================================================================================================================
#         <stmt 68, line 128: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n259_statement_begin_α: mov              r11, 260
                        mov              r10, 68;                             jmp   n260_var_α
n259_statement_begin_β: mov              r11, 260;                            jmp   n263_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n260_var_α:             sub              rsp, 16
                        mov              r11, 261
                        mov              rax, qword ptr [r9 + 208]            # w1
                        mov              rdx, qword ptr [r9 + 216]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n261_call_α
#-----------------------------------------------------------------------------------------------------------------------
n261_call_α:            sub              rsp, 16
                        mov              r11, 262
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
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx738_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx738_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n259_statement_begin_β
.Lx738_240:                                                                   jmp   n262_statement_end_α
n261_call_β:            mov              r11, 262;                            jmp   n259_statement_begin_β
.Lx738_0:               .quad            .Lx738_0_s
.Lx738_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n262_statement_end_α:   mov              r11, 263
                        mov              r10, 68
                        add              rsp, 32;                             jmp   n263_statement_begin_α
#=======================================================================================================================
#         <stmt 69, line 129: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n263_statement_begin_α: mov              r11, 264
                        mov              r10, 69;                             jmp   n264_var_α
n263_statement_begin_β: mov              r11, 264;                            jmp   n267_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n264_var_α:             sub              rsp, 16
                        mov              r11, 265
                        mov              rax, qword ptr [r9 + 224]            # w2
                        mov              rdx, qword ptr [r9 + 232]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n265_call_α
#-----------------------------------------------------------------------------------------------------------------------
n265_call_α:            sub              rsp, 16
                        mov              r11, 266
                        lea              rcx, [rip + .Lsig745z]
                        lea              rax, [rip + stack_push_α];           jmp   rax
.Lsig745z:              .quad            1
                        .quad            .Lx745_2
                        .quad            .Lx745_2
                        .quad            16
.Lx745_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx745_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx745_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx745_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n263_statement_begin_β
.Lx745_240:                                                                   jmp   n266_statement_end_α
n265_call_β:            mov              r11, 266;                            jmp   n263_statement_begin_β
.Lx745_0:               .quad            .Lx745_0_s
.Lx745_0_s:             .string          "stack_push"
#-----------------------------------------------------------------------------------------------------------------------
n266_statement_end_α:   mov              r11, 267
                        mov              r10, 69
                        add              rsp, 32;                             jmp   n267_statement_begin_α
#=======================================================================================================================
#         <stmt 70, line 130: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n267_statement_begin_α: mov              r11, 268
                        mov              r10, 70;                             jmp   n268_call_α
n267_statement_begin_β: mov              r11, 268;                            jmp   n271_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n268_call_α:            sub              rsp, 16
                        mov              r11, 269
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
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx751_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx751_240
                        add              rsp, 16;                             jmp   n267_statement_begin_β
.Lx751_240:                                                                   jmp   n269_assign_α
n268_call_β:            mov              r11, 269;                            jmp   n267_statement_begin_β
.Lx751_0:               .quad            .Lx751_0_s
.Lx751_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n269_assign_α:          mov              r11, 270
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx752_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n270_statement_end_α
.Lx752_0:               .quad            .Lx752_0_s
.Lx752_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n270_statement_end_α:   mov              r11, 271
                        mov              r10, 70
                        add              rsp, 16;                             jmp   n271_statement_begin_α
#=======================================================================================================================
#         <stmt 71, line 131: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n271_statement_begin_α: mov              r11, 272
                        mov              r10, 71;                             jmp   n272_call_α
n271_statement_begin_β: mov              r11, 272;                            jmp   n275_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n272_call_α:            sub              rsp, 16
                        mov              r11, 273
                        lea              rcx, [rip + .Lsig758z]
                        lea              rax, [rip + stack_pop_α];            jmp   rax
.Lsig758z:              .quad            0
                        .quad            .Lx758_2
                        .quad            .Lx758_2
.Lx758_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx758_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx758_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx758_240
                        add              rsp, 16;                             jmp   n271_statement_begin_β
.Lx758_240:                                                                   jmp   n273_assign_α
n272_call_β:            mov              r11, 273;                            jmp   n271_statement_begin_β
.Lx758_0:               .quad            .Lx758_0_s
.Lx758_0_s:             .string          "stack_pop"
#-----------------------------------------------------------------------------------------------------------------------
n273_assign_α:          mov              r11, 274
                        mov              rsi, qword ptr [rsp + 0]             # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx759_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n274_statement_end_α
.Lx759_0:               .quad            .Lx759_0_s
.Lx759_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n274_statement_end_α:   mov              r11, 275
                        mov              r10, 71
                        add              rsp, 16;                             jmp   n275_statement_begin_α
#=======================================================================================================================
#         <stmt 72, line 132: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n275_statement_begin_α: mov              r11, 276
                        mov              r10, 72;                             jmp   n276_statement_end_α
n275_statement_begin_β: mov              r11, 276;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n276_statement_end_α:   mov              r11, 277
                        mov              r10, 72;                             jmp   main_γ
#=======================================================================================================================
# bad_s2  <stmt 73, line 133: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n277_statement_begin_α: mov              r11, 278
                        mov              r10, 73;                             jmp   n278_lit_string_α
n277_statement_begin_β: mov              r11, 278;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n278_lit_string_α:      sub              rsp, 16
                        mov              r11, 279
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 26
                        mov              rax, qword ptr [rip + .Lx768_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n279_assign_α
.Lx768_0:               .quad            .Lx768_0_s
.Lx768_0_s:             .string          "FAIL: pattern match failed"
#-----------------------------------------------------------------------------------------------------------------------
n279_assign_α:          mov              r11, 280
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx769_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n280_statement_end_α
.Lx769_0:               .quad            .Lx769_0_s
.Lx769_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n280_statement_end_α:   mov              r11, 281
                        mov              r10, 73
                        add              rsp, 16;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 mov              r11, 282
                        pop              rcx
                        add              rsp, 8;                              jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                mov              r11, 283
                        add              rsp, 8
                        pop              rcx;                                 jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
NRETURN:                mov              r11, 284
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
