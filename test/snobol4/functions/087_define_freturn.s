                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ispos_α:
proc_LBL__ispos_α_body:
#=======================================================================================================================
# ispos   GT(x, 0)                                                   :S(RETURN)F(FRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_var_α
n0_statement_begin_β:
                                                                                        jmp   n8_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # x
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n2_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx12_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n3_coerce_numeric_α
n2_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n8_save_restore_α
.Lx12_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n3_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx14_1
                        cmp              eax, 3
                                                                                        jne   .Lx14_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3
                                                                                        jne   .Lx14_0
.Lx14_1:
                        mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n4_coerce_numeric_α
.Lx14_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 16]                                # lit_integer
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 111                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n4_coerce_numeric_α
n3_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n2_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n4_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5
                                                                                        je    .Lx16_1
                        cmp              eax, 3
                                                                                        jne   .Lx16_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx16_0
.Lx16_1:
                        mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n5_cmp_test_α
.Lx16_0:
                        lea              rdi, [rsp + 32]                                # lit_integer
                        lea              rsi, [rsp + 48]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 112                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n5_cmp_test_α
n4_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n3_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n5_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]                                # b
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jg    .Lx18_240
                        add              rsp, 16
                                                                                        jmp   n4_coerce_numeric_β
.Lx18_240:
                                                                                        jmp   n6_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:
                                                                                        jmp   n7_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n7_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n8_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ispos_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ispos_β:
                                                                                        jmp   proc_LBL__ispos_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ispos_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ispos_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_ispos_α
proc_ispos_α:
proc_ispos_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n25_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n26_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n26_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx30_0]                 # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx30_1
.Lx30_0:
                        .quad            .Lx30_0_s
.Lx30_0_s:
                        .string          "ispos"
.Lx30_1:
                                                                                        jmp   proc_ispos_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_ispos_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_ispos_β:
                                                                                        jmp   proc_ispos_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_ispos_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_ispos_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__ispos"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__ispos_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 320
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "ispos"
.Lstartup_pp1_0:        .string          "x"
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
                        lea              rsi, [rip + proc_ispos_α]
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
.Lgvan0:                .string          "ispos"
.Lgvan1:                .string          "x"
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
                        mov              r12, qword ptr [0x70000000]
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         DEFINE('ispos(x)')                                          :(ispos_end)
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_begin_α:
                                                                                        jmp   n32_statement_end_α
n31_statement_begin_β:
                                                                                        jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:
                                                                                        jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:
                                                                                        jmp   n34_statement_end_α
n33_statement_begin_β:
                                                                                        jmp   n35_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:
                                                                                        jmp   n35_statement_begin_α
#=======================================================================================================================
#         ispos(5)                                                    :S(A)F(B)
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:
                                                                                        jmp   n36_lit_integer_α
n35_statement_begin_β:
                                                                                        jmp   n45_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx90_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n37_call_α
.Lx90_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n37_call_α:
                        sub              rsp, 16
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rdx, qword ptr [rsp + 24]                      # v
                        call             rt_arg_stage@PLT
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]                    # x
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]                    # ispos
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx92_0]                 # name
                        mov              esi, 1                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx92_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # x
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx92_6]
                        lea              rdx, [rip + .Lx92_7]
                                                                                        jmp   rax
.Lx92_6:
                        mov              rdi, qword ptr [1879052288]                    # ispos
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx92_2
.Lx92_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax                    # ispos
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx92_2
.Lx92_5:
                        add              rsp, 32
.Lx92_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx92_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n45_statement_begin_α
.Lx92_240:
                                                                                        jmp   n38_statement_end_α
n37_call_β:
                                                                                        jmp   n45_statement_begin_α
.Lx92_0:
                        .quad            .Lx92_0_s
.Lx92_0_s:
                        .string          "ispos"
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n39_statement_begin_α
#=======================================================================================================================
# A       OUTPUT = 'positive'
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_begin_α:
                                                                                        jmp   n40_lit_string_α
n39_statement_begin_β:
                                                                                        jmp   n43_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n40_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 8
                        mov              rax, qword ptr [rip + .Lx97_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n41_assign_α
.Lx97_0:
                        .quad            .Lx97_0_s
.Lx97_0_s:
                        .string          "positive"
#-----------------------------------------------------------------------------------------------------------------------
n41_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx98_0]                 # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n42_statement_end_α
.Lx98_0:
                        .quad            .Lx98_0_s
.Lx98_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n43_statement_begin_α
#=======================================================================================================================
#         :(NEXT)
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_begin_α:
                                                                                        jmp   n44_statement_end_α
n43_statement_begin_β:
                                                                                        jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_end_α:
                                                                                        jmp   n49_statement_begin_α
#=======================================================================================================================
# B       OUTPUT = 'wrong'
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_begin_α:
                                                                                        jmp   n46_lit_string_α
n45_statement_begin_β:
                                                                                        jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n46_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx107_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n47_assign_α
.Lx107_0:
                        .quad            .Lx107_0_s
.Lx107_0_s:
                        .string          "wrong"
#-----------------------------------------------------------------------------------------------------------------------
n47_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx108_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n48_statement_end_α
.Lx108_0:
                        .quad            .Lx108_0_s
.Lx108_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n49_statement_begin_α
#=======================================================================================================================
# NEXT    ispos(-3)                                                   :S(C)F(D)
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_begin_α:
                                                                                        jmp   n50_lit_integer_α
n49_statement_begin_β:
                                                                                        jmp   n60_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx113_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n51_unop_α
.Lx113_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
n51_unop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]                      # lit_integer
                        mov              rsi, qword ptr [rsp + 24]                      # a
                        call             rt_num_neg@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n52_call_α
n51_unop_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n60_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n52_call_α:
                        sub              rsp, 16
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rsp + 16]                      # unop
                        mov              rdx, qword ptr [rsp + 24]                      # v
                        call             rt_arg_stage@PLT
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]                    # x
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]                    # ispos
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx116_0]                # name
                        mov              esi, 1                                         # np
                        mov              edx, 1                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx116_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # x
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx116_6]
                        lea              rdx, [rip + .Lx116_7]
                                                                                        jmp   rax
.Lx116_6:
                        mov              rdi, qword ptr [1879052288]                    # ispos
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx116_2
.Lx116_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax                    # ispos
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # x
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx116_2
.Lx116_5:
                        add              rsp, 32
.Lx116_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx116_240
                        add              rsp, 16
                                                                                        jmp   n51_unop_β
.Lx116_240:
                                                                                        jmp   n53_statement_end_α
n52_call_β:
                                                                                        jmp   n51_unop_β
.Lx116_0:
                        .quad            .Lx116_0_s
.Lx116_0_s:
                        .string          "ispos"
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n54_statement_begin_α
#=======================================================================================================================
# C       OUTPUT = 'wrong'
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_begin_α:
                                                                                        jmp   n55_lit_string_α
n54_statement_begin_β:
                                                                                        jmp   n58_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx121_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n56_assign_α
.Lx121_0:
                        .quad            .Lx121_0_s
.Lx121_0_s:
                        .string          "wrong"
#-----------------------------------------------------------------------------------------------------------------------
n56_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx122_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n57_statement_end_α
.Lx122_0:
                        .quad            .Lx122_0_s
.Lx122_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n58_statement_begin_α
#=======================================================================================================================
#         :(END)
#-----------------------------------------------------------------------------------------------------------------------
n58_statement_begin_α:
                                                                                        jmp   n59_statement_end_α
n58_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_end_α:
                                                                                        jmp   main_γ
#=======================================================================================================================
# D       OUTPUT = 'not positive'
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_begin_α:
                                                                                        jmp   n61_lit_string_α
n60_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 12
                        mov              rax, qword ptr [rip + .Lx131_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n62_assign_α
.Lx131_0:
                        .quad            .Lx131_0_s
.Lx131_0_s:
                        .string          "not positive"
#-----------------------------------------------------------------------------------------------------------------------
n62_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx132_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n63_statement_end_α
.Lx132_0:
                        .quad            .Lx132_0_s
.Lx132_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n63_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n64_goto_α:
                                                                                        jmp   n65_statement_begin_α
n64_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# ispos   GT(x, 0)                                                   :S(RETURN)F(FRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_begin_α:
                                                                                        jmp   n66_var_α
n65_statement_begin_β:
                                                                                        jmp   n73_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n66_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # x
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n67_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx139_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n68_coerce_numeric_α
n67_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n73_save_restore_α
.Lx139_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n68_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # var
                        cmp              eax, 5
                                                                                        je    .Lx141_1
                        cmp              eax, 3
                                                                                        jne   .Lx141_0
                        mov              eax, dword ptr [rsp + 16]                      # lit_integer
                        cmp              eax, 3
                                                                                        jne   .Lx141_0
.Lx141_1:
                        mov              rax, qword ptr [rsp + 32]                      # var
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # var
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n69_coerce_numeric_α
.Lx141_0:
                        lea              rdi, [rsp + 32]                                # var
                        lea              rsi, [rsp + 16]                                # lit_integer
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 111                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n69_coerce_numeric_α
n68_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n67_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n69_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]                      # lit_integer
                        cmp              eax, 5
                                                                                        je    .Lx143_1
                        cmp              eax, 3
                                                                                        jne   .Lx143_0
                        mov              eax, dword ptr [rsp + 48]                      # var
                        cmp              eax, 3
                                                                                        jne   .Lx143_0
.Lx143_1:
                        mov              rax, qword ptr [rsp + 32]                      # lit_integer
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              rax, qword ptr [rsp + 40]                      # lit_integer
                        mov              qword ptr [rsp + 8], rax                       # result
                                                                                        jmp   n70_cmp_test_α
.Lx143_0:
                        lea              rdi, [rsp + 32]                                # lit_integer
                        lea              rsi, [rsp + 48]                                # var
                        lea              rdx, [rsp + 0]                                 # result
                        mov              rcx, 112                                       # codes
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n70_cmp_test_α
n69_coerce_numeric_β:
                        add              rsp, 16
                                                                                        jmp   n68_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n70_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]                                # coerce_numeric
                        lea              rsi, [rsp + 16]                                # b
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jg    .Lx145_240
                        add              rsp, 16
                                                                                        jmp   n69_coerce_numeric_β
.Lx145_240:
                                                                                        jmp   n71_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_end_α:
                                                                                        jmp   n72_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n72_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n73_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n74_goto_α:
                                                                                        jmp   n33_statement_begin_α
n74_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n75_goto_α:
                                                                                        jmp   n39_statement_begin_α
n75_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n76_goto_α:
                                                                                        jmp   n45_statement_begin_α
n76_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n77_goto_α:
                                                                                        jmp   n49_statement_begin_α
n77_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n78_goto_α:
                                                                                        jmp   n54_statement_begin_α
n78_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n79_goto_α:
                                                                                        jmp   n60_statement_begin_α
n79_goto_β:
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
                        .section         .data
                        .align           8
fn_cell$ispos:
                        .quad            rt_ab_undef_fn_stub
                        .section         .text
                        .intel_syntax    noprefix
ispos_act_α:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 88
                        mov              rax, qword ptr [1879052288]                    # ispos
                        mov              qword ptr [rbp + -64], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rbp + -56], rax
                        mov              rax, qword ptr [1879052304]                    # x
                        mov              qword ptr [rbp + -80], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rbp + -72], rax
                        xor              eax, eax                                       # ispos
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rax
                        xor              eax, eax                                       # x
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rax
                        movabs           rcx, 140087139627744
                        mov              rax, qword ptr [rcx + 0]
                        add              rax, 1
                        mov              qword ptr [rcx + 0], rax
                        lea              rdi, [rip + .S1]
                        call             rt_bomb@PLT
                        ud2
                                                                                        jmp   ispos_act_γ
ispos_act_β:
                        lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
                                                                                        jmp   ispos_act_ω
.Lx158_0:
                        .quad            .Lx158_0_s
.Lx158_0_s:
                        .string          "ispos"
                        .section         .rodata
.S0:                    .string          "mX\310\025"
.S1:                    .string          "\215\\\001"
                        .text
                        .section         .note.GNU-stack,"",@progbits
