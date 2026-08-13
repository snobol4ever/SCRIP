                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__add_α:
proc_LBL__add_α_body:
#=======================================================================================================================
# add     add = a + b                                                :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_var_α
n0_statement_begin_β:
                                                                                        jmp   n6_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # a
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n2_var_α
#-----------------------------------------------------------------------------------------------------------------------
n2_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [r9 + 32]                       # b
                        mov              rdx, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n3_binop_α
n2_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n0_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n3_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_add@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx11_240
                        add              rsp, 16
                                                                                        jmp   n2_var_β
.Lx11_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n4_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n4_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # add
                        mov              qword ptr [r9 + 8], rdx
                                                                                        jmp   n5_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_end_α:
                                                                                        jmp   n6_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n6_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx16_0
                        mov              rax, qword ptr [rcx + -64]
                        mov              ecx, 0
                                                                                        jmp   rax
.Lx16_0:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__add_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__add_β:
                                                                                        jmp   proc_LBL__add_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__add_γ:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__add_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_add_α
proc_add_α:
proc_add_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n17_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rsp
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_wire_adopt@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n18_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n18_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx22_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_goto_transfer@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx22_1
.Lx22_0:
                        .quad            .Lx22_0_s
.Lx22_0_s:
                        .string          "add"
.Lx22_1:
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
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 0]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_add_ω:
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_flat_ret_snap@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              rcx, qword ptr [rax + 8]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
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
                        mov              esi, 272
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "add"
.Lstartup_pp1_0:        .string          "a"
.Lstartup_pp1_1:        .string          "b"
                        .align           8
.Lstartup_pnames1:
                        .quad            .Lstartup_pp1_0
                        .quad            .Lstartup_pp1_1
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + .Lstartup_pnames1]
                        mov              edx, 2
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_add_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 2
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 2
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
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
                        push             rbp
                        mov              rbp, rsp
#=======================================================================================================================
#         DEFINE('add(a,b)')                                          :(add_end)
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_begin_α:
                                                                                        jmp   n24_statement_end_α
n23_statement_begin_β:
                                                                                        jmp   n25_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_end_α:
                                                                                        jmp   n25_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_begin_α:
                                                                                        jmp   n26_statement_end_α
n25_statement_begin_β:
                                                                                        jmp   n27_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_end_α:
                                                                                        jmp   n27_statement_begin_α
#=======================================================================================================================
#         OUTPUT = add(3, 4)
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_begin_α:
                                                                                        jmp   n28_lit_integer_α
n27_statement_begin_β:
                                                                                        jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n28_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx50_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n29_lit_integer_α
.Lx50_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx51_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n30_call_α
n29_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n27_statement_begin_β
.Lx51_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n30_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rdx, qword ptr [rsp + 40]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 48
                        mov              rax, qword ptr [r9 + 16]                       # a
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 32]                       # b
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [r9 + 0]                        # add
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 40], rax
                        mov              rdi, qword ptr [rip + .Lx53_0]
                        mov              esi, 2
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx53_5
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx53_6]
                        lea              rdx, [rip + .Lx53_7]
                                                                                        jmp   rax
.Lx53_6:
                        mov              rdi, qword ptr [r9 + 0]                        # add
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 8], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 40], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 48
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx53_2
.Lx53_7:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 0], rax                        # add
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 8], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 40], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 48
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx53_2
.Lx53_5:
                        add              rsp, 48
.Lx53_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx53_240
                        add              rsp, 16
                                                                                        jmp   n29_lit_integer_β
.Lx53_240:
                                                                                        jmp   n31_assign_α
n30_call_β:
                                                                                        jmp   n29_lit_integer_β
.Lx53_0:
                        .quad            .Lx53_0_s
.Lx53_0_s:
                        .string          "add"
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx54_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n32_statement_end_α
.Lx54_0:
                        .quad            .Lx54_0_s
.Lx54_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n33_statement_begin_α
#=======================================================================================================================
#         OUTPUT = add(10, 32)
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:
                                                                                        jmp   n34_lit_integer_α
n33_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx59_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n35_lit_integer_α
.Lx59_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx60_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n36_call_α
n35_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n33_statement_begin_β
.Lx60_0:
                        .quad            32
#-----------------------------------------------------------------------------------------------------------------------
n36_call_α:
                        sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rdx, qword ptr [rsp + 40]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        mov              edi, 1
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_arg_stage@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        sub              rsp, 48
                        mov              rax, qword ptr [r9 + 16]                       # a
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [r9 + 32]                       # b
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [r9 + 40]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [r9 + 0]                        # add
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 40], rax
                        mov              rdi, qword ptr [rip + .Lx62_0]
                        mov              esi, 2
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_open_slim@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        test             rax, rax
                                                                                        je    .Lx62_5
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 24], rax
                        lea              r8, [rip + g_call_args]
                        mov              rax, qword ptr [r8 + 16]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              rax, qword ptr [r8 + 24]
                        mov              qword ptr [r9 + 40], rax
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_open_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        lea              rcx, [rip + .Lx62_6]
                        lea              rdx, [rip + .Lx62_7]
                                                                                        jmp   rax
.Lx62_6:
                        mov              rdi, qword ptr [r9 + 0]                        # add
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 8], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 40], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 48
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_γ@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx62_2
.Lx62_7:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [r9 + 0], rax                        # add
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [r9 + 8], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [r9 + 32], rax                       # b
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [r9 + 40], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 24], rax
                        add              rsp, 48
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_proc_call_epilogue_slim_ω@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   .Lx62_2
.Lx62_5:
                        add              rsp, 48
.Lx62_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx62_240
                        add              rsp, 16
                                                                                        jmp   n35_lit_integer_β
.Lx62_240:
                                                                                        jmp   n37_assign_α
n36_call_β:
                                                                                        jmp   n35_lit_integer_β
.Lx62_0:
                        .quad            .Lx62_0_s
.Lx62_0_s:
                        .string          "add"
#-----------------------------------------------------------------------------------------------------------------------
n37_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx63_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                                                                                        jmp   n38_statement_end_α
.Lx63_0:
                        .quad            .Lx63_0_s
.Lx63_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n39_goto_α:
                                                                                        jmp   n25_statement_begin_α
n39_goto_β:
                                                                                        jmp   main_ω
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
                        .section         .note.GNU-stack,"",@progbits
