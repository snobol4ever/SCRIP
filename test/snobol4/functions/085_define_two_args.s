                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__add_α:
proc_LBL__add_α_body:
#=======================================================================================================================
# add     add = a + b                                                :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_var_α
n0_statement_begin_β:                                               jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # a
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n2_var_α
#-----------------------------------------------------------------------------------------------------------------------
n2_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # b
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n3_binop_α
n2_var_β:               add              rsp, 16
                        add              rsp, 16;                   jmp   n0_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n3_binop_α:             sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_add@PLT
                        cmp              eax, 104;                  jne   .Lx10_240
                        add              rsp, 16;                   jmp   n2_var_β
.Lx10_240:              mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n4_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n4_assign_α:            mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # add
                        mov              qword ptr [r9 + 8], rdx;   jmp   n5_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_end_α:     add              rsp, 48;                   jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__add_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__add_β:
                                                                    jmp   proc_LBL__add_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__add_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__add_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_add_α
proc_add_α:
proc_add_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n14_save_restore_α:                                                 jmp   n15_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n15_goto_deferred_α:    lea              rax, [rip + proc_LBL__add_α]; jmp   rax
                                                                    jmp   n16_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n16_save_restore_α:
add_alpha:              mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 32
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx22_2
                        mov              eax, 32
.Lx22_2:                sub              rsp, 80
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 0]                        # add
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        cmp              rcx, 0;                    jbe   .Lx22_10
                        mov              rax, qword ptr [r8 + 64]
                        mov              rdx, qword ptr [r9 + 16]                       # a
                        mov              qword ptr [r9 + 16], rax
                        mov              qword ptr [r8 + 64], rdx
                        mov              rax, qword ptr [r8 + 72]
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [r9 + 24], rax
                        mov              qword ptr [r8 + 72], rdx;  jmp   .Lx22_41
.Lx22_10:               mov              rax, qword ptr [r9 + 16]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [r8 + 8], rax
                        mov              qword ptr [r9 + 16], 0
                        mov              qword ptr [r9 + 24], 0
.Lx22_41:               cmp              rcx, 1;                    jbe   .Lx22_11
                        mov              rax, qword ptr [r8 + 80]
                        mov              rdx, qword ptr [r9 + 32]                       # b
                        mov              qword ptr [r9 + 32], rax
                        mov              qword ptr [r8 + 80], rdx
                        mov              rax, qword ptr [r8 + 88]
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [r9 + 40], rax
                        mov              qword ptr [r8 + 88], rdx;  jmp   .Lx22_42
.Lx22_11:               mov              rax, qword ptr [r9 + 32]
                        mov              qword ptr [r8 + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [r8 + 24], rax
                        mov              qword ptr [r9 + 32], 0
                        mov              qword ptr [r9 + 40], 0
.Lx22_42:               lea              r10, [rip + add_gamma]
                        lea              r11, [rip + add_omega]
                        lea              rax, [rip + proc_LBL__add_α]; jmp   rax
add_gamma:              mov              rdi, qword ptr [r9 + 0]                        # add
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx22_75
                        mov              eax, 32
.Lx22_75:               lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rcx, 0;                    jbe   .Lx22_80
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx22_110
.Lx22_80:               mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx22_110:              cmp              rcx, 1;                    jbe   .Lx22_81
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx22_111
.Lx22_81:               mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx22_111:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                    jbe   .Lx22_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx22_3:                mov              rcx, qword ptr [r8 + 48]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
add_omega:              mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 2;                    jbe   .Lx22_145
                        mov              eax, 32
.Lx22_145:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax                        # add
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        cmp              rcx, 0;                    jbe   .Lx22_150
                        mov              rax, qword ptr [r8 + 64]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              rax, qword ptr [r8 + 72]
                        mov              qword ptr [r9 + 24], rax;  jmp   .Lx22_180
.Lx22_150:              mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
.Lx22_180:              cmp              rcx, 1;                    jbe   .Lx22_151
                        mov              rax, qword ptr [r8 + 80]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              rax, qword ptr [r8 + 88]
                        mov              qword ptr [r9 + 40], rax;  jmp   .Lx22_181
.Lx22_151:              mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
.Lx22_181:              mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 112
                        cmp              rcx, 2;                    jbe   .Lx22_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx22_4:                mov              rcx, qword ptr [r8 + 56]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_add_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_add_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_add_β:
                                                                    jmp   proc_add_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_add_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_add_ω:
                                                                    jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__add"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__add_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 288
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "add"
.Lgvan1:                .string          "a"
.Lgvan2:                .string          "b"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 3
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 3
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                    jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         DEFINE('add(a,b)')                                          :(add_end)
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_begin_α:                                              jmp   n24_func_activate_α
n23_statement_begin_β:                                              jmp   n26_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n24_func_activate_α:    mov              rdi, qword ptr [rip + .Lx45_0]
                        mov              rsi, qword ptr [rip + .Lx45_1]
                        mov              edx, 2
                        mov              ecx, 2
                        mov              r8d, 0
                        lea              r9, [rip + proc_add_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n25_statement_end_α
n24_func_activate_β:                                                jmp   n23_statement_begin_β
.Lx45_0:                .quad            .Lx45_0_s
.Lx45_0_s:              .string          "add"
.Lx45_1:                .quad            .Lx45_1_s
.Lx45_1_s:              .string          "a,b"
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_end_α:                                                jmp   n26_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_begin_α:                                              jmp   n27_statement_end_α
n26_statement_begin_β:                                              jmp   n28_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_end_α:                                                jmp   n28_statement_begin_α
#=======================================================================================================================
#         OUTPUT = add(3, 4)
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_begin_α:                                              jmp   n29_lit_integer_α
n28_statement_begin_β:                                              jmp   n34_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx54_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n30_lit_integer_α
.Lx54_0:                .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx55_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n31_call_α
n30_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n28_statement_begin_β
.Lx55_0:                .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n31_call_α:             sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx57_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + add_alpha];    jmp   rax
.Lx57_2:                mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx57_240
                        add              rsp, 16;                   jmp   n30_lit_integer_β
.Lx57_240:                                                          jmp   n32_assign_α
n31_call_β:                                                         jmp   n30_lit_integer_β
.Lx57_0:                .quad            .Lx57_0_s
.Lx57_0_s:              .string          "add"
#-----------------------------------------------------------------------------------------------------------------------
n32_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx58_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n33_statement_end_α
.Lx58_0:                .quad            .Lx58_0_s
.Lx58_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_end_α:    add              rsp, 48;                   jmp   n34_statement_begin_α
#=======================================================================================================================
#         OUTPUT = add(10, 32)
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_begin_α:                                              jmp   n35_lit_integer_α
n34_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx63_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n36_lit_integer_α
.Lx63_0:                .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx64_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n37_call_α
n36_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n34_statement_begin_β
.Lx64_0:                .quad            32
#-----------------------------------------------------------------------------------------------------------------------
n37_call_α:             sub              rsp, 16
                        sub              rsp, 64
                        mov              rax, qword ptr [rsp + 96]                      # lit_integer
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                        mov              eax, 2
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx66_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + add_alpha];    jmp   rax
.Lx66_2:                mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx66_240
                        add              rsp, 16;                   jmp   n36_lit_integer_β
.Lx66_240:                                                          jmp   n38_assign_α
n37_call_β:                                                         jmp   n36_lit_integer_β
.Lx66_0:                .quad            .Lx66_0_s
.Lx66_0_s:              .string          "add"
#-----------------------------------------------------------------------------------------------------------------------
n38_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx67_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n39_statement_end_α
.Lx67_0:                .quad            .Lx67_0_s
.Lx67_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_end_α:    add              rsp, 48;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n40_goto_α:                                                         jmp   n26_statement_begin_α
n40_goto_β:                                                         jmp   main_ω
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
