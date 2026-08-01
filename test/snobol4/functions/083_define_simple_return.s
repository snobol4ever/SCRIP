                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__double_α
proc_LBL__double_α:
proc_LBL__double_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_goto_α:
                                                                                        jmp   n1_lit_integer_α
n0_goto_β:
                                                                                        jmp   proc_LBL__double_ω
#=======================================================================================================================
# double  double = 2 * s                                             :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx7_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n2_var_α
.Lx7_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n2_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # s
                        mov              rdx, qword ptr [1879052312]                    # s
                        mov              qword ptr [rsp + 0], rax                       # s
                        mov              qword ptr [rsp + 8], rdx                       # s
                                                                                        jmp   n3_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n3_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 40]                      # lit_integer
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # var
                        call             rt_mul@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx9_240
                        add              rsp, 16
                                                                                        jmp   n5_save_restore_α
.Lx9_240:
                        mov              qword ptr [rsp + 0], rax                       # binop
                        mov              qword ptr [rsp + 8], rdx                       # binop
                                                                                        jmp   n4_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n4_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # binop
                        mov              qword ptr [1879052288], rax                    # double
                        mov              qword ptr [1879052296], rdx                    # double
                                                                                        jmp   n5_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n5_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__double_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__double_β:
                                                                                        jmp   proc_LBL__double_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__double_γ:
                        mov              rsp, rbp
                        pop              rbp
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__double_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_double_α
proc_double_α:
proc_double_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n13_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
                        mov              rcx, rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n14_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n14_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx18_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx18_1
.Lx18_0:
                        .quad            .Lx18_0_s
.Lx18_0_s:
                        .string          "double"
.Lx18_1:
                                                                                        jmp   proc_double_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_double_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_double_β:
                                                                                        jmp   proc_double_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_double_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_double_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__double"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__double_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 208
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "double"
.Lstartup_pp1_0:        .string          "s"
                        .align           8
.Lstartup_pnames1:
                        .quad            .Lstartup_pp1_0
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + .Lstartup_pnames1]
                        mov              edx, 1
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_double_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
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
.Lgvan0:                .string          "double"
.Lgvan1:                .string          "s"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 2
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 2
                        call             gva_register@PLT
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 8
#=======================================================================================================================
#         DEFINE('double(s)')                                         :(double_end)
#         OUTPUT = double(5)
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_integer_α:
                        sub              rsp, 128
                        mov              qword ptr [rsp + 0], 0                         # stmt_claim
                        mov              qword ptr [rsp + 8], 0                         # stmt_claim
                        mov              qword ptr [rsp + 0], 0                         # stmt_claim
                        mov              qword ptr [rsp + 8], 0                         # stmt_claim
                        mov              qword ptr [rsp + 16], 0                        # stmt_claim
                        mov              qword ptr [rsp + 24], 0                        # stmt_claim
                        mov              qword ptr [rsp + 32], 0                        # stmt_claim
                        mov              qword ptr [rsp + 40], 0                        # stmt_claim
                        mov              qword ptr [rsp + 48], 0                        # stmt_claim
                        mov              qword ptr [rsp + 56], 0                        # stmt_claim
                        mov              qword ptr [rsp + 64], 0                        # stmt_claim
                        mov              qword ptr [rsp + 72], 0                        # stmt_claim
                        mov              qword ptr [rsp + 80], 0                        # stmt_claim
                        mov              qword ptr [rsp + 88], 0                        # stmt_claim
                        mov              qword ptr [rsp + 96], 0                        # stmt_claim
                        mov              qword ptr [rsp + 104], 0                       # stmt_claim
                        mov              qword ptr [rsp + 112], 3                       # lit_integer
                        mov              rax, qword ptr [rip + .Lx32_0]
                        mov              qword ptr [rsp + 120], rax                     # lit_integer
                                                                                        jmp   n20_call_α
.Lx32_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n20_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]                    # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]                    # s
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]                    # double
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]                    # double
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx34_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx34_5
                        mov              rax, qword ptr [rsp + 144]
                        mov              qword ptr [1879052304], rax                    # s
                        mov              rax, qword ptr [rsp + 152]
                        mov              qword ptr [1879052312], rax                    # s
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx34_6]
                        lea              rdx, [rip + .Lx34_7]
                                                                                        jmp   rax
.Lx34_6:
                        mov              rdi, qword ptr [1879052288]                    # double
                        mov              rsi, qword ptr [1879052296]                    # double
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax                    # double
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax                    # double
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax                    # s
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx34_2
.Lx34_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax                    # double
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax                    # double
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax                    # s
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx34_2
.Lx34_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx34_20
                        mov              rax, qword ptr [rsp + 112]
                        mov              rdx, qword ptr [rsp + 120]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx34_21
.Lx34_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 112]
                        mov              rdx, qword ptr [rsp + 120]
                        call             rt_arg_stage@PLT
.Lx34_21:
                        mov              rdi, qword ptr [rip + .Lx34_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx34_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx34_3]
                        lea              rdx, [rip + .Lx34_4]
                                                                                        jmp   rax
.Lx34_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx34_2
.Lx34_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx34_2
.Lx34_1:
                        call             rt_faildescr@PLT
.Lx34_2:
                        mov              qword ptr [rsp + 64], rax
                        mov              qword ptr [rsp + 72], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx34_240
                        add              rsp, 128
                                                                                        jmp   n22_lit_integer_α
.Lx34_240:
                                                                                        jmp   n21_assign_α
n20_call_β:
                                                                                        jmp   n22_lit_integer_α
.Lx34_0:
                        .quad            .Lx34_0_s
.Lx34_0_s:
                        .string          "double"
#-----------------------------------------------------------------------------------------------------------------------
n21_assign_α:
                        mov              rsi, qword ptr [rsp + 64]
                        mov              rdx, qword ptr [rsp + 72]
                        mov              rdi, qword ptr [rip + .Lx35_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 128
                                                                                        jmp   n22_lit_integer_α
.Lx35_0:
                        .quad            .Lx35_0_s
.Lx35_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = double(21)
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_integer_α:
                        sub              rsp, 192
                        mov              qword ptr [rsp + 0], 0                         # stmt_claim
                        mov              qword ptr [rsp + 8], 0                         # stmt_claim
                        mov              qword ptr [rsp + 0], 0                         # stmt_claim
                        mov              qword ptr [rsp + 8], 0                         # stmt_claim
                        mov              qword ptr [rsp + 16], 0                        # stmt_claim
                        mov              qword ptr [rsp + 24], 0                        # stmt_claim
                        mov              qword ptr [rsp + 32], 0                        # stmt_claim
                        mov              qword ptr [rsp + 40], 0                        # stmt_claim
                        mov              qword ptr [rsp + 48], 0                        # stmt_claim
                        mov              qword ptr [rsp + 56], 0                        # stmt_claim
                        mov              qword ptr [rsp + 64], 0                        # stmt_claim
                        mov              qword ptr [rsp + 72], 0                        # stmt_claim
                        mov              qword ptr [rsp + 80], 0                        # stmt_claim
                        mov              qword ptr [rsp + 88], 0                        # stmt_claim
                        mov              qword ptr [rsp + 96], 0                        # stmt_claim
                        mov              qword ptr [rsp + 104], 0                       # stmt_claim
                        mov              qword ptr [rsp + 112], 0                       # stmt_claim
                        mov              qword ptr [rsp + 120], 0                       # stmt_claim
                        mov              qword ptr [rsp + 128], 0                       # stmt_claim
                        mov              qword ptr [rsp + 136], 0                       # stmt_claim
                        mov              qword ptr [rsp + 144], 0                       # stmt_claim
                        mov              qword ptr [rsp + 152], 0                       # stmt_claim
                        mov              qword ptr [rsp + 160], 0                       # stmt_claim
                        mov              qword ptr [rsp + 168], 0                       # stmt_claim
                        mov              qword ptr [rsp + 176], 3                       # lit_integer
                        mov              rax, qword ptr [rip + .Lx36_0]
                        mov              qword ptr [rsp + 184], rax                     # lit_integer
                                                                                        jmp   n23_call_α
.Lx36_0:
                        .quad            21
#-----------------------------------------------------------------------------------------------------------------------
n23_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]                    # s
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]                    # s
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]                    # double
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]                    # double
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx38_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx38_5
                        mov              rax, qword ptr [rsp + 208]
                        mov              qword ptr [1879052304], rax                    # s
                        mov              rax, qword ptr [rsp + 216]
                        mov              qword ptr [1879052312], rax                    # s
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx38_6]
                        lea              rdx, [rip + .Lx38_7]
                                                                                        jmp   rax
.Lx38_6:
                        mov              rdi, qword ptr [1879052288]                    # double
                        mov              rsi, qword ptr [1879052296]                    # double
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax                    # double
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax                    # double
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax                    # s
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx38_2
.Lx38_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax                    # double
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax                    # double
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # s
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax                    # s
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx38_2
.Lx38_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx38_20
                        mov              rax, qword ptr [rsp + 176]
                        mov              rdx, qword ptr [rsp + 184]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx38_21
.Lx38_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 176]
                        mov              rdx, qword ptr [rsp + 184]
                        call             rt_arg_stage@PLT
.Lx38_21:
                        mov              rdi, qword ptr [rip + .Lx38_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx38_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx38_3]
                        lea              rdx, [rip + .Lx38_4]
                                                                                        jmp   rax
.Lx38_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx38_2
.Lx38_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx38_2
.Lx38_1:
                        call             rt_faildescr@PLT
.Lx38_2:
                        mov              qword ptr [rsp + 128], rax
                        mov              qword ptr [rsp + 136], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx38_240
                        add              rsp, 192
                                                                                        jmp   main_γ
.Lx38_240:
                                                                                        jmp   n24_assign_α
n23_call_β:
                                                                                        jmp   main_γ
.Lx38_0:
                        .quad            .Lx38_0_s
.Lx38_0_s:
                        .string          "double"
#-----------------------------------------------------------------------------------------------------------------------
n24_assign_α:
                        mov              rsi, qword ptr [rsp + 128]
                        mov              rdx, qword ptr [rsp + 136]
                        mov              rdi, qword ptr [rip + .Lx39_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 192
                                                                                        jmp   main_γ
.Lx39_0:
                        .quad            .Lx39_0_s
.Lx39_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n25_goto_α:
                                                                                        jmp   n26_lit_integer_α
n25_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# double  double = 2 * s                                             :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # lit_integer
                        mov              rax, qword ptr [rip + .Lx41_0]
                        mov              qword ptr [rsp + 8], rax                       # lit_integer
                                                                                        jmp   n27_var_α
.Lx41_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n27_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # s
                        mov              rdx, qword ptr [1879052312]                    # s
                        mov              qword ptr [rsp + 0], rax                       # s
                        mov              qword ptr [rsp + 8], rdx                       # s
                                                                                        jmp   n28_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n28_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 40]                      # lit_integer
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # var
                        call             rt_mul@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx43_240
                        add              rsp, 16
                                                                                        jmp   n30_save_restore_α
.Lx43_240:
                        mov              qword ptr [rsp + 0], rax                       # binop
                        mov              qword ptr [rsp + 8], rdx                       # binop
                                                                                        jmp   n29_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n29_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # binop
                        mov              qword ptr [1879052288], rax                    # double
                        mov              qword ptr [1879052296], rdx                    # double
                                                                                        jmp   n30_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n30_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n31_goto_α:
                                                                                        jmp   n19_lit_integer_α
n31_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              rsp, rbp
                        pop              rbp
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
                        .section         .note.GNU-stack,"",@progbits
