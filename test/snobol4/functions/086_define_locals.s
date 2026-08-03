                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__swap_α:
proc_LBL__swap_α_body:
#=======================================================================================================================
# swap    tmp = a
#-----------------------------------------------------------------------------------------------------------------------
n0_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # a
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n1_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n1_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052336], rax                    # tmp
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n2_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_α:
                        add              rsp, 16
                                                                                        jmp   n3_var_α
#=======================================================================================================================
#         a = b
#-----------------------------------------------------------------------------------------------------------------------
n3_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # b
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n4_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n4_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # a
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n5_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_α:
                        add              rsp, 16
                                                                                        jmp   n6_var_α
#=======================================================================================================================
#         b = tmp
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # tmp
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n7_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n7_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # b
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n8_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_α:
                        add              rsp, 16
                                                                                        jmp   n9_var_α
#=======================================================================================================================
#         OUTPUT = a ' ' b                                            :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n9_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # a
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n10_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n10_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx30_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n11_binop_α
.Lx30_0:
                        .quad            .Lx30_0_s
.Lx30_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n11_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n12_var_α
#-----------------------------------------------------------------------------------------------------------------------
n12_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # b
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n13_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n13_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n14_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx34_0]                 # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n15_statement_α
.Lx34_0:
                        .quad            .Lx34_0_s
.Lx34_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_α:
                                                                                        jmp   n16_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n16_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__swap_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__swap_β:
                                                                                        jmp   proc_LBL__swap_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__swap_γ:
                        mov              rsp, rbp
                        pop              rbp
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__swap_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_swap_α
proc_swap_α:
proc_swap_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n39_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n40_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n40_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx44_0]                 # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx44_1
.Lx44_0:
                        .quad            .Lx44_0_s
.Lx44_0_s:
                        .string          "swap"
.Lx44_1:
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
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_swap_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
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
                        mov              esi, 256
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "swap"
.Lstartup_pp1_0:        .string          "a"
.Lstartup_pp1_1:        .string          "b"
.Lstartup_pp1_2:        .string          "tmp"
                        .align           8
.Lstartup_pnames1:
                        .quad            .Lstartup_pp1_0
                        .quad            .Lstartup_pp1_1
                        .quad            .Lstartup_pp1_2
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + .Lstartup_pnames1]
                        mov              edx, 3
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_swap_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 3
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
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 8
#=======================================================================================================================
#         DEFINE('swap(a,b)tmp')                                      :(swap_end)
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_α:
                                                                                        jmp   n46_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_α:
                                                                                        jmp   n47_lit_string_α
#=======================================================================================================================
#         swap('hello', 'world')
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx74_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n48_lit_string_α
.Lx74_0:
                        .quad            .Lx74_0_s
.Lx74_0_s:
                        .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx75_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n49_call_α
.Lx75_0:
                        .quad            .Lx75_0_s
.Lx75_0_s:
                        .string          "world"
#-----------------------------------------------------------------------------------------------------------------------
n49_call_α:
                        sub              rsp, 16
                        mov              edi, 0                                         # idx
                        mov              rsi, qword ptr [rsp + 32]                      # lit_string
                        mov              rdx, qword ptr [rsp + 40]                      # v
                        call             rt_arg_stage@PLT
                        mov              edi, 1                                         # idx
                        mov              rsi, qword ptr [rsp + 16]                      # v
                        mov              rdx, qword ptr [rsp + 24]                      # v
                        call             rt_arg_stage@PLT
                        sub              rsp, 64
                        mov              rax, qword ptr [1879052304]                    # a
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]                    # b
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]                    # tmp
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052288]                    # swap
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 56], rax
                        mov              rdi, qword ptr [rip + .Lx77_0]                 # name
                        mov              esi, 3                                         # np
                        mov              edx, 2                                         # nargs
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx77_5
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 0]
                        mov              qword ptr [1879052304], rax                    # a
                        mov              rax, qword ptr [r10 + 8]
                        mov              qword ptr [1879052312], rax
                        lea              r10, [rip + g_call_args]
                        mov              rax, qword ptr [r10 + 16]
                        mov              qword ptr [1879052320], rax                    # b
                        mov              rax, qword ptr [r10 + 24]
                        mov              qword ptr [1879052328], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx77_6]
                        lea              rdx, [rip + .Lx77_7]
                                                                                        jmp   rax
.Lx77_6:
                        mov              rdi, qword ptr [1879052288]                    # swap
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax                    # tmp
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax                    # b
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # a
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 64
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx77_2
.Lx77_7:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052288], rax                    # swap
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax                    # tmp
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax                    # b
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax                    # a
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 64
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx77_2
.Lx77_5:
                        add              rsp, 64
.Lx77_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx77_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   main_γ
.Lx77_240:
                                                                                        jmp   n50_statement_α
n49_call_β:
                                                                                        jmp   main_γ
.Lx77_0:
                        .quad            .Lx77_0_s
.Lx77_0_s:
                        .string          "swap"
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_α:
                        add              rsp, 48
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n51_goto_α:
                                                                                        jmp   n52_var_α
n51_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# swap    tmp = a
#-----------------------------------------------------------------------------------------------------------------------
n52_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # a
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n53_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n53_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052336], rax                    # tmp
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n54_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_α:
                        add              rsp, 16
                                                                                        jmp   n55_var_α
#=======================================================================================================================
#         a = b
#-----------------------------------------------------------------------------------------------------------------------
n55_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # b
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n56_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n56_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # a
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n57_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n57_statement_α:
                        add              rsp, 16
                                                                                        jmp   n58_var_α
#=======================================================================================================================
#         b = tmp
#-----------------------------------------------------------------------------------------------------------------------
n58_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # tmp
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n59_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n59_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # b
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n60_statement_α
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_α:
                        add              rsp, 16
                                                                                        jmp   n61_var_α
#=======================================================================================================================
#         OUTPUT = a ' ' b                                            :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n61_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # a
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n62_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n62_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx94_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n63_binop_α
.Lx94_0:
                        .quad            .Lx94_0_s
.Lx94_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n63_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n64_var_α
#-----------------------------------------------------------------------------------------------------------------------
n64_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # b
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n65_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n65_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n66_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n66_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx98_0]                 # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n67_statement_α
.Lx98_0:
                        .quad            .Lx98_0_s
.Lx98_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_α:
                                                                                        jmp   n68_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n68_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n69_goto_α:
                                                                                        jmp   n46_statement_α
n69_goto_β:
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
