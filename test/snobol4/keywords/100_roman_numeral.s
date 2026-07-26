                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__roman_α
proc_LBL__roman_α:
                        .global          proc_LBL__roman_α
                        .global          proc_LBL__roman_β
                        .global          proc_LBL__roman_γ
                        .global          proc_LBL__roman_ω
                        sub              rsp, 3360
                        mov              [rsp + 3336], rcx
                        mov              [rsp + 3344], rdx
                        mov              [rsp + 3352], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_LBL__roman_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_goto_α:
                                                                                        jmp   n5_lit_string_α
n0_goto_β:
                                                                                        jmp   proc_LBL__roman_ω
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n6_var_α
n1_goto_β:
                                                                                        jmp   proc_LBL__roman_ω
#-----------------------------------------------------------------------------------------------------------------------
n2_goto_α:
                                                                                        jmp   n7_var_α
n2_goto_β:
                                                                                        jmp   proc_LBL__roman_ω
#-----------------------------------------------------------------------------------------------------------------------
n3_goto_α:
                                                                                        jmp   n8_var_α
n3_goto_β:
                                                                                        jmp   proc_LBL__roman_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_goto_α:
                                                                                        jmp   n9_lit_integer_α
n4_goto_β:
                                                                                        jmp   proc_LBL__roman_ω
#=======================================================================================================================
# roman   s = ''
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx202_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n10_assign_α
.Lx202_0:
                        .quad            .Lx202_0_s
.Lx202_0_s:
                        .string          ""
#=======================================================================================================================
# RLOOP   GT(n, 0)                                                       :F(RDONE)
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 2384], rax
                        mov              qword ptr [rbp + 2392], rdx
                                                                                        jmp   n11_lit_integer_α
#=======================================================================================================================
# RNEXT   i = i + 1                                                      :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n7_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n12_lit_integer_α
#=======================================================================================================================
# RDONE   roman = s                                                      :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n13_assign_α
#=======================================================================================================================
#         DEFINE('roman(n)s,v,r,i')                                     :(roman_end)
#         OUTPUT = roman(1)
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_integer_α:
                        mov              qword ptr [rbp + 2912], 6
                        mov              rax, qword ptr [rip + .Lx206_0]
                        mov              qword ptr [rbp + 2920], rax
                                                                                        jmp   n15_call_α
.Lx206_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n10_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        mov              qword ptr [rbp + 16], rax
                        mov              qword ptr [rbp + 24], rdx
                                                                                        jmp   n16_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:
                        mov              qword ptr [rbp + 2368], 6
                        mov              rax, qword ptr [rip + .Lx208_0]
                        mov              qword ptr [rbp + 2376], rax
                                                                                        jmp   n17_op75_α
.Lx208_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx209_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n18_binop_α
.Lx209_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n13_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 2816], rax
                        mov              qword ptr [rbp + 2824], rdx
                                                                                        jmp   n14_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n14_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n15_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx214_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx214_5
                        mov              rax, qword ptr [rbp + 2912]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 2920]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx214_6]
                        lea              rdx, [rip + .Lx214_7]
                                                                                        jmp   rax
.Lx214_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx214_2
.Lx214_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx214_2
.Lx214_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx214_20
                        mov              rax, qword ptr [rbp + 2912]
                        mov              rdx, qword ptr [rbp + 2920]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx214_21
.Lx214_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 2912]
                        mov              rdx, qword ptr [rbp + 2920]
                        call             rt_arg_stage@PLT
.Lx214_21:
                        mov              rdi, qword ptr [rip + .Lx214_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx214_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx214_3]
                        lea              rdx, [rip + .Lx214_4]
                                                                                        jmp   rax
.Lx214_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx214_2
.Lx214_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx214_2
.Lx214_1:
                        call             rt_faildescr@PLT
.Lx214_2:
                        mov              qword ptr [rbp + 2864], rax
                        mov              qword ptr [rbp + 2872], rdx
                        cmp              eax, 99
                                                                                        je    n20_lit_integer_α
                                                                                        jmp   n19_assign_α
n15_call_β:
                                                                                        jmp   n20_lit_integer_α
.Lx214_0:
                        .quad            .Lx214_0_s
.Lx214_0_s:
                        .string          "roman"
#=======================================================================================================================
#         v = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:
                        mov              qword ptr [rbp + 112], 6
                        mov              rax, qword ptr [rip + .Lx215_0]
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   n21_call_α
.Lx215_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n17_op75_α:
                        mov              eax, dword ptr [rbp + 2384]
                        cmp              eax, 7
                                                                                        je    .Lx217_1
                        cmp              eax, 6
                                                                                        jne   .Lx217_0
                        mov              eax, dword ptr [rbp + 2368]
                        cmp              eax, 6
                                                                                        jne   .Lx217_0
.Lx217_1:
                        mov              rax, qword ptr [rbp + 2384]
                        mov              qword ptr [rbp + 2352], rax
                        mov              rax, qword ptr [rbp + 2392]
                        mov              qword ptr [rbp + 2360], rax
                                                                                        jmp   n22_op75_α
.Lx217_0:
                        lea              rdi, [rbp + 2384]
                        lea              rsi, [rbp + 2368]
                        lea              rdx, [rbp + 2352]
                        mov              rcx, 111
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n22_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n18_binop_α:
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 100
                                                                                        je    .Lx218_0
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, 100
                                                                                        je    .Lx218_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx218_2
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, 6
                                                                                        jne   .Lx218_2
                        mov              rax, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rsp + 8]
                        add              rax, rcx
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n23_assign_α
.Lx218_0:
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, qword ptr [rsp + 8]
                        mov              r8d, 0
                        lea              r9, [rsp + 16]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx218_2
                        add              rsp, 16
                                                                                        jmp   n23_assign_α
.Lx218_2:
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, qword ptr [rsp + 8]
                        mov              r8d, 0
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx218_240
                        add              rsp, 32
                                                                                        jmp   n6_var_α
.Lx218_240:
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n23_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n19_assign_α:
                        mov              rsi, qword ptr [rbp + 2864]
                        mov              rdx, qword ptr [rbp + 2872]
                        mov              rdi, qword ptr [rip + .Lx219_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 2848], rax
                        mov              qword ptr [rbp + 2856], rdx
                                                                                        jmp   n20_lit_integer_α
.Lx219_0:
                        .quad            .Lx219_0_s
.Lx219_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(4)
#-----------------------------------------------------------------------------------------------------------------------
n20_lit_integer_α:
                        mov              qword ptr [rbp + 2992], 6
                        mov              rax, qword ptr [rip + .Lx220_0]
                        mov              qword ptr [rbp + 3000], rax
                                                                                        jmp   n24_call_α
.Lx220_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n21_call_α:
                        mov              rax, qword ptr [rbp + 112]
                        mov              qword ptr [rbp + 80], rax
                        mov              rax, qword ptr [rbp + 120]
                        mov              qword ptr [rbp + 88], rax
                        .section         .rodata
.Lrkfn222:              .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn222]
                        lea              rsi, [rbp + 80]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 64], rax
                        mov              qword ptr [rbp + 72], rdx
                        cmp              eax, 99
                                                                                        je    n26_var_α
                                                                                        jmp   n25_assign_α
n21_call_β:
                                                                                        jmp   n26_var_α
#-----------------------------------------------------------------------------------------------------------------------
n22_op75_α:
                        mov              eax, dword ptr [rbp + 2368]
                        cmp              eax, 7
                                                                                        je    .Lx224_1
                        cmp              eax, 6
                                                                                        jne   .Lx224_0
                        mov              eax, dword ptr [rbp + 2384]
                        cmp              eax, 6
                                                                                        jne   .Lx224_0
.Lx224_1:
                        mov              rax, qword ptr [rbp + 2368]
                        mov              qword ptr [rbp + 2336], rax
                        mov              rax, qword ptr [rbp + 2376]
                        mov              qword ptr [rbp + 2344], rax
                                                                                        jmp   n27_op77_α
.Lx224_0:
                        lea              rdi, [rbp + 2368]
                        lea              rsi, [rbp + 2384]
                        lea              rdx, [rbp + 2336]
                        mov              rcx, 112
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n27_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n23_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                        mov              qword ptr [rbp + 2752], rax
                        mov              qword ptr [rbp + 2760], rdx
                                                                                        jmp   n6_var_α
#-----------------------------------------------------------------------------------------------------------------------
n24_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx227_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx227_5
                        mov              rax, qword ptr [rbp + 2992]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 3000]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx227_6]
                        lea              rdx, [rip + .Lx227_7]
                                                                                        jmp   rax
.Lx227_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx227_2
.Lx227_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx227_2
.Lx227_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx227_20
                        mov              rax, qword ptr [rbp + 2992]
                        mov              rdx, qword ptr [rbp + 3000]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx227_21
.Lx227_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 2992]
                        mov              rdx, qword ptr [rbp + 3000]
                        call             rt_arg_stage@PLT
.Lx227_21:
                        mov              rdi, qword ptr [rip + .Lx227_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx227_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx227_3]
                        lea              rdx, [rip + .Lx227_4]
                                                                                        jmp   rax
.Lx227_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx227_2
.Lx227_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx227_2
.Lx227_1:
                        call             rt_faildescr@PLT
.Lx227_2:
                        mov              qword ptr [rbp + 2944], rax
                        mov              qword ptr [rbp + 2952], rdx
                        cmp              eax, 99
                                                                                        je    n29_lit_integer_α
                                                                                        jmp   n28_assign_α
n24_call_β:
                                                                                        jmp   n29_lit_integer_α
.Lx227_0:
                        .quad            .Lx227_0_s
.Lx227_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n25_assign_α:
                        mov              rax, qword ptr [rbp + 64]
                        mov              rdx, qword ptr [rbp + 72]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                        mov              qword ptr [rbp + 48], rax
                        mov              qword ptr [rbp + 56], rdx
                                                                                        jmp   n26_var_α
#=======================================================================================================================
#         v<1> = 1000
#-----------------------------------------------------------------------------------------------------------------------
n26_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 128], rax
                        mov              qword ptr [rbp + 136], rdx
                                                                                        jmp   n30_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n27_op77_α:
                        lea              rdi, [rbp + 2352]
                        lea              rsi, [rbp + 2336]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jle   n8_var_α
                        mov              qword ptr [rbp + 2320], 0
                        mov              qword ptr [rbp + 2328], 0
                                                                                        jmp   n32_var_α
#-----------------------------------------------------------------------------------------------------------------------
n28_assign_α:
                        mov              rsi, qword ptr [rbp + 2944]
                        mov              rdx, qword ptr [rbp + 2952]
                        mov              rdi, qword ptr [rip + .Lx232_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 2928], rax
                        mov              qword ptr [rbp + 2936], rdx
                                                                                        jmp   n29_lit_integer_α
.Lx232_0:
                        .quad            .Lx232_0_s
.Lx232_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(9)
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:
                        mov              qword ptr [rbp + 3072], 6
                        mov              rax, qword ptr [rip + .Lx233_0]
                        mov              qword ptr [rbp + 3080], rax
                                                                                        jmp   n33_call_α
.Lx233_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_integer_α:
                        mov              qword ptr [rbp + 144], 6
                        mov              rax, qword ptr [rip + .Lx234_0]
                        mov              qword ptr [rbp + 152], rax
                                                                                        jmp   n34_subscript_α
.Lx234_0:
                        .quad            1
#=======================================================================================================================
#         v<2> = 900
#-----------------------------------------------------------------------------------------------------------------------
n31_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 208], rax
                        mov              qword ptr [rbp + 216], rdx
                                                                                        jmp   n35_lit_integer_α
#=======================================================================================================================
#         GE(n, v<i>)                                                    :F(RNEXT)
#-----------------------------------------------------------------------------------------------------------------------
n32_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 2512], rax
                        mov              qword ptr [rbp + 2520], rdx
                                                                                        jmp   n37_var_α
#-----------------------------------------------------------------------------------------------------------------------
n33_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx238_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx238_5
                        mov              rax, qword ptr [rbp + 3072]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 3080]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx238_6]
                        lea              rdx, [rip + .Lx238_7]
                                                                                        jmp   rax
.Lx238_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx238_2
.Lx238_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx238_2
.Lx238_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx238_20
                        mov              rax, qword ptr [rbp + 3072]
                        mov              rdx, qword ptr [rbp + 3080]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx238_21
.Lx238_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 3072]
                        mov              rdx, qword ptr [rbp + 3080]
                        call             rt_arg_stage@PLT
.Lx238_21:
                        mov              rdi, qword ptr [rip + .Lx238_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx238_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx238_3]
                        lea              rdx, [rip + .Lx238_4]
                                                                                        jmp   rax
.Lx238_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx238_2
.Lx238_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx238_2
.Lx238_1:
                        call             rt_faildescr@PLT
.Lx238_2:
                        mov              qword ptr [rbp + 3024], rax
                        mov              qword ptr [rbp + 3032], rdx
                        cmp              eax, 99
                                                                                        je    n39_lit_integer_α
                                                                                        jmp   n38_assign_α
n33_call_β:
                                                                                        jmp   n39_lit_integer_α
.Lx238_0:
                        .quad            .Lx238_0_s
.Lx238_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n34_subscript_α:
                        mov              rdi, qword ptr [rbp + 128]
                        mov              rsi, qword ptr [rbp + 136]
                        mov              rdx, qword ptr [rbp + 144]
                        mov              rcx, qword ptr [rbp + 152]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n31_var_α
                        mov              qword ptr [rbp + 160], rax
                        mov              qword ptr [rbp + 168], rdx
                                                                                        jmp   n40_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_integer_α:
                        mov              qword ptr [rbp + 224], 6
                        mov              rax, qword ptr [rip + .Lx240_0]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n41_subscript_α
.Lx240_0:
                        .quad            2
#=======================================================================================================================
#         v<3> = 500
#-----------------------------------------------------------------------------------------------------------------------
n36_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 288], rax
                        mov              qword ptr [rbp + 296], rdx
                                                                                        jmp   n42_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n37_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 2448], rax
                        mov              qword ptr [rbp + 2456], rdx
                                                                                        jmp   n44_var_α
#-----------------------------------------------------------------------------------------------------------------------
n38_assign_α:
                        mov              rsi, qword ptr [rbp + 3024]
                        mov              rdx, qword ptr [rbp + 3032]
                        mov              rdi, qword ptr [rip + .Lx243_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 3008], rax
                        mov              qword ptr [rbp + 3016], rdx
                                                                                        jmp   n39_lit_integer_α
.Lx243_0:
                        .quad            .Lx243_0_s
.Lx243_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(42)
#-----------------------------------------------------------------------------------------------------------------------
n39_lit_integer_α:
                        mov              qword ptr [rbp + 3152], 6
                        mov              rax, qword ptr [rip + .Lx244_0]
                        mov              qword ptr [rbp + 3160], rax
                                                                                        jmp   n45_call_α
.Lx244_0:
                        .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n40_lit_integer_α:
                        mov              qword ptr [rbp + 176], 6
                        mov              rax, qword ptr [rip + .Lx245_0]
                        mov              qword ptr [rbp + 184], rax
                                                                                        jmp   n46_assign_var_α
.Lx245_0:
                        .quad            1000
#-----------------------------------------------------------------------------------------------------------------------
n41_subscript_α:
                        mov              rdi, qword ptr [rbp + 208]
                        mov              rsi, qword ptr [rbp + 216]
                        mov              rdx, qword ptr [rbp + 224]
                        mov              rcx, qword ptr [rbp + 232]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n36_var_α
                        mov              qword ptr [rbp + 240], rax
                        mov              qword ptr [rbp + 248], rdx
                                                                                        jmp   n47_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_integer_α:
                        mov              qword ptr [rbp + 304], 6
                        mov              rax, qword ptr [rip + .Lx247_0]
                        mov              qword ptr [rbp + 312], rax
                                                                                        jmp   n48_subscript_α
.Lx247_0:
                        .quad            3
#=======================================================================================================================
#         v<4> = 400
#-----------------------------------------------------------------------------------------------------------------------
n43_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 368], rax
                        mov              qword ptr [rbp + 376], rdx
                                                                                        jmp   n49_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n44_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 2464], rax
                        mov              qword ptr [rbp + 2472], rdx
                                                                                        jmp   n51_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n45_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx251_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx251_5
                        mov              rax, qword ptr [rbp + 3152]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 3160]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx251_6]
                        lea              rdx, [rip + .Lx251_7]
                                                                                        jmp   rax
.Lx251_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx251_2
.Lx251_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx251_2
.Lx251_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx251_20
                        mov              rax, qword ptr [rbp + 3152]
                        mov              rdx, qword ptr [rbp + 3160]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx251_21
.Lx251_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 3152]
                        mov              rdx, qword ptr [rbp + 3160]
                        call             rt_arg_stage@PLT
.Lx251_21:
                        mov              rdi, qword ptr [rip + .Lx251_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx251_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx251_3]
                        lea              rdx, [rip + .Lx251_4]
                                                                                        jmp   rax
.Lx251_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx251_2
.Lx251_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx251_2
.Lx251_1:
                        call             rt_faildescr@PLT
.Lx251_2:
                        mov              qword ptr [rbp + 3104], rax
                        mov              qword ptr [rbp + 3112], rdx
                        cmp              eax, 99
                                                                                        je    n53_lit_integer_α
                                                                                        jmp   n52_assign_α
n45_call_β:
                                                                                        jmp   n53_lit_integer_α
.Lx251_0:
                        .quad            .Lx251_0_s
.Lx251_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n46_assign_var_α:
                        mov              rdi, qword ptr [rbp + 160]
                        mov              rsi, qword ptr [rbp + 168]
                        mov              rdx, qword ptr [rbp + 176]
                        mov              rcx, qword ptr [rbp + 184]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n31_var_α
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                                                                                        jmp   n31_var_α
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_integer_α:
                        mov              qword ptr [rbp + 256], 6
                        mov              rax, qword ptr [rip + .Lx253_0]
                        mov              qword ptr [rbp + 264], rax
                                                                                        jmp   n54_assign_var_α
.Lx253_0:
                        .quad            900
#-----------------------------------------------------------------------------------------------------------------------
n48_subscript_α:
                        mov              rdi, qword ptr [rbp + 288]
                        mov              rsi, qword ptr [rbp + 296]
                        mov              rdx, qword ptr [rbp + 304]
                        mov              rcx, qword ptr [rbp + 312]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n43_var_α
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   n55_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_integer_α:
                        mov              qword ptr [rbp + 384], 6
                        mov              rax, qword ptr [rip + .Lx255_0]
                        mov              qword ptr [rbp + 392], rax
                                                                                        jmp   n56_subscript_α
.Lx255_0:
                        .quad            4
#=======================================================================================================================
#         v<5> = 100
#-----------------------------------------------------------------------------------------------------------------------
n50_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                                                                                        jmp   n57_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n51_subscript_α:
                        mov              rdi, qword ptr [rbp + 2448]
                        mov              rsi, qword ptr [rbp + 2456]
                        mov              rdx, qword ptr [rbp + 2464]
                        mov              rcx, qword ptr [rbp + 2472]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n7_var_α
                        mov              qword ptr [rbp + 2480], rax
                        mov              qword ptr [rbp + 2488], rdx
                                                                                        jmp   n59_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n52_assign_α:
                        mov              rsi, qword ptr [rbp + 3104]
                        mov              rdx, qword ptr [rbp + 3112]
                        mov              rdi, qword ptr [rip + .Lx258_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 3088], rax
                        mov              qword ptr [rbp + 3096], rdx
                                                                                        jmp   n53_lit_integer_α
.Lx258_0:
                        .quad            .Lx258_0_s
.Lx258_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(1999)
#-----------------------------------------------------------------------------------------------------------------------
n53_lit_integer_α:
                        mov              qword ptr [rbp + 3232], 6
                        mov              rax, qword ptr [rip + .Lx259_0]
                        mov              qword ptr [rbp + 3240], rax
                                                                                        jmp   n60_call_α
.Lx259_0:
                        .quad            1999
#-----------------------------------------------------------------------------------------------------------------------
n54_assign_var_α:
                        mov              rdi, qword ptr [rbp + 240]
                        mov              rsi, qword ptr [rbp + 248]
                        mov              rdx, qword ptr [rbp + 256]
                        mov              rcx, qword ptr [rbp + 264]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n36_var_α
                        mov              qword ptr [rbp + 272], rax
                        mov              qword ptr [rbp + 280], rdx
                                                                                        jmp   n36_var_α
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_integer_α:
                        mov              qword ptr [rbp + 336], 6
                        mov              rax, qword ptr [rip + .Lx261_0]
                        mov              qword ptr [rbp + 344], rax
                                                                                        jmp   n61_assign_var_α
.Lx261_0:
                        .quad            500
#-----------------------------------------------------------------------------------------------------------------------
n56_subscript_α:
                        mov              rdi, qword ptr [rbp + 368]
                        mov              rsi, qword ptr [rbp + 376]
                        mov              rdx, qword ptr [rbp + 384]
                        mov              rcx, qword ptr [rbp + 392]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n50_var_α
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                                                                                        jmp   n62_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_integer_α:
                        mov              qword ptr [rbp + 464], 6
                        mov              rax, qword ptr [rip + .Lx263_0]
                        mov              qword ptr [rbp + 472], rax
                                                                                        jmp   n63_subscript_α
.Lx263_0:
                        .quad            5
#=======================================================================================================================
#         v<6> = 90
#-----------------------------------------------------------------------------------------------------------------------
n58_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 528], rax
                        mov              qword ptr [rbp + 536], rdx
                                                                                        jmp   n64_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n59_deref_α:
                        mov              rdi, qword ptr [rbp + 2480]
                        mov              rsi, qword ptr [rbp + 2488]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n7_var_α
                        mov              qword ptr [rbp + 2496], rax
                        mov              qword ptr [rbp + 2504], rdx
                                                                                        jmp   n66_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n60_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx267_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx267_5
                        mov              rax, qword ptr [rbp + 3232]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 3240]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx267_6]
                        lea              rdx, [rip + .Lx267_7]
                                                                                        jmp   rax
.Lx267_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx267_2
.Lx267_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx267_2
.Lx267_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx267_20
                        mov              rax, qword ptr [rbp + 3232]
                        mov              rdx, qword ptr [rbp + 3240]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx267_21
.Lx267_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 3232]
                        mov              rdx, qword ptr [rbp + 3240]
                        call             rt_arg_stage@PLT
.Lx267_21:
                        mov              rdi, qword ptr [rip + .Lx267_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx267_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx267_3]
                        lea              rdx, [rip + .Lx267_4]
                                                                                        jmp   rax
.Lx267_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx267_2
.Lx267_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx267_2
.Lx267_1:
                        call             rt_faildescr@PLT
.Lx267_2:
                        mov              qword ptr [rbp + 3184], rax
                        mov              qword ptr [rbp + 3192], rdx
                        cmp              eax, 99
                                                                                        je    n68_lit_integer_α
                                                                                        jmp   n67_assign_α
n60_call_β:
                                                                                        jmp   n68_lit_integer_α
.Lx267_0:
                        .quad            .Lx267_0_s
.Lx267_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n61_assign_var_α:
                        mov              rdi, qword ptr [rbp + 320]
                        mov              rsi, qword ptr [rbp + 328]
                        mov              rdx, qword ptr [rbp + 336]
                        mov              rcx, qword ptr [rbp + 344]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n43_var_α
                        mov              qword ptr [rbp + 352], rax
                        mov              qword ptr [rbp + 360], rdx
                                                                                        jmp   n43_var_α
#-----------------------------------------------------------------------------------------------------------------------
n62_lit_integer_α:
                        mov              qword ptr [rbp + 416], 6
                        mov              rax, qword ptr [rip + .Lx269_0]
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   n69_assign_var_α
.Lx269_0:
                        .quad            400
#-----------------------------------------------------------------------------------------------------------------------
n63_subscript_α:
                        mov              rdi, qword ptr [rbp + 448]
                        mov              rsi, qword ptr [rbp + 456]
                        mov              rdx, qword ptr [rbp + 464]
                        mov              rcx, qword ptr [rbp + 472]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n58_var_α
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                                                                                        jmp   n70_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_integer_α:
                        mov              qword ptr [rbp + 544], 6
                        mov              rax, qword ptr [rip + .Lx271_0]
                        mov              qword ptr [rbp + 552], rax
                                                                                        jmp   n71_subscript_α
.Lx271_0:
                        .quad            6
#=======================================================================================================================
#         v<7> = 50
#-----------------------------------------------------------------------------------------------------------------------
n65_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 608], rax
                        mov              qword ptr [rbp + 616], rdx
                                                                                        jmp   n72_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n66_op75_α:
                        mov              eax, dword ptr [rbp + 2512]
                        cmp              eax, 7
                                                                                        je    .Lx274_1
                        cmp              eax, 6
                                                                                        jne   .Lx274_0
                        mov              eax, dword ptr [rbp + 2496]
                        cmp              eax, 6
                                                                                        jne   .Lx274_0
.Lx274_1:
                        mov              rax, qword ptr [rbp + 2512]
                        mov              qword ptr [rbp + 2432], rax
                        mov              rax, qword ptr [rbp + 2520]
                        mov              qword ptr [rbp + 2440], rax
                                                                                        jmp   n74_op75_α
.Lx274_0:
                        lea              rdi, [rbp + 2512]
                        lea              rsi, [rbp + 2496]
                        lea              rdx, [rbp + 2432]
                        mov              rcx, 109
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n74_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n67_assign_α:
                        mov              rsi, qword ptr [rbp + 3184]
                        mov              rdx, qword ptr [rbp + 3192]
                        mov              rdi, qword ptr [rip + .Lx275_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 3168], rax
                        mov              qword ptr [rbp + 3176], rdx
                                                                                        jmp   n68_lit_integer_α
.Lx275_0:
                        .quad            .Lx275_0_s
.Lx275_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(2024)
#-----------------------------------------------------------------------------------------------------------------------
n68_lit_integer_α:
                        mov              qword ptr [rbp + 3312], 6
                        mov              rax, qword ptr [rip + .Lx276_0]
                        mov              qword ptr [rbp + 3320], rax
                                                                                        jmp   n75_call_α
.Lx276_0:
                        .quad            2024
#-----------------------------------------------------------------------------------------------------------------------
n69_assign_var_α:
                        mov              rdi, qword ptr [rbp + 400]
                        mov              rsi, qword ptr [rbp + 408]
                        mov              rdx, qword ptr [rbp + 416]
                        mov              rcx, qword ptr [rbp + 424]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n50_var_α
                        mov              qword ptr [rbp + 432], rax
                        mov              qword ptr [rbp + 440], rdx
                                                                                        jmp   n50_var_α
#-----------------------------------------------------------------------------------------------------------------------
n70_lit_integer_α:
                        mov              qword ptr [rbp + 496], 6
                        mov              rax, qword ptr [rip + .Lx278_0]
                        mov              qword ptr [rbp + 504], rax
                                                                                        jmp   n76_assign_var_α
.Lx278_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n71_subscript_α:
                        mov              rdi, qword ptr [rbp + 528]
                        mov              rsi, qword ptr [rbp + 536]
                        mov              rdx, qword ptr [rbp + 544]
                        mov              rcx, qword ptr [rbp + 552]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n65_var_α
                        mov              qword ptr [rbp + 560], rax
                        mov              qword ptr [rbp + 568], rdx
                                                                                        jmp   n77_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n72_lit_integer_α:
                        mov              qword ptr [rbp + 624], 6
                        mov              rax, qword ptr [rip + .Lx280_0]
                        mov              qword ptr [rbp + 632], rax
                                                                                        jmp   n78_subscript_α
.Lx280_0:
                        .quad            7
#=======================================================================================================================
#         v<8> = 40
#-----------------------------------------------------------------------------------------------------------------------
n73_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 688], rax
                        mov              qword ptr [rbp + 696], rdx
                                                                                        jmp   n79_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n74_op75_α:
                        mov              eax, dword ptr [rbp + 2496]
                        cmp              eax, 7
                                                                                        je    .Lx283_1
                        cmp              eax, 6
                                                                                        jne   .Lx283_0
                        mov              eax, dword ptr [rbp + 2512]
                        cmp              eax, 6
                                                                                        jne   .Lx283_0
.Lx283_1:
                        mov              rax, qword ptr [rbp + 2496]
                        mov              qword ptr [rbp + 2416], rax
                        mov              rax, qword ptr [rbp + 2504]
                        mov              qword ptr [rbp + 2424], rax
                                                                                        jmp   n81_op77_α
.Lx283_0:
                        lea              rdi, [rbp + 2496]
                        lea              rsi, [rbp + 2512]
                        lea              rdx, [rbp + 2416]
                        mov              rcx, 110
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n81_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n75_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx285_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx285_5
                        mov              rax, qword ptr [rbp + 3312]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 3320]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx285_6]
                        lea              rdx, [rip + .Lx285_7]
                                                                                        jmp   rax
.Lx285_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx285_2
.Lx285_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx285_2
.Lx285_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx285_20
                        mov              rax, qword ptr [rbp + 3312]
                        mov              rdx, qword ptr [rbp + 3320]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx285_21
.Lx285_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 3312]
                        mov              rdx, qword ptr [rbp + 3320]
                        call             rt_arg_stage@PLT
.Lx285_21:
                        mov              rdi, qword ptr [rip + .Lx285_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx285_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx285_3]
                        lea              rdx, [rip + .Lx285_4]
                                                                                        jmp   rax
.Lx285_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx285_2
.Lx285_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx285_2
.Lx285_1:
                        call             rt_faildescr@PLT
.Lx285_2:
                        mov              qword ptr [rbp + 3264], rax
                        mov              qword ptr [rbp + 3272], rdx
                        cmp              eax, 99
                                                                                        je    proc_LBL__roman_γ
                                                                                        jmp   n82_assign_α
n75_call_β:
                                                                                        jmp   proc_LBL__roman_γ
.Lx285_0:
                        .quad            .Lx285_0_s
.Lx285_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n76_assign_var_α:
                        mov              rdi, qword ptr [rbp + 480]
                        mov              rsi, qword ptr [rbp + 488]
                        mov              rdx, qword ptr [rbp + 496]
                        mov              rcx, qword ptr [rbp + 504]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n58_var_α
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   n58_var_α
#-----------------------------------------------------------------------------------------------------------------------
n77_lit_integer_α:
                        mov              qword ptr [rbp + 576], 6
                        mov              rax, qword ptr [rip + .Lx287_0]
                        mov              qword ptr [rbp + 584], rax
                                                                                        jmp   n83_assign_var_α
.Lx287_0:
                        .quad            90
#-----------------------------------------------------------------------------------------------------------------------
n78_subscript_α:
                        mov              rdi, qword ptr [rbp + 608]
                        mov              rsi, qword ptr [rbp + 616]
                        mov              rdx, qword ptr [rbp + 624]
                        mov              rcx, qword ptr [rbp + 632]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n73_var_α
                        mov              qword ptr [rbp + 640], rax
                        mov              qword ptr [rbp + 648], rdx
                                                                                        jmp   n84_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n79_lit_integer_α:
                        mov              qword ptr [rbp + 704], 6
                        mov              rax, qword ptr [rip + .Lx289_0]
                        mov              qword ptr [rbp + 712], rax
                                                                                        jmp   n85_subscript_α
.Lx289_0:
                        .quad            8
#=======================================================================================================================
#         v<9> = 10
#-----------------------------------------------------------------------------------------------------------------------
n80_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 768], rax
                        mov              qword ptr [rbp + 776], rdx
                                                                                        jmp   n86_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n81_op77_α:
                        lea              rdi, [rbp + 2432]
                        lea              rsi, [rbp + 2416]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        js    n7_var_α
                        mov              qword ptr [rbp + 2400], 0
                        mov              qword ptr [rbp + 2408], 0
                                                                                        jmp   n88_var_α
#-----------------------------------------------------------------------------------------------------------------------
n82_assign_α:
                        mov              rsi, qword ptr [rbp + 3264]
                        mov              rdx, qword ptr [rbp + 3272]
                        mov              rdi, qword ptr [rip + .Lx293_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 3248], rax
                        mov              qword ptr [rbp + 3256], rdx
                                                                                        jmp   proc_LBL__roman_γ
.Lx293_0:
                        .quad            .Lx293_0_s
.Lx293_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n83_assign_var_α:
                        mov              rdi, qword ptr [rbp + 560]
                        mov              rsi, qword ptr [rbp + 568]
                        mov              rdx, qword ptr [rbp + 576]
                        mov              rcx, qword ptr [rbp + 584]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n65_var_α
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                                                                                        jmp   n65_var_α
#-----------------------------------------------------------------------------------------------------------------------
n84_lit_integer_α:
                        mov              qword ptr [rbp + 656], 6
                        mov              rax, qword ptr [rip + .Lx295_0]
                        mov              qword ptr [rbp + 664], rax
                                                                                        jmp   n89_assign_var_α
.Lx295_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n85_subscript_α:
                        mov              rdi, qword ptr [rbp + 688]
                        mov              rsi, qword ptr [rbp + 696]
                        mov              rdx, qword ptr [rbp + 704]
                        mov              rcx, qword ptr [rbp + 712]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n80_var_α
                        mov              qword ptr [rbp + 720], rax
                        mov              qword ptr [rbp + 728], rdx
                                                                                        jmp   n90_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n86_lit_integer_α:
                        mov              qword ptr [rbp + 784], 6
                        mov              rax, qword ptr [rip + .Lx297_0]
                        mov              qword ptr [rbp + 792], rax
                                                                                        jmp   n91_subscript_α
.Lx297_0:
                        .quad            9
#=======================================================================================================================
#         v<10> = 9
#-----------------------------------------------------------------------------------------------------------------------
n87_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 848], rax
                        mov              qword ptr [rbp + 856], rdx
                                                                                        jmp   n92_lit_integer_α
#=======================================================================================================================
#         s = s r<i>
#-----------------------------------------------------------------------------------------------------------------------
n88_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rbp + 2560], rax
                        mov              qword ptr [rbp + 2568], rdx
                                                                                        jmp   n94_var_α
#-----------------------------------------------------------------------------------------------------------------------
n89_assign_var_α:
                        mov              rdi, qword ptr [rbp + 640]
                        mov              rsi, qword ptr [rbp + 648]
                        mov              rdx, qword ptr [rbp + 656]
                        mov              rcx, qword ptr [rbp + 664]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n73_var_α
                        mov              qword ptr [rbp + 672], rax
                        mov              qword ptr [rbp + 680], rdx
                                                                                        jmp   n73_var_α
#-----------------------------------------------------------------------------------------------------------------------
n90_lit_integer_α:
                        mov              qword ptr [rbp + 736], 6
                        mov              rax, qword ptr [rip + .Lx301_0]
                        mov              qword ptr [rbp + 744], rax
                                                                                        jmp   n96_assign_var_α
.Lx301_0:
                        .quad            40
#-----------------------------------------------------------------------------------------------------------------------
n91_subscript_α:
                        mov              rdi, qword ptr [rbp + 768]
                        mov              rsi, qword ptr [rbp + 776]
                        mov              rdx, qword ptr [rbp + 784]
                        mov              rcx, qword ptr [rbp + 792]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n87_var_α
                        mov              qword ptr [rbp + 800], rax
                        mov              qword ptr [rbp + 808], rdx
                                                                                        jmp   n97_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n92_lit_integer_α:
                        mov              qword ptr [rbp + 864], 6
                        mov              rax, qword ptr [rip + .Lx303_0]
                        mov              qword ptr [rbp + 872], rax
                                                                                        jmp   n98_subscript_α
.Lx303_0:
                        .quad            10
#=======================================================================================================================
#         v<11> = 5
#-----------------------------------------------------------------------------------------------------------------------
n93_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 928], rax
                        mov              qword ptr [rbp + 936], rdx
                                                                                        jmp   n99_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n94_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 2576], rax
                        mov              qword ptr [rbp + 2584], rdx
                                                                                        jmp   n101_var_α
#=======================================================================================================================
#         n = n - v<i>                                                   :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n95_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 2672], rax
                        mov              qword ptr [rbp + 2680], rdx
                                                                                        jmp   n102_var_α
#-----------------------------------------------------------------------------------------------------------------------
n96_assign_var_α:
                        mov              rdi, qword ptr [rbp + 720]
                        mov              rsi, qword ptr [rbp + 728]
                        mov              rdx, qword ptr [rbp + 736]
                        mov              rcx, qword ptr [rbp + 744]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n80_var_α
                        mov              qword ptr [rbp + 752], rax
                        mov              qword ptr [rbp + 760], rdx
                                                                                        jmp   n80_var_α
#-----------------------------------------------------------------------------------------------------------------------
n97_lit_integer_α:
                        mov              qword ptr [rbp + 816], 6
                        mov              rax, qword ptr [rip + .Lx308_0]
                        mov              qword ptr [rbp + 824], rax
                                                                                        jmp   n103_assign_var_α
.Lx308_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n98_subscript_α:
                        mov              rdi, qword ptr [rbp + 848]
                        mov              rsi, qword ptr [rbp + 856]
                        mov              rdx, qword ptr [rbp + 864]
                        mov              rcx, qword ptr [rbp + 872]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n93_var_α
                        mov              qword ptr [rbp + 880], rax
                        mov              qword ptr [rbp + 888], rdx
                                                                                        jmp   n104_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n99_lit_integer_α:
                        mov              qword ptr [rbp + 944], 6
                        mov              rax, qword ptr [rip + .Lx310_0]
                        mov              qword ptr [rbp + 952], rax
                                                                                        jmp   n105_subscript_α
.Lx310_0:
                        .quad            11
#=======================================================================================================================
#         v<12> = 4
#-----------------------------------------------------------------------------------------------------------------------
n100_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 1008], rax
                        mov              qword ptr [rbp + 1016], rdx
                                                                                        jmp   n106_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n101_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 2592], rax
                        mov              qword ptr [rbp + 2600], rdx
                                                                                        jmp   n108_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n102_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 2688], rax
                        mov              qword ptr [rbp + 2696], rdx
                                                                                        jmp   n109_var_α
#-----------------------------------------------------------------------------------------------------------------------
n103_assign_var_α:
                        mov              rdi, qword ptr [rbp + 800]
                        mov              rsi, qword ptr [rbp + 808]
                        mov              rdx, qword ptr [rbp + 816]
                        mov              rcx, qword ptr [rbp + 824]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n87_var_α
                        mov              qword ptr [rbp + 832], rax
                        mov              qword ptr [rbp + 840], rdx
                                                                                        jmp   n87_var_α
#-----------------------------------------------------------------------------------------------------------------------
n104_lit_integer_α:
                        mov              qword ptr [rbp + 896], 6
                        mov              rax, qword ptr [rip + .Lx315_0]
                        mov              qword ptr [rbp + 904], rax
                                                                                        jmp   n110_assign_var_α
.Lx315_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n105_subscript_α:
                        mov              rdi, qword ptr [rbp + 928]
                        mov              rsi, qword ptr [rbp + 936]
                        mov              rdx, qword ptr [rbp + 944]
                        mov              rcx, qword ptr [rbp + 952]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n100_var_α
                        mov              qword ptr [rbp + 960], rax
                        mov              qword ptr [rbp + 968], rdx
                                                                                        jmp   n111_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n106_lit_integer_α:
                        mov              qword ptr [rbp + 1024], 6
                        mov              rax, qword ptr [rip + .Lx317_0]
                        mov              qword ptr [rbp + 1032], rax
                                                                                        jmp   n112_subscript_α
.Lx317_0:
                        .quad            12
#=======================================================================================================================
#         v<13> = 1
#-----------------------------------------------------------------------------------------------------------------------
n107_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 1088], rax
                        mov              qword ptr [rbp + 1096], rdx
                                                                                        jmp   n113_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n108_subscript_α:
                        mov              rdi, qword ptr [rbp + 2576]
                        mov              rsi, qword ptr [rbp + 2584]
                        mov              rdx, qword ptr [rbp + 2592]
                        mov              rcx, qword ptr [rbp + 2600]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n95_var_α
                        mov              qword ptr [rbp + 2608], rax
                        mov              qword ptr [rbp + 2616], rdx
                                                                                        jmp   n115_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n109_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 2704], rax
                        mov              qword ptr [rbp + 2712], rdx
                                                                                        jmp   n116_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n110_assign_var_α:
                        mov              rdi, qword ptr [rbp + 880]
                        mov              rsi, qword ptr [rbp + 888]
                        mov              rdx, qword ptr [rbp + 896]
                        mov              rcx, qword ptr [rbp + 904]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n93_var_α
                        mov              qword ptr [rbp + 912], rax
                        mov              qword ptr [rbp + 920], rdx
                                                                                        jmp   n93_var_α
#-----------------------------------------------------------------------------------------------------------------------
n111_lit_integer_α:
                        mov              qword ptr [rbp + 976], 6
                        mov              rax, qword ptr [rip + .Lx322_0]
                        mov              qword ptr [rbp + 984], rax
                                                                                        jmp   n117_assign_var_α
.Lx322_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n112_subscript_α:
                        mov              rdi, qword ptr [rbp + 1008]
                        mov              rsi, qword ptr [rbp + 1016]
                        mov              rdx, qword ptr [rbp + 1024]
                        mov              rcx, qword ptr [rbp + 1032]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n107_var_α
                        mov              qword ptr [rbp + 1040], rax
                        mov              qword ptr [rbp + 1048], rdx
                                                                                        jmp   n118_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n113_lit_integer_α:
                        mov              qword ptr [rbp + 1104], 6
                        mov              rax, qword ptr [rip + .Lx324_0]
                        mov              qword ptr [rbp + 1112], rax
                                                                                        jmp   n119_subscript_α
.Lx324_0:
                        .quad            13
#=======================================================================================================================
#         r = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n114_lit_integer_α:
                        mov              qword ptr [rbp + 1232], 6
                        mov              rax, qword ptr [rip + .Lx325_0]
                        mov              qword ptr [rbp + 1240], rax
                                                                                        jmp   n120_call_α
.Lx325_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n115_deref_α:
                        mov              rdi, qword ptr [rbp + 2608]
                        mov              rsi, qword ptr [rbp + 2616]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n95_var_α
                        mov              qword ptr [rbp + 2624], rax
                        mov              qword ptr [rbp + 2632], rdx
                                                                                        jmp   n121_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n116_subscript_α:
                        mov              rdi, qword ptr [rbp + 2688]
                        mov              rsi, qword ptr [rbp + 2696]
                        mov              rdx, qword ptr [rbp + 2704]
                        mov              rcx, qword ptr [rbp + 2712]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n6_var_α
                        mov              qword ptr [rbp + 2720], rax
                        mov              qword ptr [rbp + 2728], rdx
                                                                                        jmp   n122_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n117_assign_var_α:
                        mov              rdi, qword ptr [rbp + 960]
                        mov              rsi, qword ptr [rbp + 968]
                        mov              rdx, qword ptr [rbp + 976]
                        mov              rcx, qword ptr [rbp + 984]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n100_var_α
                        mov              qword ptr [rbp + 992], rax
                        mov              qword ptr [rbp + 1000], rdx
                                                                                        jmp   n100_var_α
#-----------------------------------------------------------------------------------------------------------------------
n118_lit_integer_α:
                        mov              qword ptr [rbp + 1056], 6
                        mov              rax, qword ptr [rip + .Lx329_0]
                        mov              qword ptr [rbp + 1064], rax
                                                                                        jmp   n123_assign_var_α
.Lx329_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n119_subscript_α:
                        mov              rdi, qword ptr [rbp + 1088]
                        mov              rsi, qword ptr [rbp + 1096]
                        mov              rdx, qword ptr [rbp + 1104]
                        mov              rcx, qword ptr [rbp + 1112]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n114_lit_integer_α
                        mov              qword ptr [rbp + 1120], rax
                        mov              qword ptr [rbp + 1128], rdx
                                                                                        jmp   n124_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n120_call_α:
                        mov              rax, qword ptr [rbp + 1232]
                        mov              qword ptr [rbp + 1200], rax
                        mov              rax, qword ptr [rbp + 1240]
                        mov              qword ptr [rbp + 1208], rax
                        .section         .rodata
.Lrkfn332:              .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn332]
                        lea              rsi, [rbp + 1200]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1184], rax
                        mov              qword ptr [rbp + 1192], rdx
                        cmp              eax, 99
                                                                                        je    n126_var_α
                                                                                        jmp   n125_assign_α
n120_call_β:
                                                                                        jmp   n126_var_α
#-----------------------------------------------------------------------------------------------------------------------
n121_binop_α:
                        mov              rdi, qword ptr [rbp + 2560]
                        mov              rsi, qword ptr [rbp + 2568]
                        mov              rdx, qword ptr [rbp + 2624]
                        mov              rcx, qword ptr [rbp + 2632]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 2544], rax
                        mov              qword ptr [rbp + 2552], rdx
                                                                                        jmp   n127_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n122_deref_α:
                        mov              rdi, qword ptr [rbp + 2720]
                        mov              rsi, qword ptr [rbp + 2728]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n6_var_α
                        mov              qword ptr [rbp + 2736], rax
                        mov              qword ptr [rbp + 2744], rdx
                                                                                        jmp   n128_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n123_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1040]
                        mov              rsi, qword ptr [rbp + 1048]
                        mov              rdx, qword ptr [rbp + 1056]
                        mov              rcx, qword ptr [rbp + 1064]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n107_var_α
                        mov              qword ptr [rbp + 1072], rax
                        mov              qword ptr [rbp + 1080], rdx
                                                                                        jmp   n107_var_α
#-----------------------------------------------------------------------------------------------------------------------
n124_lit_integer_α:
                        mov              qword ptr [rbp + 1136], 6
                        mov              rax, qword ptr [rip + .Lx336_0]
                        mov              qword ptr [rbp + 1144], rax
                                                                                        jmp   n129_assign_var_α
.Lx336_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n125_assign_α:
                        mov              rax, qword ptr [rbp + 1184]
                        mov              rdx, qword ptr [rbp + 1192]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                        mov              qword ptr [rbp + 1168], rax
                        mov              qword ptr [rbp + 1176], rdx
                                                                                        jmp   n126_var_α
#=======================================================================================================================
#         r<1> = 'M'
#-----------------------------------------------------------------------------------------------------------------------
n126_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1248], rax
                        mov              qword ptr [rbp + 1256], rdx
                                                                                        jmp   n130_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n127_assign_α:
                        mov              rax, qword ptr [rbp + 2544]
                        mov              rdx, qword ptr [rbp + 2552]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        mov              qword ptr [rbp + 2528], rax
                        mov              qword ptr [rbp + 2536], rdx
                                                                                        jmp   n95_var_α
#-----------------------------------------------------------------------------------------------------------------------
n128_binop_α:
                        mov              eax, dword ptr [rbp + 2672]
                        cmp              eax, 100
                                                                                        je    .Lx340_0
                        mov              eax, dword ptr [rbp + 2736]
                        cmp              eax, 100
                                                                                        je    .Lx340_0
                        mov              eax, dword ptr [rbp + 2672]
                        cmp              eax, 6
                                                                                        jne   .Lx340_2
                        mov              eax, dword ptr [rbp + 2736]
                        cmp              eax, 6
                                                                                        jne   .Lx340_2
.Lx340_1:
                        mov              rax, qword ptr [rbp + 2680]
                        mov              rcx, qword ptr [rbp + 2744]
                        sub              rax, rcx
                        mov              qword ptr [rbp + 2656], 6
                        mov              qword ptr [rbp + 2664], rax
                                                                                        jmp   n132_assign_α
.Lx340_0:
                        mov              rdi, qword ptr [rbp + 2672]
                        mov              rsi, qword ptr [rbp + 2680]
                        mov              rdx, qword ptr [rbp + 2736]
                        mov              rcx, qword ptr [rbp + 2744]
                        mov              r8d, 1
                        lea              r9, [rbp + 2656]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n132_assign_α
.Lx340_2:
                        mov              rdi, qword ptr [rbp + 2672]
                        mov              rsi, qword ptr [rbp + 2680]
                        mov              rdx, qword ptr [rbp + 2736]
                        mov              rcx, qword ptr [rbp + 2744]
                        mov              r8d, 1
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n6_var_α
                        mov              qword ptr [rbp + 2656], rax
                        mov              qword ptr [rbp + 2664], rdx
                                                                                        jmp   n132_assign_α
n128_binop_β:
                                                                                        jmp   n6_var_α
#-----------------------------------------------------------------------------------------------------------------------
n129_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1120]
                        mov              rsi, qword ptr [rbp + 1128]
                        mov              rdx, qword ptr [rbp + 1136]
                        mov              rcx, qword ptr [rbp + 1144]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n114_lit_integer_α
                        mov              qword ptr [rbp + 1152], rax
                        mov              qword ptr [rbp + 1160], rdx
                                                                                        jmp   n114_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n130_lit_integer_α:
                        mov              qword ptr [rbp + 1264], 6
                        mov              rax, qword ptr [rip + .Lx342_0]
                        mov              qword ptr [rbp + 1272], rax
                                                                                        jmp   n133_subscript_α
.Lx342_0:
                        .quad            1
#=======================================================================================================================
#         r<2> = 'CM'
#-----------------------------------------------------------------------------------------------------------------------
n131_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1328], rax
                        mov              qword ptr [rbp + 1336], rdx
                                                                                        jmp   n134_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n132_assign_α:
                        mov              rax, qword ptr [rbp + 2656]
                        mov              rdx, qword ptr [rbp + 2664]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 2640], rax
                        mov              qword ptr [rbp + 2648], rdx
                                                                                        jmp   n6_var_α
#-----------------------------------------------------------------------------------------------------------------------
n133_subscript_α:
                        mov              rdi, qword ptr [rbp + 1248]
                        mov              rsi, qword ptr [rbp + 1256]
                        mov              rdx, qword ptr [rbp + 1264]
                        mov              rcx, qword ptr [rbp + 1272]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n131_var_α
                        mov              qword ptr [rbp + 1280], rax
                        mov              qword ptr [rbp + 1288], rdx
                                                                                        jmp   n136_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n134_lit_integer_α:
                        mov              qword ptr [rbp + 1344], 6
                        mov              rax, qword ptr [rip + .Lx346_0]
                        mov              qword ptr [rbp + 1352], rax
                                                                                        jmp   n137_subscript_α
.Lx346_0:
                        .quad            2
#=======================================================================================================================
#         r<3> = 'D'
#-----------------------------------------------------------------------------------------------------------------------
n135_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1408], rax
                        mov              qword ptr [rbp + 1416], rdx
                                                                                        jmp   n138_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n136_lit_string_α:
                        mov              qword ptr [rbp + 1296], 1
                        mov              rax, qword ptr [rip + .Lx348_0]
                        mov              qword ptr [rbp + 1304], rax
                                                                                        jmp   n140_assign_var_α
.Lx348_0:
                        .quad            .Lx348_0_s
.Lx348_0_s:
                        .string          "M"
#-----------------------------------------------------------------------------------------------------------------------
n137_subscript_α:
                        mov              rdi, qword ptr [rbp + 1328]
                        mov              rsi, qword ptr [rbp + 1336]
                        mov              rdx, qword ptr [rbp + 1344]
                        mov              rcx, qword ptr [rbp + 1352]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n135_var_α
                        mov              qword ptr [rbp + 1360], rax
                        mov              qword ptr [rbp + 1368], rdx
                                                                                        jmp   n141_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n138_lit_integer_α:
                        mov              qword ptr [rbp + 1424], 6
                        mov              rax, qword ptr [rip + .Lx350_0]
                        mov              qword ptr [rbp + 1432], rax
                                                                                        jmp   n142_subscript_α
.Lx350_0:
                        .quad            3
#=======================================================================================================================
#         r<4> = 'CD'
#-----------------------------------------------------------------------------------------------------------------------
n139_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1488], rax
                        mov              qword ptr [rbp + 1496], rdx
                                                                                        jmp   n143_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n140_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1280]
                        mov              rsi, qword ptr [rbp + 1288]
                        mov              rdx, qword ptr [rbp + 1296]
                        mov              rcx, qword ptr [rbp + 1304]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n131_var_α
                        mov              qword ptr [rbp + 1312], rax
                        mov              qword ptr [rbp + 1320], rdx
                                                                                        jmp   n131_var_α
#-----------------------------------------------------------------------------------------------------------------------
n141_lit_string_α:
                        mov              qword ptr [rbp + 1376], 1
                        mov              rax, qword ptr [rip + .Lx353_0]
                        mov              qword ptr [rbp + 1384], rax
                                                                                        jmp   n145_assign_var_α
.Lx353_0:
                        .quad            .Lx353_0_s
.Lx353_0_s:
                        .string          "CM"
#-----------------------------------------------------------------------------------------------------------------------
n142_subscript_α:
                        mov              rdi, qword ptr [rbp + 1408]
                        mov              rsi, qword ptr [rbp + 1416]
                        mov              rdx, qword ptr [rbp + 1424]
                        mov              rcx, qword ptr [rbp + 1432]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n139_var_α
                        mov              qword ptr [rbp + 1440], rax
                        mov              qword ptr [rbp + 1448], rdx
                                                                                        jmp   n146_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n143_lit_integer_α:
                        mov              qword ptr [rbp + 1504], 6
                        mov              rax, qword ptr [rip + .Lx355_0]
                        mov              qword ptr [rbp + 1512], rax
                                                                                        jmp   n147_subscript_α
.Lx355_0:
                        .quad            4
#=======================================================================================================================
#         r<5> = 'C'
#-----------------------------------------------------------------------------------------------------------------------
n144_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1568], rax
                        mov              qword ptr [rbp + 1576], rdx
                                                                                        jmp   n148_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n145_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1360]
                        mov              rsi, qword ptr [rbp + 1368]
                        mov              rdx, qword ptr [rbp + 1376]
                        mov              rcx, qword ptr [rbp + 1384]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n135_var_α
                        mov              qword ptr [rbp + 1392], rax
                        mov              qword ptr [rbp + 1400], rdx
                                                                                        jmp   n135_var_α
#-----------------------------------------------------------------------------------------------------------------------
n146_lit_string_α:
                        mov              qword ptr [rbp + 1456], 1
                        mov              rax, qword ptr [rip + .Lx358_0]
                        mov              qword ptr [rbp + 1464], rax
                                                                                        jmp   n150_assign_var_α
.Lx358_0:
                        .quad            .Lx358_0_s
.Lx358_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n147_subscript_α:
                        mov              rdi, qword ptr [rbp + 1488]
                        mov              rsi, qword ptr [rbp + 1496]
                        mov              rdx, qword ptr [rbp + 1504]
                        mov              rcx, qword ptr [rbp + 1512]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n144_var_α
                        mov              qword ptr [rbp + 1520], rax
                        mov              qword ptr [rbp + 1528], rdx
                                                                                        jmp   n151_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n148_lit_integer_α:
                        mov              qword ptr [rbp + 1584], 6
                        mov              rax, qword ptr [rip + .Lx360_0]
                        mov              qword ptr [rbp + 1592], rax
                                                                                        jmp   n152_subscript_α
.Lx360_0:
                        .quad            5
#=======================================================================================================================
#         r<6> = 'XC'
#-----------------------------------------------------------------------------------------------------------------------
n149_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1648], rax
                        mov              qword ptr [rbp + 1656], rdx
                                                                                        jmp   n153_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n150_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1440]
                        mov              rsi, qword ptr [rbp + 1448]
                        mov              rdx, qword ptr [rbp + 1456]
                        mov              rcx, qword ptr [rbp + 1464]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n139_var_α
                        mov              qword ptr [rbp + 1472], rax
                        mov              qword ptr [rbp + 1480], rdx
                                                                                        jmp   n139_var_α
#-----------------------------------------------------------------------------------------------------------------------
n151_lit_string_α:
                        mov              qword ptr [rbp + 1536], 1
                        mov              rax, qword ptr [rip + .Lx363_0]
                        mov              qword ptr [rbp + 1544], rax
                                                                                        jmp   n155_assign_var_α
.Lx363_0:
                        .quad            .Lx363_0_s
.Lx363_0_s:
                        .string          "CD"
#-----------------------------------------------------------------------------------------------------------------------
n152_subscript_α:
                        mov              rdi, qword ptr [rbp + 1568]
                        mov              rsi, qword ptr [rbp + 1576]
                        mov              rdx, qword ptr [rbp + 1584]
                        mov              rcx, qword ptr [rbp + 1592]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n149_var_α
                        mov              qword ptr [rbp + 1600], rax
                        mov              qword ptr [rbp + 1608], rdx
                                                                                        jmp   n156_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n153_lit_integer_α:
                        mov              qword ptr [rbp + 1664], 6
                        mov              rax, qword ptr [rip + .Lx365_0]
                        mov              qword ptr [rbp + 1672], rax
                                                                                        jmp   n157_subscript_α
.Lx365_0:
                        .quad            6
#=======================================================================================================================
#         r<7> = 'L'
#-----------------------------------------------------------------------------------------------------------------------
n154_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1728], rax
                        mov              qword ptr [rbp + 1736], rdx
                                                                                        jmp   n158_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n155_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1520]
                        mov              rsi, qword ptr [rbp + 1528]
                        mov              rdx, qword ptr [rbp + 1536]
                        mov              rcx, qword ptr [rbp + 1544]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n144_var_α
                        mov              qword ptr [rbp + 1552], rax
                        mov              qword ptr [rbp + 1560], rdx
                                                                                        jmp   n144_var_α
#-----------------------------------------------------------------------------------------------------------------------
n156_lit_string_α:
                        mov              qword ptr [rbp + 1616], 1
                        mov              rax, qword ptr [rip + .Lx368_0]
                        mov              qword ptr [rbp + 1624], rax
                                                                                        jmp   n160_assign_var_α
.Lx368_0:
                        .quad            .Lx368_0_s
.Lx368_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n157_subscript_α:
                        mov              rdi, qword ptr [rbp + 1648]
                        mov              rsi, qword ptr [rbp + 1656]
                        mov              rdx, qword ptr [rbp + 1664]
                        mov              rcx, qword ptr [rbp + 1672]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n154_var_α
                        mov              qword ptr [rbp + 1680], rax
                        mov              qword ptr [rbp + 1688], rdx
                                                                                        jmp   n161_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n158_lit_integer_α:
                        mov              qword ptr [rbp + 1744], 6
                        mov              rax, qword ptr [rip + .Lx370_0]
                        mov              qword ptr [rbp + 1752], rax
                                                                                        jmp   n162_subscript_α
.Lx370_0:
                        .quad            7
#=======================================================================================================================
#         r<8> = 'XL'
#-----------------------------------------------------------------------------------------------------------------------
n159_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1808], rax
                        mov              qword ptr [rbp + 1816], rdx
                                                                                        jmp   n163_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n160_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1600]
                        mov              rsi, qword ptr [rbp + 1608]
                        mov              rdx, qword ptr [rbp + 1616]
                        mov              rcx, qword ptr [rbp + 1624]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n149_var_α
                        mov              qword ptr [rbp + 1632], rax
                        mov              qword ptr [rbp + 1640], rdx
                                                                                        jmp   n149_var_α
#-----------------------------------------------------------------------------------------------------------------------
n161_lit_string_α:
                        mov              qword ptr [rbp + 1696], 1
                        mov              rax, qword ptr [rip + .Lx373_0]
                        mov              qword ptr [rbp + 1704], rax
                                                                                        jmp   n165_assign_var_α
.Lx373_0:
                        .quad            .Lx373_0_s
.Lx373_0_s:
                        .string          "XC"
#-----------------------------------------------------------------------------------------------------------------------
n162_subscript_α:
                        mov              rdi, qword ptr [rbp + 1728]
                        mov              rsi, qword ptr [rbp + 1736]
                        mov              rdx, qword ptr [rbp + 1744]
                        mov              rcx, qword ptr [rbp + 1752]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n159_var_α
                        mov              qword ptr [rbp + 1760], rax
                        mov              qword ptr [rbp + 1768], rdx
                                                                                        jmp   n166_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n163_lit_integer_α:
                        mov              qword ptr [rbp + 1824], 6
                        mov              rax, qword ptr [rip + .Lx375_0]
                        mov              qword ptr [rbp + 1832], rax
                                                                                        jmp   n167_subscript_α
.Lx375_0:
                        .quad            8
#=======================================================================================================================
#         r<9> = 'X'
#-----------------------------------------------------------------------------------------------------------------------
n164_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1888], rax
                        mov              qword ptr [rbp + 1896], rdx
                                                                                        jmp   n168_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n165_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1680]
                        mov              rsi, qword ptr [rbp + 1688]
                        mov              rdx, qword ptr [rbp + 1696]
                        mov              rcx, qword ptr [rbp + 1704]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n154_var_α
                        mov              qword ptr [rbp + 1712], rax
                        mov              qword ptr [rbp + 1720], rdx
                                                                                        jmp   n154_var_α
#-----------------------------------------------------------------------------------------------------------------------
n166_lit_string_α:
                        mov              qword ptr [rbp + 1776], 1
                        mov              rax, qword ptr [rip + .Lx378_0]
                        mov              qword ptr [rbp + 1784], rax
                                                                                        jmp   n170_assign_var_α
.Lx378_0:
                        .quad            .Lx378_0_s
.Lx378_0_s:
                        .string          "L"
#-----------------------------------------------------------------------------------------------------------------------
n167_subscript_α:
                        mov              rdi, qword ptr [rbp + 1808]
                        mov              rsi, qword ptr [rbp + 1816]
                        mov              rdx, qword ptr [rbp + 1824]
                        mov              rcx, qword ptr [rbp + 1832]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n164_var_α
                        mov              qword ptr [rbp + 1840], rax
                        mov              qword ptr [rbp + 1848], rdx
                                                                                        jmp   n171_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n168_lit_integer_α:
                        mov              qword ptr [rbp + 1904], 6
                        mov              rax, qword ptr [rip + .Lx380_0]
                        mov              qword ptr [rbp + 1912], rax
                                                                                        jmp   n172_subscript_α
.Lx380_0:
                        .quad            9
#=======================================================================================================================
#         r<10> = 'IX'
#-----------------------------------------------------------------------------------------------------------------------
n169_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1968], rax
                        mov              qword ptr [rbp + 1976], rdx
                                                                                        jmp   n173_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n170_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1760]
                        mov              rsi, qword ptr [rbp + 1768]
                        mov              rdx, qword ptr [rbp + 1776]
                        mov              rcx, qword ptr [rbp + 1784]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n159_var_α
                        mov              qword ptr [rbp + 1792], rax
                        mov              qword ptr [rbp + 1800], rdx
                                                                                        jmp   n159_var_α
#-----------------------------------------------------------------------------------------------------------------------
n171_lit_string_α:
                        mov              qword ptr [rbp + 1856], 1
                        mov              rax, qword ptr [rip + .Lx383_0]
                        mov              qword ptr [rbp + 1864], rax
                                                                                        jmp   n175_assign_var_α
.Lx383_0:
                        .quad            .Lx383_0_s
.Lx383_0_s:
                        .string          "XL"
#-----------------------------------------------------------------------------------------------------------------------
n172_subscript_α:
                        mov              rdi, qword ptr [rbp + 1888]
                        mov              rsi, qword ptr [rbp + 1896]
                        mov              rdx, qword ptr [rbp + 1904]
                        mov              rcx, qword ptr [rbp + 1912]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n169_var_α
                        mov              qword ptr [rbp + 1920], rax
                        mov              qword ptr [rbp + 1928], rdx
                                                                                        jmp   n176_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n173_lit_integer_α:
                        mov              qword ptr [rbp + 1984], 6
                        mov              rax, qword ptr [rip + .Lx385_0]
                        mov              qword ptr [rbp + 1992], rax
                                                                                        jmp   n177_subscript_α
.Lx385_0:
                        .quad            10
#=======================================================================================================================
#         r<11> = 'V'
#-----------------------------------------------------------------------------------------------------------------------
n174_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 2048], rax
                        mov              qword ptr [rbp + 2056], rdx
                                                                                        jmp   n178_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n175_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1840]
                        mov              rsi, qword ptr [rbp + 1848]
                        mov              rdx, qword ptr [rbp + 1856]
                        mov              rcx, qword ptr [rbp + 1864]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n164_var_α
                        mov              qword ptr [rbp + 1872], rax
                        mov              qword ptr [rbp + 1880], rdx
                                                                                        jmp   n164_var_α
#-----------------------------------------------------------------------------------------------------------------------
n176_lit_string_α:
                        mov              qword ptr [rbp + 1936], 1
                        mov              rax, qword ptr [rip + .Lx388_0]
                        mov              qword ptr [rbp + 1944], rax
                                                                                        jmp   n180_assign_var_α
.Lx388_0:
                        .quad            .Lx388_0_s
.Lx388_0_s:
                        .string          "X"
#-----------------------------------------------------------------------------------------------------------------------
n177_subscript_α:
                        mov              rdi, qword ptr [rbp + 1968]
                        mov              rsi, qword ptr [rbp + 1976]
                        mov              rdx, qword ptr [rbp + 1984]
                        mov              rcx, qword ptr [rbp + 1992]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n174_var_α
                        mov              qword ptr [rbp + 2000], rax
                        mov              qword ptr [rbp + 2008], rdx
                                                                                        jmp   n181_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n178_lit_integer_α:
                        mov              qword ptr [rbp + 2064], 6
                        mov              rax, qword ptr [rip + .Lx390_0]
                        mov              qword ptr [rbp + 2072], rax
                                                                                        jmp   n182_subscript_α
.Lx390_0:
                        .quad            11
#=======================================================================================================================
#         r<12> = 'IV'
#-----------------------------------------------------------------------------------------------------------------------
n179_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 2128], rax
                        mov              qword ptr [rbp + 2136], rdx
                                                                                        jmp   n183_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n180_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1920]
                        mov              rsi, qword ptr [rbp + 1928]
                        mov              rdx, qword ptr [rbp + 1936]
                        mov              rcx, qword ptr [rbp + 1944]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n169_var_α
                        mov              qword ptr [rbp + 1952], rax
                        mov              qword ptr [rbp + 1960], rdx
                                                                                        jmp   n169_var_α
#-----------------------------------------------------------------------------------------------------------------------
n181_lit_string_α:
                        mov              qword ptr [rbp + 2016], 1
                        mov              rax, qword ptr [rip + .Lx393_0]
                        mov              qword ptr [rbp + 2024], rax
                                                                                        jmp   n185_assign_var_α
.Lx393_0:
                        .quad            .Lx393_0_s
.Lx393_0_s:
                        .string          "IX"
#-----------------------------------------------------------------------------------------------------------------------
n182_subscript_α:
                        mov              rdi, qword ptr [rbp + 2048]
                        mov              rsi, qword ptr [rbp + 2056]
                        mov              rdx, qword ptr [rbp + 2064]
                        mov              rcx, qword ptr [rbp + 2072]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n179_var_α
                        mov              qword ptr [rbp + 2080], rax
                        mov              qword ptr [rbp + 2088], rdx
                                                                                        jmp   n186_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n183_lit_integer_α:
                        mov              qword ptr [rbp + 2144], 6
                        mov              rax, qword ptr [rip + .Lx395_0]
                        mov              qword ptr [rbp + 2152], rax
                                                                                        jmp   n187_subscript_α
.Lx395_0:
                        .quad            12
#=======================================================================================================================
#         r<13> = 'I'
#-----------------------------------------------------------------------------------------------------------------------
n184_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 2208], rax
                        mov              qword ptr [rbp + 2216], rdx
                                                                                        jmp   n188_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n185_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2000]
                        mov              rsi, qword ptr [rbp + 2008]
                        mov              rdx, qword ptr [rbp + 2016]
                        mov              rcx, qword ptr [rbp + 2024]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n174_var_α
                        mov              qword ptr [rbp + 2032], rax
                        mov              qword ptr [rbp + 2040], rdx
                                                                                        jmp   n174_var_α
#-----------------------------------------------------------------------------------------------------------------------
n186_lit_string_α:
                        mov              qword ptr [rbp + 2096], 1
                        mov              rax, qword ptr [rip + .Lx398_0]
                        mov              qword ptr [rbp + 2104], rax
                                                                                        jmp   n190_assign_var_α
.Lx398_0:
                        .quad            .Lx398_0_s
.Lx398_0_s:
                        .string          "V"
#-----------------------------------------------------------------------------------------------------------------------
n187_subscript_α:
                        mov              rdi, qword ptr [rbp + 2128]
                        mov              rsi, qword ptr [rbp + 2136]
                        mov              rdx, qword ptr [rbp + 2144]
                        mov              rcx, qword ptr [rbp + 2152]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n184_var_α
                        mov              qword ptr [rbp + 2160], rax
                        mov              qword ptr [rbp + 2168], rdx
                                                                                        jmp   n191_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n188_lit_integer_α:
                        mov              qword ptr [rbp + 2224], 6
                        mov              rax, qword ptr [rip + .Lx400_0]
                        mov              qword ptr [rbp + 2232], rax
                                                                                        jmp   n192_subscript_α
.Lx400_0:
                        .quad            13
#=======================================================================================================================
#         i = 1
#-----------------------------------------------------------------------------------------------------------------------
n189_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx401_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n193_assign_α
.Lx401_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n190_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2080]
                        mov              rsi, qword ptr [rbp + 2088]
                        mov              rdx, qword ptr [rbp + 2096]
                        mov              rcx, qword ptr [rbp + 2104]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n179_var_α
                        mov              qword ptr [rbp + 2112], rax
                        mov              qword ptr [rbp + 2120], rdx
                                                                                        jmp   n179_var_α
#-----------------------------------------------------------------------------------------------------------------------
n191_lit_string_α:
                        mov              qword ptr [rbp + 2176], 1
                        mov              rax, qword ptr [rip + .Lx403_0]
                        mov              qword ptr [rbp + 2184], rax
                                                                                        jmp   n194_assign_var_α
.Lx403_0:
                        .quad            .Lx403_0_s
.Lx403_0_s:
                        .string          "IV"
#-----------------------------------------------------------------------------------------------------------------------
n192_subscript_α:
                        mov              rdi, qword ptr [rbp + 2208]
                        mov              rsi, qword ptr [rbp + 2216]
                        mov              rdx, qword ptr [rbp + 2224]
                        mov              rcx, qword ptr [rbp + 2232]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n189_lit_integer_α
                        mov              qword ptr [rbp + 2240], rax
                        mov              qword ptr [rbp + 2248], rdx
                                                                                        jmp   n195_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n193_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                        mov              qword ptr [rbp + 2288], rax
                        mov              qword ptr [rbp + 2296], rdx
                                                                                        jmp   n6_var_α
#-----------------------------------------------------------------------------------------------------------------------
n194_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2160]
                        mov              rsi, qword ptr [rbp + 2168]
                        mov              rdx, qword ptr [rbp + 2176]
                        mov              rcx, qword ptr [rbp + 2184]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n184_var_α
                        mov              qword ptr [rbp + 2192], rax
                        mov              qword ptr [rbp + 2200], rdx
                                                                                        jmp   n184_var_α
#-----------------------------------------------------------------------------------------------------------------------
n195_lit_string_α:
                        mov              qword ptr [rbp + 2256], 1
                        mov              rax, qword ptr [rip + .Lx407_0]
                        mov              qword ptr [rbp + 2264], rax
                                                                                        jmp   n196_assign_var_α
.Lx407_0:
                        .quad            .Lx407_0_s
.Lx407_0_s:
                        .string          "I"
#-----------------------------------------------------------------------------------------------------------------------
n196_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2240]
                        mov              rsi, qword ptr [rbp + 2248]
                        mov              rdx, qword ptr [rbp + 2256]
                        mov              rcx, qword ptr [rbp + 2264]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n189_lit_integer_α
                        mov              qword ptr [rbp + 2272], rax
                        mov              qword ptr [rbp + 2280], rdx
                                                                                        jmp   n189_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__roman_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__roman_β:
                                                                                        jmp   proc_LBL__roman_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__roman_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 3336]
                        lea              rsp, [rbp + 3360]
                        mov              rbp, [rbp + 3352]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__roman_ω:
                        mov              rax, [rbp + 3344]
                        lea              rsp, [rbp + 3360]
                        mov              rbp, [rbp + 3352]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_roman_α
proc_roman_α:
                        .global          proc_roman_α
                        .global          proc_roman_β
                        .global          proc_roman_γ
                        .global          proc_roman_ω
                        sub              rsp, 3360
                        mov              [rsp + 3336], rcx
                        mov              [rsp + 3344], rdx
                        mov              [rsp + 3352], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              ecx, 3328
                        xor              eax, eax
                        rep stosb
proc_roman_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n409_op14_α:
                        mov              rdi, qword ptr [rbp + 3336]
                        mov              rsi, qword ptr [rbp + 3344]
                        lea              rdx, [rbp + 3360]
                        mov              rcx, qword ptr [rbp + 3352]
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n410_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n410_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx414_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx414_1
.Lx414_0:
                        .quad            .Lx414_0_s
.Lx414_0_s:
                        .string          "roman"
.Lx414_1:
                                                                                        jmp   proc_roman_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_β:
                                                                                        jmp   proc_roman_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 3336]
                        lea              rsp, [rbp + 3360]
                        mov              rbp, [rbp + 3352]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_roman_ω:
                        mov              rax, [rbp + 3344]
                        lea              rsp, [rbp + 3360]
                        mov              rbp, [rbp + 3352]
                                                                                        jmp   rax
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__roman"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__roman_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 3328
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "roman"
.Lstartup_pp1_0:        .string          "n"
.Lstartup_pp1_1:        .string          "s"
.Lstartup_pp1_2:        .string          "v"
.Lstartup_pp1_3:        .string          "r"
.Lstartup_pp1_4:        .string          "i"
                        .align           8
.Lstartup_pnames1:
                        .quad            .Lstartup_pp1_0
                        .quad            .Lstartup_pp1_1
                        .quad            .Lstartup_pp1_2
                        .quad            .Lstartup_pp1_3
                        .quad            .Lstartup_pp1_4
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + .Lstartup_pnames1]
                        mov              edx, 5
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_roman_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 5
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "roman"
.Lgvan1:                .string          "n"
.Lgvan2:                .string          "s"
.Lgvan3:                .string          "v"
.Lgvan4:                .string          "r"
.Lgvan5:                .string          "i"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .quad            .Lgvan4
                        .quad            .Lgvan5
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 6
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 6
                        call             gva_register@PLT
                        mov              r12, qword ptr [1879048192]
                        xor              esi, esi
                        call             main_α
                        xor              eax, eax
                        add              rsp, 24
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_α:
                        .global          main_α
                        .global          main_β
                        .global          main_γ
                        .global          main_ω
                        sub              rsp, 3336
                        mov              rdi, rsp
                        mov              ecx, 3336
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 3328], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#         DEFINE('roman(n)s,v,r,i')                                     :(roman_end)
#         OUTPUT = roman(1)
#-----------------------------------------------------------------------------------------------------------------------
n415_lit_integer_α:
                        mov              qword ptr [rbp + 2912], 6
                        mov              rax, qword ptr [rip + .Lx612_0]
                        mov              qword ptr [rbp + 2920], rax
                                                                                        jmp   n421_call_α
.Lx612_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n416_goto_α:
                                                                                        jmp   n422_lit_string_α
n416_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n417_goto_α:
                                                                                        jmp   n423_var_α
n417_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n418_goto_α:
                                                                                        jmp   n424_var_α
n418_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n419_goto_α:
                                                                                        jmp   n425_var_α
n419_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n420_goto_α:
                                                                                        jmp   n415_lit_integer_α
n420_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n421_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx619_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx619_5
                        mov              rax, qword ptr [rbp + 2912]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 2920]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx619_6]
                        lea              rdx, [rip + .Lx619_7]
                                                                                        jmp   rax
.Lx619_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx619_2
.Lx619_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx619_2
.Lx619_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx619_20
                        mov              rax, qword ptr [rbp + 2912]
                        mov              rdx, qword ptr [rbp + 2920]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx619_21
.Lx619_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 2912]
                        mov              rdx, qword ptr [rbp + 2920]
                        call             rt_arg_stage@PLT
.Lx619_21:
                        mov              rdi, qword ptr [rip + .Lx619_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx619_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx619_3]
                        lea              rdx, [rip + .Lx619_4]
                                                                                        jmp   rax
.Lx619_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx619_2
.Lx619_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx619_2
.Lx619_1:
                        call             rt_faildescr@PLT
.Lx619_2:
                        mov              qword ptr [rbp + 2864], rax
                        mov              qword ptr [rbp + 2872], rdx
                        cmp              eax, 99
                                                                                        je    n427_lit_integer_α
                                                                                        jmp   n426_assign_α
n421_call_β:
                                                                                        jmp   n427_lit_integer_α
.Lx619_0:
                        .quad            .Lx619_0_s
.Lx619_0_s:
                        .string          "roman"
#=======================================================================================================================
# roman   s = ''
#-----------------------------------------------------------------------------------------------------------------------
n422_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx620_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n428_assign_α
.Lx620_0:
                        .quad            .Lx620_0_s
.Lx620_0_s:
                        .string          ""
#=======================================================================================================================
# RLOOP   GT(n, 0)                                                       :F(RDONE)
#-----------------------------------------------------------------------------------------------------------------------
n423_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 2384], rax
                        mov              qword ptr [rbp + 2392], rdx
                                                                                        jmp   n429_lit_integer_α
#=======================================================================================================================
# RNEXT   i = i + 1                                                      :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n424_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n430_lit_integer_α
#=======================================================================================================================
# RDONE   roman = s                                                      :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n425_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n431_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n426_assign_α:
                        mov              rsi, qword ptr [rbp + 2864]
                        mov              rdx, qword ptr [rbp + 2872]
                        mov              rdi, qword ptr [rip + .Lx624_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 2848], rax
                        mov              qword ptr [rbp + 2856], rdx
                                                                                        jmp   n427_lit_integer_α
.Lx624_0:
                        .quad            .Lx624_0_s
.Lx624_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(4)
#-----------------------------------------------------------------------------------------------------------------------
n427_lit_integer_α:
                        mov              qword ptr [rbp + 2992], 6
                        mov              rax, qword ptr [rip + .Lx625_0]
                        mov              qword ptr [rbp + 3000], rax
                                                                                        jmp   n433_call_α
.Lx625_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n428_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        mov              qword ptr [rbp + 16], rax
                        mov              qword ptr [rbp + 24], rdx
                                                                                        jmp   n434_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n429_lit_integer_α:
                        mov              qword ptr [rbp + 2368], 6
                        mov              rax, qword ptr [rip + .Lx627_0]
                        mov              qword ptr [rbp + 2376], rax
                                                                                        jmp   n435_op75_α
.Lx627_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n430_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx628_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n436_binop_α
.Lx628_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n431_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 2816], rax
                        mov              qword ptr [rbp + 2824], rdx
                                                                                        jmp   n432_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n432_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n433_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx633_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx633_5
                        mov              rax, qword ptr [rbp + 2992]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 3000]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx633_6]
                        lea              rdx, [rip + .Lx633_7]
                                                                                        jmp   rax
.Lx633_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx633_2
.Lx633_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx633_2
.Lx633_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx633_20
                        mov              rax, qword ptr [rbp + 2992]
                        mov              rdx, qword ptr [rbp + 3000]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx633_21
.Lx633_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 2992]
                        mov              rdx, qword ptr [rbp + 3000]
                        call             rt_arg_stage@PLT
.Lx633_21:
                        mov              rdi, qword ptr [rip + .Lx633_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx633_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx633_3]
                        lea              rdx, [rip + .Lx633_4]
                                                                                        jmp   rax
.Lx633_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx633_2
.Lx633_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx633_2
.Lx633_1:
                        call             rt_faildescr@PLT
.Lx633_2:
                        mov              qword ptr [rbp + 2944], rax
                        mov              qword ptr [rbp + 2952], rdx
                        cmp              eax, 99
                                                                                        je    n438_lit_integer_α
                                                                                        jmp   n437_assign_α
n433_call_β:
                                                                                        jmp   n438_lit_integer_α
.Lx633_0:
                        .quad            .Lx633_0_s
.Lx633_0_s:
                        .string          "roman"
#=======================================================================================================================
#         v = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n434_lit_integer_α:
                        mov              qword ptr [rbp + 112], 6
                        mov              rax, qword ptr [rip + .Lx634_0]
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   n439_call_α
.Lx634_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n435_op75_α:
                        mov              eax, dword ptr [rbp + 2384]
                        cmp              eax, 7
                                                                                        je    .Lx636_1
                        cmp              eax, 6
                                                                                        jne   .Lx636_0
                        mov              eax, dword ptr [rbp + 2368]
                        cmp              eax, 6
                                                                                        jne   .Lx636_0
.Lx636_1:
                        mov              rax, qword ptr [rbp + 2384]
                        mov              qword ptr [rbp + 2352], rax
                        mov              rax, qword ptr [rbp + 2392]
                        mov              qword ptr [rbp + 2360], rax
                                                                                        jmp   n440_op75_α
.Lx636_0:
                        lea              rdi, [rbp + 2384]
                        lea              rsi, [rbp + 2368]
                        lea              rdx, [rbp + 2352]
                        mov              rcx, 111
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n440_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n436_binop_α:
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 100
                                                                                        je    .Lx637_0
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, 100
                                                                                        je    .Lx637_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx637_2
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, 6
                                                                                        jne   .Lx637_2
                        mov              rax, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rsp + 8]
                        add              rax, rcx
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n441_assign_α
.Lx637_0:
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, qword ptr [rsp + 8]
                        mov              r8d, 0
                        lea              r9, [rsp + 16]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        je    .Lx637_2
                        add              rsp, 16
                                                                                        jmp   n441_assign_α
.Lx637_2:
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, qword ptr [rsp + 8]
                        mov              r8d, 0
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx637_240
                        add              rsp, 32
                                                                                        jmp   n423_var_α
.Lx637_240:
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n441_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n437_assign_α:
                        mov              rsi, qword ptr [rbp + 2944]
                        mov              rdx, qword ptr [rbp + 2952]
                        mov              rdi, qword ptr [rip + .Lx638_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 2928], rax
                        mov              qword ptr [rbp + 2936], rdx
                                                                                        jmp   n438_lit_integer_α
.Lx638_0:
                        .quad            .Lx638_0_s
.Lx638_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(9)
#-----------------------------------------------------------------------------------------------------------------------
n438_lit_integer_α:
                        mov              qword ptr [rbp + 3072], 6
                        mov              rax, qword ptr [rip + .Lx639_0]
                        mov              qword ptr [rbp + 3080], rax
                                                                                        jmp   n442_call_α
.Lx639_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n439_call_α:
                        mov              rax, qword ptr [rbp + 112]
                        mov              qword ptr [rbp + 80], rax
                        mov              rax, qword ptr [rbp + 120]
                        mov              qword ptr [rbp + 88], rax
                        .section         .rodata
.Lrkfn641:              .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn641]
                        lea              rsi, [rbp + 80]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 64], rax
                        mov              qword ptr [rbp + 72], rdx
                        cmp              eax, 99
                                                                                        je    n444_var_α
                                                                                        jmp   n443_assign_α
n439_call_β:
                                                                                        jmp   n444_var_α
#-----------------------------------------------------------------------------------------------------------------------
n440_op75_α:
                        mov              eax, dword ptr [rbp + 2368]
                        cmp              eax, 7
                                                                                        je    .Lx643_1
                        cmp              eax, 6
                                                                                        jne   .Lx643_0
                        mov              eax, dword ptr [rbp + 2384]
                        cmp              eax, 6
                                                                                        jne   .Lx643_0
.Lx643_1:
                        mov              rax, qword ptr [rbp + 2368]
                        mov              qword ptr [rbp + 2336], rax
                        mov              rax, qword ptr [rbp + 2376]
                        mov              qword ptr [rbp + 2344], rax
                                                                                        jmp   n445_op77_α
.Lx643_0:
                        lea              rdi, [rbp + 2368]
                        lea              rsi, [rbp + 2384]
                        lea              rdx, [rbp + 2336]
                        mov              rcx, 112
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n445_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n441_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                        mov              qword ptr [rbp + 2752], rax
                        mov              qword ptr [rbp + 2760], rdx
                                                                                        jmp   n423_var_α
#-----------------------------------------------------------------------------------------------------------------------
n442_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx646_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx646_5
                        mov              rax, qword ptr [rbp + 3072]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 3080]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx646_6]
                        lea              rdx, [rip + .Lx646_7]
                                                                                        jmp   rax
.Lx646_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx646_2
.Lx646_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx646_2
.Lx646_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx646_20
                        mov              rax, qword ptr [rbp + 3072]
                        mov              rdx, qword ptr [rbp + 3080]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx646_21
.Lx646_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 3072]
                        mov              rdx, qword ptr [rbp + 3080]
                        call             rt_arg_stage@PLT
.Lx646_21:
                        mov              rdi, qword ptr [rip + .Lx646_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx646_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx646_3]
                        lea              rdx, [rip + .Lx646_4]
                                                                                        jmp   rax
.Lx646_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx646_2
.Lx646_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx646_2
.Lx646_1:
                        call             rt_faildescr@PLT
.Lx646_2:
                        mov              qword ptr [rbp + 3024], rax
                        mov              qword ptr [rbp + 3032], rdx
                        cmp              eax, 99
                                                                                        je    n447_lit_integer_α
                                                                                        jmp   n446_assign_α
n442_call_β:
                                                                                        jmp   n447_lit_integer_α
.Lx646_0:
                        .quad            .Lx646_0_s
.Lx646_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n443_assign_α:
                        mov              rax, qword ptr [rbp + 64]
                        mov              rdx, qword ptr [rbp + 72]
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                        mov              qword ptr [rbp + 48], rax
                        mov              qword ptr [rbp + 56], rdx
                                                                                        jmp   n444_var_α
#=======================================================================================================================
#         v<1> = 1000
#-----------------------------------------------------------------------------------------------------------------------
n444_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 128], rax
                        mov              qword ptr [rbp + 136], rdx
                                                                                        jmp   n448_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n445_op77_α:
                        lea              rdi, [rbp + 2352]
                        lea              rsi, [rbp + 2336]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jle   n425_var_α
                        mov              qword ptr [rbp + 2320], 0
                        mov              qword ptr [rbp + 2328], 0
                                                                                        jmp   n450_var_α
#-----------------------------------------------------------------------------------------------------------------------
n446_assign_α:
                        mov              rsi, qword ptr [rbp + 3024]
                        mov              rdx, qword ptr [rbp + 3032]
                        mov              rdi, qword ptr [rip + .Lx651_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 3008], rax
                        mov              qword ptr [rbp + 3016], rdx
                                                                                        jmp   n447_lit_integer_α
.Lx651_0:
                        .quad            .Lx651_0_s
.Lx651_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(42)
#-----------------------------------------------------------------------------------------------------------------------
n447_lit_integer_α:
                        mov              qword ptr [rbp + 3152], 6
                        mov              rax, qword ptr [rip + .Lx652_0]
                        mov              qword ptr [rbp + 3160], rax
                                                                                        jmp   n451_call_α
.Lx652_0:
                        .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n448_lit_integer_α:
                        mov              qword ptr [rbp + 144], 6
                        mov              rax, qword ptr [rip + .Lx653_0]
                        mov              qword ptr [rbp + 152], rax
                                                                                        jmp   n452_subscript_α
.Lx653_0:
                        .quad            1
#=======================================================================================================================
#         v<2> = 900
#-----------------------------------------------------------------------------------------------------------------------
n449_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 208], rax
                        mov              qword ptr [rbp + 216], rdx
                                                                                        jmp   n453_lit_integer_α
#=======================================================================================================================
#         GE(n, v<i>)                                                    :F(RNEXT)
#-----------------------------------------------------------------------------------------------------------------------
n450_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 2512], rax
                        mov              qword ptr [rbp + 2520], rdx
                                                                                        jmp   n455_var_α
#-----------------------------------------------------------------------------------------------------------------------
n451_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx657_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx657_5
                        mov              rax, qword ptr [rbp + 3152]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 3160]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx657_6]
                        lea              rdx, [rip + .Lx657_7]
                                                                                        jmp   rax
.Lx657_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx657_2
.Lx657_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx657_2
.Lx657_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx657_20
                        mov              rax, qword ptr [rbp + 3152]
                        mov              rdx, qword ptr [rbp + 3160]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx657_21
.Lx657_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 3152]
                        mov              rdx, qword ptr [rbp + 3160]
                        call             rt_arg_stage@PLT
.Lx657_21:
                        mov              rdi, qword ptr [rip + .Lx657_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx657_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx657_3]
                        lea              rdx, [rip + .Lx657_4]
                                                                                        jmp   rax
.Lx657_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx657_2
.Lx657_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx657_2
.Lx657_1:
                        call             rt_faildescr@PLT
.Lx657_2:
                        mov              qword ptr [rbp + 3104], rax
                        mov              qword ptr [rbp + 3112], rdx
                        cmp              eax, 99
                                                                                        je    n457_lit_integer_α
                                                                                        jmp   n456_assign_α
n451_call_β:
                                                                                        jmp   n457_lit_integer_α
.Lx657_0:
                        .quad            .Lx657_0_s
.Lx657_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n452_subscript_α:
                        mov              rdi, qword ptr [rbp + 128]
                        mov              rsi, qword ptr [rbp + 136]
                        mov              rdx, qword ptr [rbp + 144]
                        mov              rcx, qword ptr [rbp + 152]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n449_var_α
                        mov              qword ptr [rbp + 160], rax
                        mov              qword ptr [rbp + 168], rdx
                                                                                        jmp   n458_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n453_lit_integer_α:
                        mov              qword ptr [rbp + 224], 6
                        mov              rax, qword ptr [rip + .Lx659_0]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   n459_subscript_α
.Lx659_0:
                        .quad            2
#=======================================================================================================================
#         v<3> = 500
#-----------------------------------------------------------------------------------------------------------------------
n454_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 288], rax
                        mov              qword ptr [rbp + 296], rdx
                                                                                        jmp   n460_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n455_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 2448], rax
                        mov              qword ptr [rbp + 2456], rdx
                                                                                        jmp   n462_var_α
#-----------------------------------------------------------------------------------------------------------------------
n456_assign_α:
                        mov              rsi, qword ptr [rbp + 3104]
                        mov              rdx, qword ptr [rbp + 3112]
                        mov              rdi, qword ptr [rip + .Lx662_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 3088], rax
                        mov              qword ptr [rbp + 3096], rdx
                                                                                        jmp   n457_lit_integer_α
.Lx662_0:
                        .quad            .Lx662_0_s
.Lx662_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(1999)
#-----------------------------------------------------------------------------------------------------------------------
n457_lit_integer_α:
                        mov              qword ptr [rbp + 3232], 6
                        mov              rax, qword ptr [rip + .Lx663_0]
                        mov              qword ptr [rbp + 3240], rax
                                                                                        jmp   n463_call_α
.Lx663_0:
                        .quad            1999
#-----------------------------------------------------------------------------------------------------------------------
n458_lit_integer_α:
                        mov              qword ptr [rbp + 176], 6
                        mov              rax, qword ptr [rip + .Lx664_0]
                        mov              qword ptr [rbp + 184], rax
                                                                                        jmp   n464_assign_var_α
.Lx664_0:
                        .quad            1000
#-----------------------------------------------------------------------------------------------------------------------
n459_subscript_α:
                        mov              rdi, qword ptr [rbp + 208]
                        mov              rsi, qword ptr [rbp + 216]
                        mov              rdx, qword ptr [rbp + 224]
                        mov              rcx, qword ptr [rbp + 232]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n454_var_α
                        mov              qword ptr [rbp + 240], rax
                        mov              qword ptr [rbp + 248], rdx
                                                                                        jmp   n465_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n460_lit_integer_α:
                        mov              qword ptr [rbp + 304], 6
                        mov              rax, qword ptr [rip + .Lx666_0]
                        mov              qword ptr [rbp + 312], rax
                                                                                        jmp   n466_subscript_α
.Lx666_0:
                        .quad            3
#=======================================================================================================================
#         v<4> = 400
#-----------------------------------------------------------------------------------------------------------------------
n461_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 368], rax
                        mov              qword ptr [rbp + 376], rdx
                                                                                        jmp   n467_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n462_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 2464], rax
                        mov              qword ptr [rbp + 2472], rdx
                                                                                        jmp   n469_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n463_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx670_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx670_5
                        mov              rax, qword ptr [rbp + 3232]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 3240]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx670_6]
                        lea              rdx, [rip + .Lx670_7]
                                                                                        jmp   rax
.Lx670_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx670_2
.Lx670_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx670_2
.Lx670_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx670_20
                        mov              rax, qword ptr [rbp + 3232]
                        mov              rdx, qword ptr [rbp + 3240]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx670_21
.Lx670_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 3232]
                        mov              rdx, qword ptr [rbp + 3240]
                        call             rt_arg_stage@PLT
.Lx670_21:
                        mov              rdi, qword ptr [rip + .Lx670_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx670_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx670_3]
                        lea              rdx, [rip + .Lx670_4]
                                                                                        jmp   rax
.Lx670_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx670_2
.Lx670_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx670_2
.Lx670_1:
                        call             rt_faildescr@PLT
.Lx670_2:
                        mov              qword ptr [rbp + 3184], rax
                        mov              qword ptr [rbp + 3192], rdx
                        cmp              eax, 99
                                                                                        je    n471_lit_integer_α
                                                                                        jmp   n470_assign_α
n463_call_β:
                                                                                        jmp   n471_lit_integer_α
.Lx670_0:
                        .quad            .Lx670_0_s
.Lx670_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n464_assign_var_α:
                        mov              rdi, qword ptr [rbp + 160]
                        mov              rsi, qword ptr [rbp + 168]
                        mov              rdx, qword ptr [rbp + 176]
                        mov              rcx, qword ptr [rbp + 184]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n449_var_α
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                                                                                        jmp   n449_var_α
#-----------------------------------------------------------------------------------------------------------------------
n465_lit_integer_α:
                        mov              qword ptr [rbp + 256], 6
                        mov              rax, qword ptr [rip + .Lx672_0]
                        mov              qword ptr [rbp + 264], rax
                                                                                        jmp   n472_assign_var_α
.Lx672_0:
                        .quad            900
#-----------------------------------------------------------------------------------------------------------------------
n466_subscript_α:
                        mov              rdi, qword ptr [rbp + 288]
                        mov              rsi, qword ptr [rbp + 296]
                        mov              rdx, qword ptr [rbp + 304]
                        mov              rcx, qword ptr [rbp + 312]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n461_var_α
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   n473_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n467_lit_integer_α:
                        mov              qword ptr [rbp + 384], 6
                        mov              rax, qword ptr [rip + .Lx674_0]
                        mov              qword ptr [rbp + 392], rax
                                                                                        jmp   n474_subscript_α
.Lx674_0:
                        .quad            4
#=======================================================================================================================
#         v<5> = 100
#-----------------------------------------------------------------------------------------------------------------------
n468_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 448], rax
                        mov              qword ptr [rbp + 456], rdx
                                                                                        jmp   n475_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n469_subscript_α:
                        mov              rdi, qword ptr [rbp + 2448]
                        mov              rsi, qword ptr [rbp + 2456]
                        mov              rdx, qword ptr [rbp + 2464]
                        mov              rcx, qword ptr [rbp + 2472]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n424_var_α
                        mov              qword ptr [rbp + 2480], rax
                        mov              qword ptr [rbp + 2488], rdx
                                                                                        jmp   n477_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n470_assign_α:
                        mov              rsi, qword ptr [rbp + 3184]
                        mov              rdx, qword ptr [rbp + 3192]
                        mov              rdi, qword ptr [rip + .Lx677_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 3168], rax
                        mov              qword ptr [rbp + 3176], rdx
                                                                                        jmp   n471_lit_integer_α
.Lx677_0:
                        .quad            .Lx677_0_s
.Lx677_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = roman(2024)
#-----------------------------------------------------------------------------------------------------------------------
n471_lit_integer_α:
                        mov              qword ptr [rbp + 3312], 6
                        mov              rax, qword ptr [rip + .Lx678_0]
                        mov              qword ptr [rbp + 3320], rax
                                                                                        jmp   n478_call_α
.Lx678_0:
                        .quad            2024
#-----------------------------------------------------------------------------------------------------------------------
n472_assign_var_α:
                        mov              rdi, qword ptr [rbp + 240]
                        mov              rsi, qword ptr [rbp + 248]
                        mov              rdx, qword ptr [rbp + 256]
                        mov              rcx, qword ptr [rbp + 264]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n454_var_α
                        mov              qword ptr [rbp + 272], rax
                        mov              qword ptr [rbp + 280], rdx
                                                                                        jmp   n454_var_α
#-----------------------------------------------------------------------------------------------------------------------
n473_lit_integer_α:
                        mov              qword ptr [rbp + 336], 6
                        mov              rax, qword ptr [rip + .Lx680_0]
                        mov              qword ptr [rbp + 344], rax
                                                                                        jmp   n479_assign_var_α
.Lx680_0:
                        .quad            500
#-----------------------------------------------------------------------------------------------------------------------
n474_subscript_α:
                        mov              rdi, qword ptr [rbp + 368]
                        mov              rsi, qword ptr [rbp + 376]
                        mov              rdx, qword ptr [rbp + 384]
                        mov              rcx, qword ptr [rbp + 392]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n468_var_α
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                                                                                        jmp   n480_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n475_lit_integer_α:
                        mov              qword ptr [rbp + 464], 6
                        mov              rax, qword ptr [rip + .Lx682_0]
                        mov              qword ptr [rbp + 472], rax
                                                                                        jmp   n481_subscript_α
.Lx682_0:
                        .quad            5
#=======================================================================================================================
#         v<6> = 90
#-----------------------------------------------------------------------------------------------------------------------
n476_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 528], rax
                        mov              qword ptr [rbp + 536], rdx
                                                                                        jmp   n482_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n477_deref_α:
                        mov              rdi, qword ptr [rbp + 2480]
                        mov              rsi, qword ptr [rbp + 2488]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n424_var_α
                        mov              qword ptr [rbp + 2496], rax
                        mov              qword ptr [rbp + 2504], rdx
                                                                                        jmp   n484_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n478_call_α:
                        sub              rsp, 96
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052320]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052328]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [1879052336]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [1879052344]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [1879052352]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [1879052360]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [1879052368]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [1879052376]
                        mov              qword ptr [rsp + 72], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 80], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 88], rax
                        mov              rdi, qword ptr [rip + .Lx686_0]
                        mov              esi, 5
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx686_5
                        mov              rax, qword ptr [rbp + 3312]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 3320]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx686_6]
                        lea              rdx, [rip + .Lx686_7]
                                                                                        jmp   rax
.Lx686_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx686_2
.Lx686_7:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [1879052368], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [1879052376], rax
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [1879052352], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [1879052360], rax
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [1879052336], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [1879052344], rax
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052320], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052328], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 96
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx686_2
.Lx686_5:
                        add              rsp, 96
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx686_20
                        mov              rax, qword ptr [rbp + 3312]
                        mov              rdx, qword ptr [rbp + 3320]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx686_21
.Lx686_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 3312]
                        mov              rdx, qword ptr [rbp + 3320]
                        call             rt_arg_stage@PLT
.Lx686_21:
                        mov              rdi, qword ptr [rip + .Lx686_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx686_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx686_3]
                        lea              rdx, [rip + .Lx686_4]
                                                                                        jmp   rax
.Lx686_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx686_2
.Lx686_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx686_2
.Lx686_1:
                        call             rt_faildescr@PLT
.Lx686_2:
                        mov              qword ptr [rbp + 3264], rax
                        mov              qword ptr [rbp + 3272], rdx
                        cmp              eax, 99
                                                                                        je    main_γ
                                                                                        jmp   n485_assign_α
n478_call_β:
                                                                                        jmp   main_γ
.Lx686_0:
                        .quad            .Lx686_0_s
.Lx686_0_s:
                        .string          "roman"
#-----------------------------------------------------------------------------------------------------------------------
n479_assign_var_α:
                        mov              rdi, qword ptr [rbp + 320]
                        mov              rsi, qword ptr [rbp + 328]
                        mov              rdx, qword ptr [rbp + 336]
                        mov              rcx, qword ptr [rbp + 344]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n461_var_α
                        mov              qword ptr [rbp + 352], rax
                        mov              qword ptr [rbp + 360], rdx
                                                                                        jmp   n461_var_α
#-----------------------------------------------------------------------------------------------------------------------
n480_lit_integer_α:
                        mov              qword ptr [rbp + 416], 6
                        mov              rax, qword ptr [rip + .Lx688_0]
                        mov              qword ptr [rbp + 424], rax
                                                                                        jmp   n486_assign_var_α
.Lx688_0:
                        .quad            400
#-----------------------------------------------------------------------------------------------------------------------
n481_subscript_α:
                        mov              rdi, qword ptr [rbp + 448]
                        mov              rsi, qword ptr [rbp + 456]
                        mov              rdx, qword ptr [rbp + 464]
                        mov              rcx, qword ptr [rbp + 472]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n476_var_α
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                                                                                        jmp   n487_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n482_lit_integer_α:
                        mov              qword ptr [rbp + 544], 6
                        mov              rax, qword ptr [rip + .Lx690_0]
                        mov              qword ptr [rbp + 552], rax
                                                                                        jmp   n488_subscript_α
.Lx690_0:
                        .quad            6
#=======================================================================================================================
#         v<7> = 50
#-----------------------------------------------------------------------------------------------------------------------
n483_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 608], rax
                        mov              qword ptr [rbp + 616], rdx
                                                                                        jmp   n489_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n484_op75_α:
                        mov              eax, dword ptr [rbp + 2512]
                        cmp              eax, 7
                                                                                        je    .Lx693_1
                        cmp              eax, 6
                                                                                        jne   .Lx693_0
                        mov              eax, dword ptr [rbp + 2496]
                        cmp              eax, 6
                                                                                        jne   .Lx693_0
.Lx693_1:
                        mov              rax, qword ptr [rbp + 2512]
                        mov              qword ptr [rbp + 2432], rax
                        mov              rax, qword ptr [rbp + 2520]
                        mov              qword ptr [rbp + 2440], rax
                                                                                        jmp   n491_op75_α
.Lx693_0:
                        lea              rdi, [rbp + 2512]
                        lea              rsi, [rbp + 2496]
                        lea              rdx, [rbp + 2432]
                        mov              rcx, 109
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n491_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n485_assign_α:
                        mov              rsi, qword ptr [rbp + 3264]
                        mov              rdx, qword ptr [rbp + 3272]
                        mov              rdi, qword ptr [rip + .Lx694_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 3248], rax
                        mov              qword ptr [rbp + 3256], rdx
                                                                                        jmp   main_γ
.Lx694_0:
                        .quad            .Lx694_0_s
.Lx694_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n486_assign_var_α:
                        mov              rdi, qword ptr [rbp + 400]
                        mov              rsi, qword ptr [rbp + 408]
                        mov              rdx, qword ptr [rbp + 416]
                        mov              rcx, qword ptr [rbp + 424]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n468_var_α
                        mov              qword ptr [rbp + 432], rax
                        mov              qword ptr [rbp + 440], rdx
                                                                                        jmp   n468_var_α
#-----------------------------------------------------------------------------------------------------------------------
n487_lit_integer_α:
                        mov              qword ptr [rbp + 496], 6
                        mov              rax, qword ptr [rip + .Lx696_0]
                        mov              qword ptr [rbp + 504], rax
                                                                                        jmp   n492_assign_var_α
.Lx696_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n488_subscript_α:
                        mov              rdi, qword ptr [rbp + 528]
                        mov              rsi, qword ptr [rbp + 536]
                        mov              rdx, qword ptr [rbp + 544]
                        mov              rcx, qword ptr [rbp + 552]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n483_var_α
                        mov              qword ptr [rbp + 560], rax
                        mov              qword ptr [rbp + 568], rdx
                                                                                        jmp   n493_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n489_lit_integer_α:
                        mov              qword ptr [rbp + 624], 6
                        mov              rax, qword ptr [rip + .Lx698_0]
                        mov              qword ptr [rbp + 632], rax
                                                                                        jmp   n494_subscript_α
.Lx698_0:
                        .quad            7
#=======================================================================================================================
#         v<8> = 40
#-----------------------------------------------------------------------------------------------------------------------
n490_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 688], rax
                        mov              qword ptr [rbp + 696], rdx
                                                                                        jmp   n495_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n491_op75_α:
                        mov              eax, dword ptr [rbp + 2496]
                        cmp              eax, 7
                                                                                        je    .Lx701_1
                        cmp              eax, 6
                                                                                        jne   .Lx701_0
                        mov              eax, dword ptr [rbp + 2512]
                        cmp              eax, 6
                                                                                        jne   .Lx701_0
.Lx701_1:
                        mov              rax, qword ptr [rbp + 2496]
                        mov              qword ptr [rbp + 2416], rax
                        mov              rax, qword ptr [rbp + 2504]
                        mov              qword ptr [rbp + 2424], rax
                                                                                        jmp   n497_op77_α
.Lx701_0:
                        lea              rdi, [rbp + 2496]
                        lea              rsi, [rbp + 2512]
                        lea              rdx, [rbp + 2416]
                        mov              rcx, 110
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n497_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n492_assign_var_α:
                        mov              rdi, qword ptr [rbp + 480]
                        mov              rsi, qword ptr [rbp + 488]
                        mov              rdx, qword ptr [rbp + 496]
                        mov              rcx, qword ptr [rbp + 504]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n476_var_α
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                                                                                        jmp   n476_var_α
#-----------------------------------------------------------------------------------------------------------------------
n493_lit_integer_α:
                        mov              qword ptr [rbp + 576], 6
                        mov              rax, qword ptr [rip + .Lx703_0]
                        mov              qword ptr [rbp + 584], rax
                                                                                        jmp   n498_assign_var_α
.Lx703_0:
                        .quad            90
#-----------------------------------------------------------------------------------------------------------------------
n494_subscript_α:
                        mov              rdi, qword ptr [rbp + 608]
                        mov              rsi, qword ptr [rbp + 616]
                        mov              rdx, qword ptr [rbp + 624]
                        mov              rcx, qword ptr [rbp + 632]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n490_var_α
                        mov              qword ptr [rbp + 640], rax
                        mov              qword ptr [rbp + 648], rdx
                                                                                        jmp   n499_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n495_lit_integer_α:
                        mov              qword ptr [rbp + 704], 6
                        mov              rax, qword ptr [rip + .Lx705_0]
                        mov              qword ptr [rbp + 712], rax
                                                                                        jmp   n500_subscript_α
.Lx705_0:
                        .quad            8
#=======================================================================================================================
#         v<9> = 10
#-----------------------------------------------------------------------------------------------------------------------
n496_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 768], rax
                        mov              qword ptr [rbp + 776], rdx
                                                                                        jmp   n501_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n497_op77_α:
                        lea              rdi, [rbp + 2432]
                        lea              rsi, [rbp + 2416]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        js    n424_var_α
                        mov              qword ptr [rbp + 2400], 0
                        mov              qword ptr [rbp + 2408], 0
                                                                                        jmp   n503_var_α
#-----------------------------------------------------------------------------------------------------------------------
n498_assign_var_α:
                        mov              rdi, qword ptr [rbp + 560]
                        mov              rsi, qword ptr [rbp + 568]
                        mov              rdx, qword ptr [rbp + 576]
                        mov              rcx, qword ptr [rbp + 584]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n483_var_α
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                                                                                        jmp   n483_var_α
#-----------------------------------------------------------------------------------------------------------------------
n499_lit_integer_α:
                        mov              qword ptr [rbp + 656], 6
                        mov              rax, qword ptr [rip + .Lx710_0]
                        mov              qword ptr [rbp + 664], rax
                                                                                        jmp   n504_assign_var_α
.Lx710_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n500_subscript_α:
                        mov              rdi, qword ptr [rbp + 688]
                        mov              rsi, qword ptr [rbp + 696]
                        mov              rdx, qword ptr [rbp + 704]
                        mov              rcx, qword ptr [rbp + 712]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n496_var_α
                        mov              qword ptr [rbp + 720], rax
                        mov              qword ptr [rbp + 728], rdx
                                                                                        jmp   n505_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n501_lit_integer_α:
                        mov              qword ptr [rbp + 784], 6
                        mov              rax, qword ptr [rip + .Lx712_0]
                        mov              qword ptr [rbp + 792], rax
                                                                                        jmp   n506_subscript_α
.Lx712_0:
                        .quad            9
#=======================================================================================================================
#         v<10> = 9
#-----------------------------------------------------------------------------------------------------------------------
n502_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 848], rax
                        mov              qword ptr [rbp + 856], rdx
                                                                                        jmp   n507_lit_integer_α
#=======================================================================================================================
#         s = s r<i>
#-----------------------------------------------------------------------------------------------------------------------
n503_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rbp + 2560], rax
                        mov              qword ptr [rbp + 2568], rdx
                                                                                        jmp   n509_var_α
#-----------------------------------------------------------------------------------------------------------------------
n504_assign_var_α:
                        mov              rdi, qword ptr [rbp + 640]
                        mov              rsi, qword ptr [rbp + 648]
                        mov              rdx, qword ptr [rbp + 656]
                        mov              rcx, qword ptr [rbp + 664]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n490_var_α
                        mov              qword ptr [rbp + 672], rax
                        mov              qword ptr [rbp + 680], rdx
                                                                                        jmp   n490_var_α
#-----------------------------------------------------------------------------------------------------------------------
n505_lit_integer_α:
                        mov              qword ptr [rbp + 736], 6
                        mov              rax, qword ptr [rip + .Lx716_0]
                        mov              qword ptr [rbp + 744], rax
                                                                                        jmp   n511_assign_var_α
.Lx716_0:
                        .quad            40
#-----------------------------------------------------------------------------------------------------------------------
n506_subscript_α:
                        mov              rdi, qword ptr [rbp + 768]
                        mov              rsi, qword ptr [rbp + 776]
                        mov              rdx, qword ptr [rbp + 784]
                        mov              rcx, qword ptr [rbp + 792]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n502_var_α
                        mov              qword ptr [rbp + 800], rax
                        mov              qword ptr [rbp + 808], rdx
                                                                                        jmp   n512_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n507_lit_integer_α:
                        mov              qword ptr [rbp + 864], 6
                        mov              rax, qword ptr [rip + .Lx718_0]
                        mov              qword ptr [rbp + 872], rax
                                                                                        jmp   n513_subscript_α
.Lx718_0:
                        .quad            10
#=======================================================================================================================
#         v<11> = 5
#-----------------------------------------------------------------------------------------------------------------------
n508_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 928], rax
                        mov              qword ptr [rbp + 936], rdx
                                                                                        jmp   n514_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n509_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 2576], rax
                        mov              qword ptr [rbp + 2584], rdx
                                                                                        jmp   n516_var_α
#=======================================================================================================================
#         n = n - v<i>                                                   :(RLOOP)
#-----------------------------------------------------------------------------------------------------------------------
n510_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 2672], rax
                        mov              qword ptr [rbp + 2680], rdx
                                                                                        jmp   n517_var_α
#-----------------------------------------------------------------------------------------------------------------------
n511_assign_var_α:
                        mov              rdi, qword ptr [rbp + 720]
                        mov              rsi, qword ptr [rbp + 728]
                        mov              rdx, qword ptr [rbp + 736]
                        mov              rcx, qword ptr [rbp + 744]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n496_var_α
                        mov              qword ptr [rbp + 752], rax
                        mov              qword ptr [rbp + 760], rdx
                                                                                        jmp   n496_var_α
#-----------------------------------------------------------------------------------------------------------------------
n512_lit_integer_α:
                        mov              qword ptr [rbp + 816], 6
                        mov              rax, qword ptr [rip + .Lx723_0]
                        mov              qword ptr [rbp + 824], rax
                                                                                        jmp   n518_assign_var_α
.Lx723_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n513_subscript_α:
                        mov              rdi, qword ptr [rbp + 848]
                        mov              rsi, qword ptr [rbp + 856]
                        mov              rdx, qword ptr [rbp + 864]
                        mov              rcx, qword ptr [rbp + 872]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n508_var_α
                        mov              qword ptr [rbp + 880], rax
                        mov              qword ptr [rbp + 888], rdx
                                                                                        jmp   n519_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n514_lit_integer_α:
                        mov              qword ptr [rbp + 944], 6
                        mov              rax, qword ptr [rip + .Lx725_0]
                        mov              qword ptr [rbp + 952], rax
                                                                                        jmp   n520_subscript_α
.Lx725_0:
                        .quad            11
#=======================================================================================================================
#         v<12> = 4
#-----------------------------------------------------------------------------------------------------------------------
n515_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 1008], rax
                        mov              qword ptr [rbp + 1016], rdx
                                                                                        jmp   n521_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n516_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 2592], rax
                        mov              qword ptr [rbp + 2600], rdx
                                                                                        jmp   n523_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n517_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 2688], rax
                        mov              qword ptr [rbp + 2696], rdx
                                                                                        jmp   n524_var_α
#-----------------------------------------------------------------------------------------------------------------------
n518_assign_var_α:
                        mov              rdi, qword ptr [rbp + 800]
                        mov              rsi, qword ptr [rbp + 808]
                        mov              rdx, qword ptr [rbp + 816]
                        mov              rcx, qword ptr [rbp + 824]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n502_var_α
                        mov              qword ptr [rbp + 832], rax
                        mov              qword ptr [rbp + 840], rdx
                                                                                        jmp   n502_var_α
#-----------------------------------------------------------------------------------------------------------------------
n519_lit_integer_α:
                        mov              qword ptr [rbp + 896], 6
                        mov              rax, qword ptr [rip + .Lx730_0]
                        mov              qword ptr [rbp + 904], rax
                                                                                        jmp   n525_assign_var_α
.Lx730_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n520_subscript_α:
                        mov              rdi, qword ptr [rbp + 928]
                        mov              rsi, qword ptr [rbp + 936]
                        mov              rdx, qword ptr [rbp + 944]
                        mov              rcx, qword ptr [rbp + 952]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n515_var_α
                        mov              qword ptr [rbp + 960], rax
                        mov              qword ptr [rbp + 968], rdx
                                                                                        jmp   n526_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n521_lit_integer_α:
                        mov              qword ptr [rbp + 1024], 6
                        mov              rax, qword ptr [rip + .Lx732_0]
                        mov              qword ptr [rbp + 1032], rax
                                                                                        jmp   n527_subscript_α
.Lx732_0:
                        .quad            12
#=======================================================================================================================
#         v<13> = 1
#-----------------------------------------------------------------------------------------------------------------------
n522_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 1088], rax
                        mov              qword ptr [rbp + 1096], rdx
                                                                                        jmp   n528_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n523_subscript_α:
                        mov              rdi, qword ptr [rbp + 2576]
                        mov              rsi, qword ptr [rbp + 2584]
                        mov              rdx, qword ptr [rbp + 2592]
                        mov              rcx, qword ptr [rbp + 2600]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n510_var_α
                        mov              qword ptr [rbp + 2608], rax
                        mov              qword ptr [rbp + 2616], rdx
                                                                                        jmp   n530_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n524_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 2704], rax
                        mov              qword ptr [rbp + 2712], rdx
                                                                                        jmp   n531_subscript_α
#-----------------------------------------------------------------------------------------------------------------------
n525_assign_var_α:
                        mov              rdi, qword ptr [rbp + 880]
                        mov              rsi, qword ptr [rbp + 888]
                        mov              rdx, qword ptr [rbp + 896]
                        mov              rcx, qword ptr [rbp + 904]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n508_var_α
                        mov              qword ptr [rbp + 912], rax
                        mov              qword ptr [rbp + 920], rdx
                                                                                        jmp   n508_var_α
#-----------------------------------------------------------------------------------------------------------------------
n526_lit_integer_α:
                        mov              qword ptr [rbp + 976], 6
                        mov              rax, qword ptr [rip + .Lx737_0]
                        mov              qword ptr [rbp + 984], rax
                                                                                        jmp   n532_assign_var_α
.Lx737_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n527_subscript_α:
                        mov              rdi, qword ptr [rbp + 1008]
                        mov              rsi, qword ptr [rbp + 1016]
                        mov              rdx, qword ptr [rbp + 1024]
                        mov              rcx, qword ptr [rbp + 1032]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n522_var_α
                        mov              qword ptr [rbp + 1040], rax
                        mov              qword ptr [rbp + 1048], rdx
                                                                                        jmp   n533_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n528_lit_integer_α:
                        mov              qword ptr [rbp + 1104], 6
                        mov              rax, qword ptr [rip + .Lx739_0]
                        mov              qword ptr [rbp + 1112], rax
                                                                                        jmp   n534_subscript_α
.Lx739_0:
                        .quad            13
#=======================================================================================================================
#         r = ARRAY(13)
#-----------------------------------------------------------------------------------------------------------------------
n529_lit_integer_α:
                        mov              qword ptr [rbp + 1232], 6
                        mov              rax, qword ptr [rip + .Lx740_0]
                        mov              qword ptr [rbp + 1240], rax
                                                                                        jmp   n535_call_α
.Lx740_0:
                        .quad            13
#-----------------------------------------------------------------------------------------------------------------------
n530_deref_α:
                        mov              rdi, qword ptr [rbp + 2608]
                        mov              rsi, qword ptr [rbp + 2616]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n510_var_α
                        mov              qword ptr [rbp + 2624], rax
                        mov              qword ptr [rbp + 2632], rdx
                                                                                        jmp   n536_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n531_subscript_α:
                        mov              rdi, qword ptr [rbp + 2688]
                        mov              rsi, qword ptr [rbp + 2696]
                        mov              rdx, qword ptr [rbp + 2704]
                        mov              rcx, qword ptr [rbp + 2712]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n423_var_α
                        mov              qword ptr [rbp + 2720], rax
                        mov              qword ptr [rbp + 2728], rdx
                                                                                        jmp   n537_deref_α
#-----------------------------------------------------------------------------------------------------------------------
n532_assign_var_α:
                        mov              rdi, qword ptr [rbp + 960]
                        mov              rsi, qword ptr [rbp + 968]
                        mov              rdx, qword ptr [rbp + 976]
                        mov              rcx, qword ptr [rbp + 984]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n515_var_α
                        mov              qword ptr [rbp + 992], rax
                        mov              qword ptr [rbp + 1000], rdx
                                                                                        jmp   n515_var_α
#-----------------------------------------------------------------------------------------------------------------------
n533_lit_integer_α:
                        mov              qword ptr [rbp + 1056], 6
                        mov              rax, qword ptr [rip + .Lx744_0]
                        mov              qword ptr [rbp + 1064], rax
                                                                                        jmp   n538_assign_var_α
.Lx744_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n534_subscript_α:
                        mov              rdi, qword ptr [rbp + 1088]
                        mov              rsi, qword ptr [rbp + 1096]
                        mov              rdx, qword ptr [rbp + 1104]
                        mov              rcx, qword ptr [rbp + 1112]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n529_lit_integer_α
                        mov              qword ptr [rbp + 1120], rax
                        mov              qword ptr [rbp + 1128], rdx
                                                                                        jmp   n539_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n535_call_α:
                        mov              rax, qword ptr [rbp + 1232]
                        mov              qword ptr [rbp + 1200], rax
                        mov              rax, qword ptr [rbp + 1240]
                        mov              qword ptr [rbp + 1208], rax
                        .section         .rodata
.Lrkfn747:              .string          "ARRAY"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn747]
                        lea              rsi, [rbp + 1200]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1184], rax
                        mov              qword ptr [rbp + 1192], rdx
                        cmp              eax, 99
                                                                                        je    n541_var_α
                                                                                        jmp   n540_assign_α
n535_call_β:
                                                                                        jmp   n541_var_α
#-----------------------------------------------------------------------------------------------------------------------
n536_binop_α:
                        mov              rdi, qword ptr [rbp + 2560]
                        mov              rsi, qword ptr [rbp + 2568]
                        mov              rdx, qword ptr [rbp + 2624]
                        mov              rcx, qword ptr [rbp + 2632]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 2544], rax
                        mov              qword ptr [rbp + 2552], rdx
                                                                                        jmp   n542_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n537_deref_α:
                        mov              rdi, qword ptr [rbp + 2720]
                        mov              rsi, qword ptr [rbp + 2728]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    n423_var_α
                        mov              qword ptr [rbp + 2736], rax
                        mov              qword ptr [rbp + 2744], rdx
                                                                                        jmp   n543_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n538_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1040]
                        mov              rsi, qword ptr [rbp + 1048]
                        mov              rdx, qword ptr [rbp + 1056]
                        mov              rcx, qword ptr [rbp + 1064]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n522_var_α
                        mov              qword ptr [rbp + 1072], rax
                        mov              qword ptr [rbp + 1080], rdx
                                                                                        jmp   n522_var_α
#-----------------------------------------------------------------------------------------------------------------------
n539_lit_integer_α:
                        mov              qword ptr [rbp + 1136], 6
                        mov              rax, qword ptr [rip + .Lx751_0]
                        mov              qword ptr [rbp + 1144], rax
                                                                                        jmp   n544_assign_var_α
.Lx751_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n540_assign_α:
                        mov              rax, qword ptr [rbp + 1184]
                        mov              rdx, qword ptr [rbp + 1192]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                        mov              qword ptr [rbp + 1168], rax
                        mov              qword ptr [rbp + 1176], rdx
                                                                                        jmp   n541_var_α
#=======================================================================================================================
#         r<1> = 'M'
#-----------------------------------------------------------------------------------------------------------------------
n541_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1248], rax
                        mov              qword ptr [rbp + 1256], rdx
                                                                                        jmp   n545_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n542_assign_α:
                        mov              rax, qword ptr [rbp + 2544]
                        mov              rdx, qword ptr [rbp + 2552]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        mov              qword ptr [rbp + 2528], rax
                        mov              qword ptr [rbp + 2536], rdx
                                                                                        jmp   n510_var_α
#-----------------------------------------------------------------------------------------------------------------------
n543_binop_α:
                        mov              eax, dword ptr [rbp + 2672]
                        cmp              eax, 100
                                                                                        je    .Lx755_0
                        mov              eax, dword ptr [rbp + 2736]
                        cmp              eax, 100
                                                                                        je    .Lx755_0
                        mov              eax, dword ptr [rbp + 2672]
                        cmp              eax, 6
                                                                                        jne   .Lx755_2
                        mov              eax, dword ptr [rbp + 2736]
                        cmp              eax, 6
                                                                                        jne   .Lx755_2
.Lx755_1:
                        mov              rax, qword ptr [rbp + 2680]
                        mov              rcx, qword ptr [rbp + 2744]
                        sub              rax, rcx
                        mov              qword ptr [rbp + 2656], 6
                        mov              qword ptr [rbp + 2664], rax
                                                                                        jmp   n547_assign_α
.Lx755_0:
                        mov              rdi, qword ptr [rbp + 2672]
                        mov              rsi, qword ptr [rbp + 2680]
                        mov              rdx, qword ptr [rbp + 2736]
                        mov              rcx, qword ptr [rbp + 2744]
                        mov              r8d, 1
                        lea              r9, [rbp + 2656]
                        call             rt_binop_overload@PLT
                        test             eax, eax
                                                                                        jne   n547_assign_α
.Lx755_2:
                        mov              rdi, qword ptr [rbp + 2672]
                        mov              rsi, qword ptr [rbp + 2680]
                        mov              rdx, qword ptr [rbp + 2736]
                        mov              rcx, qword ptr [rbp + 2744]
                        mov              r8d, 1
                        call             rt_num_arith@PLT
                        cmp              eax, 99
                                                                                        je    n423_var_α
                        mov              qword ptr [rbp + 2656], rax
                        mov              qword ptr [rbp + 2664], rdx
                                                                                        jmp   n547_assign_α
n543_binop_β:
                                                                                        jmp   n423_var_α
#-----------------------------------------------------------------------------------------------------------------------
n544_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1120]
                        mov              rsi, qword ptr [rbp + 1128]
                        mov              rdx, qword ptr [rbp + 1136]
                        mov              rcx, qword ptr [rbp + 1144]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n529_lit_integer_α
                        mov              qword ptr [rbp + 1152], rax
                        mov              qword ptr [rbp + 1160], rdx
                                                                                        jmp   n529_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n545_lit_integer_α:
                        mov              qword ptr [rbp + 1264], 6
                        mov              rax, qword ptr [rip + .Lx757_0]
                        mov              qword ptr [rbp + 1272], rax
                                                                                        jmp   n548_subscript_α
.Lx757_0:
                        .quad            1
#=======================================================================================================================
#         r<2> = 'CM'
#-----------------------------------------------------------------------------------------------------------------------
n546_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1328], rax
                        mov              qword ptr [rbp + 1336], rdx
                                                                                        jmp   n549_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n547_assign_α:
                        mov              rax, qword ptr [rbp + 2656]
                        mov              rdx, qword ptr [rbp + 2664]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 2640], rax
                        mov              qword ptr [rbp + 2648], rdx
                                                                                        jmp   n423_var_α
#-----------------------------------------------------------------------------------------------------------------------
n548_subscript_α:
                        mov              rdi, qword ptr [rbp + 1248]
                        mov              rsi, qword ptr [rbp + 1256]
                        mov              rdx, qword ptr [rbp + 1264]
                        mov              rcx, qword ptr [rbp + 1272]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n546_var_α
                        mov              qword ptr [rbp + 1280], rax
                        mov              qword ptr [rbp + 1288], rdx
                                                                                        jmp   n551_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n549_lit_integer_α:
                        mov              qword ptr [rbp + 1344], 6
                        mov              rax, qword ptr [rip + .Lx761_0]
                        mov              qword ptr [rbp + 1352], rax
                                                                                        jmp   n552_subscript_α
.Lx761_0:
                        .quad            2
#=======================================================================================================================
#         r<3> = 'D'
#-----------------------------------------------------------------------------------------------------------------------
n550_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1408], rax
                        mov              qword ptr [rbp + 1416], rdx
                                                                                        jmp   n553_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n551_lit_string_α:
                        mov              qword ptr [rbp + 1296], 1
                        mov              rax, qword ptr [rip + .Lx763_0]
                        mov              qword ptr [rbp + 1304], rax
                                                                                        jmp   n555_assign_var_α
.Lx763_0:
                        .quad            .Lx763_0_s
.Lx763_0_s:
                        .string          "M"
#-----------------------------------------------------------------------------------------------------------------------
n552_subscript_α:
                        mov              rdi, qword ptr [rbp + 1328]
                        mov              rsi, qword ptr [rbp + 1336]
                        mov              rdx, qword ptr [rbp + 1344]
                        mov              rcx, qword ptr [rbp + 1352]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n550_var_α
                        mov              qword ptr [rbp + 1360], rax
                        mov              qword ptr [rbp + 1368], rdx
                                                                                        jmp   n556_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n553_lit_integer_α:
                        mov              qword ptr [rbp + 1424], 6
                        mov              rax, qword ptr [rip + .Lx765_0]
                        mov              qword ptr [rbp + 1432], rax
                                                                                        jmp   n557_subscript_α
.Lx765_0:
                        .quad            3
#=======================================================================================================================
#         r<4> = 'CD'
#-----------------------------------------------------------------------------------------------------------------------
n554_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1488], rax
                        mov              qword ptr [rbp + 1496], rdx
                                                                                        jmp   n558_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n555_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1280]
                        mov              rsi, qword ptr [rbp + 1288]
                        mov              rdx, qword ptr [rbp + 1296]
                        mov              rcx, qword ptr [rbp + 1304]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n546_var_α
                        mov              qword ptr [rbp + 1312], rax
                        mov              qword ptr [rbp + 1320], rdx
                                                                                        jmp   n546_var_α
#-----------------------------------------------------------------------------------------------------------------------
n556_lit_string_α:
                        mov              qword ptr [rbp + 1376], 1
                        mov              rax, qword ptr [rip + .Lx768_0]
                        mov              qword ptr [rbp + 1384], rax
                                                                                        jmp   n560_assign_var_α
.Lx768_0:
                        .quad            .Lx768_0_s
.Lx768_0_s:
                        .string          "CM"
#-----------------------------------------------------------------------------------------------------------------------
n557_subscript_α:
                        mov              rdi, qword ptr [rbp + 1408]
                        mov              rsi, qword ptr [rbp + 1416]
                        mov              rdx, qword ptr [rbp + 1424]
                        mov              rcx, qword ptr [rbp + 1432]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n554_var_α
                        mov              qword ptr [rbp + 1440], rax
                        mov              qword ptr [rbp + 1448], rdx
                                                                                        jmp   n561_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n558_lit_integer_α:
                        mov              qword ptr [rbp + 1504], 6
                        mov              rax, qword ptr [rip + .Lx770_0]
                        mov              qword ptr [rbp + 1512], rax
                                                                                        jmp   n562_subscript_α
.Lx770_0:
                        .quad            4
#=======================================================================================================================
#         r<5> = 'C'
#-----------------------------------------------------------------------------------------------------------------------
n559_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1568], rax
                        mov              qword ptr [rbp + 1576], rdx
                                                                                        jmp   n563_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n560_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1360]
                        mov              rsi, qword ptr [rbp + 1368]
                        mov              rdx, qword ptr [rbp + 1376]
                        mov              rcx, qword ptr [rbp + 1384]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n550_var_α
                        mov              qword ptr [rbp + 1392], rax
                        mov              qword ptr [rbp + 1400], rdx
                                                                                        jmp   n550_var_α
#-----------------------------------------------------------------------------------------------------------------------
n561_lit_string_α:
                        mov              qword ptr [rbp + 1456], 1
                        mov              rax, qword ptr [rip + .Lx773_0]
                        mov              qword ptr [rbp + 1464], rax
                                                                                        jmp   n565_assign_var_α
.Lx773_0:
                        .quad            .Lx773_0_s
.Lx773_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n562_subscript_α:
                        mov              rdi, qword ptr [rbp + 1488]
                        mov              rsi, qword ptr [rbp + 1496]
                        mov              rdx, qword ptr [rbp + 1504]
                        mov              rcx, qword ptr [rbp + 1512]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n559_var_α
                        mov              qword ptr [rbp + 1520], rax
                        mov              qword ptr [rbp + 1528], rdx
                                                                                        jmp   n566_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n563_lit_integer_α:
                        mov              qword ptr [rbp + 1584], 6
                        mov              rax, qword ptr [rip + .Lx775_0]
                        mov              qword ptr [rbp + 1592], rax
                                                                                        jmp   n567_subscript_α
.Lx775_0:
                        .quad            5
#=======================================================================================================================
#         r<6> = 'XC'
#-----------------------------------------------------------------------------------------------------------------------
n564_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1648], rax
                        mov              qword ptr [rbp + 1656], rdx
                                                                                        jmp   n568_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n565_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1440]
                        mov              rsi, qword ptr [rbp + 1448]
                        mov              rdx, qword ptr [rbp + 1456]
                        mov              rcx, qword ptr [rbp + 1464]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n554_var_α
                        mov              qword ptr [rbp + 1472], rax
                        mov              qword ptr [rbp + 1480], rdx
                                                                                        jmp   n554_var_α
#-----------------------------------------------------------------------------------------------------------------------
n566_lit_string_α:
                        mov              qword ptr [rbp + 1536], 1
                        mov              rax, qword ptr [rip + .Lx778_0]
                        mov              qword ptr [rbp + 1544], rax
                                                                                        jmp   n570_assign_var_α
.Lx778_0:
                        .quad            .Lx778_0_s
.Lx778_0_s:
                        .string          "CD"
#-----------------------------------------------------------------------------------------------------------------------
n567_subscript_α:
                        mov              rdi, qword ptr [rbp + 1568]
                        mov              rsi, qword ptr [rbp + 1576]
                        mov              rdx, qword ptr [rbp + 1584]
                        mov              rcx, qword ptr [rbp + 1592]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n564_var_α
                        mov              qword ptr [rbp + 1600], rax
                        mov              qword ptr [rbp + 1608], rdx
                                                                                        jmp   n571_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n568_lit_integer_α:
                        mov              qword ptr [rbp + 1664], 6
                        mov              rax, qword ptr [rip + .Lx780_0]
                        mov              qword ptr [rbp + 1672], rax
                                                                                        jmp   n572_subscript_α
.Lx780_0:
                        .quad            6
#=======================================================================================================================
#         r<7> = 'L'
#-----------------------------------------------------------------------------------------------------------------------
n569_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1728], rax
                        mov              qword ptr [rbp + 1736], rdx
                                                                                        jmp   n573_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n570_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1520]
                        mov              rsi, qword ptr [rbp + 1528]
                        mov              rdx, qword ptr [rbp + 1536]
                        mov              rcx, qword ptr [rbp + 1544]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n559_var_α
                        mov              qword ptr [rbp + 1552], rax
                        mov              qword ptr [rbp + 1560], rdx
                                                                                        jmp   n559_var_α
#-----------------------------------------------------------------------------------------------------------------------
n571_lit_string_α:
                        mov              qword ptr [rbp + 1616], 1
                        mov              rax, qword ptr [rip + .Lx783_0]
                        mov              qword ptr [rbp + 1624], rax
                                                                                        jmp   n575_assign_var_α
.Lx783_0:
                        .quad            .Lx783_0_s
.Lx783_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n572_subscript_α:
                        mov              rdi, qword ptr [rbp + 1648]
                        mov              rsi, qword ptr [rbp + 1656]
                        mov              rdx, qword ptr [rbp + 1664]
                        mov              rcx, qword ptr [rbp + 1672]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n569_var_α
                        mov              qword ptr [rbp + 1680], rax
                        mov              qword ptr [rbp + 1688], rdx
                                                                                        jmp   n576_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n573_lit_integer_α:
                        mov              qword ptr [rbp + 1744], 6
                        mov              rax, qword ptr [rip + .Lx785_0]
                        mov              qword ptr [rbp + 1752], rax
                                                                                        jmp   n577_subscript_α
.Lx785_0:
                        .quad            7
#=======================================================================================================================
#         r<8> = 'XL'
#-----------------------------------------------------------------------------------------------------------------------
n574_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1808], rax
                        mov              qword ptr [rbp + 1816], rdx
                                                                                        jmp   n578_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n575_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1600]
                        mov              rsi, qword ptr [rbp + 1608]
                        mov              rdx, qword ptr [rbp + 1616]
                        mov              rcx, qword ptr [rbp + 1624]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n564_var_α
                        mov              qword ptr [rbp + 1632], rax
                        mov              qword ptr [rbp + 1640], rdx
                                                                                        jmp   n564_var_α
#-----------------------------------------------------------------------------------------------------------------------
n576_lit_string_α:
                        mov              qword ptr [rbp + 1696], 1
                        mov              rax, qword ptr [rip + .Lx788_0]
                        mov              qword ptr [rbp + 1704], rax
                                                                                        jmp   n580_assign_var_α
.Lx788_0:
                        .quad            .Lx788_0_s
.Lx788_0_s:
                        .string          "XC"
#-----------------------------------------------------------------------------------------------------------------------
n577_subscript_α:
                        mov              rdi, qword ptr [rbp + 1728]
                        mov              rsi, qword ptr [rbp + 1736]
                        mov              rdx, qword ptr [rbp + 1744]
                        mov              rcx, qword ptr [rbp + 1752]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n574_var_α
                        mov              qword ptr [rbp + 1760], rax
                        mov              qword ptr [rbp + 1768], rdx
                                                                                        jmp   n581_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n578_lit_integer_α:
                        mov              qword ptr [rbp + 1824], 6
                        mov              rax, qword ptr [rip + .Lx790_0]
                        mov              qword ptr [rbp + 1832], rax
                                                                                        jmp   n582_subscript_α
.Lx790_0:
                        .quad            8
#=======================================================================================================================
#         r<9> = 'X'
#-----------------------------------------------------------------------------------------------------------------------
n579_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1888], rax
                        mov              qword ptr [rbp + 1896], rdx
                                                                                        jmp   n583_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n580_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1680]
                        mov              rsi, qword ptr [rbp + 1688]
                        mov              rdx, qword ptr [rbp + 1696]
                        mov              rcx, qword ptr [rbp + 1704]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n569_var_α
                        mov              qword ptr [rbp + 1712], rax
                        mov              qword ptr [rbp + 1720], rdx
                                                                                        jmp   n569_var_α
#-----------------------------------------------------------------------------------------------------------------------
n581_lit_string_α:
                        mov              qword ptr [rbp + 1776], 1
                        mov              rax, qword ptr [rip + .Lx793_0]
                        mov              qword ptr [rbp + 1784], rax
                                                                                        jmp   n585_assign_var_α
.Lx793_0:
                        .quad            .Lx793_0_s
.Lx793_0_s:
                        .string          "L"
#-----------------------------------------------------------------------------------------------------------------------
n582_subscript_α:
                        mov              rdi, qword ptr [rbp + 1808]
                        mov              rsi, qword ptr [rbp + 1816]
                        mov              rdx, qword ptr [rbp + 1824]
                        mov              rcx, qword ptr [rbp + 1832]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n579_var_α
                        mov              qword ptr [rbp + 1840], rax
                        mov              qword ptr [rbp + 1848], rdx
                                                                                        jmp   n586_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n583_lit_integer_α:
                        mov              qword ptr [rbp + 1904], 6
                        mov              rax, qword ptr [rip + .Lx795_0]
                        mov              qword ptr [rbp + 1912], rax
                                                                                        jmp   n587_subscript_α
.Lx795_0:
                        .quad            9
#=======================================================================================================================
#         r<10> = 'IX'
#-----------------------------------------------------------------------------------------------------------------------
n584_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 1968], rax
                        mov              qword ptr [rbp + 1976], rdx
                                                                                        jmp   n588_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n585_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1760]
                        mov              rsi, qword ptr [rbp + 1768]
                        mov              rdx, qword ptr [rbp + 1776]
                        mov              rcx, qword ptr [rbp + 1784]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n574_var_α
                        mov              qword ptr [rbp + 1792], rax
                        mov              qword ptr [rbp + 1800], rdx
                                                                                        jmp   n574_var_α
#-----------------------------------------------------------------------------------------------------------------------
n586_lit_string_α:
                        mov              qword ptr [rbp + 1856], 1
                        mov              rax, qword ptr [rip + .Lx798_0]
                        mov              qword ptr [rbp + 1864], rax
                                                                                        jmp   n590_assign_var_α
.Lx798_0:
                        .quad            .Lx798_0_s
.Lx798_0_s:
                        .string          "XL"
#-----------------------------------------------------------------------------------------------------------------------
n587_subscript_α:
                        mov              rdi, qword ptr [rbp + 1888]
                        mov              rsi, qword ptr [rbp + 1896]
                        mov              rdx, qword ptr [rbp + 1904]
                        mov              rcx, qword ptr [rbp + 1912]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n584_var_α
                        mov              qword ptr [rbp + 1920], rax
                        mov              qword ptr [rbp + 1928], rdx
                                                                                        jmp   n591_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n588_lit_integer_α:
                        mov              qword ptr [rbp + 1984], 6
                        mov              rax, qword ptr [rip + .Lx800_0]
                        mov              qword ptr [rbp + 1992], rax
                                                                                        jmp   n592_subscript_α
.Lx800_0:
                        .quad            10
#=======================================================================================================================
#         r<11> = 'V'
#-----------------------------------------------------------------------------------------------------------------------
n589_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 2048], rax
                        mov              qword ptr [rbp + 2056], rdx
                                                                                        jmp   n593_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n590_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1840]
                        mov              rsi, qword ptr [rbp + 1848]
                        mov              rdx, qword ptr [rbp + 1856]
                        mov              rcx, qword ptr [rbp + 1864]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n579_var_α
                        mov              qword ptr [rbp + 1872], rax
                        mov              qword ptr [rbp + 1880], rdx
                                                                                        jmp   n579_var_α
#-----------------------------------------------------------------------------------------------------------------------
n591_lit_string_α:
                        mov              qword ptr [rbp + 1936], 1
                        mov              rax, qword ptr [rip + .Lx803_0]
                        mov              qword ptr [rbp + 1944], rax
                                                                                        jmp   n595_assign_var_α
.Lx803_0:
                        .quad            .Lx803_0_s
.Lx803_0_s:
                        .string          "X"
#-----------------------------------------------------------------------------------------------------------------------
n592_subscript_α:
                        mov              rdi, qword ptr [rbp + 1968]
                        mov              rsi, qword ptr [rbp + 1976]
                        mov              rdx, qword ptr [rbp + 1984]
                        mov              rcx, qword ptr [rbp + 1992]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n589_var_α
                        mov              qword ptr [rbp + 2000], rax
                        mov              qword ptr [rbp + 2008], rdx
                                                                                        jmp   n596_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n593_lit_integer_α:
                        mov              qword ptr [rbp + 2064], 6
                        mov              rax, qword ptr [rip + .Lx805_0]
                        mov              qword ptr [rbp + 2072], rax
                                                                                        jmp   n597_subscript_α
.Lx805_0:
                        .quad            11
#=======================================================================================================================
#         r<12> = 'IV'
#-----------------------------------------------------------------------------------------------------------------------
n594_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 2128], rax
                        mov              qword ptr [rbp + 2136], rdx
                                                                                        jmp   n598_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n595_assign_var_α:
                        mov              rdi, qword ptr [rbp + 1920]
                        mov              rsi, qword ptr [rbp + 1928]
                        mov              rdx, qword ptr [rbp + 1936]
                        mov              rcx, qword ptr [rbp + 1944]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n584_var_α
                        mov              qword ptr [rbp + 1952], rax
                        mov              qword ptr [rbp + 1960], rdx
                                                                                        jmp   n584_var_α
#-----------------------------------------------------------------------------------------------------------------------
n596_lit_string_α:
                        mov              qword ptr [rbp + 2016], 1
                        mov              rax, qword ptr [rip + .Lx808_0]
                        mov              qword ptr [rbp + 2024], rax
                                                                                        jmp   n600_assign_var_α
.Lx808_0:
                        .quad            .Lx808_0_s
.Lx808_0_s:
                        .string          "IX"
#-----------------------------------------------------------------------------------------------------------------------
n597_subscript_α:
                        mov              rdi, qword ptr [rbp + 2048]
                        mov              rsi, qword ptr [rbp + 2056]
                        mov              rdx, qword ptr [rbp + 2064]
                        mov              rcx, qword ptr [rbp + 2072]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n594_var_α
                        mov              qword ptr [rbp + 2080], rax
                        mov              qword ptr [rbp + 2088], rdx
                                                                                        jmp   n601_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n598_lit_integer_α:
                        mov              qword ptr [rbp + 2144], 6
                        mov              rax, qword ptr [rip + .Lx810_0]
                        mov              qword ptr [rbp + 2152], rax
                                                                                        jmp   n602_subscript_α
.Lx810_0:
                        .quad            12
#=======================================================================================================================
#         r<13> = 'I'
#-----------------------------------------------------------------------------------------------------------------------
n599_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rbp + 2208], rax
                        mov              qword ptr [rbp + 2216], rdx
                                                                                        jmp   n603_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n600_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2000]
                        mov              rsi, qword ptr [rbp + 2008]
                        mov              rdx, qword ptr [rbp + 2016]
                        mov              rcx, qword ptr [rbp + 2024]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n589_var_α
                        mov              qword ptr [rbp + 2032], rax
                        mov              qword ptr [rbp + 2040], rdx
                                                                                        jmp   n589_var_α
#-----------------------------------------------------------------------------------------------------------------------
n601_lit_string_α:
                        mov              qword ptr [rbp + 2096], 1
                        mov              rax, qword ptr [rip + .Lx813_0]
                        mov              qword ptr [rbp + 2104], rax
                                                                                        jmp   n605_assign_var_α
.Lx813_0:
                        .quad            .Lx813_0_s
.Lx813_0_s:
                        .string          "V"
#-----------------------------------------------------------------------------------------------------------------------
n602_subscript_α:
                        mov              rdi, qword ptr [rbp + 2128]
                        mov              rsi, qword ptr [rbp + 2136]
                        mov              rdx, qword ptr [rbp + 2144]
                        mov              rcx, qword ptr [rbp + 2152]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n599_var_α
                        mov              qword ptr [rbp + 2160], rax
                        mov              qword ptr [rbp + 2168], rdx
                                                                                        jmp   n606_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n603_lit_integer_α:
                        mov              qword ptr [rbp + 2224], 6
                        mov              rax, qword ptr [rip + .Lx815_0]
                        mov              qword ptr [rbp + 2232], rax
                                                                                        jmp   n607_subscript_α
.Lx815_0:
                        .quad            13
#=======================================================================================================================
#         i = 1
#-----------------------------------------------------------------------------------------------------------------------
n604_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx816_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n608_assign_α
.Lx816_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n605_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2080]
                        mov              rsi, qword ptr [rbp + 2088]
                        mov              rdx, qword ptr [rbp + 2096]
                        mov              rcx, qword ptr [rbp + 2104]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n594_var_α
                        mov              qword ptr [rbp + 2112], rax
                        mov              qword ptr [rbp + 2120], rdx
                                                                                        jmp   n594_var_α
#-----------------------------------------------------------------------------------------------------------------------
n606_lit_string_α:
                        mov              qword ptr [rbp + 2176], 1
                        mov              rax, qword ptr [rip + .Lx818_0]
                        mov              qword ptr [rbp + 2184], rax
                                                                                        jmp   n609_assign_var_α
.Lx818_0:
                        .quad            .Lx818_0_s
.Lx818_0_s:
                        .string          "IV"
#-----------------------------------------------------------------------------------------------------------------------
n607_subscript_α:
                        mov              rdi, qword ptr [rbp + 2208]
                        mov              rsi, qword ptr [rbp + 2216]
                        mov              rdx, qword ptr [rbp + 2224]
                        mov              rcx, qword ptr [rbp + 2232]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    n604_lit_integer_α
                        mov              qword ptr [rbp + 2240], rax
                        mov              qword ptr [rbp + 2248], rdx
                                                                                        jmp   n610_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n608_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                        mov              qword ptr [rbp + 2288], rax
                        mov              qword ptr [rbp + 2296], rdx
                                                                                        jmp   n423_var_α
#-----------------------------------------------------------------------------------------------------------------------
n609_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2160]
                        mov              rsi, qword ptr [rbp + 2168]
                        mov              rdx, qword ptr [rbp + 2176]
                        mov              rcx, qword ptr [rbp + 2184]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n599_var_α
                        mov              qword ptr [rbp + 2192], rax
                        mov              qword ptr [rbp + 2200], rdx
                                                                                        jmp   n599_var_α
#-----------------------------------------------------------------------------------------------------------------------
n610_lit_string_α:
                        mov              qword ptr [rbp + 2256], 1
                        mov              rax, qword ptr [rip + .Lx822_0]
                        mov              qword ptr [rbp + 2264], rax
                                                                                        jmp   n611_assign_var_α
.Lx822_0:
                        .quad            .Lx822_0_s
.Lx822_0_s:
                        .string          "I"
#-----------------------------------------------------------------------------------------------------------------------
n611_assign_var_α:
                        mov              rdi, qword ptr [rbp + 2240]
                        mov              rsi, qword ptr [rbp + 2248]
                        mov              rdx, qword ptr [rbp + 2256]
                        mov              rcx, qword ptr [rbp + 2264]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n604_lit_integer_α
                        mov              qword ptr [rbp + 2272], rax
                        mov              qword ptr [rbp + 2280], rdx
                                                                                        jmp   n604_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 3328]
                        add              rsp, 3336
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 3328]
                        add              rsp, 3336
                        ret
                        .section         .note.GNU-stack,"",@progbits
