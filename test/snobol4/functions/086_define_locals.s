                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__swap_α:
proc_LBL__swap_α_body:
#=======================================================================================================================
# swap    tmp = a
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_var_α
n0_statement_begin_β:                                               jmp   n4_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # a
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n2_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n2_assign_α:            mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 48], rax                       # tmp
                        mov              qword ptr [r9 + 56], rdx;  jmp   n3_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n3_statement_end_α:     add              rsp, 16;                   jmp   n4_statement_begin_α
#=======================================================================================================================
#         a = b
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_begin_α:                                               jmp   n5_var_α
n4_statement_begin_β:                                               jmp   n8_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n5_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # b
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n6_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n6_assign_α:            mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              qword ptr [r9 + 24], rdx;  jmp   n7_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_end_α:     add              rsp, 16;                   jmp   n8_statement_begin_α
#=======================================================================================================================
#         b = tmp
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_begin_α:                                               jmp   n9_var_α
n8_statement_begin_β:                                               jmp   n12_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n9_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 48]                       # tmp
                        mov              rdx, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n10_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n10_assign_α:           mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              qword ptr [r9 + 40], rdx;  jmp   n11_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:    add              rsp, 16;                   jmp   n12_statement_begin_α
#=======================================================================================================================
#         OUTPUT = a ' ' b                                            :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:                                              jmp   n13_var_α
n12_statement_begin_β:                                              jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n13_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # a
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n14_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n14_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx41_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n15_binop_α
n14_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                   jmp   n12_statement_begin_β
.Lx41_0:                .quad            .Lx41_0_s
.Lx41_0_s:              .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n15_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n16_var_α
n15_binop_β:            add              rsp, 16;                   jmp   n14_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n16_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # b
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n17_binop_α
n16_var_β:              add              rsp, 16;                   jmp   n15_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n17_binop_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n18_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n18_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx45_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n19_statement_end_α
.Lx45_0:                .quad            .Lx45_0_s
.Lx45_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_end_α:    add              rsp, 80;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__swap_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__swap_β:
                                                                    jmp   proc_LBL__swap_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__swap_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__swap_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_swap_α
proc_swap_α:
proc_swap_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n48_save_restore_α:                                                 jmp   n49_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n49_goto_deferred_α:    lea              rax, [rip + proc_LBL__swap_α]; jmp   rax
                                                                    jmp   n50_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n50_save_restore_α:
swap_alpha:             mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx56_2
                        mov              eax, 32
.Lx56_2:                sub              rsp, 96
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 48]                       # tmp
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 56]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 48], 0
                        mov              qword ptr [r9 + 56], 0
                        mov              rax, qword ptr [r9 + 0]                        # swap
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 32], r10
                        mov              qword ptr [rsp + 40], r11
                        mov              qword ptr [rsp + 48], rcx
                        cmp              rcx, 0;                    jbe   .Lx56_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # a
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx;  jmp   .Lx56_41
.Lx56_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx56_41:               cmp              rcx, 1;                    jbe   .Lx56_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 32]                       # b
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx56_42
.Lx56_11:               mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx56_42:               lea              r10, [rip + swap_gamma]
                        lea              r11, [rip + swap_omega]
                        lea              rax, [rip + proc_LBL__swap_α]; jmp   rax
swap_gamma:             mov              rdi, qword ptr [r9 + 0]                        # swap
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 48]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx56_75
                        mov              eax, 32
.Lx56_75:               lea              r8, [rsp + 64]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 8], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 48], rax                       # tmp
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 56], rax
                        cmp              rcx, 0;                    jbe   .Lx56_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx56_110
.Lx56_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx56_110:              cmp              rcx, 1;                    jbe   .Lx56_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx56_111
.Lx56_81:               mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx56_111:              mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              eax, 128
                        cmp              rcx, 2;                    jbe   .Lx56_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 96
.Lx56_3:                mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
swap_omega:             mov              rcx, qword ptr [rsp + 48]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx56_145
                        mov              eax, 32
.Lx56_145:              lea              r8, [rsp + 64]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 0], rax                        # swap
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 8], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 48], rax                       # tmp
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 56], rax
                        cmp              rcx, 0;                    jbe   .Lx56_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx56_180
.Lx56_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx56_180:              cmp              rcx, 1;                    jbe   .Lx56_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx56_181
.Lx56_151:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx56_181:              mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              eax, 128
                        cmp              rcx, 2;                    jbe   .Lx56_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 96
.Lx56_4:                mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_swap_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_swap_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_swap_β:
                                                                    jmp   proc_swap_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_swap_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_swap_ω:
                                                                    jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__swap"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__swap_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 272
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "swap"
.Lgvan1:                .string          "a"
.Lgvan2:                .string          "b"
.Lgvan3:                .string          "tmp"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 4
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 4
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                    jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         DEFINE('swap(a,b)tmp')                                      :(swap_end)
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_begin_α:                                              jmp   n58_func_activate_α
n57_statement_begin_β:                                              jmp   n60_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n58_func_activate_α:    mov              rdi, qword ptr [rip + .Lx72_0]
                        mov              rsi, qword ptr [rip + .Lx72_1]
                        mov              edx, 3
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_swap_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n59_statement_end_α
n58_func_activate_β:                                                jmp   n57_statement_begin_β
.Lx72_0:                .quad            .Lx72_0_s
.Lx72_0_s:              .string          "swap"
.Lx72_1:                .quad            .Lx72_1_s
.Lx72_1_s:              .string          "a,b,tmp"
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_end_α:                                                jmp   n60_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_begin_α:                                              jmp   n61_statement_end_α
n60_statement_begin_β:                                              jmp   n62_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_end_α:                                                jmp   n62_statement_begin_α
#=======================================================================================================================
#         swap('hello', 'world')
#-----------------------------------------------------------------------------------------------------------------------
n62_statement_begin_α:                                              jmp   n63_lit_string_α
n62_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n63_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx81_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n64_lit_string_α
.Lx81_0:                .quad            .Lx81_0_s
.Lx81_0_s:              .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx82_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n65_call_α
n64_lit_string_β:       add              rsp, 16
                        add              rsp, 16;                   jmp   n62_statement_begin_β
.Lx82_0:                .quad            .Lx82_0_s
.Lx82_0_s:              .string          "world"
#-----------------------------------------------------------------------------------------------------------------------
n65_call_α:             sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_string
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx84_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + swap_alpha];   jmp   rax
.Lx84_2:                mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx84_240
                        add              rsp, 16;                   jmp   n64_lit_string_β
.Lx84_240:                                                          jmp   n66_statement_end_α
n65_call_β:                                                         jmp   n64_lit_string_β
.Lx84_0:                .quad            .Lx84_0_s
.Lx84_0_s:              .string          "swap"
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_end_α:    add              rsp, 48;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n67_goto_α:                                                         jmp   n60_statement_begin_α
n67_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
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
.S0:                    .string          "BOMB-RETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
                        .text
                        .section         .note.GNU-stack,"",@progbits
